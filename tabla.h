#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#ifndef TABLA_H_
#define TABLA_H_
using namespace std;
#define PION 'p'
#define TURA 't'
#define CAL 'c'
#define NEBUN 'n'
#define REGE 'R'
#define REGINA 'r'
#define NONE 'N'

char changeSide(char currentSide)
{
    if (currentSide == 'W')
        return 'B';

    return 'W';
}

struct Piece {
    char name=NONE, color=NONE;
};


class ChessPlayer {
    public:
    Piece **table;
    int rows = 8;
    bool BlackUp;
    bool force_mode = 0;
    bool go_mode= 0;
    char engine_side;
    // bool movedKing = false;
    // bool movedLTura = false;
    // bool movedRTura = false;
    vector<char> blackLost, whiteLost;

    ChessPlayer() {

    // init table

    table = new Piece*[rows];
    for (int i = 0; i < rows; ++i) {
        table[i] = new Piece[rows];
    }
    // movedKing = false;
    // movedLTura = false;
    // movedRTura = false;

    for(int i = 0; i < rows; i++) {
        table[1][i].name = PION;
        table[6][i].name = PION;

        table[1][i].color = 'W';
        table[0][i].color = 'W';
        table[6][i].color = 'B';
        table[7][i].color = 'B';
        
        for(int j = 2; j < 6; j++) {
            table[j][i].name = NONE;
            table[j][i].color = NONE;
        }
    }
    table[7][0].name = TURA;
    table[7][1].name = CAL;
    table[7][2].name = NEBUN;
    table[7][3].name = REGINA;
    table[7][4].name = REGE;
    table[7][5].name = NEBUN;
    table[7][6].name = CAL;
    table[7][7].name = TURA;

    table[0][0].name = TURA;
    table[0][1].name = CAL;
    table[0][2].name = NEBUN;
    table[0][3].name = REGINA;
    table[0][4].name = REGE;
    table[0][5].name = NEBUN;
    table[0][6].name = CAL;
    table[0][7].name = TURA;
    }

    ~ChessPlayer() {
        for (int i = 0; i < rows; ++i) {
            delete table[i];
        }
        delete[] table;
    }

    static string getCoords(int a, int b) {
        char first = '1' + a, second = 'a' + b;
        string returnable;
        returnable += first;
        returnable += ' ';
        returnable += second;
        return returnable;
    }

    Piece movePiece(int x1, int y1, int x2, int y2) {
        Piece previous;
        if (table[x1][y1].name == REGE && (y1 - y2 == 2)) {
            table[x2][y2].name = REGE;
            table[x2][y2].color = table[x1][y1].color;
            table[x1][y1].name = NONE;
            table[x1][y1].color = NONE;

            table[x2][y2 + 1].name = TURA;
            table[x2][y2 + 1].color = table[x2][y2 - 2].color;
            table[x2][y2 - 2].name = NONE;
            table[x2][y2 - 2].color = NONE;
        } else if (table[x1][y1].name == REGE && (y1 - y2 == -2)) {
            table[x2][y2].name = REGE;
            table[x2][y2].color = table[x1][y1].color;
            table[x1][y1].name = NONE;
            table[x1][y1].color = NONE;

            table[x2][y2 - 1].name = TURA;
            table[x2][y2 - 1].color = table[x2][y2 + 1].color;
            table[x2][y2 + 1].name = NONE;
            table[x2][y2 + 1].color = NONE;
        } else {
            previous.color=table[x2][y2].color;
            previous.name=table[x2][y2].name;

            if (table[x2][y2].color == 'W') {
                whiteLost.push_back(table[x2][y2].name);
            } else if (table[x2][y2].color == 'B') {
                blackLost.push_back(table[x2][y2].name);
            }
			
			if (table[x1][y1].name == PION && abs(y2 - y1) > 0 && table[x2][y2].color == NONE) {
				if (table[x1][y1].color == 'B') {
					whiteLost.push_back(table[x2 + 1][y2].name);
					table[x2 + 1][y2].name=NONE;     //sterge piesele vechi
					table[x2 + 1][y2].color=NONE;
				} else if (table[x1][y1].color == 'W') {
					blackLost.push_back(table[x2 - 1][y2].name);		
					table[x2 - 1][y2].name=NONE;     //sterge piesele vechi
					table[x2 - 1][y2].color=NONE;
				}
			}
			
            table[x2][y2].name = table[x1][y1].name;
            table[x2][y2].color = table[x1][y1].color;

            table[x1][y1].name=NONE;     //sterge piesele vechi
            table[x1][y1].color=NONE;
        }
        return previous;
    }

