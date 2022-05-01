#include <iostream>
#include <chrono>
#include <asio.hpp>

#include "ControlConnection.hpp"

namespace connection {

	ControlConnection::ControlConnection() : 
		ioc(),controlSocket(ioc) {}

	bool ControlConnection::isOpen() {
		return controlSocket.is_open();
	}

	void ControlConnection::closeConnection() {
		controlSocket.close();
	}

	void ControlConnection::establishConnection() {
		asio::error_code ec;

		asio::ip::tcp::endpoint endpoint(asio::ip::make_address("192.168.1.15", ec), 21);
		controlSocket.connect(endpoint, ec);

		if (ec) {std::cout << "failed to establish control connection" << std::endl;}

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);

		std::string serverResponse = readServerResponse();
		std::cout << serverResponse << std::endl;

	}

	std::string ControlConnection::sendCommand(std::string& command) {
		asio::error_code ec;

		asio::write(controlSocket, asio::buffer(command + "\r\n"), ec);

		if (ec){std::cout << "can not send command" << std::endl;}

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);

		std::string serverResponse = readServerResponse();

		return serverResponse;
	}

	std::string ControlConnection::readServerResponse() {
		asio::error_code ec;
		std::string output = "";

		size_t bytes = controlSocket.available();
		std::cout << "Bytes available: " << bytes << std::endl;

		if (bytes > 0) {

			asio::read_until(controlSocket, asio::dynamic_buffer(output), '\n', ec);

			if (ec) { std::cout << "reading from server error" << std::endl; }
		}

		return (output == "") ? "nothing to read" : output;
	}
}