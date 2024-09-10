#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

#define NB_ROWS 9
#define NB_COLUMNS 9
#define MAX_VALUE 10
#define EMPTY 0
#define AREA_SQUARE_SIZE 3
#define INF 99999

typedef struct {
	int x, y;
} Coord;

// Ma tran dinh dinh the hien su rang buoc giua cac dinh
typedef struct {
	int data[NB_ROWS * NB_COLUMNS][NB_ROWS * NB_COLUMNS];
	int n;
} Constrains;

void initContrains(Constrains *constrains){
	int i, j;
	for(i = 0; i < NB_ROWS * NB_COLUMNS; i++){
		for(j = 0; j < NB_ROWS * NB_COLUMNS; j++){
			constrains->data[i][j] = EMPTY;
		}
	}
	constrains->n = NB_ROWS * NB_COLUMNS;
}

// Chuyen doi toa do diem (x, y) sang toa do tinh (stt dinh)
int indexOf(Coord coord){
	return coord.x * NB_ROWS + coord.y;
}

// chuyen doi nguoc lai
Coord positionOfVertex(int vertex){
	Coord coord;
	coord.x = vertex / NB_ROWS;
	coord.y = vertex % NB_ROWS;
	return coord;
}

int addContrains(Constrains *constrains, Coord source, Coord target){
	int u = indexOf(source);
	int v = indexOf(target);
	if(!constrains->data[u][v]){
		constrains->data[u][v] = 1;
		constrains->data[v][u] = 1;
		return 1;
	}
	return 0;
}

vector<Coord> getContrains(Constrains constrains, Coord coord){
	int v = indexOf(coord);
	vector<Coord> result;
	result.clear();
	for(int i = 0; i < constrains.n; i++){
		if(constrains.data[v][i])
			result.push_back(positionOfVertex(i));
	}
	return result;
}

typedef struct {
	int cells[NB_ROWS][NB_COLUMNS];
	Constrains constrains;
} Sudoku;

void initSudoku(Sudoku *sudoku){
	for(int i = 0; i < NB_ROWS; i++){
		for(int j = 0; j < NB_COLUMNS; j++){
			sudoku->cells[i][j] = EMPTY;
		}
	}
	initContrains(&sudoku->constrains);
}

void initSudokuWithValues(Sudoku *sudoku, int inputs[NB_ROWS][NB_COLUMNS]){
	for(int i = 0; i < NB_ROWS; i++){
		for(int j = 0; j < NB_COLUMNS; j++){
			sudoku->cells[i][j] = inputs[i][j];
 		}
	}
}

void printSudoku(Sudoku sudoku){
	printf("Sudoku:\n");
	for(int i = 0; i < NB_ROWS; i++){
		if(i % AREA_SQUARE_SIZE == 0) printf("-------------------------\n");
		for(int j = 0; j < NB_COLUMNS; j++){
			if(j % AREA_SQUARE_SIZE == 0) printf("| ");
			printf("%d ", sudoku.cells[i][j]);
 		}
 		printf("|\n");
	}
	printf("-------------------------\n");
}

int isFilledSudoku(Sudoku sudoku){
	for(int i = 0; i < NB_ROWS; i++){
		for(int j = 0; j < NB_COLUMNS; j++){
			if(sudoku.cells[i][j] == EMPTY) return 0;
		}
	}
	return 1;
}

