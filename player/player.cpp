#include <player.hpp>
#include <sstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;



Player::Player()
{
    name = "";
    win = 0;
    total = 0;
    mark = 0;
    type = LocalPlayer;
    int socket = 0;
}

Player::~Player()
{

}

void Player::takeAMark(BoardGame &board, UI &ui, Connection *conn)
{
    int pos;
    string mark;
    Cell cell;
    SockMsg msg[MSGLEN];
    int len = 0;
    stringstream str;
    bool valid = false;
    int size = board.getSize();

    mark = this->mark == 1 ? "X" : "O";

    do
    {
        if (this->type == RemotePlayer) 
        {   
            ui.sendMessage(name + string("'s turn: "));
            do
            {
                len = 0;
                conn->sockRecv(this->socket, msg, &len);
                str << msg[0].data;
            } while (msg[0].id != FILL_MARK);
            
            pos = stoi(str.str());
            
        }
        else 
        {   
            pos = stoi(ui.getPlayerChoice(name + string("'s turn: ")));
            if (conn != NULL)
            {   
                str << pos;
                msg[0].id = FILL_MARK;
                memcpy(msg[0].data, str.str().c_str(), str.str().length());
                int ret = conn->sockSend(this->socket, msg, sizeof(SockMsg) * MSGLEN);
            }
        }

        cell.x = pos / board.getSize();
        cell.y = pos % board.getSize();
        
        if ((pos < size * size) && (board.getCell(cell) == 0)) 
        {
            valid = true;
        }
        else
        {
            valid = false;
        }

    } while (!valid);

    board.setCell(cell, this->mark);
}

std::string Player::getName()
{
    return this->name;
}

int Player::getWin()
{
    return this->win;
}

int Player::getTotal()
{
    return this->total;
}

int Player::getMark()
{
    return this->mark;
}

PlayerType Player::getType()
{
    return this->type;
}

int Player::getSock()
{
    return this->socket;
}

void Player::setName(std::string name)
{
    this->name = name;
}

void Player::setWin(int win)
{
    this->win = win;
}

void Player::setTotal(int total)
{
    this->total = total;
}

void Player::setMark(int mark)
{
    this->mark = mark;
}
void Player::setType(PlayerType type)
{
    this->type = type;
}

void Player::setSock(int sock)
{
    this->socket = sock;
}
