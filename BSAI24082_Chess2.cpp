#include<iostream>
#include<math.h>
#include<fstream>
#include<conio.h>
#include<windows.h>
using namespace std;
#define GRID_SIZE 8
#define ROWS 100
#define COLS 100
enum COLOR {
	BLACK,WHITE
};
struct piece {
	char type;
	COLOR color;
	bool (*islegal)(piece*[][GRID_SIZE], int, int, int, int) {};

};
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
		ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState ==

			FROM_LEFT_1ST_BUTTON_PRESSED)

		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void color(int textColor, int backgroundColor = 0) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD color = (backgroundColor << 4) | (textColor & 0x0F);
	SetConsoleTextAttribute(hConsole, color);
}
void printB(piece* B[][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			gotoRowCol(20 + i, 130 + j);

			if (B[i][j] != nullptr) { 
				char p = B[i][j]->type;
				if (B[i][j]->color == BLACK) {
					color(8);
				}
				else if (B[i][j]->color == WHITE) {
					color(15); 
				}
				cout << p; 
			}
			else {
				color(7);
				cout << '.';
			}
		}
	}
}

void display_turn(string n) {
	gotoRowCol(20, 130);
	cout << n << " turns!";
}
int turn_change(int n) {
	return (n % 2)+1 ;
}
void outline_box(int sr, int sc) {
	char sym = -37;
	for (int r = 0; r < GRID_SIZE; r++) {
		for (int c = 0; c < GRID_SIZE; c++) {
			gotoRowCol(r + sr, c + sc);
			if (r == 0 or r == GRID_SIZE - 1 or c == 0 or c == GRID_SIZE - 1)
				cout << sym;

		}
	}
}
void grid() {

	for (int r = 0; r < GRID_SIZE; r++) {
		for (int c = 0; c < GRID_SIZE; c++) {
			int sr = r * GRID_SIZE;
			int sc = c * GRID_SIZE;
			outline_box(sr, sc);
		}
	}

}
void calculate_grid_index(piece* B[][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			gotoRowCol(i * 8 + 4, j * 8 + 4);

			if (B[i][j] != nullptr) { 
				char p = B[i][j]->type;
				if (B[i][j]->color == BLACK) {
					color(8); 
				}
				else if (B[i][j]->color == WHITE) {
					color(15); 
				}
				cout << p;
			}
			else {
				color(7); 
				cout << '.';
			}
		}
	}
}
void calculate_Array_index(int& r, int& c) {
	 r = r / GRID_SIZE;
	c = c / GRID_SIZE;

}
bool isValid(int r, int c, piece* B[][GRID_SIZE], int turn, int r1, int c1) {
	if (r < 0 or r >= 63 or c < 0 or c >= 63 or r1 < 0 or r1 >= 63 or c1 < 0 or c1 >= 63)
		return false;

	COLOR color;
	if (turn == 1) {
		color = BLACK;
	}
	else {
		color = WHITE;
	}

	if (B[r][c] == nullptr or B[r][c]->color != color)
		return false;


	if (B[r1][c1] == nullptr or B[r1][c1]->type == '.' or B[r1][c1]->color != color)
		return true;


	return false;
}
void box(int sr, int sc) {
	color(5);
	char sym = -37;
	for (int r = 1; r < GRID_SIZE-1; r++) {
		for (int c = 1; c < GRID_SIZE-1; c++) {
			gotoRowCol(sr+r, sc+c);
				cout << sym;

		}
	}
}
void remove_box(int sr, int sc) {
	for (int r = 1; r < GRID_SIZE - 1; r++) {
		for (int c = 1; c < GRID_SIZE - 1; c++) {
			gotoRowCol(sr + r, sc + c);
			cout << " " ;

		}
	}
}

bool is_horizontal(piece* B[][GRID_SIZE], int sr, int sc, int er, int ec) {
	return sr == er;
}
bool is_vertical(piece* B[][GRID_SIZE], int sr, int sc, int er, int ec) {
	return sc == ec;
}
bool is_diagnaol(piece* B[][GRID_SIZE], int sr, int sc, int er, int ec) {
	int dr = abs(sr - er);
	int dc =abs(sc - ec);
	return dr == dc;
}

