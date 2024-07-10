#include "hyres_CSVio.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

std::vector<std::vector<double>> hyres::csvin::CSV_read(std::string filepath){
    std::vector<std::vector<double>> result;
    std::ifstream file(filepath);
    std::string line;
    int line_count = 0;

    if (!file.is_open()){
        std::out << "Error: Unable to open file:" << filepath << std::endl;
        return 1;
    }
    
    while(std::getline(file, line)){
        std::vector<std::string> row = hyres::csvin::split(line, ',');
        std::vector<double> temp;
        double data;
        for(size_t i=0; i<row.size(); i++){
            try{
                data = std::stod(row.at(i));
            }
            catch(const std::invalid_argument& e){
                data = 0;
            }
            temp.push_back(data);
        };
        result.push_back(temp);
        line_count += 1;
    };
    return result;
};

std::vector<std::vector<double>> hyres::csvin::CSV_datafield(std::vector<std::vector<double>> csvfile){
    std::vector<std::vector<double>> result;
    std::vector<double> row;

    for(size_t i=0; i<csvfile.size(); i++){
        if(i){
            csvfile[i].erase(csvfile[i].begin());
            result.push_back(csvfile[i]);
        }
    }
    return result;
};

std::vector<double> hyres::csvin::CSV_header(std::vector<std::vector<double>> csvfile){
    std::vector<double> result;

    result = csvfile[0];
    result.erase(result.begin());
    return result;
};

std::vector<double> hyres::csvin::CSV_index(std::vector<std::vector<double>> csvfile){
    std::vector<double> result;

    for(size_t i=0; i<csvfile.size(); i++){
        if(i!=0){
            result.push_back(csvfile[i][0]);
        }
    }
    return result;
};

std::vector<std::string> hyres::csvin::split(std::string &input, char delimiter){
    std::istringstream stream(input);
    std::string field;
    std::vector<std::string> result;

    while(getline(stream, field, delimiter)){
        result.push_back(field);
    }
    return result;
};

int hyres::csvout::CSV_out(std::vector<std::vector<double>> data, std::vector<std::string> header, std::string filepath){
    std::ofstream file(filepath);

    if(file){
        for(size_t i = 0; i < header.size(); i++){
            file << header[i] << ",";
        }
        file << std::endl;
        for(size_t i = 0; i < data.size(); i++){
            for(size_t j = 0; j < data[i].size(); j++){
                file << data[i][j] << ",";
            }
            file << std::endl;
        }
        return 0;
    }else{
        return 1;
    }
};
