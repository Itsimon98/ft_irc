#ifndef USER_HPP
#define USER_HPP
#include "server.hpp"

class User
{
	private:
	std::string _username;
	std::string _realname;
	std::string _password;
	int _message;
	int clientsock;
	public:
	User();
	//~User();
	std::string getUsername();
	std::string getPassword();
	std::string getRealname();
	void	setSocket(int sockfd);
	void	setUsername(std::string username);
	void	setMessage(int message);
	void	setRealname(std::string realname);
	void	setPassword(std::string password);
};

#endif
