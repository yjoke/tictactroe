////////////////////////////////////////////////////////
// 程序名称：井字棋
// 编译环境：VS2017		EasyX_2020-3-15(beta)
// 作　　者：贺运佳 <h_yunjia@163.com>
// 最后修改：2021-8-25
//
#include <graphics.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
using namespace std;



// 用于保存棋盘信息的类
class Manual
{
public:
	Manual();					// 构造函数
	void begin();				// 游戏开始
	int isOver();				// 判断游戏是否结束
	void disPlay();				// 打印当前的棋谱
	char getOutChar(int);		// 获取棋谱中数字对于的字母
	int getInNum(int);			// 获取棋谱中数字对应的气大小
	void updateQ();				// 更新气
	void updateQ1();			// 更新外气
	void updateQ2();			// 更新内气 
	int getAdd(int, int);		// getnext 的部分过程
	int getNext();				// 获取下一步落子位置
	bool moveChess(int, int);	// 下棋

	bool isOffensive();			// 选先后手
	bool isCircle();			// 选择棋子
	void grid();				// 绘制格子
	void print(int, int);		// 绘制棋子
	int user();					// 获取用户动作

private:
	int T[3][3];				// 棋谱各个位置的信息
	int q1[2][4];				// 记录外气 依次为: c1, c2, c3, 主对角线, r1, r2, r3, 次对角线
	int q2[3][3];				// 记录内气
	bool b[3][3];				// 记录是否落子
	int n[2];					// n[0] 计算机用的子， n[1] 用户用的子   1：O， 2：X；
	int num;					// 当前棋子数

};


// main 函数
int main(void)
{
	Manual m;
	m.begin();
	system("pause");
	return 0;
}


// 类函数定义

// 构造函数
Manual::Manual()
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			T[i][j] = 0;
			q2[i][j] = 0;
			b[i][j] = false;
		}

	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 4; ++j)
			q1[i][j] = 0;
	num = 0;
}

// 游戏开始函数
void Manual::begin()
{
	HWND hwnd = initgraph(250, 250);
	SetWindowText(hwnd, _T("井字棋"));
	system("title 对局过程");
	setlinecolor(RGB(0, 0, 255));

	bool isO = isOffensive();
	if (isCircle())
	{
		n[0] = 2;
		n[1] = 1;
	}
	else
	{
		n[0] = 1;
		n[1] = 2;
	}
	cout << "对局过程如下: " << endl;
	cout << "\n初始棋谱: " << endl;
	disPlay();
	grid();

	if (!isO)
	{
		// 控制第一步必为中心或者角
		srand((unsigned)time(NULL));
		if (rand() % 2)
			moveChess(1, n[0]);
		else
			moveChess(5, n[0]);
	}
	int temp;

	while (num != 9)
	{
		temp = user();
		if (!moveChess(temp, n[1]))
			continue;
		if (isOver() || num == 9)
			break;
		moveChess(getNext(), n[0]);
		if (isOver())
			break;
	}

	int flag = isOver();
	cout << "\n游戏结束, ";
	if (flag == 0)
	{
		hwnd = GetHWnd();
		MessageBox(hwnd, _T("平局!"), _T("游戏结束"), MB_OK);
		cout << "平局" << endl;
	}
	else if (flag == 1)
	{
		hwnd = GetHWnd();
		MessageBox(hwnd, _T("很遗憾，你输了"), _T("游戏结束"), MB_OK);
		cout << "电脑胜利" << endl;
	}
	else if (flag == 2)
	{
		hwnd = GetHWnd();
		MessageBox(hwnd, _T("恭喜你取得了游戏胜利"), _T("游戏结束"), MB_OK);
		cout << "恭喜你取得了胜利" << endl;
	}
	cout << "游戏对局如上" << endl;
	closegraph();
}

// 判断游戏是否结束
int Manual::isOver()
{
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 4; ++j)
			if (q1[i][j] == 3)
				return 1;
			else if (q1[i][j] == -3)
				return 2;
	return 0;
}

// 打印棋谱记录
void Manual::disPlay()
{
	for (int i = 0; i < 3; ++i)
	{
		if (i)
			cout << "---+---+---" << endl;

		for (int j = 0; j < 3; ++j)
		{
			if (j)
				cout << "|";
			cout << " " << getOutChar(T[i][j]) << " ";
		}

		cout << endl;
	}
}

// 获取棋谱中数字对于的字母
char Manual::getOutChar(int n)
{
	switch (n)
	{
	case 0: return '*';
	case 1: return 'O';
	case 2: return 'X';
	}
	return '*';
}

// 获取棋谱中数字对应的气大小
int Manual::getInNum(int n)
{
	if (n == this->n[0])
		return 1;
	else if (n == this->n[1])
		return -1;
	else
		return 0;
}

// 更新气
void Manual::updateQ()
{
	updateQ1();
	updateQ2();
}

