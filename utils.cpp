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

	int ret = 0;

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
			sockaddr sdr;
			socklen_t sdr_len;

			int clientsockfd = accept(myserv.getServSock(), NULL, NULL);

			if (clientsockfd < 0)
			{
				std::cerr<<RED<<"Accept failed"<<std::endl;

				if(errno == EAGAIN || errno == EWOULDBLOCK)
				{
					std::cerr<<RED << "Would block, continue"<<errno<<std::endl;
					continue;
				}
				else
				{
					std::cout << "Error break." << errno << std::endl;
					exit(0);
				}
			}

			std::cout << "New Client sock: " << clientsockfd << ", at n = " << n << std::endl;

			int n = myserv.getList().size() + 1;

			fds[n].fd = clientsockfd;
			fds[n].events = POLLIN;
			fds[n].revents = 0;

			User new_user = User();

			new_user.setSocket(clientsockfd);

			myserv.getList().insert(std::pair<int, User>(clientsockfd, new_user));

			send(fds[n].fd,  "Welcome! Type the server password : \n",  37, 0);

		}
		for(int a = 1; a <= myserv.getList().size(); a++)
		{

			if(fds[a].revents && POLLIN)
			{
				char buffer[1024];

				memset(buffer, 0, sizeof(buffer));

				ret = recv(fds[a].fd, buffer, sizeof(buffer), 0);

				if(ret == -1) {
					std::cerr<<RED<<" error :"<<errno<<std::endl;
					
					if(errno == EAGAIN || errno == EWOULDBLOCK) {
						std::cerr<<RED<<"Would block , continue"<<errno<<std::endl;
						continue;
					}
			 		else {
						close(fds[a].fd);
						myserv.getList().erase(fds[a].fd);
						std::cout << errno << std::endl;
						return (0);
					}
				}

			std::cout<<GREEN<< "Received Buffer: '" << buffer << "'  from a=" << a << std::endl;
			User usr = myserv.getList()[fds[a].fd];

			parser(buffer, &usr, myserv);

			}
		}
	}
	return(0);
}


void	ft_update_list(Server myserv, std::string channel, int j, std::list<User> userlist)
{
	std::cout << "Channel found	!" << std::endl;
	std::string msg = ":" + myserv.getList()[j].getNickname() + "! JOIN " + channel + "\r\n";
	std::list<User>::iterator ite = userlist.begin();
	std::string users;

	while (ite != userlist.end())
	{
		users += (*ite).getNickname();
		users += " ";
		ite++;
	}
	ite = userlist.begin();
	std::string server = "IRCserv";
	Channel& ch = myserv.getChanFromName(channel);
	while (ite != userlist.end())
	{
		myserv.sendData((*ite).getSocket(), msg);
		ite++;
	}
}


void parser(std::string buffer, User *user, Server myserv)
{
	std::string tmp;
	std::vector<std::string> args;
	std::stringstream ss(buffer);
	
	int fd = user->getSocket();

	std::getline(ss, tmp, ' ');

	if(tmp == "NICKNAME")
	{
			if(user->getStatus() != "LOGGED")
			{
				send(fd, "Use the command PASSWORD before setting your nickname\n",  56, 0);
				return;
			}
			std::string nickname;
			ss>>nickname;
			user->setNickname(nickname);
	}
	else if(tmp == "PASSWORD")
	{
		std::string password;
		ss >>  password;
		user->setPassword(password);
		if(password == myserv.getPassword())
		{
			user->setStatus("LOGGED");
			send(fd, "Succesfully logged\n",  20, 0);
		}
		else
			send(fd, "WRONG PASSWORD\n", 16 ,  0 );
	}
	else if (tmp == "PRVMSG")
	{
		std::string command;
		std::string username;
		std::string sendernick = user->getNickname();
		int strsize;
		ss>>username;
		int tsock = myserv.getSocketUser(username);
		if(tsock == 0)
		{
			send(fd, "No such user found\n", 20, 0);
			return;
		}
		std::string message;
		std::string fmessage = "!PRVMSG by " + sendernick + " ";
		while(ss)
		{
			ss >>  message;
			fmessage += message + " ";
		}
		fmessage += '\n';
		strsize = fmessage.size();
		send(tsock, fmessage.c_str() , strsize, 0);
	}
	else if (tmp == "JOIN")
	{
		std::string chname;
		ss>>chname;
		
		std::list<Channel>::iterator it = myserv.getChannel().begin();
		for(; it != myserv.getChannel().end(); it++)
		{
				if(chname == (*it).getName())
					break;
				std::cout<< (*it).getName()<< std::endl; 
		}
		if(it == myserv.getChannel().end())
		{	
			Channel newchannel(chname);
			newchannel.setClient(myserv.getList()[fd]);
			//std::cout<<myserv.getList()[fd].getNickname()<<std::endl;
			myserv.getChannel().push_back(newchannel);
			std::string msg = "Welcome " + myserv.getList()[fd].getNickname() + " to "+ newchannel.getName() + " channel\n";
			//std::cout<<myserv.getList()[fd].getNickname()<<std::endl;
			myserv.ft_send_all_chan(myserv, newchannel, msg);
			std::cout<< "dioca"<< std::endl;
		}
		else
		{
			Channel &ch = myserv.getChanFromName(chname);
			(*it).setClient(myserv.getList()[fd]);
			ft_update_list(myserv, chname, fd, (*it).getListUsers());
			std::string msg = "Welcome " + myserv.getList()[fd].getNickname() + " to "+ ch.getName() + " channel\n";
			
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
	}
}
