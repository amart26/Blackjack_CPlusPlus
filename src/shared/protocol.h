#pragma once

enum class MessageType
{
    PLAYER_CONNECT,
    PLAYER_DISCONNECTED,
    BET,
    DEAL,
    HIT,
    STAND,
    GAME_STATE,
    GAME_OVER,
    WAITING,
    YOUR_TURN,
    PLAYER_BUST,
    DEALER_TURN
};

struct Message
{
    MessageType type;
    int data;
    int playerId;
};

#ifdef _WIN32
#include <winsock2.h>
using SocketType = SOCKET;
#else
#include <sys/socket.h>
using SocketType = int;
#endif