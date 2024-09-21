#include <stdio.h>
#include <stack> 
#include <malloc.h> 

using namespace std;

#define NumberOfRod 3
#define NumberOfDisk 2
#define A 0
#define B 1
#define C 2
#define NumberOfOperators 6;

typedef struct {
	int d1, d2;
} State;

char getColumnName(int n){
	if(n == 0) return 'A';
	if(n == 1) return 'B';
	return 'C';
}

void printState(State state){
	printf("\nDisk 1: %c --- Disk 2: %c\n",
		getColumnName(state.d1),
		getColumnName(state.d2)
	);
}

int compareState(State state1, State state2){
	return state1.d1 == state2.d1
		&& state1.d2 == state2.d2;
}

int checkGoal(State state, State goal){
	return compareState(state, goal);
}

int moveD1ToA(State state, State *result){
	if(state.d1 == A || state.d1 == state.d2 || state.d2 == A)
		return 0;

	result->d1 = A;
	result->d2 = state.d2;
	return 1;
}

int moveD1ToB(State state, State *result){
	if(state.d1 == B || state.d1 == state.d2 || state.d2 == B)
		return 0;

	result->d1 = B;
	result->d2 = state.d2;
	return 1;
}

int moveD1ToC(State state, State *result){
	if(state.d1 == C || state.d1 == state.d2 || state.d2 == C)
		return 0;

	result->d1 = C;
	result->d2 = state.d2;
	return 1;
}

int moveD2ToA(State state, State *result){
	if(state.d2 == A)
		return 0;

	result->d1 = state.d1;
	result->d2 = A;
	return 1;
}

int moveD2ToB(State state, State *result){
	if(state.d2 == B)
		return 0;

	result->d1 = state.d1;
	result->d2 = B;
	return 1;
}

int moveD2ToC(State state, State *result){
	if(state.d2 == C)
		return 0;

	result->d1 = state.d1;
	result->d2 = C;
	return 1;
}

const char *action[] = {
	"First State",
	"Move Disk 1 To A",
	"Move Disk 1 To B",
	"Move Disk 1 To C",
	"Move Disk 2 To A",
	"Move Disk 2 To B",
	"Move Disk 2 To C"
};

int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return moveD1ToA(state, result);
		case 2: return moveD1ToB(state, result);
		case 3: return moveD1ToC(state, result);
		case 4: return moveD2ToA(state, result);
		case 5: return moveD2ToB(state, result);
		case 6: return moveD2ToC(state, result);
		default:
			printf("\nError: call operators with case is %d", opt);
			return 0;
	}
}

typedef struct Node {
	State state;
	struct Node *parent;
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

Node* DFS(State start, State goal){
	stack<Node*> openList;
	stack<Node*> closeList;

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = start;
	root->parent = NULL;
	root->no_act = 0;

	openList.push(root);

	while(!openList.empty()){
		Node *node = openList.top();
		openList.pop();

		closeList.push(node);

		if(checkGoal(node->state, goal)) return node;

		for(int opt = 1; opt <= 6; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){
				if(findState(newState, openList) || findState(newState, closeList))
					continue;

				Node *newNode = (Node*)malloc(sizeof(Node));
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
	while(node->parent != NULL){
		path.push(node);
		node = node->parent;
	}
	path.push(node);

	int act = 1;
	printf("===> Ways to get goal <===");
	while(!path.empty()){
		printf("\nAction %d: %s", act++, action[path.top()->no_act]);
		printState(path.top()->state);
		path.pop();
	}
}


int main(){

	State start = {A, A};
	State goal = {C, C};

	printWaysToGoal(DFS(start, goal));
	
	return 0;
}