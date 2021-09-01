#pragma once

class Manual
{
public:
	Manual();  // 构造函数
	void begin();  // 游戏开始
	int isOver();  //判断是否结束
	void disPlay();  // 打印当前的棋谱
	char getOutChar(int);  // 获取棋谱中数字对于的字母
	int getInNum(int); // 获取棋谱中数字对应的气大小
	void updateQ();  // 更新气
	void updateQ1();  // 更新外气
	void updateQ2();  // 更新内气 
	int getAdd(int, int);
	int getNext();  // 获取下一步落子位置
	bool moveChess(int, int); // 下棋
	int T[3][3];  // 棋谱各个位置的信息
	int q1[2][4];  // 记录外气  // 依次为: c1, c2, c3, 主对角线, r1, r2, r3, 次对角线
	int q2[3][3];  // 记录内气
	bool b[3][3];  // 记录是否落子
	int n[2];  // n[0] 计算机用的子， n[1] 用户用的子   1：O， 2：X；
	int num;  // 当前棋子数
};