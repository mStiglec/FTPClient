#ifndef DATA_CONNECTION_FTP_HPP
#define DATA_CONNECTION_FTP_HPP

#include <asio.hpp>
#include <string>

namespace connection {

	class DataConnection {

		private:
			asio::io_context ioc;
			asio::ip::tcp::socket dataSocket;

		public:
			DataConnection();
			bool isOpen();
			void closeConnection();
			void establishConnection(std::string);
			std::string readServerResponse();
			std::string sendCommand(std::string& command);
			void readServerResponse(std::ofstream& file);
	};

}


#endif // DATA_CONNECTION_FTP_HPP

