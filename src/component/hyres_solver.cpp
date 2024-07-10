#include "hyres_solver.hpp"

#include <fstream>
#include <iostream>
#include <math.h>
#include <iomanip>

#include "hyres_CSVio.hpp"

hyres::solver::solver(std::string config_json_file){
    std::cout << "Read json file: " << config_json_file << std::endl;
    std::ifstream json_f(config_json_file);
    if (!json_f.is_open()){std::cout << "Error: Unable to open file: " << config_json_file << std::endl;}
    json j_data = json::parse(json_f);
    std::vector<double> setting = hyres::solver::replace_json(j_data);

    hyres::csvin CSVIN;
    hyres::csvout CSVOUT;

    std::cout << "Read csv file: " << j_data["inout file name"]["cstar csv file name"] << std::endl;
    std::vector<std::vector<double>> csv = CSVIN.CSV_read(j_data["inout file name"]["cstar csv file name"]);
    cstar = CSVIN.CSV_datafield(csv);
    cstar_header = CSVIN.CSV_header(csv);
    cstar_index = CSVIN.CSV_index(csv);

    std::cout << "Read csv file: " << j_data["inout file name"]["gamma csv file name"] << std::endl;
    csv = CSVIN.CSV_read(j_data["inout file name"]["gamma csv file name"]);
    gamma = CSVIN.CSV_datafield(csv);
    gamma_header = CSVIN.CSV_header(csv);
    gamma_index = CSVIN.CSV_index(csv);

    std::cout << "Setting file loading complete" << std::endl;
    double hz = 100;
    double ini_burn_time = 5.0;

    double t_est = ini_burn_time;
    double t_est_update = ini_burn_time;

    //set initial state
    size_t max_step = 100 * hz;
    hyres::solver::solver_space.resize(max_step, std::vector<double>(19, 0));

    do{t_est = t_est_update;
        for(size_t t_step=0; t_step<solver_space.size(); t_step+=1){
            solver_space[t_step][0] = static_cast<double>(t_step)/100;
            solver_space[t_step][1] = setting[0]+(setting[1]-setting[0])*static_cast<double>(t_step)/(t_est*hz);
            solver_space[t_step][13] = setting[14]+2*setting[17]*static_cast<double>(t_step)/hz;
            solver_space[t_step][16] = pow(setting[15]/solver_space[t_step][13],2);
        }

        for(size_t t_step=0; t_step<solver_space.size(); t_step+=1){
            /* pox < 0 */
            if(solver_space[t_step][1]<0){
                hyres::solver::progress(t_est, t_step);
                t_est_update = static_cast<double>(t_step)/hz;
                break;
            }
            switch(t_step){
                case 0:
                    break;
                case 1:
                    solver_space[t_step][4] = solver_space[t_step-1][6] * (solver_space[t_step][0] - solver_space[t_step-1][0]);
                    solver_space[t_step][5] = solver_space[t_step-1][7] * (solver_space[t_step][0] - solver_space[t_step-1][0]);
                    break;
                default:
                    solver_space[t_step][4] = solver_space[t_step-1][4] + (solver_space[t_step-2][6] + solver_space[t_step-1][6]) * (solver_space[t_step-1][0] - solver_space[t_step-2][0])/2;
                    solver_space[t_step][5] = solver_space[t_step-1][5] + (solver_space[t_step-2][7] + solver_space[t_step-1][7]) * (solver_space[t_step-1][0] - solver_space[t_step-2][0])/2;
                    break;
            }
            solver_space[t_step] = hyres::solver::Pc_brute_force_method(solver_space[t_step], setting);
            /* ox used */
            if(solver_space[t_step][4]>setting[2]*setting[3]){
                hyres::solver::progress(t_est, t_step);
                t_est_update = static_cast<double>(t_step)/hz;
                solver_space.resize(t_step+1);
                break;
            }
            if(t_step%100==0) hyres::solver::progress(t_est, t_step);
        }
        std::cout << "\n";
    }while(t_est_update!=t_est);

    for(size_t t_step=0; t_step<solver_space.size(); t_step+=1){
        solver_space[t_step][14] = hyres::solver::Bilinear_interpolation((solver_space[t_step][2]/1.0e+6), solver_space[t_step][8], gamma, gamma_header, gamma_index);
        solver_space[t_step][3] = hyres::solver::Pe_brute_force_method(solver_space[t_step], setting);
        solver_space[t_step][15] = sqrt((2*pow(solver_space[t_step][14],2)/(solver_space[t_step][14]-1))*pow(2/(solver_space[t_step][14]+1),(solver_space[t_step][14]+1)/(solver_space[t_step][14]-1))*(1-pow(solver_space[t_step][3]/solver_space[t_step][2],(solver_space[t_step][14]-1)/solver_space[t_step][14])))+((solver_space[t_step][3]-setting[18])*pow(setting[15],2))/(solver_space[t_step][2]*pow(solver_space[t_step][13],2));
        solver_space[t_step][17] = (1+cos(setting[16]))*solver_space[t_step][15]*solver_space[t_step][2]*(3.141592*pow(solver_space[t_step][13],2)/8);
        if(t_step!=0){
            solver_space[t_step][18] = solver_space[t_step-1][18]+(solver_space[t_step][17]+solver_space[t_step-1][17])*(solver_space[t_step][0]-solver_space[t_step-1][0])/2;
        }
    }

    std::vector<std::string> out_hed = {"time","tank pressure","chamber pressure","nozzle outlet pressure","oxidizer consumption","fuel consumption","oxidizer mass flow","fuel mass flow","O/F","oxidizer mass flow flux","port diameter","fuel recession velocity","experimental c star","throat diameter","gamma","thrust coefficient","nozzle area ratio","thrust","total impulse"};
    CSVOUT.CSV_out(solver_space, out_hed, "./output.csv");

};

