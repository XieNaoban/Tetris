//俄罗斯方块
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<ctime>
using namespace std;
/**********************************/
//根据小键盘,将8、2、4、6定义为上下左右,5为默认,0为退出游戏
#define Sb 16//Screen的宽度
#define Sh 21//Screen的高度
bool Screen[Sh + 3][Sb + 2] = { 0 };
unsigned Speed_mulriple_limit = 2;
unsigned Speed_mulriple = 8;
unsigned SleepTime = 100;
unsigned Score = 0;
unsigned Max=0;
struct Tetris {
	bool shape[4][4];
	unsigned type;
	unsigned x, y;
}Tnow;
/**********************************/
void Start();
void Game();
void End();
void Interface();
void ScreenPrint();
void ScreenClear();
void NextShow(unsigned);
void Pause();
void TBuild(Tetris&, unsigned);
void TRotate(Tetris&);
void TMove(Tetris&, int);
void TDown(Tetris&);
void TCopy(Tetris&, const Tetris&);
void TShow(const Tetris&);
void TClear(const Tetris&);
int Judge_down(const Tetris&);
int Judge_key();
int Judge_eliminate(const Tetris&);
int Judge_gameover(const Tetris&);
void Pos(unsigned, unsigned);
void PosScreen(unsigned, unsigned);
void Swap(bool&, bool&);
/**********************************/
void Start() {
	system("mode con cols=80 lines=25");
	const CONSOLE_CURSOR_INFO cursor{ (DWORD)100,FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	srand((unsigned)time(0));
	ifstream i("tetris_data.dat", ios_base::binary);
	if (!i) i.close();
	else {
		unsigned s;
		i >> s;
		Max = s;
		i.close();
	}
	for (int i = 0;i < Sb + 2;++i)Screen[Sh + 2][i] = Screen[Sh + 1][i] = 1;
	for (int i = 0;i < Sh + 2;++i)Screen[i][0] = Screen[i][Sb + 1] = 1;
	Pos(12, 16);cout << "要开始游戏,请先关闭输入法,再";system("pause");system("cls");
	Interface();
}
void Game() {
	unsigned type_now = rand() % 7;
	unsigned type_next = rand() % 7;
	TBuild(Tnow, type_now);
	NextShow(type_next);
	Tetris Tp;//记录前面一步以方便删除轨迹
	TCopy(Tp, Tnow);
	int count = 1;
	int speed_count = 1;
	while (++count) {
		if (Judge_down(Tnow)) {
			Tp.type = 233;
			ScreenPrint();
			if (int lines=Judge_eliminate(Tnow)) {
				Score += 15*lines-5;
				if (Score > Max)Max = Score;
				++speed_count;
				speed_count %= 6;
				if (!speed_count&&Speed_mulriple>Speed_mulriple_limit)Speed_mulriple -= 1;
				Pos(14, 56);cout << Score;
				Pos(15, 56);cout << SleepTime*Speed_mulriple << 's';
				Pos(16, 58);cout << Max;
				ScreenClear();
				ScreenPrint();
				Sleep(10);
			}
			type_now = type_next;
			type_next = rand() % 7;
			TBuild(Tnow, type_now);
			Judge_gameover(Tnow);
			NextShow(type_next);
		}
		switch (Judge_key()) {
		case 8:TRotate(Tnow);break;
		case 4:TMove(Tnow, 4);break;
		case 6:TMove(Tnow, 6);break;
		case 2:TDown(Tnow);break;
		case 0:PosScreen(10, 4);cout << "Escape from Tetris!";End();
		}
		TClear(Tp);
		TShow(Tnow);
		TCopy(Tp, Tnow);
		if (!(count %= Speed_mulriple)) { count = 1;++Tnow.x; }
		Sleep(SleepTime);
	}
}
void End() {
	ofstream o("tetris_data.dat", ios_base::binary);
	if (!o) o.close();
	else {
		o << Max;
		o.close();
	}
	PosScreen(1, 1);
	int num = 0;
	int times = 0;
	int sl = 3;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 119);
	while (num < 336) {
		int i, j;
		PosScreen(times + 1, times + 1);
		for (i = times + 1;i < Sb - times;++i) {
			cout << "■";
			++num;
			Sleep(sl);
		}
		for (j = times + 1;j < Sh - times;++j) {
			PosScreen(j, i);
			cout << "■";
			++num;
			Sleep(sl);
		}
		for (;i > times + 1;--i) {
			PosScreen(j, i);
			cout << "■";
			++num;
			Sleep(sl);
		}
		for (;j > times + 1;--j) {
			PosScreen(j, i);
			cout << "■";
			++num;
			Sleep(sl);
		}
		++times;
	}
	exit(0);
}
void Interface() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	Pos(2, 3);cout << "■■■■■■■■■■■■■■■■■■\n";//18x
	for (int i = 3;i < 24;++i)cout << "  ■\n";
	cout << "  ■■■■■■■■■■■■■■■■■■\n";
	for (int i = 3;i < 24;++i) {
		Pos(i, 37); cout << "■";
		if (i <= 8 || i >= 12) {
			Pos(i, 47); cout << "■";
			Pos(i, 69); cout << "■";
		}
	}
	Pos(2, 47); cout << "■■■■■■■■■■■■";//12x
	Pos(8, 47); cout << "■■■■■■■■■■■■";
	Pos(12, 47); cout << "■■■■■■■■■■■■";
	Pos(24, 47); cout << "■■■■■■■■■■■■";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	Pos(9, 53);cout << "下一个  方块";
	Pos(14, 51);cout << "得分:" << Score;
	Pos(15, 51);cout << "速度:" << SleepTime*Speed_mulriple << 's';
	Pos(16, 51);cout << "最高分:" << Max;
	Pos(17, 51);cout << "按R键图像复位";
	Pos(18, 54);cout << "用于输入法导";
	Pos(19, 54);cout << "致的显示错误";
	Pos(20, 51);cout << "上下左右操控";
	Pos(21, 51);cout << "按空格键暂停";
	Pos(22, 51);cout << "按ESC结束游戏";
}
void ScreenPrint() {
	for (int i = 1;i <= Sh;++i) {
		for (int j = 1;j <= Sb;++j) {
			if (Screen[i][j]) {
				PosScreen(i, j);
				cout << "■";
			}
		}
	}
}
void ScreenClear() {
	for (int i = 1;i <= Sh;++i)
		for (int j = 1;j <= Sb;++j) {
			PosScreen(i, j);cout << "  ";
		}

}
void NextShow(unsigned t) {
	Pos(3, 49);cout << "                    ";
	Pos(4, 49);cout << "                    ";
	Pos(5, 49);cout << "                    ";
	Pos(6, 49);cout << "                    ";
	Pos(7, 49);cout << "                    ";
	if (t == 0) { Pos(5, 55);cout << "■■■■"; }
	else if (t == 1) { Pos(4, 57);cout << "■■";Pos(5, 57);cout << "■■"; }
	else if (t == 2) { Pos(4, 59);cout << "■";Pos(5, 55);cout << "■■■"; }
	else if (t == 3) { Pos(4, 57);cout << "■";Pos(5, 55);cout << "■■■"; }
	else if (t == 4) { Pos(4, 55);cout << "■";Pos(5, 55);cout << "■■■"; }
	else if (t == 5) { Pos(4, 57);cout << "■■";Pos(5, 55);cout << "■■"; }
	else { Pos(4, 55);cout << "■■";Pos(5, 57);cout << "■■"; }
}
void Pause() {
	Pos(11, 55);cout << "暂    停";
	do {
		Sleep(200);
	} while (!GetAsyncKeyState(VK_SPACE));
	Pos(11, 55);cout << "        ";
}

