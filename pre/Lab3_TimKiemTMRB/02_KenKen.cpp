#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

#define NB_Rows 4 // số hàng trong kenken
#define NB_Columns 4 // số cột trong kenken
#define MaxValue 5 // số giá trị tối đa trong một ô kenken
#define Empty 0 // ô trống được đánh dấu là empty với giá trị là 0
#define AreaSquareSize 1 // cạnh của mỗi khối (3 ô)
#define INF 99999

// định nghĩa kiểu tọa độ - (x, y)
typedef struct {
	int x, y;
} Coord;

void printListCoord(vector<Coord> coordList){
	for(int i = 0; i < (int)coordList.size(); i++){
		Coord pos = coordList.at(i);
		printf("(%d %d)\n", pos.x, pos.y);
	}
}

// định nghĩa ràng buộc - được thể hiện qua ma trận đỉnh-đỉnh
typedef struct {
	int data[NB_Rows * NB_Columns][NB_Rows * NB_Columns];
	int n;
} Constrains;

void initConstrains(Constrains *constrains){
	for(int i = 0; i < NB_Rows * NB_Columns; i++){
		for(int j = 0; j < NB_Rows * NB_Columns; j++){
			constrains->data[i][j] = 0;
		}
	}
	constrains->n = NB_Rows * NB_Columns;
}

// chuyển từ tọa độ sang chỉ mục(stt) của ô - (số_ô_trong_một_hàng*x + y)
int indexOf(Coord coord){
	return NB_Rows * coord.x + coord.y;
}

/*
	chuyển từ chỉ mục(stt) của ô sang tọa độ
	x = stt / số ô trong một hàng
	y = stt % số ô trong một hàng
*/
Coord positionOfVertex(int vertex){
	Coord coord;
	coord.x = vertex / NB_Rows;
	coord.y = vertex % NB_Rows;
	return coord;
}

// thêm ràng buộc cho 2 tọa độ, nếu đã có ràng buộc này trước đó thì trả về 0
int addConstrain(Constrains *constrains, Coord source, Coord target){
	int u = indexOf(source);
	int v = indexOf(target);

	if(constrains->data[u][v] == 0){
		constrains->data[u][v] = 1;
		constrains->data[v][u] = 1;
		return 1;
	}

	return 0;
}

// lấy danh sách các ô có ràng buộc với một ô cho trước
vector<Coord> getConstrains(Constrains constrains, Coord coord){
	int v = indexOf(coord);
	vector<Coord> result;
	for(int i = 0; i < constrains.n; i++){
		if(constrains.data[v][i] == 1){
			result.push_back(positionOfVertex(i));
		}
	}
	return result;
}

/*
	định nghĩa kiểu rage:
	- danh sách tọa độ các ô trong rage
	- kết quả của rage
	- phép toán thực hiện trong rage
*/
typedef struct {
	vector<Coord> listCoord;
	int result;
	char opr;
} Rage;

void initRage(Rage *rage){
	rage->listCoord.clear();
	rage->result = 0;
	rage->opr = 'n';
}

/*
	định nghĩa kenken
	- các ô trong kenken là một ma trận
	- các ràng buộc giữa các ô
	- danh sách rage
	- ma trận đánh dấu các cell theo các theo stt rage
	- số lượng rage
*/
typedef struct {
	int cells[NB_Rows][NB_Columns];
	Constrains constrains;
	vector<Rage> rageList;
	int numsRage[NB_Rows][NB_Columns];
	int NB_Rages;
} KenKen;

// cập nhật danh sách rage dựa trên kenKen và index của rage
void updateListRage(KenKen kenKen, Rage *rage, int index){
	rage->listCoord.clear();
	for(int i = 0; i < NB_Rows; i++){
		for(int j = 0; j < NB_Columns; j++){
			if(kenKen.numsRage[i][j] == index){
				Coord pos = {i, j};
				rage->listCoord.push_back(pos);
			}
		}
	}
}

void initNumsRage(KenKen *kenKen){
	for(int i = 0; i < NB_Rows; i++){
		for(int j = 0; j < NB_Columns; j++){
			kenKen->cells[i][j] = 0;
		}
	}
}

void initKenKen(KenKen *kenKen){
	int i, j;
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; j < NB_Columns; j++){
			kenKen->cells[i][j] = Empty;
		}
	}
	initConstrains(&kenKen->constrains);
	kenKen->rageList.clear();
	initNumsRage(kenKen);
	kenKen->NB_Rages = 0;
}

void printKenKen(KenKen kenKen){
	printf("KenKen:\n");
	for(int i = 0; i < NB_Rows; i++){
		if(i % AreaSquareSize == 0) printf("-----------------\n");
		for(int j = 0; j < NB_Columns; j++){
			if(j % AreaSquareSize == 0) printf("| ");
			printf("%d ", kenKen.cells[i][j]);
		}
		printf("|\n");
	}
	printf("-----------------\n");
}

