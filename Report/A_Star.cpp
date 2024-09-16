#include <stdio.h>
#include <string>
#include <vector>
#include <malloc.h>
#include <algorithm>
#include <stack>

using namespace std;

typedef struct {
	char u, v;
	int g;
} Edge;

vector<char> point_list;
vector<int> h_list;
vector<Edge> edge_list;

void readFileTxt(string filename, vector<char> *point_list, vector<int> *h_list, vector<Edge> *edge_list, char *start, char* goal){ 
	freopen(filename.c_str(), "r", stdin);

	int n, m;
	scanf("%d %d\n", &n, &m);

	char node;
	int h;
	for(int i = 0; i < n; i++){
		scanf("%c %d\n", &node, &h);
		point_list->push_back(node);
		h_list->push_back(h);
	}

	char u, v;
	int g;
	for(int i = 0; i < m; i++){
		scanf("%c %c %d\n", &u, &v, &g);
		Edge edge = {u, v, g};
		edge_list->push_back(edge);
	}

	scanf("%c %c", start, goal);
}

typedef struct Node {
	char point;
	struct Node* parent;
	int h, g, f;
} Node;

int get_heuristic(char point){
	for(int i = 0; i < point_list.size(); i++){
		if(point == point_list.at(i))
			return h_list.at(i);
	}
	return -1;
}

int get_cost(int ut, int vt){
	for(int i = 0; i < edge_list.size(); i++){
		int u = edge_list.at(i).u;
		int v = edge_list.at(i).v;
		if(ut == u && vt == v) return edge_list.at(i).g;
		if(ut == v && vt == u) return edge_list.at(i).g;
	}
	return -1;
}

vector<char> get_neighbors(char point){
	vector<char> neighbors;
	for(int i = 0; i < edge_list.size(); i++){
		char u = edge_list.at(i).u;
		char v = edge_list.at(i).v;
		if(point == u) neighbors.push_back(v);
		else if(point == v) neighbors.push_back(u);
	}
	return neighbors;
}

Node* find_point_in_list(char point, vector<Node*> list, vector<Node*>::iterator *pos){
	if(list.size() == 0) return NULL;

	vector<Node*>::iterator it = list.begin();
	while(it != list.end()){
		if((*it)->point == point){
			*pos = it;
			return *it;
		}

		it = list.erase(it);
	}
	return NULL;
}

bool compare_f(Node* a, Node* b){
	return a->f > b->f;
}

Node* AStarAlgorithm(char start, char goal, vector<char> *traversing_order){
	vector<Node*> openList;
	vector<Node*> closeList;

	Node* root = (Node*)malloc(sizeof(Node));
	root->point = start;
	root->parent = NULL;
	root->h = get_heuristic(start);
	root->g = 0;
	root->f = root->h + root->g;

	openList.push_back(root);

	while(!openList.empty()){
		Node* node = openList.back();
		openList.pop_back();
		closeList.push_back(node);

		traversing_order->push_back(node->point);
		if(node->point == goal) return node;

		vector<char> neighbors = get_neighbors(node->point);
		for(int v = 0; v < neighbors.size(); v++){
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->point = neighbors.at(v);
			newNode->parent = node;
			newNode->h = get_heuristic(newNode->point);
			newNode->g = newNode->parent->g + get_cost(newNode->parent->point, newNode->point);
			newNode->f = newNode->h + newNode->g;

			vector<Node*>::iterator posOpen, posClose;
			Node* nodeFoundOpen = find_point_in_list(newNode->point, openList, &posOpen);
			Node* nodeFoundClose = find_point_in_list(newNode->point, closeList, &posClose);

			if(nodeFoundOpen == NULL && nodeFoundClose == NULL){
				openList.push_back(newNode);
			} else if(nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g){
				openList.erase(posOpen);
				openList.push_back(newNode);
			} else if(nodeFoundClose != NULL && nodeFoundClose->g > newNode->g){
				closeList.erase(posClose);
				openList.push_back(newNode);
			} 

			sort(openList.begin(), openList.end(), compare_f);
		}
	}

	return NULL;
}

void print_way_to_goal(Node *node){
	stack<Node*> path;

	while(node->parent != NULL){
		path.push(node);
		node = node->parent;
	}
	path.push(node);

	printf("\nPath to goal: ");
	while(path.size() > 1){
		printf("%c -> ", path.top()->point);
		path.pop();
	}
	printf("%c", path.top()->point);
	path.pop();
}


int main(){

	char start, goal;
	readFileTxt("map_data.txt", &point_list, &h_list, &edge_list, &start, &goal);

	vector<char> traversing_order;

	Node* node = AStarAlgorithm(start, goal, &traversing_order);
	
	printf("Traversing Order: ");
	for(int i = 0; i < traversing_order.size() - 1; i++){
		printf("%c -> ", traversing_order.at(i));
	}
	printf("%c", traversing_order.back());

	print_way_to_goal(node);

	printf("\nTotal cost: %d", node->g);
	
	return 0;
}