#include <stdio.h>
#include <malloc.h>

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty 0
#define goal 6
#define Maxlength 100

typedef struct {
	int x, y;
} State;

void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
}

void print_State(State state){
	printf("\n     X:%d --- Y:%d\n", state.x, state.y);
}

int compareState(State state1, State state2){
	return state1.x == state2.x && state1.y == state2.y;
}

int goalcheck(State state){
	return (state.x == goal || state.y == goal);
}

int pourWaterFullX(State cur_state, State *result){
	if(cur_state.x < tankcapacity_X){
		result->x = tankcapacity_X;
		result->y = cur_state.y;
		return 1; 
	}
	return 0;
}

int pourWaterFullY(State cur_state, State *result){
	if(cur_state.y < tankcapacity_Y){
		result->x = cur_state.x;
		result->y = tankcapacity_Y;
		return 1; 
	}
	return 0;
}

int pourWaterEmptyX(State cur_state, State *result){
	if(cur_state.x > 0){
		result->x = empty;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

int pourWaterEmptyY(State cur_state, State *result){
	if(cur_state.y > 0){
		result->x = cur_state.x;
		result->y = empty;
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

int pourWaterXY(State cur_state, State *result){
	if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), empty);
		result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
		return 1;
	}
	return 0;
}

int pourWaterYX(State cur_state, State *result){
	if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
		result->x = min(cur_state.x + cur_state.y, tankcapacity_X);
		result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), empty);
		return 1;
	}
	return 0;
}

int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return pourWaterFullX(cur_state, result);
		case 2: return pourWaterFullY(cur_state, result);
		case 3: return pourWaterEmptyX(cur_state, result);
		case 4: return pourWaterEmptyY(cur_state, result);
		case 5: return pourWaterXY(cur_state, result);
		case 6: return pourWaterYX(cur_state, result);
		default:
			printf("Error calls operators");
			return 0;
	}
}

const char* action[] = {
	"First State",
	"pour Water Full X",
	"pour Water Full Y",
	"pour Water Empty X",
	"pour Water Empty Y",
	"pour Water X to Y",
	"pour Water Y to X"
};

typedef struct Node {
	State state;
	struct Node* Parent;
	int no_function;
} Node;


typedef struct {
	Node* Emlements[Maxlength];
	int Top_idx;
} Stack;

void makeNull_Stack(Stack *stack){
	stack->Top_idx = Maxlength;
}

int empty_Stack(Stack stack){
	return stack.Top_idx == Maxlength;
}

int full_Stack(Stack stack){
	return stack.Top_idx == 0;
}

void push(Node *x, Stack *stack){
	if(full_Stack(*stack))
		printf("Error! Stack is full");
	else {
		stack->Top_idx -= 1;
		stack->Emlements[stack->Top_idx] = x;
	}
}

Node* top(Stack stack){
	if(!empty_Stack(stack))
		return stack.Emlements[stack.Top_idx];
	return NULL;
}

void pop(Stack *stack){
	if(!empty_Stack(*stack))
		stack->Top_idx += 1;
	else printf("Error! Stack is empty");
}

typedef struct {
	Node* Emlements[Maxlength];
	int front, rear;
} Queue;

void makeNull_Queue(Queue *queue){
	queue->front = -1;
	queue->rear = -1;
}

int empty_Queue(Queue queue){
	return queue.front == -1;
}

int full_Queue(Queue queue){
	return ((queue.rear - queue.front + 1) % Maxlength) == 0;
}

void push_Queue(Node *x, Queue *queue){
	if(!full_Queue(*queue)){
		if(empty_Queue(*queue))
			queue->front = 0;
		queue->rear = (queue->rear + 1) % Maxlength;
		queue->Emlements[queue->rear] = x;
	} else printf("Error, Push");
}

Node* get_Font(Queue queue){
	if(empty_Queue(queue)){
		printf("Queue is empty");
		return NULL;
	}	
	return queue.Emlements[queue.front];
}

void del_Queue(Queue *queue){
	if(!empty_Queue(*queue)){
		if(queue->front == queue->rear)
			makeNull_Queue(queue);
		else queue->front = (queue->front + 1) % Maxlength;
	}
	else printf("Error, Delete");
}

int find_State(State state, Queue openQueue){
	while(!empty_Queue(openQueue)){
		if(compareState(get_Font(openQueue)->state, state))
			return 1;
		del_Queue(&openQueue);
	}
	return 0;
}

Node* BFS_Algorithm(State state){
	Queue Open_BFS;
	Queue Close_BFS;
	makeNull_Queue(&Open_BFS);
	makeNull_Queue(&Close_BFS);

	Node *root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	push_Queue(root, &Open_BFS);

	while(!empty_Queue(Open_BFS)){
		Node* node = get_Font(Open_BFS);
		del_Queue(&Open_BFS);
		push_Queue(node, &Close_BFS);

		if(goalcheck(node->state)) 
			return node;

		for(int opt = 1; opt <= 6; opt++){
			State newState;
			makeNullState(&newState);
			if(call_operator(node->state, &newState, opt)){
				if(find_State(newState, Close_BFS) || find_State(newState, Open_BFS))
					continue;

				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newState;
				newNode->Parent = node;
				newNode->no_function = opt;

				push_Queue(newNode, &Open_BFS);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node){
	Stack stackPrint;
	makeNull_Stack(&stackPrint);
	while(node->Parent != NULL){
		push(node, &stackPrint);
		node = node->Parent;
	}
	push(node, &stackPrint);

	int no_action = 0;
	while(!empty_Stack(stackPrint)){
		printf("\nAction %d: %s", no_action, action[top(stackPrint)->no_function]);
		print_State(top(stackPrint)->state);
		pop(&stackPrint);
		no_action++;
	}
}

int main(){
	State cur_state = {0, 0};
	print_WaysToGetGoal(BFS_Algorithm(cur_state));
	return 0;
}
