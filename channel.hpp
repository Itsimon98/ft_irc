#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "server.hpp"
#include <list>

class User;

class Channel
{
	private :
		std::string _name;
		std::string _password;
		std::list<User> _clients;
		std::list<User> _operator;
		std::list<User> _invited;
		int _empty;
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


};

#endif
