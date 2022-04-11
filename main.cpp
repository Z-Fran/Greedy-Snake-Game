#define _CRT_SECURE_NO_WARNINGS
#include "Client.h"
#include <conio.h>
#include<graphics.h>
#include<vector>
#include<fstream>
#include"BaseGame.h"
#include"AdvanceGame.h"
#include"OnlineGame.h"
using namespace std;

#define WINDOW_W 900
#define WINDOW_H 600

//������λ�úʹ�С
int button_w = 240;
int button_h = 40;
int button_x = WINDOW_W / 2 - button_w / 2;
int button_y[10] = { 200,250,300,350,400,450,500,550 };
RECT b0 = { button_x,button_y[0],button_x + button_w,button_y[0] + button_h };
RECT b1 = { button_x,button_y[1],button_x + button_w,button_y[1] + button_h };
RECT b2 = { button_x,button_y[2],button_x + button_w,button_y[2] + button_h };
RECT b3 = { button_x,button_y[3],button_x + button_w,button_y[3] + button_h };
RECT b4 = { button_x,button_y[4],button_x + button_w,button_y[4] + button_h };
RECT b5 = { button_x,button_y[5],button_x + button_w,button_y[5] + button_h };
RECT b6 = { button_x,button_y[6],button_x + button_w,button_y[6] + button_h };
RECT b7 = { button_x,button_y[7],button_x + button_w,button_y[7] + button_h };
RECT b8 = { button_x,button_y[8],button_x + button_w,button_y[8] + button_h };
RECT Button[10] = { b0,b1,b2,b3,b4,b5,b6,b7,b8 };


