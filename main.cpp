#include <stdio.h>
#include <conio.h>
#include "pentis.h"
#include "cmd.h"
#include <imm.h>
using namespace std;
const char *NAME="Pentis",*VERSION="1.0";

int main() {
	bool back=false,ishold=false,hashold=false,hasdrop=false,willdrop=false,willshadow=false,temp_yn,**board;
	const short DEFAULT_Y=-1;
	const unsigned int SPEED_DEFAULT=1000,SPEED_MIN=700,SPEED_DEC=10,PRESS_INTERVAL=10;
	unsigned int score=0,choice,speed,temp_a;
	unsigned long t_now,t_start;
	COORD fsize,center,active[5],hold[5],next[5],temp_center,temp[5],shadow;
	char *menu0[]={
		"1. 开始游戏",
		"2. 帮助",
		"3. 设置",
		"4. 关于作者",
		"5. 退出"
	}; 
	if (!SetConsoleOutputCP(936)) {
		puts("Error: Your computer does not have Simplified-Chinese codepage.\n");
		return -1;
	}
	initialcmd();
	title("");
	color(0x0f);
	printf("加载中...");
	fsize.X=18;fsize.Y=30;
	board=new bool*[fsize.X];
	for (int i=0;i<fsize.X;++i) board[i]=new bool[fsize.Y];
	setcs(false,-1);
	
	menu:
	windowsize(17,8);
	centerwindow(); 
	cls();
	printf("%s V%s\n!!请关闭输入法!!\n",NAME,VERSION);
	fflush(stdin);
	choice=menu(5,menu0,1,0x0e);
	switch(choice) {
	case 1:
		goto play;
	case 2:
		goto help;
	case 3:
		goto option;
	case 4:
		goto about;
	case 5:
		goto exit;
	default:
		goto menu;
	}
	
	
	play:
	score=0;
	windowsize((fsize.X+10)*2,fsize.Y+2);
	//Sleep(100);
	RedrawWindow(GetConsoleWindow(),NULL,NULL,RDW_UPDATENOW);//TODO 重新开始无法重绘
	centerwindow();
	cls();
	for (int i=0;i<fsize.X;++i) for (int j=0;j<fsize.Y;++j) board[i][j]=false;
	refresh(fsize,board);
	if (!drawUI(fsize)) {
		printf("Error:height must be at least 24.\nPress any key...");
		getk();
		goto menu;
	}
	drawScore(fsize,score);
	clearHold(fsize);
	spawn(next);
	speed=SPEED_DEFAULT;//游戏速度
	
	loop0://新方块
	willshadow=false;
	hashold=false,hasdrop=false;
	for (int i=0;i<5;++i) active[i]=next[i];
	srand(GetTickCount());
	spawn(next);
	clearNext(fsize);
	drawNext(fsize,next);
	center.X=(short)(fsize.X/2);
	center.Y=DEFAULT_Y;
	shadow=center;
	while (drop(fsize,board,shadow,active)) {}
	drawBlock(fsize,shadow,active,true,0x06);
	//clearShadow(fsize);
	drawBlock(fsize,center,active,true);
	//drawShadow(fsize,center,active);
	t_start=GetTickCount();
	loop1://继续移动直到沉底
	willdrop=false;
	if (!kbhit()) goto skipgetk;
	choice=getk();
	temp_center=center;
	for (int i=0;i<5;++i) temp[i]=active[i];
	switch(choice) {
	case 72://up,hold
		if (hashold) break;//只能换一次
		drawBlock(fsize,shadow,temp,false);
		drawBlock(fsize,temp_center,temp,false);
		if (ishold) {
			hashold=true;
			for (int i=0;i<5;++i) {temp[i]=hold[i];hold[i]=active[i];active[i]=temp[i];}
			center.X=(short)(fsize.X/2);
			center.Y=DEFAULT_Y;
			clearHold(fsize);
			drawHold(fsize,hold);
			willshadow=true;
		} else {
			ishold=true;
			for (int i=0;i<5;++i) hold[i]=active[i];
			drawHold(fsize,hold);
			goto loop0;
		}
		t_start=GetTickCount();
		break;
	case 32://space
	case 115://S,到底 
		while (drop(fsize,board,center,active)) {}
		hasdrop=true;
		break;
	case 75://left
		back=false;
		for (int i=0;i<5;++i) {
			if (center.X+active[i].X-1<0) {back=true;break;}
			if (center.Y+active[i].Y<0) continue;
			if (board[center.X+active[i].X-1][center.Y+active[i].Y]) {back=true;break;}
		}
		if (back) break;
		--center.X;
		willshadow=true;
		break;
	case 77://right
		back=false;
		for (int i=0;i<5;++i) {
			if (center.X+active[i].X+1>fsize.X-1) {back=true;break;}
			if (center.Y+active[i].Y<0) continue;
			if (board[center.X+active[i].X+1][center.Y+active[i].Y]) {back=true;break;}
		}
		if (back) break;
		++center.X;
		willshadow=true;
		break;
	case 100://D,顺时针旋转
		for (int i=0;i<5;++i) {
			temp_a=active[i].Y;
			active[i].Y=active[i].X;
			active[i].X=-temp_a;
		}
		while (pushleft(fsize,board,center,active)) {}
		while (pushright(fsize,board,center,active)) {}
		while (pushup(board,center,active)) {}
		willshadow=true;
		break;
	case 97://A,逆时针旋转
		for (int i=0;i<5;++i) {
			temp_a=active[i].X;
			active[i].X=active[i].Y;
			active[i].Y=-temp_a;
		}
		while (pushleft(fsize,board,center,active)) {}
		while (pushright(fsize,board,center,active)) {}
		while (pushup(board,center,active)) {}
		willshadow=true;
		break;
	case 80://down,下降一格
		willdrop=true;
		break;
	case 27://esc
	case 112://P,暂停
		goto pause;
	}
	if (willshadow) {
		drawBlock(fsize,shadow,temp,false);
		shadow=center;
		while (drop(fsize,board,shadow,active)) {}
		drawBlock(fsize,shadow,active,true,0x06);
	}
	drawBlock(fsize,temp_center,temp,false);
	drawBlock(fsize,center,active,true);
	skipgetk:
	t_now=GetTickCount();
	if (hasdrop) {
		drawBlock(fsize,center,active,true);
		if (!solify(board,center,active)) goto die;
		refresh(fsize,board);
		goto loop0;
	}
	if (willdrop||t_now-t_start>speed) {
		drawBlock(fsize,center,active,false);//-l
		if (!drop(fsize,board,center,active)) {
			drawBlock(fsize,center,active,true);
			if (!solify(board,center,active)) goto die;
			refresh(fsize,board);
			goto loop0;
		} else {
			drawBlock(fsize,center,active,true);
			t_start=GetTickCount();
		}
	}
	for (int a=fsize.Y-1;a>0;--a) {
		temp_yn=false;
		for (int i=0;i<fsize.X;++i) if (!board[i][a]) {temp_yn=true;break;}
		if (temp_yn) continue;
		score+=1;
		drawScore(fsize,score);
		for (int j=a;j>0;--j) for (int i=0;i<fsize.X;++i) board[i][j]=board[i][j-1];
		refresh(fsize,board);
		shadow=center;
		while (drop(fsize,board,shadow,active)) {}
		drawBlock(fsize,shadow,active,true,0x06);
		drawBlock(fsize,center,active,true);
		if (speed>SPEED_MIN) speed-=SPEED_DEC;
		break;
	}
	willshadow=false;
	goto loop1;
	
	pause:
	temp_center.X=(fsize.X+2)*2;
	temp_center.Y=20;
	WriteConsoleOutputCharacter(hConsole,"    暂停中    ",14,temp_center,&cCharsWritten);
	++temp_center.Y;
	WriteConsoleOutputCharacter(hConsole," P 继续       ",14,temp_center,&cCharsWritten);
	++temp_center.Y;
	WriteConsoleOutputCharacter(hConsole," R 重新开始   ",14,temp_center,&cCharsWritten);
	++temp_center.Y;
	WriteConsoleOutputCharacter(hConsole," M 主菜单     ",14,temp_center,&cCharsWritten);
	loop3:
	fflush(stdin);
	choice=getch();
	switch (choice) {
	case 109://M
		goto menu;
	case 112://P
		temp_center.X=(fsize.X+2)*2;
		temp_center.Y=20;
		FillConsoleOutputCharacter(hConsole,' ',14,temp_center,&cCharsWritten);
		++temp_center.Y;
		FillConsoleOutputCharacter(hConsole,' ',14,temp_center,&cCharsWritten);
		++temp_center.Y;
		FillConsoleOutputCharacter(hConsole,' ',14,temp_center,&cCharsWritten);
		++temp_center.Y;
		FillConsoleOutputCharacter(hConsole,' ',14,temp_center,&cCharsWritten);
		goto loop1;
	case 114://R
		Sleep(100);
		goto play;
	default:
		goto loop3;
	}
	
	die:
	temp_center.X=(fsize.X+2)*2;
	temp_center.Y=20;
	WriteConsoleOutputCharacter(hConsole,"   你死了..   ",14,temp_center,&cCharsWritten);
	++temp_center.Y;
	WriteConsoleOutputCharacter(hConsole," R 重新开始   ",14,temp_center,&cCharsWritten);
	++temp_center.Y;
	WriteConsoleOutputCharacter(hConsole," M 主菜单     ",14,temp_center,&cCharsWritten);
	loop2:
	fflush(stdin);
	choice=getch();
	switch (choice) {
	case 109://M
		goto menu;
	case 114://R
		Sleep(100);
		goto play;
	default:
		goto loop2;
	}
	
	
	help:
	
	
	option:
	
	
	about:
	
	
	exit:
	for (int i=0;i<fsize.X;++i) delete[] board[i];
	delete[] board;
	return -1;
}