void spreadConstrainsFrom(Coord position, Constrains *contrains, vector<Coord> *changeds){
	int row = position.x;
	int column = position.y;
	
	// Rang buoc cot
	for(int i = 0; i < NB_ROWS; i++){
		if(i != row){
			Coord pos = {i, column};
			if(addContrains(contrains, position, pos))
				changeds->push_back(pos);
		}
	}
	
	// Rang buoc hang
	for(int i = 0; i < NB_COLUMNS; i++){
		if(i != column){
			Coord pos = {row, i};
			if(addContrains(contrains, position, pos))
				changeds->push_back(pos);
		}
	}
	
	// Rang buoc khoi
	for(int i = 0; i < AREA_SQUARE_SIZE; i++){
		for(int j = 0; j < AREA_SQUARE_SIZE; j++){
			int areaX = (row / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
			int areaY = (column / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
			
			if(areaX + i != row || areaY + j != column){
				Coord pos = {areaX + i, areaY + j};
				if(addContrains(contrains, position, pos))
					changeds->push_back(pos);
			}
		}
	}
}

vector<int> getAvailabelValues(Coord position, Sudoku sudoku){
	vector<Coord> posList = getContrains(sudoku.constrains, position);
	int available[MAX_VALUE];
	
	for(int i = 1; i < MAX_VALUE; i++){
		available[i] = 1;
	}
	
	// Neu o co rang buoc voi o dang xet => gia tri cua o do khong co trong gia tri hop le cua o dang xet
	for(int i = 0; i < posList.size(); i++){
		Coord pos = posList.at(i);
		if(sudoku.cells[pos.x][pos.y] != EMPTY)
			available[sudoku.cells[pos.x][pos.y]] = 0;
	}
	
	vector<int> result;
	result.clear();
	for(int i = 1; i < MAX_VALUE; i++){
		if(available[i])
			result.push_back(i);
	}
	
	return result;
}


// Xac dinh o uu tien
// Cach 1: o trong dau tien
Coord getNextEmptyCell(Sudoku sudoku){
	for(int i = 0; i < NB_ROWS; i++){
		for(int j = 0; j < NB_COLUMNS; j++){
			Coord pos = {i, j};
			if(sudoku.cells[i][j] == EMPTY)
				return pos;
		}
	}
}


// Cach 2: o co mien gia tri hop le it nhat
Coord getNextMinDomainCell(Sudoku sudoku){
	int minLenght = INF;
	Coord result;
	for(int i = 0; i < NB_ROWS; i++){
		for(int j = 0; j < NB_COLUMNS; j++){
			if(sudoku.cells[i][j] == EMPTY){
				Coord pos = {i, j};
				int availablesLength = getAvailabelValues(pos, sudoku).size();
				if(availablesLength < minLenght){
					minLenght = availablesLength;
					result = pos;
				}
			}
		}
	}
	return result;
}

int exploredCounter = 0;
int sudokuBackTracking(Sudoku *sudoku){
	if(isFilledSudoku(*sudoku)) return 1;
	
	Coord position = getNextMinDomainCell(*sudoku);
	
	vector<int> availables = getAvailabelValues(position, *sudoku);
	
	if(availables.size() == 0) return 0;
	
	for(int j = 0; j < availables.size(); j++){
		int value = availables.at(j);
		sudoku->cells[position.x][position.y] = value;
		
		exploredCounter++;
		
		if(sudokuBackTracking(sudoku))
			return 1;
			
		sudoku->cells[position.x][position.y] = EMPTY;
	}
	return 0;
}

Sudoku solveSudoku(Sudoku sudoku){
	initContrains(&sudoku.constrains);
	for(int i = 0; i < NB_ROWS; i++){
		for(int j = 0; j < NB_COLUMNS; j++){
			vector<Coord> history;
			history.clear();
			
			Coord pos = {i, j};
			spreadConstrainsFrom(pos, &sudoku.constrains, &history);
		}
	}
	
	exploredCounter = 0;
	if(sudokuBackTracking(&sudoku)) printf("Solve:\n");
	else printf("Cannot Solve\n");
	
	printf("Explored %d state\n", exploredCounter);
	
	return sudoku;
}

int main(){
	
	int inputs[9][9] = {
		{5, 3, 0, 0, 7, 0, 0, 0, 0},
		{6, 0, 0, 1, 9, 5, 0, 0, 0},
		{0, 9, 8, 0, 0, 0, 0, 6, 0},
		{8, 0, 0, 0, 6, 0, 0, 0, 3},
		{4, 0, 0, 8, 0, 3, 0, 0, 1},
		{7, 0, 0, 0, 2, 0, 0, 0, 6},
		{0, 6, 0, 0, 0, 0, 2, 8, 0},
		{0, 0, 0, 4, 1, 9, 0, 0, 5},
		{0, 0, 0, 0, 8, 0, 0, 7, 9}
	};
	
	Sudoku sudoku;
	initSudokuWithValues(&sudoku, inputs);
	printSudoku(sudoku);
	
	printSudoku(solveSudoku(sudoku));
	
	return 0;
}






















