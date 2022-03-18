#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <string>
#include <ui.hpp>
#include <connection.hpp>

enum PlayerType
{
    LocalPlayer = 0,
    RemotePlayer = 1
};

class Player
{

    std::string name;
    int win;
    int total;
    int mark;
    PlayerType type;
    int socket = 0;

public:
    /**
     * @brief Construct a new Player object
     * 
     */
    Player();

    /**
     * @brief get name member of class
     * 
     * @return std::string: name
     */
    std::string getName();

    /**
     * @brief get the value of win member
     * 
     * @return int: value
     */
    int getWin();

    /**
     * @brief get the value of total member
     * 
     * @return int 
     */
    int getTotal();

    /**
     * @brief get the value of mark member
     * 
     * @return int 
     */
    int getMark();

    /**
     * @brief get the value of PlayerType member
     * 
     * @return PlayerType 
     */
    PlayerType getType();

    /**
     * @brief get the value of socket member
     * 
     * @return int 
     */
    int getSock();

    /**
     * @brief set value for name member
     * 
     * @param name 
     */
    void setName(std::string name);

    /**
     * @brief Set value for win member
     * 
     * @param win 
     */
    void setWin(int win);

    /**
     * @brief set total value
     * 
     * @param total 
     */
    void setTotal(int total);

    /**
     * @brief Set the Mark object
     * 
     * @param mark 
     */
    void setMark(int mark);

    /**
     * @brief Set value for type member
     * 
     * @param type 
     */
    void setType(PlayerType type);

    /**
     * @brief Set value for socket member
     * 
     * @param sock 
     */
    void setSock(int sock);

    /**
     * @brief take position from user and fill to board
     * 
     * @param board: board game 
     * @param ui: user interface 
     * @param conn: connection to play over tcp/ip 
     */
    void takeAMark(BoardGame &board, UI &ui, Connection *conn);
};

#endif
