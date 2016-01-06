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
#include <sys/poll.h>
#include <mutex>

namespace mrobot
{

class tcp_server
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


private:
	void create_socket();
	void bind_socket(int port);
	void listen_for_connection();
	void accept_connection();

	void read_data();

	void poll_client_socket();

	bool _are_poll_objects_initialized = false;
	const int _observed_sockets_count = 1; /// amount of sockets which are polled by poll()
	int _timeout = -1; /// time in ms after which poll function() terminates (if negative function never terminates)
	pollfd _ufds[1]; /// array of structs representing socket descriptor used in sockets polling

	bool _data_ready_event_subscirbed = false; /// indicates if data ready event is subscribed
	data_ready_event_handler _data_ready_handler; /// function object which holds data read event handler function
	std::thread _read_data_thread; /// thread in which we are polling socket for data

	bool _is_connected = false; /// indicates if client is connected to the server
	std::mutex _is_connected_mutex;

	const size_t _buffer_size = 255; /// communication buffer size
	char _buffer[255]; /// internal communication buffer
	std::vector<char> _output_data_buffer{static_cast<char>(255), 0}; ///

	int _listen_socket_fd = 0; /// server socket file descriptor
	int _port = 22222; /// port on which server listens

	sockaddr_in _client_addres; /// connected client address
	int _client_socket_fd = 0; /// socket used for communication which connected client

	//std::exception_ptr _accept_connection_thread_exception = nullptr;
	//std::exception_ptr _poll_client_socket_thread_exception = nullptr;

};

} /* namespace mrobot */

#endif /* TCPSERVER_H_ */
