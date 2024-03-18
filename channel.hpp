#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "server.hpp"
#include <list>

class User;
class Server;

class Channel
{
	private :
		std::string _name;
		std::string _password;
		std::list<User> _clients;
		std::list<User> _operator;
		std::list<User> _invited;
		int _empty;
		int _pwOn;
		int _invOn;
		int _topicOn;
		int _limitOn;
		int _limit;
		int _stop;
	public:
		Channel();
		Channel(std::string);
		~Channel();
		void setName(std::string name);
		void setPassword(std::string password);
		void setClient(User &user);
		std::string getName();
		std::string getPassword();
		std::list<User> &getListUsers();
		void setEmpty(int empty);
		int	isUserIn(std::string user);
		void setInvited(User &invited);
		int isUserOper(std::string user);
		void setOper(User &suser);
		std::list<User> &getOper();
		void	removeOper(std::string operatorn);
		void	setPw(std::string &pw);
		void	removePw();
		void	setInvOn();
		void	remInvOn();
		void	setTopicOn();
		void	removeTopicOn();
		void	setLimitOn(Server & myserv, int limit);
		void	removeLimitOn();
		int	isInvOn();
		int	isPwOn();
		int	isTopicOn();
		int	isUserInvited(std::string user);
		std::string getPw();
		int isLimitOn();
		int getLimit();




};

#endif