    // Piece movePiece(int x1, int y1, int x2, int y2) {
    //     Piece previous;
    //         // retine informatiile despre casuta pe care se muta piesa
    //         // e folositor cand trebuie sa facem undo la miscare in alphabeta
    //         // previous[0] CULOARE
    //         // previous[1] NUME PIESA
    //     if (table[x1][y1].name == REGE && (y1 - y2 == 2)) {
    //         table[x2][y2].name = REGE;
    //         table[x2][y2].color = table[x1][y1].color;
    //         table[x1][y1].name = NONE;
    //         table[x1][y1].color = NONE;

    //         table[x2][y2 + 1].name = TURA;
    //         table[x2][y2 + 1].color = table[x2][y2 - 2].color;
    //         table[x2][y2 - 2].name = NONE;
    //         table[x2][y2 - 2].color = NONE;
    //     } else if (table[x1][y1].name == REGE && (y1 - y2 == -2)) {
    //         table[x2][y2].name = REGE;
    //         table[x2][y2].color = table[x1][y1].color;
    //         table[x1][y1].name = NONE;
    //         table[x1][y1].color = NONE;

    //         table[x2][y2 - 1].name = TURA;
    //         table[x2][y2 - 1].color = table[x2][y2 + 1].color;
    //         table[x2][y2 + 1].name = NONE;
    //         table[x2][y2 + 1].color = NONE;
    //     } else {
    //         previous.color=table[x2][y2].color;
    //         previous.name=table[x2][y2].name;
            
    //         // if (table[x1][y1].name == REGE) {
    //         //     movedKing = true;
    //         // }

    //         if (table[x2][y2].color == 'W') {
    //             whiteLost.push_back(table[x2][y2].name);
    //         } else if (table[x2][y2].color == 'B') {
    //             blackLost.push_back(table[x2][y2].name);
    //         }

    //         table[x2][y2].name = table[x1][y1].name;
    //         table[x2][y2].color = table[x1][y1].color;

    //         table[x1][y1].name=NONE;     //sterge piesele vechi
    //         table[x1][y1].color=NONE;
    //     }

    //     return previous;
    // }
    void undoPiece(int x1,int y1,int x2,int y2, Piece info)  //info[0] are culoarea info[1] are nume piesa x1/y1 coordonate start
    {
        table[x2][y2].name = table[x1][y1].name;
        table[x2][y2].color = table[x1][y1].color;
        table[x2][y2].name = table[x1][y1].name;
        table[x2][y2].color = table[x1][y1].color;

        table[x1][y1].color=info.color;  // reface starea anterioara
        table[x1][y1].name=info.name;

        if (info.color == 'W') {
            whiteLost.pop_back();
        } else if (info.color == 'B') {
            blackLost.pop_back();
        }
    }

    Piece promote(int x1, int y1, int x2, int y2, char piece) {
        Piece previous;
            // retine informatiile despre casuta pe care se muta piesa
            // e folositor cand trebuie sa facem undo la miscare in alphabeta
            // previous[0] CULOARE
            // previous[1] NUME PIESA

        previous.color=table[x2][y2].color;
        previous.name=table[x2][y2].name;

        if (table[x2][y2].color == 'W') {
            whiteLost.push_back(table[x2][y2].name);
        } else if (table[x2][y2].color == 'B') {
            blackLost.push_back(table[x2][y2].name);
        }

        table[x2][y2].name = table[x1][y1].name;
        table[x2][y2].color = table[x1][y1].color;

        if (piece == 'q') {
            table[x2][y2].name = REGINA;
        }

        table[x1][y1].name=NONE;     //sterge piesele vechi
        table[x1][y1].color=NONE;

        return previous;
    }

    Piece getPiece(int x, int y) {
        return table[x][y];
    }

