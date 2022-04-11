#include<graphics.h>
#include<conio.h>
#include<cmath>
#include<stdlib.h>
#include<ctime>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;

#define PI 3.1415926
#define WINDOW_W 900 //���ڿ�
#define WINDOW_H 600 //���ڸ�
//�������������ɫ
DWORD COLORSET[30] = { 0X808080,0X5C5CCD,0X8080F0,0X4763FF,0X507FFF,0X008CFF,0XADDEFF,0X00FFFF,0X2FFFAD,0X00FF00,
					0X7FFF00,0XD0E040,0XEEEEAF,0XFACE87,0XE16941,0XFF901E,0X00D7FF,0XD30094,0XDDA0DD,0XC1B6FF };

/****************************************
* ����
*****************************************/
class Snake {
private:
	int length; //����
	DWORD color;//��ɫ
	int radius; //�뾶
	double angle;//��ͷ����
	int state;//�ߵ�״̬
	int speed;//���ٶ�
	int frame;//֡��
	int frame_now;
	vector<int> pathX;//�߽ڵ㵱ǰ��x����
	vector<int> pathY;//�߽ڵ㵱ǰ��y����
	vector<int> pathX_nextframe;//�߽ڵ���һ֡x����
	vector<int> pathY_nextframe;//�߽ڵ���һ֡y����
	vector<int> pathX_lastframe;//�߽ڵ���һ֡x����
	vector<int> pathY_lastframe;//�߽ڵ���һ֡y����

public:
	//���캯��
	Snake(vector<vector<int>> wall) {
		InitSnake(wall);
	}
	/****************************************
	������wallǽ������
	����ֵ���Ƿ�ɹ�
	���ܣ���ʼ��һ����
	*****************************************/
	int InitSnake(vector<vector<int>> wall) {
		srand((unsigned)time(NULL));
		//�����ߵ��������
		speed = 5;
		length = 3;
		state = 0;
		radius = 10;
		angle = 0;
		color = COLORSET[rand() % 20];
		frame = radius * 2 / speed;
		frame_now = 0;
		pathX.clear();
		pathY.clear();
		pathX_nextframe.clear();
		pathY_nextframe.clear();

		//���������ͷ����
		int headX, headY;
		srand((unsigned)time(NULL));
		bool flag = true;
		int times = 5000;//������5000��
		while (flag && times) {
			flag = false;
			times--;
			//�ڵ�ͼ�߽緶Χ����������
			headX = rand() % (wall[0][2] - wall[0][0] - 60) + 30 + wall[0][0];
			headY = rand() % (wall[0][3] - wall[0][1] - 60) + 30 + wall[0][1];
			//�ж����ɵ������Ƿ�ײ��ǽ
			unsigned int i;
			for (i = 0; i < wall[1].size(); i++) {
				if (pow(headX - wall[1][i], 2) + pow(headY - wall[2][i], 2) < pow(radius + 5, 2)) {
					flag = true;
					break;
				}
			}
		}
		//����5000�ζ�ײ��ǽ������ʧ��
		if (times == 0) {
			return -1;
		}
		else {
			pathX.insert(pathX.begin(), length, headX);
			pathY.insert(pathY.begin(), length, headY);
			pathX_nextframe.insert(pathX_nextframe.begin(), pathX.begin(), pathX.end());
			pathY_nextframe.insert(pathY_nextframe.begin(), pathY.begin(), pathY.end());
			return 0;
		}
	}
	
	//��ȡ�߳���
	int getLength() {
		return length;
	}
	//��ȡ��״̬
	int getState() {
		return state;
	}
	//��ȡ��ͷ����
	int getAngle() {
		return (int)angle;
	}
	//��ȡ�ߵ���ɫ
	DWORD getColor() {
		return color;
	}
	//��ȡ�߽ڵ�����
	vector<vector<int>> getPath() {
		vector<vector<int>> vec = { pathX,pathY };
		return vec;
	}
	//������״̬
	void setState(int a) {
		state = a;
	}
	//������ͷ����
	void ChangeOrientation(int newAngle) {
		angle = newAngle;
		frame_now = 0;
	}
	
	/****************************************
	���ܣ�������
	*****************************************/
	void DrawSnake() {
		int n = (int)pathX.size();
		setfillcolor(color);
		//�߽ڵ�
		for (int i = 0; i < n; i++) {
			solidcircle(pathX[i], pathY[i], radius);
		}
		//����
		setfillcolor(BLACK);
		solidcircle(pathX[0] + int((radius - 1) * cos((angle - 45) / 180 * PI)), pathY[0] + int((radius - 1) * sin((angle - 45) / 180 * PI)), 4);
		solidcircle(pathX[0] + int((radius - 1) * cos((angle + 45) / 180 * PI)), pathY[0] + int((radius - 1) * sin((angle + 45) / 180 * PI)), 4);
	}

