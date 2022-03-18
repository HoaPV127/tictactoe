#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <sys/socket.h>
#include <string>

#define MSGLEN 10

enum MsgID
{
    NAME = 0,
    NUM_OF_WIN,
    NUM_OF_TOTAL,
    SET_MARK,
    FILL_MARK
};

typedef struct SockMsg
{
    MsgID id;
    char data[256];
} SockMsg;

class Sock
{
protected:
    int sock;
    int domain;
    int type;
    int protocol;
    std::string ip;
    int port;


public:
    Sock();
    void setPort(int port);
    void setDomain(int domain);
    void setType(int type);
    void setProtocol(int protocol);
};

class SockClient : public Sock
{
public:
    int sockConnect(std::string ip);
};

class SockServer : public Sock
{

public:
    int sockListen();

};

class Connection
{
public:
    SockClient sockClient;
    SockServer sockServer;

    int sockSend(int sock, void *data, int len);
    void sockRecv(int sock, void *data, int *len);

    Connection();
};

#endif /* _CONNECTION_HPP_ */