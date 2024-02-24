#ifndef SERVER_HPP
#define SERVER_HPP
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
class Server
{
    private:
        std::string _password;
        int _port;
        int _servSock;
        std::string _nameServer = "MyIRC";
    public:
        Server(int port, std::string password);
        ~Server();
        int createSocket();
        int getPort();
        std::string getServerName();
};

#endif 