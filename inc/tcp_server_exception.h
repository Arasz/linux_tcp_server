#ifndef TCP_SERVER_EXCEPTION_H_
#define TCP_SERVER_EXCEPTION_H_

#include <exception>
#include <string>

namespace mrobot
{
	class tcp_server_exception: public std::exception
	{
	public:
		tcp_server_exception(std::string message):_message(message){}
		const char* what() const throw() override { return _message.c_str(); }
	private:
	const std::string _message;
	};
}

#endif /* TCP_SERVER_EXCEPTION_H_ */
