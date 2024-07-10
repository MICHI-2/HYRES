#ifndef _HYLES_SOLVER_
#define _HYLES_SOLVER_

#include <vector>
#include <string>

#include "../lib/json.hpp"
using json = nlohmann::json;

namespace hyres{

    class solver{

        public:
        solver(std::string config_json_file);

        std::vector<double> replace_json(json raw_json);
        std::vector<double> Pc_brute_force_method(std::vector<double> state, std::vector<double> setting);
        std::vector<double> pc_calculator(double pc_est, std::vector<double> state, std::vector<double> setting);
        double Pe_brute_force_method(std::vector<double> state, std::vector<double> setting);
        double epsilon_calculator(double pe_est, std::vector<double> state, std::vector<double> setting);
        

        std::vector<std::vector<double>> solver_space;

        std::vector<std::vector<double>> cstar;
        std::vector<double> cstar_header;
        std::vector<double> cstar_index;

        std::vector<std::vector<double>> gamma;
        std::vector<double> gamma_header;
        std::vector<double> gamma_index;
        
        private:
        double Bilinear_interpolation(double row_axis, double line_axis, std::vector<std::vector<double>> data, std::vector<double> header, std::vector<double> index);
        void progress(double t_est, size_t step);
        size_t return_ind(double value, std::vector<double> list);

    };

};

#endif