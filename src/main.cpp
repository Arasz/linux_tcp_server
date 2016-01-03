/*
 * main.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: rafal
 */

#include  "tcpserver.h"
#include <vector>
#include <iostream>

void test_server_set_up()
{
	using namespace std;
	using namespace mrobot;
	try
	{
		tcp_server server;

		while(true)
		{
			if(server.is_connected())
			{
				int bytes_count = server.read_data();
				cout<<"Read bytes: "<<bytes_count<<"\n";
				auto data = server.get_data(bytes_count);
				std::cout<<"Input data: ";
				for(char& c: data)
				{
					std::cout<<c;
				}
				std::cout<<std::endl;
				server.send_data(data);
			}
			else
				return;
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