// 更新外气
void Manual::updateQ1()
{
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			q1[i][j] = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			q1[0][i] += getInNum(T[j][i]);
			q1[1][i] += getInNum(T[i][j]);
		}
	}

	q1[0][3] = getInNum(T[0][0]) + getInNum(T[1][1]) + getInNum(T[2][2]);
	q1[1][3] = getInNum(T[2][0]) + getInNum(T[1][1]) + getInNum(T[0][2]);
}

// 更新内气
void Manual::updateQ2()
{
	q2[0][0] = q1[0][0] + q1[1][0] + q1[0][3];
	q2[0][1] = q1[0][1] + q1[1][0];
	q2[0][2] = q1[0][2] + q1[1][0] + q1[1][3];
	q2[1][0] = q1[0][0] + q1[1][1];
	q2[1][1] = q1[0][1] + q1[1][1] + q1[0][3] + q1[1][3];
	q2[1][2] = q1[0][2] + q1[1][1];
	q2[2][0] = q1[0][0] + q1[1][2] + q1[1][3];
	q2[2][1] = q1[0][1] + q1[1][2];
	q2[2][2] = q1[0][2] + q1[1][2] + q1[0][3];
}

// getnext() 函数的部分过程
int Manual::getAdd(int n1, int n2)
{
	if (n1 == 0 && n2 < 3)				// 属于某一列
		for (int i = 0; i < 3; ++i)		// 遍历该列
			if (!b[i][n2])				// 判断该列未填充的位置
				return i * 3 + n2 + 1;	// 返回位置 1-9

	if (n1 == 1 && n2 < 3)				// 属于某一行
		for (int i = 0; i < 3; ++i)		// 遍历该行
			if (!b[n2][i])				// 判断该行未填充位置
				return n2 * 3 + i + 1;	// 返回位置 1-9

	if (n1 == 0 && n2 == 3)				// 主对角线
		for (int i = 0; i < 3; ++i)		// 遍历对角线
			if (!b[i][i])				// 判断对角线未填充位置
				return i * 3 + i + 1;	// 返回位置 1-9

	if (n1 == 1 && n2 == 3)	// 次对角线
	{
		if (!b[0][2])		// 遍历未填充, 返回 1-9
			return 3;
		else if (!b[1][1])
			return 5;
		else if (!b[2][0])
			return 7;
	}
	return 0;
}

// 获取下一步落子位置
int Manual::getNext()
{
	// 遍历外气, 寻找是否存在 2 气, -2 气
	bool flag_q1_2 = false, flag_q1_2_ = false;	// 默认不存在
	int q1_2[2], q1_2_[2];						// 2 的位置和 -2 的位置
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 4; ++j)
			if (q1[i][j] == 2)
			{
				flag_q1_2 = true;
				q1_2[0] = i;
				q1_2[1] = j;
			}
			else if (q1[i][j] == -2)
			{
				flag_q1_2_ = true;
				q1_2_[0] = i;
				q1_2_[1] = j;
			}

	// 首先考虑是不是即将获胜或者即将失败
	if (flag_q1_2)
		return getAdd(q1_2[0], q1_2[1]);
	if (flag_q1_2_)
		return getAdd(q1_2_[0], q1_2_[1]);

	// 特殊考虑边棋子大于中心棋子优先级的的情况
	if (!b[0][1] && q2[0][1] == 0 && !b[1][0] && q2[1][0] == 1)
		return 4;
	if (!b[0][1] && q2[0][1] == 1 && !b[1][0] && q2[1][0] == 0)
		return 2;

	// 中心点未被落子, 直接占用
	if (!b[1][1])
		return 5;

	// 遍历内气, 寻找内气中值
	int num = 0;						// 需要考虑的气的数量
	int N[9] = { 0 };					// 寻找中值的辅助数组
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!b[i][j])
			{
				N[num++] = q2[i][j];	// 保存气的大小, 并计数 +1
				if (q2[i][j] == 2)		// 绝杀点
					return i * 3 + j + 1;
			}

	// 排序
	for (int i = 0; i < num - 1; ++i)
		for (int j = 0; j < num - i - 1; ++j)
			if (N[j] > N[j + 1])
			{
				int temp = N[j]; N[j] = N[j + 1]; N[j + 1] = temp;
			}

	// 中值
	int mid;
	if (num % 2 == 0)
		mid = N[num / 2 - 1];
	else
		mid = N[num / 2];

	// 再次遍历, 寻找气 = mid 的位置
	num = 0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!b[i][j] && q2[i][j] == mid)
			{
				N[num++] = i * 3 + j + 1;	// 保存气的位置, 并计数 +1
			}

	// 遍历可使用的位置集
	for (int i = num - 1; i >= 0; --i)		// 第一次遍历查看是否存在角
		if (N[i] == 1 || N[i] == 3 || N[i] == 7 || N[i] == 9)
			return N[i];
	for (int i = 0; i < num; ++i)			// 第二次遍历返回即不存在角, 返回边
		if (N[i] == 2 || N[i] == 4 || N[i] == 6 || N[i] == 8)
			return N[i];
	return 0;
}

