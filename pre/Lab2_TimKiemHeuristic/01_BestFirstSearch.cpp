#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <algorithm>

using namespace std;

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MaxOperator 4
#define Maxlength 500

typedef struct {
	int eightPuzzel[ROWS][COLS];
	int emptyRow;
	int emptyCol;
} State;

State goal;

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
			if(cur_state.eightPuzzel[row][col] != goal.eightPuzzel[row][col])
				count++;
		}
	}
	return count;
}

struct Node {
	State state;
	struct Node* parent;
	int heuristic;
	int no_function;
};

typedef struct Node Node;

Node* findState(State state, vector<Node*> v, vector<Node*>::iterator *position){
	vector<Node*>::iterator it = v.begin();
	
	if(v.size() == 0) return NULL;

	while(it != v.end()){
		if(compareState((*it)->state, state)){
			*position = it;
			return *it;
		}

		it = v.erase(it);
	}

	return NULL;
}

int compareHeuristic(Node* a, Node* b){
	return heuristic(a->state, goal) > heuristic(b->state, goal);
}

Node* BestFirstSearch(State start, State goal){
	vector<Node*> openList(Maxlength);
	openList.clear();
	vector<Node*> closeList(Maxlength);
	closeList.clear();

	Node* root = (Node*)malloc(sizeof(Node));
	root->state = start;
	root->parent = NULL;
	root->no_function = 0;
	root->heuristic = heuristic(root->state, goal);

	openList.push_back(root);

	while(!openList.empty()){
		Node* node = openList.back();
		openList.pop_back();

		closeList.push_back(node);

		if(goalCheck(node->state, goal)) return node;

		int opt;
		for(opt = 1; opt <= MaxOperator; opt++){
			State newState = node->state;

			if(callOperator(node->state, &newState, opt)){

				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_function = opt;
				newNode->heuristic = heuristic(newNode->state, goal);

				vector<Node*>::iterator posOpenList, posCloseList;
				Node* nodeFoundOpen = findState(newState, openList, &posOpenList);
				Node* nodeFoundClose = findState(newState, closeList, &posCloseList);
				
				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					openList.push_back(newNode);
				} else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
					openList.erase(posOpenList);
					openList.push_back(newNode);
				} else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic){
					closeList.erase(posCloseList);
					closeList.push_back(newNode);
				}

				sort(openList.begin(), openList.end(), compareHeuristic);
			}
		}
	}

	return NULL;
}

void printWaysToGetGoal(Node *node){
	vector<Node*> path;

	while(node->parent != NULL){
		path.push_back(node);
		node = node->parent;
	}
	path.push_back(node);

	int no_action = 0, i;
	for(i = path.size() - 1; i >= 0; i--){
		printf("\nAction %d: %s", no_action, action[path.at(i)->no_function]);
		printState(path.at(i)->state);
		no_action++;
	}
}


int main(){

	State start;
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

	goal.emptyRow = 0;
	goal.emptyCol = 0;
	goal.eightPuzzel[0][0] = 0;
	goal.eightPuzzel[0][1] = 1;
	goal.eightPuzzel[0][2] = 2;
	goal.eightPuzzel[1][0] = 3;
	goal.eightPuzzel[1][1] = 4;
	goal.eightPuzzel[1][2] = 5;
	goal.eightPuzzel[2][0] = 6;
	goal.eightPuzzel[2][1] = 7;
	goal.eightPuzzel[2][2] = 8;

	Node* node = BestFirstSearch(start, goal);

	printWaysToGetGoal(node);

	printState(node->state);

	return 0;
}

