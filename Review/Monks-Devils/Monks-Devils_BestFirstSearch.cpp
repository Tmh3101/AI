#include <stdio.h>
#include <stack>
#include <vector>
#include <malloc.h>
#include <algorithm>

using namespace std;

#define numberOfMonks 3
#define numberOfDevils 3
#define BankA 1
#define BankB 0

typedef struct {
	int monks; //Number of monks on BankA.
	int devils; //Number of devils on BankA.
	int boat; // position of the boat (BankA - 1 or BankB - 2).
} State;

void printState(State state){
	printf("\nMissionaries on BankA: %d", state.monks);
	printf("\nCannibals on BankA: %d", state.devils);
	printf("\nBoat is on %s\n", state.boat ? "BankB" : "BankA");
}

int isGoal(State state){
	return state.monks == 0
		&& state.devils == 0
		&& state.boat == 0;
}

int compareState(State state1, State state2){
	return state1.monks == state2.monks
		&& state1.devils == state2.devils 
		&& state1.boat == state2.boat;
}

int isValidState(State state){
	if(state.monks < 0 || state.devils < 0 || state.monks > 3 || state.devils > 3)
		return 0;
	if(state.monks > 0 && state.monks < state.devils)
		return 0;
	if(numberOfMonks - state.monks > 0 && numberOfMonks - state.monks < numberOfDevils - state.devils)
		return 0;
	return 1;
}

int Move1M(State state, State *result){
	*result = state;
	if(state.boat == BankA){
		result->monks -= 1;
		result->boat = BankB;
	} else {
		result->monks += 1;
		result->boat = BankA;
	}
	return isValidState(*result);
}

int Move2M(State state, State *result){
	*result = state;
	if(state.boat == BankA){
		result->monks -= 2;
		result->boat = BankB;
	} else {
		result->monks += 2;
		result->boat = BankA;
	}
	return isValidState(*result);
}

int Move1C(State state, State *result){
	*result = state;
	if(state.boat == BankA){
		result->devils -= 1;
		result->boat = BankB;
	} else {
		result->devils += 1;
		result->boat = BankA;
	}
	return isValidState(*result);
}

int Move2C(State state, State *result){
	*result = state;
	if(state.boat == BankA){
		result->devils -= 2;
		result->boat = BankB;
	} else {
		result->devils += 2;
		result->boat = BankA;
	}
	return isValidState(*result);
}

int Move1M1C(State state, State *result){
	*result = state;
	if(state.boat == BankA){
		result->monks -= 1;
		result->devils -= 1;
		result->boat = BankB;
	} else {
		result->monks += 1;
		result->devils += 1;
		result->boat = BankA;
	}
	return isValidState(*result);
}

int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return Move1M(state, result);
		case 2: return Move2M(state, result);
		case 3: return Move1C(state, result);
		case 4: return Move2C(state, result);
		case 5: return Move1M1C(state, result);
		default:
			printf("ERROR: CALL OPERATORS");
			return 0;
	}
}

const char *action[] = {
	"First State",
	"Move 1 Missionaries",
	"Move 2 Missionaries",
	"Move 1 Cannibals",
	"Move 2 Cannibals",
	"Move 1 Missionaries And 1 Cannibals"
};

typedef struct Node {
	State state;
	struct Node* parent;
	int no_act;
	int h, g;
} Node;

int heuristic(State state){
	return state.monks + state.devils;
}

Node* findState(State state, vector<Node*> list, vector<Node*>::iterator *pos){
	if(list.empty()) return NULL;

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

bool compare_f(Node *a, Node *b){
	return a->h + a->g > b->h + b->g;
}

Node* BestFirstSearch(State state){
	vector<Node*> openList;
	vector<Node*> closeList;

	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_act = 0;
	root->h = heuristic(root->state);
	root->g = 0;

	openList.push_back(root);

	while(!openList.empty()){
		Node* node = openList.back();
		openList.pop_back();
		closeList.push_back(node);

		if(isGoal(node->state)) return node;

		for(int opt = 1; opt <= 5; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){

				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_act = opt;
				newNode->h = heuristic(newNode->state);
				newNode->g = newNode->parent->g + 1;

				vector<Node*>::iterator posOpen, posClose;
				Node* foundOpen = findState(newNode->state, openList, &posOpen);
				Node* foundClose = findState(newNode->state, closeList, &posClose);

				if(foundOpen == NULL && foundClose == NULL){
					openList.push_back(newNode);
				} else if(foundOpen != NULL && foundOpen->g + foundOpen->h > newNode->g + newNode->h){
					openList.erase(posOpen);
					openList.push_back(newNode);
				} else if(foundClose != NULL && foundClose->g + foundClose->h > newNode->g + newNode->h){
					closeList.erase(posOpen);
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
	while(node->parent != NULL){
		path.push(node);
		node = node->parent;
	}
	path.push(node);

	int action_stt = 0;
	while(!path.empty()){
		printf("\nAction %d: %s", action_stt++, action[path.top()->no_act]);
		printState(path.top()->state);
		path.pop();
	}
}

int main(){

	State state = {3, 3, BankA};
	printWaysToGoal(BestFirstSearch(state));

	return 0;
}