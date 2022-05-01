#include <iostream>
#include <chrono>
#include <string>
#include <asio.hpp>
#include <fstream>

#include "DataConnection.hpp"

namespace connection {

	DataConnection::DataConnection() :
		ioc(), dataSocket(ioc) {}

	bool DataConnection::isOpen() {
		return dataSocket.is_open();
	}

	void DataConnection::closeConnection() {
		dataSocket.close();
	}

	void DataConnection::establishConnection(std::string randomServerPort) {
		asio::error_code ec;

		int randomPort = std::stoi(randomServerPort);

		asio::ip::tcp::endpoint endpoint(asio::ip::make_address("192.168.1.15", ec), randomPort);
		dataSocket.connect(endpoint, ec);

		if (ec) { std::cout << "failed to establish data connection" << std::endl; }

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);

	}

	std::string DataConnection::sendCommand(std::string& command) {
		asio::error_code ec;

		asio::write(dataSocket, asio::buffer(command + "\r\n"), ec);

		if (ec) { std::cout << "can not send command" << std::endl; }

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);

		std::string serverResponse = readServerResponse();
		return serverResponse;
	}

	std::string DataConnection::readServerResponse() {
		asio::error_code ec;
		std::string output;

		size_t bytes = dataSocket.available();
		std::cout << "Bytes available: " << bytes << std::endl;

		if (bytes > 0) {

			asio::read_until(dataSocket, asio::dynamic_buffer(output), '\n', ec);

			if (ec) { std::cout << "reading from server error" << std::endl; }
		}
		return output;
	}

	void DataConnection::readServerResponse(std::ofstream& file) {
		asio::error_code ec;
		std::string output;

		size_t bytes = dataSocket.available();
		std::cout << "Bytes available: " << bytes << std::endl;

		std::array<char, 256> dataBuffer;

		if (bytes > 0) {

			int len;

			do {
				len = dataSocket.read_some(asio::buffer(dataBuffer), ec);

				file.write(dataBuffer.data(), len);

				if(file.fail()){ std::cout << "can not read in file" << std::endl; }

			} while (len > 0);
		}

	}
}