std::vector<double> hyres::solver::replace_json(json raw_json){
    std::vector<double> result(19);
    
    result[0] = raw_json["oxidizer"]["Initial tank pressure [Nm^-2]"].get<double>();
    result[1] = raw_json["oxidizer"]["Final tank pressure [Nm^-2]"].get<double>();
    result[2] = raw_json["oxidizer"]["Oxidizer filling volume [m^3]"].get<double>();
    result[3] = raw_json["oxidizer"]["Oxidizer density [kgm^-3]"].get<double>();

    result[4] = raw_json["fuel"]["Fuel density [kgm^-3]"].get<double>();
    result[5] = raw_json["fuel"]["Fuel length [m]"].get<double>();
    result[6] = raw_json["fuel"]["Initial port diameter [m]"].get<double>();
    result[7] = raw_json["fuel"]["Fuel outer diameter [m]"].get<double>();
    result[8] = raw_json["fuel"]["Fuel port number [-]"].get<double>();

    result[9] = raw_json["Oxidizer flow characteristics"]["Orifice diameter [m]"].get<double>();
    result[10] = raw_json["Oxidizer flow characteristics"]["Flow coefficient [-]"].get<double>();

    result[11] = raw_json["Combustion characteristics"]["Oxidizer mass flux coefficient [m^3kg^-1]"].get<double>();
    result[12] = raw_json["Combustion characteristics"]["Oxidizer mass flux exponent [-]"].get<double>();
    result[13] = raw_json["Combustion characteristics"]["C-star efficiency [-]"].get<double>();

    result[14] = raw_json["Nozzle characteristics"]["Initial nozzle throat diameter [m]"].get<double>();
    result[15] = raw_json["Nozzle characteristics"]["Nozzle exit diameter [m]"].get<double>();
    result[16] = raw_json["Nozzle characteristics"]["Nozzle exit half angle [deg]"].get<double>()*3.141592/180.0;
    result[17] = raw_json["Nozzle characteristics"]["Nozzle erosion speed [ms^-1]"].get<double>();

    result[18] = raw_json["Environment"]["Back pressure [Nm^-2]"].get<double>();

    return result;
};

