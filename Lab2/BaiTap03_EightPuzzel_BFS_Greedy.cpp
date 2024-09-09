#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define Maxlength 500

const char* action[] = {
	"First State",
	"Move cell EMPTY to UP",
	"Move cell EMPTY to DOWN",
	"Move cell EMPTY to LEFT",
	"Move cell EMPTY to RIGHT"
};

typedef struct {
	int eightPuzzel[ROWS][COLS];
	int emptyRow;
	int emptyCol;
} State;

void printState(State state){
	printf("\n----------\n");
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++){
			printf("|%d ", state.eightPuzzel[row][col]);
		}
		printf("|\n");
	}
	printf("----------\n");
}

int compareState(State state1, State state2){
	if(state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
		return 0;

	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++){
			if(state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])
				return 0;
		}	
	}

	return 1;
}

int goalCheck(State state, State goal){
	return compareState(state, goal);
}

int upOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyRowCurrent > 0){
		result->emptyRow = emptyRowCurrent - 1;
		result->emptyCol = emptyColCurrent;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent - 1][emptyColCurrent];
		result->eightPuzzel[emptyRowCurrent - 1][emptyColCurrent] = EMPTY;
		return 1;
	}

	return 0;
}

int downOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyRowCurrent < ROWS - 1){
		result->emptyRow = emptyRowCurrent + 1;
		result->emptyCol = emptyColCurrent;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent + 1][emptyColCurrent];
		result->eightPuzzel[emptyRowCurrent + 1][emptyColCurrent] = EMPTY;
		return 1;
	}

	return 0;
}

int leftOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyColCurrent > 0){
		result->emptyRow = emptyRowCurrent;
		result->emptyCol = emptyColCurrent - 1;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent][emptyColCurrent - 1];
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent - 1] = EMPTY;
		return 1;
	}

	return 0;
}

int rightOperator(State state, State *result){
	*result = state;
	int emptyRowCurrent = state.emptyRow;
	int emptyColCurrent = state.emptyCol;

	if(emptyColCurrent < COLS - 1){
		result->emptyRow = emptyRowCurrent;
		result->emptyCol = emptyColCurrent + 1;
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent] = state.eightPuzzel[emptyRowCurrent][emptyColCurrent + 1];
		result->eightPuzzel[emptyRowCurrent][emptyColCurrent + 1] = EMPTY;
		return 1;
	}

	return 0;
}

int callOperator(State state, State *result, int opt){
	switch(opt){
		case 1: return upOperator(state, result);
		case 2: return downOperator(state, result);
		case 3: return leftOperator(state, result);
		case 4: return rightOperator(state, result);
		default:
			printf("Cannot call operator");
			return 0;
	}
}

// Đếm số vị trí sai khác so với trạng thái đích
int heuristic1(State state, State goal){
	int cnt = 0;
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++){
			if(state.eightPuzzel[row][col] != goal.eightPuzzel[row][col])
				cnt++;
		}
	}
	return cnt;
}

// Tình tổng số khoảng cách sai khác - đếm số bước để di chuyển các ô sai về đúng vị trí
// Khoảng cách Manhattan
int heuristic2(State state, State goal){
	int cnt = 0;
	// Duyệt qua các phần tử trong state - nếu khác Empty thì tìm phần tử tương ứng trong goal
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLS; col++){
			if(state.eightPuzzel[row][col] != EMPTY){
				// Tìm phần tử tưởng ứng với phần tử của state trong goal
				for(int row_g = 0; row_g < ROWS; row_g++){
					for(int col_g = 0; col_g < COLS; col_g++){
						if(state.eightPuzzel[row][col] == goal.eightPuzzel[row_g][col_g]){
							// Tính khoảng các Manhattan -> cộng dồn vao kết quả
							cnt += abs(row - row_g) + abs(col - col_g);
							// Break 2 vào lặp khi đã tìm được phần tử tương ứng
							col_g = COLS;
							row_g = ROWS;
						}
					}
				}
			}
		}
	}
	return cnt;
}

typedef struct Node {
	State state;
	struct Node* parent;
	int no_function;
	int heuristic;
} Node;

Node* findState(State state, vector<Node*> v, vector<Node*>::iterator *position){
	if(v.size() == 0) return NULL;

	vector<Node*>::iterator it = v.begin();
	while(it != v.end()){
		if(compareState((*it)->state, state)){
			*position = it;
			return *it;
		}

		it = v.erase(it);
	}
	return NULL;
}

bool compareHeristic(Node* a, Node* b){
	return a->heuristic > b->heuristic;
}

Node* best_first_search(State state, State goal){
	vector<Node*> Open_BFS(Maxlength);
	Open_BFS.clear();
	vector<Node*> Close_BFS(Maxlength);
	Close_BFS.clear();

	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_function = 0;
	root->heuristic = heuristic1(root->state, goal);

	Open_BFS.push_back(root);
	while(!Open_BFS.empty()){
		Node* node = Open_BFS.back();

		Open_BFS.pop_back();
		Close_BFS.push_back(node);

		if(goalCheck(node->state, goal)) return node;

		for(int opt = 1; opt <= MAX_OPERATOR; opt++){
			State newState = node->state;

			if(callOperator(node->state, &newState, opt)){
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->parent = node;
				newNode->no_function = opt;
				newNode->heuristic = heuristic1(newNode->state, goal);

				vector<Node*>::iterator pos_Open, pos_Close;
				Node* nodeFoundOpen = findState(newNode->state, Open_BFS, &pos_Open);
				Node* nodeFoundClose = findState(newNode->state, Close_BFS, &pos_Close);

				if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
					Open_BFS.push_back(newNode);
				} else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
					Open_BFS.erase(pos_Open);
					Open_BFS.push_back(newNode);
				} else if(nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic){
					Close_BFS.erase(pos_Close);
					Open_BFS.push_back(newNode);
				} 

			}
		}

		sort(Open_BFS.begin(), Open_BFS.end(), compareHeristic);
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	vector<Node*> vectorPrint;

	while(node->parent != NULL){
		vectorPrint.push_back(node);
		node = node->parent;
	}
	vectorPrint.push_back(node);

	int no_action = 0;
	for(int i = vectorPrint.size() - 1; i >= 0; i--){
		printf("\nAction %d: %s", no_action, action[vectorPrint.at(i)->no_function]);
		printState(vectorPrint.at(i)->state);
		no_action++;
	}
}

int main(){
	State state;
	state.emptyRow = 1;
	state.emptyCol = 1;
	state.eightPuzzel[0][0] = 3;
	state.eightPuzzel[0][1] = 4;
	state.eightPuzzel[0][2] = 5;
	state.eightPuzzel[1][0] = 1;
	state.eightPuzzel[1][1] = 0; 
	state.eightPuzzel[1][2] = 2;
	state.eightPuzzel[2][0] = 6;
	state.eightPuzzel[2][1] = 7;
	state.eightPuzzel[2][2] = 8;

	State goal;
	goal.emptyRow = 0;
	goal.emptyCol = 0;
	goal.eightPuzzel[0][0] = 0;
	goal.eightPuzzel[0][1] = 1;
	goal.eightPuzzel[0][2] = 2;
	goal.eightPuzzel[1][0] = 3;
	goal.eightPuzzel[1][1] = 4;
	goal.eightPuzzel[1][2] = 5;
	goal.eightPuzzel[2][0] = 6;
	goal.eightPuzzel[2][1] = 7;
	goal.eightPuzzel[2][2] = 8;

	print_WaysToGetGoal(best_first_search(state, goal));
	
	return 0;
}




