#include "utils.hpp"
#include <vector>
#include <string.h>
#include <iostream>
#include <sstream>

#define DELIMITER " "

namespace utils{

    std::vector<std::string> getCommandLine(std::string input){

        std::vector<std::string> arguments;
        std::stringstream ss(input);
        std::string word;

        while(ss >> word){
            arguments.push_back(word);
        }

        return arguments;
    }

}