void printRage(KenKen kenKen){
	printf("NumsRage:\n");
	for(int i = 0; i < NB_Rows; i++){
		if(i % AreaSquareSize == 0) printf("-----------------\n");
		for(int j = 0; j < NB_Columns; j++){
			if(j % AreaSquareSize == 0) printf("| ");
			printf("%d ", kenKen.numsRage[i][j]);
		}
		printf("|\n");
	}
	printf("-----------------\n");

	for(int i = 0; i < (int)kenKen.rageList.size(); i++){
		printf("%d: %d%c\n", i + 1, kenKen.rageList.at(i).result, kenKen.rageList.at(i).opr);
	}
}

int getRageNumber(Coord pos, vector<Rage> rageList){
	for(int i = 0; i < (int)rageList.size(); i++){ // duyệt qua các rage trong rageList
		vector<Coord> listCoord = rageList.at(i).listCoord;
		for(int j = 0; j < (int)listCoord.size(); j++){ // duyệt qua các coord trong coordList của rage
			if(pos.x == listCoord.at(j).x && pos.y == listCoord.at(j).y)
				return i;
		}
	}
	return 0;
}

// kiểm tra xem kenken đã được điền đầy đủ chưa
int isFilledSudoku(KenKen kenKen){
	for(int i = 0; i < NB_Rows; i++){
		for(int j = 0; j < NB_Columns; j++){
			if(kenKen.cells[i][j] == Empty) return 0;
		}
	}
	return 1;
}

// lan truyền ràng buộc từ một ô - và lưu lại các ô được thêm ràng buộc vào một danh sách (changeds)
void spreadConstrainsFrom(Coord position, KenKen *kenKen, vector<Coord> &changeds){
	int row = position.x;
	int column = position.y;

	// lan truyền ràng buộc theo cột
	for(int i = 0; i < NB_Rows; i++){
		if(i != row){ // nếu khác hàng của ô gốc
			Coord pos = {i, column};
			if(addConstrain(&kenKen->constrains, position, pos))
				changeds.push_back(pos);
		}
	}

	// lan truyền ràng buộc theo hàng
	for(int i = 0; i < NB_Columns; i++){
		if(i != column){ // nếu khác cột của ô gốc
			Coord pos = {row, i};
			if(addConstrain(&kenKen->constrains, position, pos))
				changeds.push_back(pos);
		}
	}

	// lan truyền ràng buộc theo rage

	// xác định chỉ mục của rage
	int indexRage = getRageNumber(position, kenKen->rageList);
	vector<Coord> listCoord = kenKen->rageList.at(indexRage).listCoord;
	for(int i = 0; i < (int)listCoord.size(); i++){
		Coord pos = listCoord.at(i);
		if(pos.x != row && pos.y != column){
			if(addConstrain(&kenKen->constrains, position, pos))
				changeds.push_back(pos);
		}
	}	
}

// tìm miền giá trị của một ô trống
vector<int> getAvailableValues(Coord position, KenKen kenKen){
	// lấy danh sách các đỉnh có ràng buộc
	vector<Coord> posList = getConstrains(kenKen.constrains, position);
	int availables[MaxValue]; //mảng đánh dấu miền giá trị hợp lệ
	for(int i = 1; i < MaxValue; i++){
		availables[i] = 1;
	}

	// nếu ô có ràng buộc đã có giá trị thì loại giá trị đó ra miền hợp lệ
	for(int i = 0; i < (int)posList.size(); i++){
		Coord pos = posList.at(i);
		if(kenKen.cells[pos.x][pos.y] != Empty){
			availables[kenKen.cells[pos.x][pos.y]] = 0;
		}
	}

	// chuyển mảng đánh dấu thành vector
	vector<int> result;
	for(int i = 1; i < MaxValue; i++){
		if(availables[i])
			result.push_back(i);
	}

	return result;
}

// xác định ô ưu tiên được điền trước
//C1: ô trống đầu tiên
Coord getNextEmptyCell(KenKen kenKen){
	for(int i = 0; i < NB_Rows; i++){
		for(int j = 0; i < NB_Columns; j++){
			Coord pos = {i, j};
			if(kenKen.cells[i][j] == Empty) return pos;
		}
	}

	return {-1, -1};// lưu ý
}

//C2: ô trống có miền giá trị thấp nhất
Coord getNextMinDomainCell(KenKen kenKen){
	int minLength = INF;
	Coord result;
	for(int i = 0; i < NB_Rows; i++){
		for(int j = 0; j < NB_Columns; j++){
			if(kenKen.cells[i][j] == Empty){
				Coord pos = {i, j};
				int availablesLength = getAvailableValues(pos, kenKen).size();
				if(availablesLength < minLength){
					minLength = availablesLength;
					result = pos;
				}
			}
		}
	}
	return result;
}

int checkRageConstrains(vector<int> values, int resultOfRage, char opr){
	int result = 0;

	// sắp xếp danh sách vector từ lớn đến bé
	sort(values.rbegin(), values.rend());

	if(opr == '+'){
		for(int i = 0; i < (int)values.size(); i++){
			result += values.at(i);
		}
	} else if(opr == '-'){
		result = values.at(0);
		for(int i = 1; i < (int)values.size(); i++){
			result -= values.at(i);
		}
	} else if(opr == '*'){
		result = 1;
		for(int i = 0; i < (int)values.size(); i++){
			result *= values.at(i);
		}
	} else if(opr == '/'){
		result = values.at(0);
		for(int i = 1; i < (int)values.size(); i++){
			result /= values.at(i);
		}
	}

	if(result != resultOfRage) return 0;
	return 1;
}


