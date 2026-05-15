#pragma once
#include "shared/Protocol.h"
#include <thread>
#include <vector>

struct Server
{
    SocketType serverSocket;
    std::vector<SocketType> clientSockets;
    std::vector<std::thread> playerThreads;
    int playerCount;
    int maxPlayers = 4;

    void startServer(int port);
    void acceptPlayers();
    void broadcastMessage(const Message& message);
};

void handlePlayer(SocketType socket);
