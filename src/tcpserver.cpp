/*
 * tcpserver.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: rafal
 */

#include "tcpserver.h"

namespace mrobot
{

tcp_server::tcp_server()
{
	// TODO Auto-generated constructor stub

}

tcp_server::~tcp_server()
{
	// TODO Auto-generated destructor stub
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
	_socket_file_descriptor = socket(PF_INET, SOCK_STREAM, 0); // | SOCK_NONBLOCK, 0);

	if(_socket_file_descriptor<0)
		throw tcp_server_exception{"Error when opening socket.\n"};
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

	// set address family for ipv4
	socket_addres.sin_family = AF_INET;
	// save port number in network byte order
	socket_addres.sin_port = htons(port);
	// set ip address of the host to the address of machine on which server is running
	socket_addres.sin_addr.s_addr = INADDR_ANY;

	int bind_ret_val = bind(_socket_file_descriptor, reinterpret_cast<sockaddr*>(&socket_addres), sizeof(socket_addres));

	if(bind_ret_val < 0 )
		throw tcp_server_exception{"Binding socket to address failed."};

}

/**
 * @brief Allow the process to listen on the socket for connections
 * @throws tcp_server_exception
 */
void tcp_server::listen_for_connection()
{
	if(_socket_file_descriptor <= 0)
		throw tcp_server_exception{"Socket isn't valid. Before listening create socket."};
	listen(_socket_file_descriptor, 5);
}

void tcp_server::send(char* buffer, int length)
{
}

/**
 * @brief Accept connection from client and creates new socket for communication
 * @throws tcp_server_exception
 */
void tcp_server::accept_connection()
{
	unsigned int client_address_size = sizeof(_client_addres);
	_client_socket_file_descriptor = accept(_socket_file_descriptor, reinterpret_cast<sockaddr*>(&_client_addres), &client_address_size);

	if(_client_socket_file_descriptor < 0)
		throw tcp_server_exception("Error when accepting connection.\n");

}

/**
 * @brief Read data from client and write to class buffer
 * @return read bytes count
 * @throws tcp_server_exception
 */
int tcp_server::read_data()
{
	int bytes_count = read(_client_socket_file_descriptor, _buffer, _buffer_size);
	if(bytes_count < 0)
		throw tcp_server_exception{"Error reading data from socket"};
	return bytes_count;
}

} /* namespace mrobot */
