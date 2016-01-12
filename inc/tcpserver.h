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
#include <ifile_descriptor_owner.h>
#include <sys/poll.h>
#include <mutex>

namespace mrobot
{

class tcp_server: public ifile_descriptor_owner
{
	using data_ready_event_handler = std::function<void(tcp_server&,std::vector<char>&)>;

public:
	tcp_server();
	tcp_server(int port);
	virtual ~tcp_server();

	//void send_data(char* buffer, int length);
	void send_data(const std::vector<char>&buffer);

	bool is_connected() const;
	void reconnect();

	void subscribe_data_ready_event(const data_ready_event_handler& data_ready_handler);
	void unsubscribe_data_ready_event();

	virtual void process_data() override;
	virtual int get_file_descriptor() override;


private:
	void create_socket();
	void bind_socket(int port);
	void listen_for_connection();
	void accept_connection();

	void read_data();


	bool _is_data_ready_event_subscirbed = false; /// indicates if data ready event is subscribed
	data_ready_event_handler _data_ready_handler; /// function object which holds data read event handler function

	bool _is_connected = false; /// indicates if client is connected to the server

	const size_t _buffer_size = 255; /// communication buffer size
	char _buffer[255]; /// internal communication buffer
	std::vector<char> _output_data_buffer{static_cast<char>(255), 0}; ///

	int _listen_socket_fd = -3; /// server socket file descriptor
	int _port = 22222; /// port on which server listens

	sockaddr_in _client_addres; /// connected client address
	int _client_socket_fd = -3; /// socket used for communication which connected client

};

} /* namespace mrobot */

#endif /* TCPSERVER_H_ */
