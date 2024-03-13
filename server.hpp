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
#include <vector>
#include "channel.hpp"
#include <list>

class User;
class Channel;

class Server
{
    private:
        std::string _password;
        int _port;
        int _servSock;
        std::string  _nameServer;
        std::map<int, User>	_clientSock;
		std::list<Channel> _channels;

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
		int getSocketUser(std::string username);
		std::list<Channel> &getChannel();
		void ft_send_all_chan(Server myserv, Channel ch, std::string msg);
		void sendData(int sockfd, const std::string& data);
		Channel& getChanFromName(std::string name);


};

int		init(int argc, char **argv, Server &myserv);
int 	cycle(Server &myserv);
void 	parser(std::string, User *user, Server myserv, int fd);
void	ft_update_list(Server myserv, std::string channel, int j, std::list<User> userlist);

#endif
