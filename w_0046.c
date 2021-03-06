#include <stdio.h>
#include <string.h>

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

#define BLACK 0
#define WHITE 1

#define VIRGIN 16

#define EMPTY -1

int piecescore[6] = { 100, 280,  320, 479, 929,  100000 };
int pqst[6][64] = {
     	 {   0,   0,   0,   0,   0,   0,   0,   0,
            78,  83,  86,  73, 102,  82,  85,  90,
             7,  29,  21,  44,  40,  31,  44,   7,
           -17,  16,  -2,  15,  14,   0,  15, -13,
           -26,   3,  10,   9,   6,   1,   0, -23,
           -22,   9,   5, -11, -10,  -2,   3, -19,
           -31,   8,  -7, -37, -36, -14,   3, -31,
             0,   0,   0,   0,   0,   0,   0,   0},
         { -66, -53, -75, -75, -10, -55, -58, -70,
            -3,  -6, 100, -36,   4,  62,  -4, -14,
            10,  67,   1,  74,  73,  27,  62,  -2,
            24,  24,  45,  37,  33,  41,  25,  17,
            -1,   5,  31,  21,  22,  35,   2,   0,
           -18,  10,  13,  22,  18,  15,  11, -14,
           -23, -15,   2,   0,   2,   0, -23, -20,
           -74, -23, -26, -24, -19, -35, -22, -69},
         { -59, -78, -82, -76, -23,-107, -37, -50,
           -11,  20,  35, -42, -39,  31,   2, -22,
            -9,  39, -32,  41,  52, -10,  28, -14,
            25,  17,  20,  34,  26,  25,  15,  10,
            13,  10,  17,  23,  17,  16,   0,   7,
            14,  25,  24,  15,   8,  25,  20,  15,
            19,  20,  11,   6,   7,   6,  20,  16,
            -7,   2, -15, -12, -14, -15, -10, -10},
         {  35,  29,  33,   4,  37,  33,  56,  50,
            55,  29,  56,  67,  55,  62,  34,  60,
            19,  35,  28,  33,  45,  27,  25,  15,
             0,   5,  16,  13,  18,  -4,  -9,  -6,
           -28, -35, -16, -21, -13, -29, -46, -30,
           -42, -28, -42, -25, -25, -35, -26, -46,
           -53, -38, -31, -26, -29, -43, -44, -53,
           -30, -24, -18,   5,  -2, -18, -31, -32},
	 {   6,   1,  -8,-104,  69,  24,  88,  26,
            14,  32,  60, -10,  20,  76,  57,  24,
            -2,  43,  32,  60,  72,  63,  43,   2,
             1, -16,  22,  17,  25,  20, -13,  -6,
           -14, -15,  -2,  -5,  -1, -10, -20, -22,
           -30,  -6, -13, -11, -16, -11, -16, -27,
           -36, -18,   0, -19, -15, -15, -21, -38,
           -39, -30, -31, -13, -31, -36, -34, -42},
 	 {   4,  54,  47, -99, -99,  60,  83, -62,
           -32,  10,  55,  56,  56,  55,  10,   3,
           -62,  12, -57,  44, -67,  28,  37, -31,
           -55,  50,  11,  -4, -19,  13,   0, -49,
           -55, -43, -52, -28, -51, -47,  -8, -50,
           -47, -42, -43, -79, -64, -32, -29, -32,
            -4,   3, -14, -50, -57, -18,  13,   4,
            17,  30,  -3, -14,   6,  -1,  40,  18}
};

//Below three functions for printing the board

void piece2character(int code){
	char *character_string = "pnbrqk";
	if (code == EMPTY){
		putchar(' ');
		return;
	}
	int type = (code >> 1) & 7;
	putchar(character_string[type] + ((code & 1) ? ('A' - 'a') : 0));
}

void print_line(){
	int i;
	printf("  ");
	for (i=0; i<8; ++i){
		printf("+---");
	}
	printf("+\n");
}

void print_board(int *board){
	int i;
	for (i=0; i<8; ++i){
		print_line();
		int j;
		printf("%d ", 8-i);
		for (j=0; j<8; ++j){
			printf("+ ");
			piece2character(board[i*8 + j]);
			printf(" ");
		}
		printf("+");
		printf("\n");
	}
	print_line();
	printf("    a   b   c   d   e   f   g   h\n");
}

// Uploading fen to board