void TBuild(Tetris& t, unsigned sh) {
	t.x = 0;
	t.type = sh;
	switch (sh)
	{
	case 0: {
		t.shape[0][0] = 0;t.shape[0][1] = 0;t.shape[0][2] = 0;t.shape[0][3] = 0;//
		t.shape[1][0] = 1;t.shape[1][1] = 1;t.shape[1][2] = 1;t.shape[1][3] = 1;//■■■■
		t.shape[2][0] = 0;t.shape[2][1] = 0;t.shape[2][2] = 0;t.shape[2][3] = 0;//
		t.shape[3][0] = 0;t.shape[3][1] = 0;t.shape[3][2] = 0;t.shape[3][3] = 0;//青色
		t.y = 7;
		break;
	}
	case 1: {
		t.shape[0][0] = 1;t.shape[0][1] = 1;t.shape[0][2] = 0;t.shape[0][3] = 0;//■■
		t.shape[1][0] = 1;t.shape[1][1] = 1;t.shape[1][2] = 0;t.shape[1][3] = 0;//■■
		t.shape[2][0] = 0;t.shape[2][1] = 0;t.shape[2][2] = 0;t.shape[2][3] = 0;//
		t.shape[3][0] = 0;t.shape[3][1] = 0;t.shape[3][2] = 0;t.shape[3][3] = 0;//蓝
		t.y = 8;
		break;
	}
	case 2: {
		t.shape[0][0] = 0;t.shape[0][1] = 0;t.shape[0][2] = 1;t.shape[0][3] = 0;//  ■
		t.shape[1][0] = 1;t.shape[1][1] = 1;t.shape[1][2] = 1;t.shape[1][3] = 0;//■■■
		t.shape[2][0] = 0;t.shape[2][1] = 0;t.shape[2][2] = 0;t.shape[2][3] = 0;//
		t.shape[3][0] = 0;t.shape[3][1] = 0;t.shape[3][2] = 0;t.shape[3][3] = 0;//紫
		t.y = 7;
		break;
	}
	case 3: {
		t.shape[0][0] = 0;t.shape[0][1] = 1;t.shape[0][2] = 0;t.shape[0][3] = 0;// ■
		t.shape[1][0] = 1;t.shape[1][1] = 1;t.shape[1][2] = 1;t.shape[1][3] = 0;//■■■
		t.shape[2][0] = 0;t.shape[2][1] = 0;t.shape[2][2] = 0;t.shape[2][3] = 0;//
		t.shape[3][0] = 0;t.shape[3][1] = 0;t.shape[3][2] = 0;t.shape[3][3] = 0;//绿
		t.y = 7;
		break;
	}
	case 4: {
		t.shape[0][0] = 1;t.shape[0][1] = 0;t.shape[0][2] = 0;t.shape[0][3] = 0;//■
		t.shape[1][0] = 1;t.shape[1][1] = 1;t.shape[1][2] = 1;t.shape[1][3] = 0;//■■■
		t.shape[2][0] = 0;t.shape[2][1] = 0;t.shape[2][2] = 0;t.shape[2][3] = 0;//
		t.shape[3][0] = 0;t.shape[3][1] = 0;t.shape[3][2] = 0;t.shape[3][3] = 0;//红
		t.y = 7;
		break;
	}
	case 5: {
		t.shape[0][0] = 0;t.shape[0][1] = 1;t.shape[0][2] = 1;t.shape[0][3] = 0;// ■■
		t.shape[1][0] = 1;t.shape[1][1] = 1;t.shape[1][2] = 0;t.shape[1][3] = 0;//■■
		t.shape[2][0] = 0;t.shape[2][1] = 0;t.shape[2][2] = 0;t.shape[2][3] = 0;//
		t.shape[3][0] = 0;t.shape[3][1] = 0;t.shape[3][2] = 0;t.shape[3][3] = 0;//橙色
		t.y = 7;
		break;
	}
	case 6: {
		t.shape[0][0] = 1;t.shape[0][1] = 1;t.shape[0][2] = 0;t.shape[0][3] = 0;//■■
		t.shape[1][0] = 0;t.shape[1][1] = 1;t.shape[1][2] = 1;t.shape[1][3] = 0;// ■■
		t.shape[2][0] = 0;t.shape[2][1] = 0;t.shape[2][2] = 0;t.shape[2][3] = 0;//
		t.shape[3][0] = 0;t.shape[3][1] = 0;t.shape[3][2] = 0;t.shape[3][3] = 0;//黄色
		t.y = 7;
		break;
	}
	default: {Pos(1, 1);cerr << "Error 2:方块种类出错!";exit(1);}
	}
}
void TRotate(Tetris& t) {
	if (t.type == 0) {
		Swap(t.shape[0][1], t.shape[1][0]);
		Swap(t.shape[2][1], t.shape[1][2]);
		Swap(t.shape[3][1], t.shape[1][3]);
		for (int i = 0;i < 4;++i)
			for (int j = 0;j < 4;++j)
				if (t.shape[i][j])
					if (i + t.x > Sh || j + t.y>Sb || Screen[t.x + i][t.y + j]) {
						Swap(t.shape[0][1], t.shape[1][0]);
						Swap(t.shape[2][1], t.shape[1][2]);
						Swap(t.shape[3][1], t.shape[1][3]);
					}
	}
	else if (t.type == 1);
	else {
		bool tmp[3][3];
		for (int i = 0;i < 3;++i)
			for (int j = 0;j < 3;++j)
				tmp[i][j] = t.shape[i][j];
		t.shape[0][0] = tmp[2][0];
		t.shape[0][1] = tmp[1][0];
		t.shape[0][2] = tmp[0][0];
		t.shape[1][0] = tmp[2][1];
		t.shape[1][2] = tmp[0][1];
		t.shape[2][0] = tmp[2][2];
		t.shape[2][1] = tmp[1][2];
		t.shape[2][2] = tmp[0][2];
		for (int i = 0;i < 3;++i)
			for (int j = 0;j < 3;++j)
				if (t.shape[i][j])
					if (i + t.x > Sh || j + t.y>Sb || Screen[t.x + i][t.y + j]) {
						for (int i = 0;i < 3;++i)
							for (int j = 0;j < 3;++j)
								t.shape[i][j] = tmp[i][j];
					}
		TShow(t);
	}
}
void TMove(Tetris& t, int d) {
	d -= 5;
	for (int i = 0;i < 4;++i)
		for (int j = 0;j < 4;++j)
			if (t.shape[i][j] && Screen[t.x + i][t.y + j + d])
				return;
	t.y += d;
}
void TDown(Tetris& t) {
	TClear(t);
	int l = 0, i = 0, j = 0;
	while (1) {
		for (i = 0;i < 4;++i)
			for (j = 0;j < 4;++j)
				if (t.shape[i][j] && Screen[t.x + i + 1][t.y + j]) {
					TShow(t);
					return;
				}
		++t.x;
	}
	Sleep(20);
}
void TCopy(Tetris& tt, const Tetris& t) {
	tt.type = t.type;
	tt.x = t.x, tt.y = t.y;
	for (int i = 0;i < 4;++i)
		for (int j = 0;j < 4;++j)
			tt.shape[i][j] = t.shape[i][j];
}
void TShow(const Tetris& t) {
	for (int i = 0;i < 4;++i)
		for (int j = 0;j < 4;++j)
			if (t.shape[i][j] && (t.x + i != 0)) {
				PosScreen(t.x + i, t.y + j);
				cout << "■";
			}
}
void TClear(const Tetris& t) {
	if (t.type == 233)return;
	for (int i = 0;i < 4;++i)
		for (int j = 0;j < 4;++j)
			if (t.shape[i][j] && (t.x + i != 0)) {
				PosScreen(t.x + i, t.y + j);
				cout << "  ";
			}
}

