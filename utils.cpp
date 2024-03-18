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
	// struct pollfd fds[500];
	myserv.fds[0].fd = myserv.getServSock();
	myserv.fds[0].events = POLLIN;
	myserv.fds[0].revents = 0;
	int ret = 0;

	while(1)
	{
		ret = poll(myserv.fds, myserv.getList().size() + 1, -1);
		if(ret < 0)
		{
			std::cerr<<RED<<"Poll Error"<<std::endl;
			break;
		}

		if(myserv.fds[0].revents && POLLIN)
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
			myserv.fds[n].fd = clientsockfd;
			myserv.fds[n].events = POLLIN;
			myserv.fds[n].revents = 0;
			myserv.getList()[clientsockfd].setSocket(clientsockfd);
			myserv.getList()[clientsockfd].setStatus("");
			myserv.getList()[clientsockfd].setRealname("");
			myserv.getList()[clientsockfd].setUsername("");
			myserv.getList()[clientsockfd].setPassword("");
			myserv.getList()[clientsockfd].setNickname("");
			//myserv.getList()[clientsockfd].setHostname("");
			myserv.getList()[clientsockfd].setStatus("UNLOGGED");

			std::cout << "Accept creato!" << std::endl;
			myserv.sendData(myserv.fds[n].fd, "Type the server password : \n");
		}
		for(int a = 1; a <= myserv.getList().size(); a++)
		{
			if(myserv.fds[a].revents && POLLIN)
			{
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				ret = recv(myserv.fds[a].fd, buffer, sizeof(buffer), 0);
				if(ret == -1)
				{
					std::cerr<<RED<<" error :"<<errno<<std::endl;
					if(errno == EAGAIN || errno == EWOULDBLOCK)
					{
						std::cerr<<RED<<"Would block , continue"<<errno<<std::endl;
						continue;
					}
			 		else
					{
						close(myserv.fds[a].fd);
						myserv.getList().erase(myserv.fds[a].fd);
						std::cout << errno << std::endl;
						return (0);
					}
				}
				else if(ret == 0)
				{
					std::cout << "buffer at rec == 0: " << buffer << std::endl;
					close(myserv.fds[a].fd);
					myserv.getList().erase(myserv.fds[a].fd);
					continue;
				}

				int fd = myserv.fds[a].fd;

				std::map<int, User>::iterator user_iterator = myserv.getList().find(fd);

				if (user_iterator == myserv.getList().end())
				{
					std::cout << "strange, no user found for fd: " << fd << std::endl;
					continue;
				}

				std::cout<<GREEN<< \
				"Tot users: '" << myserv.getList().size() << \
				"' - user: '" << myserv.getList()[fd].getNickname() << \
				"' - status: '"<< myserv.getList()[fd].getStatus() << \
				"' - socket: '" << myserv.getList()[fd].getSocket() << \
				"' - previous_message: '" << myserv.getList()[fd].getBuildcmd() << \
				"' - position: '" << a << "'\n" << \
				"------Buffer: '" << RED << buffer << GREEN << "'" << std::endl;

				parser(buffer, myserv, fd);
			}
		}
	}
	return(0);
}


void parser(std::string buffer, Server &myserv, int fd)
{
	std::stringstream ss(buffer);
	std::string tmp;
	std::getline(ss, tmp, ' ');

	tmp = myserv.getList()[fd].getBuildcmd() + tmp;

	bool is_partial_command = false;

	if(tmp == "NICKNAME")
		eval_nickname(ss, myserv, fd);
	else if(tmp == "PASSWORD")
		eval_password(ss, myserv, fd);
	else if (tmp == "PRVMSG")
		eval_message(ss, myserv, fd);
	else if (tmp == "JOIN")
		eval_join(ss, myserv, fd);
	else if(tmp == "INVITE")
		eval_invite(ss, myserv, fd);
	else if(tmp == "KICK")
		eval_kick(ss, myserv, fd);
	else if(tmp == "MODE")
		eval_mode(ss, myserv, fd);
	else
		is_partial_command = true;


	if (is_partial_command && myserv.getList()[fd].getBuildcmd().back() != '\n')
	{
		std::cout << "Setting build-cmd to : '" << tmp << "'" << std::endl;
		myserv.getList()[fd].setBuildcmd(tmp);
	}
	else if (is_partial_command && myserv.getList()[fd].getBuildcmd().back() == '\n')
	{
		myserv.sendData(fd, "You sent and invalid command!\n");
	}
	else
	{
		std::cout << "Erasing build-cmd" << std::endl;
		myserv.getList()[fd].setBuildcmd("");
	}
}


