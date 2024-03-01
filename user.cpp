#include "user.hpp"
	User::User()
    {

    }

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

    void    User::setMessage(int message)
    {
        _message = message;
    }


