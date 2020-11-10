#ifndef MUTARI_H_
#define MUTARI_H_
#include "tabla.h"
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine e(seed);


class validMove
{
public:
    validMove()
{
    this->prevRow = 0;
    this->prevColumn = 0;
    this->currentColumn = 0;
    this->currentRow = 0;
}

    validMove(int a,int b,int c,int d)
{
    this->prevRow=a;
    this->prevColumn=b;
    this->currentRow=c;
    this->currentColumn=d;
}
    int prevRow;
    int prevColumn;
    int currentRow;
    int currentColumn;
};

void miscariPion(ChessPlayer *player, char currentSide, vector<validMove> &all_moves, int i, int j)
{
    
    if (i == 1 && player->table[i + 2][j].color == 'N' && player->table[i + 1][j].color == NONE && currentSide == 'W')
        all_moves.push_back(validMove(i, j, i + 2, j)); // daca e in pozitia de inceput a jocului pionul poate muta 2 casute

    if (i == 6 && player->table[i - 2][j].color == 'N' && player->table[i - 1][j].color == NONE && currentSide == 'B')
        all_moves.push_back(validMove(i, j, i - 2, j)); // acelasi lucru doar ca pt cealalta parte

    if (currentSide == 'W')
    {
        // miscare inainte pe alb
        if (i + 1 < 8 && player->table[i + 1][j].color == 'N')
            all_moves.push_back(validMove(i, j, i + 1, j));

        // miscare pe stanga sus doar pe alb
        if (i + 1 < 8 && j - 1 >= 0 && player->table[i + 1][j - 1].color == changeSide(currentSide))
            all_moves.push_back(validMove(i, j, i + 1, j - 1));

        //miscare pe dreapta sus doar pe alb

        if (i + 1 < 8 && j + 1 < 8 && player->table[i + 1][j + 1].color == changeSide(currentSide))
            all_moves.push_back(validMove(i, j, i + 1, j + 1));
    }
    else
    {
        //miscare intainte doar pe negru
        if (i - 1 >= 0 && player->table[i - 1][j].color == 'N')
            all_moves.push_back(validMove(i, j, i - 1, j));

        // miscare pe stanga jos doar pe negru
        if (i - 1 >= 0 && j - 1 >= 0 && player->table[i - 1][j - 1].color == changeSide(currentSide))
            all_moves.push_back(validMove(i, j, i - 1, j - 1));

        //miscare pe dreapta jos doar pe negru
        if (i - 1 >= 0 && j + 1 < 8 && player->table[i - 1][j + 1].color == changeSide(currentSide))
            all_moves.push_back(validMove(i, j, i - 1, j + 1));
    }

    return;
}
void miscariTura(ChessPlayer *player, char currentSide, vector<validMove> &all_moves,int i,int j)
{
    // incearca miscasri in sus

    int x = i + 1, y = j;

    while (x < 8 && player->table[x][y].color != currentSide)
    {
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;
        x++;
    }

    //incearca miscari la stanga
    x = i;
    y = j - 1;

    while (y >= 0 && player->table[x][y].color != currentSide)
    {
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;
        y--;
    }

    // incearca miscari la dreapta
    x = i;
    y = j + 1;

    while (y < 8 && player->table[x][y].color != currentSide)
    {
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;
        y++;
    }

    // incearca miscari in jos
    x = i - 1;
    y = j;

    while (x >= 0 && player->table[x][y].color != currentSide)
    {   
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;
        x--;
    }

    return;
}
void miscariNebun(ChessPlayer *player, char currentSide, vector<validMove> &all_moves,int i,int j)
{
    int x = i + 1, y = j - 1;

    // miscari diagonala stanga sus

    while (x < 8 && y >= 0 && player->table[x][y].color != currentSide)
    {
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;

        x++;
        y--;
    }

    // miscari diagonala dreapta sus

    x = i + 1, y = j + 1;

    while (x < 8 && y < 8 && player->table[x][y].color != currentSide)
    {
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;
        x++;
        y++;
    }

    //miscari diagonala dreapta jos

    x = i - 1;
    y = j + 1;

    while (x >= 0 && y < 8 && player->table[x][y].color != currentSide)
    {
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;
        x--;
        y++;
    }
    // miscari stanga jos
    x = i - 1;
    y = j - 1;

    while (x >= 0 && y >= 0 && player->table[x][y].color != currentSide)
    {
        all_moves.push_back(validMove(i, j, x, y));
        if (player->table[x][y].color == changeSide(currentSide)) break;
        x--;
        y--;
    }

    return;
}
void miscariRege(ChessPlayer *player, char currentSide, vector<validMove> &all_moves,int i,int j)
{
    // miscare stanga

    if (j - 1 >= 0 && player->table[i][j - 1].color != currentSide)
        all_moves.push_back(validMove(i, j, i, j - 1));

    // miscare dreata

    if (j + 1 < 8 && player->table[i][j + 1].color != currentSide)
        all_moves.push_back(validMove(i, j, i, j + 1));

    // miscare sus

    if (i + 1 < 8 && player->table[i + 1][j].color != currentSide)
        all_moves.push_back(validMove(i, j, i + 1, j));

    // miscare jos

    if (i - 1 >= 0 && player->table[i - 1][j].color != currentSide)
        all_moves.push_back(validMove(i, j, i - 1, j));

    // miscare stanga sus
    if (i + 1 < 8 && j - 1 >= 0 && player->table[i + 1][j - 1].color != currentSide)
        all_moves.push_back(validMove(i, j, i + 1, j - 1));

    // miscare stanga jos

    if (i - 1 >= 0 && j - 1 >= 0 && player->table[i - 1][j - 1].color != currentSide)
        all_moves.push_back(validMove(i, j, i - 1, j - 1));

    // miscare dreapta sus
    if (i + 1 < 8 && j + 1 < 8 && player->table[i + 1][j + 1].color != currentSide)
        all_moves.push_back(validMove(i, j, i + 1, j + 1));

    //miscare dreapta jos
    if (i - 1 >= 0 && j + 1 < 8 && player->table[i - 1][j + 1].color != currentSide)
        all_moves.push_back(validMove(i, j, i - 1, j + 1));

    return;
}

