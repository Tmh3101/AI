#include <stdio.h>
#include <stack>
#include <malloc.h>

using namespace std;

#define ROWS 4
#define COLS 4
#define X_Start 0
#define Y_Start 0
#define X_Goal 3
#define Y_Goal 3

typedef struct {
	int maze[ROWS][COLS];
	int xRobot, yRobot;
} State;

void initState(State *state, int maze[ROWS][COLS]){
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			state->maze[i][j] = maze[i][j];
		}
	}
	state->xRobot = X_Start;
	state->yRobot = Y_Start;
}

void printState(State state){
	printf("\n");
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			if(i == state.xRobot && j == state.yRobot)
				printf("X ");
			else printf("%d ", state.maze[i][j]);
		}
		printf("\n");
	}
	printf("Robot Position: (%d, %d)\n", state.xRobot, state.yRobot);
}

int compareState(State state1, State state2){
	if(state1.xRobot != state2.xRobot)
		return 0;
	if(state1.yRobot != state2.yRobot)
		return 0;

	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			if(state1.maze[i][j] != state2.maze[i][j])
				return 0;
		}
	}

	return 1;
}

int isGoal(State state){
	if(state.xRobot != X_Goal)
		return 0;
	if(state.yRobot != Y_Goal)
		return 0;
	return 1;
}

int moveLeft(State state, State *result){
	*result = state;
	if(state.yRobot < COLS - 1){
		if(state.maze[state.xRobot][state.yRobot + 1] == 1){
			result->xRobot = state.xRobot;
			result->yRobot = state.yRobot + 1;
			return 1;
		}
	}
	return 0;
}

int moveRight(State state, State *result){
	*result = state;
	if(state.yRobot > 0){
		if(state.maze[state.xRobot][state.yRobot - 1] == 1){
			result->xRobot = state.xRobot;
			result->yRobot = state.yRobot - 1;
			return 1;
		}
	}
	return 0;
}

int moveUp(State state, State *result){
	*result = state;
	if(state.yRobot > 0){
		if(state.maze[state.xRobot - 1][state.yRobot] == 1){
			result->xRobot = state.xRobot - 1;
			result->yRobot = state.yRobot;
			return 1;
		}
	}
	return 0;
}

int moveDown(State state, State *result){
	*result = state;
	if(state.yRobot < ROWS - 1){
		if(state.maze[state.xRobot + 1][state.yRobot] == 1){
			result->xRobot = state.xRobot + 1;
			result->yRobot = state.yRobot;
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
	int g, h;
} Node;

int heuristic(State state){
	int h;
	
}

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

	int input[ROWS][COLS] = {
		{1, 0, 0, 0},
		{1, 1, 0, 1},
		{0, 1, 0, 0},
		{1, 1, 1, 1}
	};

	State state;
	initState(&state, input);

	printWaysToGoal(DFS(state));
	return 0;
}