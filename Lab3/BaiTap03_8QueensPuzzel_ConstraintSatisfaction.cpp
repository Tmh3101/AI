#include <stdio.h>
#include <vector>
#include <algorithm>
#include <initializer_list>

using namespace std;

#define ROWS 8
#define COLS 8
#define NB_QUEENS 8
#define EMPTY 0
#define QUEEN 1

typedef struct {
	int x, y;
} Coord;

typedef struct {
	int data[ROWS * COLS][ROWS * COLS];
	int n;
} Constraints;

int getIndexOfCoord(Coord coord){
	return coord.x * COLS + coord.y;
}

Coord getCoordOfIndex(int index){
	Coord coord = {index / ROWS, index % ROWS};
	return coord;
}

void initContraints(Constraints *constraints){
	for(int i = 0; i < ROWS * COLS; i++){
		for(int j = 0; j < ROWS * COLS; j++){
			constraints->data[i][j] = EMPTY;
		}
	}
	constraints->n = ROWS * COLS;
}

void addContraints(Constraints *constraints, Coord source, Coord target){

	if(target.x < 0 || target.y < 0)
		return;
	if(target.x >= ROWS || target.y >= COLS)
		return;

	int u = getIndexOfCoord(source);
	int v = getIndexOfCoord(target);
	constraints->data[u][v] = 1;	
	constraints->data[v][u] = 1;	
}

vector<Coord> getContraints(Constraints constraints, Coord pos){
	vector<Coord> list;

	int u = getIndexOfCoord(pos);
	for(int v = 0; v < constraints.n; v++){
		if(constraints.data[u][v]){
			list.push_back(getCoordOfIndex(v));
		}
	}

	return list;
}

int checkContraints(Coord pos, Constraints constraints){
	int u = getIndexOfCoord(pos);
	for(int v = 0; v < constraints.n; v++){
		if(constraints.data[u][v])
			return 1;
	}
	return 0;
}

typedef struct {
	int cells[ROWS][COLS];
	Constraints constraints;
} Queens;

void initQueens(Queens *queens){
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			queens->cells[i][j] = EMPTY;
		}
	}
	initContraints(&queens->constraints);
}

void printQueens(Queens queens){
	printf("\nQueen:\n");
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			printf("%c ", (queens.cells[i][j] ? 'X' : 'o'));
		}
		printf("\n");
	}
}

int isFilledQueens(Queens queens){
	for(int col = 0; col < COLS; col++){
		int queensExistence = 0;
		for(int row = 0; row < ROWS; row++){
			if(queens.cells[row][col]){
				queensExistence = 1;
				break;
			}
		}
		if(!queensExistence) return 0;
	}
	return 1;
}

void spreadConstraintsFrom(Coord pos, Constraints *constraints){
	// Lan truyen hang
	for(int i = 0; i < COLS; i++){
		if(i != pos.y){
			Coord target = {pos.x, i};
			addContraints(constraints, pos, target);
		}
	}

	// Lan truyen 2 duong cheo
	int dx[] = {-1, -1, 1, 1};
	int dy[] = {-1, 1, -1, 1};
	for(int i = 0; i < max(ROWS, COLS); i++){
		for(int k = 0; k < 4; k++){
			int dx_i = dx[k] * (i + 1);
			int dy_i = dy[k] * (i + 1);

			Coord target = {pos.x + dx_i, pos.y + dy_i};
			addContraints(constraints, pos, target);
		}
	}
}

void updateConstraints(Queens *queens){
	initContraints(&queens->constraints);
	for(int col = 0; col < COLS; col++){
		int flag = 0;
		for(int row = 0; row < ROWS; row++){
			if(queens->cells[row][col]){
				Coord pos = {row, col};
				spreadConstraintsFrom(pos, &queens->constraints);
				flag = 1;
				break;
			}
		}
		if(!flag) return;
	}
}

vector<int> getValidRowsOfCol(int col, Queens queens){
	vector<int> list;
	for(int i = 0; i < ROWS; i++){
		Coord pos = {i, col};
		if(!checkContraints(pos, queens.constraints))
			list.push_back(i);
	}
	return list;
}

int getNextCol(Queens queens){
	for(int col = 0; col < COLS; col++){
		int flag = 0;
		for(int row = 0; row < ROWS; row++){
			if(queens.cells[row][col])
				flag = 1;
		}
		if(!flag) return col;
	}
	return -1;
}

int exploredCounter;
int queensBackTracking(Queens *queens){
	if(isFilledQueens(*queens)) return 1;

	int col = getNextCol(*queens);
	vector<int> validRows = getValidRowsOfCol(col, *queens);
	if(validRows.empty()) return 0;

	for(size_t i = 0; i < validRows.size(); i++){
		int row = validRows.at(i);
		queens->cells[row][col] = QUEEN;

		Coord pos = {row, col};
		spreadConstraintsFrom(pos, &queens->constraints);
		exploredCounter++;

		if(queensBackTracking(queens))
			return 1;

		queens->cells[row][col] = EMPTY;
		updateConstraints(queens);
	}
	return 0;
}

Queens solveQueens(Queens queens){
	initQueens(&queens);
	exploredCounter = 0;

	if(queensBackTracking(&queens))
		printf("Solve:\n");
	else printf("Cannot Solve\n");

	printf("Explored %d state\n", exploredCounter);
	return queens;
}

int main(){

	Queens queens;
	printQueens(solveQueens(queens));

	return 0;
}