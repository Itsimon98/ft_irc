#include "server.hpp"

int main(int argc, char **argv)
{
    Server myserv(0, "");
    if(init(argc, argv, myserv) == 1)
        return(1);
	cycle(myserv);

}