vector<int> values[NB_Rows * NB_Columns];
int exploredCounter = 0; // số các trường hợp tìm được
// giải thuật quay lui
int kenKenBackTracking(KenKen *kenKen){
	//nếu kenken đã được điền đầy đủ => hoàn thành
	if(isFilledSudoku(*kenKen)) return 1;

	// xác định tọa độ của ô ưu tiên được điền trước
	//	C1: Coord position = getNextEmptyCell(*kenKen);
	//	C2:
	Coord position = getNextMinDomainCell(*kenKen);

	// xác định miền giá trị của ô
	vector<int> availables = getAvailableValues(position, *kenKen);

	// xác định chỉ mục rage của ô
	int indexRage = getRageNumber(position, kenKen->rageList);

	// xác định size của Rage chứa ô
	int rageSize = kenKen->rageList.at(indexRage).listCoord.size();

	// duyệt qua các giá trị trong hợp lệ

	for(int j = 0; j < (int)availables.size(); j++){

		// gán lần giá trị tim được cho ô
		kenKen->cells[position.x][position.y] = availables.at(j);
		values[indexRage].push_back(availables.at(j));
		exploredCounter++; // mỗi lần gán sinh thêm một trạng thái mới

		// kiểm tra xem nếu rage đã được điền đầy đủ thì có thỏa mãn phép toán không
		if((int)values[indexRage].size() == rageSize){
			//nếu không thỏa mãn thì đặt lại và đặt giá trị hợp lệ tiếp theo
			if(!checkRageConstrains(values[indexRage], kenKen->rageList.at(indexRage).result, kenKen->rageList.at(indexRage).opr)){
				values[indexRage].pop_back();
				kenKen->cells[position.x][position.y] = Empty;
				continue;
			}
		}

		// nếu phép gán trên dẫn đến trạng thái đích => kết thúc
		if(kenKenBackTracking(kenKen)) return 1;
		//ngược lại, xóa giá trị vừa gán => gán giá trị hợp lệ tiếp theo
		values[indexRage].pop_back();
		kenKen->cells[position.x][position.y] = Empty;
	}

	// nếu gán tất cả các giá trị hợp lệ mà không tìm được trạng thái đích => thất bại
	return 0;
}

// xử lí kenken
KenKen solveKenKen(KenKen kenKen){
	initConstrains(&kenKen.constrains);
	//duyệt qua tất cả các ô trong kenken => thêm các ràng buộc
	for(int i = 0; i < NB_Rows; i++){
		for(int j = 0; j < NB_Columns; j++){
			vector<Coord> history; // tạo danh sách lưu lại các tọa độ đã thêm ràng buộc 
			Coord pos = {i, j};
			spreadConstrainsFrom(pos, &kenKen, history);
		}
	}

	exploredCounter = 0; // khởi tạo giá trị cho bộ đếm trạng thái tìm được
	
	// khởi tạo values
	for(int i = 0; i < NB_Rows * NB_Columns; i++){
		values[i].clear();
	}

	if(kenKenBackTracking(&kenKen)) printf("Solved\n");
	else printf("Can not solve\n");
	printf("Explored %d states\n", exploredCounter);
	// trả về kenken sau khi đã xử lí
	return kenKen;
}



int main(){

	KenKen kenKen;
	initKenKen(&kenKen);
	// lấy inputs từ file txt
	freopen("data_kenken.txt", "r", stdin);
	int numsRage, res;
	char opr;

	for(int i = 0; i < NB_Rows; i++){
		for(int j= 0; j < NB_Columns; j++){
			int x;
			scanf("%d", &x);
			kenKen.numsRage[i][j] = x;
		}
	}

	scanf("%d\n", &numsRage);
	kenKen.NB_Rages = numsRage;

	for(int i = 0; i < numsRage; i++){
		Rage rage;
		initRage(&rage);
		kenKen.rageList.push_back(rage);
		updateListRage(kenKen, &kenKen.rageList.at(i), i + 1);
	}

	for(int i = 0; i < numsRage; i++){
		scanf("%d %c", &res, &opr);
		kenKen.rageList.at(i).result = res;
		kenKen.rageList.at(i).opr = opr;
	}

	Coord pos;
	for(int i = 0; i < numsRage; i++){
		if(kenKen.rageList.at(i).opr == 'n'){
			for(int j = 0; j < (int)kenKen.rageList.at(i).listCoord.size(); j++){
				pos = kenKen.rageList.at(i).listCoord.at(j);
				kenKen.cells[pos.x][pos.y] = kenKen.rageList.at(i).result;
			}
		}
	}

	printKenKen(kenKen);
	printRage(kenKen);
	KenKen result = solveKenKen(kenKen);
	printKenKen(result);

	return 0;
}
