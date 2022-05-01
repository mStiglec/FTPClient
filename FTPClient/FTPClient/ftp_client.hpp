#ifndef FTP_CLIENT_HPP
#define FTP_CLIENT_HPP

#include <string>
#include <asio.hpp>
#include <vector>

#include "DataConnection.hpp"
#include "ControlConnection.hpp"

namespace ftp_client
{

    class FtpClient{
        private:
            connection::DataConnection dataConnection;
            connection::ControlConnection controlConnection;

        public:
            FtpClient();

            void executeCommand(std::string& command, std::vector<std::string>& arguments);
            void open(std::vector<std::string>& arguments);
            void login(std::string& username, std::string& password);
            void ls(std::vector<std::string>& arguments);
            void get(std::vector<std::string>& arguments);
    };

}


#endif //FTP_CLIENT_HPP