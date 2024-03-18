#include "server.hpp"
void parser(std::string buffer, User &user, Server &myserv, int fd)
{
	std::string tmp;
	std::string tmp1;
	std::vector<std::string> args;
	std::stringstream ss(buffer);
	std::getline(ss, tmp, ' ');
	int cmdflag = 0;
													//---------------NICKNAME--------------//
	if(tmp == "NICKNAME"|| "NICKNAME" == myserv.getList()[fd].getBuildcmd())
	{
			myserv.getList()[fd].remBuildcmd();
			if(user.getStatus() == "UNLOGGED")
			{
				myserv.sendData(fd, "Use the command PASSWORD before setting your nickname\n");
				return;
			}
			std::string nickname;
			ss>>nickname;
			std::cout<<"entering NICKNAME: "<< nickname << std::endl;
			std::map<int, User>::iterator it = myserv.getList().begin();
			for (; it != myserv.getList().end(); it++)
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

			std::cout<< " size users :" << myserv.getList().size()<<std::endl;
			cmdflag = 1;
	}
															//---------------PASSWORD--------------//
	else if(tmp == "PASSWORD" || "PASSWORD" == myserv.getList()[fd].getBuildcmd())
	{
		myserv.getList()[fd].remBuildcmd();
		std::string password;
		ss >>  password;
		std::cout<< password<< std::endl;
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
			myserv.getList()[fd].remBuildcmd();
		}
		cmdflag = 1;
	}
																//---------------PRVMSG--------------//
	else if (tmp == "PRVMSG" || "PRVMSG" == myserv.getList()[fd].getBuildcmd())
		{
			myserv.getList()[fd].remBuildcmd();
			if(myserv.getList()[fd].getStatus() != "NICKNAME")
			{
				myserv.sendData(fd, "Use the command NICKNAME\n");
				return;
			}
			std::string command;
			std::string username;
			std::string sendernick = myserv.getList()[fd].getNickname();
			int strsize;
			ss>>username;
			int tsock = myserv.getSocketUser(username);
			if(username[0] == '#')
			{
				std::string message;
				std::string fmessage = "PRIVMSG " + username + ": ";
				while(ss >>  message)
				{
					fmessage += message + " ";
				}
				fmessage += '\n';
				myserv.sendChanMsg(username, fmessage, myserv, user);
				return;

			}
			if(tsock == 0)
			{
				myserv.sendData(fd, "No such user found\n");
				return;
			}
			std::string message;
			std::string fmessage = "PRVIMSG " + sendernick + ": ";
			while(ss >> message)
			{
				fmessage += message + " ";
			}
			fmessage += '\n';
			strsize = fmessage.size();
			send(tsock, fmessage.c_str() , strsize, 0);
			cmdflag = 1;
		}
																	//---------------JOIN--------------//
		else if (tmp == "JOIN"|| "JOIN" == myserv.getList()[fd].getBuildcmd())
		{
			myserv.getList()[fd].remBuildcmd();
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
				 if(chname == (*it).getName())
					 break;
					std::cout<< (*it).getName()<< std::endl;
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
			if (ch.isInvOn() && !ch.isUserInvited(myserv.getUsernameFromSock(fd)))
			{
				myserv.sendData(fd, "You need to be invited to join this channel!\r\n");
				return;
			}
			else if(ch.isPwOn() != 0 && !ch.isUserInvited(myserv.getUsernameFromSock(fd)))
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

				if (ch.isUserIn(user.getNickname()))
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
			cmdflag = 1;
		}
																	//---------------INVITE--------------//
		else if(tmp == "INVITE"|| "INVITE" == myserv.getList()[fd].getBuildcmd())
		{
			myserv.getList()[fd].remBuildcmd();
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

			if(!ch.isUserIn(myserv.getUsernameFromSock(fd)))
			{
				myserv.sendData(fd, "You are not in the channel, you can't invite\n");
				return;
			}
			if(!myserv.isUserReal(invnick))
			{
				myserv.sendData(fd, "The user you are inviting doesn't exists\n");
				return;
			}
			std::map<int, User>::iterator finder = myserv.getList().find(myserv.getUserSockFromNick(invnick));
			ch.setInvited(finder->second);
			std::string msg = ":" + invnick + " INVITE " + myserv.getList()[fd].getNickname() + " " + chname + "\r\n";
			myserv.getList()[fd].remBuildcmd();
			myserv.sendData(myserv.getUserSockFromNick(invnick), msg);
			cmdflag = 1;
		}
																		//---------------KICK--------------//
		else if(tmp == "KICK" ||"KICK" == myserv.getList()[fd].getBuildcmd())  //KICK command command
		{
			myserv.getList()[fd].remBuildcmd();
			if(myserv.getList()[fd].getStatus() != "NICKNAME")
			{
				myserv.sendData(fd, "Use the command NICKNAME\n");
				return;
			}
		std::string channel;
		ss >> channel;
		User sender_user = myserv.getList().find(fd)->second;
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
		if(!ch.isUserOper(myserv.getUsernameFromSock(fd)))
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
		std::string msg = "You have been kicked from #" + channel + '\n';
		myserv.sendData(myserv.getUserSockFromNick(nick), msg);
		std::cout << "User " << it->getNickname() << " removed from the channel" << std::endl;
		myserv.getList()[fd].remBuildcmd();
		}
		}
																//---------------USERNAME--------------//
		else if(tmp == "USERNAME"  || "USERNAME" == myserv.getList()[fd].getBuildcmd())
		{
			myserv.getList()[fd].remBuildcmd();	
			if(myserv.getList()[fd].getStatus() != "NICKNAME")
			{
				myserv.sendData(fd, "Use the command NICKNAME\n");
				return;
			}

		std::string username;
		ss>>username;
	
		std::cout << "User arrivato: " << username << std::endl;
		myserv.getList()[fd].setUsername(username);
		myserv.getList()[fd].setRealname(username);
		//std::cout << "User salvato: " << myserv.getList()[fd].getUsername() << std::endl;
		std::string welcome = "001 "+ myserv.getList()[fd].getNickname() + " :Welcome to the server MYIrc, " + myserv.getList()[fd].getNickname() + "\r\n";
		myserv.sendData(fd, welcome);
		welcome = "MYIrc This is NOT the global chat, to join a channel type /join <channel_name>!\n";
		myserv.sendData(fd, welcome);
			return ;
		}
															//---------------TOPIC--------------//
		else if (tmp == "TOPIC" || "TOPIC" == myserv.getList()[fd].getBuildcmd())
		{	
			myserv.getList()[fd].remBuildcmd();
			std::string channel;
			ss>>channel;
			std::string topic;
			ss>> topic;
			if(!myserv.isChanReal(channel))
			{
				myserv.sendData(fd, "Channel not found!");
				return;
			}
			Channel &ch = myserv.getChanFromName(channel);
			if(!ch.isUserOper(myserv.getUsernameFromSock(fd)) && ch.isTopicOn())
			{
				myserv.sendData(fd, "You are not an operator, you can´t change the topic");
				return;
			}
			std::string msg = channel + ": topic has beeen changed to : " + topic + '\n';
			myserv.ft_send_all_chan(myserv, ch, msg);
			

		}
																	//---------------MODE--------------//
		else if(tmp == "MODE" || "MODE" == myserv.getList()[fd].getBuildcmd()) 
		{
			myserv.getList()[fd].remBuildcmd();
			std::string channel;
			ss >> channel;
			std::string mode;
			ss >> mode;
			std::string nick;
			ss >> nick;
			std::cout<< channel << mode << nick;
			if(!myserv.isChanReal(channel))
			{
				std::cout << "CCHannel not found!" << std::endl;
				myserv.sendData(fd, "MyIRC CHannel not found!\n");
				return ;
			}
			Channel& ch = myserv.getChanFromName(channel);
			if (mode == "+o" || mode == "-o")
			{
				if(!ch.isUserIn(myserv.getUsernameFromSock(fd)) || !ch.isUserOper(myserv.getUsernameFromSock(fd)))
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
				return;
			}
		}
	}
	if (ch.isUserOper(myserv.getUsernameFromSock(fd)))
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
				int a = std::atoi(nick.c_str());
				if(isdigit(a))
				{
					std::string msg = "MYIRC not a number\n";
					myserv.sendData(fd, msg);
					return;
				}
			}
			limit = std::atoi(nick.c_str());
			if (limit < 1)
			{
				std::string msg = "MYIRC not a valid parameter!\n";
				myserv.sendData(fd, msg);
				return;
			}
			std::cout << "limit: " << limit << std::endl;
			ch.setLimitOn(myserv, limit);
			std::string msg = ":"+ myserv.getList()[fd].getNickname() + " MODE "  + channel + " +l "+ "\n";
			myserv.ft_send_all_chan(myserv, ch, msg);
		}
	}
	else
	{
		myserv.sendData(fd, "You are not an operator, you can't use MODE command!\n");
		myserv.getList()[fd].remBuildcmd();
	}
	cmdflag = 1;
	}
	int _size;
	_size = myserv.getList()[fd].getBuildcmd().size();
	if(myserv.getList()[fd].getBuildcmd()[_size] != '\n' && cmdflag == 0)
	 	myserv.getList()[fd].setBuildcmd(tmp);
	else
		myserv.getList()[fd].remBuildcmd();
	std::cout<< "build commadn : "<<myserv.getList()[fd].getBuildcmd()<<std::endl;

}


