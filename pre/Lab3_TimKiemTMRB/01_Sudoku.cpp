#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

#define NB_Rows 9 // số hàng trong sudoku
#define NB_Columns 9 // số cột trong sudoku
#define MaxValue 10 // số giá trị tối đa trong một ô sudoku
#define Empty 0 // ô trống được đánh dấu là empty với giá trị là 0
#define AreaSquareSize 3 // cạnh của mỗi khối (3 ô)
#define INF 99999

// định nghĩa kiểu tọa độ - (x, y)
typedef struct {
	int x, y;
} Coord;

// định nghĩa ràng buộc - được thể hiện qua ma trận đỉnh-đỉnh
typedef struct {
	int data[NB_Rows * NB_Columns][NB_Rows * NB_Columns];
	int n;
} Constrains;

void initConstrains(Constrains *constrains){
	int i, j;
	for(i = 0; i < NB_Rows * NB_Columns; i++){
		for(j = 0; j < NB_Rows * NB_Columns; j++){
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
	int i;
	int v = indexOf(coord);
	vector<Coord> result;
	for(i = 0; i < constrains.n; i++){
		if(constrains.data[v][i] == 1){
			result.push_back(positionOfVertex(i));
		}
	}
	return result;
}

/*
	định nghĩa sudoku
	- các ô trong sudoku la một ma trận
	- các ràng buộc giữa các ô
*/
typedef struct {
	int cells[NB_Rows][NB_Columns];
	Constrains constrains;
} Sudoku;

void initSudoku(Sudoku *sudoku){
	int i, j;
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; j < NB_Columns; j++){
			sudoku->cells[i][j] = Empty;
		}
	}
	initConstrains(&sudoku->constrains);
}

void initSudokuWithValue(Sudoku *sudoku, int inputs[NB_Rows][NB_Columns]){
	int i, j;
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; j < NB_Columns; j++){
			sudoku->cells[i][j] = inputs[i][j];
		}
	}
	initConstrains(&sudoku->constrains);
}

void printSudoku(Sudoku sudoku){
	int i, j;
	printf("Sudoku:\n");
	for(i = 0; i < NB_Rows; i++){
		if(i % AreaSquareSize == 0) printf("-------------------------\n");
		for(j = 0; j < NB_Columns; j++){
			if(j % AreaSquareSize == 0) printf("| ");
			printf("%d ", sudoku.cells[i][j]);
		}
		printf("|\n");
	}
	printf("-------------------------\n");
}

// kiểm tra xem sudoku đã được điền đầy đủ chưa
int isFilledSudoku(Sudoku sudoku){
	int i, j;
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; j < NB_Columns; j++){
			if(sudoku.cells[i][j] == Empty) return 0;
		}
	}
	return 1;
}

// lan truyền ràng buộc từ một ô - và lưu lại các ô được thêm ràng buộc vào một danh sách (changeds)
void spreadConstrainsFrom(Coord position, Constrains *constrains, vector<Coord> &changeds){
	int row = position.x;
	int column = position.y;

	int i, j;
	// lan truyền ràng buộc theo cột
	for(i = 0; i < NB_Rows; i++){
		if(i != row){ // nếu khác hàng của ô gốc
			Coord pos = {i, column};
			if(addConstrain(constrains, position, pos))
				changeds.push_back(pos);
		}
	}

	// lan truyền ràng buộc theo hàng
	for(i = 0; i < NB_Columns; i++){
		if(i != column){ // nếu khác cột của ô gốc
			Coord pos = {row, i};
			if(addConstrain(constrains, position, pos))
				changeds.push_back(pos);
		}
	}

	// lan truyền ràng buộc theo khối

	// tìm tọa độ cell đầu tiên trong khối - (areaX, areaY)
	int areaX = (row / AreaSquareSize) * AreaSquareSize; //hàng đầu tiên của khối chứa ô gốc
	int areaY = (column / AreaSquareSize) * AreaSquareSize; //cột đầu tiên của khối chứa ô gốc
	for(i = 0; i < AreaSquareSize; i++){
		for(j = 0; j < AreaSquareSize; j++){
			if(areaX + i != row || areaY + j != column){ // nếu khác ô gốc
				Coord pos = {areaX + i, areaY + j};
				if(addConstrain(constrains, position, pos))
					changeds.push_back(pos);
			}
		}
	}
}

