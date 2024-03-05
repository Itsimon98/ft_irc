#include "server.hpp"


int main(int argc, char **argv)
{
    Server myserv(0, "");
    if(init(argc, argv, myserv))
        return(1);
	if(cycle(myserv))
		return(1);

}