std::vector<double> hyres::solver::Pc_brute_force_method(std::vector<double> state, std::vector<double> setting){
    double ans_pc = 0.0;
    double pc_div_min = 10.0e+6;

    for(double pc_est=0.5e+6; pc_est<setting[0]; pc_est+=0.001e+6){
        if(state[1]<(pc_est+0.1e+6)){
            continue;
        }
        state = hyres::solver::pc_calculator(pc_est, state, setting);
        double pc_div = std::abs(pc_est-state[2]);
        if(pc_div<pc_div_min){
            ans_pc = pc_est;
            pc_div_min = pc_div;
        }
    }
    state = hyres::solver::pc_calculator(ans_pc, state, setting);
    return state;
};

std::vector<double> hyres::solver::pc_calculator(double pc_est, std::vector<double> state, std::vector<double> setting){
    state[6] = setting[10]*(3.141592/4.0*pow(setting[9],2))*sqrt(2*setting[3]*(state[1]-pc_est));
    state[10] = sqrt(4.0*state[5]/(3.141592*setting[4]*setting[5])+pow(setting[6],2));
    state[9] = 4.0*state[6]/(3.141592*pow(state[10],2));
    state[11] = setting[11]*pow(state[9],setting[12]);
    state[7] = setting[5]*3.141592*state[10]*setting[4]*state[11];
    state[8] = state[6]/state[7];
    state[12] = setting[13]*hyres::solver::Bilinear_interpolation((pc_est/1.0e+6), state[8], cstar, cstar_header, cstar_index);
    state[2] = 4.0*state[12]*(state[6]+state[7])/(3.141592*pow(state[13],2));

    return state;
};


double hyres::solver::Pe_brute_force_method(std::vector<double> state, std::vector<double> setting){
    double ans_pe = 0.0;
    double epsilon_div_min = 10.0e+6;

    for(double pe_est=0.001e+6; pe_est<1.0e+6; pe_est+=0.001e+6){
        if(state[2]<(pe_est+0.001e+6)){
            continue;
        }
        double epsilon_cal = hyres::solver::epsilon_calculator(pe_est, state, setting);
        double epsilon_div = std::abs(epsilon_cal-state[16]);
        if(epsilon_div_min>epsilon_div){
            ans_pe = pe_est;
            epsilon_div_min = epsilon_div;
        }
    }
    return ans_pe;
};

double hyres::solver::epsilon_calculator(double pe_est, std::vector<double> state, std::vector<double> setting){
    return pow(2/(state[14]+1),(1/(state[14]+1)))*pow(state[2]/pe_est,1/state[14])/sqrt(((state[14]+1)/(state[14]-1))*(1-pow(pe_est/state[2],(state[14]-1)/state[14])));
};

double hyres::solver::Bilinear_interpolation(double row_axis, double line_axis, std::vector<std::vector<double>> data, std::vector<double> header, std::vector<double> index){
    size_t head = hyres::solver::return_ind(row_axis, header);
    size_t ind = hyres::solver::return_ind(line_axis, index);

    double z_xy1 = data[ind][head]+(data[ind][head+1]-data[ind][head])*(row_axis-header[head])/(header[head+1]-header[head]);
    double z_xy2 = data[ind+1][head]+(data[ind+1][head+1]-data[ind+1][head])*(row_axis-header[head])/(header[head+1]-header[head]);
    double ans = z_xy1+(z_xy2-z_xy1)*(line_axis-index[ind])/(index[ind+1]-index[ind]);

    return ans;
};

size_t hyres::solver::return_ind(double value, std::vector<double> list){
    for(size_t i=0; i<list.size(); i+=1){
        if(value<list[i]){
            if(i==0) return 0;
            return i-1;
        }
    }
    return list.size()-2;
};

void hyres::solver::progress(double t_est, size_t step){
    int barWidth = 30;
    float ratio = float(step) / float(t_est*100);
    int pos = barWidth * ratio;

    std::cout << std::fixed << std::setprecision(2) << t_est << "  [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(ratio*100.0) << " %\r";
    std::cout.flush();
};