#include <stdio.h>
#include <stack>
#include <stdlib.h>
#include <algorithm>

using namespace std;

#define BankA 1
#define BankB 0

typedef struct {
	int wolf, goat, cabbage, farmer;
} State;

void printState(State state){
	printf("\nWolf is on %s", state.wolf ? "BankA" : "BankB");
	printf("\nGoat is on %s", state.goat ? "BankA" : "BankB");
	printf("\nCabbage is on %s", state.cabbage ? "BankA" : "BankB");
	printf("\nFarmer is on %s\n", state.farmer ? "BankA" : "BankB");
}

int compareState(State state1, State state2){
	return state1.wolf == state2.wolf
		&& state1.goat == state2.goat
		&& state1.cabbage == state2.cabbage
		&& state1.farmer == state2.farmer;
}

int isGoal(State state){
	return state.wolf == BankB
		&& state.goat == BankB
		&& state.cabbage == BankB
		&& state.farmer == BankB;
}

int isValidState(State state){
	if(state.wolf == state.goat && state.farmer != state.wolf)
		return 0;
	if(state.goat == state.cabbage && state.farmer != state.goat)
		return 0;
	return 1;
}

int moveWolf(State state, State *result){
	*result = state;
	if(state.farmer == state.wolf){
		result->wolf = !state.wolf;
		result->farmer = !state.farmer;
		return isValidState(*result);
	}
	return 0;
}

int moveGoat(State state, State *result){
	*result = state;
	if(state.farmer == state.goat){
		result->goat = !state.goat;
		result->farmer = !state.farmer;
		return isValidState(*result);
	}
	return 0;
}

int moveCabbage(State state, State *result){
	*result = state;
	if(state.farmer == state.cabbage){
		result->cabbage = !state.cabbage;
		result->farmer = !state.farmer;
		return isValidState(*result);
	}
	return 0;
}

int moveFarmer(State state, State *result){
	*result = state;
	result->farmer = !state.farmer;
	return isValidState(*result);
}

int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return moveWolf(state, result);
		case 2: return moveGoat(state, result);
		case 3: return moveCabbage(state, result);
		case 4: return moveFarmer(state, result);
		default:
			printf("Error: CALL OPERATORS");
			return 0;
	}
}

const char *actions[] = {
	"First State",
	"Move Wolf",
	"Move Goat",
	"Move Cabbage",
	"Move Farmer"
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

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_act = 0;

	openList.push(root);
	while(!openList.empty()){

		Node* node = openList.top();
		openList.pop();

		closeList.push(node);

		if(isGoal(node->state)) return node;

		for(int i = 1; i <= 4; i++){
			State newState;
			if(callOperators(node->state, &newState, i)){

				if(findState(newState, openList) || findState(newState, closeList))
					continue;

				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_act = i;

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

	int action_stt = 0;
	while(!path.empty()){
		printf("\nACTION %d: %s", action_stt++, actions[path.top()->no_act]);
		printState(path.top()->state);
		path.pop();
	}
}


int main(){

	State state = {1, 1, 1, 1};
	printWaysToGoal(DFS(state));

	return 0;
}