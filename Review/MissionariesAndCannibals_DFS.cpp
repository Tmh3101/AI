#include <stdio.h>
#include <stack>
#include <malloc.h>

using namespace std;

#define numberOfMissionaries 3
#define numberOfCannibals 3
#define BankA 0
#define BankB 1

typedef struct {
	int M; //Number of missionaries on BankA.
	int C; //Number of cannibals on BankA.
	int B; // position of the boat (BankA - 1 or BankB - 2).
} State;

void printState(State state){
	printf("\nMissionaries on BankA: %d", state.M);
	printf("\nCannibals on BankA: %d", state.C);
	printf("\nBoat is on %s\n", state.B ? "BankB" : "BankA");
}

int isGoal(State state){
	return state.M == 0
		&& state.C == 0
		&& state.B;
}

int compareState(State state1, State state2){
	return state1.M == state2.M
		&& state1.C == state2.C 
		&& state1.B == state2.B;
}

int isValidState(State state){
	if(state.M < 0 || state.C < 0 || state.M > 3 || state.C > 3)
		return 0;
	if(state.M > 0 && state.M < state.C)
		return 0;
	if(numberOfMissionaries - state.M > 0 && numberOfMissionaries - state.M < numberOfCannibals - state.C)
		return 0;
	return 1;
}

int Move1M(State state, State *result){
	*result = state;
	if(state.B == BankA){
		result->M -= 1;
		result->B = BankB;
	} else {
		result->M += 1;
		result->B = BankA;
	}
	return isValidState(*result);
}

int Move2M(State state, State *result){
	*result = state;
	if(state.B == BankA){
		result->M -= 2;
		result->B = BankB;
	} else {
		result->M += 2;
		result->B = BankA;
	}
	return isValidState(*result);
}

int Move1C(State state, State *result){
	*result = state;
	if(state.B == BankA){
		result->C -= 1;
		result->B = BankB;
	} else {
		result->C += 1;
		result->B = BankA;
	}
	return isValidState(*result);
}

int Move2C(State state, State *result){
	*result = state;
	if(state.B == BankA){
		result->C -= 2;
		result->B = BankB;
	} else {
		result->C += 2;
		result->B = BankA;
	}
	return isValidState(*result);
}

int Move1M1C(State state, State *result){
	*result = state;
	if(state.B == BankA){
		result->M -= 1;
		result->C -= 1;
		result->B = BankB;
	} else {
		result->M += 1;
		result->C += 1;
		result->B = BankA;
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
} Node;

int findState(State state, stack<Node*> list){
	while(!list.empty()){
		if(compareState(list.top()->state, state))
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

		for(int opt = 1; opt <= 5; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){
				if(!findState(newState, openList) && !findState(newState, closeList)){
					Node* newNode = (Node*)malloc(sizeof(Node));
					newNode->state = newState;
					newNode->parent = node;
					newNode->no_act = opt;

					openList.push(newNode);
				}
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
		printf("\nAction %d: %s", action_stt++, action[path.top()->no_act]);
		printState(path.top()->state);
		path.pop();
	}
}


int main(){

	State state = {3, 3, BankA};

	printWaysToGoal(DFS(state));

	printf("\n\n\n");
	State result;
	printf("Trang thai bat dau\n");
	printState(state);
	for(int opt = 1; opt<=5; opt++){
		int thuchien = callOperators(state, &result, opt);
		if(thuchien == 1){
			printf("Hanh dong %s thanh cong\n", action[opt] );
			printState(result);
		} else printf("Hanh dong %s KHONG thanh cong\n", action[opt]);
	}

	return 0;
}
