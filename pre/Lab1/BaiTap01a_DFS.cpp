#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stack>

using namespace std;

#define tankcapacity_X 10
#define tankcapacity_Y 6
#define tankcapacity_Z 5
#define goal 8

typedef struct {
	int x, y, z;
} State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
	state->z = 0;
}

void printState(State state){
	printf("\nX:%d --- Y:%d --- Z:%d", state.x, state.y, state.z);
}

int goalCheck(State state){
	return state.x == goal;
}

int compareStates(State state1, State state2){
	return state1.x == state2.x && state1.y == state2.y && state1.z == state2.z;
}

int pourWaterFullX(State cur_state, State *res_state){
	if(cur_state.x < tankcapacity_X){
		res_state->x = tankcapacity_X;
		res_state->y = cur_state.y;
		res_state->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterFullY(State cur_state, State *res_state){
	if(cur_state.y < tankcapacity_Y){
		res_state->x = cur_state.x;
		res_state->y = tankcapacity_Y;
		res_state->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterFullZ(State cur_state, State *res_state){
	if(cur_state.z < tankcapacity_Z){
		res_state->x = cur_state.x;
		res_state->y = cur_state.y;
		res_state->z = tankcapacity_Z;
		return 1;
	}
	return 0;
}

int pourWaterEmptyX(State cur_state, State *res_state){
	if(cur_state.x > 0){
		res_state->x = 0;
		res_state->y = cur_state.y;
		res_state->x = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterEmptyY(State cur_state, State *res_state){
	if(cur_state.y > 0){
		res_state->x = cur_state.x;
		res_state->y = 0;
		res_state->x = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterEmptyZ(State cur_state, State *res_state){
	if(cur_state.z > 0){
		res_state->x = cur_state.x;
		res_state->y = cur_state.y;
		res_state->x = 0;
		return 1;
	}
	return 0;
}

int max(int a, int b){
	return a > b ? a : b;
}

int min(int a, int b){
	return a < b ? a : b;
}

int pourWaterXY(State cur_state, State *res_state){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		res_state->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), 0);
		res_state->y = min(cur_state.y + cur_state.x, tankcapacity_Y);
		res_state->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterXZ(State cur_state, State *res_state){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		res_state->x = max(cur_state.x - (tankcapacity_Z - cur_state.z), 0);
		res_state->y = cur_state.y;
		res_state->z = min(cur_state.z + cur_state.x, tankcapacity_Z);
		return 1;
	}
	return 0;
}

int pourWaterYX(State cur_state, State *res_state){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		res_state->x = min(cur_state.x + cur_state.y, tankcapacity_X);
		res_state->y = max(cur_state.y - (tankcapacity_X - cur_state.x), 0);
		res_state->z = cur_state.z;
		return 1;
	}
	return 0;
}

int pourWaterYZ(State cur_state, State *res_state){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		res_state->x = cur_state.x;
		res_state->y = max(cur_state.y - (tankcapacity_Z - cur_state.z), 0);
		res_state->z = min(cur_state.z + cur_state.y, tankcapacity_Z);
		return 1;
	}
	return 0;
}

int pourWaterZX(State cur_state, State *res_state){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		res_state->x = min(cur_state.x + cur_state.z, tankcapacity_X);
		res_state->y = cur_state.y;
		res_state->z = max(cur_state.z - (tankcapacity_X - cur_state.x), 0);
		return 1;
	}
	return 0;
}

int pourWaterZY(State cur_state, State *res_state){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		res_state->x = cur_state.x;
		res_state->y = min(cur_state.y + cur_state.z, tankcapacity_X);
		res_state->z = max(cur_state.z - (tankcapacity_Y - cur_state.y), 0);
		return 1;
	}
	return 0;
}

int callOperator(State cur_state, State *res_state, int opt){
	switch(opt){
		case 1: return pourWaterFullX(cur_state, res_state);
		case 2:	return pourWaterFullY(cur_state, res_state);
		case 3:	return pourWaterFullZ(cur_state, res_state);
		case 4: return pourWaterEmptyX(cur_state, res_state);
		case 5:	return pourWaterEmptyY(cur_state, res_state);
		case 6:	return pourWaterEmptyZ(cur_state, res_state);
		case 7:	return pourWaterXY(cur_state, res_state);
		case 8:	return pourWaterXZ(cur_state, res_state);
		case 9:	return pourWaterYX(cur_state, res_state);
		case 10: return pourWaterYZ(cur_state, res_state);
		case 11: return pourWaterZX(cur_state, res_state);
		case 12: return pourWaterZY(cur_state, res_state);
		default:
			printf("Error calls operators");
			return 0;
	}
}

const char* action[] = {
	"First State",
	"Pour Water Full X",
	"Pour Water Full Y",
	"Pour Water Full Z",
	"Pour Water Empty X",
	"Pour Water Empty Y",
	"Pour Water Empty Z",
	"Pour Water XY",
	"Pour Water XZ",
	"Pour Water YX",
	"Pour Water YZ",
	"Pour Water ZX",
	"Pour Water ZY"
};

struct Node {
	State state;
	struct Node* parent;
	int no_function;
};

typedef struct Node Node;

int findState(State state, stack<Node*> list){
	while(!list.empty()){
		if(compareStates(list.top()->state, state)) return 1;
		list.pop();
	}
	return 0;
}

Node* DFS_Althorithm(State start){
	stack<Node*> openList;
	stack<Node*> closeList;

	Node* root = (Node*)malloc(sizeof(Node));
	root->state = start;
	root->parent = NULL;
	root->no_function = 0;

	openList.push(root);

	while(!openList.empty()){
		Node* node = openList.top();
		openList.pop();
		closeList.push(node);

		if(goalCheck(node->state)) return node;

		int opt;
		for(opt = 1; opt <= 12; opt++){
			State newState;
			makeNullState(&newState);

			if(callOperator(node->state, &newState, opt)){
				if(findState(newState, openList) || findState(newState, closeList)) continue;

				Node* newNode = (Node*)malloc(sizeof(Node));
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
	stack<Node*> way;
	while(node->parent != NULL){
		way.push(node);
		node = node->parent;
	}
	way.push(node);

	int no_action = 0;
	while(!way.empty()){
		printf("\nAction %d: %s", no_action, action[way.top()->no_function]);
		printState(way.top()->state);
		way.pop();
		no_action++;
	}
}

int main(){
	State start = {10, 0, 0};

	Node* node = DFS_Althorithm(start);
	printWaysToGetGoal(node);

	return 0;
}