void eval_password(std::stringstream &ss, Server &myserv, int fd)
{
	std::string password;
	ss >> password;

	std::cout<< "typed password" << password << std::endl;
	myserv.getList()[fd].setPassword(password);

	if(password == myserv.getPassword())
	{
		myserv.getList()[fd].setStatus("LOGGED");
		myserv.sendData(fd, "Succesfully logged\n");
	}
	else
	{
		myserv.sendData(fd, "WRONG PASSWORD\n");
		close(fd);
		myserv.getList().erase(fd);
	}
}


void eval_nickname(std::stringstream &ss, Server &myserv, int fd)
{
	std::string nickname;
	ss >> nickname;
	std::cout<<"entering NICKNAME: "<< nickname << std::endl;

	if(myserv.getList()[fd].getStatus() == "UNLOGGED")
	{
		myserv.sendData(fd, "Use the command PASSWORD before setting your nickname\n");
		return;
	}

	for (std::map<int, User>::iterator it = myserv.getList().begin(); it != myserv.getList().end(); it++)
	{
		User u = it->second;
		if (u.getNickname() == nickname)
		{
			myserv.sendData(fd, "Nickname is not available\n");
			return ;
		}
	}
	myserv.getList()[fd].setNickname(nickname);
	myserv.getList()[fd].setStatus("NICKNAME");
}

void eval_message(std::stringstream &ss, Server &myserv, int fd)
{
	if(myserv.getList()[fd].getStatus() != "NICKNAME")
	{
		myserv.sendData(fd, "Use the command NICKNAME\n");
		return;
	}

	std::string command;
	std::string destinatary;
	std::string sendernick = myserv.getList()[fd].getNickname();
	int strsize;

	ss>>destinatary;

	if(destinatary[0] == '#')
	{
		std::string message;
		std::string fmessage = "!CHANNMSG " + destinatary + " ";
		while(ss)
		{
			ss >>  message;
			fmessage += message + " ";
		}
		fmessage += '\n';
		myserv.sendChanMsg(destinatary, fmessage, myserv, myserv.getList()[fd]);
		return;
	}

	int tsock = myserv.getSocketUser(destinatary);

	if(tsock == 0)
	{
		myserv.sendData(fd, "No such user found\n");
		return;
	}
	std::string message;
	std::string fmessage = "!PRVMSG by " + sendernick + " ";
	while(ss)
	{
		ss >> message;
		fmessage += message + " ";
	}
	fmessage += '\n';
	strsize = fmessage.size();
	myserv.sendData(tsock, fmessage);
}



void	ft_update_list(Server myserv, std::string channel, int fd, std::list<User> userlist)
{
	std::cout << "Channel found	!" << std::endl;
	std::string msg = ":" + myserv.getList()[fd].getNickname() + "! JOIN " + channel + "\r\n";
	std::string users;
	std::list<User>::iterator ite = userlist.begin();
	while (ite != userlist.end())
	{
		users += ite->getNickname();
		users += " ";
		ite++;
	}
	ite = userlist.begin();
	std::string server = "IRCserv";
	Channel& ch = myserv.getChanFromName(channel);
	while (ite != userlist.end())
	{
		myserv.sendData(ite->getSocket(), msg);
		ite++;
	}
}


