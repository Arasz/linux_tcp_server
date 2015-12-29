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
 * @brief Creates new, nonblocking TCP/IP server socket
 *
 * @throws tcp_server_exception
 */
void tcp_server::create_socket()
{
	// create ipv4 no blocking socket, with full duplex byte stream communication
	// protocol is chosen by OS
	_socket_file_descriptor = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

	if(_socket_file_descriptor<0)
	{
		std::string error_msg{"Error when opening socket.\n"};
		std::cerr<<error_msg;
		throw tcp_server_exception{error_msg};
	}
}

/**
 * @brief Bind socket to machine address and given port
 * @param port Port on which server listens for connections
 * @throws tcp_server_exception
 */
void tcp_server::bind_socket(int port)
{
	sockaddr_in socket_addres;

	// Set address family for ipv4
	socket_addres.sin_family = AF_INET;
	// Save port number in network byte order
	socket_addres.sin_port = htons(port);
	// Set ip address of the host to the address of machine on which server is running
	socket_addres.sin_addr.s_addr = INADDR_ANY;

	int bind_ret_val = bind(_socket_file_descriptor, reinterpret_cast<sockaddr*>(&socket_addres), sizeof(socket_addres));

	if(bind_ret_val < 0 )
	{
		std::string error_msg = "Binding socket to address failed.";
		std::cerr<<error_msg;
		throw tcp_server_exception{error_msg};
	}

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

/**
 * @brief Accept connection from client and creates new socket for communication
 */
void tcp_server::accept_connection()
{
}

} /* namespace mrobot */
