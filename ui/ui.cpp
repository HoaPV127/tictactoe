#include <iostream>

#include <ui.hpp>

using namespace std; 

Cell::Cell()
{

}

Cell::Cell(int x, int y)
{
    this->x = x;
    this->y = y;
}

BoardGame::BoardGame()
{
    this->numOfMarks = 0;
}

BoardGame::~BoardGame()
{

}

void BoardGame::draw()
{
    char row_mark = 'A';
    cout << "  ";
        for (int i = 0; i < size; i++)
        {
            cout <<"   " << i << "  ";
        }
    cout << endl;
    
    for (int i = 0; i < size; i++)
    {   
        cout << "  ";
        for (int j = 0; j < size; j++)
        {
            cout << "+-----";
        }
        cout << "+" << endl;

        cout << row_mark << " ";
        row_mark ++;

        for (int j = 0; j < size; j++)
        {
            int val = board[i][j];
            
            if (val == 1)
                cout << "|  X  ";
            else if (val == -1)
                cout << "|\033[1;31m  X  \033[0m";
            else if (val == -2)
                cout << "|\033[1;31m  O  \033[0m";
            else if (val == 2)
                cout << "|  O  ";
            else
            {
                val = i*size + j;
                cout << "|     ";
            } 
        }
        cout << "|\n";
    }

    cout << "  ";
    for (int j = 0; j < size; j++)
    {
        cout << "+-----";
    }
    cout << "+" << endl;
}

void BoardGame::clear()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = 0;
        }
        
    }
    
}

int BoardGame::getCell(Cell pos)
{
    return this->board[pos.x][pos.y];
}

void BoardGame::setCell(Cell pos, int val)
{   
    this->board[pos.x][pos.y] = val;
    this->lastMark = pos;
    this->numOfMarks += 1;
}

void BoardGame::setSize(int size)
{
    this->size = size;
}

int BoardGame::getSize()
{
    return this->size;
}

void BoardGame::setWin(int win)
{
    this->win = win;
}

int BoardGame::getWin()
{
    return this->win;
}

void BoardGame::colorizeWinLine(Cell cell1, Cell cell2)
{
    if (cell1.x == -1 && cell1.y == -1 && cell2.x == -1 && cell2.y == -1)
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                board[i][j] = 0 - board[i][j];
    }
    else
    {
        if (cell1.x == cell2.x)  /* win in row */
        {
            for (int y = cell1.y; y <= cell2.y; y++)
                board[cell1.x][y] = 0 - board[cell1.x][y];
        }
        else if (cell1.y == cell2.y)    /* win in col */
        {
            for (int x = cell1.x; x <= cell2.x; x++)
                board[x][cell1.y] = 0 - board[x][cell1.y];
        }
        else if (cell1.x > cell2.x)    /* back diagonal */
        {
            for (int i = 0; i <= cell1.x - cell2.x; i++)
                board[cell1.x - i][cell1.y + i] = 0 - board[cell1.x - i][cell1.y + i];
        }
        else if (cell1.x < cell2.x)    /* diagonal */
        {
            for (int i = 0; i <= cell2.x - cell1.x; i++)
                board[cell1.x + i][cell1.y + i] = 0 - board[cell1.x + i][cell1.y + i];
        }
    }
}

bool BoardGame::winInRow(Cell &cell1, Cell &cell2)
{
    int range;
    int len = 1;
    int x, y;

    x = lastMark.x;
    y = lastMark.y;

    cell1.x = x;
    cell1.y = y;
    cell2.x = x;
    cell2.y = y;

    /* check to the left */
    range = y;
    for (int i = 1; i <= range; i++)
    {
        if (board[x][y - i] == board[x][y])
        {
            len += 1;
            cell1.y = y - i;
        }
        else
        {
            break;
        }
    }

    /* check for the right */
    range = size - y - 1;
    for (int i = 1; i <= range; i++)
    {
        if (board[x][y + i] == board[x][y])
        {
            len += 1;
            cell2.y = y + i;
        }
        else
        {
            break;
        }
    }

    if (len >= win)
        return true;
    
    return false;
}

