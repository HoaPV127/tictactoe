#ifndef _TICTACTOE_HPP_
#define _TICTACTOE_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>

#include <ui.hpp>
#include <player.hpp>
#include <connection.hpp>

class TicTacToe
{

    UI ui;
    Menu mainMenu;
    Menu gameMenu;
    Menu rankMenu;
    BoardGame board;
    std::ifstream ifListPlayer;
    std::ofstream ofListPlayer;

    std::vector<std::shared_ptr<Player>> vPlayers;
    std::unordered_map<std::string, std::shared_ptr<Player>> mPlayers;
    std::shared_ptr<Player> player;
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;

    Connection conn;

    void loadPlayerRank();
    void storePlayerRank();
    void getRemotePlayerInfo(std::shared_ptr<Player> remotePlayer);
    void sendPlayerInfo(std::shared_ptr<Player> localPlayer, int remotePlayerSock);

public:
    TicTacToe();

    void play();

    void getInGameMenu();

    void getInRankMenu();

    void playGame();

    void playGameTCP();

    void togglePlayer();
};

#endif