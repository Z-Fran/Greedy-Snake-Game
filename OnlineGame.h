#define _CRT_SECURE_NO_WARNINGS
#include "Client.h"
#include<graphics.h>
#include<conio.h>
#include<cmath>
#include<ctime>
#include<stdlib.h>
#include<sstream>
#include<string>
using namespace std;
using namespace NetworkHelper;
#define PI 3.1415926
#define MAP_W 2000
#define MAP_H 2000
#define WINDOW_W 900
#define WINDOW_H 600
//�������������ɫ
DWORD COLORSETSS[30] = { 0X808080,0X5C5CCD,0X8080F0,0X4763FF,0X507FFF,0X008CFF,0XADDEFF,0X00FFFF,0X2FFFAD,0X00FF00,
					0X7FFF00,0XD0E040,0XEEEEAF,0XFACE87,0XE16941,0XFF901E,0X00D7FF,0XD30094,0XDDA0DD,0XC1B6FF };

//�߽ڵ�����
struct SnakeNode {
	double length;
	int x[100];
	int y[100];
};
//�ߵ������Ϣ
struct SnakeInfo {
	DWORD color;//��ɫ
	int angle;
	SnakeNode path;
};
//��ͼ��������
struct MapObj {
	short num;
	short pos[100][2];
};

/****************************************
* ����
*****************************************/
class SnakeO {
private:
	double length_next;//����
	int length;
	DWORD color;//��ɫ
	int radius;//�뾶
	int angle;//��ͷ����
	int speed;//�ٶ�
	int frame;//֡��
	int frame_now;
	int isInit;
	int path[2][100];
	int path_next[2][100];
	int path_last[2][100];

public:
	//���캯��
	SnakeO() {
		InitSnake();
	}
	//�����ߵ�����
	void Set(SnakeInfo sn) {
		color = sn.color;
		angle = sn.angle;
		length = (int)sn.path.length;
		memcpy(path[0], sn.path.x, 100 * sizeof(int));
		memcpy(path[1], sn.path.y, 100 * sizeof(int));
	}
	/****************************************
	���ܣ���ʼ��һ����
	*****************************************/
	void InitSnake() {
		srand((unsigned)time(NULL));
		//���Գ�ʼ��
		speed = 5;
		length = 3;
		length_next = 3;
		radius = 10;
		angle = rand() % 360;
		color = COLORSETSS[rand() % 20];
		frame = radius * 2 / speed;
		frame_now = 0;
		isInit = 0;

		int headX, headY;
		headX = rand() % (MAP_W - 60) + 30;
		headY = rand() % (MAP_H - 60) + 30;
		for (int i = 0; i < length; i++) {
			path[0][i] = headX;
			path[1][i] = headY;
			path_next[0][i] = headX;
			path_next[1][i] = headY;
			path_last[0][i] = headX;
			path_last[1][i] = headY;
		}
	}
	//�����ٶ�
	void setSpeed() {
		int s;
		if (path[0][0] > 700 && path[0][0] < 1300 && path[1][0]>700 && path[1][0] < 1300) {
			s = 10;
		}
		else {
			s = 5;
		}
		if (s != speed) {
			speed = s;
			frame_now = 0;
			frame = radius * 2 / speed;
		}
	}
	//���ó���
	void setLength(double l) {
		length_next = l;
	}
	//���÷���
	void ChangeOrientation(int newAngle) {
		angle = newAngle;
		frame_now = 0;//Ư�Ƶ�����
	}
	//��ȡ����
	double getLength() {
		return length;
	}
	//��ȡ����
	int getAngle() {
		return angle;
	}
	//��ȡ��ɫ
	DWORD getColor() {
		return color;
	}
	//��ȡ�߽ڵ�����
	struct SnakeNode getPath() {
		SnakeNode a;
		a.length = length;
		for (int i = 0; i < 100; i++) {
			a.x[i] = path[0][i];
			a.y[i] = path[1][i];
		}
		//memcpy(a.x, path[0], 100 * sizeof(int));
		//memcpy(a.y, path[1], 100 * sizeof(int));
		return a;
	}

	/****************************************
	���ܣ�������
	*****************************************/
	void DrawSnake() {
		setfillcolor(color);
		for (int i = 0; i < length; i++) {
			solidcircle(path[0][i], path[1][i], radius);
		}
		setfillcolor(BLACK);
		solidcircle(path[0][0] + int(((double)radius - 1) * cos(((double)angle - 45) / 180.0 * PI)), path[1][0] + int(((double)radius - 1) * sin(((double)angle - 45) / 180.0 * PI)), 4);
		solidcircle(path[0][0] + int(((double)radius - 1) * cos(((double)angle + 45) / 180.0 * PI)), path[1][0] + int(((double)radius - 1) * sin(((double)angle + 45) / 180.0 * PI)), 4);
	}

