#ifndef USER_HPP
#define USER_HPP
#include "server.hpp"

class User
{
	private:
	std::string _userame;
	std::string _realname;
	std::string _password;
	public:
	User();
	//~User();
	std::string getUsername();
	std::string getPassword();
	std::string getRealname();

};

#endif
