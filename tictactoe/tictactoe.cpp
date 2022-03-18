#include <tictactoe.hpp>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;

TicTacToe::TicTacToe()
{
    
    mainMenu.setTitle("*** TicTacToe ***");
    mainMenu.addItem("Play Game");
    mainMenu.addItem("Ranking");
    mainMenu.addItem("Exit");

    gameMenu.setTitle("*** TicTacToe ***");
    gameMenu.addItem("Local Game");
    gameMenu.addItem("Remote Game");
    gameMenu.addItem("Exit");
    
    loadPlayerRank();
    rankMenu.setTitle("***** Ranking *****");

}

void TicTacToe::loadPlayerRank()
{
    string buffer;

    ifListPlayer.open(".ini");

    while (!ifListPlayer.eof())
    {

        getline(ifListPlayer, buffer);

        if (buffer.length() == 0)
            break;

        stringstream strstream(buffer);

        string name, win, total;
        getline(strstream, name, ' ');
        getline(strstream, win, ' ');
        getline(strstream, total, ' ');

        if (name.length() <= 0 || win.length() <= 0 || total.length() <= 0)
            continue;

        shared_ptr<Player> newPlayer = make_shared<Player>();
        newPlayer->setName(name);
        newPlayer->setWin(stoi(win));
        newPlayer->setTotal(stoi(total));
        
        vPlayers.emplace_back(newPlayer);
    }

    ifListPlayer.close();

    for (int i = 0; i < vPlayers.size(); i ++)
    {
        mPlayers[vPlayers[i]->getName()] = vPlayers[i];
    }
}

void TicTacToe::storePlayerRank()
{
    ofListPlayer.open(".ini");

    for (size_t i = 0; i < vPlayers.size(); i++)
    {
        ofListPlayer << vPlayers[i]->getName() << " ";
        ofListPlayer << vPlayers[i]->getWin() << " ";
        ofListPlayer << vPlayers[i]->getTotal() << endl; 
    }

    ofListPlayer.close();
}

void TicTacToe::play()
{
    string userInp;
    int choiceNumber;
    bool exit = false;

    while (!exit)
    {

        mainMenu.show();

#ifdef DEBUG
        choiceNumber = 1;
#else
        userInp = ui.getPlayerChoice(string("Enter your choice: "));
        choiceNumber = stoi(userInp);
#endif

        switch (choiceNumber)
        {
        case 1:
            getInGameMenu();
            break;

        case 2:
            getInRankMenu();
            break;

        case 3:
            exit = 1;
            break;

        default:
            userInp = ui.getPlayerChoice(string("Enter your choice: "));
            choiceNumber = stoi(userInp);
            break;
        }

    }

    storePlayerRank();
}

void TicTacToe::getInGameMenu()
{
    string userInp;
    int choiceNumber;
    
    gameMenu.show();

#ifdef DEBUG
    choiceNumber = 1;
#else
    userInp = ui.getPlayerChoice(string("Enter your choice: "));
    choiceNumber = stoi(userInp);
#endif
    switch (choiceNumber)
    {
    case 1: /* Local game */
        this->playGame();
        break;

    case 2: /* TCP/IP game */ 
        this->playGameTCP();
        break;
    
    default:
        break;
    }
}

void TicTacToe::playGame()
{
    int size, win;
    string mark;
    string name1, name2;

#ifdef DEBUG
    size = 5;
    win = 3;
    name1 = "james";
    name2 = "peter";   
#else
    size = stoi(ui.getPlayerChoice("Enter size of board: "));
    win = stoi(ui.getPlayerChoice("Enter length to win: "));
    name1 = ui.getPlayerChoice("Enter Player1's name: ");
    name2 = ui.getPlayerChoice("Enter Player2's name: ");
#endif

    board.setSize(size);
    board.setWin(win);
    board.clear();


    if (mPlayers[name1] != NULL && mPlayers[name1]->getName().compare(name1) == 0)
    {
        player1 = mPlayers[name1];
    }
    else
    {
        player1 = make_shared<Player>();
        player1->setName(name1);
        vPlayers.emplace_back(player1);
        mPlayers[name1] = player1;
    }

    if (mPlayers[name2] != NULL && mPlayers[name2]->getName().compare(name2) == 0)
    {
        player2 = mPlayers[name2];
    }
    else
    {
        player2 = make_shared<Player>();
        player2->setName(name2);
        vPlayers.emplace_back(player2);
        mPlayers[name2] = player2;
    }

    player1->setType(LocalPlayer);
    player2->setType(LocalPlayer);


    srand (time(NULL));
    int r = rand();
    if (r % 2 == 0)
    {
        player1->setMark(1);
        player2->setMark(2);
        player = player1;
    }
    else
    {
        player1->setMark(2);
        player2->setMark(1);
        player = player2;
    }

    while (1)
    {   
        board.draw();

        player->takeAMark(board, ui, NULL);     

        if (board.isGameOver())
        {
            board.draw();
            player->setWin(player->getWin() + 1);
            player1->setTotal(player1->getTotal() + 1);
            player2->setTotal(player2->getTotal() + 1);
            break;
        }        

        player = (player == player1) ? player2 : player1;
    }
}

