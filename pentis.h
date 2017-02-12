#ifndef _PENTIS_H
#define _PENTIS_H
#include <conio.h>
#include "cmd.h"
int getk() {
	int temp_i;
	fflush(stdin);
	temp_i=getch();
	if (temp_i==0||temp_i==224) temp_i=getch();
	if (temp_i>127) {
		fflush(stdin);
		return -1;
	}
	return temp_i;
}

bool drawUI(const COORD temp_size,char cunit[]="█") { 
 	COORD temp_c;
	if (temp_size.Y<24) return false;
	for (int j=0;j<temp_size.Y;++j) for (int i=0;i<temp_size.X;++i) {
		temp_c.X=(i+1)*2;
		temp_c.Y=j+1;
		WriteConsoleOutputCharacter(hConsole,cunit,2,temp_c,&cCharsWritten);
	}
	gotoxy(0,0);
	printf("┏");
	for (int i=0;i<temp_size.X;++i) printf("━");
	printf("┳━━━━━━━┓");
	for (int i=0;i<temp_size.Y;++i) {
		gotoxy(0,i+1);
		printf("┃");
		gotoxy((temp_size.X+1)*2,i+1);
		printf("┃");
		gotoxy((temp_size.X+9)*2,i+1);
		printf("┃");
	}
	gotoxy((temp_size.X+2)*2,1);
	printf("┏━━━━━┓");
	gotoxy((temp_size.X+2)*2,2);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,3);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,4);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,5);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,6);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,7);
	printf("┗━━━━━┛");
	gotoxy((temp_size.X+2)*2,8);
	printf("     保持");
	gotoxy((temp_size.X+2)*2,9);
	printf("┏━━━━━┓");
	gotoxy((temp_size.X+2)*2,10);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,11);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,12);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,13);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,14);
	printf("┃█████┃");
	gotoxy((temp_size.X+2)*2,15);
	printf("┗━━━━━┛");
	gotoxy((temp_size.X+2)*2,16);
	printf("    下一个");
	gotoxy((temp_size.X+2)*2,18);
	printf("分数：");
	gotoxy(0,temp_size.Y+1);
	printf("┗");
	for (int i=0;i<temp_size.X;++i) printf("━");
	printf("┻━━━━━━━┛");
	return true;
}

inline bool refresh(const COORD size,bool **target,const unsigned short cunit=0x0f) {
	COORD temp_c;
	for (int i=0;i<size.X;++i) for (int j=0;j<size.Y;++j) {
		temp_c.X=(i+1)*2,temp_c.Y=j+1;
		if (target[i][j]) {
			FillConsoleOutputAttribute(hConsole,cunit,2,temp_c,&cCharsWritten);
		} else {
			FillConsoleOutputAttribute(hConsole,0,2,temp_c,&cCharsWritten);
		}
	}
}

bool drawScore(const COORD size,const unsigned int score) {
	gotoxy((size.X+5)*2,18);
	printf("%d",score);
	return true;
}

bool drawBlock(const COORD size,const COORD center,const COORD shift[5],bool mode,const unsigned short cunit=0x0f) {
	/*
	mode=true 显示
	mode=false 擦除
	*/
	COORD temp_c;
	for (int i=0;i<5;++i) {
		if (center.Y+shift[i].Y<0) continue;
		temp_c.X=(center.X+shift[i].X+1)*2,temp_c.Y=center.Y+shift[i].Y+1;
		if (mode) FillConsoleOutputAttribute(hConsole,cunit,2,temp_c,&cCharsWritten); else FillConsoleOutputAttribute(hConsole,0,2,temp_c,&cCharsWritten);
	}
	return true;
}

bool drawHold(const COORD size,const COORD shift[5],const unsigned short cunit=0x0f) {
	COORD temp_c;
	for (int i=0;i<5;++i) {
		temp_c.X=(shift[i].X+size.X+5)*2,temp_c.Y=shift[i].Y+4;
		FillConsoleOutputAttribute(hConsole,cunit,2,temp_c,&cCharsWritten);
	}
	return true;
}

bool clearHold(const COORD size) {
	COORD temp_coord;
	temp_coord.X=(size.X+3)*2;
	for (int i=0;i<5;++i) {
		temp_coord.Y=i+2;
		FillConsoleOutputAttribute(hConsole,0,10,temp_coord,&cCharsWritten);
	}
	return true;
}

bool drawNext(const COORD size,const COORD shift[5],const unsigned short cunit=0x0f) {
	COORD temp_c;
	for (int i=0;i<5;++i) {
		temp_c.X=(shift[i].X+size.X+5)*2,temp_c.Y=shift[i].Y+12;
		FillConsoleOutputAttribute(hConsole,cunit,2,temp_c,&cCharsWritten);
	}
	return true;
}

bool clearNext(const COORD size) {
	COORD temp_coord;
	temp_coord.X=(size.X+3)*2;
	for (int i=0;i<5;++i) {
		temp_coord.Y=i+10;
		FillConsoleOutputAttribute(hConsole,0,10,temp_coord,&cCharsWritten);
	}
	return true;
}