void put_fen(int *board, char *fen){
	int i;
	i=0;
	while (1){
		if (fen[i] == ' '){
			break;
		}
		if 
		(
			('a' < fen[i] && fen[i] < 'z') ||
			('A' < fen[i] && fen [i] <'Z')
		){
			int index=i;
			int rank = 0;
			while (1){
				if (index == 0) break;
				--index;
				if (fen[index] == '/'){
					++rank;
				}
			}
			int file = 0;
			index = i;
			while (1){
				if (index == 0) break;
				--index;
				if (fen[index] == '/') break;
				if 
				(
					('a' < fen[index] && fen[index] < 'z') ||
					('A' < fen[index] && fen[index] < 'Z')
				){
					++file;
				}
				else if ('0' < fen[index] && fen[index] < '9'){
					file += fen[index] - '0';
				}
			}
			int piece;
			int type = fen[i];
			int color = BLACK;
			if ('A' < type && type < 'Z'){ type += ('a' - 'A'); color = WHITE; }
			switch (type){
				case 'p': piece = PAWN; break;
				case 'r': piece = ROOK; break;
				case 'n': piece = KNIGHT; break;
				case 'b': piece = BISHOP; break;
				case 'q': piece = QUEEN; break;
				case 'k': piece = KING; break;
			}
			int pieceType = piece;
			piece <<= 1;
			piece |= color;
			if (pieceType == KING){
				board[65 + color] = rank * 8 + file;
			}
			if (pieceType == KING || pieceType == ROOK){
				piece |= VIRGIN;
			}
			board[rank * 8 + file] = piece;
		}
		++i;
	}
}

#define B(x) (((7 - (((x) >> 3) & 7)) << 3) | ((x) & 7))

//Socring function

int scoreval(int *board){
    int value=0;
    int rank, file;
    for (rank=0; rank < 8; ++rank){
	    for (file =0; file < 8; ++file){
		    int i;
		    int piece = board[rank * 8 + file];
		    if (piece == EMPTY) continue;
		    if ((piece & 1) == BLACK){
			    value -= piecescore[(piece >> 1) & 7];
			    value -= pqst[(piece >> 1) & 7][(7 - rank) * 8 + file];
		    } else {
			    value += piecescore[(piece >> 1) & 7];
			    value += pqst[(piece >> 1) & 7][rank * 8 + file];
		    }
	    }
    }
    return value;
}

//Checking if a one piece gets moved outside the board
// a is the position of the piece and b is the number of ranks/files moves

int sc(int a, int b){
	if (b == 0){
		return 1;
	} else if (b > 0){
		return ((a + b) < 8);
	} else {
		return ((a + b) >= 0);
	}
}

//If the piece doesnot fall off and both rank and file are in range it is ok

int outside(int rank, int file, int r, int f){
	return sc(rank, r) && sc(file, f);
}

//Direct move generation for sliding pieces

#define playundo(r, f) \
	if (outside(rank, file, r, f)){ \
		t1 = rank * 8 + file; \
		int t2 = board[t1 + (r * 8) + f]; \
		if (t2 != EMPTY && (t2 & 1) == !color){ \
			extend = 1; \
		} \
		if (t2 == EMPTY || (t2 != EMPTY && (t2 & 1) == !color)){ \
			t3 = board[t1]; \
			if (((board[t1] >> 1) & 7) == KING){ \
				board[65 + color] = t1 + (r * 8) + f; \
				board[t1] &= ~ VIRGIN; \
			} \
			board[t1 + (r * 8) + f]  = board[t1]; \
			board[t1] = EMPTY; \
			start = t1; \
			end = t1 + (r * 8) + f; \
			addv = (color == BLACK ? -1 : 1) * (pqst[(t3 >> 1) & 7][color == BLACK ? B(end) : end] - \
							    pqst[(t3 >> 1) & 7][color == BLACK ? B(start) : start]); \
			if (extend){ \
				addv -= (color == BLACK ? 1 : -1) * (piecescore[(t2 >> 1) & 7] + \
					pqst[(t2 >> 1) & 7][color == BLACK ? end : B(end)]); \
			} \
			SEARCH(); \
			extend = 0; \
			board[t1] = t3; \
			board[t1 + (r * 8) + f] = t2; \
			if (((board[t1] >> 1) & 7) == KING){ \
				board[65 + color] = t1; \
			} \
			ABC(); \
		} \
	}

//Move generation by increasing ranges slowly one by one for sliding pieces

