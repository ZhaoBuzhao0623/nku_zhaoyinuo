#include<stdio.h>
#include<graphics.h>

#define WIN_WIDTH 900
#define WIN_HEIGHT 600

enum{WAN_DOU,XIANG_RI_KUI,ZHI_WU_COUNT};

IMAGE imgBg;
IMAGE imgBar;
IMAGE imgCards[ZHI_WU_COUNT];
IMAGE* imgZhiWu[ZHI_WU_COUNT][20];

int curX, curY;//植物移动过程
int curZhiWu;//0:没有选择，1：第一种植物

struct zhiwu {
	int type;
	int frameIndex;//序列帧的序号
};
struct zhiwu map[3][9];


//判断文件是否存在
bool fileExist(const char* name) {
	FILE* fp = fopen(name, "r");
	if (fp == NULL) {
		return false;
	}
	else {
		fclose(fp);
		return true;
	}
}

void gameInit() {
	loadimage(&imgBg, "sucai/bg.jpg");
	loadimage(&imgBar, "sucai/bar5.png");
	
	memset(imgZhiWu, 0, sizeof(imgZhiWu));
	memset(map, 0, sizeof(map));

	//初始化植物卡牌
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		sprintf_s(name, sizeof(name), "sucai/Cards/card_%d.png", i + 1);
		loadimage(&imgCards[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "sucai/zhiwu/%d/%d.png", i ,j+1);
			if (fileExist(name)) {
				//分配内存
				imgZhiWu[i][j] = new IMAGE;
				loadimage(imgZhiWu[i][j], name);
			}
			else {
				break;
			}
		}
	}
	curZhiWu = 0;
	//窗口,控制台
	initgraph(WIN_WIDTH, WIN_HEIGHT,1);

}

//渲染函数
void updateWindow() {
	//缓冲
	BeginBatchDraw();
	/*
	
	
	
	
	putimagePNG拿到tool后记得改！！！
	
	
	
	
	*/
	putimage(0, 0, &imgBg);
	putimage(250, 0, &imgBar);//卡牌栏

	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		int x = 338+ i * 65;
		int y = 6;
		putimage(x, y, &imgCards[i]);
	}

	if (curZhiWu > 0) {
		IMAGE* img = imgZhiWu[curZhiWu - 1][0];
		putimage(curX-img->getwidth()/2, curY-img->getheight()/2,img);
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {
				int x = 256 + j * 81;
				int y = 179 + i * 102+14;
				int zhiWuType = map[i][j].type - 1;
				int index = map[i][j].frameIndex;
				putimage(x, y, imgZhiWu[zhiWuType][index]);
			}
		}
	}

	//缓冲结束
	EndBatchDraw();

}

void userClick() {
	ExMessage msg;
	static int status = 0;

	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96) {
				int index = (msg.x - 338) / 65;
				status = 1;
				curZhiWu = index + 1;
			}
		}
		else if (msg.message == WM_MOUSEMOVE&&status==1) {
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP) {
			if (msg.x > 256 && msg.y > 179 && msg.y < 489) {

				int row = (msg.y - 179) / 102;
				int col = (msg.x - 256) / 81;
				
				if (map[row][col].type == 0) {
					map[row][col].type = curZhiWu;
					map[row][col].frameIndex = 0;
				}
			}
			curZhiWu = 0;
			status = 0;
		}
	}
}

int main(void) 
{
	gameInit();

	while (1) {
		userClick();

		updateWindow();
	}
	


	system("pause");
	return 0;
}