#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define Maxlength 500

typedef struct {
	int eightPuzzel[ROWS][COLS];
	int emptyRow;
	int emptyCol;
} State;

const char* action[] = {
	"First State",
	"Move cell EMPTY to UP",
	"Move cell EMPTY to DOWN",
	"Move cell EMPTY to LEFT",
	"Move cell EMPTY to RIGHT"
};

void printState(State state){
	int row, col;
	printf("\n----------\n");
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			printf("|%d ", state.eightPuzzel[row][col]);
		}
		printf("|\n");
	}
	printf("----------\n");
}

int compareState(State state1, State state2){
	if(state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
		return 0;

	int row, col;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			if(state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])
				return 0;
		}
	}
	return 1;
}

int goalCheck(State cur_state, State goal){
	return compareState(cur_state, goal);
}

int moveEmptyCellUp(State cur_state, State *res_state){
	*res_state = cur_state;
	int cur_emptyRow = cur_state.emptyRow; 
	int cur_emptyCol= cur_state.emptyCol; 
	if(cur_state.emptyRow > 0){
		res_state->emptyRow = cur_emptyRow - 1;
		res_state->emptyCol = cur_emptyCol;
		res_state->eightPuzzel[cur_emptyRow][cur_emptyCol] = cur_state.eightPuzzel[cur_emptyRow - 1][cur_emptyCol];
		res_state->eightPuzzel[cur_emptyRow - 1][cur_emptyCol] = EMPTY;
		return 1;
	}
	return 0;
}

int moveEmptyCellDown(State cur_state, State *res_state){
	*res_state = cur_state;
	int cur_emptyRow = cur_state.emptyRow; 
	int cur_emptyCol= cur_state.emptyCol; 
	if(cur_state.emptyRow < ROWS - 1){
		res_state->emptyRow = cur_emptyRow + 1;
		res_state->emptyCol = cur_emptyCol;
		res_state->eightPuzzel[cur_emptyRow][cur_emptyCol] = cur_state.eightPuzzel[cur_emptyRow + 1][cur_emptyCol];
		res_state->eightPuzzel[cur_emptyRow + 1][cur_emptyCol] = EMPTY;
		return 1;
	}
	return 0;
}

int moveEmptyCellLeft(State cur_state, State *res_state){
	*res_state = cur_state;
	int cur_emptyRow = cur_state.emptyRow; 
	int cur_emptyCol= cur_state.emptyCol; 
	if(cur_state.emptyCol > 0){
		res_state->emptyRow = cur_emptyRow;
		res_state->emptyCol = cur_emptyCol - 1;
		res_state->eightPuzzel[cur_emptyRow][cur_emptyCol] = cur_state.eightPuzzel[cur_emptyRow][cur_emptyCol - 1];
		res_state->eightPuzzel[cur_emptyRow][cur_emptyCol - 1] = EMPTY;
		return 1;
	}
	return 0;
}

int moveEmptyCellRight(State cur_state, State *res_state){
	*res_state = cur_state;
	int cur_emptyRow = cur_state.emptyRow; 
	int cur_emptyCol= cur_state.emptyCol; 
	if(cur_state.emptyCol < COLS - 1){
		res_state->emptyRow = cur_emptyRow;
		res_state->emptyCol = cur_emptyCol + 1;
		res_state->eightPuzzel[cur_emptyRow][cur_emptyCol] = cur_state.eightPuzzel[cur_emptyRow][cur_emptyCol + 1];
		res_state->eightPuzzel[cur_emptyRow][cur_emptyCol + 1] = EMPTY;
		return 1;
	}
	return 0;
}

int callOperator(State cur_state, State *res_state, int opt){
	switch(opt){
		case 1: return moveEmptyCellUp(cur_state, res_state);
		case 2: return moveEmptyCellDown(cur_state, res_state);
		case 3: return moveEmptyCellLeft(cur_state, res_state);
		case 4: return moveEmptyCellRight(cur_state, res_state);
		default:
			printf("Error calls operator");
			return 0;
	}
}

int heuristic(State cur_state, State goal){
	int row, col, count = 0;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			if(cur_state.eightPuzzel[row][col] != start.eightPuzzel[row][col])
				count++;
		}
	}
	return 0;
}

struct Node {
	State state;
	struct Node* parent;
	int heuristic;
	int no_function;
};

typedef struct Node Node;




int main(){

	State start, res_state;

	start.emptyRow = 1;
	start.emptyCol = 1;
	start.eightPuzzel[0][0] = 3;
	start.eightPuzzel[0][1] = 4;
	start.eightPuzzel[0][2] = 5;
	start.eightPuzzel[1][0] = 1;
	start.eightPuzzel[1][1] = 0;
	start.eightPuzzel[1][2] = 2;
	start.eightPuzzel[2][0] = 6;
	start.eightPuzzel[2][1] = 7;
	start.eightPuzzel[2][2] = 8;

	printState(start);
	int opt;
	for(opt = 1; opt <= 4; opt++){
		if(callOperator(start, &res_state, opt)){
			printf("Hanh dong %s thanh cong", action[opt]);
			printState(res_state);
		} else printf("Hanh dong %s KHONG thanh cong", action[opt]);
	}

	return 0;
}

