#include "server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password) , _nameServer("MyIrc"){}
Server::~Server(){}

void Server::setPort(int port)
{
    _port = port;
}

int Server::getPort()
{
    return(_port);
}

std::string Server::getServerName()
{
    return(_nameServer);
}

void Server::setPassword(std::string password)
{
    _password = password;
}

std::string Server::getPassword()
{
    return(_password);
}
int Server::getServSock()
{
    return(_servSock);
}
std::map<int, User>	&Server::getList()
{
	return (_clientSock);
}

int Server::createSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr<<RED<<"Creation of the socket failed"<<std::endl;
        return(-1);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr<<RED<<"Failed to bind socket"<<std::endl;
        return(-1);
    }
    std::cout<<GREEN<<"Succcesfully created and bound the socket"<<std::endl;
    _servSock = sockfd;
    return(0);
}