void TicTacToe::playGameTCP()
{
    string choice;
    string mark;
    string name1, name2;
    int sock;
    bool isHost;
    SockMsg msg[MSGLEN];
    int len;

    name1 = ui.getPlayerChoice("Enter Your name: ");
    if (mPlayers[name1] != NULL && mPlayers[name1]->getName().compare(name1) == 0)
    {
        player1 = mPlayers[name1];
    }
    else
    {
        player1 = make_shared<Player>();
        player1->setName(name1);
        vPlayers.emplace_back(player1);
        mPlayers[name1] = player1;
    }

    player2 = make_shared<Player>();

    choice = ui.getPlayerChoice("You are host (yY) ? ");
    if (choice == "y" || choice == "Y")
    {   
        // size = stoi(ui.getPlayerChoice("Enter size of board: "));
        // win = stoi(ui.getPlayerChoice("Enter length to win: "));
        ui.sendMessage("Wait for other connects to the game: ");

        sock = conn.sockServer.sockListen();   
        player1->setSock(sock);
        player2->setSock(sock);

        getRemotePlayerInfo(player2);
        sendPlayerInfo(player1, sock);

        ui.sendMessage(player2->getName() + " connected to the game");
        isHost = true;
    }
    else
    {   
        do
        {
            sock = conn.sockClient.sockConnect(string("127.0.0.1"));
            if (sock <= 0)
            {
                ui.sendMessage("cannot connect, retry ... ");
                sleep(1);
                continue;
            }

            player1->setSock(sock);
            player2->setSock(sock);
            sendPlayerInfo(player1, sock);
            getRemotePlayerInfo(player2);

            ui.sendMessage("connected to the " + player2->getName() + "'s game");
            isHost = false;
        } while (sock <= 0);
    }

    
    player1->setType(LocalPlayer);
    player2->setType(RemotePlayer);

    if (isHost)
    {
        srand (time(NULL));
        int r = rand();
        if (r % 2 == 0)
        {
            player1->setMark(1);
            player2->setMark(2);
            player = player1;
        }
        else
        {
            player1->setMark(2);
            player2->setMark(1);
            player = player2;
        }

        stringstream mark;
        msg[0].id = SET_MARK;
        mark << player2->getMark();
        strcpy(msg[0].data, mark.str().c_str());
        conn.sockSend(player2->getSock(), msg, sizeof(SockMsg));
    }
    else 
    {      
        do
        {
            conn.sockRecv(player2->getSock(), msg, &len);

        } while (msg[0].id != SET_MARK);
        
        if (msg[0].id == SET_MARK)
        {   
            if (string(msg[0].data) == "1")
            {
                player1->setMark(1);
                player2->setMark(2);
                player = player1;
            }
            else
            {
                player1->setMark(2);
                player2->setMark(1);
                player = player2;
            }
        }
        else
        {
            return;
        }
    }

    ui.sendMessage(player->getName() + " goes first\n");



    board.setSize(5);
    board.setWin(3);
    board.clear();
    
    while (1)
    {   
        board.draw();

        player->takeAMark(board, ui, &conn);     
    
        if (board.isGameOver())
        {
            board.draw();
            player->setWin(player->getWin() + 1);
            player1->setTotal(player1->getTotal() + 1);
            player2->setTotal(player2->getTotal() + 1);
            break;
        }        

        player = (player == player1) ? player2 : player1;
    }

}

void TicTacToe::getInRankMenu()
{
    rankMenu.clearItems();
    for (int i = 0; i < vPlayers.size(); i++)
    {
        stringstream item;
        item << vPlayers[i]->getName() << " ";
        item << vPlayers[i]->getWin()  << " ";
        item << vPlayers[i]->getTotal();
        
        rankMenu.addItem(item.str());
    }

    rankMenu.show();
}

void TicTacToe::getRemotePlayerInfo(std::shared_ptr<Player> remotePlayer)
{
    int len;
    SockMsg msg[MSGLEN];
    memset(msg, 0, sizeof(SockMsg) * MSGLEN);
    conn.sockRecv(remotePlayer->getSock(), msg, &len);
    string data;

    for (int i = 0; i < len/sizeof(SockMsg); i ++)
    {
        data = msg[i].data;
        switch (msg[i].id)
        {
        case NAME:
            player2->setName(data);
            break;

        case NUM_OF_WIN:
            player2->setWin(stoi(data));
            break;

        case NUM_OF_TOTAL:
            player2->setTotal(stoi(data));
            break;
        
        default:
            break;
        }
    }
    
    string name = remotePlayer->getName();

    if (mPlayers[name] != NULL && mPlayers[name]->getName().compare(name) == 0)
    {
        mPlayers[name]->setWin(player2->getWin());
        mPlayers[name]->setTotal(player2->getTotal());
        mPlayers[name]->setSock(player2->getSock());

        remotePlayer = move(mPlayers[name]);
    }
    else
    {
        vPlayers.emplace_back(remotePlayer);
        mPlayers[name] = remotePlayer;
    }

    remotePlayer->setType(RemotePlayer);

}

void TicTacToe::sendPlayerInfo(std::shared_ptr<Player> localPlayer, int remotePlayerSock)
{
    SockMsg msg[MSGLEN];
    memset(msg, 0, sizeof(SockMsg) * MSGLEN);
    stringstream win;
    stringstream total;

    msg[0].id = NAME;
    strcpy(msg[0].data, localPlayer->getName().c_str());

    msg[1].id = NUM_OF_WIN;
    win << localPlayer->getWin();
    strcpy(msg[1].data, win.str().c_str());

    msg[2].id = NUM_OF_TOTAL;
    total << localPlayer->getTotal();
    strcpy(msg[2].data, total.str().c_str());

    int ret = conn.sockSend(remotePlayerSock, msg, sizeof(SockMsg) * 3);
}