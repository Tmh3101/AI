#include <stdio.h>
#include <stack> 
#include <malloc.h> 

using namespace std;

#define NumberOfRod 3
#define NumberOfDisk 3
#define RodA 0
#define RodB 1
#define RodC 2

typedef struct {
	int disk1; // đĩa lớn nhất
	int disk2;
	int disk3; // đĩa nhỏ nhất
} State;

char getColumnName(int n){
	if(n == 0) return 'A';
	if(n == 1) return 'B';
	return 'C';
}

void printState(State state){
	printf("\nDisk 1:%c --- Disk 2:%c --- Disk 3:%c\n",
		getColumnName(state.disk1),
		getColumnName(state.disk2),
		getColumnName(state.disk3)
	);
}

int compareState(State state1, State state2){
	return state1.disk1 == state2.disk1
		&& state1.disk2 == state2.disk2
		&& state1.disk3 == state2.disk3;
}

int checkGoal(State state){
	return state.disk1 == RodC 
		&& state.disk2 == RodC 
		&& state.disk3 == RodC;
}

int moveDisk1ToRodA(State state, State *result){
	if(state.disk1 == RodA)
		return 0;
	if(state.disk1 == state.disk2 || state.disk1 == state.disk3)
		return 0;
	if(state.disk2 == RodA || state.disk3 == RodA)
		return 0;

	*result = state;
	result->disk1 = RodA;
	return 1;
}

int moveDisk1ToRodB(State state, State *result){
	if(state.disk1 == RodB)
		return 0;
	if(state.disk1 == state.disk2 || state.disk1 == state.disk3)
		return 0;
	if(state.disk2 == RodB || state.disk3 == RodB)
		return 0;

	*result = state;
	result->disk1 = RodB;
	return 1;
}

int moveDisk1ToRodC(State state, State *result){
	if(state.disk1 == RodC)
		return 0;
	if(state.disk1 == state.disk2 || state.disk1 == state.disk3)
		return 0;
	if(state.disk2 == RodC || state.disk3 == RodC)
		return 0;

	*result = state;
	result->disk1 = RodC;
	return 1;
}

int moveDisk2ToRodA(State state, State *result){
	if(state.disk2 == RodA)
		return 0;
	if(state.disk2 == state.disk3)
		return 0;
	if(state.disk3 == RodA)
		return 0;

	*result = state;
	result->disk2 = RodA;
	return 1;
}

int moveDisk2ToRodB(State state, State *result){
	if(state.disk2 == RodB)
		return 0;
	if(state.disk2 == state.disk3)
		return 0;
	if(state.disk3 == RodB)
		return 0;

	*result = state;
	result->disk2 = RodB;
	return 1;
}

int moveDisk2ToRodC(State state, State *result){
	if(state.disk2 == RodC)
		return 0;
	if(state.disk2 == state.disk3)
		return 0;
	if(state.disk3 == RodC)
		return 0;

	*result = state;
	result->disk2 = RodC;
	return 1;
}

int moveDisk3ToRodA(State state, State *result){
	if(state.disk3 == RodA)
		return 0;

	*result = state;
	result->disk3 = RodA;
	return 1;
}

int moveDisk3ToRodB(State state, State *result){
	if(state.disk3 == RodB)
		return 0;

	*result = state;
	result->disk3 = RodB;
	return 1;
}

int moveDisk3ToRodC(State state, State *result){
	if(state.disk3 == RodC)
		return 0;

	*result = state;
	result->disk3 = RodC;
	return 1;
}

const char *action[] = {
	"First State",
	"Move Disk 1 To Rod A",
	"Move Disk 1 To Rod B",
	"Move Disk 1 To Rod C",
	"Move Disk 2 To Rod A",
	"Move Disk 2 To Rod B",
	"Move Disk 2 To Rod C",
	"Move Disk 3 To Rod A",
	"Move Disk 3 To Rod B",
	"Move Disk 3 To Rod C"
};

int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return moveDisk1ToRodA(state, result);
		case 2: return moveDisk1ToRodB(state, result);
		case 3: return moveDisk1ToRodC(state, result);
		case 4: return moveDisk2ToRodA(state, result);
		case 5: return moveDisk2ToRodB(state, result);
		case 6: return moveDisk2ToRodC(state, result);
		case 7: return moveDisk3ToRodA(state, result);
		case 8: return moveDisk3ToRodB(state, result);
		case 9: return moveDisk3ToRodC(state, result);
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

Node* DFS(State start){
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

		if(checkGoal(node->state)) return node;

		for(int opt = 1; opt <= 9; opt++){
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

	State start = {RodA, RodA, RodA};
	printWaysToGoal(DFS(start));
	
	return 0;
}