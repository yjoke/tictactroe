#include"Manual.h"
#include<iostream>
#include<string>
#include<windows.h>
using namespace std;

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

void Manual::begin()
{
	cout << "请选择你要的棋子形状 'O':1 'X':2 " << endl;
	cin >> n[1];
	if (n[1] == 1)
		n[0] = 2;
	else if (n[1] == 2)
		n[0] = 1;
	else
	{
		cout << "输入错误, 默认用户棋子为 'O'" << endl;
		n[0] = 2;
		n[1] = 1;
	}
	cout << "请选择先手(1)还是后手(2)" << endl;
	int temp;
	cin >> temp;
	cout << "\n初始棋谱: " << endl;
	disPlay();
	if (temp == 2)
	{
		cout << "\n电脑落子:" << endl;
		moveChess(getNext(), n[0]);
	}
	while (num != 9)
	{
		cout << "\n请输入你要落子的位置: ";
		cin >> temp;
		if (!moveChess(temp, n[1]))
		{
			cout << "输入错误, 该位置已有棋子" << endl;
			continue;
		}
		if (isOver() || num == 9)
			break;
		cout << "\n电脑落子:" << endl;
		moveChess(getNext(), n[0]);
		if (isOver())
			break;
	}
	cout << "\n游戏结束, ";
	int flag = isOver();
	if (flag == 0)
		cout << "平局" << endl;
	else if (flag == 1)
		cout << "电脑胜利" << endl;
	else if (flag == 2)
		cout << "恭喜你取得了胜利" << endl;
}

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

int Manual::getInNum(int n)
{
	if (n == this->n[0])
		return 1;
	else if (n == this->n[1])
		return -1;
	else
		return 0;
}

void Manual::updateQ()
{
	updateQ1();
	updateQ2();
}

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

int Manual::getAdd(int n1, int n2)
{
	if (n1 == 0 && n2 < 3)  // 属于某一列
		for (int i = 0; i < 3; ++i)  // 遍历该列
			if (!b[i][n2])  // 判断该列未填充的位置
				return i * 3 + n2 + 1;  // 返回位置 1-9

	if (n1 == 1 && n2 < 3)  // 属于某一行
		for (int i = 0; i < 3; ++i)  // 遍历该行
			if (!b[n2][i])  // 判断该行未填充位置
				return n2 * 3 + i + 1;  // 返回位置 1-9

	if (n1 == 0 && n2 == 3)  // 主对角线
		for (int i = 0; i < 3; ++i)  // 遍历对角线
			if (!b[i][i])  // 判断对角线未填充位置
				return i * 3 + i + 1;  // 返回位置 1-9

	if (n1 == 1 && n2 == 3)  // 次对角线
	{
		if (!b[0][2])  // 遍历未填充, 返回 1-9
			return 3;
		else if (!b[1][1])
			return 5;
		else if (!b[2][0])
			return 7;
	}
	return 0;
}

int Manual::getNext()
{
	// 遍历外气, 寻找是否存在 2 气, -2 气
	bool flag_q1_2 = false, flag_q1_2_ = false;  // 默认不存在
	int q1_2[2], q1_2_[2];  // 2 的位置和 -2 的位置
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
	if (flag_q1_2)  // 存在返回应当落子的位置
		return getAdd(q1_2[0], q1_2[1]);
	if (flag_q1_2_)  // 存在返回应当落子的位置
		return getAdd(q1_2_[0], q1_2_[1]);

	if (!b[0][1] && q2[0][1] == 0 && !b[1][0] && q2[1][0] == 1)
		return 4;  // 特殊考虑边棋子大于中心棋子优先级的的情况
	if (!b[0][1] && q2[0][1] == 1 && !b[1][0] && q2[1][0] == 0)
		return 2;
	if (!b[1][1])
		return 5;  // 中心点未被落子, 直接占用

	// 遍历内气, 寻找内气中值
	int num = 0;  // 需要考虑的气的数量
	int N[9] = { 0 };  // 寻找中值的辅助数组
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!b[i][j])
			{
				N[num++] = q2[i][j];  // 保存气的大小, 并计数 +1
				if (q2[i][j] == 2)  // 绝杀点
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
		mid = N[num / 2 - 1];// - 1;  // cuouw
	else
		mid = N[num / 2];

	// 再次遍历, 寻找气 =mid 的位置
	num = 0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!b[i][j] && q2[i][j] == mid)
			{
				N[num++] = i * 3 + j + 1;
			}  // 保存气的位置, 并计数 +1

// 遍历可使用的位置集
	for (int i = num - 1; i >= 0; --i)  // 第一次遍历查看是否存在角
		if (N[i] == 1 || N[i] == 3 || N[i] == 7 || N[i] == 9)
			return N[i];
	for (int i = 0; i < num; ++i)  // 第二次遍历返回即不存在角, 返回边
		if (N[i] == 2 || N[i] == 4 || N[i] == 6 || N[i] == 8)
			return N[i];
	return 0;
}

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
	if (b[n1][n2])
		return false;
	b[n1][n2] = true;  // 修改位置信息
	T[n1][n2] = n;  // 修改位置棋子
	updateQ();  // 更新气
	this->num++;  // 棋子数量 + 1
	disPlay();  // 打印当前棋谱
	return true;
}

