#include <stdio.h>
#include <malloc.h>
#include <stack>

using namespace std;

#define soLuongTS 3
#define soLuongQ 3
#define soLuongToiDa 2

typedef struct {
	int so_tusi;
	int so_quy;
	char vitri_thuyen;
} State;

void makeNullState(State *state){
	state->so_tusi = 0;
	state->so_quy = 0;
	state->vitri_thuyen = 'A';
}

void printState(State state){
	printf("\nTu si ben bo A: %d --- Quy ben bo A: %d --- Thuyen dang o ben bo %c", state.so_tusi, state.so_quy, state.vitri_thuyen);
}

int goalCheck(State state){
	return state.so_tusi == 0
		&& state.so_quy == 0
		&& state.vitri_thuyen == 'B';
}

int validCheck(State state){
	return !(state.so_tusi < state.so_quy && state.so_tusi > 0);
}

int compareStates(State state1, State state2){
	return state1.so_tusi == state2.so_tusi
		&& state1.so_quy == state2.so_quy
		&& state1.vitri_thuyen == state2.vitri_thuyen;
}

int move_1TS(State cur_state, State *res_state){
	if(cur_state.vitri_thuyen == 'A'){
		if(cur_state.so_tusi >= 1){
			res_state->so_tusi = cur_state.so_tusi - 1;
			res_state->so_quy = cur_state.so_quy;
			res_state->vitri_thuyen = 'B';
		} else return 0;
	} else {
		if(soLuongTS - cur_state.so_tusi >= 1){
			res_state->so_tusi = cur_state.so_tusi + 1;
			res_state->so_quy = cur_state.so_quy;
			res_state->vitri_thuyen = 'A';
		} else return 0;
	}

	return validCheck(*res_state);
}

int move_2TS(State cur_state, State *res_state){
	if(cur_state.vitri_thuyen == 'A'){
		if(cur_state.so_tusi >= 2){
			res_state->so_tusi = cur_state.so_tusi - 2;
			res_state->so_quy = cur_state.so_quy;
			res_state->vitri_thuyen = 'B';
		} else return 0;
	} else {
		if(soLuongTS - cur_state.so_tusi >= 2){
			res_state->so_tusi = cur_state.so_tusi + 2;
			res_state->so_quy = cur_state.so_quy;
			res_state->vitri_thuyen = 'A';
		} else return 0;
	}

	return validCheck(*res_state);
}

int move_1Q(State cur_state, State *res_state){
	if(cur_state.vitri_thuyen == 'A'){
		if(cur_state.so_quy >= 1){
			res_state->so_tusi = cur_state.so_tusi;
			res_state->so_quy = cur_state.so_quy - 1;
			res_state->vitri_thuyen = 'B';
		} else return 0;
	} else {
		if(soLuongQ - cur_state.so_quy >= 1){
			res_state->so_tusi = cur_state.so_tusi;
			res_state->so_quy = cur_state.so_quy + 1;
			res_state->vitri_thuyen = 'A';
		} else return 0;
	}

	return validCheck(*res_state);
}

int move_2Q(State cur_state, State *res_state){
	if(cur_state.vitri_thuyen == 'A'){
		if(cur_state.so_quy >= 2){
			res_state->so_tusi = cur_state.so_tusi;
			res_state->so_quy = cur_state.so_quy - 2;
			res_state->vitri_thuyen = 'B';
		} else return 0;
	} else {
		if(soLuongQ - cur_state.so_quy >= 2){
			res_state->so_tusi = cur_state.so_tusi;
			res_state->so_quy = cur_state.so_quy + 2;
			res_state->vitri_thuyen = 'A';
		} else return 0;
	}

	return validCheck(*res_state);
}

int move_1TS1Q(State cur_state, State *res_state){
	if(cur_state.vitri_thuyen == 'A'){
		if(cur_state.so_tusi >= 1 && cur_state.so_quy >= 1){
			res_state->so_tusi = cur_state.so_tusi - 1;
			res_state->so_quy = cur_state.so_quy - 1;
			res_state->vitri_thuyen = 'B';
		} else return 0;
	} else {
		if(soLuongTS - cur_state.so_tusi >= 1 && soLuongQ - cur_state.so_quy >= 1){
			res_state->so_tusi = cur_state.so_tusi + 1;
			res_state->so_quy = cur_state.so_quy + 1;
			res_state->vitri_thuyen = 'A';
		} else return 0;
	}

	return validCheck(*res_state);
}

int callOperator(State cur_state, State *res_state, int opt){
	switch(opt){
		case 1: return move_1TS(cur_state, res_state);
		case 2: return move_2TS(cur_state, res_state);
		case 3: return move_1Q(cur_state, res_state);
		case 4: return move_2Q(cur_state, res_state);
		case 5: return move_1TS1Q(cur_state, res_state);
		default:
			printf("Error calls operators");
			return 0;
	}
}

const char *action[] = {
	"First State",
	"Move 1 TS",
	"Move 2 TS",
	"Move 1 Q",
	"Move 2 Q",
	"Move 1 TS & 1 Q"
};

struct Node {
	State state;
	struct Node *parent;
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
		for(opt = 1; opt <= 5; opt++){

			State newState;
			makeNullState(&newState);

			if(callOperator(node->state, &newState, opt)){
				if(findState(newState, openList) || findState(newState, closeList)) 
					continue;

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

	State start = {3, 3, 'A'};

	Node* node = DFS_Althorithm(start);
	printWaysToGetGoal(node);

	return 0;
}