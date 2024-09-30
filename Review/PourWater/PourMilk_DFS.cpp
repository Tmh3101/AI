#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <algorithm>

using namespace std;

#define Max_X 16 
#define Max_Y 7 
#define Max_Z 3

#define GOAL 8

typedef struct {
	int x, y, z;
} State;

void printState(State state){
	printf("\nX:%d -- Y:%d -- Z:%d\n", state.x, state.y, state.z);
}

int compareState(State state1, State state2){
	return state1.x == state2.x 
		&& state1.y == state2.y 
		&& state1.z == state2.z;
}

int isGoal(State state){
	return state.x == GOAL;
}

int pourMilkXY(State state, State *result){
	if(state.x > 0 && state.y < Max_Y){
		result->x = max(state.x - (Max_Y - state.y), 0);
		result->y = min(state.x + state.y, Max_Y);
		result->z = state.z;
		return 1;
	}
	return 0;
}

int pourMilkXZ(State state, State *result){
	if(state.x > 0 && state.z < Max_Z){
		result->x = max(state.x - (Max_Z - state.z), 0);
		result->y = state.y;
		result->z = min(state.x + state.z, Max_Z);
		return 1;
	}
	return 0;
}

int pourMilkYX(State state, State *result){
	if(state.y > 0 && state.x < Max_X){
		result->x = min(state.y + state.x, Max_X);
		result->y = max(state.y - (Max_X - state.x), 0);
		result->z = state.z;
		return 1;
	}
	return 0;
}

int pourMilkYZ(State state, State *result){
	if(state.y > 0 && state.z < Max_Z){
		result->x = state.x;
		result->y = max(state.y - (Max_Z - state.z), 0);
		result->z = min(state.y + state.z, Max_Z);
		return 1;
	}
	return 0;
}

int pourMilkZX(State state, State *result){
	if(state.z > 0 && state.x < Max_X){
		result->x = min(state.z + state.x, Max_X);
		result->y = state.y;
		result->z = max(state.z - (Max_X - state.x), 0);
		return 1;
	}
	return 0;
}

int pourMilkZY(State state, State *result){
	if(state.z > 0 && state.y < Max_Y){
		result->x = state.x;
		result->y = min(state.z + state.y, Max_Y);
		result->z = max(state.z - (Max_Y - state.y), 0);
		return 1;
	}
	return 0;
}

const char *actions[] = {
	"First State",
	"Pour Milk X To Y",
	"Pour Milk X To Z",
	"Pour Milk Y To X",
	"Pour Milk Y To Z",
	"Pour Milk Z To X",
	"Pour Milk Z To Y"
};

int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return pourMilkXY(state, result);
		case 2: return pourMilkXZ(state, result);
		case 3: return pourMilkYX(state, result);
		case 4: return pourMilkYZ(state, result);
		case 5: return pourMilkZX(state, result);
		case 6: return pourMilkZY(state, result);
		default:
			printf("Error: CALL OPERATORS");
			return 0;
	}
}

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

		for(int i = 1; i <= 6; i++){
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

	State state = {16, 0, 0};
	printWaysToGoal(DFS(state));

	return 0;
}