void eval_join(std::stringstream &ss, Server &myserv, int fd)
{
	if(myserv.getList()[fd].getStatus() != "NICKNAME")
	{
		myserv.sendData(fd, "Use the command NICKNAME\n");
		return;
	}

	std::string chname;
	ss>>chname;

	std::list<Channel>::iterator it = myserv.getChannel().begin();

	for(; it != myserv.getChannel().end(); it++)
	{
			if(chname == it->getName())
			break;
		std::cout<< it->getName()<< std::endl;
	}
	if(it == myserv.getChannel().end())
	{
		Channel newchannel(chname);
		newchannel.setClient(myserv.getList()[fd]);
		newchannel.setOper(myserv.getList()[fd]);

		myserv.getChannel().push_back(newchannel);
		std::string msg = "Welcome " + myserv.getList()[fd].getNickname() + " to "+ newchannel.getName() + " channel\n";

		myserv.ft_send_all_chan(myserv, newchannel, msg);
	}
	else
	{
		Channel &ch = myserv.getChanFromName(chname);
		// TODO: qui va controllato se il canale ha bisogno di password, oppure solo invito, oppure etc.etc...
		//
		if (ch.isInvOn() && !ch.isUserInvited(myserv.getList()[fd].getNickname()))
		{
			myserv.sendData(fd, "You need to be invited to join this channel!\r\n");
			return;
		}
		else if(ch.isPwOn() != 0 && !ch.isUserInvited(myserv.getList()[fd].getNickname()))
		{
			if(ss)
			{
				std::string pw_ins;
				ss >> pw_ins;
				if (pw_ins != ch.getPw())
				{
					myserv.sendData(fd, "You need to join with the correct channel password!\n");
					return;
				}
			}
			else
			{
				myserv.sendData(fd, "IRCserv You need to join with the channel password!\n");
				return ;
			}
		}

		if (ch.isLimitOn() && ch.getLimit() == ch.getListUsers().size())
		{
			myserv.sendData(fd, "Channel User limit reached!\n");
			return;
		}

		if (ch.isUserIn(myserv.getList()[fd].getNickname()))
		{
			myserv.sendData(fd, "You already joined this channel\n");
			return;
		}

		it->setClient(myserv.getList()[fd]);
		ft_update_list(myserv, chname, fd, it->getListUsers());

		std::string msg = "Welcome " + myserv.getList()[fd].getNickname() + " to "+ ch.getName() + " channel\n";
		myserv.ft_send_all_chan(myserv, ch, msg);
		std::string chmessage;

	}
}


void eval_invite(std::stringstream &ss, Server &myserv, int fd)
{
	if(myserv.getList()[fd].getStatus() != "NICKNAME")
	{
		myserv.sendData(fd, "Use the command NICKNAME\n");
		return;
	}

	std::string chname;
	std::string invnick;
	ss>>chname;
	ss>>invnick;

	if(!myserv.isChanReal(chname))
	{
		myserv.sendData(fd, "Channel not found!\n");
		return ;
	}

	Channel& ch = myserv.getChanFromName(chname);

	if(!ch.isUserIn(myserv.getList()[fd].getNickname()))
	{
		myserv.sendData(fd, "You are not in the channel, you can't invite\n");
		return;
	}

	User *invited = myserv.isUserReal(invnick);
	if(invited == NULL)
	{
		myserv.sendData(fd, "The user you are inviting doesn't exists\n");
		return;
	}
	std::map<int, User>::iterator finder = myserv.getList().find(invited->getSocket());
	ch.setInvited(finder->second);

	std::string msg = ":" + invnick + " INVITE " + myserv.getList()[fd].getNickname() + " " + chname + "\r\n";
	myserv.sendData(invited->getSocket(), msg);
}


void eval_kick(std::stringstream &ss, Server &myserv, int fd)
{
	if(myserv.getList()[fd].getStatus() != "NICKNAME")
	{
		myserv.sendData(fd, "Use the command NICKNAME\n");
		return;
	}

	std::string channel;
	ss >> channel;

	// User sender_user = myserv.getList().find(fd)->second;
	if(!myserv.isChanReal(channel))
	{
		std::cout << "Channel not found" << std::endl;
		myserv.sendData(fd, "MyIRC Channel not found!\r\n");
		return ;
	}

	Channel& ch = myserv.getChanFromName(channel);
	std::list<User>::iterator it = ch.getListUsers().begin();
	std::string nick;
	ss >> nick;

	if(!ch.isUserOper(myserv.getList()[fd].getNickname()))
	{
		myserv.sendData(fd, "You are not an operator\r\n");
		return;
	}
	else if (!ch.isUserIn(nick))
	{
		myserv.sendData(fd, "The user you want to kick is not in the channel\r\n");
		return;
	}
	else
	{
		for(; it != ch.getListUsers().end(); it++)
		{
			if (it->getNickname() == nick)
				break ;
		}
	}

	if (it != ch.getListUsers().end())
	{
		if(ch.isUserOper(nick))
			ch.removeOper(nick);
			
		ch.getListUsers().erase(it);
		myserv.sendData(myserv.getList()[fd].getSocket(), "You have been kicked\n");
		std::cout << "User " << it->getNickname() << " removed from the channel" << std::endl;
	}
}


