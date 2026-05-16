

#include "Server.h"
#include <iostream>

int main()
{
    Server server;
    server.startServer(7777);
    std::cout << "Waiting for connection...\n";
    server.acceptPlayers();
}
