#ifndef _HYLES_CALCULATOR_
#define _HYLES_CALCULATOR_

#include <math.h>

namespace hyres{

    class calculator{

        private:
        const double pi = 3.14159265;

        public:
        double tank_pressure(double Pt_ini, double Pt_end, double et_conbersion_time, double time);

        double Fuel_mass_flow_rate(double length_f, double d_f, double rho_f, double r_dot);
        double Oxidizer_mass_flow_rate(double C_D, double d_o, double rho_ox, double Pt, double Pc);
        double Fuel_consumption(double pre_m_f, double pre_m_dot_f, double ppre_m_dot_f, double dt);
        double Oxidizer_consumption(double pre_m_ox, double pre_m_dot_ox, double ppre_m_dot_ox, double dt);

        double Port_diameter(double d_f_ini, double rho_f, double length_f, double m_f);
        double Oxidizer_mass_flux(double m_dot_ox, double d_f);
        double Fuel_recession_velocity(double a, double G_ox, double n);

        double Oxidizer_fuel_ratio(double m_dot_ox, double m_dot_f);
        double Throat_diameter(double pre_d_t, double nozzle_erosion, double dt);
        double Thrust_coefficient(double gamma, double Pe, double Pc, double Pa, double d_t, double d_e);
        double Thrust(double C_F, double Pc, double d_t, double alpha);
        double Total_impulse(double pre_I, double pre_th, double ppre_th, double dt);

    };

};

#endif