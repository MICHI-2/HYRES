//////////////////////////////

//////////////////////////////

#include <iostream>
#include <chrono>

#include "component/hyres_solver.hpp"

//#include "component/hyres_CSVio.hpp"

int main(){
    std::cout << "hello, I am HYRES v0.1" << std::endl;
    auto start = std::chrono::system_clock::now();

    hyres::solver("./input.json");
    
    auto end = std::chrono::system_clock::now();
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << msec << " msec" << std::endl;

    return 0;
};