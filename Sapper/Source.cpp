#include <iostream>
#include <windows.h>
#include <vector>
#include <stack>
#include <ctime>
#include <conio.h>

using namespace std;

//1.
//���������� �������

class Game {

private:

	void show_logo(); //������ ����

public:

	void game_over(); //��������
	void you_won(); //������
	void run();
};

class Map {

private:

	int size;                       //������ ����, ������� �������
	vector <vector<int>> map;
	vector <vector<int>> mask;

public:

	Map();

	int open_cell(int x, int y);
	bool is_border(int x, int y);
	void init_vector(vector <vector<int>>& vec);
	void init_map();
	void init_mask();
	void cout_color(char ch, int color);
	void show();
	void set_mines(int mines_count);
	void set_digits();
	void fill(int px, int py);
	bool is_over();
};

class Keyboard {

private:

	int ch = 0;

public:

	Keyboard();

	void wait_key();
	int get_key();
};

class Cursor {

private:

	int x = 1;
	int y = 1;
	int tx = 1;
	int ty = 1;

public:

	void save();
	void undo();
	void ink_x();
	void dec_x();
	void ink_y();
	void dec_y();
	int get_x();
	int get_y();
	void move();
};


//2.
//����� �������

//������ ������� �� x ,y
void gotoxy(int x, int y) {        

	COORD p = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);

}

//������������� ���� ������ � ���� � �������
void set_color(int background, int text) {

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));

}

//�������� ������ � ������ ����������
const int empty_cell = 0;     //������ ������
const int border = 100;       //������� ����
const int mine = 10;          //����
const int mine_count = 2;    //���-�� ���



void Game::show_logo() {

	gotoxy(55, 14);
	cout << "Sapper" << endl;
	Sleep(2000);
	system("cls");
}

void Game::game_over() {

	gotoxy(55, 14);
	cout << "Game over" << endl;
	Sleep(2000);
	gotoxy(0, 25);
	system("pause");
}

void Game::you_won() {

	gotoxy(55, 14);
	cout << "You won!!!" << endl;
	Sleep(2000);
	gotoxy(0, 25);
	system("pause");
}

void Game::run() {
	show_logo();
	Map map;
	map.init_map();
	map.init_mask();
	map.set_mines(mine_count);
	map.set_digits();
	map.show();

	Keyboard kb;
	Cursor cs;

	cs.move();

	bool exit = false;

	while (!exit) {
		kb.wait_key();
		cs.save();

		switch (kb.get_key()){      //���� ������ �� ����������
		case 77: cs.ink_x(); break; //������   
		case 75: cs.dec_x(); break; //�����
		case 80: cs.ink_y(); break; //����
		case 72: cs.dec_y(); break; //�����
		case 13:					//������������� ������
			int result = map.open_cell(cs.get_x(), cs.get_y());
			if (result == mine) {
				game_over();
				exit = true;
			}
			if (result == empty_cell) {
				map.fill(cs.get_x(), cs.get_y());
				map.show();
			}
			if (map.is_over())
				you_won();
			break;
		}
		if (map.is_border(cs.get_x(), cs.get_y())) {
			cs.undo();
		}
		cs.move();
	}

}




Map::Map() {
	size = 8;
}

int Map::open_cell(int x, int y) {         //�������� ������
	int result = 1;
	mask[x][y] = 1;
	if (map[x][y] == mine) {          //������� ��������� �� ����
		result = mine;
	}
	else if (map[x][y] == empty_cell) {          //������� ��������� �� ������ ������
		result = empty_cell;
	}
	show();
	return result;
}
	
bool Map::is_border(int x, int y) {         //�������� "�������� �� ������� ������ ��������"

	if (x < 0 || x >= size)
		return false;
	if (y < 0 || y >= size)
		return false;
	if (map[x][y] == border) {
		return true;
	}
	return false;
}

//������ ��� map � mask
void Map::init_vector(vector <vector<int>>& vec) {
	for (int i = 0; i < size; i++) {
		vector <int> temp;
		for (int j = 0; j < size; j++) {
			if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
				temp.push_back(border);
			else
				temp.push_back(empty_cell);
		}
		vec.push_back(temp);
	}
}

//�������� �������� ����
void Map::init_map() {
	init_vector(map);
}

//�������� �����
void Map::init_mask() {
	init_vector(mask);
}

//����������� ������ �����
void Map::cout_color(char ch, int color) {
	set_color(15, color);
	cout << ch;
	set_color(15, 0);
}

