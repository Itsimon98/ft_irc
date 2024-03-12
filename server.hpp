#ifndef SERVER_HPP
#define SERVER_HPP
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <map>
#include "user.hpp"
#include <cstring>
#include <sstream>
class User;

class Server
{
    private:
        std::string _password;
        int _port;
        int _servSock;
        std::string  _nameServer;
        std::map<int, User>	_clientSock;
    public:
        Server(int port, std::string password);
        ~Server();
        int createSocket();
        int getPort();
        std::string getPassword();
        void setPassword(std::string password);
        void setPort(int port);
        int getServSock();
        std::string getServerName();
		std::map<int, User>& getList();

};
int	init(int argc, char **argv, Server &myserv);
int cycle(Server &myserv);
void parser(std::string, User *user, Server myserv, int fd);
#endif
