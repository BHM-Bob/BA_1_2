﻿//BASIC_ALL_1_2
//H
//Writen By BHM
//2022年7月30日

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_QUI_H
#define BA_QUI_H

#include"BA_Base.hpp"
#include"BA_UI.hpp"


SDL_Color* SetSDLCol(SDL_Color* col, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
SDL_Color* MakeSDLCol(List* mem, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
SDL_Rect* SetSDLRect(SDL_Rect* pos, int w, int h, int x, int y);
SDL_Rect* MakeSDLRect(List* mem, int w, int h, int x, int y);


typedef struct QUI_butts QUI_butts;
struct QUI_butts
{
	list<int>* events;//1 left ; 2 right
	list<int>* statue;//按钮列表占用 0不存在   1存在且显示   2存在不显示
	list<SDL_MyButton>* butts;//按钮列表
};


typedef struct QUI_win QUI_win;
struct QUI_win
{
	char* titlepc;
	SDL_Window* pwin;
	SDL_SysWMinfo info;
	HWND hwnd;
	int winw;
	int winh;
	SDL_Rect* pre_win;
	SDL_Renderer* rend;
	SDL_PixelFormat* format;
	SDL_Surface* pwinSur;
	SDL_Color* bgc;
	SDL_Texture* pwinTex;
	SDL_Event* peve;
	clock_t time;
	float FPS;
	int exitbutt;//order
	int titlebutt;//title
	SDL_Rect* pre_title;
};

typedef struct QUI_fonts QUI_fonts;
struct QUI_fonts
{
	TTF_Font* pdefaultfont;//"C:\\Windows\\Fonts\\simkai.ttf"
	list< TTF_Font>* fonts;
};

typedef struct QUI_set QUI_set;
struct QUI_set
{
	SDL_Rect re_win;
	SDL_Rect re_title;
	SDL_Rect re_exitbutt;
	SDL_Color col_win;
	SDL_Color col_title;
	SDL_Color col_exitbutt;
};

class QUI
{
public:
	QUI_win* win = NULL;
	QUI_butts* butts = NULL;
	QUI_fonts* fonts = NULL;
	QUI_set* set = NULL;//Settings

	list< SDL_Texture>* otherTex = NULL;
	list< SDL_Rect>* otherTexRe = NULL;

	List* mem = NULL;

	QUI(const char* titlepc = "QUI", int winw = 800, int winh = 800, int winflags = 0, SDL_Color* bgc = NULL);
	QUI();
	~QUI();

	bool AddFont(const char* ppath, const char* name);
	bool AddButt(const char* name, int charSize, SDL_Color* charCol, SDL_Color* bgc, SDL_Rect* pos, SDL_Surface* bg);
	bool DelButt(const char* name);
	bool DelButt(SDL_MyButton* pButt);
	bool CheckButt();
	bool CheckTitle();
	bool Update(bool rendclear = true, bool copyTex = true);
	bool PollQuit();
	friend int QUI_Quit(void* pui_, int code, ...);
};

//***************************************************************************************************************************Struct MyUI ***End
//************************************************************************************************************************
//*********************************************SDL2******************************************************************End

#endif
