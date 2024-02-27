#include "server.hpp"

int main(int argc, char **argv)
{
    Server myserv(0, "");
    init(argc, argv, myserv);
	cycle(myserv);

}
