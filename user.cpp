#include "user.hpp"
User::User(){}
User::~User(){}

User::User(const User &src) {
	*this = src;
	return;
}

User	&User::operator=(const User &src) {
	if (this != &src) {

	}
	return (*this);
}

void    User::setSocket(int sockfd)
{
    this->clientsock = sockfd;
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

std::string User::getBuildcmd()
{
	return(this->_buildcmd);
}

void User::setBuildcmd(std::string s)
{
	this->_buildcmd = s;
}
