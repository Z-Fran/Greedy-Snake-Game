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
#define WINDOW_W 900 //窗口宽
#define WINDOW_H 600 //窗口高
//用于生成随机颜色
DWORD COLORSET[30] = { 0X808080,0X5C5CCD,0X8080F0,0X4763FF,0X507FFF,0X008CFF,0XADDEFF,0X00FFFF,0X2FFFAD,0X00FF00,
					0X7FFF00,0XD0E040,0XEEEEAF,0XFACE87,0XE16941,0XFF901E,0X00D7FF,0XD30094,0XDDA0DD,0XC1B6FF };

/****************************************
* 蛇类
*****************************************/
class Snake {
private:
	int length; //长度
	DWORD color;//颜色
	int radius; //半径
	double angle;//蛇头方向
	int state;//蛇的状态
	int speed;//蛇速度
	int frame;//帧数
	int frame_now;
	vector<int> pathX;//蛇节点当前的x坐标
	vector<int> pathY;//蛇节点当前的y坐标
	vector<int> pathX_nextframe;//蛇节点下一帧x坐标
	vector<int> pathY_nextframe;//蛇节点下一帧y坐标
	vector<int> pathX_lastframe;//蛇节点上一帧x坐标
	vector<int> pathY_lastframe;//蛇节点上一帧y坐标

public:
	//构造函数
	Snake(vector<vector<int>> wall) {
		InitSnake(wall);
	}
	/****************************************
	参数：wall墙的坐标
	返回值：是否成功
	功能：初始化一条蛇
	*****************************************/
	int InitSnake(vector<vector<int>> wall) {
		srand((unsigned)time(NULL));
		//设置蛇的相关属性
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

		//随机生成蛇头坐标
		int headX, headY;
		srand((unsigned)time(NULL));
		bool flag = true;
		int times = 5000;//最多产生5000次
		while (flag && times) {
			flag = false;
			times--;
			//在地图边界范围内生成坐标
			headX = rand() % (wall[0][2] - wall[0][0] - 60) + 30 + wall[0][0];
			headY = rand() % (wall[0][3] - wall[0][1] - 60) + 30 + wall[0][1];
			//判断生成的坐标是否撞到墙
			unsigned int i;
			for (i = 0; i < wall[1].size(); i++) {
				if (pow(headX - wall[1][i], 2) + pow(headY - wall[2][i], 2) < pow(radius + 5, 2)) {
					flag = true;
					break;
				}
			}
		}
		//生成5000次都撞到墙则生成失败
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
	
	//获取蛇长度
	int getLength() {
		return length;
	}
	//获取蛇状态
	int getState() {
		return state;
	}
	//获取蛇头方向
	int getAngle() {
		return (int)angle;
	}
	//获取蛇的颜色
	DWORD getColor() {
		return color;
	}
	//获取蛇节点坐标
	vector<vector<int>> getPath() {
		vector<vector<int>> vec = { pathX,pathY };
		return vec;
	}
	//设置蛇状态
	void setState(int a) {
		state = a;
	}
	//设置蛇头方向
	void ChangeOrientation(int newAngle) {
		angle = newAngle;
		frame_now = 0;
	}
	
	/****************************************
	功能：绘制蛇
	*****************************************/
	void DrawSnake() {
		int n = (int)pathX.size();
		setfillcolor(color);
		//蛇节点
		for (int i = 0; i < n; i++) {
			solidcircle(pathX[i], pathY[i], radius);
		}
		//蛇眼
		setfillcolor(BLACK);
		solidcircle(pathX[0] + int((radius - 1) * cos((angle - 45) / 180 * PI)), pathY[0] + int((radius - 1) * sin((angle - 45) / 180 * PI)), 4);
		solidcircle(pathX[0] + int((radius - 1) * cos((angle + 45) / 180 * PI)), pathY[0] + int((radius - 1) * sin((angle + 45) / 180 * PI)), 4);
	}

	/****************************************
	功能：移动蛇
	*****************************************/
	void Move() {
			//绘制完4帧重新开始
			if (frame_now == 0) {
				pathX.clear();
				pathY.clear();
				pathX.insert(pathX.begin(), pathX_nextframe.begin(), pathX_nextframe.end());
				pathY.insert(pathY.begin(), pathY_nextframe.begin(), pathY_nextframe.end());
				//蛇的长度增加时添加节点
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
			frame_now = (frame_now + 1) % frame;//更新当前帧数
	}

	/****************************************
	参数：mapInfo边界和墙的坐标 normalFood普通食物坐标
		  bigFood蛇死后生成食物的坐标
	返回值：撞到物体的索引
	功能：检查蛇是否撞到相关物体
	*****************************************/
	int updateState(vector<vector<int>> mapInfo, vector<vector<int>>normalFood, vector<vector<int>>bigFood) {
		//撞上边界state为1
		int xMin = mapInfo[0][0], yMin = mapInfo[0][1], xMax = mapInfo[0][2], yMax = mapInfo[0][3];
		if (pathX[0] - radius<xMin || pathY[0] - radius<yMin ||
			pathX[0] + radius>xMax || pathY[0] + radius>yMax) {
			state = 1;
			return 0;
		}
		////撞上自己state为2
		//i=3开始检查解决一生成就死的问题
		for (unsigned int i = 3; i < pathX.size(); i++) {
			if (pow(pathX[0] - pathX[i], 2) + pow(pathY[0] - pathY[i], 2) < pow(2 * radius, 2)) {
				state = 2;
				return i;
			}
		}
		//撞上墙state为3
		for (unsigned int i = 0; i < mapInfo[1].size(); i++) {
			if (pow(pathX[0] - mapInfo[1][i], 2) + pow(pathY[0] - mapInfo[2][i], 2) < pow(radius + 5, 2)) {
				state = 3;
				return i;
			}
		}
		//吃到普通食物为state为4
		for (unsigned int i = 0; i < normalFood[0].size(); i++) {
			if (pow(pathX[0] - normalFood[0][i], 2) + pow(pathY[0] - normalFood[1][i], 2) < pow(radius + 3, 2)) {
				state = 4;
				length += 1;
				return i;
			}
		}
		//吃到大食物state为5
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
* 地图类
*****************************************/
class Map {
private:
	DWORD fillColor; //地图背景色
	int xMin, yMin, xMax, yMax;//地图范围
	int mapWidth, mapHeight; //地图宽度和高度

	vector<int> X_Wall, Y_Wall;//墙的坐标
	DWORD wallColor;//墙的颜色
	int wallSize;//墙的尺寸

	vector<int> X_normalFood, Y_normalFood;//普通食物坐标
	vector<DWORD> C_normalFood;//普通食物颜色
	int N_normalFood;//普通食物数量
	int R_normalFood;//普通食物半径
	//蛇死后生成的食物
	vector<int> X_bigFood, Y_bigFood;
	vector<DWORD> C_bigFood;
	int R_bigFood;

	bool isBlank;//地图空间是否足够标志

public:
	//构造函数
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
	//获取普通食物坐标
	vector<vector<int>> getNormalFood() {
		vector<vector<int>> vec;
		vec.push_back(X_normalFood);
		vec.push_back(Y_normalFood);
		return vec;
	}
	//获取蛇生成食物坐标
	vector<vector<int>> getBigFood() {
		vector<vector<int>> vec;
		vec.push_back(X_bigFood);
		vec.push_back(Y_bigFood);
		return vec;
	}
	//获取地图边界和墙坐标
	vector<vector<int>> getMapInfo() {
		int info[4] = { xMin,yMin,xMax,yMax };
		vector<int> border(info, info + 4);
		vector<vector<int>> mapinfo = { border,X_Wall,Y_Wall };
		return mapinfo;
	}
	//检查地图是否有空间剩余
	bool getIsBlank() {
		if (wallSize * wallSize * ((int)X_Wall.size() + 3) > (xMax - xMin)* (yMax - yMin)) {
			isBlank = false;
		}
		return isBlank;
	}

	/****************************************
	功能：初始化食物
	*****************************************/
	void InitFood() {
		srand((unsigned)time(NULL));
		int x, y;
		for (int i = 0; i < 5; i++) {
			bool flag = true;
			int times = 5000;//最多生成次数
			while (flag && times) {
				flag = false;
				times--;
				x = rand() % (xMax - xMin - 20) + 10 + xMin;
				y = rand() % (yMax - yMin - 20) + 10 + yMin;
				unsigned int j;
				for (j = 0; j < X_Wall.size(); j++) {
					if (pow(x - X_Wall[j], 2) + pow(y - Y_Wall[j], 2) < pow(R_normalFood + wallSize, 2)) {
						flag = true;
						break;//撞到墙重新生成
					}
				}
			}
			if (times == 0) {
				isBlank = false;
				N_normalFood = (int)X_normalFood.size();
				return;//生成失败，更新食物数量
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
	参数：index食物索引 foodType食物类型
	功能：删除指定食物
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
	参数：x，y死亡蛇节点坐标 color蛇颜色
	功能：蛇死亡后添加食物
	*****************************************/
	void AddFood(vector<int> x, vector<int> y, DWORD color) {
		X_bigFood.insert(X_bigFood.begin(), x.begin() + 1, x.end());
		Y_bigFood.insert(Y_bigFood.begin(), y.begin() + 1, y.end());
		C_bigFood.insert(C_bigFood.begin(), x.size() - 1, color);
	}

	/****************************************
	参数：x，y死亡蛇节点坐标
	功能：蛇死亡后添加墙
	*****************************************/
	void AddWall(vector<int> x, vector<int> y) {
		X_Wall.insert(X_Wall.begin(), x.begin(), x.end());
		Y_Wall.insert(Y_Wall.begin(), y.begin(), y.end());
	}

	//绘制食物
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
	//绘制墙
	void DrawWall() {
		int n = (int)X_Wall.size();
		setfillcolor(wallColor);
		for (int i = 0; i < n; i++) {
			solidrectangle(X_Wall[i] - wallSize, Y_Wall[i] - wallSize, X_Wall[i] + wallSize, Y_Wall[i] + wallSize);
		}
	}
	//绘制地图
	void DrawMap() {
		setfillcolor(fillColor);
		solidrectangle(xMin, yMin, xMax, yMax);
		DrawFood();
		DrawWall();
	}
};



/****************************************
* 游戏控制类
*****************************************/
class BaseGame {
private:
	int mode;//游戏模式(入门版1，进阶版2，高级版3)
	int window_w, window_h;//窗口大小

	//按钮大小与位置
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

	//游戏UI信息
	struct GameInfo {
		int length;
		int hp;
		int score;
		int highestScore;
		clock_t startTime;
	}gameInfo;

public:
	//构造函数 m：设置游戏模式
	BaseGame(int m) {
		mode = m;
		window_w = 1200;
		window_h = 700;
		gameInfo.hp = mode == 3 ? 5 : 1;
		gameInfo.score = 0;
	}

	//绘制UI信息
	void DrawUI() {
		setbkmode(TRANSPARENT);
		int buttonNum = 6;
		setlinestyle(PS_SOLID, 3);
		setlinecolor(WHITE);
		settextstyle(36, 0, _T("楷体"), 0, 0, 1000, false, false, false);
		TCHAR s[50];
		_stprintf_s(s, _T("游戏时间 %d s"), (clock() - gameInfo.startTime) / 1000);
		drawtext(s, &Button[0], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("当前长度 %d"), gameInfo.length);
		drawtext(s, &Button[1], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("剩余生命 %d"), gameInfo.hp);
		drawtext(s, &Button[2], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("当前得分 %d"), gameInfo.score);
		drawtext(s, &Button[3], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("最高得分 %d"), gameInfo.highestScore);
		if (gameInfo.score > gameInfo.highestScore) {
			_stprintf_s(s, _T("最高得分 %d"), gameInfo.score);
		}
		drawtext(s, &Button[4], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		_stprintf_s(s, _T("结束游戏"));
		drawtext(s, &Button[5], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	/****************************************
	返回值：得分
	功能：游戏运行与控制
	*****************************************/
	int GameRun() {
		//从文件中读取最高分记录
		int HighScore[5];
		ifstream fhscore("resource/HighestScore.log");
		if (!fhscore) {
			for (int i = 0; i < 5; i++) {
				HighScore[i] = 0;
			}
		}//打开文件失败的情况
		else {
			for (int i = 0; i < 5; i++) {
				fhscore >> HighScore[i];
			}
			fhscore.close();
		}
		gameInfo.highestScore = HighScore[mode - 1];//获取当前游戏模式最高分

		Map bigMap; //地图对象实例化
		Snake p1(bigMap.getMapInfo());//蛇对象实例化
		gameInfo.length = p1.getLength();

		MOUSEMSG m{ 0 };

		//初始化窗口
		initgraph(window_w, window_h);
		loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);//读取背景图	
		//初始绘制
		BeginBatchDraw();
		bigMap.DrawMap();
		p1.DrawSnake();
		DrawUI();
		FlushBatchDraw();
		//ReadyGO显示
		for (int i = 0; i < 256; i++) {
			settextcolor(RGB(i, i, i));
			settextstyle(100, 0, _T("楷体"));
			drawtext(_T("READY"), &Button[7], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(_T("GO!"), &Button[8], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			Sleep(2);
		}


		bool isPlaying = true;//表示游戏是否结束
		gameInfo.startTime = clock();//记录游戏开始时间
		while (isPlaying) {
			//当没有键盘事件时进入循环
			while (!_kbhit()) {
				//每进入一次循环绘制四帧画面
				for (int i = 0; i < 4; i++) {
					//检测鼠标是否移动到或点击结束游戏按钮
					for (int i = 0; i < 5; i++) {
						if (MouseHit()) m = GetMouseMsg();
						switch (m.uMsg) {
						case WM_LBUTTONDOWN:
							if (m.x > Button[5].left&& m.x<Button[5].right && m.y>Button[5].top&& m.y < Button[5].bottom) {
								isPlaying = false;
							}//点击结束游戏按钮
							break;
						case WM_MOUSEMOVE:
							if (m.x > Button[5].left&& m.x<Button[5].right && m.y>Button[5].top&& m.y < Button[5].bottom) {
								setlinecolor(RED);
							}//移动到结束游戏按钮改变颜色
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

					p1.Move();//移动蛇
					int actIndex = p1.updateState(bigMap.getMapInfo(), bigMap.getNormalFood(), bigMap.getBigFood());//检查蛇是否与物体相撞
					//0-3表示撞上了物体
					if (p1.getState() > 0 && p1.getState() <= 3) {
						//更新分数和命数						
						gameInfo.score -= p1.getState() * 10;
						if (mode == 1) {
							gameInfo.hp--;
							isPlaying = false;
						}//入门版结束游戏
						else if (mode == 2) {
							i = 1;
							vector<vector<int>> path = p1.getPath();
							bigMap.AddWall(path[0], path[1]);
							//空间不足
							if (!bigMap.getIsBlank() || p1.InitSnake(bigMap.getMapInfo()) != 0) {
								isPlaying = false;
							}
						}//进阶版生成墙
						else if (mode == 3) {
							gameInfo.hp--;
							if (gameInfo.hp <= 0) {
								isPlaying = false;
							}
							i = 1;
							vector<vector<int>> path = p1.getPath();
							bigMap.AddFood(path[0], path[1], p1.getColor());
							p1.InitSnake(bigMap.getMapInfo());
						}//高级版生成食物
					}
					else if (p1.getState() == 4) {
						gameInfo.score += 10;
						bigMap.DeleteFood(actIndex, 1);
					}//吃到普通食物
					else if (p1.getState() == 5) {
						gameInfo.score += 50;
						bigMap.DeleteFood(actIndex, 2);
					}//吃到蛇死后生成的食物
					p1.setState(0);

					//绘制
					cleardevice();
					loadimage(NULL, _T("resource/2.jpg"), window_w, window_h);
					bigMap.DrawMap();//每帧重新绘制解决闪烁的问题
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
			//检测按键并修改方向
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

		//更新历史最高分并写入文件
		if (gameInfo.score > HighScore[mode - 1]) {
			HighScore[mode - 1] = gameInfo.score;
		}
		ofstream fhscore_out("resource/HighestScore.log");
		if (!fhscore_out) {
			cerr << "输出最高分错误!" << endl;
		}
		else {
			for (int i = 0; i < 5; i++) {
				fhscore_out << HighScore[i];
				fhscore_out.put('\n');
			}
			fhscore_out.close();
		}

		//GameOver显示
		settextcolor(RGB(0, 0, 0));
		settextstyle(30, 0, _T("楷体"));
		TCHAR s[50];
		_stprintf_s(s, _T("Game Over\n\n最终得分: %d"), gameInfo.score);
		drawtext(s, &Button[7], DT_CENTER);
		drawtext(_T("点击返回游戏主页"), &Button[8], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
		return gameInfo.score;//游戏结束返回得分
	}
};