////////////////////////////////////////////////////////
// 程序名称：井字棋(release)
// 编译环境：VS2017		EasyX_2021-7-30
// 作　　者：贺运佳 <h_yunjia@163.com>
// 最后修改：2021-8-30
//
#include <graphics.h>
#include <cstdlib>
#include <time.h>
#include <iostream>



// 用于保存棋盘信息的类
class Manual
{
public:
	Manual();					// 构造函数
	void begin();				// 游戏开始
	int isOver();				// 判断游戏是否结束
	int getInNum(int);			// 获取棋谱中数字对应的气大小
	void updateQ();				// 更新气
	void updateQ1();			// 更新外气
	void updateQ2();			// 更新内气 
	int getAdd(int, int);		// getnext 的部分过程
	int getNext();				// 获取下一步落子位置
	bool moveChess(int, int);	// 下棋

	bool isOffensive();			// 选先后手
	void grid();				// 绘制格子
	void print(int, int);		// 绘制棋子
	int user();					// 获取用户动作
	void over();				// 游戏结束

private:
	int T[3][3];				// 棋谱各个位置的信息
	int q1[2][4];				// 记录外气 依次为: c1, c2, c3, 主对角线, r1, r2, r3, 次对角线
	int q2[3][3];				// 记录内气
	bool b[3][3];				// 记录是否落子
	int n[2];					// n[0] 计算机用子固定为 O, n[1] 用户用子固定为 X; 1: O, 2: X;
	int num;					// 当前棋子数
	bool isPlay;				// 判断是否停止游戏
};


// main 函数
int main(void)
{
	HWND hwnd = initgraph(640, 480);
	SetWindowText(hwnd, _T("井字棋"));
	Manual m;
	m.begin();
	closegraph();
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
	n[0] = 1;			// 电脑的棋子为 1: O
	n[1] = 2;			// 用户的棋子为 2: X
	isPlay = true;		// 游戏默认进行中
}

// 游戏开始函数
void Manual::begin()
{
	while (isPlay)
	{
		new (this)Manual();
		bool isO = isOffensive();
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
		bool rePlay = false;
		while (num != 9)
		{
			temp = user();
			if (temp == 0)
			{
				rePlay = true;
				break;
			}
			if (!moveChess(temp, n[1]))
				continue;
			if (isOver() || num == 9)
				break;
			moveChess(getNext(), n[0]);
			if (isOver())
				break;
		}
		if (rePlay)
			continue;
		over();
	}
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

	if (n1 == 1 && n2 == 3)				// 次对角线
	{
		if (!b[0][2])					// 遍历未填充, 返回 1-9
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
		HWND hwnd = GetHWnd();
		MessageBox(hwnd, _T("程序异常错误, 程序退出"), _T("error"), MB_OK | MB_ICONHAND);
		exit(0);
	}

	// 求得坐标
	int n1 = (num - 1) / 3;
	int n2 = (num - 1) % 3;

	// 错误输入
	if (b[n1][n2])
		return false;

	b[n1][n2] = true;		// 修改位置信息
	T[n1][n2] = n;			// 修改位置棋子
	updateQ();				// 更新气
	this->num++;			// 棋子数量 + 1
	print(num, n);			// 绘图
	return true;
}

// 判断用户先手后手
bool Manual::isOffensive()
{
	// 背景
	setfillstyle(BS_SOLID);
	setfillcolor(RGB(255, 205, 150));
	solidrectangle(0, 0, 640, 480);

	// 字
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 0, _T("宋体"));
	outtextxy(235, 155, _T("请选择先后手"));
	outtextxy(245, 265, _T("先手"));
	outtextxy(335, 265, _T("后手"));

	// 鼠标操作
	ExMessage msg;
	while (true)
	{
		msg = getmessage(EM_MOUSE | EM_KEY);
		if (WM_LBUTTONDOWN == msg.message)
		{
			if (msg.x > 245 && msg.x < 305 &&
				msg.y > 265 && msg.y < 295)
				return true;	// 先手
			if (msg.x > 335 && msg.x < 395 &&
				msg.y > 265 && msg.y < 295)
				return false;	// 后手
		}
		Sleep(10);
	}
}

// 绘制网格线
void Manual::grid()
{
	// 背景
	setfillstyle(BS_SOLID);
	setfillcolor(RGB(255, 205, 150));
	solidrectangle(0, 0, 640, 480);
	outtextxy(500, 35, _T("重新开始"));

	// 画分隔线
	setlinecolor(RGB(0, 0, 0));
	setlinestyle(PS_SOLID, 5);
	line(275, 115, 275, 365);
	line(360, 115, 360, 365);
	line(195, 195, 445, 195);
	line(195, 280, 445, 280);
}

// 绘制棋子
void Manual::print(int num, int n)
{
	int n1 = (num - 1) / 3;
	int n2 = (num - 1) % 3;
	int other1 = n1 * 5;
	int other2 = n2 * 5;	// 考虑线条粗细, 计算偏移量

	setlinestyle(PS_SOLID, 3);
	if (n == 1)				// 画圆
	{
		setlinecolor(RGB(255, 255, 255));
		circle(234 + n2 * 80 + other2, 154 + n1 * 80 + other1, 30);
	}
	else if (n == 2)		// 画叉
	{
		setlinecolor(RGB(0, 0, 0));
		line(210 + n2 * 80 + other2, 130 + n1 * 80 + other1,
			260 + n2 * 80 + other2, 180 + n1 * 80 + other1);
		line(260 + n2 * 80 + other2, 130 + n1 * 80 + other1,
			210 + n2 * 80 + other2, 180 + n1 * 80 + other1);
	}
}

// 获取用户操作
int Manual::user()
{
	ExMessage msg;
	while (true)
	{
		msg = getmessage(EM_MOUSE | EM_KEY);
		if (WM_LBUTTONDOWN == msg.message)
		{
			for (int n1 = 0; n1 < 3; ++n1)
				for (int n2 = 0; n2 < 3; ++n2)
					if (msg.x > n1 * 85 + 195 &&
						msg.x < (n1 + 1) * 80 + n1 * 5 + 195 &&
						msg.y > n2 * 85 + 115 &&
						msg.y < (n2 + 1) * 80 + n2 * 5 + 115)
						return n2 * 3 + n1 + 1;		// 落子位置
			if (msg.x > 500 && msg.x < 620 &&
				msg.y > 35 && msg.y < 65)
				return 0;							// 重新开始
		}
		Sleep(10);
	}
}

// 游戏结束
void Manual::over()
{
	int flag = isOver();
	if (flag == 0)
		outtextxy(285, 15, _T("平局!"));
	else if (flag == 1)
		outtextxy(285, 15, _T("失败!"));
	else if (flag == 2)
		outtextxy(285, 15, _T("获胜!"));

	outtextxy(245, 65, _T("继续"));
	outtextxy(335, 65, _T("退出"));
	ExMessage msg;
	while (true)
	{
		msg = getmessage(EM_MOUSE | EM_KEY);
		if (WM_LBUTTONDOWN == msg.message)
		{
			if(msg.x > 245 && msg.x < 305 &&
				msg.y > 65 && msg.y < 95)
				return;		// 继续游戏
			if (msg.x > 335 && msg.x < 395 &&
				msg.y > 65 && msg.y < 95)
			{
				isPlay = false;
				return;		// 退出游戏
			}
			if (msg.x > 500 && msg.x < 620 &&
				msg.y > 35 && msg.y < 65)
				return;		// 重新开始
		}
		Sleep(10);
	}
}