//����� �������� ���� � ������� ��������
void Map::show() {
	gotoxy(0, 0);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {

			if (mask[j][i] == empty_cell) {
				cout << ".";
				continue;
			}

			if (map[j][i] == border)                   //����������� ������ �������� ����
				cout_color('#', 12);
			else if (map[j][i] == empty_cell)          //����������� ������ �����
				cout << " ";
			else if (map[j][i] == mine)                //����������� ����
				cout << "*";
			else if (map[j][i] == 1)
				cout_color('1', 1);
			else if (map[j][i] == 2)
				cout_color('2', 2);
			else if (map[j][i] == 3)
				cout_color('3', 13);
			else if (map[j][i] == 4)
				cout_color('4', 4);
			else if (map[j][i] == 5)
				cout_color('5', 5);
			else if (map[j][i] == 6)
				cout_color('6', 9);
			else if (map[j][i] == 7)
				cout_color('7', 8);
			else if (map[j][i] == 8)
				cout_color('8', 14);
		}
		cout << endl;
	}
}

//��������� ����������� ���
void Map::set_mines(int num_mines) {

	//�������� ���������� ���, ������� ����� ���������� �� ����
	if (num_mines >= (size - 2) * (size - 2)) {
		cout << "too many mines " << endl;
		return;
	}

	for (int i = 0; i < num_mines; i++)
	{
		int x = 0;
		int y = 0;

		//����� ������ ������, ��������� �����
		do {
			x = rand() % (size - 2) + 1;
			y = rand() % (size - 2) + 1;
		} while (map[x][y] == mine);
		map[x][y] = mine;
	}
}

//����������� ����� �� ������� ����
void Map::set_digits() {
	int d = 0;
	for (int i = 1; i < size - 1; i++) {
		for (int j = 1; j < size - 1; j++) {

			if (map[j][i] == mine)
				continue;
			if (map[j][i + 1] == mine)
				d++;
			if (map[j][i - 1] == mine)
				d++;
			if (map[j + 1][i + 1] == mine)
				d++;
			if (map[j + 1][i - 1] == mine)
				d++;
			if (map[j - 1][i + 1] == mine)
				d++;
			if (map[j - 1][i - 1] == mine)
				d++;
			if (map[j + 1][i] == mine)
				d++;
			if (map[j - 1][i] == mine)
				d++;
			map[j][i] = d;
			d = 0;
		}
	}
}

//����� ��� "�������" ��� ��������� �� ������ ������
void Map::fill(int px, int py) {
	stack <int> stk;
	stk.push(px);
	stk.push(py);
	int x = 0, y = 0;

	while (true) {
		y = stk.top();
		stk.pop();
		x = stk.top();
		stk.pop();

		if (map[x][y + 1] == empty_cell && mask[x][y + 1] == 0) {
			stk.push(x);
			stk.push(y + 1);
		}
		mask[x][y + 1] = 1;

		if (map[x][y - 1] == empty_cell && mask[x][y - 1] == 0) {
			stk.push(x);
			stk.push(y - 1);
		}
		mask[x][y - 1] = 1;

		if (map[x + 1][y + 1] == empty_cell && mask[x + 1][y + 1] == 0) {
			stk.push(x + 1);
			stk.push(y + 1);
		}
		mask[x + 1][y + 1] = 1;

		if (map[x + 1][y - 1] == empty_cell && mask[x + 1][y - 1] == 0) {
			stk.push(x + 1);
			stk.push(y - 1);
		}
		mask[x + 1][y - 1] = 1;

		if (map[x - 1][y - 1] == empty_cell && mask[x - 1][y - 1] == 0) {
			stk.push(x - 1);
			stk.push(y - 1);
		}
		mask[x - 1][y - 1] = 1;

		if (map[x - 1][y + 1] == empty_cell && mask[x - 1][y + 1] == 0) {
			stk.push(x - 1);
			stk.push(y + 1);
		}
		mask[x - 1][y + 1] = 1;

		if (map[x + 1][y] == empty_cell && mask[x + 1][y] == 0) {
			stk.push(x + 1);
			stk.push(y);
		}
		mask[x + 1][y] = 1;

		if (map[x - 1][y] == empty_cell && mask[x - 1][y] == 0) {
			stk.push(x - 1);
			stk.push(y);
		}
		mask[x - 1][y] = 1;

		if (stk.empty())
			break;
	}
}

bool Map::is_over() {

	int count = 0;
	for (int i = 1; i < size - 1; i++) {
		for (int j = 1; j < size - 1; j++) {
			if (mask[i][j] == 0)
				count++;
		}
	}

	if (count == mine_count)
		return 1;
	return 0;
}



Keyboard:: Keyboard() {

	ch = 0;
}

void Keyboard::wait_key() {       //��� ����� � ����������

	ch = _getch();
}

int Keyboard::get_key() {        //�������� ���� � ����������

	return ch;
}



void Cursor::save() {

	tx = x;
	ty = y;
}

void Cursor::undo() {

	x = tx;
	y = ty;
}

void Cursor::ink_x() {

	x++;
}

void Cursor::dec_x() {

	x--;
}

void Cursor::ink_y() {

	y++;
}

void Cursor::dec_y() {

	y--;
}

int Cursor::get_x() {

	return x;
}

int Cursor::get_y() {

	return y;
}

void Cursor::move() {

	gotoxy(x, y);
}




int main()
{
	srand(time(0));
	Game game;
	game.run();
}

