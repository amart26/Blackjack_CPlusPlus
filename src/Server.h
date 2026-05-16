#pragma once
#include "shared/Card.h"
#include "shared/Protocol.h"
#include <thread>
#include <vector>

struct Server
{
    SocketType serverSocket;
    std::vector<SocketType> clientSockets;
    std::vector<std::thread> playerThreads;
    std::vector<int> playerBets;
    std::vector<Hand> playerHands;

    Deck deck;
    Hand dealerHand;

    int currentPlayerTurn;
    int playerCount = 0;
    int maxPlayers = 4;
    int dealerScore = 0;
    int playerScore = 0;

    bool isDealerBusted = false;

    void startServer(int port);
    void acceptPlayers();
    void broadcastMessage(const Message& message);

    void startBettingPhase();
    void startGamePhase();
    void nextPlayerTurn();

    void runDealerTurn();

    void calculatePayouts();
};

void handlePlayer(SocketType socket, int playerId);
