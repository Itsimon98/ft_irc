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