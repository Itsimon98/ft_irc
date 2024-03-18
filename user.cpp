#include "user.hpp"
User::User(){}
User::~User(){}

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
void User::setBuildcmd(std::string buildcmd)
{
	this->_buildcmd += buildcmd;
}

std::string User::getBuildcmd()
{
	return(this->_buildcmd);
}
void User::remBuildcmd()
{
	this->_buildcmd = "";
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
