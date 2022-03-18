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
    /**
     * @brief Construct a new Sock object
     * default constructor for Sock
     */
    Sock();
    /**
     * @brief set value for port member
     * 
     * @param port 
     */
    void setPort(int port);
    /**
     * @brief set value for domain member
     * 
     * @param domain 
     */
    void setDomain(int domain);
    /**
     * @brief set value for type member
     * 
     * @param type 
     */
    void setType(int type);
    /**
     * @brief set value for protocol member
     * 
     * @param protocol 
     */
    void setProtocol(int protocol);
};

class SockClient : public Sock
{
public:
    /**
     * @brief create socket and connect to server
     * 
     * @param ip : ip of server
     * @return int : socket fd
     */
    int sockConnect(std::string ip);
};

class SockServer : public Sock
{

public:
    /**
     * @brief create socket and listen for other connections
     * 
     * @return int: socketfd of client which connects to 
     */
    int sockListen();

};

class Connection
{
public:
    SockClient sockClient;
    SockServer sockServer;

    /**
     * @brief send data over socket
     * 
     * @param sock: sockfd of destination host
     * @param data: pointer point to data want to send
     * @param len : size of data (byte)
     * @return int: return number of transfered data
     */
    int sockSend(int sock, void *data, int len);

    /**
     * @brief receive data over socket
     * 
     * @param sock: sockfd of source host
     * @param data: pointer point to memory where data stored 
     * @param len: pointer to get size of received data
     */
    void sockRecv(int sock, void *data, int *len);
};

#endif /* _CONNECTION_HPP_ */