// tìm miền giá trị của một ô trống
vector<int> getAvailableValues(Coord position, Sudoku sudoku){
	// lấy danh sách các đỉnh có ràng buộc
	vector<Coord> posList = getConstrains(sudoku.constrains, position);
	int availables[MaxValue]; //mảng đánh dấu miền giá trị hợp lệ
	int i;
	for(i = 1; i < MaxValue; i++){
		availables[i] = 1;
	}

	// nếu ô có ràng buộc đã có giá trị thì loại giá trị đó ra miền hợp lệ
	for(i = 0; i < (int)posList.size(); i++){
		Coord pos = posList.at(i);
		if(sudoku.cells[pos.x][pos.y] != Empty){
			availables[sudoku.cells[pos.x][pos.y]] = 0;
		}
	}

	// chuyển mảng đánh dấu thành vector
	vector<int> result;
	for(i = 1; i < MaxValue; i++){
		if(availables[i])
			result.push_back(i);
	}

	return result;
}

// xác định ô ưu tiên được điền trước
//C1: ô trống đầu tiên
Coord getNextEmptyCell(Sudoku sudoku){
	int i, j;
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; i < NB_Columns; j++){
			Coord pos = {i, j};
			if(sudoku.cells[i][j] == Empty) return pos;
		}
	}

	return {-1, -1};// lưu ý
}

//C2: ô trống có miền giá trị thấp nhất
Coord getNextMinDomainCell(Sudoku sudoku){
	int minLength = INF;
	int i, j;
	Coord result;
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; j < NB_Columns; j++){
			if(sudoku.cells[i][j] == Empty){
				Coord pos = {i, j};
				int availablesLength = getAvailableValues(pos, sudoku).size();
				if(availablesLength < minLength){
					minLength = availablesLength;
					result = pos;
				}
			}
		}
	}
	return result;
}


int exploredCounter = 0; // số các trường hợp tìm được
// giải thuật quay lui
int sudokuBackTracking(Sudoku *sudoku){
	//nếu sudoku đã được điền đầy đủ => hoàn thành
	if(isFilledSudoku(*sudoku)) return 1;

	// xác định tọa độ của ô ưu tiên được điền trước
	//	C1: Coord position = getNextEmptyCell(*sudoku);
	//	C2:
	Coord position = getNextMinDomainCell(*sudoku);

	// xác định miền giá trị của ô
	vector<int> availables = getAvailableValues(position, *sudoku);

	// duyệt qua các giá trị trong hợp lệ
	int j;
	for(j = 0; j < (int)availables.size(); j++){
		// gán lần giá trị tim được cho ô
		sudoku->cells[position.x][position.y] = availables.at(j);;
		exploredCounter++; // mỗi lần gán sinh thêm một trạng thái mới

		// nếu phép gán trên dẫn đến trạng thái đích => kết thúc
		if(sudokuBackTracking(sudoku)) return 1;
		//ngược lại, xóa giá trị vừa gán => gán giá trị hợp lệ tiếp theo
		sudoku->cells[position.x][position.y] = Empty;
	}

	// nếu gán tất cả các giá trị hợp lệ mà không tìm được trạng thái đích => thất bại
	return 0;
}

// xử lí sudoku
Sudoku solveSudoku(Sudoku sudoku){
	int i, j;
	initConstrains(&sudoku.constrains);
	//duyệt qua tất cả các ô trong sudoku => thêm các ràng buộc
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; j < NB_Columns; j++){
			vector<Coord> history; // tạo danh sách lưu lại các tọa độ đã thêm ràng buộc 
			Coord pos = {i, j};
			spreadConstrainsFrom(pos, &sudoku.constrains, history);
		}
	}

	exploredCounter = 0; // khởi tạo giá trị cho bộ đếm trạng thái tìm được
	if(sudokuBackTracking(&sudoku)) printf("Solved\n");
	else printf("Can not solve\n");
	printf("Explored %d states\n", exploredCounter);
	// trả về sudoku sau khi đã xử lí
	return sudoku;
}



int main(){

	// lấy inputs từ file txt
	freopen("data_sudoku.txt", "r", stdin);
	int inputs[NB_Rows][NB_Columns];
	int i, j;
	for(i = 0; i < NB_Rows; i++){
		for(j = 0; j < NB_Columns; j++){
			scanf("%d", &inputs[i][j]);
		}
	}

	Sudoku sudoku;
	initSudokuWithValue(&sudoku, inputs);
	printSudoku(sudoku);
	Sudoku result = solveSudoku(sudoku);
	printSudoku(result);

	return 0;
}