	/****************************************
	���ܣ��ƶ���
	*****************************************/
	void Move() {
			//������4֡���¿�ʼ
			if (frame_now == 0) {
				pathX.clear();
				pathY.clear();
				pathX.insert(pathX.begin(), pathX_nextframe.begin(), pathX_nextframe.end());
				pathY.insert(pathY.begin(), pathY_nextframe.begin(), pathY_nextframe.end());
				//�ߵĳ�������ʱ��ӽڵ�
				if ((unsigned int)length > pathX.size()) {
					pathX.push_back(pathX_lastframe[pathX.size() - 1]);
					pathY.push_back(pathY_lastframe[pathY.size() - 1]);
				}
				pathX_nextframe.clear();
				pathY_nextframe.clear();
				pathX_lastframe.clear();
				pathY_lastframe.clear();
				pathX_nextframe.insert(pathX_nextframe.begin(), pathX.begin(), pathX.end() - 1);
				pathY_nextframe.insert(pathY_nextframe.begin(), pathY.begin(), pathY.end() - 1);
				pathX_nextframe.insert(pathX_nextframe.begin(), pathX[0] + int(2 * 10 * cos(angle / 180 * PI)));
				pathY_nextframe.insert(pathY_nextframe.begin(), pathY[0] + int(2 * 10 * sin(angle / 180 * PI)));
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

	/****************************************
	������mapInfo�߽��ǽ������ normalFood��ͨʳ������
		  bigFood����������ʳ�������
	����ֵ��ײ�����������
	���ܣ�������Ƿ�ײ���������
	*****************************************/
	int updateState(vector<vector<int>> mapInfo, vector<vector<int>>normalFood, vector<vector<int>>bigFood) {
		//ײ�ϱ߽�stateΪ1
		int xMin = mapInfo[0][0], yMin = mapInfo[0][1], xMax = mapInfo[0][2], yMax = mapInfo[0][3];
		if (pathX[0] - radius<xMin || pathY[0] - radius<yMin ||
			pathX[0] + radius>xMax || pathY[0] + radius>yMax) {
			state = 1;
			return 0;
		}
		////ײ���Լ�stateΪ2
		//i=3��ʼ�����һ���ɾ���������
		for (unsigned int i = 3; i < pathX.size(); i++) {
			if (pow(pathX[0] - pathX[i], 2) + pow(pathY[0] - pathY[i], 2) < pow(2 * radius, 2)) {
				state = 2;
				return i;
			}
		}
		//ײ��ǽstateΪ3
		for (unsigned int i = 0; i < mapInfo[1].size(); i++) {
			if (pow(pathX[0] - mapInfo[1][i], 2) + pow(pathY[0] - mapInfo[2][i], 2) < pow(radius + 5, 2)) {
				state = 3;
				return i;
			}
		}
		//�Ե���ͨʳ��ΪstateΪ4
		for (unsigned int i = 0; i < normalFood[0].size(); i++) {
			if (pow(pathX[0] - normalFood[0][i], 2) + pow(pathY[0] - normalFood[1][i], 2) < pow(radius + 3, 2)) {
				state = 4;
				length += 1;
				return i;
			}
		}
		//�Ե���ʳ��stateΪ5
		for (unsigned int i = 0; i < bigFood[0].size(); i++) {
			if (pow(pathX[0] - bigFood[0][i], 2) + pow(pathY[0] - bigFood[1][i], 2) < pow(radius + 5, 2)) {
				state = 5;
				length += 1;
				return i;
			}
		}
		return -1;
	}
};

/****************************************
* ��ͼ��
*****************************************/
class Map {
private:
	DWORD fillColor; //��ͼ����ɫ
	int xMin, yMin, xMax, yMax;//��ͼ��Χ
	int mapWidth, mapHeight; //��ͼ��Ⱥ͸߶�

	vector<int> X_Wall, Y_Wall;//ǽ������
	DWORD wallColor;//ǽ����ɫ
	int wallSize;//ǽ�ĳߴ�

	vector<int> X_normalFood, Y_normalFood;//��ͨʳ������
	vector<DWORD> C_normalFood;//��ͨʳ����ɫ
	int N_normalFood;//��ͨʳ������
	int R_normalFood;//��ͨʳ��뾶
	//���������ɵ�ʳ��
	vector<int> X_bigFood, Y_bigFood;
	vector<DWORD> C_bigFood;
	int R_bigFood;

	bool isBlank;//��ͼ�ռ��Ƿ��㹻��־

public:
	//���캯��
	Map() {
		fillColor = WHITE;
		mapWidth = 800;
		mapHeight = 600;
		xMin = yMin = 50;
		xMax = xMin + mapWidth;
		yMax = yMin + mapHeight;
		wallColor = BROWN;
		wallSize = 10;
		R_normalFood = 3;
		N_normalFood = 5;
		R_bigFood = 5;
		InitFood();
		isBlank = true;
	}
	//��ȡ��ͨʳ������
	vector<vector<int>> getNormalFood() {
		vector<vector<int>> vec;
		vec.push_back(X_normalFood);
		vec.push_back(Y_normalFood);
		return vec;
	}
	//��ȡ������ʳ������
	vector<vector<int>> getBigFood() {
		vector<vector<int>> vec;
		vec.push_back(X_bigFood);
		vec.push_back(Y_bigFood);
		return vec;
	}
	//��ȡ��ͼ�߽��ǽ����
	vector<vector<int>> getMapInfo() {
		int info[4] = { xMin,yMin,xMax,yMax };
		vector<int> border(info, info + 4);
		vector<vector<int>> mapinfo = { border,X_Wall,Y_Wall };
		return mapinfo;
	}
	//����ͼ�Ƿ��пռ�ʣ��
	bool getIsBlank() {
		if (wallSize * wallSize * ((int)X_Wall.size() + 3) > (xMax - xMin)* (yMax - yMin)) {
			isBlank = false;
		}
		return isBlank;
	}

	/****************************************
	���ܣ���ʼ��ʳ��
	*****************************************/
	void InitFood() {
		srand((unsigned)time(NULL));
		int x, y;
		for (int i = 0; i < 5; i++) {
			bool flag = true;
			int times = 5000;//������ɴ���
			while (flag && times) {
				flag = false;
				times--;
				x = rand() % (xMax - xMin - 20) + 10 + xMin;
				y = rand() % (yMax - yMin - 20) + 10 + yMin;
				unsigned int j;
				for (j = 0; j < X_Wall.size(); j++) {
					if (pow(x - X_Wall[j], 2) + pow(y - Y_Wall[j], 2) < pow(R_normalFood + wallSize, 2)) {
						flag = true;
						break;//ײ��ǽ��������
					}
				}
			}
			if (times == 0) {
				isBlank = false;
				N_normalFood = (int)X_normalFood.size();
				return;//����ʧ�ܣ�����ʳ������
			}
			else {
				X_normalFood.push_back(x);
				Y_normalFood.push_back(y);
				C_normalFood.push_back(RGB(rand() % 200, rand() % 200, rand() % 200));
			}

		}
		N_normalFood = (int)X_normalFood.size();
	}

	/****************************************
	������indexʳ������ foodTypeʳ������
	���ܣ�ɾ��ָ��ʳ��
	*****************************************/
	void DeleteFood(int index, int foodType) {
		if (foodType == 1) {
			X_normalFood.erase(X_normalFood.begin() + index);
			Y_normalFood.erase(Y_normalFood.begin() + index);
			C_normalFood.erase(C_normalFood.begin() + index);
			N_normalFood--;
			if (N_normalFood <= 0) {
				InitFood();
			}
		}
		else if (foodType == 2) {
			X_bigFood.erase(X_bigFood.begin() + index);
			Y_bigFood.erase(Y_bigFood.begin() + index);
			C_bigFood.erase(C_bigFood.begin() + index);
		}
	}

	/****************************************
	������x��y�����߽ڵ����� color����ɫ
	���ܣ������������ʳ��
	*****************************************/
	void AddFood(vector<int> x, vector<int> y, DWORD color) {
		X_bigFood.insert(X_bigFood.begin(), x.begin() + 1, x.end());
		Y_bigFood.insert(Y_bigFood.begin(), y.begin() + 1, y.end());
		C_bigFood.insert(C_bigFood.begin(), x.size() - 1, color);
	}

	/****************************************
	������x��y�����߽ڵ�����
	���ܣ������������ǽ
	*****************************************/
	void AddWall(vector<int> x, vector<int> y) {
		X_Wall.insert(X_Wall.begin(), x.begin(), x.end());
		Y_Wall.insert(Y_Wall.begin(), y.begin(), y.end());
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
	}
	//����ǽ
	void DrawWall() {
		int n = (int)X_Wall.size();
		setfillcolor(wallColor);
		for (int i = 0; i < n; i++) {
			solidrectangle(X_Wall[i] - wallSize, Y_Wall[i] - wallSize, X_Wall[i] + wallSize, Y_Wall[i] + wallSize);
		}
	}
	//���Ƶ�ͼ
	void DrawMap() {
		setfillcolor(fillColor);
		solidrectangle(xMin, yMin, xMax, yMax);
		DrawFood();
		DrawWall();
	}
};



/****************************************
* ��Ϸ������
*****************************************/
class BaseGame {
private:
	int mode;//��Ϸģʽ(���Ű�1�����װ�2���߼���3)
	int window_w, window_h;//���ڴ�С

	//��ť��С��λ��
	int button_w = 300;
	int button_h = 60;
	int button_x = 900;
	int button_y[10] = { 50,120,190,260,330,500 };
	RECT b0 = { button_x,button_y[0],button_x + button_w,button_y[0] + button_h };
	RECT b1 = { button_x,button_y[1],button_x + button_w,button_y[1] + button_h };
	RECT b2 = { button_x,button_y[2],button_x + button_w,button_y[2] + button_h };
	RECT b3 = { button_x,button_y[3],button_x + button_w,button_y[3] + button_h };
	RECT b4 = { button_x,button_y[4],button_x + button_w,button_y[4] + button_h };
	RECT b5 = { button_x,button_y[5],button_x + button_w - 50,button_y[5] + button_h };
	RECT b6 = { button_x,button_y[6],button_x + button_w,button_y[6] + button_h };
	RECT b7 = { 300,240,600,340 };
	RECT b8 = { 300,360,600,460 };
	RECT Button[10] = { b0,b1,b2,b3,b4,b5,b6,b7,b8 };

	//��ϷUI��Ϣ
	struct GameInfo {
		int length;
		int hp;
		int score;
		int highestScore;
		clock_t startTime;
	}gameInfo;

public:
	//���캯�� m��������Ϸģʽ
	BaseGame(int m) {
		mode = m;
		window_w = 1200;
		window_h = 700;
		gameInfo.hp = mode == 3 ? 5 : 1;
		gameInfo.score = 0;
	}

	//����UI��Ϣ
	void DrawUI() {
		setbkmode(TRANSPARENT);
		int buttonNum = 6;
		setlinestyle(PS_SOLID, 3);
		setlinecolor(WHITE);
		settextstyle(36, 0, _T("����"), 0, 0, 1000, false, false, false);
		TCHAR s[50];
		_stprintf_s(s, _T("��Ϸʱ�� %d s"), (clock() - gameInfo.startTime) / 1000);
		drawtext(s, &Button[0], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("��ǰ���� %d"), gameInfo.length);
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
		_stprintf_s(s, _T("������Ϸ"));
		drawtext(s, &Button[5], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
		gameInfo.highestScore = HighScore[mode - 1];//��ȡ��ǰ��Ϸģʽ��߷�

		Map bigMap; //��ͼ����ʵ����
		Snake p1(bigMap.getMapInfo());//�߶���ʵ����
		gameInfo.length = p1.getLength();

		MOUSEMSG m{ 0 };

		//��ʼ������
		initgraph(window_w, window_h);
		loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);//��ȡ����ͼ	
		//��ʼ����
		BeginBatchDraw();
		bigMap.DrawMap();
		p1.DrawSnake();
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


		bool isPlaying = true;//��ʾ��Ϸ�Ƿ����
		gameInfo.startTime = clock();//��¼��Ϸ��ʼʱ��
		while (isPlaying) {
			//��û�м����¼�ʱ����ѭ��
			while (!_kbhit()) {
				//ÿ����һ��ѭ��������֡����
				for (int i = 0; i < 4; i++) {
					//�������Ƿ��ƶ�������������Ϸ��ť
					for (int i = 0; i < 5; i++) {
						if (MouseHit()) m = GetMouseMsg();
						switch (m.uMsg) {
						case WM_LBUTTONDOWN:
							if (m.x > Button[5].left&& m.x<Button[5].right && m.y>Button[5].top&& m.y < Button[5].bottom) {
								isPlaying = false;
							}//���������Ϸ��ť
							break;
						case WM_MOUSEMOVE:
							if (m.x > Button[5].left&& m.x<Button[5].right && m.y>Button[5].top&& m.y < Button[5].bottom) {
								setlinecolor(RED);
							}//�ƶ���������Ϸ��ť�ı���ɫ
							else {
								setlinecolor(WHITE);
							}
							break;
						}
						setlinestyle(PS_SOLID, 3);
						rectangle(Button[5].left, Button[5].top, Button[5].right, Button[5].bottom);
						FlushBatchDraw();
						Sleep(1);
					}
					if (!isPlaying) {
						break;
					}

					p1.Move();//�ƶ���
					int actIndex = p1.updateState(bigMap.getMapInfo(), bigMap.getNormalFood(), bigMap.getBigFood());//������Ƿ���������ײ
					//0-3��ʾײ��������
					if (p1.getState() > 0 && p1.getState() <= 3) {
						//���·���������						
						gameInfo.score -= p1.getState() * 10;
						if (mode == 1) {
							gameInfo.hp--;
							isPlaying = false;
						}//���Ű������Ϸ
						else if (mode == 2) {
							i = 1;
							vector<vector<int>> path = p1.getPath();
							bigMap.AddWall(path[0], path[1]);
							//�ռ䲻��
							if (!bigMap.getIsBlank() || p1.InitSnake(bigMap.getMapInfo()) != 0) {
								isPlaying = false;
							}
						}//���װ�����ǽ
						else if (mode == 3) {
							gameInfo.hp--;
							if (gameInfo.hp <= 0) {
								isPlaying = false;
							}
							i = 1;
							vector<vector<int>> path = p1.getPath();
							bigMap.AddFood(path[0], path[1], p1.getColor());
							p1.InitSnake(bigMap.getMapInfo());
						}//�߼�������ʳ��
					}
					else if (p1.getState() == 4) {
						gameInfo.score += 10;
						bigMap.DeleteFood(actIndex, 1);
					}//�Ե���ͨʳ��
					else if (p1.getState() == 5) {
						gameInfo.score += 50;
						bigMap.DeleteFood(actIndex, 2);
					}//�Ե����������ɵ�ʳ��
					p1.setState(0);

					//����
					cleardevice();
					loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
					bigMap.DrawMap();//ÿ֡���»��ƽ����˸������
					gameInfo.length = p1.getLength();
					setlinestyle(PS_SOLID, 3);
					rectangle(Button[5].left, Button[5].top, Button[5].right, Button[5].bottom);
					DrawUI();
					p1.DrawSnake();
					FlushBatchDraw();
				}
				if (!isPlaying) {
					break;
				}
			}
			if (!isPlaying) {
				break;
			}
			//��ⰴ�����޸ķ���
			char key1 = _getch();
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
			if (angleNext == 0 || angleNow == 0 || angleNext / abs(angleNext) != angleNow / abs(angleNow)) {
				if (abs(angleNext) + abs(angleNow) == 180) {
					angleNext = angleNow;
				}
			}
			p1.ChangeOrientation(angleNext);
		}

		//������ʷ��߷ֲ�д���ļ�
		if (gameInfo.score > HighScore[mode - 1]) {
			HighScore[mode - 1] = gameInfo.score;
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
		settextcolor(RGB(0, 0, 0));
		settextstyle(30, 0, _T("����"));
		TCHAR s[50];
		_stprintf_s(s, _T("Game Over\n\n���յ÷�: %d"), gameInfo.score);
		drawtext(s, &Button[7], DT_CENTER);
		drawtext(_T("���������Ϸ��ҳ"), &Button[8], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 5);
		rectangle(Button[8].left, Button[8].top, Button[8].right, Button[8].bottom);
		FlushBatchDraw();
		FlushBatchDraw();
		bool quit = false;
		while (!quit) {
			if (MouseHit()) m = GetMouseMsg();
			switch (m.uMsg) {
			case WM_LBUTTONDOWN:
				if (m.x > Button[8].left&& m.x<Button[8].right && m.y>Button[8].top&& m.y < Button[8].bottom) {
					quit = true;
				}
				break;
			case WM_MOUSEMOVE:
				if (m.x > Button[8].left&& m.x<Button[8].right && m.y>Button[8].top&& m.y < Button[8].bottom) {
					setlinecolor(RED);
				}
				else {
					setlinecolor(BLACK);
				}
			}
			setlinestyle(PS_SOLID, 5);
			rectangle(Button[8].left, Button[8].top, Button[8].right, Button[8].bottom);
			FlushBatchDraw();
		}
		EndBatchDraw();
		return gameInfo.score;//��Ϸ�������ص÷�
	}
};