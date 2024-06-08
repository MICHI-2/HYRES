#ifndef _HYLES_UTILITY_
#define _HYLES_UTILITY_

#include <vector>

namespace hyres{
    
    class Engine{

        public:

        double tank_pressure;
        double final_tank_pressure;
        double fillng_volume;
        double oxidizer_density;

        double fuel_length;
        double port_diameter;
        double initial_port_diameter;
        double outor_diometer;
        double port_number;
        double fuel_density;

        double orifice_diameter;
        double flow_coefficient;

        double mass_flux_coefficient;
        double mass_flux_expponent;
        double c_star_efficiency;

        double throat;
        double erosion_speed;
        double exit_diamerer;
        double exit_half_angle;

        double back_pressure;

    };

    class input_csv{

        public:
        std::vector<std::vector<double>> data;
        std::vector<double> index;
        std::vector<double> header;
    };

    class hyres_util{

        private:
        const double pi = 3.14159265;

        public:
        double Bilinear_interpolation(double x1, double x2, double y1, double y2, double z1, double z2, double z3, double z4, double x, double y);

        std::vector<std::vector<double>> Chamber_pressure_estimate2calculate(
            double Pc_est, 
            std::vector<std::vector<double>> &set, 
            hyres::Engine* engin, 
            input_csv* c_cstar,
            input_csv* gammma,
            double dt
        );

        size_t nv_indx(double obj, std::vector<double> array);

        double extraction(double obj, double of, std::vector<std::vector<double>> data, std::vector<double> index, std::vector<double> header);

    };

};


#endif