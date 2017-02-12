#ifndef _CMD_H
#define _CMD_H
#include <windows.h>
#include <stdio.h>
using namespace std;
HANDLE hConsole;
DWORD dwConSize;
COORD upperleft={0,0};
DWORD cCharsWritten;
CONSOLE_SCREEN_BUFFER_INFO csbi;
CONSOLE_CURSOR_INFO csinfo;

inline void initialcmd() {
	hConsole=GetStdHandle(0xfffffff5);//VERY IMPORTANT
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	dwConSize=csbi.dwSize.X * csbi.dwSize.Y;
}

inline void cls() {
	FillConsoleOutputCharacter(hConsole,(TCHAR)' ',dwConSize,upperleft,&cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,upperleft,&cCharsWritten);
	SetConsoleCursorPosition(hConsole,upperleft);
	return;
}

inline void gotoxy(short x,short y) {
	COORD temp_coord={x,y};
	SetConsoleCursorPosition(hConsole,temp_coord);
	return;
}

inline void color(unsigned int temp_color) {
	SetConsoleTextAttribute(hConsole,temp_color);
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,upperleft,&cCharsWritten);
}

inline void title(char* titletext) {
	SetConsoleTitle(titletext);
}

inline void echox(char* temp_text,unsigned int temp_color) {
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	SetConsoleTextAttribute(hConsole,temp_color);
	printf(temp_text);
	SetConsoleTextAttribute(hConsole,csbi.wAttributes);
}

inline void setcs(bool isVisible,int size) {
	if (size<0) {
		GetConsoleCursorInfo(hConsole,&csinfo);
		size=csinfo.dwSize;
	} else {
		csinfo.dwSize=size;
	}
	csinfo.bVisible=isVisible;
	SetConsoleCursorInfo(hConsole,&csinfo);
}

void windowsize(short x,short y) {
	SMALL_RECT temp_sr={0,0,x,y};
	RECT temp_r1,temp_r2; 
	COORD temp_c={x+1,y+1};
	GetWindowRect(GetConsoleWindow(),&temp_r1);
	SetConsoleScreenBufferSize(hConsole,GetLargestConsoleWindowSize(hConsole));
	SetConsoleWindowInfo(hConsole,true,&temp_sr);
	SetConsoleScreenBufferSize(hConsole,temp_c);
	while (1) {
		GetWindowRect(GetConsoleWindow(),&temp_r2);
		if (temp_r1.bottom!=temp_r2.bottom||temp_r1.left!=temp_r2.left||temp_r1.right!=temp_r2.right||temp_r1.top!=temp_r2.top) break;
	}
}

bool centerwindow() {
	RECT temp_rd,temp_rc;
	GetWindowRect(GetDesktopWindow(),&temp_rd);
	GetWindowRect(GetConsoleWindow(),&temp_rc);
	MoveWindow(GetConsoleWindow(),(int)((temp_rd.right+temp_rd.left-temp_rc.right+temp_rc.left)/2),(int)((temp_rd.bottom+temp_rd.top-temp_rc.bottom+temp_rc.top))/2,temp_rc.right-temp_rc.left,temp_rc.bottom-temp_rc.top,true);
}

int menu(int size,char* list[],unsigned int shift,unsigned int color) {
	if (size>122) {
		printf("ERROR: list too long(is over 122 lines)");
		return -1;
	}
	
	bool exit=false;
	int ch=0;//char buffer
	int no=1;//current number on the menu
	int el=0;//errorlevel
	COORD temp_coord;
	CONSOLE_CURSOR_INFO invisible;
	CONSOLE_CURSOR_INFO cursorinfo;
	
	GetConsoleCursorInfo(hConsole,&cursorinfo);
	invisible.bVisible=false;
	invisible.dwSize=cursorinfo.dwSize;
	SetConsoleCursorInfo(hConsole,&invisible);
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	
	for (int i=1;i<=size;i++) {
		gotoxy(shift+csbi.dwCursorPosition.X,csbi.dwCursorPosition.Y+i-1);
		printf("%s\n",list[i-1]);
	}
	
	while (!exit) {
		temp_coord.X=0;
		temp_coord.Y=csbi.dwCursorPosition.Y+no-1;
		FillConsoleOutputAttribute(hConsole,color,csbi.dwSize.X,temp_coord,&cCharsWritten);
		ch=getch();
		switch(ch) {
			case 0://numpad, ect.
			ch=getch();
			switch(ch) {
				case 72://up
					FillConsoleOutputAttribute(hConsole,csbi.wAttributes,csbi.dwSize.X,temp_coord,&cCharsWritten);
					if (no>1) {
						no--;
					} else {
						no=size;
					}
				break;
				
				case 80: //down
					FillConsoleOutputAttribute(hConsole,csbi.wAttributes,csbi.dwSize.X,temp_coord,&cCharsWritten);
					if (no<size) {
						no++;
					} else {
						no=1;
					}
				break;
				
				case 75://left
					el=0;
					exit=true;
				break;
				
				case 77://right
					el=no;
					exit=true;
				break;
			}	
			break;
			
			case 8://escape
				el=0;
				exit=true;
			break;
			
			case 13://Enter
				el=no;
				exit=true;
			break;
			
			case 27://ESC
				el=0;
				exit=true;
			break;
			
			case 224://function keys
			ch=getch();
			switch(ch) {
				case 72://up
					FillConsoleOutputAttribute(hConsole,csbi.wAttributes,csbi.dwSize.X,temp_coord,&cCharsWritten);
					if (no>1) {
						no--;
					} else {
						no=size;
					}
				break;
				
				case 80: //down
					FillConsoleOutputAttribute(hConsole,csbi.wAttributes,csbi.dwSize.X,temp_coord,&cCharsWritten);
					if (no<size) {
						no++;
					} else {
						no=1;
					}
				break;
				
				case 75://left
					el=0;
					exit=true;
				break;
				
				case 77://right
					el=no;
					exit=true;
				break;
			}
			break;
			
			default:
				if (ch>=48&&ch<=57) {
					if (ch-48<=size) {
						el=ch-48;
						exit=true;
					}
				} else if (ch>=97&&ch<=122) {
					if (ch-87<=size) {
						el=ch-87;
						exit=true;
					}
				}
				if (ch==-1||ch>127) {
					ExitProcess(-1);
				} 
			break;
		}
	}
	SetConsoleCursorInfo(hConsole,&cursorinfo);
	return el;
}
#endif