	/****************************************
	���ܣ��ƶ���
	*****************************************/
	void Move() {
		if (isInit < 20) {
			isInit++;
		}
		//������һ��֡�����¿�ʼ
		if (frame_now == 0) {
			memcpy(path, path_next, sizeof(path));
			//for (int i = 0; i < length; i++) {
			//	path[0][i] = path_next[0][i];
			//	path[1][i] = path_next[1][i];
			//}
			//�ߵĳ�������ʱ��ӽڵ�
			if ((int)length_next > length){
				if (length > 0) {
					path[0][length++] = path_last[0][length - 1];
					path[1][length++] = path_last[1][length - 1];
				}
			}
			//�ߵĳ��ȼ���ʱɾ���ڵ�
			else if ((int)length_next < length) {
				length--;
			}
			for (int i = 1; i < length; i++) {
				path_last[0][i] = path[0][i];
				path_last[1][i] = path[1][i];
				path_next[0][i] = path[0][i-1];
				path_next[1][i] = path[1][i-1];
			}
			path_last[0][0] = path[0][0];
			path_last[1][0] = path[1][0];
			path_next[0][0] = path[0][0] + int(2 * radius * cos((double)angle / 180.0 * PI));
			path_next[1][0] = path[1][0] + int(2 * radius * sin((double)angle / 180.0 * PI));
		}
		else {
			for (int i = 0; i < length; i++) {
				double degree = atan2(path_next[1][i] - path_last[1][i], path_next[0][i] - path_last[0][i]);
				path[0][i] += int(speed * cos(degree));
				path[1][i] += int(speed * sin(degree));
			}
		}
		frame_now = (frame_now + 1) % frame;//���µ�ǰ֡��
	}

	//����Ƿ�ײ���߽�
	bool CheckBorder() {
		if (path[0][0] - 10 > 0 && path[0][0] + 10 < MAP_W && path[1][0] - 10 > 0 && path[1][0] + 10 < MAP_H) {
			return false;
		}
		else {
			return true;
		}
	}
	