void eval_mode(std::stringstream &ss, Server &myserv, int fd)
{
	std::string channel;
	std::string mode;
	std::string nick;

	ss >> channel;
	ss >> mode;
	ss >> nick;

	std::cout<< channel << mode << nick;

	if(!myserv.isChanReal(channel))
	{
		std::cout << "CCHannel not found!" << std::endl;
		myserv.sendData(fd, "MyIRC CHannel not found!\n");
		return;
	}
	
	Channel& ch = myserv.getChanFromName(channel);
	std::list<User>::iterator it = ch.getListUsers().begin();

	if (mode == "+o" || mode == "-o")
	{
		if(!ch.isUserIn(myserv.getList()[fd].getNickname()) || !ch.isUserOper(myserv.getList()[fd].getNickname()))
		{
			myserv.sendData(fd, "You are not operator\n");
			return;
		}
		else if (!ch.isUserIn(nick))
		{
			myserv.sendData(fd, "THe user  is not in this Channel, can't make him operator.\n");
			return;
		}
		else if (mode == "+o")
		{
			if(ch.isUserOper(nick))
			{
				myserv.sendData(fd, "MyIRC the user is not an operator!\n");
				return;
			}
			else
			{
				std::map<int, User>::iterator finder = myserv.getList().find(myserv.getUserSockFromNick(nick));
				ch.setOper(finder->second);
				std::string msg = ":"+myserv.getList()[fd].getNickname() + " MODE "  + channel + " +o " + nick + "\n";
				myserv.ft_send_all_chan(myserv, ch, msg);
				std::cout << "the user " << nick << " is now an operator!" << std::endl;
			}
		}
		else
		{
			if(ch.isUserOper(nick))
			{
				std::list<User>::iterator finder = ch.getOper().begin();
				for (; finder != ch.getOper().end(); finder++)
				{
					if (finder->getNickname() == nick)
					{
						ch.removeOper(nick);
						std::cout << "The user " << nick << "  remove from operators!" << std::endl;
						std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " -o " + nick + "\n";
						myserv.ft_send_all_chan(myserv, ch, msg);
						break ;
				}
			}
			}
			else
			{
				myserv.sendData(fd, "IRCserv L'utente è già un comune cittadino del canale!\n");
				return ;
			}
		}
	}

	if (ch.isUserOper(myserv.getList()[fd].getNickname()))
	{
		if (mode == "+k")
		{
			ch.setPw(nick); //password
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " +k " + nick + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
		else if (mode == "-k")
		{
			ch.removePw();
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " -k " + nick + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
		else if (mode == "+i")
		{
			ch.setInvOn();
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " +i " + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
		else if (mode == "-i")
		{
			ch.remInvOn();
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " -i " + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
		else if (mode == "+t")
		{
			ch.setTopicOn();
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " +t " + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
		else if (mode == "-t")
		{
			ch.removeTopicOn();
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " -t " + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
		else if (mode == "-l")
		{
			ch.removeLimitOn();
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " -l " + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
		else if (mode == "+l")
		{
			int limit = 500;
			if (nick != " ")
			{
				try
				{
					std::stoi(nick);
				}
				catch(const std::exception& e)
				{
					std::string msg = "MYIRC not a number\n";
					myserv.sendData(fd, msg);
					return ;
				}
			}
			limit = std::stoi(nick);
			if (limit < 1)
			{
				std::string msg = "MYIRC not a valid parameter!\n";
				myserv.sendData(fd, msg);
				return ;
			}
			std::cout << "limit: " << limit << std::endl;
			ch.setLimitOn(myserv, limit);
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " +l " + std::to_string(limit) + " " + "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
	}
	else
	{
		myserv.sendData(fd, "You are not an operator, you can't use MODE command!\n");
	}
	return ;
}