      bool checkPion(int x, int y, char color) {
        if (color == 'W' && x < 6) { 
            if (y > 0) {
                if (table[x + 1][y - 1].color == 'B' && table[x + 1][y - 1].name == 'p') {
                    return true;
                }
            }  
            if (y < 7) {
                if (table[x + 1][y + 1].color == 'B' && table[x + 1][y + 1].name == 'p') {
                    return true;
                }
            }
        } else if (color == 'B' && x > 1) {
            if (y > 0) {
                if (table[x - 1][y - 1].color == 'W' && table[x - 1][y - 1].name == 'p') {
                    return true;
                }
            }  
            if (y < 7) {
                if (table[x - 1][y + 1].color == 'W' && table[x - 1][y + 1].name == 'p') {
                    return true;
                }
            }
        }
        return false;
    }

 

    bool checkCal(int x, int y, char color) {
        char enemySide = changeSide(color);
        if (x > 1) {
            if (y > 0) {
                if (table[x - 2][y - 1].color == enemySide && table[x - 2][y - 1].name == CAL) {
                    return true;
                }
            }
            if (y < 7) {
                if (table[x - 2][y + 1].color == enemySide && table[x - 2][y + 1].name == CAL) {
                    return true;
                }
            }
        }
        if (x < 6) {
            if (y > 0) {
                if (table[x + 2][y - 1].color == enemySide && table[x + 2][y - 1].name == CAL) {
                    return true;
                }
            }
            if (y < 7) {
                if (table[x + 2][y + 1].color == enemySide && table[x + 2][y + 1].name == CAL) {
                    return true;
                }
            }
        }
        if (y > 1) {
            if (x > 0) {
                if (table[x - 1][y - 2].color == enemySide && table[x - 1][y - 2].name == CAL) {
                    return true;
                }
            }
            if (x < 7) {
                if (table[x + 1][y - 2].color == enemySide && table[x + 1][y - 2].name == CAL) {
                    return true;
                }
            }
        }
        if (y < 6) {
            if (x > 0) {
                if (table[x - 1][y + 2].color == enemySide && table[x - 1][y + 2].name == CAL) {
                    return true;
                }
            }
            if (x < 7) {
                if (table[x + 1][y + 2].color == enemySide && table[x + 1][y + 2].name == CAL) {
                    return true;
                }
            }
        }
        return false;
    }

 

    bool checkRege(int x, int y, char color) {
        char enemySide = changeSide(color);
        if (x > 0) {
            if (y > 0) {
                if (table[x - 1][y - 1].color == enemySide && table[x - 1][y - 1].name == REGE) {
                    return true;
                }
            }
            if (table[x - 1][y].color == enemySide && table[x - 1][y].name == REGE) {
                    return true;
            }
            if (y < 7) {
                if (table[x - 1][y + 1].color == enemySide && table[x - 1][y + 1].name == REGE) {
                    return true;
                }
            }
        }
        if (x < 7) {
            if (y > 0) {
                if (table[x + 1][y - 1].color == enemySide && table[x + 1][y - 1].name == REGE) {
                    return true;
                }
            }
            if (table[x + 1][y].color == enemySide && table[x + 1][y].name == REGE) {
                    return true;
            }
            if (y < 7) {
                if (table[x + 1][y + 1].color == enemySide && table[x + 1][y + 1].name == REGE) {
                    return true;
                }
            }
        }
        if (y > 0) {
            if (table[x][y - 1].color == enemySide && table[x][y - 1].name == REGE) {
                return true;
            }
        }
        if (y < 7) {
            if (table[x][y + 1].color == enemySide && table[x][y + 1].name == REGE) {
                return true;
            }
        }
        return false;
    }

 

    // bool checkDiag(int x, int y, char color) {
    //     char enemySide = changeSide(color);
    //     int i = x, j = y;
    //     x = i + 1, y = j - 1;

    //     while (x < 8 && y >= 0 && table[x][y].color != color) {
    //         if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         x++;
    //         y--;
    //     }

    //     x = i + 1;
    //     y = j + 1;
    //     while (x < 8 && y < 8 && table[x][y].color != color) {
    //         if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         x++;
    //         y++;
    //     }
        

    //     x = i - 1;
    //     y = j + 1;

    //     while (x >= 0 && y < 8 && table[x][y].color != color) {
    //         if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         x--;
    //         y++;
    //     }
        

    //     x = i - 1;
    //     y = j - 1;