	//����Ƿ�ײ��Ӳǽ
	int CheckHardWall(short HardWall[][2]) {
		for (int i = 0; i < 10; i++) {
			if (pow(path[0][0] - HardWall[i][0], 2) + pow(path[1][0] - HardWall[i][1], 2) <= pow(radius + 20, 2)) {
				length_next /= 2;
				if (length_next < 1) {
					length_next = 1;
				}
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�ײ����ǽ
	int CheckSoftWall(short SoftWall[][2], int num) {
		for (int i = 0; i < num; i++) {
			if (pow(path[0][0] - SoftWall[i][0], 2) + pow(path[1][0] - SoftWall[i][1], 2) <= pow(radius + 10, 2)) {
				length_next -= 1;
				if (length_next < 1) {
					length_next = 1;
				}
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�Ե���ͨʳ��
	int CheckFood(short food[][2], int r, int num, double increase) {
		for (int i = 0; i < num; i++) {
			if (pow(path[0][0] - food[i][0], 2) + pow(path[1][0] - food[i][1], 2) <= pow(radius + r, 2)) {
				length_next += increase;
				if (length_next > 100) {
					length_next = 100;
				}
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�ײ��������
	bool CheckCrashOther(SnakeNode a) {
		//�Ƿ����޵�״̬
		if (isInit < 20) {
			return false;
		}
		for (int i = 1; i < a.length; i++) {
			if (pow(path[0][0] - a.x[i], 2) + pow(path[1][0] - a.y[i], 2) <= pow(2 * radius, 2)) {
				return true;
			}
		}
		return false;
	}
};

/****************************************
* ��ͼ��
*****************************************/
class MapO {
private:
	int hardSize = 40, softSize = 20;//Ӳǽ����ǽ�ߴ�
	int N_HardWall = 10;//Ӳǽ����
	MapObj HardWall, SoftWall, NormalFood, BigFood, GoodFood, BadFood;
	int R_normalFood = 3;
	int R_bigFood = 6;
	int C_NormalFood[100];
	int C_BigFood[100];
	//������
	RECT SpeedArea = { MAP_W / 2 - 300,MAP_H / 2 - 300,MAP_W / 2 + 300,MAP_H / 2 + 300 };

public:
	//���캯��
	MapO() {
		HardWall.num = 10;
		SoftWall.num = 0;
		GoodFood.num = 30;
		BadFood.num = 5;
		NormalFood.num = 100;
		BigFood.num = 0;
		InitFood();
		InitHardWall();
	}
	//��ȡ��ͼ�����������
	struct MapObj getMapObj(int c) {
		switch (c) {
		case 1:return HardWall; break;
		case 2:return SoftWall; break;
		case 3:return GoodFood; break;
		case 4:return BadFood; break;
		case 5:return NormalFood; break;
		case 6:return BigFood; break;
		}
		return HardWall;
	}
	//���õ�ͼ�����������
	void SetMap(MapObj hw, MapObj sw, MapObj goodFood, MapObj  badFood, MapObj nFood, MapObj bigFood){
		HardWall = hw; SoftWall = sw;
		GoodFood = goodFood; BadFood = badFood;
		NormalFood = nFood; BigFood = bigFood;
	}

	/****************************************
	���ܣ���ʼ����ͨʳ�ﾫ����Ͷ��
	*****************************************/
	void InitFood() {
		srand((unsigned)time(NULL));
		for (int i = 0; i < NormalFood.num; i++) {
			NormalFood.pos[i][0] = rand() % (MAP_W - 40) + 20;
			NormalFood.pos[i][1] = rand() % (MAP_H - 40) + 20;
			C_NormalFood[i] = COLORSETSS[rand() % 20];
			C_BigFood[i] = COLORSETSS[rand() % 20];
		}
		for (int i = 0; i < GoodFood.num; i++) {
			GoodFood.pos[i][0] = rand() % (MAP_W - 40) + 20;
			GoodFood.pos[i][1] = rand() % (MAP_H - 40) + 20;
		}
		for (int i = 0; i < BadFood.num; i++) {
			BadFood.pos[i][0] = rand() % (MAP_W - 40) + 20;
			BadFood.pos[i][1] = rand() % (MAP_H - 40) + 20;
		}
	}
	/****************************************
	������x,y������꣬color��ɫ
	���ܣ������������ʳ��
	*****************************************/
	void AddBigFood(int x, int y) {
		if (BigFood.num < 100) {
			BigFood.pos[BigFood.num][0] = x;
			BigFood.pos[BigFood.num][1] = y;
			BigFood.num++;
		}		
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ����ͨʳ�ﲢ����һ���µ�
	*****************************************/
	void DeleteNormalFood(int index) {
		if (NormalFood.num > index) {
			srand((unsigned)time(NULL));
			NormalFood.pos[index][0] = rand() % (MAP_W - 40) + 20;
			NormalFood.pos[index][1] = rand() % (MAP_H - 40) + 20;
		}
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ�����������ɵ�ʳ��
	*****************************************/
	void DeleteBigFood(int index) {
		if (BigFood.num > index) {
			for (int i = index; i < BigFood.num - 1; i++) {
				BigFood.pos[i][0] = BigFood.pos[i + 1][0];
				BigFood.pos[i][1] = BigFood.pos[i + 1][1];
			}
			BigFood.num--;
		}
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ�������������һ���µ�
	*****************************************/
	void DeleteGoodFood(int index) {
		if (GoodFood.num > index) {
			srand((unsigned)time(NULL));
			GoodFood.pos[index][0] = rand() % (MAP_W - 40) + 20;
			GoodFood.pos[index][1] = rand() % (MAP_H - 40) + 20;
		}
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ�����������һ���µ�
	*****************************************/
	void DeleteBadFood(int index) {
		if (BadFood.num > index) {
			srand((unsigned)time(NULL));
			BadFood.pos[index][0] = rand() % (MAP_W - 40) + 20;
			BadFood.pos[index][1] = rand() % (MAP_H - 40) + 20;
		}
	}
	//����ʳ��
	void DrawFood() {
		for (int i = 0; i < NormalFood.num; i++) {
			setfillcolor(C_NormalFood[i]);
			solidcircle(NormalFood.pos[i][0], NormalFood.pos[i][1], R_normalFood);
		}
		for (int i = 0; i < BigFood.num; i++) {
			setfillcolor(C_BigFood[i]);
			solidcircle(BigFood.pos[i][0], BigFood.pos[i][1], R_bigFood);
		}
		//IMAGE foodpng;
		//loadimage(&foodpng, _T("resource/6.png"), 20, 20, true);
		
		setlinestyle(PS_SOLID, 5);
		setlinecolor(GREEN);
		for (int i = 0; i < GoodFood.num; i++) {
			//putimage(GoodFood.pos[i][0] - 10, GoodFood.pos[i][1] - 10, &foodpng);
			circle(GoodFood.pos[i][0], GoodFood.pos[i][1], 10);
		}
		//loadimage(&foodpng, _T("resource/7.png"), 20, 20, true);
		setlinecolor(BLACK);
		for (int i = 0; i < BadFood.num; i++) {
			//putimage(BadFood.pos[i][0] - 10, BadFood.pos[i][1] - 10, &foodpng);
			circle(BadFood.pos[i][0], BadFood.pos[i][1], 10);
		}
	}

	/****************************************
	���ܣ���ʼ��Ӳǽ
	*****************************************/
	void InitHardWall() {
		srand((unsigned)time(NULL));
		for (int i = 0; i < HardWall.num; i++) {
			HardWall.pos[i][0] = rand() % (MAP_W - 200) + 100;
			HardWall.pos[i][1] = rand() % (MAP_H - 200) + 100;
		}
	}
	/****************************************
	������indexǽ����
	���ܣ�ɾ��ָ��Ӳǽ������һ���µ�
	*****************************************/
	void DeleteHardWall(int index) {
		if (HardWall.num > index) {
			srand((unsigned)time(NULL));
			HardWall.pos[index][0] = rand() % (MAP_W - 200) + 100;
			HardWall.pos[index][1] = rand() % (MAP_H - 200) + 100;
		}
	}
	/****************************************
	������indexǽ����
	���ܣ�ɾ��ָ����ǽ
	*****************************************/
	void DeleteSoftWall(int index) {
		if (SoftWall.num > index) {
			for (int i = index; i < SoftWall.num - 1; i++) {
				SoftWall.pos[i][0] = SoftWall.pos[i + 1][0];
				SoftWall.pos[i][1] = SoftWall.pos[i + 1][1];
			}
			SoftWall.num--;
		}
	}
	/****************************************
	������x,y�������
	���ܣ��������������ǽ
	*****************************************/
	void AddSoftWall(int x, int y) {
		if (SoftWall.num < 100) {
			SoftWall.pos[SoftWall.num][0] = x;
			SoftWall.pos[SoftWall.num][1] = y;
			SoftWall.num++;
		}	
	}
	//����ǽ
	void DrawWall() {
		//IMAGE wallpng;
		//loadimage(&wallpng, _T("resource/5.png"), softSize, softSize, true);
		setfillcolor(BROWN);
		for (int i = 0; i < SoftWall.num; i++) {
			solidrectangle(SoftWall.pos[i][0] - softSize / 2, SoftWall.pos[i][1] - softSize / 2, SoftWall.pos[i][0] + softSize / 2, SoftWall.pos[i][1] + softSize / 2);
			//putimage(SoftWall.pos[i][0] - softSize / 2, SoftWall.pos[i][1] - softSize / 2, &wallpng);
		}
		//loadimage(&wallpng, _T("resource/4.png"), hardSize, hardSize, true);
		for (int i = 0; i < HardWall.num; i++) {
			solidrectangle(HardWall.pos[i][0] - hardSize / 2, HardWall.pos[i][1] - hardSize / 2, HardWall.pos[i][0] + hardSize / 2, HardWall.pos[i][1] + hardSize / 2);
			//putimage(HardWall.pos[i][0] - hardSize / 2, HardWall.pos[i][1] - hardSize / 2, &wallpng);
		}
	}

	//���Ƶ�ͼ
	void DrawMap() {
		//����
		setfillcolor(WHITE);
		solidrectangle(0, 0, MAP_W, MAP_H);
		//������
		setlinecolor(YELLOW);
		setlinestyle(PS_SOLID, 60);
		rectangle(SpeedArea.left, SpeedArea.top, SpeedArea.right, SpeedArea.bottom);
		//ʳ���ǽ
		DrawFood();
		DrawWall();
	}
};



///****************************************
//* ������Ϸ������
//*****************************************/
class MultiGame {
private:
	int window_w, window_h;
	IMAGE MAP;
	
	//��ť��С��λ��
	RECT b0 = { 900, 330, 1050, 360 };
	RECT b1 = { 900, 360, 1050, 390 };
	RECT b2 = { 900, 390, 1050, 420 };
	RECT b3 = { 900, 420, 1050, 450 };
	RECT b4 = { 900, 450, 1050, 480 };
	RECT b5 = { 900, 480, 1050, 510 };
	RECT b6 = { 900, 510, 1050, 540 };
	RECT b7 = { 900, 540, 1050, 570 };
	RECT b8 = { 900, 570, 1050, 600 };
	RECT b9 = { 900, 630, 1150, 670 };
	RECT b10 = { 900, 630, 1150, 670 };
	RECT Button[30] = { b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10 };
	RECT Button2[10] = { { 40, 480, 300, 550 },{ 360, 480, 620, 550 } };
	
	//10����ҵ���Ϣ
	struct PlayerInfo {
		int hp;
		int score;
		int power;
	}PlayerInfos[10];
	SnakeO Snakes[10];
	char IP[10][40] = { 0 };
	char isInRoom[10] = { 0 };//�Ƿ��ڷ�����
	MapO bigMap;
	
	//�ҵ���Ϣ
	char myName[40] = { 0 };
	int meId;
	ClientHelper me;
	ClientHelper host;
	room_list_t rlt;
	uint32_t roomId;

public:
	//���캯��
	MultiGame(char s[11]) {
		//���õ�ͼ��С�ʹ��ڴ�С
		window_w = 1200; window_w = 700;
		Resize(&MAP, MAP_W, MAP_H);
		strcpy(myName, s);
		myName[39] = '\0';
		meId = 0;
		host.connectToServer("111.231.112.136", 10001, myName);
		me.connectToServer("111.231.112.136", 10001, myName);
		//host.connectToServer("127.0.0.1", 10001, myName);
		//me.connectToServer("127.0.0.1", 10001, myName);
	}

	//��ȡ�����б�
	bool GetRoom() {
		if (me.getRoomList(rlt)) {
			return true;
		}
		else {
			return false;
		}
	}
	
	//���Ʒ����б�
	void DrawRoomList() {
		GetRoom();
		rectangle(0, 0, 900, 430);
		setbkmode(TRANSPARENT);
		settextcolor(WHITE);
		settextstyle(36, 0, _T("����"));
		settextcolor(BLACK);
		settextstyle(20, 0, _T("����"));
		for (uint32_t i = 0; i < rlt.roomNum; ++i) {
			RECT b1 = { 0,50 + (LONG)i * 20,300,50 + (LONG)(i + 1) * 20 };
			char s[100];
			TCHAR S[100];
			sprintf(s, "%s", rlt.rooms[i].roomName);
			for (int j = 0; j < 30; j++) {
				S[j] = s[j];
			}
			drawtext(S, &b1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT b2 = { 300,50 + (LONG)i * 20,600,50 + (LONG)(i + 1) * 20 };
			_stprintf_s(S, _T("%u"), rlt.rooms[i].roomId);
			drawtext(S, &b2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT b3 = { 600,50 + (LONG)i * 20,750,50 + (LONG)(i + 1) * 20 };
			_stprintf_s(S, _T("%u"), rlt.rooms[i].existing);
			drawtext(S, &b3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		setlinecolor(WHITE);
		setlinestyle(PS_SOLID, 3);
		for (int i = 0; i < 2; i++) {
			roundrect(Button[i].left, Button[i].top, Button[i].right, Button[i].bottom, 150, 150);
		}
		settextstyle(36, 0, _T("����"));
		settextcolor(WHITE);
		drawtext(_T("�����ҵķ���"), &Button2[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("���뷿��"), &Button2[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	//����UI
	void DrawUI() {
		setbkmode(TRANSPARENT);
		settextstyle(20, 0, _T("΢���ź�"), 0, 0, 1000, false, false, false);
		TCHAR ip[10][50] = { 0 };
		for (int i = 0; i < 10; i++) {
			for (unsigned int j = 0; j < 40; j++) {
				ip[i][j] = IP[i][j];
			}
		}
		int n = 0;
		for (int i = 0; i < 10; i++) {
			if (isInRoom[i]) {
				settextcolor(COLORSETSS[rand() % 20]);
				drawtext(ip[i], &Button[n], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				n++;
			}
		}
		RECT b[4] = { {80,0,250,50},{250,0,450,50},{450,0,650,50},{650,0,850,50} };
		settextcolor(WHITE);
		settextstyle(20, 0, _T("����"), 0, 0, 1000, false, false, false);
		TCHAR s[50];
		drawtext(ip[meId], &b[0], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("���� %d"), PlayerInfos[meId].hp);
		drawtext(s, &b[1], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("���� %d"), (int)Snakes[meId].getLength());
		drawtext(s, &b[2], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("�÷� %d"), PlayerInfos[meId].score);
		drawtext(s, &b[3], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf_s(s, _T("ESC��������Ϸ"));
		//drawtext(s, &Button[10], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//������
		setfillcolor(GREEN);
		solidroundrect(870, int(300 - 250 * (double)PlayerInfos[meId].power / 100.0), 890, 300, 30, 30);
		setfillcolor(WHITE);
		setlinecolor(YELLOW);
		setlinestyle(PS_SOLID, 5);
		roundrect(870, 50, 890, 300, 30, 30);
		if (PlayerInfos[meId].power < 10) {
			RECT bb = { 300,50,600,100 };
			settextcolor(0X00FF00);
			settextstyle(28, 0, _T("����"), 0, 0, 1000, false, false, false);
			drawtext(_T("���ܲ��㣡��ʱ���䣡"), &bb, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	//�ͻ��������˹��ã�ʵ�ֻ��ƺͼ�ⰴ�������͵Ĺ���
	bool PlayerRun() {
		setbkcolor(BROWN);
		cleardevice();
		SetWorkingImage(&MAP);
		cleardevice();
		bigMap.DrawMap();
		for (int i = 0; i < 10; i++) {
			if (isInRoom[i]) {
				Snakes[i].DrawSnake();
			}
		}
		SetWorkingImage();
		setaspectratio(float(250.0 / MAP_W), float(250.0 / MAP_H));
		putimage(int(900 * MAP_W / 250.0), int(50 * MAP_H / 250.0), &MAP);//����С��ͼ
		setaspectratio(1, 1);
		putimage(50, 50, 800, 600, &MAP, Snakes[meId].getPath().x[0] - 400, Snakes[meId].getPath().y[0] - 300);
		DrawUI();
		FlushBatchDraw();

		//��ⰴ�����޸ķ���
		if (_kbhit()) {
			char key1 = _getch();
			if (key1 == 27) {
				return false;
			}
			char key2;
			int keyNum = 1;
			if (key1 != 'w' && key1 != 'W' && key1 != 'a' && key1 != 'A' && key1 != 's'
				&& key1 != 'S' && key1 != 'd' && key1 != 'D' && key1 != -32) {
				return true;
			}
			if (key1 == -32) {
				key1 = _getch();
			}
			Sleep(30);
			if (_kbhit()) {
				key2 = _getch();
				keyNum++;
				if (key2 == -32) {
					key2 = _getch();
				}
			}
			int angleNext = 100;
			switch (key1) {
			case'w':
			case'W':
			case 72:
				angleNext = -90;
				if (keyNum == 2) {
					if (key2 == 'a' || key2 == 'A' || key2 == 75) {
						angleNext = -135;
					}
					else if (key2 == 'd' || key2 == 'D' || key2 == 77) {
						angleNext = -45;
					}
				}
				break;
			case's':
			case'S':
			case 80:
				angleNext = 90;
				if (keyNum == 2) {
					if (key2 == 'a' || key2 == 'A' || key2 == 75) {
						angleNext = 135;
					}
					else if (key2 == 'd' || key2 == 'D' || key2 == 77) {
						angleNext = 45;
					}
				}
				break;
			case'a':
			case'A':
			case 75:
				angleNext = 180;
				if (keyNum == 2) {
					if (key2 == 'w' || key2 == 'W' || key2 == 72) {
						angleNext = -135;
					}
					else if (key2 == 's' || key2 == 'S' || key2 == 80) {
						angleNext = 135;
					}
				}
				break;
			case'd':
			case'D':
			case 77:
				angleNext = 0;
				if (keyNum == 2) {
					if (key2 == 'w' || key2 == 'W' || key2 == 72) {
						angleNext = -45;
					}
					else if (key2 == 's' || key2 == 'S' || key2 == 80) {
						angleNext = 45;
					}
				}
				break;
			}
			switch (angleNext) {
			case -90:me.sendMsg("1"); break;
			case -45:me.sendMsg("2"); break;
			case 0:me.sendMsg("3"); break;
			case 45:me.sendMsg("4"); break;
			case 90:me.sendMsg("5"); break;
			case 135:me.sendMsg("6"); break;
			case 180:me.sendMsg("7"); break;
			case -135:me.sendMsg("8"); break;
			}
		}
		return true;
	}
	
	//������߼�
	bool HostRun() {
		//��ʼ��		
		char* info = new(nothrow)char[1000];

		strcpy(IP[0], myName);	 
		isInRoom[0] = 1;
		PlayerInfos[0] = { 5,0,100, };
		int PlayerNum = 1;
		MapObj hw, sw, goodFood, badFood, bigFood, nFood;
		struct SnakeNode sn;	
		while (1) {
			////����Ƿ������û�
			char isQuit[10] = { 0 };
			room_t RoomInfo;
			if (host.getRoomInfo(roomId, RoomInfo)) {
				for (uint32_t i = 0; i < RoomInfo.existing; i++) {
					char ip[40] = { 0 };
					strcpy(ip,RoomInfo.everyOne[i].name);//���ڱ��ص���IP����ͬ�����������ִ���
					int j;
					for (j = 0; j < 10; j++) {
						if (strcmp(IP[j], ip) == 0) {
							isQuit[j] = 1;//��ip�Ѵ��ڲ���û���˳�
							break;
						}
					}
					//ip�����������
					if (j == 10 && PlayerNum < 10) {
						//�ҵ�һ����λ
						for (int k = 0; k < 10; k++) {
							if (isInRoom[k] == 0) {
								isInRoom[k] = 1;
								strcpy(IP[k], ip);
								PlayerNum++;
								PlayerInfos[k].hp = 5; PlayerInfos[k].power = 100; PlayerInfos[k].score = 0;
								Snakes[k].InitSnake();
								break;
							}
						}
					}
				}
			}
			//����Ƿ����˳������
			//for (int i = 0; i < 10; i++) {
			//	if (isQuit[i] == 0) {
			//		isInRoom[i] = 0;
			//		PlayerNum--;
			//	}
			//}

			//������Ҳ�����Ϣ
			msg_package_t msg;
			if (host.recvMsg(msg)) {
				for (uint32_t i = 0; i < msg.msgNum; i++) {
					char ip[40] = { 0 };
					strcpy(ip, msg.msgs[i]->name);
					int j;
					for (j = 0; j < 10; j++) {
						if (strcmp(IP[j], ip) == 0) {
							if (msg.msgs[i]->msgLen > 0) {
								char c = msg.msgs[i]->msgContent[0];
								switch (c) {
								case '1':Snakes[j].ChangeOrientation(-90); break;
								case '2':Snakes[j].ChangeOrientation(-45); break;
								case '3':Snakes[j].ChangeOrientation(0); break;
								case '4':Snakes[j].ChangeOrientation(45); break;
								case '5':Snakes[j].ChangeOrientation(90); break;
								case '6':Snakes[j].ChangeOrientation(135); break;
								case '7':Snakes[j].ChangeOrientation(180); break;
								case '8':Snakes[j].ChangeOrientation(-135); break;
								}
							}
							break;
						}
					}
				}
			}

			//���ƶ�
			for (int frame = 0; frame < 4; frame++) {
				for (int i = 0; i < 10; i++) {
					if (isInRoom[i] == 0)continue;
					//�����Ƿ��ڼ����������ٶȲ��ƶ�
					Snakes[i].setSpeed();
					Snakes[i].Move();
					//��ȡ�߽ڵ�
					sn = Snakes[i].getPath();
					//��ȡ��ͼ�ϵ�����
					hw = bigMap.getMapObj(1);
					sw = bigMap.getMapObj(2);
					goodFood = bigMap.getMapObj(3);
					badFood = bigMap.getMapObj(4);
					nFood = bigMap.getMapObj(5);
					bigFood = bigMap.getMapObj(6);

					int index;
					//ײ���߽�
					if (Snakes[i].CheckBorder()) {
						for (int k = 0; k < sn.length; k++) {
							bigMap.AddSoftWall(sn.x[k], sn.y[k]);
						}
						Snakes[i].InitSnake();
						PlayerInfos[i].power = 100;
						PlayerInfos[i].hp--;
						PlayerInfos[i].score -= 2;
						break;
					}
					//ײ����ǽ
					else if ((index = Snakes[i].CheckSoftWall(sw.pos, sw.num)) != -1) {
						PlayerInfos[i].score -= 3;
						PlayerInfos[i].power -= 5;
						bigMap.DeleteSoftWall(index);
					}
					//ײ��Ӳǽ
					else if ((index = Snakes[i].CheckHardWall(hw.pos)) != -1) {
						PlayerInfos[i].score -= 5;
						PlayerInfos[i].power -= 10;
						bigMap.DeleteHardWall(index);
					}
					//�Ե����
					else if ((index = Snakes[i].CheckFood(badFood.pos, 10, badFood.num, 0)) != -1) {
						PlayerInfos[i].score -= 5;
						PlayerInfos[i].hp--;
						PlayerInfos[i].power = 100;
						Snakes[i].InitSnake();
						bigMap.DeleteBadFood(index);
					}
					//�Ե������
					else if ((index = Snakes[i].CheckFood(goodFood.pos, 10, goodFood.num, 0)) != -1) {
						PlayerInfos[i].score += 10;
						PlayerInfos[i].power = 100;
						bigMap.DeleteGoodFood(index);
					}
					//�Ե���ʳ��
					else if ((index = Snakes[i].CheckFood(bigFood.pos, 6, bigFood.num, 1)) != -1) {
						PlayerInfos[i].score += 5;
						PlayerInfos[i].power -= 2;
						bigMap.DeleteBigFood(index);
					}
					//�Ե���ͨʳ��
					else if ((index = Snakes[i].CheckFood(nFood.pos, 3, nFood.num, 0.2)) != -1) {
						PlayerInfos[i].score += 1;
						PlayerInfos[i].power -= 1;
						bigMap.DeleteNormalFood(index);
					}
					//ײ��������
					for (int k = 0; k < 10; k++) {
						if (isInRoom[k] && i != k && Snakes[i].CheckCrashOther(Snakes[k].getPath())) {
							PlayerInfos[i].hp--;
							PlayerInfos[i].score -= 5;
							PlayerInfos[i].power = 100;
							for (int j = 0; j < sn.length; j++) {
								bigMap.AddBigFood(sn.x[j], sn.y[j]);
							}
							Snakes[i].InitSnake();
							break;
						}
					}
				}

				//���͵�ͼ��Ϣ
				MapObj mapobj[6];				
				mapobj[0] = bigMap.getMapObj(1);
				mapobj[1] = bigMap.getMapObj(2);
				mapobj[2] = bigMap.getMapObj(3);
				mapobj[3] = bigMap.getMapObj(4);
				mapobj[4] = bigMap.getMapObj(5);
				mapobj[5] = bigMap.getMapObj(6);
				host.sendBinaryMsg((const char*)mapobj, sizeof(mapobj));
				
				//��������Ϣ
				SnakeInfo b[10];
				for (int k = 0; k < 10; k++) {
					b[k] = { Snakes[k].getColor(),Snakes[k].getAngle(),Snakes[k].getPath() };
				}
				host.sendBinaryMsg((const char*)b, sizeof(b));
				
				////�����Ϣ���ⲿ�ֻ���ɳ����쳣
				//char is[11] = { 0 };
				//for (int k = 0; k < 10; k++) {
				//	is[k] = isInRoom[k];
				//}
				//host.sendBinaryMsg((const char*)isInRoom, sizeof(is));
				//host.sendBinaryMsg((const char*)PlayerInfos, sizeof(PlayerInfos));
				//char aip[400] = {0};
				//for (int k = 0; k < 10; k++) {
				//	for (int l = 0; l < 40; l++) {
				//		aip[40 * k + l] = IP[k][l];
				//	}
				//}
				//host.sendBinaryMsg((const char*)aip, sizeof(aip));
				
				PlayerRun();
			}
		}
		return true;
	}

	//�ͻ����߼�
	void ClientRun() {
		//���ӵ�����
		me.getRoomList(rlt);
		roomId = rlt.rooms[0].roomId;
		bool isPlaying = true;
		if (me.connectToRoom(roomId)) isPlaying = true;//���ӳɹ�������Ϸ
		while (isPlaying) {
			//����
			msg_package_t msg;
			if (me.recvMsg(msg)) {
				for (uint32_t i = 0; i < msg.msgNum; i++) {
					if (msg.msgs[i]->msgLen == 2412) {
						struct MapObj mapobj[6];
						memcpy(mapobj, msg.msgs[i]->msgContent, msg.msgs[i]->msgLen);
						bigMap.SetMap(mapobj[0], mapobj[1], mapobj[2], mapobj[3], mapobj[4], mapobj[5]);
					}
					else if (msg.msgs[i]->msgLen == 8160) {
						struct SnakeInfo sn[10];
						memcpy(sn, msg.msgs[i]->msgContent, msg.msgs[i]->msgLen);
						for (int k = 0; k < 10; k++) {
							Snakes[k].Set(sn[k]);
						}
					}
					//else if (msg.msgs[i]->msgLen == 11) {
					//	char is[11] = { 0 };
					//	memcpy(is, msg.msgs[i]->msgContent, msg.msgs[i]->msgLen);
					//	for (int k = 0; k < 10; k++) {
					//		isInRoom[k] = is[k];
					//	}
					//}
					//else if (msg.msgs[i]->msgLen == 120) {
					//	memcpy(PlayerInfos, msg.msgs[i]->msgContent, msg.msgs[i]->msgLen);
					//}
					//else if (msg.msgs[i]->msgLen == 400) {
					//	for (int k = 0; k < 10; k++) {
					//		for (int l = 0; l < 40; l++) {
					//			IP[k][l] = msg.msgs[i]->msgContent[40 * k + l];
					//		}
					//	}
					//}

				}
			}
			PlayerRun();
		}
	}

	//ѡ�񷿼�
	void ChooseRoom() {
		MOUSEMSG m{ 0 };
		initgraph(WINDOW_W, WINDOW_H);
		BeginBatchDraw();
		while (1) {
			cleardevice();
			Resize(NULL, WINDOW_W, WINDOW_H);
			loadimage(NULL, _T("resource/8.jpg"), WINDOW_W, WINDOW_H);
			DrawRoomList();
			//�������¼�
			if (MouseHit()) m = GetMouseMsg();
			int i;
			switch (m.uMsg) {
			case WM_LBUTTONDOWN:
				for (i = 0; i < 2; i++) {
					if (m.x > Button2[i].left&& m.x<Button2[i].right && m.y>Button2[i].top&& m.y < Button2[i].bottom) {
						break;
					}
				}
				if (i < 2) {
					if (i == 0) {
						loadimage(NULL, _T("resource/8.jpg"), WINDOW_W, WINDOW_H);
						cleardevice();
						MOUSEMSG m{ 0 };
						//���뷿����
						bool inputing = true;
						TCHAR namebuffer[15] = _T("Snake_");
						TCHAR s[50];
						int nameLen = 6;
						RECT inputBox = { WINDOW_W / 2 - 180,420,WINDOW_W / 2 + 200,470 };
						RECT b[2] = { {300,500,600,540},{300,550,600,600} };
						setbkmode(TRANSPARENT);
						int inputCursor = 0;
						int choice = 0;
						while (inputing) {
							while (!_kbhit()) {
								if (MouseHit()) m = GetMouseMsg();
								switch (m.uMsg) {
								case WM_LBUTTONDOWN:
									if (m.x > b[0].left&& m.x<b[0].right && m.y>b[0].top&& m.y < b[0].bottom) {
										if (nameLen > 6) {
											inputing = false;
										}
									}
									else if (m.x > b[1].left&& m.x<b[1].right && m.y>b[1].top&& m.y < b[1].bottom) {
										break;
									}
									break;
								case WM_MOUSEMOVE:
									if (m.x > b[0].left&& m.x<b[0].right && m.y>b[0].top&& m.y < b[0].bottom) {
										choice = 1;

									}
									else if (m.x > b[1].left&& m.x<b[1].right && m.y>b[1].top&& m.y < b[1].bottom) {
										choice = 2;
									}
									else {
										choice = 0;
									}
								}
								setlinestyle(PS_SOLID, 5);
								roundrect(b[0].left, b[0].top, b[0].right, b[0].bottom, 50, 50);
								roundrect(b[1].left, b[1].top, b[1].right, b[1].bottom, 50, 50);
								if (choice != 0) {
									setlinecolor(RED);
									roundrect(b[choice - 1].left, b[choice - 1].top, b[choice - 1].right, b[choice - 1].bottom, 50, 50);
								}
								setlinecolor(WHITE);
								settextstyle(36, 0, _T("����"));
								drawtext(_T("ȷ�ϲ�����"), &b[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								drawtext(_T("ȡ��������"), &b[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								settextstyle(30, 0, _T("΢���ź�"));
								_stprintf_s(s, _T("�����뷿����(<10): %s"), namebuffer);
								drawtext(s, &inputBox, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
								if (inputCursor < 50) {
									setlinestyle(PS_SOLID, 5);
									line(WINDOW_W / 2 + 22 + nameLen * 13, 430, WINDOW_W / 2 + 22 + nameLen * 13, 460);
									inputCursor++;
								}
								else {
									inputCursor++;
									if (inputCursor > 100) {
										inputCursor = 0;
									}
								}
								setlinestyle(PS_SOLID, 8);
								roundrect(inputBox.left - 20, inputBox.top, inputBox.right, inputBox.bottom, 50, 50);
								FlushBatchDraw();
								cleardevice();
								loadimage(NULL, _T("resource/8.jpg"), WINDOW_W, WINDOW_H);
								if (!inputing) {
									break;
								}
							}
							if (!inputing) {
								break;
							}
							char c = _getch();
							if (c >= '0' && c <= '9') {
								if (nameLen < 10) {
									namebuffer[nameLen++] = c;
									namebuffer[nameLen] = '\0';
								}
							}
							else if (c == 8) {
								if (nameLen > 6) {
									nameLen--;
									namebuffer[nameLen] = '\0';
								}
							}
						}
						Sleep(100);
						char name[21];
						for (int i = 0; i < 15; i++) {
							name[i] = (char)namebuffer[i];
						}
						name[15] = '\0';
						if (!host.hostNewRoom(name, roomId)) {
							break;
						}
						//�Ѿ������˷���
						if (!me.connectToRoom(roomId)) {
							break;
						}
						initgraph(1200, 700);
						HostRun();
					}
					else if (i == 1) {
						initgraph(1200, 700);
						ClientRun();
					}
				}
				break;
			}
			FlushBatchDraw();
		}
		closegraph();             //�˳�ͼ�ν���	
	}
};