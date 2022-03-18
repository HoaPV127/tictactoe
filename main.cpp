#include <iostream>
#include <tictactoe.hpp>


using namespace std;


int main()
{
    TicTacToe *game = new TicTacToe();
    game->play();
    return 0;
}