bool horizontal_path_clear(piece* B[][GRID_SIZE], int sr, int sc, int er, int ec) {
	int hehe;
	if (sc < ec) 
		hehe = 1;
	else 
		hehe = -1;
	for (int i = sc + hehe; i != ec; i += hehe)
		if (B[sr][i] != nullptr) 
			return false;
		
	return true; 
}
bool vertical_path_clear(piece* B[][GRID_SIZE], int sr, int sc, int er, int ec) {
	int hehe = -1;
	if (sr < er) 
		hehe = 1;
	
	for (int r = sr + hehe; r != er; r += hehe) 
		if (B[r][sc] != nullptr) 
			return false; 
		
	return true;
}
bool diagonal_path_clear(piece* B[][GRID_SIZE], int sr, int sc, int er, int ec) {
	int dr = er - sr;
	int dc = ec - sc;

	int hehe_r = 0;
	if (dr > 0) 
		hehe_r = 1;

	else if (dr < 0) 
		hehe_r = -1;
	

	int hehe_c = 0;
	if (dc > 0) 
		hehe_c = 1;
	
	else if (dc < 0) 
		hehe_c = -1;
	

	int i = sr + hehe_r;
	int j = sc + hehe_c;

	while (i != er and j != ec) {
		if (B[i][j] != nullptr)
			return false;
		
		i += hehe_r;
		j += hehe_c;
	}

	return true;
}

bool rook_islegal(piece* p[][GRID_SIZE], int sr, int sc, int er, int ec) {
	if (is_horizontal(p,sr, sc, er, ec)) 
		return horizontal_path_clear(p, sr, sc, er, ec);

	else if (is_vertical(p, sr, sc, er, ec)) 
		return vertical_path_clear(p, sr, sc, er, ec);
	return false; 
}
bool knight_islegal(piece* p[][GRID_SIZE], int sr, int sc, int er, int ec) {
	int dr = abs(sr - er);
	int dc = abs(sc - ec);
	return (dr == 2 and dc == 1) or (dr == 1 and dc == 2); 
}
bool pawn_islegal(piece* p[][GRID_SIZE], int sr, int sc, int er, int ec) {
	if (p[sr][sc]->color == WHITE) {
		if (sr - 1 == er and sc == ec and p[er][ec] == nullptr) 
			return true; 

		if (sr == 6 and sr - 2 == er and sc == ec and p[er][ec] == nullptr and p[sr - 1][sc] == nullptr) 
			return true; 
		
		if (sr - 1 == er and abs(sc - ec) == 1 and p[er][ec] != nullptr and p[er][ec]->color == BLACK) 
			return true; 
		
	}
	else if (p[sr][sc]->color == BLACK) {
		if (sr + 1 == er and sc == ec and p[er][ec] == nullptr) 
			return true; 
		
		if (sr == 1 and sr + 2 == er and sc == ec and p[er][ec] == nullptr and p[sr + 1][sc] == nullptr) 
			return true; 
		
		if (sr + 1 == er and abs(sc - ec) == 1 and p[er][ec] != nullptr and p[er][ec]->color == WHITE) 
			return true; 
		
	}
	return false; 
}
bool bishop_islegal(piece* p[][GRID_SIZE], int sr, int sc, int er, int ec) {
	if (is_diagnaol(p,sr, sc, er, ec)) 
		return diagonal_path_clear(p, sr, sc, er, ec); 
	
	return false;
}
bool queen_islegal(piece* p[][GRID_SIZE], int sr, int sc, int er, int ec) {
	if (is_horizontal(p, sr, sc, er, ec) or is_vertical(p, sr, sc, er, ec) or is_diagnaol(p, sr, sc, er, ec)) {
		if (is_horizontal(p, sr, sc, er, ec)) 
			return horizontal_path_clear(p, sr, sc, er, ec);
		
		else if (is_vertical(p, sr, sc, er, ec)) 
			return vertical_path_clear(p, sr, sc, er, ec);
		
		else if (is_diagnaol(p, sr, sc, er, ec)) 
			return diagonal_path_clear(p, sr, sc, er, ec);
		
	}
	return false; 
}
bool king_islegal(piece* p[][GRID_SIZE], int sr, int sc, int er, int ec) {
	int dr = abs(sr - er);
	int dc = abs(sc - ec);
	if ((dr <= 1 and dc <= 1) and !(dr == 0 and dc == 0))
		return true;
	
	return false; 
}

void init(piece* B[GRID_SIZE][GRID_SIZE], int& n) {
	char blackPieces[] = { 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R' };
	char whitePieces[] = { 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R' };


	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			B[i][j] = nullptr;
		}
	}

	for (int i = 0; i < GRID_SIZE; i++) {
		B[0][i] = new piece{ blackPieces[i], BLACK };
		switch (blackPieces[i]) {
		case 'R': B[0][i]->islegal = rook_islegal; break;
		case 'N': B[0][i]->islegal = knight_islegal; break;
		case 'B': B[0][i]->islegal = bishop_islegal; break;
		case 'Q': B[0][i]->islegal = queen_islegal; break;
		case 'K': B[0][i]->islegal = king_islegal; break;
		}

		B[1][i] = new piece{ 'p', BLACK };
		B[1][i]->islegal = pawn_islegal;
	}

	
	for (int i = 0; i < GRID_SIZE; i++) {
		B[7][i] = new piece{ whitePieces[i], WHITE };
		switch (whitePieces[i]) {
		case 'R': B[7][i]->islegal = rook_islegal; break;
		case 'N': B[7][i]->islegal = knight_islegal; break;
		case 'B': B[7][i]->islegal = bishop_islegal; break;
		case 'Q': B[7][i]->islegal = queen_islegal; break;
		case 'K': B[7][i]->islegal = king_islegal; break;
		}

		B[6][i] = new piece{ 'P', WHITE };
		B[6][i]->islegal = pawn_islegal;
	}

	n = 2; 
}

