# include "channel.hpp"
Channel::Channel(std::string name)
{
	_name = name;
}
Channel::~Channel(){}

void Channel::setClient(User &user)
{
	_clients.push_back(user);
}
std::list<User> &Channel::getListUsers()
{
	return(_clients);
}
std::string Channel::getName()
{
	return(_name);
}
void Channel::setEmpty(int empty)
{
    _empty = empty;
}