/****************************************
������choice�˵��ĵȼ�
����ֵ����ǰ�˵���ť����
���ܣ����Ʋ˵�
*****************************************/
int DrawMenu(int choice) {
	setbkmode(TRANSPARENT);
	//�˵�һ
	if (choice == 1) {
		int buttonNum = 8;
		setlinestyle(PS_SOLID, 3);
		setlinecolor(WHITE);
		for (int i = 0; i < buttonNum; i++) {
			roundrect(Button[i].left, Button[i].top, Button[i].right, Button[i].bottom, 50, 50);
		}
		settextstyle(36, 0, _T("����"));
		settextcolor(WHITE);
		drawtext(_T("�� �� ��"), &Button[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("�� �� ��"), &Button[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("�� �� ��"), &Button[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("�� �� ��"), &Button[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("���뵥��ģʽ"), &Button[4], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("����˫��ģʽ"), &Button[5], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��Ϸ��¼"), &Button[6], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("�˳���Ϸ"), &Button[7], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return buttonNum;
	}
	//�˵�2
	else if (choice == 2) {
		int buttonNum = 4;
		setlinestyle(PS_SOLID, 3);
		setlinecolor(WHITE);
		for (int i = 0; i < buttonNum; i++) {
			roundrect(Button[i].left, Button[i].top, Button[i].right, Button[i].bottom, 50, 50);
		}
		settextstyle(36, 0, _T("����"));
		settextcolor(WHITE);
		drawtext(_T("����ģʽ"), &Button[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("˫��ģʽ"), &Button[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("����ģʽ"), &Button[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��    ��"), &Button[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return buttonNum;
	}
	return 0;
}


/****************************************
���ܣ�ʵ����ʷ��¼����ɾ�Ĳ�
*****************************************/
void ShowScore() {
	struct Record {
		int mode;
		int score;
		char name[12];
	};
	vector<Record>records;
	//��ȡ�ļ�
	ifstream fScore("resource/score.log");
	if (!fScore) {
		TCHAR s[] = _T("��ȡ�ļ�ʧ�ܣ�");
		outtextxy(WINDOW_W / 2, WINDOW_H / 2, s);
		Sleep(2000);
		return;
	}
	while (!fScore.eof()) {
		Record r;
		fScore >> r.mode >> r.name >> r.score;
		records.push_back(r);
	}
	fScore.close();
	//ҳ��
	int pageAll = int((double)records.size() / 5 + 1);
	if (records.size() % 5 == 0)pageAll--;
	int pageNow = 1;
	int listnum = pageAll > 1 ? 5 : (int)records.size();
	RECT title[4] = { {0,0,150,100},{150,0,300,100},{300,0,650,100},{650,0,900,100} };
	RECT Line[5][5] = {
			{ {0,100,150,180},{150,100,300,180},{300,100,650,180},{700,100,750,180},{800,100,850,180} },
			{ {0,180,150,260},{150,180,300,260},{300,180,650,260},{700,180,750,260},{800,180,850,260} },
			{ {0,260,150,340},{150,260,300,340},{300,260,650,340},{700,260,750,340},{800,260,850,340} },
			{ {0,340,150,420},{150,340,300,420},{300,340,650,420},{700,340,750,420},{800,340,850,420} },
			{ {0,420,150,500},{150,420,300,500},{300,420,650,500},{700,420,750,500},{800,420,850,500} } };

	RECT Page[3] = { {0,500,300,550} ,{300,500,600,550} ,{600,500,900,550} };
	RECT Op[2] = { {0,550,450,600} ,{450,550,900,600} };
	TCHAR Mode[5][20] = { _T("���Ű�"),_T("���װ�"),_T("�߼���"),_T("AIģʽ"),_T("˫��ģʽ") };
	MOUSEMSG m{ 0 };
	BeginBatchDraw();
	setbkcolor(BROWN);
	settextcolor(WHITE);
	bool runflag = true;
	while (runflag) {
		//��ʾ��¼
		cleardevice();
		settextstyle(36, 0, _T("����"));
		drawtext(_T("��  ��"), &title[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��  ��"), &title[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��  ��  ��"), &title[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��    ��"), &title[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��  һ  ҳ"), &Page[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��  һ  ҳ"), &Page[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		TCHAR s[50];
		_stprintf_s(s, _T("%d / %d"), pageNow, pageAll);
		drawtext(s, &Page[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��    ��"), &Op[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��    ��"), &Op[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		settextstyle(20, 0, _T("����"));
		for (int i = 0; i < listnum; i++) {
			int index = (pageNow - 1) * 5 + i;
			drawtext(Mode[records[index].mode], &Line[i][0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			_stprintf_s(s, _T("%d"), records[index].score);
			drawtext(s, &Line[i][1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(_T("�޸�"), &Line[i][3], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(_T("ɾ��"), &Line[i][4], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			for (int j = 0; j < 12; j++) {
				s[j] = records[index].name[j];
			}
			drawtext(s, &Line[i][2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		setlinecolor(WHITE);
		rectangle(title[0].left, title[0].top, title[0].right, title[0].bottom);
		rectangle(title[1].left, title[1].top, title[1].right, title[1].bottom);
		rectangle(title[2].left, title[2].top, title[2].right, title[2].bottom);
		rectangle(title[3].left, title[3].top, title[3].right, title[3].bottom);
		rectangle(Page[0].left, Page[0].top, Page[0].right, Page[0].bottom);
		rectangle(Page[1].left, Page[1].top, Page[1].right, Page[1].bottom);
		rectangle(Page[2].left, Page[2].top, Page[2].right, Page[2].bottom);
		rectangle(Op[0].left, Op[0].top, Op[0].right, Op[0].bottom);
		rectangle(Op[1].left, Op[1].top, Op[1].right, Op[1].bottom);
		//�������¼�
		if (MouseHit()) m = GetMouseMsg();
		switch (m.uMsg) {
		case WM_LBUTTONDOWN:
			if (m.y < 500) {
				int c;//0�޸� 1ɾ��
				int i;
				for (i = 0; i < listnum; i++) {
					if (m.x > Line[i][3].left&& m.x<Line[i][3].right && m.y>Line[i][3].top&& m.y < Line[i][3].bottom) {
						c = 0;
						break;
					}
					else if (m.x > Line[i][4].left&& m.x<Line[i][4].right && m.y>Line[i][4].top&& m.y < Line[i][4].bottom) {
						c = 1;
						break;
					}
				}
				if (i == listnum)break;
				if (c == 0) {
					bool inputing = true;
					TCHAR UserName[11] = _T("\0");
					TCHAR s[50];
					int nameLen = 0;
					RECT inputBox = { WINDOW_W / 2 - 180,420,WINDOW_W / 2 + 200,470 };
					setbkmode(TRANSPARENT);
					int inputCursor = 0;
					int choice = 0;
					while (inputing) {
						while (!_kbhit()) {
							if (MouseHit()) m = GetMouseMsg();
							switch (m.uMsg) {
							case WM_LBUTTONDOWN:
								if (m.x > Button[6].left&& m.x<Button[6].right && m.y>Button[6].top&& m.y < Button[6].bottom) {
									if (nameLen > 0) {
										for (int j = 0; j < 11; j++) {
											records[(pageNow - 1) * 5 + i].name[j] = (char)UserName[j];
										}
										inputing = false;
									}
								}
								else if (m.x > Button[7].left&& m.x<Button[7].right && m.y>Button[7].top&& m.y < Button[7].bottom) {
									inputing = false;
								}
								break;
							case WM_MOUSEMOVE:
								if (m.x > Button[6].left&& m.x<Button[6].right && m.y>Button[6].top&& m.y < Button[6].bottom) {
									choice = 6;
								}
								else if (m.x > Button[7].left&& m.x<Button[7].right && m.y>Button[7].top&& m.y < Button[7].bottom) {
									choice = 7;
								}
								else {
									choice = 0;
								}
							}
							setlinestyle(PS_SOLID, 5);
							roundrect(Button[6].left, Button[6].top, Button[6].right, Button[6].bottom, 50, 50);
							roundrect(Button[7].left, Button[7].top, Button[7].right, Button[7].bottom, 50, 50);
							if (choice != 0) {
								setlinecolor(RED);
								roundrect(Button[choice].left, Button[choice].top, Button[choice].right, Button[choice].bottom, 50, 50);
							}
							setlinecolor(WHITE);
							settextstyle(36, 0, _T("����"));
							drawtext(_T("ȷ    ��"), &Button[6], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							drawtext(_T("ȡ    ��"), &Button[7], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							settextstyle(30, 0, _T("΢���ź�"));
							_stprintf_s(s, _T("�������û���(<10): %s"), UserName);
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
							if (!inputing) {
								break;
							}
						}
						if (!inputing) {
							break;
						}
						char c = _getch();
						if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9') {
							if (nameLen < 10) {
								UserName[nameLen++] = c;
								UserName[nameLen] = '\0';
							}
						}
						else if (c == 8) {
							if (nameLen > 0) {
								nameLen--;
								UserName[nameLen] = '\0';
							}
						}
					}
				}
				else if (c == 1) {
					records.erase(records.begin() + (pageNow - 1) * 5 + i);
					pageAll = int((double)records.size() / 5 + 1);	
					if (pageNow == pageAll) {
						listnum--;
					}
					if (records.size() % 5 == 0)pageAll--;
				}
			}
			else if (m.y < 550) {
				if (m.x > Page[0].left&& m.x<Page[0].right && m.y>Page[0].top&& m.y < Page[0].bottom) {
					if (pageNow > 1) {
						pageNow--;
					}
					listnum = 5;
				}
				else if (m.x > Page[2].left&& m.x<Page[2].right && m.y>Page[2].top&& m.y < Page[2].bottom) {
					if (pageNow < pageAll) {
						pageNow++;
					}
					if (pageNow == pageAll) {
						listnum = records.size() - 5 * (pageNow - 1);
					}
				}
			}
			else {
				if (m.x > Op[0].left&& m.x<Op[0].right && m.y>Op[0].top&& m.y < Op[0].bottom) {
					//����
					bool inputing = true;
					TCHAR UserName[11] = _T("\0");
					char userName[11] = "\0";
					int nameLen = 0;
					RECT inputBox = { 30,560,390,590 };
					while (inputing) {
						while (!_kbhit()) {
							cleardevice();
							settextstyle(36, 0, _T("����"));
							drawtext(_T("��  ��"), &title[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							drawtext(_T("��  ��"), &title[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							drawtext(_T("��  ��  ��"), &title[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							drawtext(_T("��    ��"), &title[3], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							drawtext(_T("��  һ  ҳ"), &Page[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							drawtext(_T("��  һ  ҳ"), &Page[2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							TCHAR s[50];
							_stprintf_s(s, _T("%d / %d"), pageNow, pageAll);
							drawtext(s, &Page[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							drawtext(_T("��    ��"), &Op[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							settextstyle(20, 0, _T("����"));
							for (int i = 0; i < listnum; i++) {
								int index = (pageNow - 1) * 5 + i;
								if (strcmp(userName, records[index].name) == 0) {
									setlinecolor(0X00FF00);
									rectangle(0, Line[i][0].top, 900, Line[i][0].bottom);
									settextcolor(0X00FF00);
								}
								drawtext(Mode[records[index].mode], &Line[i][0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								_stprintf_s(s, _T("%d"), records[index].score);
								drawtext(s, &Line[i][1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								drawtext(_T("�޸�"), &Line[i][3], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								drawtext(_T("ɾ��"), &Line[i][4], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								for (int j = 0; j < 12; j++) {
									s[j] = records[index].name[j];
								}
								drawtext(s, &Line[i][2], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								settextcolor(WHITE);
							}
							setlinecolor(WHITE);
							rectangle(title[0].left, title[0].top, title[0].right, title[0].bottom);
							rectangle(title[1].left, title[1].top, title[1].right, title[1].bottom);
							rectangle(title[2].left, title[2].top, title[2].right, title[2].bottom);
							rectangle(title[3].left, title[3].top, title[3].right, title[3].bottom);
							rectangle(Page[0].left, Page[0].top, Page[0].right, Page[0].bottom);
							rectangle(Page[1].left, Page[1].top, Page[1].right, Page[1].bottom);
							rectangle(Page[2].left, Page[2].top, Page[2].right, Page[2].bottom);
							rectangle(Op[0].left, Op[0].top, Op[0].right, Op[0].bottom);
							rectangle(Op[1].left, Op[1].top, Op[1].right, Op[1].bottom);
							settextstyle(36, 0, _T("����"));
							settextstyle(30, 0, _T("΢���ź�"));
							_stprintf_s(s, _T("�������û���(<10): %s"), UserName);
							drawtext(s, &inputBox, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
							
							if (MouseHit()) m = GetMouseMsg();
							switch (m.uMsg) {
							case WM_LBUTTONDOWN:
								if (m.y < 500) {
									int c;//0�޸� 1ɾ��
									int i;
									for (i = 0; i < listnum; i++) {
										if (m.x > Line[i][3].left&& m.x<Line[i][3].right && m.y>Line[i][3].top&& m.y < Line[i][3].bottom) {
											c = 0;
											break;
										}
										else if (m.x > Line[i][4].left&& m.x<Line[i][4].right && m.y>Line[i][4].top&& m.y < Line[i][4].bottom) {
											c = 1;
											break;
										}
									}
									if (i == listnum)break;
									if (c == 0) {
										bool inputing = true;
										TCHAR UserName[11] = _T("\0");
										TCHAR s[50];
										int nameLen = 0;
										RECT inputBox = { WINDOW_W / 2 - 180,420,WINDOW_W / 2 + 200,470 };
										setbkmode(TRANSPARENT);
										int inputCursor = 0;
										int choice = 0;
										while (inputing) {
											while (!_kbhit()) {
												if (MouseHit()) m = GetMouseMsg();
												switch (m.uMsg) {
												case WM_LBUTTONDOWN:
													if (m.x > Button[6].left&& m.x<Button[6].right && m.y>Button[6].top&& m.y < Button[6].bottom) {
														if (nameLen > 0) {
															for (int j = 0; j < 11; j++) {
																records[(pageNow - 1) * 5 + i].name[j] = (char)UserName[j];
															}
															inputing = false;
														}
													}
													else if (m.x > Button[7].left&& m.x<Button[7].right && m.y>Button[7].top&& m.y < Button[7].bottom) {
														inputing = false;
													}
													break;
												case WM_MOUSEMOVE:
													if (m.x > Button[6].left&& m.x<Button[6].right && m.y>Button[6].top&& m.y < Button[6].bottom) {
														choice = 6;
													}
													else if (m.x > Button[7].left&& m.x<Button[7].right && m.y>Button[7].top&& m.y < Button[7].bottom) {
														choice = 7;
													}
													else {
														choice = 0;
													}
												}
												setlinestyle(PS_SOLID, 5);
												roundrect(Button[6].left, Button[6].top, Button[6].right, Button[6].bottom, 50, 50);
												roundrect(Button[7].left, Button[7].top, Button[7].right, Button[7].bottom, 50, 50);
												if (choice != 0) {
													setlinecolor(RED);
													roundrect(Button[choice].left, Button[choice].top, Button[choice].right, Button[choice].bottom, 50, 50);
												}
												setlinecolor(WHITE);
												settextstyle(36, 0, _T("����"));
												drawtext(_T("ȷ    ��"), &Button[6], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
												drawtext(_T("ȡ    ��"), &Button[7], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
												settextstyle(30, 0, _T("΢���ź�"));
												_stprintf_s(s, _T("�������û���(<10): %s"), UserName);
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
												if (!inputing) {
													break;
												}
											}
											if (!inputing) {
												break;
											}
											char c = _getch();
											if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9') {
												if (nameLen < 10) {
													UserName[nameLen++] = c;
													UserName[nameLen] = '\0';
												}
											}
											else if (c == 8) {
												if (nameLen > 0) {
													nameLen--;
													UserName[nameLen] = '\0';
												}
											}
										}
									}
									else if (c == 1) {
										records.erase(records.begin() + (pageNow - 1) * 5 + i);
										pageAll = int((double)records.size() / 5 + 1);
										if (pageNow == pageAll) {
											listnum--;
										}
										if (records.size() % 5 == 0)pageAll--;
									}
								}
								else if (m.y < 550) {
									if (m.x > Page[0].left&& m.x<Page[0].right && m.y>Page[0].top&& m.y < Page[0].bottom) {
										if (pageNow > 1) {
											pageNow--;
										}
										listnum = 5;
									}
									else if (m.x > Page[2].left&& m.x<Page[2].right && m.y>Page[2].top&& m.y < Page[2].bottom) {
										if (pageNow < pageAll) {
											pageNow++;
										}
										if (pageNow == pageAll) {
											listnum = records.size() - 5 * (pageNow - 1);
										}
									}
								}
								else if (m.x > Op[1].left&& m.x<Op[1].right && m.y>Op[1].top&& m.y < Op[1].bottom) {
									inputing = false;
								}
								Sleep(300);
								break;
							}
							for (int i = 0; i < 3; i++) {
								if (m.x > Page[i].left&& m.x<Page[i].right && m.y>Page[i].top&& m.y < Page[i].bottom) {
									setlinecolor(RED);
									rectangle(Page[i].left, Page[i].top, Page[i].right, Page[i].bottom);
									break;
								}
							}
							for (int i = 0; i < 2; i++) {
								if (m.x > Op[i].left&& m.x<Op[i].right && m.y>Op[i].top&& m.y < Op[i].bottom) {
									setlinecolor(RED);
									rectangle(Op[i].left, Op[i].top, Op[i].right, Op[i].bottom);
									break;
								}
							}
							FlushBatchDraw();
							if (!inputing) {
								break;
							}
						}
						if (!inputing) {
							break;
						}
						char c = _getch();
						if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9') {
							if (nameLen < 10) {
								UserName[nameLen] = c;
								userName[nameLen] = c;
								nameLen++;
								UserName[nameLen] = '\0';								
								userName[nameLen] = '\0';
							}
						}
						else if (c == 8) {
							if (nameLen > 0) {
								nameLen--;
								UserName[nameLen] = '\0';
								userName[nameLen] = '\0';
							}
						}
					}
				}
				else if (m.x > Op[1].left&& m.x<Op[1].right && m.y>Op[1].top&& m.y < Op[1].bottom) {
					runflag = false;
				}
			}
			Sleep(300);
			break;
		}
		for (int i = 0; i < 3; i++) {
			if (m.x > Page[i].left&& m.x<Page[i].right && m.y>Page[i].top&& m.y < Page[i].bottom) {
				setlinecolor(RED);
				rectangle(Page[i].left, Page[i].top, Page[i].right, Page[i].bottom);
			}
		}
		for (int i = 0; i < 2; i++) {
			if (m.x > Op[i].left&& m.x<Op[i].right && m.y>Op[i].top&& m.y < Op[i].bottom) {
				setlinecolor(RED);
				rectangle(Op[i].left, Op[i].top, Op[i].right, Op[i].bottom);
				break;
			}
		}
		FlushBatchDraw();
	}
	//����д���ļ���
	ofstream fout("resource/score.log");
	if (!fout) {
		TCHAR s[] = _T("��ȡ�ļ�ʧ�ܣ�");
		outtextxy(WINDOW_W / 2, WINDOW_H / 2, s);
		Sleep(2000);
		return;
	}
	for (unsigned int i = 0; i < records.size(); i++) {
		fout << records[i].mode << " " << records[i].name << " " << records[i].score << endl;
	}
	fout.close();
}


int main() {
	//��ʼ��ͼ�ν���
	initgraph(WINDOW_W, WINDOW_H);
	loadimage(NULL, _T("resource/1.jpg"), WINDOW_W, WINDOW_H);
	MOUSEMSG m{ 0 };
	BeginBatchDraw();

	//�����û����Ĺ���
	bool inputing = true;
	TCHAR UserName[11] = _T("\0");
	TCHAR s[50];
	int nameLen = 0;
	RECT inputBox = { WINDOW_W / 2 - 180,420,WINDOW_W / 2 + 200,470 };
	setbkmode(TRANSPARENT);
	int inputCursor = 0;
	int choice = 0;
	while (inputing) {
		while (!_kbhit()) {
			if (MouseHit()) m = GetMouseMsg();
			switch (m.uMsg) {
			case WM_LBUTTONDOWN:
				if (m.x > Button[6].left&& m.x<Button[6].right && m.y>Button[6].top&& m.y < Button[6].bottom) {
					if (nameLen > 0) {
						inputing = false;
					}
				}
				else if (m.x > Button[7].left&& m.x<Button[7].right && m.y>Button[7].top&& m.y < Button[7].bottom) {
					closegraph();
					return 0;
				}
				break;
			case WM_MOUSEMOVE:
				if (m.x > Button[6].left&& m.x<Button[6].right && m.y>Button[6].top&& m.y < Button[6].bottom) {
					choice = 6;
				}
				else if (m.x > Button[7].left&& m.x<Button[7].right && m.y>Button[7].top&& m.y < Button[7].bottom) {
					choice = 7;
				}
				else {
					choice = 0;
				}
			}
			setlinestyle(PS_SOLID, 5);
			roundrect(Button[6].left, Button[6].top, Button[6].right, Button[6].bottom, 50, 50);
			roundrect(Button[7].left, Button[7].top, Button[7].right, Button[7].bottom, 50, 50);
			if (choice != 0) {
				setlinecolor(RED);
				roundrect(Button[choice].left, Button[choice].top, Button[choice].right, Button[choice].bottom, 50, 50);
			}
			setlinecolor(WHITE);
			settextstyle(36, 0, _T("����"));
			drawtext(_T("��    ��"), &Button[6], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			drawtext(_T("��    ��"), &Button[7], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextstyle(30, 0, _T("΢���ź�"));
			_stprintf_s(s, _T("�������û���(<10): %s"), UserName);
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
			loadimage(NULL, _T("resource/1.jpg"), WINDOW_W, WINDOW_H);
			if (!inputing) {
				break;
			}
		}
		if (!inputing) {
			break;
		}
		char c = _getch();
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9') {
			if (nameLen < 10) {
				UserName[nameLen++] = c;
				UserName[nameLen] = '\0';
			}
		}
		else if (c == 8) {
			if (nameLen > 0) {
				nameLen--;
				UserName[nameLen] = '\0';
			}
		}
	}
	Sleep(100);
	char userName[11];
	for (int i = 0; i < 11; i++) {
		userName[i] = (char)UserName[i];
	}
	userName[10] = 0;

	//����������
	int menuType = 1;
	int buttonNum = DrawMenu(menuType);
	while (1) {
		BeginBatchDraw();
		cleardevice();
		Resize(NULL, WINDOW_W, WINDOW_H);
		loadimage(NULL, _T("resource/1.jpg"), WINDOW_W, WINDOW_H);
		buttonNum = DrawMenu(menuType);
		//�������¼�
		if (MouseHit()) m = GetMouseMsg();
		int i;
		switch (m.uMsg) {
		case WM_LBUTTONDOWN:
			EndBatchDraw();
			for (i = 0; i < buttonNum; i++) {
				if (m.x > Button[i].left&& m.x<Button[i].right && m.y>Button[i].top&& m.y < Button[i].bottom) {
					break;
				}
			}
			if (i < buttonNum) {
				//������
				if (menuType == 1) {
					if (i <= 2) {
						BaseGame game(i + 1);
						int score = game.GameRun();
						//д�����
						ofstream foutScore("resource/score.log", ios::app);
						if (!foutScore) {
							cerr << "�򿪷�����¼�ļ�ʧ�ܣ�" << endl;
						}
						else {
							foutScore << i << " " << userName << " " << score << endl;
							foutScore.close();
						}
					}
					else if (i == 3) {
						menuType = 2;//����ӷְ����
					}
					else if (i == 4) {
						SingleGame game(1);
						int score = game.GameRun();
						//д�����
						ofstream foutScore("resource/score.log", ios::app);
						if (!foutScore) {
							cerr << "�򿪷�����¼�ļ�ʧ�ܣ�" << endl;
						}
						else {
							foutScore << 3 << " " << userName << " " << score << endl;
							foutScore.close();
						}
					}
					else if (i == 5) {
						DoubleGame game(1);
						int score = game.GameRun();
						//д�����
						ofstream foutScore("resource/score.log", ios::app);
						if (!foutScore) {
							cerr << "�򿪷�����¼�ļ�ʧ�ܣ�" << endl;
						}
						else {
							foutScore << 4 << " " << userName << " " << score << endl;
							foutScore.close();
						}
					}
					else if (i == 6) {
						ShowScore();//��ʷ��¼
					}
					else if (i == 7) {
						closegraph();  //�˳�ͼ�ν���
						return 0;
					}
					Sleep(100);
				}
				//�ӷְ����
				else if (menuType == 2) {
					if (i == 0) {
						//������Ϸ
						SingleGame game(0);
						int score = game.GameRun();
						//д�����
						ofstream foutScore("resource/score.log", ios::app);
						if (!foutScore) {
							cerr << "�򿪷�����¼�ļ�ʧ�ܣ�" << endl;
						}
						else {
							foutScore << 3 << " " << userName << " " << score << endl;
							foutScore.close();
						}
					}
					else if (i == 1) {
						//˫����Ϸ
						DoubleGame game(0);
						int score = game.GameRun();
						//д�����
						ofstream foutScore("resource/score.log", ios::app);
						if (!foutScore) {
							cerr << "�򿪷�����¼�ļ�ʧ�ܣ�" << endl;
						}
						else {
							foutScore << 4 << " " << userName << " " << score << endl;
							foutScore.close();
						}
					}
					else if (i == 2) {
						//������Ϸ
						MultiGame game(userName);
						game.ChooseRoom();
					}
					else if (i == 3) {
						//����������
						menuType = 1;
					}
					Sleep(100);
				}
			}
			break;
		case WM_MOUSEMOVE:
			for (i = 0; i < buttonNum; i++) {
				if (m.x > Button[i].left&& m.x<Button[i].right && m.y>Button[i].top&& m.y < Button[i].bottom) {
					setlinecolor(RED);
					setlinestyle(PS_SOLID, 3);
					roundrect(Button[i].left, Button[i].top, Button[i].right, Button[i].bottom, 50, 50);
					break;
				}
			}
			if (i == buttonNum) {
				buttonNum = DrawMenu(menuType);
			}
			break;
		}
		FlushBatchDraw();
	}
	closegraph(); //�˳�ͼ�ν���
	return 0;
}