//The while loop does the moves for the sliding pieces
//The below if conditions checks if a piece can be eaten and an extra range is there or not

#define pu2(r, f) \
	x = r; \
	y = f; \
	t1 = rank * 8 + file; \
	while (outside(rank, file, x, y) && board[t1 + (x * 8) + y] == EMPTY){ \
		t3 = board[t1]; \
		if (((board[t1] >> 1) & 7) == ROOK){ \
                        board[t1] &= ~VIRGIN; \
                } \
		board[t1 + (x * 8) + y] = board[t1]; \
		board[t1] = EMPTY; \
		start = t1; \
        	end = t1 + (x * 8) + y; \
		addv = (color == BLACK ? -1 : 1) * (pqst[(t3 >> 1) & 7][color == BLACK ? B(end) : end] - \
                                                    pqst[(t3 >> 1) & 7][color == BLACK ? B(start) : start]); \
		SEARCH(); \
		board[t1] = t3; \
		board[t1 + (x * 8) + y] = EMPTY; \
		ABC(); \
		x += r; \
		y += f; \
	} \
	t2 = board[t1 + (x * 8) + y]; \
	if (outside(rank, file, x, y) && t2 != EMPTY && (t2 & 1) == !color){ \
		extend = 1; \
		t3 = board[t1]; \
		if (((board[t1] >> 1) & 7) == ROOK){ \
                        board[t1] &= ~VIRGIN; \
                } \
		board[t1 + (x * 8) + y] = board[t1]; \
		board[t1] = EMPTY; \
		start = t1; \
                end = t1 + (x * 8) + y; \
		addv = (color == BLACK ? -1 : 1) * (pqst[(t3 >> 1) & 7][color == BLACK ? B(end) : end] - \
						    pqst[(t3 >> 1) & 7][color == BLACK ? B(start) : start]); \
		addv -= (color == BLACK ? 1 : -1) * (piecescore[(t2 >> 1) & 7] + \
                                                        pqst[(t2 >> 1) & 7][color == BLACK ? end : B(end)]); \
		SEARCH(); \
		extend = 0; \
		board[t1] = t3; \
		board[t1 + (x * 8) + y] = t2; \
		ABC(); \
	}

#define INFINITY 1000000

struct LINE {
	int cmove;
	int argmove[100];
};

//Our negamax recursion

#define SEARCH() tmp = -negamax(board, extend && depth == 1 ? 1 :depth - 1, \
				-beta, -alpha, !color, score + addv, &line)
//		 if (tmp > score) {pv[depth] = (start << 6) | end; score = tmp;}

#define ABC() if (tmp >= beta) { return beta; } \
	      if (tmp > alpha){ alpha = tmp; pline -> argmove[0] = (start << 6) | end; \
	      memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(int)); \
	      pline -> cmove = line.cmove + 1;}

//The function for moves generation

