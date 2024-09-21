#include <stdio.h>
#include <vector> 
#include <stack> 
#include <malloc.h> 
#include <algorithm>

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

int heuristic(State state, State goal){
	int h = 0;
	if(state.d1 != goal.d1) h++;
	if(state.d2 != goal.d2) h++;
	return h;
}

typedef struct Node {
	State state;
	struct Node *parent;
	int no_act;
	int g, h;
} Node;

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

bool compare_f(Node* node1, Node* node2){
	return node1->g + node1->h > node2->g + node2->h;
}

Node* AStarAlgorithm(State start, State goal){
	vector<Node*> openList;
	vector<Node*> closeList;

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = start;
	root->parent = NULL;
	root->no_act = 0;
	root->g = 0;
	root->h = heuristic(start, goal);

	openList.push_back(root);

	while(!openList.empty()){
		Node *node = openList.back();
		openList.pop_back();

		closeList.push_back(node);

		if(checkGoal(node->state, goal)) return node;

		for(int opt = 1; opt <= 6; opt++){
			State newState;
			if(callOperators(node->state, &newState, opt)){

				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_act = opt;
				newNode->g = newNode->parent->g + 1;
				newNode->h = heuristic(newNode->state, goal);

				vector<Node*>::iterator posOpen, posClose;
				Node* foundOpen = findState(newNode->state, openList, &posOpen);
				Node* foundClose = findState(newNode->state, closeList, &posClose);

				if(foundOpen == NULL && foundClose == NULL){
					openList.push_back(newNode);
				}

				else if(foundOpen != NULL && foundOpen->g > newNode->g){
					openList.erase(posOpen);
					openList.push_back(newNode);
				}

				else if(foundClose != NULL && foundClose->g > newNode->g){
					closeList.erase(posClose);
					openList.push_back(newNode);
				}

				// Sắp xếp lại open theo chiều giảm dần
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

	printWaysToGoal(AStarAlgorithm(start, goal));
	
	return 0;
}