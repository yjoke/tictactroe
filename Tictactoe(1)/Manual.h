#pragma once

class Manual
{
public:
	Manual();  // ���캯��
	void begin();  // ��Ϸ��ʼ
	int isOver();  //�ж��Ƿ����
	void disPlay();  // ��ӡ��ǰ������
	char getOutChar(int);  // ��ȡ���������ֶ��ڵ���ĸ
	int getInNum(int); // ��ȡ���������ֶ�Ӧ������С
	void updateQ();  // ������
	void updateQ1();  // ��������
	void updateQ2();  // �������� 
	int getAdd(int, int);
	int getNext();  // ��ȡ��һ������λ��
	bool moveChess(int, int); // ����
	int T[3][3];  // ���׸���λ�õ���Ϣ
	int q1[2][4];  // ��¼����  // ����Ϊ: c1, c2, c3, ���Խ���, r1, r2, r3, �ζԽ���
	int q2[3][3];  // ��¼����
	bool b[3][3];  // ��¼�Ƿ�����
	int n[2];  // n[0] ������õ��ӣ� n[1] �û��õ���   1��O�� 2��X��
	int num;  // ��ǰ������
};