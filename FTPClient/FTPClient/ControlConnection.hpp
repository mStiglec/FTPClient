#ifndef CONTROL_CONNECTION_FTP_HPP
#define CONTROL_CONNECTION_FTP_HPP

#include <asio.hpp>

namespace connection {

	class ControlConnection {

		private:
			asio::io_context ioc;
			asio::ip::tcp::socket controlSocket;

		public:
			ControlConnection();
			bool isOpen();
			void closeConnection();
			void establishConnection();
			std::string readServerResponse();
			std::string sendCommand(std::string& command);
	};

}


#endif // CONTROL_CONNECTION_FTP_HPP
