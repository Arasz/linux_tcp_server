/*
 * tcpserver.h
 *
 *  Created on: Dec 29, 2015
 *      Author: rafal
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include "tcp_server_exception.h"
#include <functional>
#include <cstring>
#include <vector>
#include <array>
#include <thread>
#include <errno.h>

namespace mrobot
{

class tcp_server
{
public:
	tcp_server();
	tcp_server(int port);
	virtual ~tcp_server();
	void send_data(char* buffer, int length);
	void send_data(std::vector<char>&buffer);
	int read_data();
	std::vector<char> get_data(int length);
	bool is_connected();

private:
	void create_socket();
	void bind_socket(int port);
	void listen_for_connection();
	void accept_connection();

	std::thread _read_data_thread; /// thread in which we are polling socket for data
	bool _is_connected = false; /// indicates if client is connected to the server
	const size_t _buffer_size = 255; /// communication buffer size
	char _buffer[255]; /// communication buffer
	int _listen_socket_fd = 0; /// server socket file descriptor
	int _port = 22222; /// port on which server listens
	sockaddr_in _client_addres; /// connected client address
	int _client_socket_fd = 0; /// socket used for communication which connected client
};

} /* namespace mrobot */

#endif /* TCPSERVER_H_ */
