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





int main(){
	Sudoku sudoku;
	initSudoku(&sudoku);
	printSudoku(sudoku);
	return 0;
}