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
int	Channel::isUserIn(std::string nickname)
{
	std::list<User>::iterator it = _clients.begin();

	for (; it != _clients.end(); it++)
	{
		if (it->getNickname() == nickname)
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

void Channel::setOper(User &user)
{
	_operator.push_back(user);
}
std::list<User> &Channel::getOper()
{
	return(this->_operator);
}

void Channel::removeOper(std::string operatorn)
{
	std::list<User>::iterator finder = _operator.begin();
	for(; finder != _operator.end(); finder++)
	{
		if (finder->getNickname() == operatorn)
		{
			_operator.erase(finder);
			std::cout << "Operator removed!\n" << std::endl;
			break ;
		}
	}
	if (finder == _operator.end())
	 	std::cout << "User is not an operator!\n" << std::endl;
}
void Channel::setPw(std::string &password)
{
	this->_pwOn = 1;
	this->_password = password;
}
void Channel::removePw()
{
	this->_password = "";
	this->_pwOn = 0;
}
void Channel::setInvOn()
{
	this->_invOn = 1;
}

void Channel::remInvOn()
{
	this->_invOn = 0;
}

void Channel::setTopicOn()
{
	this->_topicOn = 1;
}
void Channel::removeTopicOn()
{
	this->_topicOn = 0;
}
void Channel::removeLimitOn()
{
	this->_limitOn = 0;
}
void Channel::setLimitOn(Server &myserv, int limit)
{
	this->_limitOn = 1;
	_limit = limit;

	if (_limit > this->_clients.size())
	{
		_stop = 0;
		return ;
	}
	else if (_limit == this->_clients.size())
	{
		_stop = 1;
		return ;
	}
	size_t i = 0;
	for (std::list<User>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (i >= _limit)
		{
			std::string server = "IrcServer";
			std::string reason = "Channel is full";
			std::string msg = ":" + server + " KICK " + getName() + " " + (*it).getNickname() + " " + reason + "\r\n";
			std::string msg1 = ":" + server +  "!" + " KICK " + getName() + " " + (*it).getNickname() + " " + reason + "\n";
			myserv.ft_send_all_chan(myserv, (*this), msg1);
			myserv.sendData((*it).getSocket(), msg);
			this->_clients.erase(it);
			it--;
		}
		i++;
	}
	_stop = 1;
}

int Channel::isInvOn()
{
	return(this->_invOn);
}

int Channel::isPwOn()
{
	return(this->_pwOn);
}
int Channel::isLimitOn()
{
	return(this->_limitOn);
}
int Channel::isTopicOn()
{
	return(this->_topicOn);
}

std::string Channel::getPw()
{
	return(this->_password);
}
size_t Channel::getLimit()
{
	return(this->_limit);
}

int	Channel::isUserInvited(std::string user)
{
    for (std::list<User>::iterator it = this->_invited.begin(); it != this->_invited.end(); it++)
    {
                if (it->getNickname() == user)
                    return (1);
    }
    return (0);
}
