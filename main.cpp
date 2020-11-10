#include <bits/stdc++.h>
#include "tabla.h"
#include "mutari.h"
#include "values.h"
using namespace std;
#define STR_BUFF 256

// ofstream fout("deb.out");

#define P_PION 100
#define P_CAL 320
#define P_NEBUN 330
#define P_TURA 500
#define P_REGINA 900
#define P_REGE 20000

int evaluate(ChessPlayer *player, char currentSide)
{
    int score = 0;
	
    for (int i = 0; i <= 7; ++i) {
        for (int j = 0; j <= 7; ++j) {
            if (player->table[i][j].color == currentSide) {
                switch (player->table[i][j].name) {
                    case PION:
                        score += P_PION;
                        break;
                    case CAL:
                        score += P_CAL;
                        break;
                    case NEBUN:
                        score += P_NEBUN;
                        break;
                    case TURA:
                        score += P_TURA;
                        break;
                    case REGINA:
                        score += P_REGINA;
                        break;
                }
            }
            if (player->table[i][j].color == changeSide(currentSide)) {
                switch (player->table[i][j].name) {
                    case PION:
                        score -= P_PION;
                        break;
                    case CAL:
                        score -= P_CAL;
                        break;
                    case NEBUN:
                        score -= P_NEBUN;
                        break;
                    case TURA:
                        score -= P_TURA;
                        break;
                    case REGINA:
                        score -= P_REGINA;
                        break;
                }
            }
        }
    }
    
    for (int i = 0; i < 8 ; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (player->table[i][j].color == currentSide) {
                switch (player->table[i][j].name) {
                    case PION: 
                        score += pawnValues[i][j];
                        break;
                    case CAL:
                        score += knightValues[i][j];
                        break;
                    case NEBUN: 
                        score += bishopValues[i][j];
                        break;
                    case TURA:
                        score += rookValues[i][j];
                        break;
                    case REGINA: 
                        score += queenValues[i][j];
                        break;
                    case REGE:
                        score += kingValues[i][j];
                        break;
                }
            }
            if (player->table[i][j].color == changeSide(currentSide)) {
                switch (player->table[i][j].name) {
                    case PION: 
                        score -= transpose(pawnValues, i, j);
                        break;
                    case CAL:
                        score -= transpose(knightValues, i, j);
                        break;
                    case NEBUN: 
                        score -= transpose(bishopValues, i, j);
                        break;
                    case TURA:
                        score -= transpose(rookValues, i, j);
                        break;
                    case REGINA: 
                        score -= transpose(queenValues, i, j);
                        break;
                    case REGE:
                        score -= transpose(kingValues, i, j);
                        break;
                }
            }
        }
    }

    
    return score;
    
}

int alphabeta_negamax(int depth, int start, ChessPlayer *player, char currentSide)
{
    if (depth == 0) {
        return evaluate(player, currentSide);
    }

    vector<validMove> allMoves = findAllMoves(player, currentSide);
    int score, i, max = -1000000009;
    bool checkMate = true;
	validMove bestMove = validMove(allMoves[0].prevRow, allMoves[0].prevColumn, allMoves[0].currentRow, allMoves[0].currentColumn);
    for (i = 0; i < allMoves.size(); i++)
    {
        Piece info = player->movePiece(allMoves[i].prevRow, allMoves[i].prevColumn, allMoves[i].currentRow, allMoves[i].currentColumn);
        if (!player->Check(currentSide)) {
            checkMate = false;
            // info are informatii despre starea anterioara a casutei pe care s-a facut mutarea
            score = (-1) * alphabeta_negamax(depth - 1, start, player, changeSide(currentSide));
            if (score > max)
            {
                max = score;
                bestMove = validMove(allMoves[i].prevRow, allMoves[i].prevColumn, allMoves[i].currentRow, allMoves[i].currentColumn);
            }
        }
        player->undoPiece(allMoves[i].currentRow, allMoves[i].currentColumn, allMoves[i].prevRow, allMoves[i].prevColumn, info);
    }

    if (checkMate == true) {
        return -P_REGE;
    }

    if (depth == start) // a trecut prin tot arborele si iteratia de miscari iar acum muta cea mai buna miscare
    {
        player->movePiece(bestMove.prevRow, bestMove.prevColumn, bestMove.currentRow, bestMove.currentColumn);
        printf("move %c%d%c%d\n",bestMove.prevColumn+'a',bestMove.prevRow+1,bestMove.currentColumn+'a',bestMove.currentRow+1);
        fflush(stdout);
    }

    return max;
}


