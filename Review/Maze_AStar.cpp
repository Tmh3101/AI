#include <stdio.h>
#include <stack>
#include <malloc.h>
#include <vector>
#include <cmath>
#include <algorithm>

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
	int g;
	float h;
} Node;

float euclidean(int x_a, int y_a, int x_b, int y_b){
	return sqrt(pow(x_a - x_b, 2) + pow(y_a - y_b, 2));
}

float manhattan(int x_a, int y_a, int x_b, int y_b){
	return abs(x_a - x_b) + abs(y_a - y_b);
}

float heuristic(State state){
	int goalRow, goalCol;
	for(int i = 0; i < state.row; i++){
		for(int j = 0; j < state.col; j++){
			if(state.maze[i][j] == 3){
				goalRow = i;
				goalCol = j;
			}
		}
	}
	return manhattan(state.rowRobot, state.colRobot, goalRow, goalCol);
}

int compare_f(Node* node1, Node* node2){
	return node1->g + node1->h > node2->g + node2->h;
}

Node* findState(State state, vector<Node*> list, vector<Node*>::iterator *pos){
	if(list.empty()) return 0;

	vector<Node*>::iterator it = list.begin();
	while(it != list.end()){
		if(compareState((*it)->state, state)){
			*pos = it;
			return *it;
		}
		it = list.erase(it);
	}
	return NULL;
}

Node* DFS(State state){
	vector<Node*> openList;
	vector<Node*> closeList;

	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_act = 0;
	root->g = 0;
	root->h = heuristic(root->state);

	openList.push_back(root);

	while(!openList.empty()){
		Node* node = openList.back();
		openList.pop_back();
		closeList.push_back(node);

		if(isGoal(node->state)) return node;

		for(int opt = 1; opt <= 4; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_act = opt;
				newNode->g = node->g + 1;
				newNode->h = heuristic(newNode->state);

				vector<Node*>::iterator posOpen, posClose;
				Node* foundOpen = findState(newNode->state, openList, &posOpen);
				Node* foundClose = findState(newNode->state, closeList, &posClose);

				if(foundOpen == NULL && foundClose == NULL){
					openList.push_back(newNode);
				} else if(foundOpen != NULL && foundOpen->g > newNode->g){
					openList.erase(posOpen);
					openList.push_back(newNode);
				} else if(foundClose != NULL && foundClose->g > newNode->g){
					closeList.erase(posClose);
					openList.push_back(newNode);
				}

				sort(openList.begin(), openList.end(), compare_f);
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