void printBoard(bool** highlight, int r, int c) {
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			
			if (highlight[i][j])
				box(i*GRID_SIZE, j*GRID_SIZE);
	
		}
		cout << endl;
	}
}
void remove_board(bool** highlight, int r, int c) {
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {

			if (highlight[i][j]) {
				remove_box(i * GRID_SIZE, j * GRID_SIZE);

			}
		}
		cout << endl;
	}
}
bool** computeHighlight(piece* B[][GRID_SIZE], int r, int c) {
	bool** highlight = new bool* [GRID_SIZE];
	for (int i = 0; i < GRID_SIZE; ++i) {
		highlight[i] = new bool[GRID_SIZE] {false}; 
	}

	if (B[r][c] != nullptr) {
	
		for (int i = 0; i < GRID_SIZE; ++i) {
			for (int j = 0; j < GRID_SIZE; ++j) {
				if (B[r][c]->islegal(B, r, c, i, j)) {
					//if( B[i][j] == nullptr)
					highlight[i][j] = true; 
				}
			}
		}
	}

	return highlight; 
}
void delete_Highlight(bool** highlight) {
	for (int i = 0; i < GRID_SIZE; ++i) {
		delete[] highlight[i];
	}
	delete[] highlight;
}


bool isInCheck(piece* B[][GRID_SIZE], COLOR color) {
	int kingRow=0, kingCol=0;

	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			if (B[i][j] != nullptr && B[i][j]->type == 'K' && B[i][j]->color == color) {
				kingRow = i;
				kingCol = j;
				break;
			}
		}
	}

	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			if (B[i][j] != nullptr && B[i][j]->color != color) {
				if (B[i][j]->islegal(B, i, j, kingRow, kingCol)) {
					return true; 
				}
			}
		}
	}

	return false; 
}
bool isSelfCheck(piece* B[][GRID_SIZE], int sr, int sc, int er, int ec, COLOR playerColor) {

	piece* temp = B[er][ec];
	B[er][ec] = B[sr][sc];
	B[sr][sc] = nullptr;

	bool check = isInCheck(B, playerColor);


	B[sr][sc] = B[er][ec];
	B[er][ec] = temp;

	return check;
}
int main() {
	int r, c;
	int r1, c1;

	piece* B[GRID_SIZE][GRID_SIZE];

	int Turn = 0, number_of_players = 2;
	string names[2] = { "Player 1", "Player 2" };
	init(B, Turn);
	grid();

	bool hehe_boi = true;

	do {
		calculate_grid_index(B);
		printB(B);
		display_turn(names[Turn % number_of_players]);

		bool validMove = false;
		do {
			getRowColbyLeftClick(r, c);
			r /= GRID_SIZE;
			c /= GRID_SIZE;

			bool** highlight = computeHighlight(B, r, c);
			printBoard(highlight, r, c);
			calculate_grid_index(B);

			getRowColbyLeftClick(r1, c1);
			remove_board(highlight, r, c);
			calculate_grid_index(B);

			r1 /= GRID_SIZE;
			c1 /= GRID_SIZE;

			if (isValid(r, c, B, Turn, r1, c1)) {
				if (B[r][c]->islegal(B, r, c, r1, c1)) {
					if (isSelfCheck(B, r, c, r1, c1, B[r][c]->color)) {
						gotoRowCol(10, 130);
						cout << "Move places your king in check!" << endl;
						Sleep(1000);
						gotoRowCol(10, 130);
						cout << "                             " << endl;
						exit(0);
					}
					else {
						if (B[r1][c1] != nullptr) {
							delete B[r1][c1];
						}
						B[r1][c1] = B[r][c];
						B[r][c] = nullptr;
						validMove = true;
					}
				}
				else {
					gotoRowCol(10, 130);
					cout << "Invalid move for this piece!" << endl;
					Sleep(1000);
					gotoRowCol(20, 130);
					cout << "                             " << endl;
				}
			}
			else {
				gotoRowCol(10, 130);
				cout << "Invalid move!" << endl;
				Sleep(1000);
				gotoRowCol(10, 130);
				cout << "                             " << endl;
			}

		} while (!validMove);

	
	
		Turn = turn_change(Turn);

	} while (hehe_boi);

	
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			delete B[i][j]; 
		}
	}

	return 0;
}