#ifndef TCP_SERVER_EXCEPTION_H_
#define TCP_SERVER_EXCEPTION_H_

#include <exception>
#include <string>

namespace mrobot
{
	class tcp_server_exception: public std::exception
	{
	public:
		tcp_server_exception(std::string message, std::string error = "None"):_message(message),_error_description(error){}
		const char* what() const throw() override
	    {
			std::string result = ("Message: "+_message+"\n"+"Error description: "+_error_description+"\n");
			return result.c_str();
	    }
	private:
	const std::string _message;
	const std::string _error_description;
	};
}

#endif /* TCP_SERVER_EXCEPTION_H_ */
