#include <stdio.h>
#include <malloc.h>
#include <stack>

using namespace std;

#define soLuongLy 6
#define ngua 1
#define up 0

typedef struct {
	int dsLy[soLuongLy];
} State;

void makeNullState(State *state){
	int i;
	for(i = 0; i < soLuongLy; i++){
		state->dsLy[i] = (i % 2 == 0);
	}
}

void printState(State state){
	printf("\n");
	int i;
	for(i = 0; i < soLuongLy; i++){
		printf("%d:%-7s", i + 1, state.dsLy[i] == 0 ? "Up" : "Ngua");

	}
}

int goalCheck(State state){
	int i;
	for(i = 0; i < soLuongLy; i++){
		if(state.dsLy[i] == 0) return 0;
	}
	return 1;
}

int compareStates(State state1, State state2){
	int i;
	for(i = 0; i < soLuongLy; i++){
		if(state1.dsLy[i] != state2.dsLy[i]) return 0;
	}
	return 1;
}

int reverse_123(State cur_state, State *res_state){
	int i;
	for(i = 0; i < soLuongLy; i++){
		if(i == 0 || i == 1 || i == 2)
			res_state->dsLy[i] = !cur_state.dsLy[i];
		else res_state->dsLy[i] = cur_state.dsLy[i];
	}
	return 1;
}

int reverse_234(State cur_state, State *res_state){
	int i;
	for(i = 0; i < soLuongLy; i++){
		if(i == 1 || i == 2 || i == 3)
			res_state->dsLy[i] = !cur_state.dsLy[i];
		else res_state->dsLy[i] = cur_state.dsLy[i];
	}
	return 1;
}

int reverse_345(State cur_state, State *res_state){
	int i;
	for(i = 0; i < soLuongLy; i++){
		if(i == 2 || i == 3 || i == 4)
			res_state->dsLy[i] = !cur_state.dsLy[i];
		else res_state->dsLy[i] = cur_state.dsLy[i];
	}
	return 1;
}

int reverse_456(State cur_state, State *res_state){
	int i;
	for(i = 0; i < soLuongLy; i++){
		if(i == 3 || i == 4 || i == 5)
			res_state->dsLy[i] = !cur_state.dsLy[i];
		else res_state->dsLy[i] = cur_state.dsLy[i];
	}
	return 1;
}

int callOperator(State cur_state, State *res_state, int opt){
	switch(opt){
		case 1: return reverse_123(cur_state, res_state);
		case 2: return reverse_234(cur_state, res_state);
		case 3: return reverse_345(cur_state, res_state);
		case 4: return reverse_456(cur_state, res_state);
		default:
			printf("Error calls operators");
			return 0;
	}
}

const char *action[]{
	"First State",
	"Reverse 1 2 3",
	"Reverse 2 3 4",
	"Reverse 3 4 5",
	"Reverse 4 5 6"
};

struct Node {
	State state;
	struct Node *parent;
	int no_function;
};

typedef struct Node Node;

int findState(State state, stack<Node*> list){
	while(!list.empty()){
		if(compareStates(list.top()->state, state))
			return 1;
		list.pop();
	}
	return 0;
}

Node* DFS_Althorithm(State start){
	stack<Node*> openList;
	stack<Node*> closeList;

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = start;
	root->parent = NULL;
	root->no_function = 0;

	openList.push(root);

	while(!openList.empty()){

		Node *node = openList.top();
		openList.pop();
		closeList.push(node);

		if(goalCheck(node->state)) return node;

		int opt;
		for(opt = 1; opt <= 4; opt++){


			State newState;
			makeNullState(&newState);

			if(callOperator(node->state, &newState, opt)){
				if(findState(newState, openList) || findState(newState, closeList))
					continue;
				
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_function = opt;

				openList.push(newNode);
			}
		}
	}

	return NULL;
}

void printWaysToGetGoal(Node *node){
	stack<Node*> path;

	while(node->parent != NULL){
		path.push(node);
		node = node->parent;
	}
	path.push(node);

	int no_action = 0;
	while(!path.empty()){
		printf("\nAction %d: %s", no_action, action[path.top()->no_function]);
		printState(path.top()->state);
		path.pop();
		no_action++;
	}
}




int main(){
	State start;
	makeNullState(&start);
	
	Node* node = DFS_Althorithm(start);
	printWaysToGetGoal(node);
}