#ifndef _UI_H_
#define _UI_H_

#include <vector>
#include <string>

#define MAX_SZ 10
#define MIN_SZ 3

enum BoardType 
{
    LocalBoard = 0,
    RemoteBoar = 1
};

class Cell
{
public:
    int x;
    int y;
    Cell();
    Cell(int x, int y);
};

class Menu
{
    std::string title;
    std::vector<std::string> items;

public:
    Menu();

    void setTitle(std::string tit);
    std::string getTitle();

    void addItem(std::string item);
    void clearItems();
    std::string getItem(int idx);

    void show();

};

class BoardGame
{
    int size;
    int win;
    int numOfMarks;
    int board[MAX_SZ][MAX_SZ];
    Cell lastMark;

    bool winInRow(Cell &cell1, Cell &cell2);
    bool winInCol(Cell &cell1, Cell &cell2);
    bool winInDiagonal(Cell &cell1, Cell &cell2);
    bool winInBackDiagonal(Cell &cell1, Cell &cell2);
    void colorizeWinLine(Cell cell1, Cell cell2);

public:
    BoardGame();
    ~BoardGame();
    void draw();
    void clear();

    int getCell(Cell pos);
    void setCell(Cell cell, int val);
    void setSize(int size);
    void setWin(int win);
    int getSize();
    int getWin();
    bool isGameOver();
};

class UI
{
public:
    std::string getPlayerChoice(std::string guide);
    void sendMessage(std::string msg);
};

#endif