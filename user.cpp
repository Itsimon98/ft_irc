#include "user.hpp"

User::User(){
	this->_username = "";
	this->_realname = "";
	this->_password ="";
	this->_status = "UNLOGGED";
	this->status = "";
	this->_nickname = "";
	//this->setHostname("");
}

User::~User(){}

void    User::setSocket(int socketfd)
{
    this->clientsock = socketfd;
}

void    User::setRealname(std::string realname)
{
    this->_realname =realname;
}

void    User::setUsername(std::string username)
{
    this->_username = username;
}

void	User::setNickname(std::string nickname)
{
    this->_nickname = nickname;
}

std::string	User::getStatus()
{
    return (this->_status);
}

std::string	User::getNickname()
{
    return (this->_nickname);
}

void    User::setStatus(std::string status)
{
    this->_status = status;
}

void    User::setPassword(std::string password)
{
    this->_password = password;
}

int User::getSocket()
{
	return(this->clientsock);
}