int negamax(int *board, int depth, int alpha, int beta, int color, int score, struct LINE *pline){
	int t1, t2, t3, t4, t5, x, y;
	struct LINE line;
	if (depth == 0 || (board[board[65 + color]] & 15) != ((KING << 1) | color)){
		pline ->cmove = 0;
		int t5 = (color == board[64] ? 1 : -1)  * score * (board[64] == WHITE ? 1 : -1);
		//printf(" !%d! ", t5);
		return t5;
	}
	
	int tmp;

	int extend=0;
	int addv;
	int rank, file;
	int value;
	int start, end;
	//Iterate through the whole board
	if (color == BLACK){
		if (board[4] == (VIRGIN | (KING << 1) | BLACK) &&
		    board[7] == (VIRGIN | (ROOK << 1) | BLACK) &&
		    board[5] == EMPTY && board[6] == EMPTY){
			board[6] = (KING << 1) | BLACK;
			board[5] = (ROOK << 1) | BLACK;
			board[4] = board[7] = EMPTY;
			start = 4;
			end = 6;
			board[65 + BLACK] = 6;
			addv = -1 * (pqst[KING][B(end)] - pqst[KING][B(start)]); \
			addv += -1 * (pqst[ROOK][B(5)] - pqst[ROOK][B(7)]); \
			SEARCH();
			board[65 + BLACK] = 4;
			board[4] = VIRGIN | (KING << 1) | BLACK;
			board[7] = VIRGIN | (ROOK << 1) | BLACK;
			board[5] = board[6] = EMPTY;
			ABC();
		}
		if (board[4] == (VIRGIN | (KING << 1) | BLACK) &&
                    board[0] == (VIRGIN | (ROOK << 1) | BLACK) &&
                    board[1] == EMPTY && board[2] == EMPTY && board[3] == EMPTY){
                        board[2] = (KING << 1) | BLACK;
                        board[3] = (ROOK << 1) | BLACK;
                        board[0] = board[4] = EMPTY;
			start = 4;
			end = 2;
			board[65 + BLACK] = 2;
			addv = -1 * (pqst[KING][B(end)] - pqst[KING][B(start)]); \
                        addv += -1 * (pqst[ROOK][B(3)] - pqst[ROOK][B(0)]); \
                        SEARCH();
			board[65 + BLACK] = 4;
                        board[4] = VIRGIN | (KING << 1) | BLACK;
                        board[0] = VIRGIN | (ROOK << 1) | BLACK;
                        board[3] = board[2] = EMPTY;
                        ABC();
                }
	} else {
		if (board[60] == (VIRGIN | (KING << 1) | WHITE) &&
                    board[63] == (VIRGIN | (ROOK << 1) | WHITE) &&
                    board[61] == EMPTY && board[62] == EMPTY){
                        board[62] = (KING << 1) | WHITE;
                        board[61] = (ROOK << 1) | WHITE;
                        board[60] = board[63] = EMPTY;
			start = 60;
			end = 62;
			board[65 + WHITE] = 62;
			addv = (pqst[KING][end] - pqst[KING][start]); \
                        addv += pqst[ROOK][61] - pqst[ROOK][63]; \
                        SEARCH();
			board[65 + WHITE] = 60;
                        board[60] = VIRGIN | (KING << 1) | WHITE;
                        board[63] = VIRGIN | (ROOK << 1) | WHITE;
                        board[61] = board[62] = EMPTY;
                        ABC();
                }
		if (board[60] == (VIRGIN | (KING << 1) | BLACK) &&
                    board[56] == (VIRGIN | (ROOK << 1) | BLACK) &&
                    board[57] == EMPTY && board[58] == EMPTY && board[59] == EMPTY){
                        board[58] = (KING << 1) | BLACK;
                        board[59] = (ROOK << 1) | BLACK;
                        board[60] = board[56] = EMPTY;
			start = 60;
			end = 58;
			board[65 + WHITE] = 58;
			addv = (pqst[KING][end] - pqst[KING][start]); \
                        addv += pqst[ROOK][59] - pqst[ROOK][56]; \
                        SEARCH();
			board[65 + WHITE] = 60;
                        board[60] = VIRGIN | (KING << 1) | BLACK;
                        board[56] = VIRGIN | (ROOK << 1) | WHITE;
                        board[58] = board[59] = EMPTY;
                        ABC();
                }
	}
	for (rank = 0; rank < 8; ++rank){
		for (file = 0; file < 8; ++file){
			//printf(" .%d. ", rank * 8 + file);
			if (board[rank * 8 + file] == EMPTY) continue;
			int pieceType = (board[rank * 8 + file] >> 1) & 7;
			//Generate pawn moves
			if (pieceType == PAWN && (board[rank * 8 + file] & 1) == color){
				int a = rank * 8 + file;
				start = a;
				//Check colors of the pawns because they are different directions
				if (color == BLACK){
					if (board[a + 8] == EMPTY){
						//Single move forward
						board[a + 8] = board[a];
						board[a] = EMPTY;
						end = a + 8;
						addv = -1 * (pqst[PAWN][B(end)] - pqst[PAWN][B(start)]);
						SEARCH();
						board[a] = board[a + 8];
						board[a + 8] = EMPTY;
						ABC();
						//Double move forward
						if (board[a + 16] == EMPTY && rank == 1){
							board[a + 16] = board[a];
							board[a] = EMPTY;
							end = a + 16;
							addv = -1 * (pqst[PAWN][B(end)] - pqst[PAWN][B(start)]);
							SEARCH();
							board[a] = board[a + 16];
							board[a + 16] = EMPTY;
							ABC();
						}
					}
					//Eating the pieces
					if (outside(rank, file, 1, 1) \
					    && board[a + 9] != EMPTY && (board[a + 9] & 1) == !color){
						extend = 1;
						value = board[a + 9];
						board[a + 9] = board[a];
						board[a] = EMPTY;
						end = a + 9;
						addv = -1 * (pqst[PAWN][B(end)] - pqst[PAWN][B(start)]);
						addv -= (piecescore[(value >> 1) & 7] + pqst[(value >> 1) & 7][end]);
						SEARCH();
						extend = 0;
						board[a] =  board[a + 9];
						board[a + 9] = value;
						ABC();
					}
					//Eating the pieces on the other side
					if (outside(rank, file, 1, -1) \
					    && board[a + 7] != EMPTY && (board[a + 7] & 1) == !color){
						extend = 1;
                                                value = board[a + 7];
                                                board[a + 7] = board[a];
                                                board[a] = EMPTY;
						end = a + 7;
						addv = -1 * (pqst[PAWN][B(end)] - pqst[PAWN][B(start)]);
						addv -= (piecescore[(value >> 1) & 7] + pqst[(value >> 1) & 7][end]);
                                                SEARCH();
						extend = 0;
                                                board[a] =  board[a + 7];
                                                board[a + 7] = value;
						ABC();
                                        }
				} else {
					//Same stuff in the other direction in the other color
					if (board[a - 8] == EMPTY){
                                                board[a - 8] = board[a];
                                                board[a] = EMPTY;
						end = a - 8;
						addv = (pqst[PAWN][end] - pqst[PAWN][start]);
                                                SEARCH();
                                                board[a] = board[a - 8];
                                                board[a - 8] = EMPTY;
						ABC();
                                                if (board[a - 16] == EMPTY && rank == 6){
                                                        board[a - 16] = board[a];
                                                        board[a] = EMPTY;
							end = a - 16;
							addv = (pqst[PAWN][end] - pqst[PAWN][start]);
                                                       	SEARCH();
                                                        board[a] = board[a - 16];
                                                        board[a - 16] = EMPTY;
							ABC();
                                                }
                                        }
                                        if (outside(rank, file, -1, -1) \
					    && board[a - 9] != EMPTY && (board[a - 9] & 1) == !color){
						extend = 1;
                                                value = board[a - 9];
                                                board[a - 9] = board[a];
                                                board[a] = EMPTY;
						end = a - 9;
						addv = (pqst[PAWN][end] - pqst[PAWN][start]);
						addv -= -1 * (piecescore[(value >> 1) & 7] +
								pqst[(value >> 1) & 7][B(end)]);
                                                SEARCH();
						extend = 0;
                                                board[a] =  board[a - 9];
                                                board[a - 9] = value;
						ABC();
                                        }
                                        if (outside(rank, file, -1, 1) \
					    && board[a - 7] != EMPTY && (board[a - 7] & 1) == !color){
						extend = 1;
                                                value = board[a - 7];
                                                board[a - 7] = board[a];
                                                board[a] = EMPTY;
						end = a - 7;
						addv = (pqst[PAWN][end] - pqst[PAWN][start]);
						addv -= -1 * (piecescore[(value >> 1) & 7] +
                                                                pqst[(value >> 1) & 7][B(end)]);
                                                SEARCH();
						extend = 0;
                                                board[a] =  board[a - 7];
                                                board[a - 7] = value;
						ABC();
                                        }
				}
					
			}
			//Generate according to the given direction
			else if (pieceType == KNIGHT && (board[rank * 8 + file] & 1) == color){
				playundo(+1, +2);
				playundo(+1, -2);
				playundo(-1, +2);
				playundo(-1, -2);
				playundo(+2, +1);
				playundo(+2, -1);
				playundo(-2, +1);
				playundo(-2, -1);
			}
			else if (pieceType == ROOK && (board[rank * 8 + file] & 1) == color){
				pu2(+1, 0);
				//printf(" @ ");
				pu2(-1, 0);
				//printf(" ! ");
				pu2(0, +1);
				pu2(0, -1);
			}
			else if (pieceType == BISHOP &&(board[rank * 8 + file] & 1) == color){
				pu2(+1, -1);
				pu2(+1, +1);
				pu2(-1, -1);
				pu2(-1, +1);
			}
			else if (pieceType == QUEEN && (board[rank * 8 + file] & 1) == color){
				pu2(+1, 0);
                                pu2(-1, 0);
                                pu2(0, +1);
                                pu2(0, -1);
				pu2(+1, -1);
                                pu2(+1, +1);
                                pu2(-1, -1);
                                pu2(-1, +1);
			}
			else if (pieceType == KING && (board[rank * 8 + file] & 1) == color){
				playundo(+1, 0);
				//printf(" %d ", board[60]);
                                playundo(-1, 0);
				//printf(" %d ", board[60]);
                                playundo(0, +1);
				//printf(" %d ", board[60]);
                                playundo(0, -1);
				//printf(" %d ", board[60]);
                                playundo(+1, +1);
				//printf(" %d ", board[60]);
                                playundo(+1, -1);
				//printf(" %d ", board[60]);
                                playundo(-1, +1);
                                playundo(-1, -1);
			}
		}
	}
	
	return alpha; // Negamax stuff return the best possible node
}

