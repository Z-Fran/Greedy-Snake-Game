#define _CRT_SECURE_NO_WARNINGS
#include "Client.h"
#include<graphics.h>
#include<conio.h>
#include<cmath>
#include<stdlib.h>
#include<ctime>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<strstream>
using namespace std;
using namespace NetworkHelper;
#define PI 3.1415926
#define MAP_W 2000
#define MAP_H 2000
#define WINDOW_W 900
#define WINDOW_H 600
//�������������ɫ
DWORD COLORSETS[30] = { 0X808080,0X5C5CCD,0X8080F0,0X4763FF,0X507FFF,0X008CFF,0XADDEFF,0X00FFFF,0X2FFFAD,0X00FF00,
					0X7FFF00,0XD0E040,0XEEEEAF,0XFACE87,0XE16941,0XFF901E,0X00D7FF,0XD30094,0XDDA0DD,0XC1B6FF };

/****************************************
* ����
*****************************************/
class SnakeA {
private:
	double length;//����
	DWORD color;//��ɫ
	int radius;//�뾶
	int angle;//��ͷ����
	int speed;//�ٶ�
	int frame;//֡��
	int frame_now;
	int isInit;
	vector<int> pathX;//�߽ڵ㵱ǰ��x����
	vector<int> pathY;//�߽ڵ㵱ǰ��y����
	vector<int> pathX_nextframe;//�߽ڵ���һ֡x����
	vector<int> pathY_nextframe;//�߽ڵ���һ֡y����
	vector<int> pathX_lastframe;//�߽ڵ���һ֡x����
	vector<int> pathY_lastframe;//�߽ڵ���һ֡y����

public:
	//���캯��
	SnakeA() {
		speed = 5;
		length = 3;
		radius = 10;
		angle = 0;
		color = COLORSETS[rand() % 20];
		frame = radius * 2 / speed;
		frame_now = 0;
		isInit = 0;
	}
	SnakeA(vector<vector<vector<int>>>wall) {
		InitSnake(wall);
	}
	/****************************************
	������wallǽ������
	���ܣ���ʼ��һ����
	*****************************************/
	void InitSnake(vector<vector<vector<int>>>wall) {
		srand((unsigned)time(NULL));
		//���Գ�ʼ��
		speed = 5;
		length = 3;
		radius = 10;
		angle = rand() % 360;
		color = COLORSETS[rand() % 20];
		frame = radius * 2 / speed;
		frame_now = 0;
		isInit = 0;
		pathX.clear();
		pathY.clear();
		pathX_nextframe.clear();
		pathY_nextframe.clear();
		//��ȡǽ������
		vector<vector<int>> HardWall = wall[0];
		vector<vector<int>> SoftWall = wall[1];
		int headX, headY;
		bool flag = true;
		while (flag) {
			flag = false;
			headX = rand() % (MAP_W - 60) + 30 ;
			headY = rand() % (MAP_H - 60) + 30 ;
			unsigned int i;
			for (i = 0; i < HardWall[0].size(); i++) {
				if (pow(headX - HardWall[0][i], 2) + pow(headY - HardWall[1][i], 2) < pow(radius + 10, 2)) {
					flag = true;
					break;
				}
			}
			for (i = 0; i < SoftWall[0].size(); i++) {
				if (pow(headX - SoftWall[0][i], 2) + pow(headY - SoftWall[1][i], 2) < pow(radius + 10, 2)) {
					flag = true;
					break;
				}
			}
		}
		pathX.insert(pathX.begin(), (int)length, headX);
		pathY.insert(pathY.begin(), (int)length, headY);
		pathX_nextframe.insert(pathX_nextframe.begin(), pathX.begin(), pathX.end());
		pathY_nextframe.insert(pathY_nextframe.begin(), pathY.begin(), pathY.end());

	}
	//�����ٶ�
	void setSpeed() {
		int s;
		if (pathX[0] > 700 && pathX[0] < 1300 && pathY[0]>700 && pathY[0] < 1300) {
			s = 10;
		}
		else{
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
		length = l;
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
	vector<vector<int>> getPath() {
		vector<vector<int>> vec = { pathX,pathY };
		return vec;
	}
	vector<vector<int>> getNextPath() {
		vector<vector<int>> vec = { pathX_nextframe,pathY_nextframe };
		return vec;
	}
	vector<vector<int>> getLastPath() {
		vector<vector<int>> vec = { pathX_lastframe,pathY_lastframe };
		return vec;
	}
	void Set(vector<vector<int>> path) {
		pathX.clear();
		pathY.clear();
		pathX_nextframe.clear();
		pathY_nextframe.clear();
		pathX.insert(pathX.begin(), path[0].begin(), path[0].end());
		pathY.insert(pathY.begin(), path[1].begin(), path[1].end());
		pathX_nextframe.insert(pathX_nextframe.begin(), pathX.begin(), pathX.end());
		pathY_nextframe.insert(pathY_nextframe.begin(), pathY.begin(), pathY.end());
		length = pathX.size();
	}
	/****************************************
	���ܣ�������
	*****************************************/
	void DrawSnake() {
		int n = (int)pathX.size();
		setfillcolor(color);
		for (int i = 0; i < n; i++) {
			solidcircle(pathX[i], pathY[i], radius);
		}
		setfillcolor(BLACK);
		solidcircle(pathX[0] + int(((double)radius - 1) * cos(((double)angle - 45) / 180.0 * PI)), pathY[0] + int(((double)radius - 1) * sin(((double)angle - 45) / 180.0 * PI)), 4);
		solidcircle(pathX[0] + int(((double)radius - 1) * cos(((double)angle + 45) / 180.0 * PI)), pathY[0] + int(((double)radius - 1) * sin(((double)angle + 45) / 180.0 * PI)), 4);
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
			pathX.clear();
			pathY.clear();
			//vector<int>().swap(pathX);
			//vector<int>().swap(pathY);
			pathX.insert(pathX.begin(), pathX_nextframe.begin(), pathX_nextframe.end());
			pathY.insert(pathY.begin(), pathY_nextframe.begin(), pathY_nextframe.end());
			//�ߵĳ�������ʱ��ӽڵ�
			if ((unsigned int)length > pathX.size()) {
				pathX.push_back(pathX_lastframe[pathX.size() - 1]);
				pathY.push_back(pathY_lastframe[pathY.size() - 1]);
			}
			//�ߵĳ��ȼ���ʱɾ���ڵ�
			else if ((unsigned int)length < pathX.size()) {
				pathX.erase(pathX.end() - 1);
				pathY.erase(pathY.end() - 1);
			}
			pathX_nextframe.clear();
			pathY_nextframe.clear();
			pathX_lastframe.clear();
			pathY_lastframe.clear();
			//vector<int>().swap(pathX_nextframe);
			//vector<int>().swap(pathY_nextframe);
			//vector<int>().swap(pathX_lastframe);
			//vector<int>().swap(pathY_lastframe);
			pathX_nextframe.insert(pathX_nextframe.begin(), pathX.begin(), pathX.end() - 1);
			pathY_nextframe.insert(pathY_nextframe.begin(), pathY.begin(), pathY.end() - 1);
			pathX_nextframe.insert(pathX_nextframe.begin(), pathX[0] + int(2 * radius * cos((double)angle / 180.0 * PI)));
			pathY_nextframe.insert(pathY_nextframe.begin(), pathY[0] + int(2 * radius * sin((double)angle / 180.0 * PI)));
			pathX_lastframe.insert(pathX_lastframe.begin(), pathX.begin(), pathX.end());
			pathY_lastframe.insert(pathY_lastframe.begin(), pathY.begin(), pathY.end());
		}
		else {
			for (unsigned int i = 0; i < pathY_nextframe.size(); i++) {
				double degree = atan2(pathY_nextframe[i] - pathY_lastframe[i], pathX_nextframe[i] - pathX_lastframe[i]);
				pathX[i] = pathX[i] + int(speed * cos(degree));
				pathY[i] = pathY[i] + int(speed * sin(degree));
			}
		}
		frame_now = (frame_now + 1) % frame;//���µ�ǰ֡��
	}

	//����Ƿ�ײ���߽�
	bool CheckBorder() {
		if (pathX[0] - 10 > 0 && pathX[0] + 10 < MAP_W && pathY[0] - 10 > 0 && pathY[0] + 10 < MAP_H) {
			return false;
		}
		else {
			return true;
		}
	}
	//����Ƿ�ײ��Ӳǽ
	int CheckHardWall(vector<vector<int>> HardWall) {
		for (unsigned int i = 0; i < HardWall[0].size();i++) {
			if (pow(pathX[0] - HardWall[0][i], 2) + pow(pathY[0] - HardWall[1][i], 2) <= pow(radius + 20, 2)) {
				length /= 2;
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�ײ����ǽ��
	int CheckSoftWall(vector<vector<int>> SoftWall) {
		for (unsigned int i = 0; i < SoftWall[0].size(); i++) {
			if (pow(pathX[0] - SoftWall[0][i], 2) + pow(pathY[0] - SoftWall[1][i], 2) <= pow(radius + 10, 2)) {
				length -= 1;
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�Ե���ͨʳ��
	int CheckNormalFood(vector<vector<int>> food) {
		for (unsigned int i = 0; i < food[0].size(); i++) {
			if (pow(pathX[0] - food[0][i], 2) + pow(pathY[0] - food[1][i], 2) <= pow(radius + 3, 2)) {
				length += 0.2;
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�Ե����������ɵ�ʳ��
	int CheckBigFood(vector<vector<int>> food) {
		for (unsigned int i = 0; i < food[0].size(); i++) {
			if (pow(pathX[0] - food[0][i], 2) + pow(pathY[0] - food[1][i], 2) <= pow(radius + 6, 2)) {
				length += 1;
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�Ե������
	int CheckGoodFood(vector<vector<int>> food) {
		for (unsigned int i = 0; i < food[0].size(); i++) {
			if (pow(pathX[0] - food[0][i], 2) + pow(pathY[0] - food[1][i], 2) <= pow(radius + 10, 2)) {
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�Ե����
	int CheckBadFood(vector<vector<int>> food) {
		for (unsigned int i = 0; i < food[0].size(); i++) {
			if (pow(pathX[0] - food[0][i], 2) + pow(pathY[0] - food[1][i], 2) <= pow(radius + 10, 2)) {
				return i;
			}
		}
		return -1;
	}
	//����Ƿ�ײ��������
	bool CheckCrashOther(vector<vector<int>> pos) {
		//�Ƿ����޵�״̬
		if (isInit < 20) {
			return false;
		}
		for (unsigned int i = 1; i < pos[0].size(); i++) {
			if (pow(pathX[0] - pos[0][i], 2) + pow(pathY[0] - pos[1][i], 2) <= pow(2 * radius, 2)) {
				return true;
			}
		}
		return false;
	}
};

/****************************************
* ��ͼ��
*****************************************/
class MapA {
private:
	DWORD fillColor;//��ͼ����ɫ
	int hardSize, softSize;//Ӳǽ����ǽ�ߴ�
	vector<vector<int>> HardWall, SoftWall;//ǽ����
	int N_HardWall;//Ӳǽ����

	vector<int> X_normalFood, Y_normalFood;//��ͨʳ������
	vector<DWORD> C_normalFood;//��ͨʳ����ɫ
	int N_normalFood;//��ͨʳ������
	int R_normalFood;//��ͨʳ��뾶
	//���������ɵ�ʳ��
	vector<int> X_bigFood, Y_bigFood;
	vector<DWORD> C_bigFood;
	int R_bigFood;
	//�����������������
	vector<vector<int>> GoodFood, BadFood;
	int N_GoodFood, N_BadFood;
	//������
	RECT SpeedArea = { MAP_W/2 - 300,MAP_H/2 - 300,MAP_W/2 + 300,MAP_H/2 + 300 };

public:
	//���캯��
	MapA() {
		HardWall.resize(2);
		SoftWall.resize(2);
		fillColor = WHITE;
		hardSize = 40;
		softSize = 20;
		N_HardWall = 20;
		R_normalFood = 3;
		N_normalFood = 500;
		R_bigFood = 6;
		GoodFood.resize(2);
		BadFood.resize(2);
		N_GoodFood = 30;
		N_BadFood = 10;
		InitFood();
		InitHardWall();
	}
	
	void SetMap(vector<vector<int>>hw, vector<vector<int>>sw, vector<vector<int>>gf, vector<vector<int>>bf) {
		HardWall = hw;
		SoftWall = sw;
		GoodFood = gf;
		BadFood = bf;
		N_BadFood = BadFood[0].size();
		N_GoodFood = GoodFood[0].size();
		N_HardWall = HardWall[0].size();
	}
	//��ȡʳ����ɫ
	vector<DWORD> getNormalC() {
		return C_normalFood;
	}
	vector<DWORD> getBigC() {
		return C_bigFood;
	}
	//��ȡ��ͨʳ������
	vector<vector<int>> getNormalFood() {
		vector<vector<int>> vec;
		vec.push_back(X_normalFood);
		vec.push_back(Y_normalFood);
		return vec;
	}
	//��ȡ����������ʳ������
	vector<vector<int>> getBigFood() {
		vector<vector<int>> vec;
		vec.push_back(X_bigFood);
		vec.push_back(Y_bigFood);
		return vec;
	}
	//��ȡ���������
	vector<vector<int>> getGoodFood() {
		return GoodFood;
	}
	//��ȡ�������
	vector<vector<int>> getBadFood() {
		return BadFood;
	}
	//��ȡ��ǽ��Ӳǽ����
	vector<vector<vector<int>>> getWall() {
		vector<vector<vector<int>>> wall = { HardWall,SoftWall };
		return wall;
	}
	//���ĳ���Ƿ��ڼ�����
	bool isSpeedArea(int x, int y) {
		if (x > SpeedArea.left&& x<SpeedArea.right && y>SpeedArea.top&& y < SpeedArea.bottom) {
			return true;
		}
		else {
			return false;
		}
	}

	/****************************************
	���ܣ���ʼ����ͨʳ�ﾫ����Ͷ��
	*****************************************/
	void InitFood() {
		srand((unsigned)time(NULL));
		int x, y;
		X_normalFood.clear();
		Y_normalFood.clear();
		C_normalFood.clear();
		for (int i = 0; i < N_normalFood; i++) {
			x = rand() % (MAP_W - 40) + 20;
			y = rand() % (MAP_H - 40) + 20;
			X_normalFood.push_back(x);
			Y_normalFood.push_back(y);
			C_normalFood.push_back(COLORSETS[rand() % 20]);
		}
		BadFood[0].clear();
		BadFood[1].clear();
		for (int i = 0; i < N_BadFood; i++) {
			BadFood[0].push_back(rand() % (MAP_W - 40) + 20);
			BadFood[1].push_back(rand() % (MAP_H - 40) + 20);
		}
		GoodFood[0].clear();
		GoodFood[1].clear();
		for (int i = 0; i <= N_GoodFood; i++) {
			GoodFood[0].push_back(rand() % (MAP_W - 40) + 20);
			GoodFood[1].push_back(rand() % (MAP_H - 40) + 20);
		}
	}
	/****************************************
	������x,y������꣬color��ɫ
	���ܣ������������ʳ��
	*****************************************/
	void AddBigFood(vector<int> x, vector<int> y, DWORD color) {
		X_bigFood.insert(X_bigFood.begin(), x.begin() + 1, x.end());
		Y_bigFood.insert(Y_bigFood.begin(), y.begin() + 1, y.end());
		C_bigFood.insert(C_bigFood.begin(), x.size() - 1, color);
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ����ͨʳ�ﲢ����һ���µ�
	*****************************************/
	void DeleteNorFood(int index) {
		if ((int)X_normalFood.size() > index) {
			X_normalFood.erase(X_normalFood.begin() + index);
			Y_normalFood.erase(Y_normalFood.begin() + index);
			C_normalFood.erase(C_normalFood.begin() + index);
			srand((unsigned)time(NULL));
			X_normalFood.push_back(rand() % (MAP_W - 40) + 20);
			Y_normalFood.push_back(rand() % (MAP_H - 40) + 20);
			C_normalFood.push_back(RGB(rand() % 200, rand() % 200, rand() % 200));
		}
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ�����������ɵ�ʳ��
	*****************************************/
	void DeleteBigFood(int index) {
		if ((int)X_bigFood.size() > index) {
			X_bigFood.erase(X_bigFood.begin() + index);
			Y_bigFood.erase(Y_bigFood.begin() + index);
			C_bigFood.erase(C_bigFood.begin() + index);
		}
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ�������������һ���µ�
	*****************************************/
	void DeleteGoodFood(int index) {
		if ((int)GoodFood[0].size() > index) {
			GoodFood[0].erase(GoodFood[0].begin() + index);
			GoodFood[1].erase(GoodFood[1].begin() + index);
			srand((unsigned)time(NULL));
			GoodFood[0].push_back(rand() % (MAP_W - 40) + 20);
			GoodFood[1].push_back(rand() % (MAP_H - 40) + 20);

		}
	}
	/****************************************
	������indexʳ������
	���ܣ�ɾ��ָ�����������һ���µ�
	*****************************************/
	void DeleteBadFood(int index) {
		if ((int)BadFood[0].size() > index) {
			BadFood[0].erase(BadFood[0].begin() + index);
			BadFood[1].erase(BadFood[1].begin() + index);
			srand((unsigned)time(NULL));
			BadFood[0].push_back(rand() % (MAP_W - 40) + 20);
			BadFood[1].push_back(rand() % (MAP_H - 40) + 20);
		}
	}
	//����ʳ��
	void DrawFood() {
		for (int i = 0; i < N_normalFood; i++) {
			setfillcolor(C_normalFood[i]);
			solidcircle(X_normalFood[i], Y_normalFood[i], R_normalFood);
		}
		for (unsigned int i = 0; i < X_bigFood.size(); i++) {
			setfillcolor(C_bigFood[i]);
			solidcircle(X_bigFood[i], Y_bigFood[i], R_bigFood);
		}
		IMAGE foodpng;
		loadimage(&foodpng, _T("resource/6.png"), 20, 20, true);
		for (int i = 0; i < N_GoodFood; i++) {
			putimage(GoodFood[0][i] - 10, GoodFood[1][i] - 10, &foodpng);
		}
		loadimage(&foodpng, _T("resource/7.png"), 20, 20, true);
		for (int i = 0; i < N_BadFood; i++) {
			putimage(BadFood[0][i] - 10, BadFood[1][i] - 10, &foodpng);
		}
	}

	/****************************************
	���ܣ���ʼ��Ӳǽ
	*****************************************/
	void InitHardWall() {
		srand((unsigned)time(NULL));
		int x, y;
		for (int i = 0; i < N_HardWall; i++) {
			x = rand() % (MAP_W - 200) + 100;
			y = rand() % (MAP_H - 200) + 100;
			HardWall[0].push_back(x);
			HardWall[1].push_back(y);
		}
	}
	/****************************************
	������indexǽ����
	���ܣ�ɾ��ָ��Ӳǽ������һ���µ�
	*****************************************/
	void DeleteHardWall(int index) {
		if ((int)HardWall[0].size() > index) {
			HardWall[0].erase(HardWall[0].begin() + index);
			HardWall[1].erase(HardWall[1].begin() + index);
			srand((unsigned)time(NULL));
			HardWall[0].push_back(rand() % (MAP_W - 100) + 50);
			HardWall[1].push_back(rand() % (MAP_H - 100) + 50);
		}
	}
	/****************************************
	������indexǽ����
	���ܣ�ɾ��ָ����ǽ
	*****************************************/
	void DeleteSoftWall(int index) {
		if ((int)SoftWall[0].size() > index) {
			SoftWall[0].erase(SoftWall[0].begin() + index);
			SoftWall[1].erase(SoftWall[1].begin() + index);
		}
	}
	/****************************************
	������x,y�������
	���ܣ��������������ǽ
	*****************************************/
	void AddSoftWall(vector<int> x, vector<int> y) {
		SoftWall[0].insert(SoftWall[0].begin(), x.begin(), x.end());
		SoftWall[1].insert(SoftWall[1].begin(), y.begin(), y.end());
	}
	//����ǽ
	void DrawWall() {
		IMAGE wallpng;
		loadimage(&wallpng, _T("resource/5.png"), softSize, softSize, true);
		int n = (int)SoftWall[0].size();
		for (int i = 0; i < n; i++) {
			putimage(SoftWall[0][i] - softSize / 2, SoftWall[1][i] - softSize / 2, &wallpng);
		}
		loadimage(&wallpng, _T("resource/4.png"), hardSize, hardSize, true);
		n = (int)HardWall[0].size();
		for (int i = 0; i < n; i++) {
			putimage(HardWall[0][i] - hardSize / 2, HardWall[1][i] - hardSize / 2, &wallpng);
		}
	}

	//���Ƶ�ͼ
	void DrawMap() {
		//����
		setfillcolor(fillColor);
		solidrectangle(0, 0, MAP_W, MAP_H);
		//������
		setlinecolor(YELLOW);
		setlinestyle(PS_SOLID, 60);
		rectangle(SpeedArea.left, SpeedArea.top, SpeedArea.right, SpeedArea.bottom);
		settextcolor(YELLOW);
		setbkmode(TRANSPARENT);
		settextstyle(300, 0, _T("΢���ź�"));
		TCHAR s[50];
		_stprintf_s(s, _T(">>>\n<<<"));
		drawtext(s, &SpeedArea, DT_CENTER);
		//ʳ���ǽ
		DrawFood();
		DrawWall();
	}
};

/****************************************
������bigMap��ͼ����AI����������AI��p1���1��p2���2
���ܣ�ʵ��AI�����߼�
*****************************************/
void AIControl(MapA& bigMap, SnakeA AI[], SnakeA p1, SnakeA p2) {
	vector<vector<int>>BadGoal, GoodGoal;//������Ŀ��λ�ú�������Ŀ��λ��
	vector<double> GoodDistance;//������Ŀ��ľ���
	vector<double> BadDistance;//������Ŀ��ľ���
	BadGoal.resize(2);
	GoodGoal.resize(2);
	//����10��AI
	for (int i = 0; i < 10; i++) {
		BadGoal[0].clear();
		BadGoal[1].clear();
		GoodGoal[0].clear();
		GoodGoal[1].clear();
		GoodDistance.clear();
		vector<int>().swap(BadGoal[0]);
		vector<int>().swap(BadGoal[1]);
		vector<int>().swap(GoodGoal[0]);
		vector<int>().swap(GoodGoal[1]);
		vector<double>().swap(GoodDistance);
		vector<vector<int>> path = AI[i].getPath();
		int headX = path[0][0];
		int headY = path[1][0];
		int searchRange = 50;//�ٽ緶Χ
		int crashFlag = 0;//ײ����������ı�־
		
		//�����Χ��ǽ
		vector<vector<int>> HardWall = bigMap.getWall()[0];
		vector<vector<int>> SoftWall = bigMap.getWall()[1];
		for (unsigned int j = 0; j < HardWall[0].size(); j++) {
			double distance = pow(headX - HardWall[0][j], 2) + pow(headY - HardWall[1][j], 2);
			//��ǽ�ľ���С��һ����Χʱ
			if (distance < pow(10 + 20 + searchRange, 2)) {
				if (distance < pow(10 + 20, 2)) {
					bigMap.DeleteHardWall(j);
					AI[i].setLength(AI[i].getLength() / 2);
					if (AI[i].getLength() < 1) {
						AI[i].InitSnake(bigMap.getWall());
						crashFlag = 1;
						break;
					}
				}//�Ѿ�ײ������³���
				else {
					BadDistance.push_back(distance);
					BadGoal[0].push_back(HardWall[0][j]);
					BadGoal[1].push_back(HardWall[1][j]);
				}//û��ײ������벻��λ��
			}
		}
		if (crashFlag) continue;
		for (unsigned int j = 0; j < SoftWall[0].size(); j++) {
			double distance = pow(headX - SoftWall[0][j], 2) + pow(headY - SoftWall[1][j], 2);
			if (distance < pow(10 + 10 + searchRange, 2)) {
				if (distance < pow(10 + 10, 2)) {
					bigMap.DeleteSoftWall(j);
					AI[i].setLength(AI[i].getLength() - 1);
					if (AI[i].getLength() < 1) {
						AI[i].InitSnake(bigMap.getWall());
						crashFlag = 1;
						break;
					}
				}
				else {
					BadDistance.push_back(distance);
					BadGoal[0].push_back(SoftWall[0][j]);
					BadGoal[1].push_back(SoftWall[1][j]);
				}
			}
		}
		if (crashFlag) continue;

		//�����Χ��������
		searchRange = 100;
		vector<vector<int>> pathp = p1.getPath();
		for (unsigned int k = 0; k < pathp[0].size(); k++) {
			double distance = pow(headX - pathp[0][k], 2) + pow(headY - pathp[1][k], 2);
			if (distance < pow(20 + searchRange, 2)) {
				if (distance <= 400) {
					bigMap.AddBigFood(path[0], path[1], AI[i].getColor());
					AI[i].InitSnake(bigMap.getWall());
					crashFlag = 1;
					break;
				}
				else {
					BadDistance.push_back(distance);
					BadGoal[0].push_back(pathp[0][k]);
					BadGoal[1].push_back(pathp[1][k]);
				}
			}
		}
		if (crashFlag) continue;
		pathp = p2.getPath();
		for (unsigned int k = 0; k < pathp[0].size(); k++) {
			double distance = pow(headX - pathp[0][k], 2) + pow(headY - pathp[1][k], 2);
			if (distance < pow(20 + searchRange, 2)) {			
				if (distance <= 400) {
					bigMap.AddBigFood(path[0], path[1], AI[i].getColor());
					AI[i].InitSnake(bigMap.getWall());
					crashFlag = 1;
					break;
				}
				else {
					BadGoal[0].push_back(pathp[0][k]);
					BadGoal[1].push_back(pathp[1][k]);
					BadDistance.push_back(distance);
				}
			}
		}
		if (crashFlag) continue;
		for (int j = 0; j < 10; j++) {
			if (i == j) continue;
			vector<vector<int>> path2 = AI[j].getPath();
			for (unsigned int k = 0; k < path2[0].size(); k++) {
				double distance = pow(headX - path2[0][k], 2) + pow(headY - path2[1][k], 2);
				if (distance < pow(20 + searchRange, 2)) {	
					if (distance <= 400) {
						bigMap.AddBigFood(path[0], path[1], AI[i].getColor());
						AI[i].InitSnake(bigMap.getWall());
						crashFlag = 1;
						break;
					}
					else {
						BadDistance.push_back(distance);
						BadGoal[0].push_back(path2[0][k]);
						BadGoal[1].push_back(path2[1][k]);
					}
				}
			}
			if (crashFlag) break;
		}
		if (crashFlag) continue;

		//�����Χ��ʳ��
		vector<vector<int>> NormalFood = bigMap.getNormalFood();
		vector<vector<int>> BigFood = bigMap.getBigFood();
		for (unsigned int j = 0; j < BigFood[0].size(); j++) {
			double distance = pow(headX - BigFood[0][j], 2) + pow(headY - BigFood[1][j], 2);
			if (distance < pow(10 + 6 + 300, 2)) {
				if (distance <= pow(10 + 6, 2)) {
					bigMap.DeleteBigFood(j);
					AI[i].setLength(AI[i].getLength() + 1);
					break;
				}
				else {
					unsigned int l = 0;
					for (l = 0; l < GoodDistance.size(); l++) {
						if (GoodDistance[l] > distance) {
							break;
						}
					}
					GoodDistance.insert(GoodDistance.begin() + l, distance);
					GoodGoal[0].insert(GoodGoal[0].begin() + l, BigFood[0][j]);
					GoodGoal[1].insert(GoodGoal[1].begin() + l, BigFood[1][j]);
				}
			}
		}
		for (unsigned int j = 0; j < NormalFood[0].size(); j++) {
			double distance = pow(headX - NormalFood[0][j], 2) + pow(headY - NormalFood[1][j], 2);
			if (distance < pow(10 + 3 + 300, 2)) {
				if (distance <= pow(10 + 3, 2)) {
					bigMap.DeleteNorFood(j);
					AI[i].setLength(AI[i].getLength() + 0.2);
					break;
				}
				else {
					unsigned int l = 0;
					for (l = 0; l < GoodDistance.size(); l++) {
						if (GoodDistance[l] > distance) {
							break;
						}
					}
					GoodDistance.insert(GoodDistance.begin() + l, distance);
					GoodGoal[0].insert(GoodGoal[0].begin() + l, NormalFood[0][j]);
					GoodGoal[1].insert(GoodGoal[1].begin() + l, NormalFood[1][j]);
				}
			}
		}
		
		//ѡ��Ƕ�
		int max = 0;
		int maxAngle = 0;
		if (GoodGoal[0].size() > 0) {
			for (unsigned int j = 0; j < GoodGoal[0].size(); j++) {
				double degree = atan2(GoodGoal[1][j] - headY, GoodGoal[0][j] - headX);
				int nextx = headX + int(20 * cos(degree));
				int nexty = headY + int(20 * sin(degree));
				unsigned int k;
				for (k = 0; k < BadGoal[0].size(); k++) {
					if (nextx - 30 < 0 || nexty - 30 < 0 || nextx + 30 > MAP_W || nexty + 30 > MAP_H) {
						break;
					}
					if (pow(nextx - BadGoal[0][k], 2) + pow(nexty - BadGoal[1][k], 2) < 900) {
						break;
					}
					if (pow(GoodGoal[0][j] - BadGoal[0][k], 2) + pow(GoodGoal[1][j] - BadGoal[1][k], 2) < 650) {
						break;
					}
				}
				if (k == BadGoal[0].size()) {
					AI[i].ChangeOrientation(int(180 * degree / PI));
					break;
				}
				if (BadDistance.size() > 0) {
					int min = 0;
					for (unsigned int k = 0; k < BadGoal[0].size(); k++) {
						if (BadDistance[k] < BadDistance[min]) {
							min = k;
						}
					}
					double degree = atan2(BadGoal[1][min] - headY, BadGoal[0][min] - headX);
					AI[i].ChangeOrientation((int(180 * degree / PI) + 180) % 360);
				}			
			}

		}

	}
}

/****************************************
* ������Ϸ������
*****************************************/
class SingleGame {
private:
	int window_w, window_h;
	//��ť��С��λ��
	int button_w = 300;
	int button_h = 60;
	int button_x = 900;
	int button_y[10] = { 300,370,450,520,590,640 };
	RECT b0 = { button_x,button_y[0],button_x + button_w,button_y[0] + button_h };
	RECT b1 = { button_x,button_y[1],button_x + button_w,button_y[1] + button_h };
	RECT b2 = { button_x,button_y[2],button_x + button_w,button_y[2] + button_h };
	RECT b3 = { button_x,button_y[3],button_x + button_w,button_y[3] + button_h };
	RECT b4 = { button_x,button_y[4],button_x + button_w,button_y[4] + button_h };
	RECT b5 = { button_x,button_y[5],button_x + button_w,button_y[5] + button_h };
	RECT b6 = { button_x,button_y[6],button_x + button_w,button_y[6] + button_h };
	RECT b7 = { 300,240,600,340 };
	RECT b8 = { 300,360,600,460 };
	RECT b9 = { 300,480,600,580 };
	RECT Button[10] = { b0,b1,b2,b3,b4,b5,b6,b7,b8,b9 };

	struct GameInfoA {
		double length;
		int hp;
		int score;
		int power;
		int highestScore;
		clock_t startTime;
	} gameInfo;
	IMAGE MAP;
	int readSave;

public:
	//���캯��
	SingleGame(int r) {
		readSave = r;
		//���õ�ͼ��С�ʹ��ڴ�С
		Resize(&MAP, MAP_W, MAP_H);
		window_w = 1200;
		window_h = 700;
		gameInfo.hp = 5;
		gameInfo.score = 0;
	}
	//����UI��Ϣ
	void DrawUI() {
		setbkmode(TRANSPARENT);
		setlinestyle(PS_SOLID, 3);
		setlinecolor(WHITE);
		settextstyle(36, 0, _T("����"), 0, 0, 1000, false, false, false);
		TCHAR s[50];
		settextcolor(WHITE);
		_stprintf_s(s, _T("��Ϸʱ�� %d s"), (clock() - gameInfo.startTime) / 1000);
		drawtext(s, &Button[0], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("��ǰ���� %d"), (int)gameInfo.length);
		drawtext(s, &Button[1], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("ʣ������ %d"), gameInfo.hp);
		drawtext(s, &Button[2], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("��ǰ�÷� %d"), gameInfo.score);
		drawtext(s, &Button[3], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("��ߵ÷� %d"), gameInfo.highestScore);
		if (gameInfo.score > gameInfo.highestScore) {
			_stprintf_s(s, _T("��ߵ÷� %d"), gameInfo.score);
		}
		drawtext(s, &Button[4], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("��ESC��������Ϸ"));
		drawtext(s, &Button[5], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		//������
		setfillcolor(GREEN);
		solidroundrect(870, int(300 - 250 * (double)gameInfo.power / 100.0), 890, 300, 30, 30);
		setfillcolor(WHITE);
		setlinecolor(YELLOW);
		setlinestyle(PS_SOLID, 5);
		roundrect(870, 50, 890, 300, 30, 30);
		if (gameInfo.power < 10) {
			RECT b = { 300,50,600,100 };
			settextcolor(0X00FF00);
			settextstyle(28, 0, _T("����"), 0, 0, 1000, false, false, false);
			drawtext(_T("���ܲ��㣡��ʱ���䣡"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	/****************************************
	����ֵ���÷�
	���ܣ���Ϸ���������
	*****************************************/
	int GameRun() {
		//���ļ��ж�ȡ��߷ּ�¼
		int HighScore[5];
		ifstream fhscore("resource/HighestScore.log");
		if (!fhscore) {
			for (int i = 0; i < 5; i++) {
				HighScore[i] = 0;
			}
		}//���ļ�ʧ�ܵ����
		else {
			for (int i = 0; i < 5; i++) {
				fhscore >> HighScore[i];
			}
			fhscore.close();
		}
		gameInfo.highestScore = HighScore[3];

		MOUSEMSG m{ 0 };
		MapA bigMap; //��ͼ����ʵ����
		SnakeA p1(bigMap.getWall());//����߶���ʵ����
		SnakeA AI[10];//AI��������
		for (int i = 0; i < 10; i++) {
			AI[i].InitSnake(bigMap.getWall());
		}
		gameInfo.length = p1.getLength();
		gameInfo.power = 100;
		if (readSave == 1) {
			ifstream Info("resource/SingleSave.log");
			if (!Info) {
				cerr << "���ļ�ʧ�ܣ�" << endl;
				exit(1);
			}
			vector<vector<int>> HardWall;
			vector<vector<int>> SoftWall;
			HardWall.resize(2); SoftWall.resize(2);
			unsigned int n;
			Info.read((char*)&n, sizeof(n));
			int x, y;
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				HardWall[0].push_back(x); HardWall[1].push_back(y);
			}
			Info.read((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				SoftWall[0].push_back(x); SoftWall[1].push_back(y);
			}
			//д��ʳ��
			vector<vector<int>> GoodFood;
			GoodFood.resize(2);
			Info.read((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				GoodFood[0].push_back(x); GoodFood[1].push_back(y);
			}
			vector<vector<int>> BadFood;
			BadFood.resize(2);
			Info.read((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				BadFood[0].push_back(x); BadFood[1].push_back(y);
			}
		    //��ȡ�����Ϣ
			Info.read((char*)&gameInfo, sizeof(gameInfo));
			gameInfo.length = (int)gameInfo.length;
			vector<vector<int>> path;
			path.resize(2);
			Info.read((char*)&n, sizeof(n));
			for (unsigned int j = 0; j < n; j++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				path[0].push_back(x); path[1].push_back(y);
			}
			Info.close();
			bigMap.SetMap(HardWall, SoftWall, GoodFood, BadFood);
			p1.Set(path);
		}
		//��ʼ������
		initgraph(window_w, window_h);
		loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
		BeginBatchDraw();
		//���û�ͼ�豸ΪMAP
		SetWorkingImage(&MAP);
		cleardevice();
		bigMap.DrawMap();
		p1.DrawSnake();
		SetWorkingImage();
		setaspectratio(float(300.0 / MAP_W), float(300.0 / MAP_H));
		putimage(int(900 * MAP_W / 300.0), int(50 * MAP_H / 300.0), &MAP);//����С��ͼ
		setaspectratio(1, 1);
		putimage(50, 50, 800, 600, &MAP, p1.getPath()[0][0] - 400, p1.getPath()[1][0] - 300);
		DrawUI();
		FlushBatchDraw();
		//ReadyGO��ʾ
		for (int i = 0; i < 256; i++) {
			settextcolor(RGB(i, i, i));
			settextstyle(100, 0, _T("����"));
			drawtext(_T("READY"), &Button[7], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(_T("GO!"), &Button[8], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			Sleep(2);
		}

		bool isPlaying = true;
		gameInfo.startTime = clock();//��¼��ʼʱ��
		while (isPlaying) {
			while (!_kbhit()) {
				for (int i = 0; i < 4; i++) {
					//�����Ƿ��ڼ����������ٶ�
					p1.setSpeed();
					for (int i = 0; i < 10; i++) {
						AI[i].setSpeed();
					}
					//�ƶ�
					p1.Move();
					for (int i = 0; i < 10; i++) {
						AI[i].Move();
					}
					//���AI�Ƿ�ײ���߽�
					for (int i = 0; i < 10; i++) {
						if (AI[i].CheckBorder()) {
							bigMap.AddSoftWall(AI[i].getPath()[0], AI[i].getPath()[1]);
							AI[i].InitSnake(bigMap.getWall());
						}
					}

					//�����ҵ���ײ
					int index;
					//ײ���߽�
					if (p1.CheckBorder()) {
						bigMap.AddSoftWall(p1.getPath()[0], p1.getPath()[1]);
						gameInfo.hp--;
						gameInfo.score -= 2;
						p1.InitSnake(bigMap.getWall());
						gameInfo.power = 100;
					}
					//ײ����ǽ
					else if ((index = p1.CheckSoftWall(bigMap.getWall()[1])) != -1) {
						gameInfo.score -= 3;
						gameInfo.power -= 5;
						bigMap.DeleteSoftWall(index);
					}
					//ײ��Ӳǽ
					else if ((index = p1.CheckHardWall(bigMap.getWall()[0])) != -1) {
						gameInfo.score -= 5;
						gameInfo.power -= 10;
						bigMap.DeleteHardWall(index);
					}
					//�Ե����
					else if ((index = p1.CheckBadFood(bigMap.getBadFood())) != -1) {
						gameInfo.score -= 5;
						gameInfo.hp--;
						p1.InitSnake(bigMap.getWall());
						gameInfo.power = 100;
						bigMap.DeleteBadFood(index);
					}
					//�Ե������
					else if ((index = p1.CheckGoodFood(bigMap.getGoodFood())) != -1) {
						gameInfo.score += 10;
						gameInfo.power = 100;
						bigMap.DeleteGoodFood(index);
					}
					//�Ե���ʳ��
					else if ((index = p1.CheckBigFood(bigMap.getBigFood())) != -1) {
						gameInfo.score += 5;
						gameInfo.power -= 2;
						bigMap.DeleteBigFood(index);
					}
					//�Ե���ͨʳ��
					else if ((index = p1.CheckNormalFood(bigMap.getNormalFood())) != -1) {
						gameInfo.score += 1;
						gameInfo.power -= 1;
						bigMap.DeleteNorFood(index);
					}
					//ײ��������
					for (int i = 0; i < 10; i++) {
						if (p1.CheckCrashOther(AI[i].getPath())) {
							bigMap.AddBigFood(p1.getPath()[0], p1.getPath()[1], p1.getColor());
							gameInfo.hp--;
							gameInfo.score -= 5;
							p1.InitSnake(bigMap.getWall());
							gameInfo.power = 100;
							bigMap.InitFood();
							break;
						}
					}
					//����
					cleardevice();
					loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
					SetWorkingImage(&MAP);
					cleardevice();
					bigMap.DrawMap();
					for (int i = 0; i < 10; i++) {
						AI[i].DrawSnake();
					}
					p1.DrawSnake();
					SetWorkingImage();
					setaspectratio(float(250.0 / MAP_W), float(250.0 / MAP_H));
					putimage(int(900 * MAP_W / 250.0), int(50 * MAP_H / 250.0), &MAP);//����С��ͼ
					setaspectratio(1, 1);
					putimage(50, 50, 800, 600, &MAP, p1.getPath()[0][0] - 400, p1.getPath()[1][0] - 300);
					gameInfo.length = p1.getLength();//���³���
					DrawUI();
					FlushBatchDraw();

					//����С��1������С��0������
					if (gameInfo.power <= 0 || gameInfo.length < 1) {
						gameInfo.hp--;
						gameInfo.power = 100;
						p1.InitSnake(bigMap.getWall());
					}
					if (gameInfo.hp <= 0) {
						isPlaying = false;
						break;
					}
				}
				//��ʱɾ��Ӳǽ����ǽ���µ�ͼ
				bigMap.DeleteSoftWall(0);
				if (!isPlaying) {
					break;
				}
				AIControl(bigMap, AI, p1, p1);//�޸�AI�ƶ�����
			}
			if (!isPlaying) {
				break;
			}
			//��ⰴ�����޸ķ���
			char key1 = _getch();
			if (key1 == 27) {
				isPlaying = false;
				break;
			}
			char key2;
			int keyNum = 1;
			if (key1 != 'w' && key1 != 'W' && key1 != 'a' && key1 != 'A' && key1 != 's'
				&& key1 != 'S' && key1 != 'd' && key1 != 'D' && key1 != -32) {
				continue;
			}
			if (key1 == -32) {
				key1 = _getch();
			}
			//Sleep(10);
			if (_kbhit()) {
				key2 = _getch();
				keyNum++;
				if (key2 == -32) {
					key2 = _getch();
				}
			}
			int angleNow = p1.getAngle();
			int angleNext;
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
			p1.ChangeOrientation(angleNext);
		}

		//������ʷ��߷ֲ�д���ļ�
		if (gameInfo.score > HighScore[3]) {
			HighScore[3] = gameInfo.score;
		}
		ofstream fhscore_out("resource/HighestScore.log");
		if (!fhscore_out) {
			cerr << "�����߷ִ���!" << endl;
		}
		else {
			for (int i = 0; i < 5; i++) {
				fhscore_out << HighScore[i];
				fhscore_out.put('\n');
			}
			fhscore_out.close();
		}
		//GameOver��ʾ
		settextcolor(0X71B33C);
		settextstyle(30, 0, _T("����"));
		TCHAR s[50];
		_stprintf_s(s, _T("Game Over\n\n���յ÷�: %d"), gameInfo.score);
		drawtext(s, &Button[7], DT_CENTER);
		drawtext(_T("���������Ϸ��ҳ"), &Button[8], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("������Ϸ�浵������"), &Button[9], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setlinecolor(0X00FF00);
		setlinestyle(PS_SOLID, 5);
		rectangle(Button[8].left, Button[8].top, Button[8].right, Button[8].bottom);
		rectangle(Button[9].left, Button[9].top, Button[9].right, Button[9].bottom);
		FlushBatchDraw();
		bool quit = false;
		bool isSave = false;
		while (!quit) {
			if (MouseHit()) m = GetMouseMsg();
			int choice = 0;
			switch (m.uMsg) {
			case WM_LBUTTONDOWN:
				if (m.x > Button[8].left&& m.x<Button[8].right && m.y>Button[8].top&& m.y < Button[8].bottom) {

					quit = true;
				}
				else if (m.x > Button[9].left&& m.x<Button[9].right && m.y>Button[9].top&& m.y < Button[9].bottom) {
					isSave = true;
					quit = true;
				}
				break;
			case WM_MOUSEMOVE:
				if (m.x > Button[8].left&& m.x<Button[8].right && m.y>Button[8].top&& m.y < Button[8].bottom) {
					choice = 1;
				}
				else if (m.x > Button[9].left&& m.x<Button[9].right && m.y>Button[9].top&& m.y < Button[9].bottom) {
					choice = 2;
				}
			}
			setlinestyle(PS_SOLID, 5);
			setlinecolor(0X00FF00);
			rectangle(Button[8].left, Button[8].top, Button[8].right, Button[8].bottom);
			rectangle(Button[9].left, Button[9].top, Button[9].right, Button[9].bottom);
			if (choice != 0) {
				setlinecolor(RED);
				rectangle(Button[choice + 7].left, Button[choice + 7].top, Button[choice + 7].right, Button[choice + 7].bottom);
			}
			FlushBatchDraw();
		}
		//�浵
		if (isSave) {
			ofstream Info("resource/SingleSave.log");
			if (!Info) {
				cerr << "���ļ�ʧ�ܣ�" << endl;
				exit(1);
			}
			//д��ǽ
			vector<vector<int>> HardWall(bigMap.getWall()[0]);
			vector<vector<int>> SoftWall(bigMap.getWall()[1]);
			unsigned int n = HardWall[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < HardWall[0].size(); i++) {
				Info.write((char*)&HardWall[0][i], sizeof(HardWall[0][i]));
				Info.write((char*)&HardWall[1][i], sizeof(HardWall[1][i]));
			}
			n = SoftWall[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < SoftWall[0].size(); i++) {
				Info.write((char*)&SoftWall[0][i], sizeof(SoftWall[0][i]));
				Info.write((char*)&SoftWall[1][i], sizeof(SoftWall[1][i]));
			}
			//д��ʳ��
			vector<vector<int>> GoodFood = bigMap.getGoodFood();
			n = GoodFood[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < GoodFood[0].size(); i++) {
				Info.write((char*)&GoodFood[0][i], sizeof(GoodFood[0][i]));
				Info.write((char*)&GoodFood[1][i], sizeof(GoodFood[1][i]));
			}
			vector<vector<int>> BadFood = bigMap.getBadFood();
			n = BadFood[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < BadFood[0].size(); i++) {
				Info.write((char*)&BadFood[0][i], sizeof(BadFood[0][i]));
				Info.write((char*)&BadFood[1][i], sizeof(BadFood[1][i]));
			}
			//д�������Ϣ
			Info.write((char*)&gameInfo, sizeof(gameInfo));
			vector<vector<int>> path = p1.getPath();
			unsigned int num = path[0].size();
			Info.write((char*)&num, sizeof(num));
			for (unsigned int j = 0; j < num; j++) {
				Info.write((char*)&path[0][j], sizeof(path[0][j]));
				Info.write((char*)&path[1][j], sizeof(path[1][j]));
			}
			Info.close();
		}
		EndBatchDraw();
		return gameInfo.score;//��Ϸ�������ص÷�
	}
};

/****************************************
* ˫����Ϸ������
*****************************************/
class DoubleGame {
private:
	int window_w, window_h;
	RECT b0 = { 10,500,250,600 };
	RECT b1 = { 260,500,500,600 };
	RECT b2 = { 10,600,250,700 };
	RECT b3 = { 260,600,500,700 };
	RECT b4 = { 700,500,940,600 };
	RECT b5 = { 950,500,1190,600 };
	RECT b6 = { 700,600,940,700 };
	RECT b7 = { 950,600,1190,700 };
	RECT b8 = { 510,420,690,450 };
	RECT b9 = { 510,450,690,500 };
	RECT b10 = { 510,500,690,550 };
	RECT b11 = { 450,300,750,400 };
	RECT b12 = { 450,420,750,520 };
	RECT b13 = { 450,540,750,640 };
	//RECT b7 = { 300,240,600,340 };
	//RECT b8 = { 300,360,600,460 };
	//RECT b9 = { 300,480,600,580 };
	RECT Button[15] = { b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13 };

	struct GameInfoA {
		double length;
		int hp;
		int score;
		int power;
		int highestScore;
		clock_t startTime;
	} gameInfo[2];
	IMAGE MAP;
	int readSave;
public:
	//���캯��
	DoubleGame(int r) {
		//���õ�ͼ��С�ʹ��ڴ�С
		Resize(&MAP, MAP_W, MAP_H);
		window_w = 1200;
		window_h = 700;
		gameInfo[0].hp = 5;
		gameInfo[0].score = 0;
		gameInfo[1].hp = 5;
		gameInfo[1].score = 0;
		readSave = r;
	}
	//����UI��Ϣ
	void DrawUI() {
		setbkmode(TRANSPARENT);
		setlinestyle(PS_SOLID, 3);
		setlinecolor(WHITE);
		settextstyle(36, 0, _T("����"), 0, 0, 1000, false, false, false);
		TCHAR s[50];
		settextcolor(WHITE);
		drawtext(_T("Player 1"), &Button[0], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("Player 2"), &Button[5], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("���� %d"), (int)gameInfo[0].length);
		drawtext(s, &Button[1], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("���� %d"), gameInfo[0].hp);
		drawtext(s, &Button[3], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("�÷� %d"), gameInfo[0].score);
		drawtext(s, &Button[2], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("���� %d"), (int)gameInfo[1].length);
		drawtext(s, &Button[4], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("���� %d"), gameInfo[1].hp);
		drawtext(s, &Button[6], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("�÷� %d"), gameInfo[1].score);
		drawtext(s, &Button[7], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		settextstyle(26, 0, _T("����"), 0, 0, 1000, false, false, false);
		_stprintf_s(s, _T("ʱ  �� %ds"), (clock() - gameInfo[0].startTime) / 1000);
		drawtext(s, &Button[8], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("��߷� %d"), gameInfo[0].highestScore);
		if (gameInfo[0].score > gameInfo[0].highestScore) {
			_stprintf_s(s, _T("��ߵ÷� %d"), gameInfo[0].score);
		}
		else if (gameInfo[1].score > gameInfo[0].highestScore) {
			_stprintf_s(s, _T("��ߵ÷� %d"), gameInfo[1].score);
		}
		drawtext(s, &Button[9], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("ESC��������Ϸ"));
		drawtext(s, &Button[10], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		////������
		setfillcolor(GREEN);
		solidroundrect(520, int(400 - 200 * (double)gameInfo[0].power / 100.0), 580, 400, 30, 30);
		solidroundrect(620, int(400 - 200 * (double)gameInfo[1].power / 100.0), 680, 400, 30, 30);
		setfillcolor(WHITE);
		setlinecolor(YELLOW);
		setlinestyle(PS_SOLID, 5);
		roundrect(520, 200, 580, 400, 30, 30);
		roundrect(620, 200, 680, 400, 30, 30);
		if (gameInfo[0].power < 10) {
			RECT b = { 105,10,405,60 };
			settextcolor(0X00FF00);
			settextstyle(28, 0, _T("����"), 0, 0, 1000, false, false, false);
			drawtext(_T("���ܲ��㣡��ʱ���䣡"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		if (gameInfo[1].power < 10) {
			RECT b = { 795,10,1095,60 };
			settextcolor(0X00FF00);
			settextstyle(28, 0, _T("����"), 0, 0, 1000, false, false, false);
			drawtext(_T("���ܲ��㣡��ʱ���䣡"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	/****************************************
	����ֵ���÷�
	���ܣ���Ϸ���������
	*****************************************/
	int GameRun() {
		//���ļ��ж�ȡ��߷ּ�¼
		int HighScore[5];
		ifstream fhscore("resource/HighestScore.log");
		if (!fhscore) {
			for (int i = 0; i < 5; i++) {
				HighScore[i] = 0;
			}
		}//���ļ�ʧ�ܵ����
		else {
			for (int i = 0; i < 5; i++) {
				fhscore >> HighScore[i];
			}
			fhscore.close();
		}
		gameInfo[0].highestScore = HighScore[3];

		MOUSEMSG m{ 0 };
		MapA bigMap; //��ͼ����ʵ����
		SnakeA p1(bigMap.getWall());//����߶���ʵ����
		SnakeA p2(bigMap.getWall());
		SnakeA AI[10];//AI��������
		for (int i = 0; i < 10; i++) {
			AI[i].InitSnake(bigMap.getWall());
		}
		gameInfo[0].length = p1.getLength();
		gameInfo[0].power = 100;
		gameInfo[1].length = p2.getLength();
		gameInfo[1].power = 100;

		if (readSave == 1) {
			ifstream Info("resource/DoubleSave.log");
			if (!Info) {
				cerr << "���ļ�ʧ�ܣ�" << endl;
				exit(1);
			}
			vector<vector<int>> HardWall;
			vector<vector<int>> SoftWall;
			HardWall.resize(2); SoftWall.resize(2);
			unsigned int n;
			Info.read((char*)&n, sizeof(n));
			int x, y;
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				HardWall[0].push_back(x); HardWall[1].push_back(y);
			}
			Info.read((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				SoftWall[0].push_back(x); SoftWall[1].push_back(y);
			}
			//д��ʳ��
			vector<vector<int>> GoodFood;
			GoodFood.resize(2);
			Info.read((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				GoodFood[0].push_back(x); GoodFood[1].push_back(y);
			}
			vector<vector<int>> BadFood;
			BadFood.resize(2);
			Info.read((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < n; i++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				BadFood[0].push_back(x); BadFood[1].push_back(y);
			}
			bigMap.SetMap(HardWall, SoftWall, GoodFood, BadFood);
			//��ȡ�����Ϣ
			Info.read((char*)&gameInfo, sizeof(gameInfo));
			gameInfo[0].length = (int)gameInfo[0].length;
			gameInfo[1].length = (int)gameInfo[1].length;
			vector<vector<int>> path;
			path.resize(2);
			Info.read((char*)&n, sizeof(n));
			for (unsigned int j = 0; j < n; j++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				path[0].push_back(x); path[1].push_back(y);
			}
			p1.Set(path);
			path[0].clear(); path[1].clear();
			Info.read((char*)&n, sizeof(n));
			for (unsigned int j = 0; j < n; j++) {
				Info.read((char*)&x, sizeof(int));
				Info.read((char*)&y, sizeof(int));
				path[0].push_back(x); path[1].push_back(y);
			}
			Info.close();
			p2.Set(path);
		}
		//��ʼ������
		initgraph(window_w, window_h);
		loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
		BeginBatchDraw();
		//���û�ͼ�豸ΪMAP
		SetWorkingImage(&MAP);
		cleardevice();
		bigMap.DrawMap();
		p1.DrawSnake();
		p2.DrawSnake();
		SetWorkingImage();
		setaspectratio(float(180.0/MAP_W), float(180.0/MAP_H));
		putimage(int(510 * MAP_W / 180.0), int(10 * MAP_H / 180.0), &MAP);//����С��ͼ
		setaspectratio(1, 1);
		putimage(10, 10, 490, 490, &MAP, p1.getPath()[0][0] - 245, p1.getPath()[1][0] - 240);	
		putimage(700, 10, 490, 490, &MAP, p2.getPath()[0][0] - 245, p2.getPath()[1][0] - 240);
		DrawUI();
		FlushBatchDraw();
		//ReadyGO��ʾ
		for (int i = 0; i < 256; i++) {
			settextcolor(RGB(i, i, i));
			settextstyle(100, 0, _T("����"));
			drawtext(_T("READY"), &Button[11], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(_T("GO!"), &Button[12], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			Sleep(2);
		}

		bool isPlaying = true;
		gameInfo[0].startTime = clock();//��¼��ʼʱ��
		while (isPlaying) {
			while (!_kbhit()) {
				for (int i = 0; i < 4; i++) {
					//�����Ƿ��ڼ����������ٶ�
					p1.setSpeed();p2.setSpeed();
					for (int i = 0; i < 10; i++) {
						AI[i].setSpeed();
					}
					//�ƶ�
					p1.Move(); p2.Move();
					for (int i = 0; i < 10; i++) {
						AI[i].Move();
					}
					//���AI�Ƿ�ײ���߽�
					for(int i = 0; i < 10; i++) {
						if (AI[i].CheckBorder()) {
							bigMap.AddSoftWall(AI[i].getPath()[0], AI[i].getPath()[1]);
							AI[i].InitSnake(bigMap.getWall());
						}
					}

					//�����ҵ���ײ
					int index;
					//ײ���߽�
					if (p1.CheckBorder()) {
						bigMap.AddSoftWall(p1.getPath()[0], p1.getPath()[1]);
						gameInfo[0].hp--;
						gameInfo[0].score -= 2;
						p1.InitSnake(bigMap.getWall());
						gameInfo[0].power = 100;
					}
					//ײ����ǽ
					else if ((index=p1.CheckSoftWall(bigMap.getWall()[1]))!=-1) {
						gameInfo[0].score -= 3;
						gameInfo[0].power -= 5;
						bigMap.DeleteSoftWall(index);
					}
					//ײ��Ӳǽ
					else if ((index = p1.CheckHardWall(bigMap.getWall()[0])) != -1) {
						gameInfo[0].score -= 5;
						gameInfo[0].power -= 10;
						bigMap.DeleteHardWall(index);
					}
					//�Ե����
					else if ((index = p1.CheckBadFood(bigMap.getBadFood())) != -1) {
						gameInfo[0].score -= 5;
						gameInfo[0].hp--;
						p1.InitSnake(bigMap.getWall());
						gameInfo[0].power = 100;
						bigMap.DeleteBadFood(index);
					}
					//�Ե������
					else if ((index = p1.CheckGoodFood(bigMap.getGoodFood())) != -1) {
						gameInfo[0].score += 10;
						gameInfo[0].power = 100;
						bigMap.DeleteGoodFood(index);
					}
					//�Ե���ʳ��
					else if ((index = p1.CheckBigFood(bigMap.getBigFood())) != -1) {
						gameInfo[0].score += 5;
						gameInfo[0].power -= 2;
						bigMap.DeleteBigFood(index);
					}
					//�Ե���ͨʳ��
					else if ((index = p1.CheckNormalFood(bigMap.getNormalFood())) != -1) {
						gameInfo[0].score += 1;
						gameInfo[0].power -= 1;
						bigMap.DeleteNorFood(index);
					}
					//ײ��������
					if (p1.CheckCrashOther(p2.getPath())) {
						bigMap.AddBigFood(p1.getPath()[0], p1.getPath()[1], p1.getColor());
						gameInfo[0].hp--;
						gameInfo[0].score -= 5;
						p1.InitSnake(bigMap.getWall());
						gameInfo[0].power = 100;
					}
					else {
						for (int i = 0; i < 10; i++) {
							if (p1.CheckCrashOther(AI[i].getPath())) {
								bigMap.AddBigFood(p1.getPath()[0], p1.getPath()[1], p1.getColor());
								gameInfo[0].hp--;
								gameInfo[0].score -= 5;
								p1.InitSnake(bigMap.getWall());
								gameInfo[0].power = 100;
								break;
							}
						}
					}

					//ײ���߽�
					if (p2.CheckBorder()) {
						bigMap.AddSoftWall(p2.getPath()[0], p2.getPath()[1]);
						gameInfo[1].hp--;
						gameInfo[1].score -= 2;
						p2.InitSnake(bigMap.getWall());
						gameInfo[1].power = 100;
					}
					//ײ����ǽ
					else if ((index = p2.CheckSoftWall(bigMap.getWall()[1])) != -1) {
						gameInfo[1].score -= 3;
						gameInfo[1].power -= 5;
						bigMap.DeleteSoftWall(index);
					}
					//ײ��Ӳǽ
					else if ((index = p2.CheckHardWall(bigMap.getWall()[0])) != -1) {
						gameInfo[1].score -= 5;
						gameInfo[1].power -= 10;
						bigMap.DeleteHardWall(index);
					}
					//�Ե����
					else if ((index = p2.CheckBadFood(bigMap.getBadFood())) != -1) {
						gameInfo[1].score -= 5;
						gameInfo[1].hp--;
						p2.InitSnake(bigMap.getWall());
						gameInfo[1].power = 100;
						bigMap.DeleteBadFood(index);
					}
					//�Ե������
					else if ((index = p2.CheckGoodFood(bigMap.getGoodFood())) != -1) {
						gameInfo[1].score += 10;
						gameInfo[1].power = 100;
						bigMap.DeleteGoodFood(index);
					}
					//�Ե���ʳ��
					else if ((index = p2.CheckBigFood(bigMap.getBigFood())) != -1) {
						gameInfo[1].score += 5;
						gameInfo[1].power -= 2;
						bigMap.DeleteBigFood(index);
					}
					//�Ե���ͨʳ��
					else if ((index = p2.CheckNormalFood(bigMap.getNormalFood())) != -1) {
						gameInfo[1].score += 1;
						gameInfo[1].power -= 1;
						bigMap.DeleteNorFood(index);
					}
					//ײ��������
					if (p2.CheckCrashOther(p1.getPath())) {
						bigMap.AddBigFood(p2.getPath()[0], p2.getPath()[1], p2.getColor());
						gameInfo[1].hp--;
						gameInfo[1].score -= 5;
						p2.InitSnake(bigMap.getWall());
						gameInfo[1].power = 100;
					}
					else {
						for (int i = 0; i < 10; i++) {
							if (p2.CheckCrashOther(AI[i].getPath())) {
								bigMap.AddBigFood(p2.getPath()[0], p2.getPath()[1], p2.getColor());
								gameInfo[1].hp--;
								gameInfo[1].score -= 5;
								p2.InitSnake(bigMap.getWall());
								gameInfo[1].power = 100;
								break;
							}
						}
					}

					//����
					cleardevice();
					loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
					SetWorkingImage(&MAP);
					cleardevice();
					bigMap.DrawMap();
					for (int i = 0; i < 10; i++) {
						AI[i].DrawSnake();
					}
					p1.DrawSnake();
					p2.DrawSnake();
					SetWorkingImage();
					setaspectratio(float(180.0 / MAP_W), float(180.0 / MAP_H));
					putimage(int(510 * MAP_W / 180.0), int(10 * MAP_H / 180.0), &MAP);//����С��ͼ
					setaspectratio(1, 1);
					putimage(10, 10, 490, 490, &MAP, p1.getPath()[0][0] - 245, p1.getPath()[1][0] - 240);
					putimage(700, 10, 490, 490, &MAP, p2.getPath()[0][0] - 245, p2.getPath()[1][0] - 240);
					gameInfo[0].length = p1.getLength();//���³���
					gameInfo[1].length = p2.getLength();//���³���
					DrawUI();
					FlushBatchDraw();

					//����С��1������С��0������
					if (gameInfo[0].power <= 0 || gameInfo[0].length < 1) {
						gameInfo[0].hp--;
						gameInfo[0].power = 100;
						p1.InitSnake(bigMap.getWall());
					}
					if (gameInfo[1].power <= 0 || gameInfo[1].length < 1) {
						gameInfo[1].hp--;
						gameInfo[1].power = 100;
						p2.InitSnake(bigMap.getWall());
					}
					if (gameInfo[0].hp <= 0 || gameInfo[1].hp <= 0) {
						isPlaying = false;
						break;
					}
				}
				//��ʱɾ��Ӳǽ����ǽ���µ�ͼ
				bigMap.DeleteSoftWall(0);
				if (!isPlaying) {
					break;
				}
				AIControl(bigMap, AI, p1, p2);//�޸�AI�ƶ�����
			}
			if (!isPlaying) {
				break;
			}
			//��ⰴ�����޸ķ���
			int choice = 1;
			char key1 = _getch();
			if (key1 == 27) {
				isPlaying = false;
				break;
			}
			char key2;
			int keyNum = 1;
			if (key1 != 'w' && key1 != 'W' && key1 != 'a' && key1 != 'A' && key1 != 's'
				&& key1 != 'S' && key1 != 'd' && key1 != 'D' && key1 != -32) {
				continue;
			}
			if (key1 == -32) {
				choice = 2;
				key1 = _getch();
			}
			if (_kbhit()) {
				key2 = _getch();
				keyNum++;
				if (key2 == -32) {
					key2 = _getch();
				}
			}	
			if (choice == 1) {
				int angleNow = p1.getAngle();
				int angleNext = p1.getAngle();
				switch (key1) {
				case'w':
				case'W':
					angleNext = -90;
					if (keyNum == 2) {
						if (key2 == 'a' || key2 == 'A') {
							angleNext = -135;
						}
						else if (key2 == 'd' || key2 == 'D') {
							angleNext = -45;
						}
					}
					break;
				case's':
				case'S':
					angleNext = 90;
					if (keyNum == 2) {
						if (key2 == 'a' || key2 == 'A') {
							angleNext = 135;
						}
						else if (key2 == 'd' || key2 == 'D') {
							angleNext = 45;
						}
					}
					break;
				case'a':
				case'A':
					angleNext = 180;
					if (keyNum == 2) {
						if (key2 == 'w' || key2 == 'W') {
							angleNext = -135;
						}
						else if (key2 == 's' || key2 == 'S') {
							angleNext = 135;
						}
					}
					break;
				case'd':
				case'D':
					angleNext = 0;
					if (keyNum == 2) {
						if (key2 == 'w' || key2 == 'W') {
							angleNext = -45;
						}
						else if (key2 == 's' || key2 == 'S') {
							angleNext = 45;
						}
					}
					break;
				}
				if (angleNext == 0 || angleNow == 0 || angleNext / abs(angleNext) != angleNow / abs(angleNow)) {
					if (abs(angleNext) + abs(angleNow) == 180) {
						angleNext = angleNow;
					}
				}
				p1.ChangeOrientation(angleNext);
			}
			else if (choice == 2) {
				int angleNow = p2.getAngle();
				int angleNext = p2.getAngle();
				switch (key1) {
				case 72:
					angleNext = -90;
					if (keyNum == 2) {
						if (key2 == 75) {
							angleNext = -135;
						}
						else if (key2 == 77) {
							angleNext = -45;
						}
					}
					break;
				case 80:
					angleNext = 90;
					if (keyNum == 2) {
						if (key2 == 75) {
							angleNext = 135;
						}
						else if (key2 == 77) {
							angleNext = 45;
						}
					}
					break;
				case 75:
					angleNext = 180;
					if (keyNum == 2) {
						if (key2 == 72) {
							angleNext = -135;
						}
						else if (key2 == 80) {
							angleNext = 135;
						}
					}
					break;
				case 77:
					angleNext = 0;
					if (keyNum == 2) {
						if (key2 == 72) {
							angleNext = -45;
						}
						else if (key2 == 80) {
							angleNext = 45;
						}
					}
					break;
				}
				p2.ChangeOrientation(angleNext);
			}
		}
		//������ʷ��߷ֲ�д���ļ�
		if (gameInfo[0].score > HighScore[4]|| gameInfo[1].score > HighScore[4]) {
			if (gameInfo[0].score > gameInfo[1].score) {
				HighScore[4] = gameInfo[0].score;
			}
			else {
				HighScore[4] = gameInfo[1].score;
			}			
		}
		ofstream fhscore_out("resource/HighestScore.log");
		if (!fhscore_out) {
			cerr << "�����߷ִ���!" << endl;
		}
		else {
			for (int i = 0; i < 5; i++) {
				fhscore_out << HighScore[i];
				fhscore_out.put('\n');
			}
			fhscore_out.close();
		}

		//GameOver��ʾ
		setfillcolor(BROWN);
		fillrectangle(400, 250, 800, 700);
		settextcolor(0X71B33C);
		settextstyle(30, 0, _T("����"));
		TCHAR s[50];
		_stprintf_s(s, _T("Game Over\n\n���յ÷�: %d"), gameInfo[0].score);
		drawtext(s, &Button[11], DT_CENTER);
		drawtext(_T("���������Ϸ��ҳ"), &Button[12], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("������Ϸ�浵������"), &Button[13], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setlinecolor(0X00FF00);
		setlinestyle(PS_SOLID, 5);
		rectangle(Button[12].left, Button[12].top, Button[12].right, Button[12].bottom);
		rectangle(Button[13].left, Button[13].top, Button[13].right, Button[13].bottom);
		FlushBatchDraw();
		bool quit = false;
		bool isSave = false;
		while (!quit) {
			if (MouseHit()) m = GetMouseMsg();
			int choice = 0;
			switch (m.uMsg) {
			case WM_LBUTTONDOWN:
				if (m.x > Button[12].left&& m.x<Button[12].right && m.y>Button[12].top&& m.y < Button[12].bottom) {
					quit = true;
				}
				else if (m.x > Button[13].left&& m.x<Button[13].right && m.y>Button[13].top&& m.y < Button[13].bottom) {
					isSave = true;
					quit = true;
				}
				break;
			case WM_MOUSEMOVE:
				if (m.x > Button[12].left&& m.x<Button[12].right && m.y>Button[12].top&& m.y < Button[12].bottom) {
					choice = 1;
				}
				else if (m.x > Button[13].left&& m.x<Button[13].right && m.y>Button[13].top&& m.y < Button[13].bottom) {
					choice = 2;
				}
			}
			setlinestyle(PS_SOLID, 5);
			setlinecolor(0X00FF00);
			rectangle(Button[12].left, Button[12].top, Button[12].right, Button[12].bottom);
			rectangle(Button[13].left, Button[13].top, Button[13].right, Button[13].bottom);
			if (choice != 0) {
				setlinecolor(RED);
				rectangle(Button[choice + 11].left, Button[choice + 11].top, Button[choice + 11].right, Button[choice + 11].bottom);
			}
			FlushBatchDraw();
		}
		//�浵
		if (isSave) {
			ofstream Info("resource/DoubleSave.log");
			if (!Info) {
				cerr << "���ļ�ʧ�ܣ�" << endl;
				exit(1);
			}
			//д��ǽ
			vector<vector<int>> HardWall(bigMap.getWall()[0]);
			vector<vector<int>> SoftWall(bigMap.getWall()[1]);
			unsigned int n = HardWall[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < HardWall[0].size(); i++) {
				Info.write((char*)&HardWall[0][i], sizeof(HardWall[0][i]));
				Info.write((char*)&HardWall[1][i], sizeof(HardWall[1][i]));
			}
			n = SoftWall[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < SoftWall[0].size(); i++) {
				Info.write((char*)&SoftWall[0][i], sizeof(SoftWall[0][i]));
				Info.write((char*)&SoftWall[1][i], sizeof(SoftWall[1][i]));
			}
			//д��ʳ��
			vector<vector<int>> GoodFood = bigMap.getGoodFood();
			n = GoodFood[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < GoodFood[0].size(); i++) {
				Info.write((char*)&GoodFood[0][i], sizeof(GoodFood[0][i]));
				Info.write((char*)&GoodFood[1][i], sizeof(GoodFood[1][i]));
			}
			vector<vector<int>> BadFood = bigMap.getBadFood();
			n = BadFood[0].size();
			Info.write((char*)&n, sizeof(n));
			for (unsigned int i = 0; i < BadFood[0].size(); i++) {
				Info.write((char*)&BadFood[0][i], sizeof(BadFood[0][i]));
				Info.write((char*)&BadFood[1][i], sizeof(BadFood[1][i]));
			}
			//д�������Ϣ
			Info.write((char*)&gameInfo, sizeof(gameInfo));
			vector<vector<int>> path = p1.getPath();
			unsigned int num = path[0].size();
			Info.write((char*)&num, sizeof(num));
			for (unsigned int j = 0; j < num; j++) {
				Info.write((char*)&path[0][j], sizeof(path[0][j]));
				Info.write((char*)&path[1][j], sizeof(path[1][j]));
			}
			path = p2.getPath();
			num= path[0].size();
			Info.write((char*)&num, sizeof(num));
			for (unsigned int j = 0; j < num; j++) {
				Info.write((char*)&path[0][j], sizeof(path[0][j]));
				Info.write((char*)&path[1][j], sizeof(path[1][j]));
			}
			Info.close();
		}
		EndBatchDraw();
		return gameInfo[0].score;//��Ϸ�������ص÷�
	}
};


/////****************************************
////* ������Ϸ������(ʧ�ܴ���)
////*****************************************/
//class MultiGame {
//private:
//	int window_w, window_h;
//	//��ť��С��λ��
//	RECT b0 = { 40, 480, 300, 550 };
//	RECT b1 = { 360, 480, 620, 550 };
//	RECT b7 = { 300,240,600,340 };
//	RECT b8 = { 300,360,600,460 };
//	RECT b9 = { 300,480,600,580 };
//	RECT Button[10] = { b0,b1 };
//	
//	struct PlayerInfo {
//		int hp;
//		int score;
//		int power;
//	};
//
//	char myName[11];
//	ClientHelper me;
//	ClientHelper host;
//	room_list_t rlt;
//	uint32_t roomId;
//
//public:
//	//���캯��
//	MultiGame(char s[11]) {
//		//���õ�ͼ��С�ʹ��ڴ�С
//		window_w = 1200;
//		window_h = 700;
//		strcpy(myName, s);
//		host.connectToServer("111.231.112.136", 10001, myName);
//		me.connectToServer("111.231.112.136", 10001, myName);
//	}
//
//	bool GetRoom() {
//		if (me.getRoomList(rlt)) {
//			return true;
//		}
//		else {
//			return false;
//		}
//		//printf("��������:\t%u\n----�б�����----\n", rlt.roomNum);
//		//for (uint32_t i = 0; i < rlt.roomNum; ++i) {
//		//	printf("����%u:\n", i + 1);
//		//	printf("������:%s\t����id:%u\t��ǰ����:%u\n", rlt.rooms[i].roomName, rlt.rooms[i].roomId, rlt.rooms[i].existing);
//		//}
//	}
//	bool HostRoom() {
//		loadimage(NULL, _T("resource/8.jpg"), WINDOW_W, WINDOW_H);
//		cleardevice();
//		MOUSEMSG m{ 0 };
//		//�����û���
//		bool inputing = true;
//		TCHAR namebuffer[15] = _T("Snake_");
//		TCHAR s[50];
//		int nameLen = 6;
//		RECT inputBox = { WINDOW_W / 2 - 180,420,WINDOW_W / 2 + 200,470 };
//		RECT b[2] = { {300,500,600,540},{300,550,600,600} };
//		setbkmode(TRANSPARENT);
//		int inputCursor = 0;
//		int choice = 0;	
//		while (inputing) {
//			while (!_kbhit()) {
//				
//				if (MouseHit()) m = GetMouseMsg();
//				switch (m.uMsg) {
//				case WM_LBUTTONDOWN:
//					if (m.x > b[0].left&& m.x<b[0].right && m.y>b[0].top&& m.y < b[0].bottom) {
//						if (nameLen > 6) {
//							inputing = false;
//						}
//					}
//					else if (m.x > b[1].left&& m.x<b[1].right && m.y>b[1].top&& m.y < b[1].bottom) {
//						return false;
//					}
//					break;
//				case WM_MOUSEMOVE:
//					if (m.x > b[0].left&& m.x<b[0].right && m.y>b[0].top&& m.y < b[0].bottom) {
//						choice = 1;
//
//					}
//					else if (m.x > b[1].left&& m.x<b[1].right && m.y>b[1].top&& m.y < b[1].bottom) {
//						choice = 2;
//					}
//					else {
//						choice = 0;
//					}
//				}
//				setlinestyle(PS_SOLID, 5);
//				roundrect(b[0].left, b[0].top, b[0].right, b[0].bottom, 50, 50);
//				roundrect(b[1].left, b[1].top, b[1].right, b[1].bottom, 50, 50);
//				if (choice != 0) {
//					setlinecolor(RED);
//					roundrect(b[choice - 1].left, b[choice - 1].top, b[choice - 1].right, b[choice - 1].bottom, 50, 50);
//				}
//				setlinecolor(WHITE);
//				settextstyle(36, 0, _T("����"));
//				drawtext(_T("ȷ�ϲ�����"), &b[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//				drawtext(_T("ȡ��������"), &b[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//				settextstyle(30, 0, _T("΢���ź�"));
//				_stprintf_s(s, _T("�������û���(<10): %s"), namebuffer);
//				drawtext(s, &inputBox, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//				if (inputCursor < 50) {
//					setlinestyle(PS_SOLID, 5);
//					line(WINDOW_W / 2 + 22 + nameLen * 13, 430, WINDOW_W / 2 + 22 + nameLen * 13, 460);
//					inputCursor++;
//				}
//				else {
//					inputCursor++;
//					if (inputCursor > 100) {
//						inputCursor = 0;
//					}
//				}
//				setlinestyle(PS_SOLID, 8);
//				roundrect(inputBox.left - 20, inputBox.top, inputBox.right, inputBox.bottom, 50, 50);
//				FlushBatchDraw();
//				cleardevice();
//				loadimage(NULL, _T("resource/8.jpg"), WINDOW_W, WINDOW_H);
//				if (!inputing) {
//					break;
//				}
//			}
//			if (!inputing) {
//				break;
//			}
//			char c = _getch();
//			if (c >= '0' && c <= '9') {
//				if (nameLen < 10) {
//					namebuffer[nameLen++] = c;
//					namebuffer[nameLen] = '\0';
//				}
//			}
//			else if (c == 8) {
//				if (nameLen > 6) {
//					nameLen--;
//					namebuffer[nameLen] = '\0';
//				}
//			}
//		}
//		Sleep(100);
//		char name[21];
//		for (int i = 0; i < 15; i++) {
//			name[i] = (char)namebuffer[i];
//		}
//		name[15] = '\0';
//		if (!host.hostNewRoom(name, roomId)) {
//			return false;
//		}
//		//�Ѿ������˷���
//		if (!me.connectToRoom(roomId)) {
//			return false;
//		}
//		struct OP1 {
//			int opType;
//			int x1;
//			int y1;
//			int x2;
//			int y2;
//		};
//
//		struct OP2 {
//			int opType;
//			int x;
//			int y;
//		};
//
//		struct HW {
//			int x[20];
//			int y[20];
//		};
//
//
//		//��ʼ��
//		stringstream Info;	
//		//string infostr = "";
//		//initgraph(1200, 700);
//		string o;
//		MapA bigMap;
//		SnakeA Snakes[10];
//		string IP[10];
//		PlayerInfo PlayerInfos[10];
//		Snakes[0].InitSnake(bigMap.getWall());
//		IP[0] = myName;
//		PlayerInfos[0] = { 100,0,5 };
//		int PlayerNum = 1;
//		while (1) {
//			//����Ƿ������û�
//			//vector<int> isInRoom(IP.size(), 0);
//			//room_t RoomInfo;
//			//if (host.getRoomInfo(roomId, RoomInfo)) {
//			//	for (uint32_t i = 0; i < RoomInfo.existing; i++) {
//			//		string ip(RoomInfo.everyOne[i].name);//���ڱ��ص���IP����ͬ�����������ִ���
//			//		uint32_t j;
//			//		for (j = 0; j < IP.size(); j++) {
//			//			if (IP[j] == ip) {
//			//				//isInRoom[j] = 1;
//			//				break;
//			//			}
//			//		}
//			//		//�����������
//			//		if (j == IP.size()) {
//			//			IP.push_back(ip);
//			//			//isInRoom.push_back(1);
//			//			SnakeA asnake(bigMap.getWall());
//			//			Snakes.push_back(asnake);
//			//			PlayerInfo ainfo;
//			//			ainfo.hp = 5; ainfo.power = 100; ainfo.score = 0;
//			//			PlayerInfos.push_back(ainfo);
//			//		}
//			//	}
//			//}
//			////����Ƿ����˳������
//			//for (unsigned int i = 0; i < IP.size();i++) {
//			//	if (isInRoom[i] == 0) {
//			//		IP.erase(IP.begin() + i);
//			//		Snakes.erase(Snakes.begin() + i);
//			//		PlayerInfos.erase(PlayerInfos.begin() + i);
//			//		isInRoom.erase(isInRoom.begin() + i);
//			//		i--;
//			//	}
//			//}
//
//			//������Ϣ
//			//msg_package_t msg;
//		/*	if (host.recvMsg(msg)) {
//				for (uint32_t i = 0; i < msg.msgNum; i++) {
//					string ip(msg.msgs[i]->name);
//					uint32_t j;
//					for (j = 0; j < IP.size(); j++) {
//						if (IP[j] == ip) {
//							if (msg.msgs[i]->msgLen > 0) {
//								char c = msg.msgs[i]->msgContent[0];
//								switch (c) {
//								case '1':Snakes[j].ChangeOrientation(-90); break;
//								case '2':Snakes[j].ChangeOrientation(-45); break;
//								case '3':Snakes[j].ChangeOrientation(0); break;
//								case '4':Snakes[j].ChangeOrientation(45); break;
//								case '5':Snakes[j].ChangeOrientation(90); break;
//								case '6':Snakes[j].ChangeOrientation(135); break;
//								case '7':Snakes[j].ChangeOrientation(180); break;
//								case '8':Snakes[j].ChangeOrientation(-135); break;
//								}
//							}
//							break;
//						}				
//					}
//				}
//			}*/
//			//host.clearSend();
//			//�ƶ���֡
//			for (int frame = 0; frame < 4; frame++) {
//				for (unsigned int i = 0; i < PlayerNum; i++) {
//					//�����Ƿ��ڼ����������ٶȲ��ƶ�
//					//Snakes[i].setSpeed();
//					//Snakes[i].Move();
//
//				//	//int index;
//				//	////ײ���߽�
//				//	//if (Snakes[i].CheckBorder()) {
//				//	//	bigMap.AddSoftWall(Snakes[i].getPath()[0], Snakes[i].getPath()[1]);
//				//	//	Snakes[i].InitSnake(bigMap.getWall());
//				//	//	PlayerInfos[i].power = 100;
//				//	//	PlayerInfos[i].hp--;
//				//	//	PlayerInfos[i].score -= 2;
//				//	//	break;
//				//	//}
//				//	////ײ����ǽ
//				//	//else if ((index = Snakes[i].CheckSoftWall(bigMap.getWall()[1])) != -1) {
//				//	//	PlayerInfos[i].score -= 3;
//				//	//	PlayerInfos[i].power -= 5;
//				//	//	bigMap.DeleteSoftWall(index);
//				//	//	
//				//	//}
//				//	////ײ��Ӳǽ
//				//	//else if ((index = Snakes[i].CheckHardWall(bigMap.getWall()[0])) != -1) {
//				//	//	PlayerInfos[i].score -= 5;
//				//	//	PlayerInfos[i].power -= 10;
//				//	//	bigMap.DeleteHardWall(index);
//				//	//}
//				//	////�Ե����
//				//	//else if ((index = Snakes[i].CheckBadFood(bigMap.getBadFood())) != -1) {
//				//	//	PlayerInfos[i].score -= 5;
//				//	//	PlayerInfos[i].hp--;
//				//	//	PlayerInfos[i].power = 100;
//				//	//	Snakes[i].InitSnake(bigMap.getWall());
//				//	//	bigMap.DeleteBadFood(index);
//				//	//}
//				//	////�Ե������
//				//	//else if ((index = Snakes[i].CheckGoodFood(bigMap.getGoodFood())) != -1) {
//				//	//	PlayerInfos[i].score += 10;
//				//	//	PlayerInfos[i].power = 100;
//				//	//	bigMap.DeleteGoodFood(index);
//				//	//}
//				//	////�Ե���ʳ��
//				//	//else if ((index = Snakes[i].CheckBigFood(bigMap.getBigFood())) != -1) {
//				//	//	PlayerInfos[i].score += 5;
//				//	//	PlayerInfos[i].power -= 2;
//				//	//	bigMap.DeleteBigFood(index);
//				//	//}
//				//	////�Ե���ͨʳ��
//				//	//else if ((index = Snakes[i].CheckNormalFood(bigMap.getNormalFood())) != -1) {
//				//	//	PlayerInfos[i].score += 1;
//				//	//	PlayerInfos[i].power -= 1;
//				//	//	bigMap.DeleteNorFood(index);
//				//	//}
//				//	////ײ��������
//				//	//for (unsigned int k = 0; k < Snakes.size(); k++) {
//				//	//	if (i != k && Snakes[i].CheckCrashOther(Snakes[k].getPath())) {
//				//	//		PlayerInfos[i].hp--;
//				//	//		PlayerInfos[i].score -= 5;
//				//	//		PlayerInfos[i].power = 100;
//				//	//		bigMap.AddBigFood(Snakes[i].getPath()[0], Snakes[i].getPath()[1], Snakes[i].getColor());
//				//	//		Snakes[i].InitSnake(bigMap.getWall());
//				//	//		break;
//				//	//	}
//				//	//}
//				//	////����С��1������С��0������
//				//	//if (PlayerInfos[i].power <= 0 || Snakes[i].getLength() < 2) {
//				//	//	PlayerInfos[i].hp--;
//				//	//	PlayerInfos[i].power = 100;
//				//	//	Snakes[i].InitSnake(bigMap.getWall());
//				//	//}
//				}
//				//
//				//����һ֡����Ϣ
//
//				//д��ǽ
//				//unsigned int n;
//				//int h[60];
//				//struct {
//				//	int x;
//				//	int y;
//				//}hw;
//				
//				//int* a = new int[80];
//				//delete[]a;
//				
//				//int a[40];
//				//host.sendBinaryMsg((const char*)&a, 40 * sizeof(int));
//				char info[5000] = { 0 };
//				host.sendBinaryMsg((const char*)info, 5000);
//				host.sendBinaryMsg((const char*)info, 5000);
//				host.sendBinaryMsg((const char*)info, 5000);
//				host.sendBinaryMsg((const char*)info, 5000);
//				//if (HardWall) {
//				//	memcpy(HardWall, &HardWallX[0], HardWallX.size() * sizeof(int));
//				//	memcpy(HardWall + 20, &HardWallY[0], HardWallY.size() * sizeof(int));
//				//	//host.sendBinaryMsg((char*)HardWall, 40 * sizeof(int));
//				o = "234242342343243242454323";
//				host.sendMsg(o.c_str());
//				//}
//				//delete[]HardWall;
//				
//				vector<int> HardWallX(bigMap.getWall()[0][0]);
//				vector<int> HardWallY(bigMap.getWall()[0][1]);
//				int* HardWall = new(nothrow)int[40];
//				if (HardWall) {
//					memcpy(HardWall, &HardWallX[0], HardWallX.size() * sizeof(int));
//					memcpy(HardWall + 20, &HardWallY[0], HardWallY.size() * sizeof(int));
//					host.sendBinaryMsg((char*)HardWall, 40);
//				}
//				delete[]HardWall;
//
//				//for (int k = 0; k < HardWallX.size(); k++) {
//				//	
//				//}
//				//hw.x = HardWallX[0];
//				//hw.y = HardWallY[0];
//				//host.sendBinaryMsg((const char*)&hw, sizeof(hw));
//				//Sleep(500);
//				//hw.x = HardWallX[1];
//				//hw.y = HardWallY[1];
//				//host.sendBinaryMsg((char*)h, 240);
//				//HardWall = new(nothrow)int[HardWallX.size() * 2 + 2];
//				//if (HardWall) {
//				//	memcpy(HardWall + 2, &HardWallX[0], HardWallX.size() * sizeof(int));
//				//	memcpy(HardWall + 2 + HardWallX.size(), &HardWallY[0], HardWallY.size() * sizeof(int));
//				//	HardWall[0] = 1; HardWall[1] = (int)HardWallX.size();
//				//	host.sendBinaryMsg((char*)HardWall, (HardWallX.size()*2 + 2)*4);
//				//}
//				//if (HardWall) {
//
//				//}
//				
//				
//				//int* SoftWall = new(nothrow)int[bigMap.getWall()[1][0].size() * 2 + 2];
//				//if (SoftWall) {
//				//	if (!bigMap.getWall()[1][0].empty()) {
//				//		memcpy(SoftWall + 2, &bigMap.getWall()[1][0][0], bigMap.getWall()[1][0].size() * sizeof(int));
//				//		memcpy(SoftWall + 2 + bigMap.getWall()[1][0].size(), &bigMap.getWall()[1][1][0], bigMap.getWall()[1][0].size() * sizeof(int));
//				//		SoftWall[0] = 1; SoftWall[1] = (int)bigMap.getWall()[1][0].size();
//				//		host.sendBinaryMsg((char*)SoftWall, bigMap.getWall()[1][0].size()*2 + 2);
//				//	}
//				//}
//				//delete[]SoftWall;
//
//				//n = HardWall[0].size();
//				//Info.write((char*)&n, sizeof(n));
//				//for (unsigned int i = 0; i < HardWall[0].size(); i++) {
//				//	Info.write((char*)&HardWall[0][i], sizeof(HardWall[0][i]));
//				//	Info.write((char*)&HardWall[1][i], sizeof(HardWall[1][i]));
//				//}
//				//n= SoftWall[0].size();
//				//Info.write((char*)&n, sizeof(n));
//				//for (unsigned int i = 0; i < SoftWall[0].size(); i++) {
//				//	Info.write((char*)&SoftWall[0][i], sizeof(SoftWall[0][i]));
//				//	Info.write((char*)&SoftWall[1][i], sizeof(SoftWall[1][i]));
//				//}
//				////д��ʳ��
//				//vector<vector<int>> GoodFood = bigMap.getGoodFood();
//				//n= GoodFood[0].size();
//				//Info.write((char*)&n, sizeof(n));
//				//for (unsigned int i = 0; i < GoodFood[0].size(); i++) {
//				//	Info.write((char*)&GoodFood[0][i], sizeof(GoodFood[0][i]));
//				//	Info.write((char*)&GoodFood[1][i], sizeof(GoodFood[1][i]));
//				//}
//				//vector<vector<int>> BadFood = bigMap.getBadFood();
//				//n= BadFood[0].size();
//				//Info.write((char*)&n, sizeof(n));
//				//for (unsigned int i = 0; i < BadFood[0].size(); i++) {
//				//	Info.write((char*)&BadFood[0][i], sizeof(BadFood[0][i]));
//				//	Info.write((char*)&BadFood[1][i], sizeof(BadFood[1][i]));
//				//}
//				//vector<vector<int>> NormalFood = bigMap.getNormalFood();
//				//vector<DWORD> C_normalFood = bigMap.getNormalC();
//				//n= NormalFood[0].size();
//				//Info.write((char*)&n, sizeof(n));
//				//for (unsigned int i = 0; i < NormalFood[0].size(); i++) {
//				//	Info.write((char*)&NormalFood[0][i], sizeof(NormalFood[0][i]));
//				//	Info.write((char*)&NormalFood[1][i], sizeof(NormalFood[1][i]));
//				//	Info.write((char*)&C_normalFood[i], sizeof(C_normalFood[i]));
//				//}
//				//vector<vector<int>> BigFood = bigMap.getBigFood();
//				//vector<DWORD> C_bigFood = bigMap.getBigC();
//				//n= BigFood[0].size();
//				//Info.write((char*)&n, sizeof(n));
//				//for (unsigned int i = 0; i < BigFood[0].size(); i++) {
//				//	Info.write((char*)&BigFood[0][i], sizeof(BigFood[0][i]));
//				//	Info.write((char*)&BigFood[1][i], sizeof(BigFood[1][i]));
//				//	Info.write((char*)&C_bigFood[i], sizeof(C_bigFood[i]));
//				//}
//
//				//д���������
//				//host.sendBinaryMsg((const char*)PlayerInfos, sizeof(PlayerInfos));
//				//unsigned int n= IP.size();
//				//Info.write((char*)&n, sizeof(n));
//				//for (unsigned int k = 0; k < n; k++) {
//				//	Info.write((char*)PlayerInfos+k, sizeof(PlayerInfos[k]));
//				//}
//				//if (Info.str().data() != NULL) {
//				//	host.sendBinaryMsg(Info.str().data(), 0);
//				//}
//				//
//				//Info.clear();
//				//Info.str("");
//				////д���ߵ���Ϣ
//				//for (unsigned int i = 0; i < n; i++) {
//				//	SnakeInfo si = Snakes[i].getInfo();
//				//	Info.write((char*)&si, sizeof(si));
//				//	vector<vector<int>> path = Snakes[i].getPath();
//				//	int num = path[0].size();
//				//	Info.write((char*)&num, sizeof(num));
//				//	for (unsigned int j = 0; j < num; j++) {
//				//		Info.write((char*)&path[0][j], sizeof(path[0][j]));
//				//		Info.write((char*)&path[1][j], sizeof(path[1][j]));
//				//	}
//				//	path = Snakes[i].getNextPath();
//				//	num = path[0].size();
//				//	Info.write((char*)&num, sizeof(num));
//				//	for (unsigned int j = 0; j < num; j++) {
//				//		Info.write((char*)&path[0][j], sizeof(path[0][j]));
//				//		Info.write((char*)&path[1][j], sizeof(path[1][j]));
//				//	}
//				//	path = Snakes[i].getLastPath();
//				//	num = path[0].size();
//				//	Info.write((char*)&num, sizeof(num));
//				//	for (unsigned int j = 0; j < num; j++) {
//				//		Info.write((char*)&path[0][j], sizeof(path[0][j]));
//				//		Info.write((char*)&path[1][j], sizeof(path[1][j]));
//				//	}
//				//}
//				//if (nn < 100) {
//				//	infostr[nn] = Info.str();
//				//	Info.clear();
//				//	Info.str("");
//				//	host.sendBinaryMsg(infostr[nn].data(), infostr[nn].length());
//				//	string().swap(infostr[(nn + 100) % 100]);
//				//	nn++;
//				//}
//				//else {
//				//	nn = 0;
//				//	infostr[nn] = Info.str();
//				//	Info.clear();
//				//	Info.str("");
//				//	host.sendBinaryMsg(infostr[nn].data(), infostr[nn].length());
//				//	//string().swap(infostr[(nn + 100) % 100]);
//				//	nn++;
//				//}
//
//				////д��IP
//				//Info << " ";
//				//for (unsigned int i = 0; i < n; i++) {
//				//	Info << IP[i] << " ";
//				//}
//				
//				//IMAGE MAP;
//				//Resize(&MAP, MAP_W, MAP_H);
//				cleardevice();
//				loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
//				//SetWorkingImage(&MAP);
//				//cleardevice();
//				//bigMap.DrawMap();
//				//for (unsigned int i = 0; i < Snakes.size(); i++) {
//				//	Snakes[i].DrawSnake();
//				//}
//				//SetWorkingImage();
//				//setaspectratio(float(250.0 / MAP_W), float(250.0 / MAP_H));
//				//putimage(int(900 * MAP_W / 250.0), int(50 * MAP_H / 250.0), &MAP);//����С��ͼ
//				//setaspectratio(1, 1);
//				//putimage(50, 50, 800, 600, &MAP, Snakes[0].getPath()[0][0] - 400, Snakes[0].getPath()[1][0] - 300);
//				////DrawUI();
//				//FlushBatchDraw();				
//			}
//			//�����Լ�
//		}
//		return true;
//	}
//	void DrawRoomList() {
//		rectangle(0, 0, 900, 430);
//		//������
//		setbkmode(TRANSPARENT);
//		settextcolor(WHITE);
//		settextstyle(36, 0, _T("����"));
//		//RECT L1[4] = { {0,0,300,50},{300,0,600,50},{600,0,750,50},{750,0,900,50} };
//		//drawtext(_T("��������"), &L1[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//		//drawtext(_T("����ID"), &L1[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//		//drawtext(_T("��ǰ����"), &L1[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//		//drawtext(_T("����"), &L1[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//		settextcolor(WHITE);
//		settextstyle(20, 0, _T("����"));
//		for (uint32_t i = 0; i < rlt.roomNum; ++i) {
//			RECT b1 = { 0,50 + i * 20,300,50 + (i + 1) * 20 };
//			char s[100];
//			TCHAR S[100];
//			sprintf(s, "%s", rlt.rooms[i].roomName);
//			for (int i = 0; i < 30; i++) {
//				S[i] = s[i];
//			}
//			drawtext(S, &b1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			RECT b2 = { 300,50 + i * 20,600,50 + (i + 1) * 20 };
//			_stprintf_s(S, _T("%u"), rlt.rooms[i].roomId);
//			drawtext(S, &b2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			RECT b3 = { 600,50 + i * 20,750,50 + (i + 1) * 20 };
//			_stprintf_s(S, _T("%u"), rlt.rooms[i].existing);
//			drawtext(S, &b3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
//		}
//		setlinecolor(WHITE);
//		setlinestyle(PS_SOLID, 3);
//		for (int i = 0; i < 2; i++) {
//			roundrect(Button[i].left, Button[i].top, Button[i].right, Button[i].bottom, 150, 150);
//		}
//		settextstyle(36, 0, _T("����"));
//		settextcolor(WHITE);
//		drawtext(_T("�����ҵķ���"), &Button[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//		drawtext(_T("ˢ�·����б�"), &Button[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	}
//
//	void PlayerRun() {
//		me.getRoomList(rlt);
//		me.connectToRoom(rlt.rooms[0]);
//		msg_package_t msg;
//		MapA bigMap; 
//		//PlayerInfo PlayerInfos[100];
//		//SnakeA Snakes[100];
//		//string IP[100];
//		//int playerNum = 0;
//		initgraph(window_w, window_h);
//		BeginBatchDraw();
//		bool isPlaying = true;
//		while (isPlaying) {
//			while (!_kbhit()) {
//				//������Ϣ
//				msg_package_t msg;
//				if (me.recvMsg(msg)) {
//					for (uint32_t i = 0; i < msg.msgNum; i++) {
//						string ip(msg.msgs[i]->name);
//						uint32_t j;
//						if (msg.msgs[i]->msgLen == 160) {
//							int HardWall[40];
//							memcpy(HardWall, msg.msgs[i]->msgContent, 160);
//							bigMap.SetHardWall(HardWall);
//							drawtext(_T("9879w8482823498"), &Button[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//						}
//					}
//				}
//				drawtext(_T("9879w84wedwd2erff98"), &Button[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//				////��ȡ��Ϣ
////				if (me.recvMsg(msg)) {
////					for (uint32_t i = 0; i < msg.msgNum; i++) {
////						char* m = msg.msgs[i]->msgContent;
/////*						i
////						string str();
////						stringstream Info(str);	*/	
////						unsigned int n;
////						int x, y;
////		
////					}
////				}
////				////����
//				IMAGE MAP;
//				Resize(&MAP, MAP_W, MAP_H);
//				//cleardevice();
//				loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
//				SetWorkingImage(&MAP);
//				cleardevice();
//				bigMap.DrawMap();
//				//for (int i = 0; i < playerNum; i++) {
//				//	Snakes[i].DrawSnake();
//				//}
//				SetWorkingImage();
//				setaspectratio(float(250.0 / MAP_W), float(250.0 / MAP_H));
//				putimage(int(900 * MAP_W / 250.0), int(50 * MAP_H / 250.0), &MAP);//����С��ͼ
//				setaspectratio(1, 1);
//				//putimage(50, 50, 800, 600, &MAP, Snakes[0].getPath()[0][0] - 400, Snakes[0].getPath()[1][0] - 300);
//				//gameInfo.length = p1.getLength();//���³���
//				//DrawUI();
//				FlushBatchDraw();
//			}
//
//			//��ⰴ�����޸ķ���
//			char key1 = _getch();
//			if (key1 == 27) {
//				isPlaying = false;
//				break;
//			}
//			char key2;
//			int keyNum = 1;
//			if (key1 != 'w' && key1 != 'W' && key1 != 'a' && key1 != 'A' && key1 != 's'
//				&& key1 != 'S' && key1 != 'd' && key1 != 'D' && key1 != -32) {
//				continue;
//			}
//			if (key1 == -32) {
//				key1 = _getch();
//			}
//			Sleep(100);
//			if (_kbhit()) {
//				key2 = _getch();
//				keyNum++;
//				if (key2 == -32) {
//					key2 = _getch();
//				}
//			}
//			int angleNext = 100;
//			switch (key1) {
//			case'w':
//			case'W':
//			case 72:
//				angleNext = -90;
//				if (keyNum == 2) {
//					if (key2 == 'a' || key2 == 'A' || key2 == 75) {
//						angleNext = -135;
//					}
//					else if (key2 == 'd' || key2 == 'D' || key2 == 77) {
//						angleNext = -45;
//					}
//				}
//				break;
//			case's':
//			case'S':
//			case 80:
//				angleNext = 90;
//				if (keyNum == 2) {
//					if (key2 == 'a' || key2 == 'A' || key2 == 75) {
//						angleNext = 135;
//					}
//					else if (key2 == 'd' || key2 == 'D' || key2 == 77) {
//						angleNext = 45;
//					}
//				}
//				break;
//			case'a':
//			case'A':
//			case 75:
//				angleNext = 180;
//				if (keyNum == 2) {
//					if (key2 == 'w' || key2 == 'W' || key2 == 72) {
//						angleNext = -135;
//					}
//					else if (key2 == 's' || key2 == 'S' || key2 == 80) {
//						angleNext = 135;
//					}
//				}
//				break;
//			case'd':
//			case'D':
//			case 77:
//				angleNext = 0;
//				if (keyNum == 2) {
//					if (key2 == 'w' || key2 == 'W' || key2 == 72) {
//						angleNext = -45;
//					}
//					else if (key2 == 's' || key2 == 'S' || key2 == 80) {
//						angleNext = 45;
//					}
//				}
//				break;
//			}
//			switch (angleNext) {
//			case -90:me.sendMsg("1"); break;
//			case -45:me.sendMsg("2"); break;
//			case 0:me.sendMsg("3"); break;
//			case 45:me.sendMsg("4"); break;
//			case 90:me.sendMsg("5"); break;
//			case 135:me.sendMsg("6"); break;
//			case 180:me.sendMsg("7"); break;
//			case -135:me.sendMsg("8"); break;
//			}
//		}
//	}
//
//	//����UI��Ϣ
//	//void DrawUI() {
//	//	setbkmode(TRANSPARENT);
//	//	setlinestyle(PS_SOLID, 3);
//	//	setlinecolor(WHITE);
//	//	settextstyle(36, 0, _T("����"), 0, 0, 1000, false, false, false);
//	//	TCHAR s[50];
//	//	settextcolor(WHITE);
//	//	drawtext(_T("Player 1"), &Button[0], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	drawtext(_T("Player 2"), &Button[5], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("���� %lf"), gameInfo[0].length);
//	//	drawtext(s, &Button[1], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("���� %d"), gameInfo[0].hp);
//	//	drawtext(s, &Button[3], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("�÷� %d"), gameInfo[0].score);
//	//	drawtext(s, &Button[2], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("���� %lf"), gameInfo[1].length);
//	//	drawtext(s, &Button[4], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("���� %d"), gameInfo[1].hp);
//	//	drawtext(s, &Button[6], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("�÷� %d"), gameInfo[1].score);
//	//	drawtext(s, &Button[7], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	settextstyle(26, 0, _T("����"), 0, 0, 1000, false, false, false);
//	//	_stprintf_s(s, _T("ʱ  �� %ds"), (clock() - gameInfo[0].startTime) / 1000);
//	//	drawtext(s, &Button[8], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("��߷� %d"), gameInfo[0].highestScore);
//	//	if (gameInfo[0].score > gameInfo[0].highestScore) {
//	//		_stprintf_s(s, _T("��ߵ÷� %d"), gameInfo[0].score);
//	//	}
//	//	else if (gameInfo[1].score > gameInfo[0].highestScore) {
//	//		_stprintf_s(s, _T("��ߵ÷� %d"), gameInfo[1].score);
//	//	}
//	//	drawtext(s, &Button[9], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//	//	_stprintf_s(s, _T("ESC��������Ϸ"));
//	//	drawtext(s, &Button[10], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	//	////������
//	//	setfillcolor(GREEN);
//	//	solidroundrect(520, int(400 - 200 * (double)gameInfo[0].power / 100.0), 580, 400, 30, 30);
//	//	solidroundrect(620, int(400 - 200 * (double)gameInfo[1].power / 100.0), 680, 400, 30, 30);
//	//	setfillcolor(WHITE);
//	//	setlinecolor(YELLOW);
//	//	setlinestyle(PS_SOLID, 5);
//	//	roundrect(520, 200, 580, 400, 30, 30);
//	//	roundrect(620, 200, 680, 400, 30, 30);
//	//	if (gameInfo[0].power < 10) {
//	//		RECT b = { 105,10,405,60 };
//	//		settextcolor(0X00FF00);
//	//		settextstyle(28, 0, _T("����"), 0, 0, 1000, false, false, false);
//	//		drawtext(_T("���ܲ��㣡��ʱ���䣡"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	//	}
//	//	if (gameInfo[1].power < 10) {
//	//		RECT b = { 795,10,1095,60 };
//	//		settextcolor(0X00FF00);
//	//		settextstyle(28, 0, _T("����"), 0, 0, 1000, false, false, false);
//	//		drawtext(_T("���ܲ��㣡��ʱ���䣡"), &b, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	//	}
//	//}
//
//
//
//	void ChooseRoom() {
//		MOUSEMSG m{ 0 };
//		GetRoom();
//		initgraph(WINDOW_W, WINDOW_H);
//		BeginBatchDraw();
//		while (1) {	
//			cleardevice();
//			Resize(NULL, WINDOW_W, WINDOW_H);
//			loadimage(NULL, _T("resource/8.jpg"), WINDOW_W, WINDOW_H);
//			DrawRoomList();
//			//�������¼�
//			if (MouseHit()) m = GetMouseMsg();
//			int i;
//			switch (m.uMsg) {
//			case WM_LBUTTONDOWN:
//				for (i = 0; i < 2; i++) {
//					if (m.x > Button[i].left&& m.x<Button[i].right && m.y>Button[i].top&& m.y < Button[i].bottom) {
//						break;
//					}
//				}
//				if (i < 2) {
//					//������
//					if (i == 0) {
//						HostRoom();
//					}
//					else if (i == 1) {
//						/*GetRoom();*/
//						//PlayerRun();
//
//					}
//				}
//				break;
//			case WM_MOUSEMOVE:
//				for (i = 0; i < 2; i++) {
//					if (m.x > Button[i].left&& m.x<Button[i].right && m.y>Button[i].top&& m.y < Button[i].bottom) {
//						setlinecolor(RED);
//						setlinestyle(PS_SOLID, 3);
//						roundrect(Button[i].left, Button[i].top, Button[i].right, Button[i].bottom, 150, 150);
//						break;
//					}
//				}
//				break;
//			}
//			FlushBatchDraw();
//		}
//		closegraph();             //�˳�ͼ�ν���	
//	}
//}