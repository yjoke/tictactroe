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
	cout << "��ѡ����Ҫ��������״ 'O':1 'X':2 " << endl;
	cin >> n[1];
	if (n[1] == 1)
		n[0] = 2;
	else if (n[1] == 2)
		n[0] = 1;
	else
	{
		cout << "�������, Ĭ���û�����Ϊ 'O'" << endl;
		n[0] = 2;
		n[1] = 1;
	}
	cout << "��ѡ������(1)���Ǻ���(2)" << endl;
	int temp;
	cin >> temp;
	cout << "\n��ʼ����: " << endl;
	disPlay();
	if (temp == 2)
	{
		cout << "\n��������:" << endl;
		moveChess(getNext(), n[0]);
	}
	while (num != 9)
	{
		cout << "\n��������Ҫ���ӵ�λ��: ";
		cin >> temp;
		if (!moveChess(temp, n[1]))
		{
			cout << "�������, ��λ����������" << endl;
			continue;
		}
		if (isOver() || num == 9)
			break;
		cout << "\n��������:" << endl;
		moveChess(getNext(), n[0]);
		if (isOver())
			break;
	}
	cout << "\n��Ϸ����, ";
	int flag = isOver();
	if (flag == 0)
		cout << "ƽ��" << endl;
	else if (flag == 1)
		cout << "����ʤ��" << endl;
	else if (flag == 2)
		cout << "��ϲ��ȡ����ʤ��" << endl;
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
	if (n1 == 0 && n2 < 3)  // ����ĳһ��
		for (int i = 0; i < 3; ++i)  // ��������
			if (!b[i][n2])  // �жϸ���δ����λ��
				return i * 3 + n2 + 1;  // ����λ�� 1-9

	if (n1 == 1 && n2 < 3)  // ����ĳһ��
		for (int i = 0; i < 3; ++i)  // ��������
			if (!b[n2][i])  // �жϸ���δ���λ��
				return n2 * 3 + i + 1;  // ����λ�� 1-9

	if (n1 == 0 && n2 == 3)  // ���Խ���
		for (int i = 0; i < 3; ++i)  // �����Խ���
			if (!b[i][i])  // �ж϶Խ���δ���λ��
				return i * 3 + i + 1;  // ����λ�� 1-9

	if (n1 == 1 && n2 == 3)  // �ζԽ���
	{
		if (!b[0][2])  // ����δ���, ���� 1-9
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
	// ��������, Ѱ���Ƿ���� 2 ��, -2 ��
	bool flag_q1_2 = false, flag_q1_2_ = false;  // Ĭ�ϲ�����
	int q1_2[2], q1_2_[2];  // 2 ��λ�ú� -2 ��λ��
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
	if (flag_q1_2)  // ���ڷ���Ӧ�����ӵ�λ��
		return getAdd(q1_2[0], q1_2[1]);
	if (flag_q1_2_)  // ���ڷ���Ӧ�����ӵ�λ��
		return getAdd(q1_2_[0], q1_2_[1]);

	if (!b[0][1] && q2[0][1] == 0 && !b[1][0] && q2[1][0] == 1)
		return 4;  // ���⿼�Ǳ����Ӵ��������������ȼ��ĵ����
	if (!b[0][1] && q2[0][1] == 1 && !b[1][0] && q2[1][0] == 0)
		return 2;
	if (!b[1][1])
		return 5;  // ���ĵ�δ������, ֱ��ռ��

	// ��������, Ѱ��������ֵ
	int num = 0;  // ��Ҫ���ǵ���������
	int N[9] = { 0 };  // Ѱ����ֵ�ĸ�������
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!b[i][j])
			{
				N[num++] = q2[i][j];  // �������Ĵ�С, ������ +1
				if (q2[i][j] == 2)  // ��ɱ��
					return i * 3 + j + 1;
			}

	// ����
	for (int i = 0; i < num - 1; ++i)
		for (int j = 0; j < num - i - 1; ++j)
			if (N[j] > N[j + 1])
			{
				int temp = N[j]; N[j] = N[j + 1]; N[j + 1] = temp;
			}

	// ��ֵ
	int mid;
	if (num % 2 == 0)
		mid = N[num / 2 - 1];// - 1;  // cuouw
	else
		mid = N[num / 2];

	// �ٴα���, Ѱ���� =mid ��λ��
	num = 0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (!b[i][j] && q2[i][j] == mid)
			{
				N[num++] = i * 3 + j + 1;
			}  // ��������λ��, ������ +1

// ������ʹ�õ�λ�ü�
	for (int i = num - 1; i >= 0; --i)  // ��һ�α����鿴�Ƿ���ڽ�
		if (N[i] == 1 || N[i] == 3 || N[i] == 7 || N[i] == 9)
			return N[i];
	for (int i = 0; i < num; ++i)  // �ڶ��α������ؼ������ڽ�, ���ر�
		if (N[i] == 2 || N[i] == 4 || N[i] == 6 || N[i] == 8)
			return N[i];
	return 0;
}

bool Manual::moveChess(int num, int n)
{
	if (num == 0)
	{
		cout << "�����쳣����, �����˳�, ";
		system("pause");
		exit(0);
	}
	int n1 = (num - 1) / 3;
	int n2 = (num - 1) % 3;
	if (b[n1][n2])
		return false;
	b[n1][n2] = true;  // �޸�λ����Ϣ
	T[n1][n2] = n;  // �޸�λ������
	updateQ();  // ������
	this->num++;  // �������� + 1
	disPlay();  // ��ӡ��ǰ����
	return true;
}

