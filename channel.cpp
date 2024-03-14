# include "channel.hpp"
Channel::Channel(std::string name)
{
	_name = name;
	_empty = 0;
	_pwOn = 0;
	_invOn = 0;
	_topicOn = 0;
    _empty = 0;
	_limitOn = 0;
	_limit = 50;
	_stop = 0;
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
