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

namespace mrobot
{

class tcp_server
{
public:
	tcp_server();
	virtual ~tcp_server();
	void send(char* buffer, int length);

private:
	void create_socket();
	void bind_socket(int port);
	void listen_for_connection();
	void accept_connection();
	int read_data();

	std::function<void()> on_data_ready;
	const size_t _buffer_size = 255;
	char* _buffer[255]; /// communication buffer
	int _socket_file_descriptor = 0; /// server socket file descriptor
	int _port = 22222; /// port on which server listens
	sockaddr_in _client_addres; /// connected client address
	int _client_socket_file_descriptor = 0; /// socket used for communication which connected client
};

} /* namespace mrobot */

#endif /* TCPSERVER_H_ */
