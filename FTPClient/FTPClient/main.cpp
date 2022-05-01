#include <iostream>
#include <string>
#include <vector>

#include <asio.hpp>
#include "utils.hpp"
#include "ftp_client.hpp"

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
//start of the program, catch input and execute commands
int main(int argc,char* argv[]){

    std::string input;
    std::vector<std::string> inputSplit;

    std::string command;
    std::vector<std::string> arguments;

    ftp_client::FtpClient ftpClient;

    for(;;){
        std::cout<<"ftp> ";
        getline(std::cin,input);

        std::cin.clear();

        inputSplit = utils::getCommandLine(input);
        
        if(inputSplit.size() == 0){
            std::cout<<"Incorrect Input"<<std::endl;
        }

        command = inputSplit[0];
        inputSplit.erase(inputSplit.begin());

        arguments.clear();
        for(std::string argument : inputSplit){
            arguments.push_back(argument);
        }

        ftpClient.executeCommand(command,arguments);

    }
}