void clear_pv(struct LINE *line){
	line->cmove = 0;
	int i;
	for (i=0; i<100; ++i) line->argmove[i] = 0;
}

void clear_board(int *board){
	int i;
	for (i=0; i<67; ++i) board[i] = EMPTY;
}

void reset_board(int *board){
	clear_board(board);
	put_fen(board, "1rb5/1p2k2r/p5n1/2p1pp2/2B5/6P1/PPPB1PP1/2KR4 ");
	//put_fen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ");
}

void set_fen(int *board){
	char ary[100] = {0};
	scanf("%s", ary);
	int size = strlen(ary);
	ary[size] = ' ';
	ary[size + 1] = '\0';
	put_fen(board, ary);
}

void think(int *board, int depth, int color, struct LINE *line){
	int i;
	board[64] = color;
	//printf(" %d ", board[60]);
	int score = negamax(board, depth, -INFINITY, +INFINITY, color, scoreval(board), line);
	//printf(" %d ", board[60]);
	for (i=0; i<line->cmove; ++i){
                int a = line->argmove[i] >> 6;
                int b = line->argmove[i] & ((1 << 6) - 1);
                printf("%c%c%c%c ", (a & 7) + 'a', 8 - (a >> 3) + '0', (b & 7) + 'a', 8 - (b >> 3) + '0');
        }
	printf("\n%d\n", score);
}