int Judge_down(const Tetris& t) {
	for (int i = 0;i < 4;++i)
		for (int j = 0;j < 4;++j)
			if (t.shape[i][j] && Screen[t.x + i][t.y + j]) {
				for (int i = 0;i < 4;++i)
					for (int j = 0;j < 4;++j)
						if (t.shape[i][j])
							Screen[t.x + i - 1][t.y + j] = 1;
				return 1;
			}
	return 0;
}
int Judge_key() {
	unsigned d = 5;
	if (GetAsyncKeyState(VK_UP)) d = 8;
	else if (GetAsyncKeyState(VK_DOWN)) d = 2;
	else if (GetAsyncKeyState(VK_LEFT)) d = 4;
	else if (GetAsyncKeyState(VK_RIGHT)) d = 6;
	else if (GetAsyncKeyState(VK_SPACE)) Pause();
	else if (GetAsyncKeyState(VK_ESCAPE)) d = 0;
	else if (GetAsyncKeyState(0x52)) {
		system("cls");
		Interface();
		ScreenPrint();
		Pos(4, 49);cout << "R键图像复位导致下一";
		Pos(5, 49);cout << "个方块的显示失效BUG";
		Pos(6, 49);cout << "不高兴修复了";
	}
	return d;
}
int Judge_eliminate(const Tetris& t) {
	int judge = 0;
	for (int i = 0;i < 4;++i) {
		for (int j = 0;j < 4;++j)
			if (t.shape[i][j]) {
				int k;
				unsigned xx = t.x - 1;
				for (k = 1;k <= Sb&&Screen[xx + i][k];++k);
				if (k > Sb) {
					++judge;
					int blank = 0;
					if (xx == 1) {
						for (int l = 1;l <= Sb;++l)
							Screen[1][l] = 0;
						break;
					}
					while (blank != Sb) {
						blank = 0;
						for (int l = 1;l <= Sb;++l) {
							Screen[xx + i][l] = Screen[xx + i - 1][l];
							if (!Screen[xx + i - 1][l])++blank;
						}
						--xx;
					}
				}
				break;
			}
	}
	return judge;
}
int Judge_gameover(const Tetris& t) {
	for (int i = 0;i < 4;++i)
		if (t.shape[1][i] && Screen[1][t.y + i]) {
			PosScreen(10, 6);cout << "Game Over!";
			End();
		}
	return 0;
}

void Pos(unsigned x, unsigned y) {
	COORD p;
	try { if (x < 1 || y < 1) throw 1; }//测试代码，记得删除
	catch (int) { Pos(1, 1);cerr << "Error 1:坐标出错!";exit(1); }//测试代码，记得删除
	p.X = y - 1;p.Y = x - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
void PosScreen(unsigned x, unsigned y)
{
	Pos(x + 2, 2 * y + 3);
}
void Swap(bool& a, bool& b) {
	bool t = a;
	a = b;b = t;
}


int main()
{
	Start();
	Game();
	Pos(11, 50);
	return 0;
}