#include "server.hpp"
Server myserv(0, "");
   
void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		std::map<int, User>::iterator it = myserv.getList().begin();
		std::map<int, User>::iterator ite = myserv.getList().end();
		std::cout << std::endl;
		std::cout << "Server chiuso!" << std::endl;
		//myserv.remBuildcmd();
		for(size_t i = 0; (it != ite) && (i < myserv.getList().size()); it++, i++)
		{
			myserv.sendData((*it).first, "DISCONNECTED\r\n");
			close((*it).first);
		}
		close(myserv.getServSock());
		void exit();
	}
}

int main(int argc, char **argv)
{
	signal(SIGINT, sig_handler);
    if(init(argc, argv, myserv))
	{
        return(1);
	}
	if(cycle(myserv))
	{
		return(1);
	}

}