void edit_board(int *board){
	char ary[100];
	scanf("%s", ary);
	int x = (ary[0] - 'a') + 8 * (7 - (ary[1] - '0' - 1));
	int y = (ary[2] - 'a') + 8 * (7 - (ary[3] - '0' - 1));
	printf("%d %d \n", x, y);
	board[y] = board[x];
	board [x] = EMPTY;
}

void playpv(int *board, struct LINE *line){
	int tmp = line->argmove[0];
	int x = (tmp >> 6);
	int y = (tmp & ((1 << 6) - 1));
	if (x == 4 && board[x] == (VIRGIN | (KING << 1) | BLACK)){
		if (y == 6){
			board[5] = board[7];
			board[7] = EMPTY;
		}
		else if (y == 2){
			board[3] = board[0];
			board[0] = EMPTY;
		}
	}
	else if (x == 60 && board[x] == (VIRGIN | (KING << 1) | WHITE)){
		if (y == 62){
			board[61] = board[63];
			board[63] = EMPTY;
		}
		else if (y == 58){
			board[59] = board[56];
			board[56] = EMPTY;
		}
	}
	board[y] = board[x];
	board[x] = EMPTY;
}

int main (){
	int board[67] = {0};
	struct LINE line;
	clear_pv(&line);
	reset_board(board);
	print_board(board);
	char a, b;
	while(1){
		scanf("%c%c", &a, &b);
		if (a == 'w'){
			clear_pv(&line);
			//reset_board(board);
			think(board, b - '0', WHITE, &line);
		}
		else if (a == 'b'){
			clear_pv(&line);
			think(board, b - '0', BLACK, &line);
		}
		else if (a == 'f'){
			set_fen(board);
		}
		else if (a == 'x'){
			edit_board(board);
		}
		else if (a == 'c'){
			clear_pv(&line);
			reset_board(board);
		}
		else if (a == 'e'){
			clear_pv(&line);
			clear_board(board);
		}
		else if (a == 'p'){
			print_board(board);
		}
		else if (a == 'q'){
			break;
		}
		else if (a == 'y'){
			clear_pv(&line);
		}
		else if (a == 'z'){
			int i;
			clear_pv(&line);
			print_board(board);
			for (i=0; i<100; ++i){
				think(board, b - '0', WHITE, &line);
				playpv(board, &line);
				//print_board(board);
				clear_pv(&line);
				think(board, b - '0', BLACK, &line);
				playpv(board, &line);
				//print_board(board);
				clear_pv(&line);
			}
		}
		else if (a == 'h'){
			playpv(board, &line);
		}

	}
	return 0;
}
				