int main()
{
    char input[STR_BUFF];
    char output[STR_BUFF];
    char com[15];
    char engine_side;
    char move[5];
    char *pos;
    ChessPlayer *player = NULL;
    bool Najdorf = false;
    bool Ruy = false;
    bool Caro = false;
    bool Scandinav = false;
    bool Alekhine = false;
    bool miniMax = false;
    bool blackFlag = false;

    // FILE *f;
    // f=fopen("testare.out","wt");

    // if(f==NULL)
    // printf("blabla");

    signal(SIGINT, SIG_IGN);
    // printf("\n");
    int i = 1;
    while (1)
    {
        // printf("# ba0\n");
        validMove *defaultMove = new validMove(0, 0, 0, 0);
        // printf("# ba0.3\n");
        fflush(stdout);
        // printf("# ba0.6\n");
        // flush(fout);
        //read input
        // printf("# ba1\n");
        if (!fgets(input, STR_BUFF, stdin))
        {
            printf("terminated\n");
            return 0;
        }
        // printf("# ba2\n");
        com[4] = '0';
        sscanf(input, "%s", com);
        // printf("# ba3\n");
        // printf("# comanda: %s\n", com);

        if (strcmp(com, "xboard") == 0)
        {
            continue;
        }
        if (strcmp(com, "new") == 0)
        {
            // printf("doi\n");
            i = 1;
            player = new ChessPlayer();
            continue;
        }
        if (strcmp(com, "protover") == 0) {
            printf("feature done=1\n");
        fflush(stdout);
            continue;
        }
        if (strcmp(com, "white") == 0)
        {
            player->engine_side = 'W';
            continue;
        }
        if (strcmp(com, "black") == 0)
        {
            player->engine_side = 'B';
            continue;
        }
        if (strcmp(com, "quit") == 0 || strcmp(com, "resign") == 0)
        {
            break;
        }
        if (strcmp(com, "go") == 0)
        {
            player->setForce(0);
            player->setGoMode(1);
        }
        if (strcmp(com, "force") == 0)
        {
            player->setForce(1);
            //player->setGoMode(0);
            continue;
        }
        if ((com[0] >= 'a' && com[0] <= 'h') && (com[1] >= '1' && com[1] <= '8') &&
            (com[2] >= 'a' && com[2] <= 'h') && (com[3] >= '1' && com[3] <= '8'))
        {
            if(!(com[4] == 'q')) {
                // flush(fout);
                player->movePiece(com[1]-'1',com[0]-'a',com[3]-'1',com[2]-'a');
            } else {
                player->promote(com[1]-'1',com[0]-'a',com[3]-'1',com[2]-'a', com[4]);
            }
		   	// flush(fout);
			// player->printTable(fout);
            blackFlag = true;

        }
        else if (player && player->engine_side == 'W' && player->go_mode==1)
        {
            if (i == 1)
            {
                // flush(fout);
                // fout << "WEmove e2e4\n";
                printf("move e2e4\n");
        fflush(stdout);
                player->movePiece(1, 4, 3, 4);
                i++;

            }
            continue;
        }
        if (player && player->engine_side == 'W' && player->go_mode == 1)
        {
            miniMax = true;
            if (miniMax == true) {
                alphabeta_negamax(4, 4, player, 'W');
            }
            // flush(fout);
            // player->printTable(fout);
        }
        if (player && player->engine_side == 'B' && player->go_mode == 1 && blackFlag)
        {
            miniMax = true;
            if (miniMax == true) {
                alphabeta_negamax(4, 4, player, 'B');
            }
			// flush(fout);
			// player->printTable(fout);
            blackFlag = false;
        }
        fflush(stdout);
    }

    // fflush(stdout);
    // flush(fout);
    // fout.close();
    return 0;
}
