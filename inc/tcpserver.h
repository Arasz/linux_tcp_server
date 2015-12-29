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
#include <netinet/in.h>
#include <iostream>
#include <string>
#include "tcp_server_exception.h"

namespace mrobot
{

class tcp_server
{
public:
	tcp_server();
	virtual ~tcp_server();


private:
	void create_socket();
	void bind_socket(int port);
	void listen_for_connection();
	void accept_connection();

	int _socket_file_descriptor;
	int _port;
};

} /* namespace mrobot */

#endif /* TCPSERVER_H_ */
