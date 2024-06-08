#include "hyres_utility.hpp"
#include "hyres_calculator.hpp"

double hyres::hyres_util::Bilinear_interpolation(double x1, double x2, double y1, double y2, double z1, double z2, double z3, double z4, double x, double y){
    double z_xy1 = z1+(z2-z1)*(x-x1)/(x2-x1);
    double z_xy2 = z4+(z3-z4)*(x-x1)/(x2-x1);
    return z_xy1+(z_xy2-z_xy1)*(y-y1)/(y2-y1);
};

std::vector<std::vector<double>> hyres::hyres_util::Chamber_pressure_estimate2calculate(
    double Pc_est, 
    std::vector<std::vector<double>> &set, 
    hyres::Engine* engin,
    input_csv* c_cstar,
    input_csv* gammma, 
    double dt){
    hyres::calculator cal_Pc;
    // 124 15 16 17 18 19 20
    set[2][3] = Pc_est;
    set[2][6] = cal_Pc.Oxidizer_mass_flow_rate(engin->flow_coefficient, engin->orifice_diameter, engin->oxidizer_density, set[2][1], set[2][3]);
    set[2][5] = cal_Pc.Fuel_consumption(set[1][5], set[1][7], set[0][7], dt);
    set[2][10] = cal_Pc.Port_diameter(engin->initial_port_diameter, engin->fuel_density, engin->fuel_length, set[2][5]);
    set[2][9] = cal_Pc.Oxidizer_mass_flux(set[2][6], set[2][10]);
    set[2][11] = cal_Pc.Fuel_recession_velocity(engin->mass_flux_coefficient, set[2][9], engin->mass_flux_expponent);
    set[2][7] = cal_Pc.Fuel_mass_flow_rate(engin->fuel_length, set[2][10], engin->fuel_density, set[2][11]);
    set[2][8] = cal_Pc.Oxidizer_fuel_ratio(set[2][6], set[2][7]);
    set[2][12] = engin->c_star_efficiency*extraction(set[2][3], set[2][8], c_cstar->data, c_cstar->index, c_cstar->header);
    set[2][13] = extraction(set[2][3], set[2][8], gammma->data, gammma->index, gammma->header);
    set[2][14] = cal_Pc.Throat_diameter(set[1][14], engin->erosion_speed, dt);
    set[2][19] = 4*set[2][12]*(set[2][6]+set[2][7])/(hyres::hyres_util::pi*pow(set[2][14], 2));
    return set;
};

size_t hyres::hyres_util::nv_indx(double obj, std::vector<double> array){
    if(obj<array[0]){
        return 0;
    }else if(obj>=array[array.size()-1]){
        return array[array.size()-2];
    }else{
        for(size_t i=0; i<array.size(); i++){
            if(array[i]>obj){
                return i-1;
            }
        }
    }
    return 0;
};

double hyres::hyres_util::extraction(double obj, double of, const std::vector<std::vector<double>> data, const std::vector<double> index, const std::vector<double> header){
    size_t ind = nv_indx(of, index);
    size_t head = nv_indx(obj, header);
    return Bilinear_interpolation(index[ind], index[ind+1], header[head], header[head+1], data[ind][head], data[ind+1][head], data[ind+1][head+1], data[ind][head+1], of, obj);
};