bool BoardGame::winInCol(Cell &cell1, Cell &cell2)
{
    int range;
    int len = 1;
    int x, y;

    x = lastMark.x;
    y = lastMark.y;

    cell1.x = x;
    cell1.y = y;
    cell2.x = x;
    cell2.y = y;

    /* check to the top */
    range = x;
    for (int i = 1; i <= range; i++)
    {
        if (board[x - i][y] == board[x][y])
        {
            len += 1;
            cell1.x = x - i;
        }
        else
        {
            break;
        }
    }

    /* check for the bottom */
    range = size - x - 1;
    for (int i = 1; i <= range; i++)
    {
        if (board[x + i][y] == board[x][y])
        {
            len += 1;
            cell2.x= x + i;
        }
        else
        {
            break;
        }
    }

    if (len >= win)
        return true;
    
    return false;
}

bool BoardGame::winInDiagonal(Cell &cell1, Cell &cell2)
{
    int range;
    int len = 1;
    int x, y;

    x = lastMark.x;
    y = lastMark.y;

    cell1.x = x;
    cell1.y = y;
    cell2.x = x;
    cell2.y = y;

    /* check to the left top corner */
    range = (x < y) ? x : y;
    for (int i = 1; i <= range; i++)
    {
        if (board[x - i][ y - y] == board[x][y])
        {
            len += 1;
            cell1.x = x - i;
            cell1.y = y - i;
        }
        else
        {
            break;
        }
    }

    /* check to the right bottom corner */
    range = (x < y) ? (size - y - 1) : (size - x - 1);
    for (int i = 1; i <= range; i++)
    {
        if (board[x + i][y + i] == board[x][y])
        {
            len += 1;
            cell2.x = x + i;
            cell2.y = y + i;
        }
        else 
        {
            break;
        }
    }

    if (len >= win)
        return true;

    return false;
}

bool BoardGame::winInBackDiagonal(Cell &cell1, Cell &cell2)
{
    int range;
    int len = 1;
    int x, y;

    x = lastMark.x;
    y = lastMark.y;

    cell1.x = x;
    cell1.y = y;
    cell2.x = x;
    cell2.y = y;

    /* check to the right top corner */
    range = (size - x - 1 < y) ? (size - x - 1) : y;
    for (int i = 1; i <= range; i++)
    {
        if (board[x + i][y - i] == board[x][y])
        {
            len += 1;
            cell1.x = x + i;
            cell1.y = y - i;
        }
        else
        {
            break;
        }
    } 

    /* check to the left bottom corner */
    range = (x < size - y - 1) ? x : (size - y - 1);
    for (int i = 1; i <= range; i++)
    {
        if (board[x - i][y + i] == board[x][y])
        {
            len += 1;
            cell2.x = x - i;
            cell2.y = y + i;
        }
        else
        {
            break;
        }
    }

    if (len >= win)
        return true;

    return false;
}

bool BoardGame::isGameOver()
{
    Cell cell1(-1,-1), cell2(-1,-1);

    if (numOfMarks == size * size)
    {
        colorizeWinLine(cell1, cell2);
        return true;
    }

    if (winInRow(cell1, cell2))
    {
        colorizeWinLine(cell1, cell2);
        return true;
    }
    else if (winInCol(cell1, cell2))
    {
        colorizeWinLine(cell1, cell2);
        return true;
    }
    else if (winInDiagonal(cell1, cell2))
    {
        colorizeWinLine(cell1, cell2);
        return true;
    }
    else if (winInBackDiagonal(cell1, cell2))
    {
        colorizeWinLine(cell1, cell2);
        return true;
    }


    return false;
}

Menu::Menu()
{

}

void Menu::setTitle(string tit)
{
    this->title = tit;
}

string Menu::getTitle()
{
    return this->title;
}

void Menu::addItem(string item)
{
    this->items.emplace_back(item);
}

void Menu::clearItems()
{
    items.clear();
}

std::string Menu::getItem(int idx)
{
    return this->items[idx];
}

void Menu::show()
{
    cout << this->title << endl;

    for (int i = 0; i < items.size(); i ++)
    {
        cout << i + 1 << ". " << items[i] << endl; 
    }
}

string UI::getPlayerChoice(string guide)
{
    string inp;
    cout << guide;
    cin >> inp;

    return inp;
}

void UI::sendMessage(std::string msg)
{
    cout << msg << endl;
}
