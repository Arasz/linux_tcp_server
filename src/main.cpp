/*
 * main.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: rafal
 */

#include <poll_controler.h>
#include "tcpserver.h"
#include <vector>
#include <iostream>
#include <functional>
#include <atomic>
#include <signal.h>
#include <cctype>

using namespace std;

void echo(mrobot::tcp_server& server,vector<char>& data)
{
	if(server.is_connected())
	{
		server.send_data(data);
		string str_data;

		cout<<"Sent data: ";
		for(const char& c : data)
		{
			cout<<c;
			if(isalpha(c))
				str_data.push_back(c);
		}

		cout<<"Data size: "<<data.size()<<endl;
		cout<<"String size: "<<str_data.size()<<" String: "<<str_data;
		if(str_data.compare("exit") == 0)
			throw runtime_error{"Exit received"};

	}
}

void test_server_set_up()
{
	using namespace std;
	using namespace mrobot;
	bool was_connected = false;
	try
	{
		tcp_server server;
		poll_controler poller;

		function<void(mrobot::tcp_server&,vector<char>&)> data_ready_handler = echo;

		server.subscribe_data_ready_event(data_ready_handler);


		cout<< "Server object constructed\n";

		while(true)
		{
			if(server.is_connected()&&!was_connected)
			{
				poller.attach(&server);
				poller.start_polling();
				was_connected = true;
			}
			else if(was_connected&&!server.is_connected())
			{
				was_connected = false;
				cout<<endl;
				cout<<"Disconnected..."<<endl;
				poller.detach(&server);
				poller.stop_polling();
				server.reconnect();
				cout<<"Reconnecting..."<<endl;
			}
		}
	}
	catch(tcp_server_exception& ex)
	{
		cerr<<ex.what()<<endl;
	}

}

int main(int argc, char* argv[])
{
	test_server_set_up();
	return 0;
}


