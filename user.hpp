#ifndef USER_HPP
#define USER_HPP
#include "server.hpp"

class User
{
	private:
	std::string _username;
	std::string _realname;
	std::string _password;
	std::string _status;
	std::string _nickname;
	int clientsock;
	std::string _buildcmd;

	public:
	User();
	~User();
	std::string getUsername();
	std::string getPassword();
	std::string getRealname();
	std::string	getStatus();
	std::string getNickname();
	int getSocket();
	void	setSocket(int sockfd);
	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setStatus(std::string status);
	void	setRealname(std::string realname);
	void	setPassword(std::string password);
	void setBuildcmd(std::string cmd);
	std::string getBuildcmd();
	void remBuildcmd();
};

#endif
