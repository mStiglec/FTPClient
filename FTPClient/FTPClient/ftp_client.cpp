#include <string>
#include <vector>
#include <asio.hpp>
#include <iostream>
#include <fstream>

#include "ftp_client.hpp"
#include "DataConnection.hpp"
#include "ControlConnection.hpp"

namespace ftp_client{

    FtpClient::FtpClient() : dataConnection(),controlConnection(){}

    void FtpClient::executeCommand(std::string& command, std::vector<std::string>& arguments){
        if(command == "open"){
            open(arguments);
        }
        else if (command == "get") {
            get(arguments);
        }
        else if (command == "ls") {
            ls(arguments);
        }
        else {
            std::cout << "command is not recognizable" << std::endl;
        }
    }

    //open controlConnection on port 21
    void FtpClient::open(std::vector<std::string>& arguments) {
        this->controlConnection.establishConnection();

        std::string username,password;
        std::cout << "Enter username: ";
        getline(std::cin, username);

        std::cout << "Enter password: ";
        getline(std::cin, password);

        this->login(username,password);
    }

    void FtpClient::login(std::string& username, std::string& password) {

        if (!this->controlConnection.isOpen()) { std::cout << "control connection closed" << std::endl; }

        std::string serverResponse;

        std::string userCommand = "USER " + username;
        std::string passCommand = "PASS " + password;

        serverResponse = this->controlConnection.sendCommand(userCommand);
        std::cout << serverResponse << std::endl;
        
        serverResponse = this->controlConnection.sendCommand(passCommand);
        std::cout << serverResponse << std::endl;
    }

    void FtpClient::ls(std::vector<std::string>& arguments) {

        std::string serverResponse;

        if (!this->dataConnection.isOpen()) { 
            std::string epsvCommand = "EPSV";
            serverResponse = this->controlConnection.sendCommand(epsvCommand);
            std::cout << serverResponse << std::endl;
        }

        std::string portStr = serverResponse.substr(39, 5);

        this->dataConnection.establishConnection(portStr);

        std::string listCommand = "LIST";
        serverResponse =  this->controlConnection.sendCommand(listCommand);
        std::cout << serverResponse << std::endl;
        
        serverResponse = this->dataConnection.readServerResponse();
        std::cout << serverResponse << std::endl;

        this->dataConnection.closeConnection();

    }

    void FtpClient::get(std::vector<std::string>& arguments) {
        std::string remoteFile, localFile;

        if (arguments.empty()) {
            std::cout << "Enter remote file name: ";
            getline(std::cin,remoteFile);
            localFile = "D:/" + remoteFile;
        }
        else if (arguments.size() == 1) {
            remoteFile = arguments[0];
            localFile = "D:/" + remoteFile;
        }
        else if (arguments.size() == 2) {
            remoteFile = arguments[0];
            localFile = "D:/" + arguments[1];
        }

        std::cout << "remote: " << remoteFile << " " << "local: " << localFile << std::endl << std::endl;
        
        std::string serverResponse;

        if (!this->dataConnection.isOpen()) {
            std::string epsvCommand = "EPSV";
            serverResponse = this->controlConnection.sendCommand(epsvCommand);
            std::cout << serverResponse << std::endl;
        }

        std::string portStr = serverResponse.substr(39, 5);

        std::ofstream file(localFile, std::ios_base::binary);

        this->dataConnection.establishConnection(portStr);

        std::string listCommand = "RETR " + remoteFile;
        serverResponse = this->controlConnection.sendCommand(listCommand);
        std::cout << serverResponse << std::endl;

        this->dataConnection.readServerResponse(file);

        this->dataConnection.closeConnection();
    }

    

}