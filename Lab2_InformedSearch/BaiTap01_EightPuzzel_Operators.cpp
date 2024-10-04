#include <stdio.h>

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define Maxlength 500

const char* action[] = {
	"First State",
	"Move cell EMPTY to UP",
	"Move cell EMPTY to DOWN",
	"Move cell EMPTY to LEFT",
	"Move cell EMPTY to RIGHT"
};

typedef struct {
	int eightPuzzel[ROWS][COLS];
	int emptyRow;
	int emptyCol;
} State;

void printState(State state){
	printf("\n----------\n");
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++){
			printf("|%d ", state.eightPuzzel[row][col]);
		}
		printf("|\n");
	}
	printf("----------\n");
}

int compareState(State state1, State state2){
	if(state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
		return 0;

	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++){
			if(state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])
				return 0;
		}	
	}

	return 1;
}


int goalCheck(State state, State goal){
	return compareState(state, goal);
}

int upOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyRowCurrent > 0){
		result->emptyRow = emptyRowCurrent - 1;
		result->emptyCol = emptyColCurrent;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent - 1][emptyColCurrent];
		result->eightPuzzel[emptyRowCurrent - 1][emptyColCurrent] = EMPTY;
		return 1;
	}

	return 0;
}

int downOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyRowCurrent < ROWS - 1){
		result->emptyRow = emptyRowCurrent + 1;
		result->emptyCol = emptyColCurrent;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent + 1][emptyColCurrent];
		result->eightPuzzel[emptyRowCurrent + 1][emptyColCurrent] = EMPTY;
		return 1;
	}

	return 0;
}

int leftOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyColCurrent > 0){
		result->emptyRow = emptyRowCurrent;
		result->emptyCol = emptyColCurrent - 1;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent][emptyColCurrent - 1];
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent - 1] = EMPTY;
		return 1;
	}

	return 0;
}

int rightOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyColCurrent < COLS){
		result->emptyRow = emptyRowCurrent;
		result->emptyCol = emptyColCurrent + 1;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent][emptyColCurrent + 1];
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent + 1] = EMPTY;
		return 1;
	}

	return 0;
}

int callOperator(State state, State *result, int opt){
	switch(opt){
		case 1: return upOperator(state, result);
		case 2: return downOperator(state, result);
		case 3: return leftOperator(state, result);
		case 4: return rightOperator(state, result);
		default:
			printf("Cannot call operator");
			return 0;
	}
}

int main(){
	State state;

	state.emptyRow = 1;
	state.emptyCol = 1;
	state.eightPuzzel[0][0] = 3;
	state.eightPuzzel[0][1] = 4;
	state.eightPuzzel[0][2] = 5;
	state.eightPuzzel[1][0] = 1;
	state.eightPuzzel[1][1] = 0;
	state.eightPuzzel[1][2] = 2;
	state.eightPuzzel[2][0] = 6;
	state.eightPuzzel[2][1] = 7;
	state.eightPuzzel[2][2] = 8;

	printf("Trang thai bat dau: \n");
	printState(state);
	for(int opt = 1; opt <= 4; opt++){
		State result;
		callOperator(state, &result, opt);
		if(!compareState(state, result)){
			printf("Hanh dong %s thanh cong\n", action[opt]);
			printState(result);
		} else {
			printf("Hanh dong %s KHONG thanh cong\n", action[opt]);
		}
	}

	return 0;
}




