#include <connection.hpp>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

Sock::Sock()
{
    this->domain = AF_INET;
    this->type = SOCK_STREAM;
    this->protocol = 0;
    this->port = 8080;
}

void Sock::setPort(int port)
{
    this->port = port;
}
void Sock::setDomain(int domain)
{
    this->domain = domain;
}
void Sock::setType(int type)
{
    this->type = type;
}
void Sock::setProtocol(int protocol)
{
    this->protocol = protocol;
}

int SockClient::sockConnect(std::string ip)
{
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0) 
    {
        return -2;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return -3;
    }

    return sock;
}

int SockServer::sockListen()
{
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int clisock = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock <= 0)
    {
        return -1;
    }
       
    // Forcefully attaching socket to the port 8080
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
       
    // Forcefully attaching socket to the port 
    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        return -2;
    }

    if (listen(sock, 1) < 0)
    {
        return -3;
    }
    

    clisock = accept(sock, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    return clisock;
}

Connection::Connection()
{
    
}

int Connection::sockSend(int sock, void *data, int len)
{
    return send(sock, data, len, 0);
}

void Connection::sockRecv(int sock, void *data, int *len)
{
    int ret;
    SockMsg msg[10];

    ret = read(sock, msg, 10240);

    memcpy(data, msg, ret);
    *len = ret;
}