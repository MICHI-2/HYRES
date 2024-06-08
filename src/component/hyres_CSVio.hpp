#ifndef _HYLES_CSVIO_
#define _HYLES_CSVIO_

#include <vector>
#include <string>

namespace hyres{

    class csvin{
        
        public:
        std::vector<std::vector<double>> CSV_read(std::string filepath);
        std::vector<std::vector<double>> CSV_datafield(std::vector<std::vector<double>> csvfile);
        std::vector<double> CSV_header(std::vector<std::vector<double>> csvfile);
        std::vector<double> CSV_index(std::vector<std::vector<double>> csvfile);
        std::vector<std::string> split(std::string &input, char delimiter);
        
    };

    class csvout{

        public:
        int CSV_out(std::vector<std::vector<double>> data, std::vector<std::string> header, std::string filepath);

    };

};

#endif