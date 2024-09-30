#include <stdio.h>
#include <stack>
#include <malloc.h>

using namespace std;

#define MAX_ROWS 10
#define MAX_COLS 10

typedef struct {
	int row, col;
	int maze[MAX_ROWS][MAX_COLS];
	int rowRobot, colRobot;
} State;

void initState(State *state, int maze[MAX_ROWS][MAX_COLS], int row, int col){
	state->row = row;
	state->col = col;
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			state->maze[i][j] = maze[i][j];
			if(maze[i][j] == 2){
				state->rowRobot = i;
				state->colRobot = j;
			}
		}
	}
}

void printState(State state){
	printf("\n");
	for(int i = 0; i < state.row; i++){
		for(int j = 0; j < state.col; j++){
			if(i == state.rowRobot && j == state.colRobot)
				printf("X ");
			else printf("%d ", state.maze[i][j]);
		}
		printf("\n");
	}
	printf("Robot Position: [%d, %d]\n", state.rowRobot, state.colRobot);
}

int compareState(State state1, State state2){
	return state1.rowRobot == state2.rowRobot
		&& state1.colRobot == state2.colRobot;
}

int isGoal(State state){
	return state.maze[state.rowRobot][state.colRobot] == 3;
}

int moveLeft(State state, State *result){
	*result = state;
	if(state.colRobot > 0){
		if(state.maze[state.rowRobot][state.colRobot - 1]){
			result->colRobot -= 1;
			return 1;
		}
	}
	return 0;
}

int moveRight(State state, State *result){
	*result = state;
	if(state.colRobot < state.col - 1){
		if(state.maze[state.rowRobot][state.colRobot + 1]){
			result->colRobot += 1;
			return 1;
		}
	}
	return 0;
}

int moveUp(State state, State *result){
	*result = state;
	if(state.rowRobot > 0){
		if(state.maze[state.rowRobot - 1][state.colRobot]){
			result->rowRobot -= 1;
			return 1;
		}
	}
	return 0;
}

int moveDown(State state, State *result){
	*result = state;
	if(state.rowRobot < state.row - 1){
		if(state.maze[state.rowRobot + 1][state.colRobot]){
			result->rowRobot += 1;
			return 1;
		}
	}
	return 0;
}

int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return moveLeft(state, result);
		case 2: return moveRight(state, result);
		case 3: return moveUp(state, result);
		case 4: return moveDown(state, result);
		default:
			printf("ERROR: CALL OPERATORS");
			return 0;
	}
}

const char* actions[] = {
	"First State",
	"Robot Move Left",
	"Robot Move Right",
	"Robot Move Up",
	"Robot Move Down"
};

typedef struct Node {
	State state;
	struct Node* parent;
	int no_act;
} Node;

int findState(State state, stack<Node*> list){
	while(!list.empty()){
		if(compareState(state, list.top()->state))
			return 1;
		list.pop();
	}
	return 0;
}

Node* DFS(State state){
	stack<Node*> openList;
	stack<Node*> closeList;

	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_act = 0;

	openList.push(root);
	while(!openList.empty()){
		Node* node = openList.top();
		openList.pop();
		closeList.push(node);

		if(isGoal(node->state)) return node;

		for(int opt = 1; opt <= 4; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){
				if(findState(newState, openList) || findState(newState, closeList))
					continue;

				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_act = opt;

				openList.push(newNode);
			}
		}
	}

	return NULL;
}

void printWaysToGoal(Node *node){
	stack<Node*> path;
	while(node != NULL){
		path.push(node);
		node = node->parent;
	}

	int cnt = 0;
	while(!path.empty()){
		printf("\nAction %d: %s", cnt++, actions[path.top()->no_act]);
		printState(path.top()->state);
		path.pop();
	}
}

int main(){

	int n = 4, m = 4;
	int input[MAX_ROWS][MAX_COLS] = {
		{2, 0, 0, 0},
		{1, 1, 0, 1},
		{0, 1, 0, 0},
		{1, 1, 1, 3}
	};

	State state;
	initState(&state, input, n, m);
	printWaysToGoal(DFS(state));
	
	return 0;
}