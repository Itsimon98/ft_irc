#include "server.hpp"

int	init(int argc, char **argv, Server &myserv)
{
	 if(argc != 3)
    {
        std::cerr<<RED<<"TO RUN : ./ircserv <port> <password>"<<std::endl;
		return (1);
    }
	try
	{
		std::stoi (argv[1]);
		if(std::stoi (argv[1]) < 1024 || std::stoi (argv[1]) > 65535)
		{
			std::cerr<<RED<<"Insert a numeric value between 1024 and 65535"<<std::endl;
			return (1);
		}

	}
	catch(const std::exception& e)
		{
			std::cerr<<RED<< "Insert a numeric value" << std::endl;
		}
		myserv.setPort(std::stoi(argv[1]));
		myserv.setPassword(argv[2]);
		if(myserv.createSocket() == -1)
		{
			std::cerr<<RED<< "Error creating socket" << std::endl;
			return (1);
		}
		std::cout <<GREEN<< "Socket created!" << std::endl;
		if(listen(myserv.getServSock(), 10) == -1)
		{
			std::cerr<<RED<< "Socket not listening!" << std::endl;
			return (1);
		}
		std::cout <<GREEN<< "Socket listening!" << std::endl;
	return (0);
}

int cycle(Server &myserv)
{
	int flags = fcntl(myserv.getServSock(), F_GETFL, 0);
	fcntl(myserv.getServSock(), F_SETFL, flags | O_NONBLOCK);
	struct pollfd fds[500];
	fds[0].fd = myserv.getServSock();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	int ret =0;
	while(1)
	{
		ret = poll(fds, myserv.getList().size() + 1, -1);
		if(ret < 0)
		{
			std::cerr<<RED<<"Poll Error"<<std::endl;
			break;
		}

		if(fds[0].revents && POLLIN)
		{
			int clientsockfd = accept(myserv.getServSock(), NULL, NULL);
			if (clientsockfd < 0)
			{
				std::cerr<<RED<<"Accept failed"<<std::endl;
			}
			if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
				std::cerr<<RED<<"Would block"<<errno<<std::endl;
				continue;
			}
			else
				{
					std::cout << "Error break." << errno << std::endl;
					exit(0);
				}
		}
		
	}
}