#include "hyres_calculator.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------------
double hyres::calculator::tank_pressure(double Pt_ini, double Pt_end, double et_conbersion_time, double time){
    return (Pt_end-Pt_ini)/et_conbersion_time*time+Pt_ini;
};


//----------------------------------------------------------------------------------------------------------------------------------------------
double hyres::calculator::Fuel_mass_flow_rate(double length_f, double d_f, double rho_f, double r_dot){
    return length_f*hyres::calculator::pi*d_f*rho_f*r_dot;
};

double hyres::calculator::Oxidizer_mass_flow_rate(double C_D, double d_o, double rho_ox, double Pt, double Pc){
    return C_D*(hyres::calculator::pi*pow(d_o, 2)/4)*sqrt(2*rho_ox*(Pt-Pc));
};

double hyres::calculator::Fuel_consumption(double pre_m_f, double pre_m_dot_f, double ppre_m_dot_f, double dt){
    return pre_m_f+(pre_m_dot_f-ppre_m_dot_f)*dt/2;
};

double hyres::calculator::Oxidizer_consumption(double pre_m_ox, double pre_m_dot_ox, double ppre_m_dot_ox, double dt){
    return pre_m_ox+(pre_m_dot_ox-ppre_m_dot_ox)*dt/2;
};


//----------------------------------------------------------------------------------------------------------------------------------------------
double hyres::calculator::Port_diameter(double d_f_ini, double rho_f, double length_f, double m_f){
    return sqrt((4*m_f/(hyres::calculator::pi*rho_f*length_f))+pow(d_f_ini, 2));
};

double hyres::calculator::Oxidizer_mass_flux(double m_dot_ox, double d_f){
    return 4*m_dot_ox/(hyres::calculator::pi*pow(d_f, 2));
};

double hyres::calculator::Fuel_recession_velocity(double a, double G_ox, double n){
    return a*pow(G_ox, n);
};

//----------------------------------------------------------------------------------------------------------------------------------------------
double hyres::calculator::Oxidizer_fuel_ratio(double m_dot_ox, double m_dot_f){
    return m_dot_ox/m_dot_f;
};

double hyres::calculator::Throat_diameter(double pre_d_t, double nozzle_erosion, double dt){
    return pre_d_t+2*nozzle_erosion*dt;
};

double hyres::calculator::Thrust_coefficient(double gamma, double Pe, double Pc, double Pa, double d_t, double d_e){
    return sqrt(2*pow(gamma, 2)/(gamma-1)*pow(2/(gamma+1), (gamma+1)/(gamma-1))*(1-pow(Pe/Pc, (gamma-1)/gamma)))+(Pe-Pa)/Pc*d_e/d_t;
};

double hyres::calculator::Thrust(double C_F, double Pc, double d_t, double alpha){
    return (1+cos(alpha))/2*C_F*Pc*hyres::calculator::pi*pow(d_t, 2)/4;
};

double hyres::calculator::Total_impulse(double pre_I, double pre_th, double ppre_th, double dt){
    return pre_I+(pre_th-ppre_th)*dt/2;
};