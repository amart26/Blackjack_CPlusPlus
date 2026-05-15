#include "Server.h"
#include <iostream>

void Server::startServer(int port)
{
#ifdef _WIN32
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, maxPlayers);

    std::cout << "Server started on port: " << port << "\n ";
}

void Server::acceptPlayers()
{
    while (playerCount < maxPlayers)
    {
        SocketType newSocket = accept(serverSocket, nullptr, nullptr);
        clientSockets.push_back(newSocket);

        playerCount++;

        std::cout << "Player connected! Total Players: " << playerCount << "\n";

        std::thread playerThread(handlePlayer, newSocket, playerCount - 1);
        playerThread.detach();
    }
}

void Server::broadcastMessage(const Message& message)
{
    for (int i = 0; i < clientSockets.size(); i++)
    {
        sendMessage(clientSockets[i], message);
    }
}

void Server::startBettingPhase()
{
    Message waitingMessage;
    waitingMessage.type = MessageType::WAITING;
    waitingMessage.data = 0;
    waitingMessage.playerId = 0;

    std::cout << "Betting phase started\n";
    broadcastMessage(waitingMessage);

    playerBets.assign(playerCount, 0);
}

void Server::startGamePhase()
{
    std::cout << "Game phase has start.\n";
    fillDeck(deck);
    shuffleDeck(deck);

    for (int i = 0; i < playerCount; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            dealCard(deck, playerHands[i]);
        }
    }

    dealCard(deck, dealerHand);
    dealCard(deck, dealerHand);
    currentPlayerTurn = 0;

    Message yourTurnMessage;
    yourTurnMessage.type = MessageType::YOUR_TURN;
    yourTurnMessage.data = 0;
    yourTurnMessage.playerId = 0;
    sendMessage(clientSockets[0], yourTurnMessage);
}

void Server::nextPlayerTurn()
{
    currentPlayerTurn++;
    if (currentPlayerTurn >= playerCount)
    {
        Message dealerTurnMessage;
        dealerTurnMessage.type = MessageType::DEALER_TURN;
        dealerTurnMessage.data = 0;
        broadcastMessage(dealerTurnMessage);
    }
    else
    {

        Message yourTurnMessage;
        yourTurnMessage.type = MessageType::YOUR_TURN;
        yourTurnMessage.data = 0;
        yourTurnMessage.playerId = currentPlayerTurn;
        sendMessage(clientSockets[currentPlayerTurn], yourTurnMessage);
    }
}

void Server::runDealerTurn()
{
    dealerScore = calculateScore(dealerHand);
    while (dealerScore < 17)
    {
        dealCard(deck, dealerHand);
        dealerScore = calculateScore(dealerHand);
    }
    if (dealerScore > 21)
    {
        Message gameOverMessage;
        gameOverMessage.type = MessageType::GAME_OVER;
        gameOverMessage.data = 0;

        broadcastMessage(gameOverMessage);
    }
    else if (dealerScore >= 17 && dealerScore <= 21)
    {
        Message gameOverMessage;
        gameOverMessage.type = MessageType::GAME_OVER;
        gameOverMessage.data = 0;

        broadcastMessage(gameOverMessage);
    }
}

void handlePlayer(SocketType socket, int playerId)
{
    while (true)
    {
        Message message = receiveMessage(socket);
        switch (message.type)
        {
        case MessageType::HIT:
            std::cout << "Player wants to hit.\n";
            break;
        case MessageType::STAND:
            std::cout << "Player wants to stand.\n";
            break;
        case MessageType::BET:
            std::cout << "Player wants to bet. " << message.data << "\n";
            break;
        case MessageType::DEAL:
            std::cout << "Player wants to deal.\n";
            break;
        default:
            std::cout << "unkown message";
            break;
        }
    }
}
