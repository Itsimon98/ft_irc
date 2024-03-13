#include "user.hpp"
User::User(){}
User::~User(){}

void    User::setSocket(int sockfd)
{
    clientsock = sockfd;
}

void    User::setRealname(std::string realname)
{
    _realname =realname;
}

void    User::setUsername(std::string username)
{
    _username = username;
}
void	User::setNickname(std::string nickname)
{
    _nickname = nickname;
}
std::string	User::getStatus()
{
    return (_status);
}
std::string	User::getNickname()
{
    return (_nickname);
}

void    User::setStatus(std::string status)
{
    _status = status;
}


void    User::setPassword(std::string password)
{
    _password = password;
}

int User::getSocket()
{
	return(clientsock);
}
