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
int	Channel::isUserIn(std::string user)
{
	std::list<User>::iterator it = _clients.begin();
	for (; it != _clients.end(); it++)
	{
		if ((*it).getNickname() == user)
			return (1);
	}
	return (0);
}

void Channel::setInvited(User &invited)
{
	_invited.push_back(invited);
}

int	Channel::isUserOper(std::string user)
{
    for (std::list<User>::iterator it = _operator.begin(); it != _operator.end(); it++)
    {
                if (it->getNickname() == user)
                    return (1);
    }
    return (0);
}

void Channel::removeOper(std::string sbirro)
{
	std::list<User>::iterator finder = _operator.begin();
	for(; finder != _operator.end(); finder++)
	{
		if (finder->getNickname() == sbirro)
		{
			_operator.erase(finder);
			std::cout << "User removed from operators" << std::endl;
			break ;
		}
	}
	if (finder == _operator.end())
	 	std::cout << "The user in not an operator" << std::endl;
}
void Channel::setOper(User &user)
{
	_operator.push_back(user);
}