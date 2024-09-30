#include <stdio.h>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <malloc.h>

#define NumberOfGFs 3
#define NumberOfOFs 3
#define NumberOfStones 7
#define GF1 1
#define GF2 2
#define GF3 3
#define EMPTY 0
#define OF1 -1
#define OF2 -2
#define OF3 -3

using namespace std;

typedef struct {
	int stones[NumberOfStones];
	int n_stones;
} State;

void initState(State *state){
	int m = -3;
	for(int i = 0; i < NumberOfStones; i++){
		state->stones[i] = m++; 
	}
	state->n_stones = NumberOfStones;
}

int checkGoal(State state){
	if(state.stones[3] != EMPTY)
		return 0;

	for(int i = 0; i < state.n_stones; i++){
		if(i < 3 && state.stones[i] < 0)
			return 0;
		if(i > 3 && state.stones[i] > 0)
			return 0;
	}
	return 1;
}

int compareState(State state1, State state2){
	for(int i = 0; i < state1.n_stones; i++){
		if(state1.stones[i] != state2.stones[i])
			return 0;
	}
	return 1;
}

char* getFogName(int i){
	if(i == GF3) return "GF3";
	if(i == GF2) return "GF2";
	if(i == GF1) return "GF1";
	if(i == EMPTY) return "EMPTY";
	if(i == OF1) return "OF1";
	if(i == OF2) return "OF2";
	if(i == OF3) return "OF3";
}

void printState(State state){
	printf("\n");
	for(int i = 0; i < state.n_stones; i++){
		printf("%s", getFogName(state.stones[i]));
		if(i < NumberOfStones - 1)
			printf(" - ");
	}
	printf("\n");
}

int checkSameColor(int frog1, int frog2){
	return frog1 * frog2 >= 0;
}

int GF1Jump1Step(State state, State *result){
	*result = state;
	for(int i = 1; i < state.n_stones; i++){
		if(state.stones[i] == GF1){
			if(state.stones[i - 1] == EMPTY){
				result->stones[i - 1] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int GF1Jump2Step(State state, State *result){
	*result = state;
	for(int i = 2; i < state.n_stones; i++){
		if(state.stones[i] == GF1){
			if(state.stones[i - 2] == EMPTY && !checkSameColor(state.stones[i], state.stones[i - 1])){
				result->stones[i - 2] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int GF2Jump1Step(State state, State *result){
	*result = state;
	for(int i = 1; i < state.n_stones; i++){
		if(state.stones[i] == GF2){
			if(state.stones[i - 1] == EMPTY){
				result->stones[i - 1] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int GF2Jump2Step(State state, State *result){
	*result = state;
	for(int i = 2; i < state.n_stones; i++){
		if(state.stones[i] == GF2){
			if(state.stones[i - 2] == EMPTY && !checkSameColor(state.stones[i], state.stones[i - 1])){
				result->stones[i - 2] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int GF3Jump1Step(State state, State *result){
	*result = state;
	for(int i = 1; i < state.n_stones; i++){
		if(state.stones[i] == GF3){
			if(state.stones[i - 1] == EMPTY){
				result->stones[i - 1] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int GF3Jump2Step(State state, State *result){
	*result = state;
	for(int i = 2; i < state.n_stones; i++){
		if(state.stones[i] == GF3){
			if(state.stones[i - 2] == EMPTY && !checkSameColor(state.stones[i], state.stones[i - 1])){
				result->stones[i - 2] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int OF1Jump1Step(State state, State *result){
	*result = state;
	for(int i = 0; i < state.n_stones - 1; i++){
		if(state.stones[i] == OF1){
			if(state.stones[i + 1] == EMPTY){
				result->stones[i + 1] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int OF1Jump2Step(State state, State *result){
	*result = state;
	for(int i = 0; i < state.n_stones - 2; i++){
		if(state.stones[i] == OF1){
			if(state.stones[i + 2] == EMPTY && !checkSameColor(state.stones[i], state.stones[i + 1])){
				result->stones[i + 2] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}


int OF2Jump1Step(State state, State *result){
	*result = state;
	for(int i = 0; i < state.n_stones - 1; i++){
		if(state.stones[i] == OF2){
			if(state.stones[i + 1] == EMPTY){
				result->stones[i + 1] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int OF2Jump2Step(State state, State *result){
	*result = state;
	for(int i = 0; i < state.n_stones - 2; i++){
		if(state.stones[i] == OF2){
			if(state.stones[i + 2] == EMPTY && !checkSameColor(state.stones[i], state.stones[i + 1])){
				result->stones[i + 2] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int OF3Jump1Step(State state, State *result){
	*result = state;
	for(int i = 0; i < state.n_stones - 1; i++){
		if(state.stones[i] == OF3){
			if(state.stones[i + 1] == EMPTY){
				result->stones[i + 1] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}

int OF3Jump2Step(State state, State *result){
	*result = state;
	for(int i = 0; i < state.n_stones - 2; i++){
		if(state.stones[i] == OF3){
			if(state.stones[i + 2] == EMPTY && !checkSameColor(state.stones[i], state.stones[i + 1])){
				result->stones[i + 2] = result->stones[i];
				result->stones[i] = EMPTY;
				return 1;
			} else return 0;
		}
	}
	return 0;
}


int callOperators(State state, State *result, int opt){
	switch(opt){
		case 1: return GF1Jump1Step(state, result);
		case 2: return GF1Jump2Step(state, result);
		case 3: return GF2Jump1Step(state, result);
		case 4: return GF2Jump2Step(state, result);
		case 5: return GF3Jump1Step(state, result);
		case 6: return GF3Jump2Step(state, result);
		case 7: return OF1Jump1Step(state, result);
		case 8: return OF1Jump2Step(state, result);
		case 9: return OF2Jump1Step(state, result);
		case 10: return OF2Jump2Step(state, result);
		case 11: return OF3Jump1Step(state, result);
		case 12: return OF3Jump2Step(state, result);
		default:
			printf("ERROR: CALL OPERATORS");
			return 0;
	}
}

const char* action[] = {
	"First State",
	"Green Fog 1 Jump 1 Step",
	"Green Fog 1 Jump 2 Step",
	"Green Fog 2 Jump 1 Step",
	"Green Fog 2 Jump 2 Step",
	"Green Fog 3 Jump 1 Step",
	"Green Fog 3 Jump 2 Step",
	"Orange Fog 1 Jump 1 Step",
	"Orange Fog 1 Jump 2 Step",
	"Orange Fog 2 Jump 1 Step",
	"Orange Fog 2 Jump 2 Step",
	"Orange Fog 3 Jump 1 Step",
	"Orange Fog 3 Jump 2 Step"
};

typedef struct Node {
	State state;
	struct Node *parent;
	int no_act;
} Node;


int findState(State state, queue<Node*> list){
	while(!list.empty()){
		if(compareState(state, list.front()->state))
			return 1;
		list.pop();
	}
	return 0;
}

Node* BFS(State start){
	queue<Node*> openList;
	queue<Node*> closeList;

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = start;
	root->parent = NULL;
	root->no_act = 0;

	openList.push(root);

	while(!openList.empty()){
		Node* node = openList.front();
		openList.pop();
		closeList.push(node);

		if(checkGoal(node->state)) return node;

		for(int opt = 1; opt <= 12; opt++){
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

	State state;
	initState(&state);
	printWaysToGoal(BFS(state));

	return 0;
}