    //     while (x >= 0 && y >= 0 && table[x][y].color != color) {
    //         if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         x--;
    //         y--;
    //     }
    // }

 

    // bool checkLine(int x, int y, char color) {
    //     char enemySide = changeSide(color);
    //     int i = x, j = y;
    //     x = i + 1, y = j;

    //     while (x < 8 && table[x][y].color != color) {
            
    //         if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         x++;
    //     }

        
    //     x = i;
    //     y = j + 1;

    //     while (y < 8 && table[x][y].color != color) {
            
    //         if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         y++;
    //     }
        
    //     x = i - 1;
    //     y = j;
    
    //     while (x >= 0 && table[x][y].color != color) {
            
    //         if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         x--;
    //     }
        
    //     x = i;
    //     y = j - 1;

    //     while (y >= 0 && table[x][y].color != color) {
            
    //         if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
    //             return true;
    //         }
    //         y--;
    //     }
        
    //     return false;
    // }

    bool checkDiag(int x, int y, char color) {
        char enemySide = changeSide(color);
        int i = x, j = y;
        x = i + 1, y = j - 1;
        while (x < 7 && y >= 1 && table[x][y].color == NONE) {
            x++;
            y--;
        }

 
		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
				return true;
			}
		}

        x = i + 1;
        y = j + 1;

        while (x < 7 && y < 7 && table[x][y].color == NONE) {
            x++;
            y++;
        }
		
		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
				return true;
			}
		}

        x = i - 1;
        y = j + 1;  

        while (x >= 1 && y < 7 && table[x][y].color == NONE) {
            x--;
            y++;
        }
		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
				return true;
			}
		}

        x = i - 1;
        y = j - 1;

		
        while (x >= 1 && y >= 1 && table[x][y].color == NONE) {
            x--;
            y--;
        }
		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == NEBUN || table[x][y].name == REGINA)) {
				return true;
			}
		}
        return false;   
    }

 

    bool checkLine(int x, int y, char color) {
        char enemySide = changeSide(color);
        int i = x, j = y;
        x = i + 1, y = j;

        while (x < 8 && table[x][y].color == NONE) {
            x++;
        }

		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
				return true;
			}
		}

        x = i;
        y = j + 1;

        while (y < 8 && table[x][y].color == NONE) {
            y++;
        }
		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
				return true;
			}
		}

        x = i - 1;
        y = j;

        while (x >= 0 && table[x][y].color == NONE) {
            x--;
        }
		
		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
				return true;
			}
		}

        x = i;
        y = j - 1;

        while (y >= 0 && table[x][y].color == NONE) {
            y--;
        }
		if( (0 <= x && x <= 7) && (0 <= y && y <= 7)) {
			if (table[x][y].color == enemySide && (table[x][y].name == TURA || table[x][y].name == REGINA)) {
				return true;
			}
		}
        return false;
    }

 

    bool isAttackedMate(int x, int y, char color) {
        // check pion
        return (checkPion(x, y, color) &&
            checkCal(x, y, color) &&
            checkDiag(x, y, color) &&
            checkLine(x, y, color) &&
            checkRege(x, y, color));
    }

    bool isAttacked(int x, int y, char color) {
        // check pion
        return (
            checkPion(x, y, color) ||
            checkCal(x, y, color) ||
            checkDiag(x, y, color) ||
            checkLine(x, y, color) ||
            checkRege(x, y, color));
    }

    bool CheckMate(char color) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (table[i][j].name == REGE && table[i][j].color == color) {
                    return isAttackedMate(i, j, color);
                }
            }
        }
    }

    bool Check(char color) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (table[i][j].name == REGE && table[i][j].color == color) {
                    return isAttacked(i, j, color);
                }
            }
        }
    }

    void setForce(bool toSet) {
        force_mode = toSet;
    }
    void setGoMode(bool toSet)
    {
        go_mode=toSet;
    }

    bool validMove(char* com) {
        //TODO
        return 1;
    }

   
    void printTable(ofstream &fout){
        fout << endl;
        for(int i = 0; i < rows; i++) {
            fout << "        ";
            for(int j = 0; j < rows; j++){
                fout << table[rows - i - 1][j].name << " " ;
            }
            fout << endl;
        }
    }
   


};
#endif