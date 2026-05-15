#include "Protocol.h"

void sendMessage(SocketType socket, const Message& message)
{
    send(socket, (char*)&message, sizeof(Message), 0);
}

Message receiveMessage(SocketType socket)
{
    Message message;
    recv(socket, (char*)&message, sizeof(message), 0);
    return message;
}