// 下棋
bool Manual::moveChess(int num, int n)
{
	if (num == 0)
	{
		cout << "程序异常错误, 程序退出, ";
		system("pause");
		exit(0);
	}

	int n1 = (num - 1) / 3;
	int n2 = (num - 1) % 3;

	// 错误输入
	if (b[n1][n2])
		return false;

	b[n1][n2] = true;		// 修改位置信息
	T[n1][n2] = n;			// 修改位置棋子
	updateQ();				// 更新气
	this->num++;			// 棋子数量 + 1

	if (n == this->n[0])
		cout << "\n电脑回合: " << endl;
	else if (n == this->n[1])
		cout << "\n用户回合:" << endl;
	disPlay();				// 打印当前棋谱
	print(num, n);			// 绘图
	return true;
}

// 判断用户先手后手
bool Manual::isOffensive()
{
	// 背景
	setfillstyle(BS_SOLID);
	setfillcolor(RGB(211, 211, 211));
	solidrectangle(0, 0, 250, 250);

	// 字
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, _T("宋体"));
	outtextxy(40, 40, _T("请选择先后手"));
	outtextxy(50, 150, _T("先手"));
	outtextxy(140, 150, _T("后手"));

	// 鼠标操作
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (true)
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (WM_LBUTTONDOWN == msg.uMsg)
			{
				if (msg.x > 50 && msg.x < 110 &&
					msg.y > 150 && msg.y < 180)
					return true;
				if (msg.x > 140 && msg.x < 200 &&
					msg.y > 150 && msg.y < 180)
					return false;
			}
		}
		Sleep(10);
	}
}

// 判断用户使用的棋子
bool Manual::isCircle()
{
	// 背景
	setfillstyle(BS_SOLID);
	setfillcolor(RGB(211, 211, 211));
	solidrectangle(0, 0, 250, 250);

	// 写字
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, _T("宋体"));
	outtextxy(20, 40, _T("请选择棋子形状"));
	outtextxy(50, 150, _T(""));
	outtextxy(140, 150, _T(""));
	setlinecolor(RGB(0, 0, 0));
	setlinestyle(PS_SOLID, 2);
	circle(80, 160, 25);
	line(140, 135, 190, 185);
	line(140, 185, 190, 135);

	// 鼠标操作
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (true)
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (WM_LBUTTONDOWN == msg.uMsg)
			{
				if (msg.x > 65 && msg.x < 115 && msg.y > 135 && msg.y < 185)
					return true;
				if (msg.x > 140 && msg.x < 190 && msg.y > 140 && msg.y < 185)
					return false;
			}
		}
		Sleep(10);
	}
}

// 绘制网格线
void Manual::grid()
{
	// 背景
	setfillstyle(BS_SOLID);
	setfillcolor(RGB(185, 157, 41));
	solidrectangle(0, 0, 250, 250);

	// 画线
	setlinecolor(RGB(126, 82, 14));
	setlinestyle(PS_SOLID, 5);
	line(80, 0, 80, 250);
	line(165, 0, 165, 250);
	line(0, 80, 250, 80);
	line(0, 165, 250, 165);
}

// 绘制棋子
void Manual::print(int num, int n)
{
	int n1 = (num - 1) / 3;
	int n2 = (num - 1) % 3;
	int other1 = n1 * 5;
	int other2 = n2 * 5;	// 考虑线条粗细, 计算偏移量

	setlinestyle(PS_SOLID, 3);
	if (n == 1)
	{
		setlinecolor(RGB(238, 246, 221));
		circle(39 + n2 * 80 + other2, 39 + n1 * 80 + other1, 30);
	}
	else if (n == 2)
	{
		setlinecolor(RGB(21, 22, 15));
		line(15 + n2 * 80 + other2, 15 + n1 * 80 + other1, 65 + n2 * 80 + other2, 65 + n1 * 80 + other1);
		line(65 + n2 * 80 + other2, 15 + n1 * 80 + other1, 15 + n2 * 80 + other2, 65 + n1 * 80 + other1);
	}
}

// 获取用户操作
int Manual::user()
{
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (true)
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (WM_LBUTTONDOWN == msg.uMsg)
			{
				for (int n1 = 0; n1 < 3; ++n1)
					for (int n2 = 0; n2 < 3; ++n2)
						if (msg.x > n1 * 85 && msg.x < (n1 + 1) * 80 + n1 * 5 &&
							msg.y > n2 * 85 && msg.y < (n2 + 1) * 80 + n2 * 5)
							return n2 * 3 + n1 + 1;
			}
		}
		Sleep(10);
	}
}