bool drawShadow(const COORD size,const COORD center,const COORD shift[5],const unsigned short cunit=0xef) {
	COORD temp_coord;
	temp_coord.Y=size.Y+1;
	for (int i=0;i<5;++i) {
		temp_coord.X=(center.X+shift[i].X+1)*2;
		FillConsoleOutputAttribute(hConsole,cunit,2,temp_coord,&cCharsWritten);
		/*
		for (int i=center.Y+shift[i].Y+1;i<=size.Y;++i) {
			temp_coord.Y=i;
			FillConsoleOutputAttribute(hConsole,cunit,2,temp_coord,NULL);
		}
		*/
	}
	return true;
}

bool clearShadow(const COORD size,const unsigned short cunit=0x0f) {
	COORD temp_coord;
	temp_coord.X=2;
	temp_coord.Y=size.Y+1;
	FillConsoleOutputAttribute(hConsole,cunit,size.X*2,temp_coord,&cCharsWritten);
	return true;
}

bool solify(bool **target,const COORD center,COORD shift[5]) {//同时检测是否到顶
	for (int i=0;i<5;++i){
		if (center.Y+shift[i].Y<1) return false;
		target[center.X+shift[i].X][center.Y+shift[i].Y]=true;
	}
	return true;
}

bool drop(const COORD size,bool **target,COORD &center,COORD shift[5]) {
	for (int i=0;i<5;++i){
		if (center.Y+shift[i].Y>=size.Y-1) return false;
		if (center.Y+shift[i].Y>=0) if (target[center.X+shift[i].X][center.Y+shift[i].Y+1]) return false;
	}
	++center.Y;
	return true;
}

bool pushup(bool **target,COORD &center,COORD shift[5]) {
	for (int i=0;i<5;++i) if (center.Y+shift[i].Y<0) return false;
	for (int i=0;i<5;++i) if (target[center.X+shift[i].X][center.Y+shift[i].Y]) {
		--center.Y;
		return true;
	}
	return false;
}

bool pushleft(const COORD size,bool **target,COORD &center,COORD shift[5]) {
	for (int i=0;i<5;++i) if (center.X+shift[i].X>size.X-1) {
		--center.X;
		return true;
	}
	return false;
}

bool pushright(const COORD size,bool **target,COORD &center,COORD shift[5]) {
	for (int i=0;i<5;++i) if (center.X+shift[i].X<0) {
		++center.X;
		return true;
	}
	return false;
}

bool spawn(COORD target[5]) {
	switch(rand()%17+1) {
	case 1:
		target[0].X=0;
		target[0].Y=-2;
		target[1].X=0;
		target[1].Y=-1;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=0;
		target[3].Y=1;
		target[4].X=0;
		target[4].Y=2;
		break;
	case 2:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=-1;
		target[3].Y=1;
		target[4].X=0;
		target[4].Y=1;
		break;
	case 3:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=0;
		target[1].Y=0;
		target[2].X=1;
		target[2].Y=0;
		target[3].X=0;
		target[3].Y=1;
		target[4].X=1;
		target[4].Y=1;
		break;
	case 4:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=0;
		target[4].Y=1;
		break;
	case 5:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=1;
		target[4].Y=1;
		break;
	case 6:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=-1;
		target[4].Y=1;
		break;
	case 7:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=2;
		target[4].Y=0;
		break;
	case 8:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=-2;
		target[4].Y=0;
		break;
	case 9:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=0;
		target[1].Y=0;
		target[2].X=-1;
		target[2].Y=1;
		target[3].X=0;
		target[3].Y=1;
		target[4].X=1;
		target[4].Y=1;
		break;
	case 10:
		target[0].X=-1;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=-1;
		target[2].Y=1;
		target[3].X=0;
		target[3].Y=1;
		target[4].X=1;
		target[4].Y=1;
		break;
	case 11:
		target[0].X=-1;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=2;
		target[4].Y=0;
		break;
	case 12:
		target[0].X=1;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=-2;
		target[4].Y=0;
		break;
	case 13:
		target[0].X=-1;
		target[0].Y=-1;
		target[1].X=0;
		target[1].Y=-1;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=0;
		target[3].Y=1;
		target[4].X=1;
		target[4].Y=1;
		break;
	case 14:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=1;
		target[1].Y=-1;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=-1;
		target[3].Y=1;
		target[4].X=0;
		target[4].Y=1;
		break;
	case 15:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=-1;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=0;
		target[4].X=2;
		target[4].Y=0;
		break;
	case 16:
		target[0].X=0;
		target[0].Y=-1;
		target[1].X=-1;
		target[1].Y=0;
		target[2].X=0;
		target[2].Y=0;
		target[3].X=1;
		target[3].Y=-1;
		target[4].X=-2;
		target[4].Y=0;
		break;
	case 17:
		target[0].X=-1;
		target[0].Y=-1;
		target[1].X=1;
		target[1].Y=-1;
		target[2].X=-1;
		target[2].Y=0;
		target[3].X=0;
		target[3].Y=0;
		target[4].X=1;
		target[4].Y=0;
		break;
	}
}
#endif
