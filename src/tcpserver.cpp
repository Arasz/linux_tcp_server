/*
 * tcpserver.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: rafal
 */

#include "tcpserver.h"

namespace mrobot
{
/**
 * @brief Construct server object listening on default port
 */
tcp_server::tcp_server(): tcp_server(22222)
{

}

/**
 * @brief Construct server object listening on given port
 * @param port port on which server is listening for connection
 */
tcp_server::tcp_server(int port):_port(port)
{
	this->create_socket();
	this->bind_socket(_port);
	this->listen_for_connection();

	// wait for connection in another thread
	std::thread accept_connection(&tcp_server::accept_connection, this);
	// detach thread - it will run in background without synchronization with main thread
	accept_connection.detach();
}

tcp_server::~tcp_server()
{
	close(_listen_socket_fd);
	close(_client_socket_fd);
}

/**
 * @brief Create new TCP/IP server socket
 *
 * @throws tcp_server_exception
 */
void tcp_server::create_socket()
{
	// create ipv4 no blocking socket, with full duplex byte stream communication
	// protocol is chosen by OS
	_listen_socket_fd = socket(PF_INET, SOCK_STREAM, 0); // | SOCK_NONBLOCK, 0);

	if(_listen_socket_fd<0)
		throw tcp_server_exception{"Error when opening socket.", strerror(errno)};
}

/**
 * @brief Bind socket to machine address and given port
 * @param port Port on which server listens for connections
 * @throws tcp_server_exception
 */
void tcp_server::bind_socket(int port)
{
	sockaddr_in socket_addres;

	// set all values in buffer to a zero
	//bzero(reinterpret_cast<char*>(&socket_addres), sizeof(socket_addres));
	std::memset(reinterpret_cast<char*>(&socket_addres), 0, sizeof(socket_addres));

	// set address family for ipv4
	socket_addres.sin_family = AF_INET;
	// save port number in network byte order
	socket_addres.sin_port = htons(port);
	// set ip address of the host to the address of machine on which server is running
	socket_addres.sin_addr.s_addr = INADDR_ANY;

	int bind_ret_val = bind(_listen_socket_fd, reinterpret_cast<sockaddr*>(&socket_addres), sizeof(socket_addres));

	if(bind_ret_val < 0 )
		throw tcp_server_exception{"Binding socket to address failed.", strerror(errno)};

}

/**
 * @brief Allow the process to listen on the socket for connections
 * @throws tcp_server_exception
 */
void tcp_server::listen_for_connection()
{
	if(_listen_socket_fd <= 0)
		throw tcp_server_exception{"Socket isn't valid. Before listening create socket.", strerror(errno)};
	listen(_listen_socket_fd, 5);
}
/**
 * @brief Send data to connected client
 * @param buffer data buffer
 * @param length data buffer length in bytes
 * @throws tcp_server_exception
 */
void tcp_server::send_data(char* buffer, int length)
{
	// send can send less data than we want in nonblocking mode
	int byte_count = send(_client_socket_fd, buffer, length, 0);

	//TODO Change this behavior
	if(byte_count < length)
		std::cerr<<"Less data written than expected.";
	if(byte_count < 0)
		throw tcp_server_exception{"Error when sending data", strerror(errno)};
}

void tcp_server::send_data(std::vector<char>& buffer)
{
	int length = buffer.size();

	//
	char data[length];

	int i = 0;
	for(char& c:buffer)
	{
		data[i++] = c;
	}

	// send can send less data than we want in nonblocking mode
	int byte_count = send(_client_socket_fd, data, length, 0);

	//TODO Change this behavior
	if(byte_count < length)
		std::cerr<<"Less data written than expected.";
	if(byte_count < 0)
		throw tcp_server_exception{"Error when sending data", strerror(errno)};
}

/**
 * @brief Gets data from server buffer
 * @param buffer
 * @param length
 */
std::vector<char> tcp_server::get_data(int length)
{
	std::vector<char> data(_buffer, _buffer+length);
	return data;
}

/**
 * @brief Assigns event handler called on data ready event
 * @param data_ready_handler function object with event handler
 */
void tcp_server::subscribe_data_ready_event(
		std::function<void()>& data_ready_handler)
{
	if(_data_ready_event_subscirbed == false)
	{
		_data_ready_event_subscirbed = true;
		_data_ready_handler = data_ready_handler;
	}
}
/**
 * @brief Unsubscribe data ready event handler
 */
void tcp_server::unsubscribe_data_ready_event()
{
	if(_data_ready_event_subscirbed)
	{
		_data_ready_event_subscirbed = false;
	}
}

/**
 * @brief Accept connection from client and creates new socket for communication
 * @throws tcp_server_exception
 */
void tcp_server::accept_connection()
{
	unsigned int client_address_size = sizeof(_client_addres);
	_client_socket_fd = accept(_listen_socket_fd, reinterpret_cast<sockaddr*>(&_client_addres), &client_address_size);

	if(_client_socket_fd < 0)
		throw tcp_server_exception("Error when accepting connection.", strerror(errno));
	_is_connected = true;

}

/**
 * @brief Read data from client and write to class buffer
 * @return read bytes count
 * @throws tcp_server_exception
 */
int tcp_server::read_data()
{
	std::memset(_buffer, 0, _buffer_size);

	int bytes_count = recv(_client_socket_fd, _buffer, _buffer_size, 0);
	//TODO Think about what we can do if connection with client is broken
	if(bytes_count == 0)
	{
		std::cerr<<"Client was disconnected.\n";
		_is_connected = false;
	}
	if(bytes_count < 0)
		throw tcp_server_exception{"Error reading data from socket", strerror(errno)};
	return bytes_count;
}
/**
 * @brief Gets connection state
 * @return connection state
 */
bool tcp_server::is_connected()
{
	return _is_connected;
}

} /* namespace mrobot */