void miscariCal(ChessPlayer *player, char currentSide, vector<validMove> &all_moves,int i,int j)
{
    // miscare 3 la dreapta 1 jos

    int x = i - 1, y = j + 2;

    if (x >= 0 && y < 8 && player->table[x][y].color!= currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    // miscare 1 la dreapta 3 in jos

    x = i - 2;
    y = j + 1;

    if (x >= 0 && y < 8 && player->table[x][y].color!= currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    // miscare 3 la stanga 1 in jos

    x = i - 1;
    y = j - 2;

    if (x >= 0 && y >= 0 && player->table[x][y].color!= currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    //miscare 1 la stanga 3 in jos

    x = i - 2;
    y = j - 1;

    if (x >= 0 && y >= 0 && player->table[x][y].color!= currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    // miscare 3 la stanga 1 in sus

    x = i + 1;
    y = j - 2;

    if (x < 8 && y >= 0 && player->table[x][y].color!= currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    // miscare 1 la stanga 3 in sus

    x = i + 2;
    y = j - 1;

    if (x < 8 && y >= 0 && player->table[x][y].color != currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    //miscare 3 la dreapta 1 in sus

    x = i + 1;
    y = j + 2;

    if (x < 8 && y < 8 && player->table[x][y].color != currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    //miscare 1 la dreapta 3 in sus

    x = i + 2;
    y = j + 1;

    if (x < 8 && y < 8 && player->table[x][y].color != currentSide)
        all_moves.push_back(validMove(i, j, x, y));

    return;
}

vector<validMove> findAllMoves(ChessPlayer *player, char currentSide)
{

    int i, j;

    vector<validMove> all_moves;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
            if (player->table[i][j].color == currentSide)
            {
                switch (player->table[i][j].name)
                {
                case PION:
                    miscariPion(player, currentSide, all_moves,i,j);
                    break;
                case TURA:
                    miscariTura(player, currentSide, all_moves,i,j);
                    break;
                case REGINA:
                    miscariNebun(player, currentSide, all_moves,i,j);
                    miscariTura(player, currentSide, all_moves,i,j);
                    break;
				case CAL:
                    miscariCal(player, currentSide, all_moves,i,j);
                    break;
                case NEBUN:
                    miscariNebun(player, currentSide, all_moves,i,j);
                    break;
                case REGE:
                    miscariRege(player, currentSide, all_moves,i,j);
                    break;
                }
            }
    }
	// shuffle(all_moves.begin(), all_moves.end(), e);
    return all_moves;
}
#endif