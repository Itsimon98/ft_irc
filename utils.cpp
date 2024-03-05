#include "server.hpp"


int	init(int argc, char **argv, Server &myserv)
{
	 if(argc != 3)
    {
        std::cerr<<RED<<"To RUN : ./ircserv <port> <password>"<<std::endl;
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
	//int flags = fcntl(myserv.getServSock(), F_GETFL, 0);
	fcntl(myserv.getServSock(), F_SETFL, O_NONBLOCK);
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

				if(errno == EAGAIN || errno == EWOULDBLOCK)
				{
					std::cerr<<RED<<"Would block, continue"<<errno<<std::endl;
					continue;
				}
				else
				{
					std::cout << "Error break." << errno << std::endl;
					exit(0);
				}
			}
			std::map<int, User>::iterator it = myserv.getList().begin();
			myserv.getList().insert(std::pair<int, User>(clientsockfd, User()));
			std::cout<< "Client sock: " << clientsockfd << std::endl;

			int n = myserv.getList().size();
			fds[n].fd = clientsockfd;
			fds[n].events = POLLIN;
			fds[n].revents = 0;
			myserv.getList()[clientsockfd].setSocket(clientsockfd);	
			myserv.getList()[clientsockfd].setMessage(0);
			myserv.getList()[clientsockfd].setRealname("");
			myserv.getList()[clientsockfd].setUsername("");
			myserv.getList()[clientsockfd].setPassword("");
			
			std::cout << "Accept creato!" << std::endl;
		}
		for(int a = 1; a <= myserv.getList().size(); a++)
		{
			
			if(fds[a].revents && POLLIN)
			{
				
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				ret = recv(fds[a].fd, buffer, sizeof(buffer), 0);
				if(ret == -1)
				{
				std::cerr<<RED<<"Receive error :"<<errno<<std::endl;
					if(errno == EAGAIN || errno == EWOULDBLOCK)
					{
						std::cerr<<RED<<"Would block , continue"<<errno<<std::endl;
						continue;
					}
					
			 		else 
					{
						close(fds[a].fd);
						myserv.getList().erase(fds[a].fd);
						std::cout << errno << std::endl;
						return (0);
					}
				}
			std::cout<<GREEN<< buffer<< std::endl;
			}
		}	
		
			
		}
	return(0);
}
