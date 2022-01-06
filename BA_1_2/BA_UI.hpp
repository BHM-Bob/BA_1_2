//BASIC_ALL_1_2
//H
//Writen By BHM
//2021��11��18�� 23��08��

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_UI_H
#define BA_UI_H

#include"BA_Base.hpp"

int* ProduceRainbowCol(int* col, float* i);// r g b
//************************************************************************************************************************
//*********************************************SDL2*****************************************************************Start
//*************************************************************************************************************************
//***************************************************************************************************************************Struct SDL_MyButton ***Start

typedef struct MyUI_ColorSur_dot MyUI_ColorSur_dot;
struct MyUI_ColorSur_dot
{
	_LL x;
	_LL y;
	int color[3];
	float fx;
	float fy;
	float b;
};

typedef struct MyUI_ColorSur MyUI_ColorSur;
struct MyUI_ColorSur
{
	SDL_Rect* pre;
	_ULL sumdot;
	MyUI_ColorSur_dot* pdot;
	float* plv;
	float* plen;
	int col[4];
	SDL_Surface* distSur;//�ⲿ��NULL Surface�������
	SDL_Rect re_paint;
	MyUI_ColorSur* (*pF_Update)(MyUI_ColorSur* pcs);
};

MyUI_ColorSur* MyUI_ColorSur_Init(SDL_Surface* distSur);
void MyUI_ColorSur_CacuCol(MyUI_ColorSur* pcs);
MyUI_ColorSur* MyUI_ColorSur_Update(MyUI_ColorSur* pcs);

typedef struct MyUI_ColorText MyUI_ColorText;
struct MyUI_ColorText
{
	List* mem;
	char* pc;
	int charsize;
	bool** ppb;
	SDL_Rect* pre;
	_ULL sumdot;
	MyUI_ColorSur_dot* pdot;
	float* plv;
	float* plen;
	int col[4];
	TTF_Font* font;
	SDL_Renderer* rend;
	SDL_Surface* fontSur;
	SDL_Surface* pSur;
	SDL_Rect re_paint;
	MyUI_ColorText* (*pF_Update)(MyUI_ColorText* pcf);
};

MyUI_ColorText* MyUI_ColorText_Init(SDL_Renderer* rend, TTF_Font* font, const char* pc);
SDL_Surface* MyUI_ColorText_Get(MyUI_ColorText* pct);
bool MyUI_ColorText_Destroy(MyUI_ColorText* pct);




typedef struct SDL_MyButton SDL_MyButton;
struct SDL_MyButton
{
	SDL_Texture** ppTex;//Button ��������,button��ռ����Tex,���ޱ��������Ĭ��Tex�Ѵ���
	SDL_Rect re_butt;
	int backcolor[3];
	char* pc;
	TTF_Font* pfont;
	unsigned int charsize;
	int charcolor[3];
	Uint32 Format;
	MyUI_ColorSur* pct;
	SDL_Surface* back1;
	SDL_Surface* back2;//��������
	SDL_Surface* back3;//��������
};
SDL_MyButton* SDL_Create_MyButton(SDL_Renderer* rend, Uint32 Format, SDL_Rect re_butt_in, int backcolor[3], const char* pc, TTF_Font* pfont, unsigned int charsize, int charcolor[3], SDL_Surface* back);
//backcolor == NULL ͸������

bool SDL_Destroy_MyButton(SDL_MyButton* pbutt);

bool SDL_Destroy_MyButtons(SDL_MyButton* pbutt, ...);//End with a NULL

SDL_MyButton* SDL_Move_MyButton(SDL_MyButton* pbutt, SDL_Renderer* rend, int x, int y);

SDL_MyButton* SDL_Rewrite_MyButton(SDL_MyButton* pbutt, SDL_Renderer* rend, int backcolor[3], const char* pc, TTF_Font* pfont, int charsize, int charcolor[3]);

SDL_MyButton* SDL_ChangeBack_MyButton(SDL_MyButton* pbutt, SDL_Renderer* rend, int backcolor[3], SDL_Surface** back);

bool SDL_CheckOver_MyButton(SDL_Rect* re_butt, SDL_Event* peve);

bool SDL_CheckPressOn_MyButton(SDL_Rect* re_butt, SDL_Event* peve);

int SDL_CheckClick_MyButton(SDL_Rect* re_butt, SDL_Event* peve);
//return 0 >> NULL
//return 1 >> LEFT;
//return 2 >> RIGHT
//***************************************************************************************************************************Struct SDL_MyButton ***End

//***************************************************************************************************************************Struct MyTextInput ***Start
typedef struct MyTextInput MyTextInput;//�����ַ�����(ascii<<<�����ַ�>>><<<backspace>>>)
struct MyTextInput//����enter�����������
{
	SDL_Texture** ppTex;//MyTextInput ��������,MyTextInput��ռ����Tex,���ޱ��������Ĭ��Tex�Ѵ���
	SDL_Rect re_text;
	int edgingwidth;
	int edgingwidthcolor[3];
	int backcolor[3];
	char* pc;
	TTF_Font* pfont;
	int charsize;
	int charcolor[3];
	SDL_Surface* back1;
	SDL_Surface* back2;//��������
	SDL_Surface* back3;//��������
};
MyTextInput* SDL_Create_MyTextInput(SDL_Renderer* rend, Uint32 Format, SDL_Rect re_text_in, int edgingwidth, int edgingwidthcolor[3], int backcolor[3], TTF_Font* pfont, int charsize, int charcolor[3], SDL_Surface* back);

MyTextInput* SDL_Input_MyTextInput(SDL_Renderer* rend, MyTextInput* pext);

MyTextInput* SDL_Move_MyTextInput(MyTextInput* ptext, SDL_Renderer* rend, int x, int y);

MyTextInput* SDL_Rewrite_MyTextInput(MyTextInput* ptext, SDL_Renderer* rend, int backcolor[3], char* pc, TTF_Font* pfont, int charsize, int charcolor[3]);

MyTextInput* SDL_ChangeBack_MyTextInput(MyTextInput* ptext, SDL_Renderer* rend, int edgingwidth, int edgingwidthcolor[3], int backcolor[3], SDL_Surface* back);


//***************************************************************************************************************************Struct Pushmess ***Start
typedef struct SD_Pushmess SD_Pushmess;
struct SD_Pushmess
{
	char* mess;
	TTF_Font* pfont;
	int charsize;
	int sec;
};
SDL_Window* SDL_Pushmess(const char* mess, TTF_Font* pfont, int charsize, int sec);
int SDL_Pushmess2(void* pwin);
//ģ�°�׿���׺ڿ�
//***************************************************************************************************************************Struct Pussmess ***End


bool SDL_Init_s(void);

Uint32 SDL_GetPixel(SDL_Surface* surface, int x, int y);

SDL_Texture* SDL_CreateRGBTexture(SDL_Renderer* rend, int w, int h, int r, int g, int b);

SDL_Surface* SDL_GetImage_Sur(const char* ppath);

SDL_Texture* SDL_GetImage_Tex(SDL_Renderer* renderer, const char* ppath, bool BackColorKeep, int BR, int BG, int BB);

SDL_Texture* SDL_Get_Font_Texture(SDL_Renderer* prend, SDL_Surface* pscr, TTF_Font* pfont, int* charcolor, int Size, const char* pc, int x, int y);

SDL_Surface* SDL_Get_Font_Surface(SDL_Surface** ppscr, TTF_Font* pfont, int R, int G, int B, int Size, char* pc, int x, int y);

void SDL_printf_Texture(SDL_Renderer* prenderer, int x, int y, int w, int h, int point_x, int point_y, int angel, SDL_Texture* pmess);

void SDL_printf(char* pc, SDL_Renderer* prenderer, bool Prensent_Now, int x, int y, int Size, int R, int G, int B, TTF_Font* pfont);

bool SDL_Poll_Quit(SDL_Event* pevent);

char* SDL_Poll_Scnaf(SDL_Event* peve, char** pc);

bool SDL_Quit_s(SDL_Window** win, SDL_Renderer** renderer, SDL_Surface** ppSurface1, ...);
//End With a NULL


//***************************************************************************************************************************Struct MyUI ***Start
typedef struct MyUI_butt MyUI_butt;
struct MyUI_butt
{
	int sumbutt;
	int* eventbutt;//1 left ; 2 right
	int* buttava;//��ť�б�ռ�� 0������   1��������ʾ   2���ڲ���ʾ
	SDL_MyButton* pbutt[1024];//��ť�б�
};


typedef struct MyUI_win MyUI_win;
struct MyUI_win
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
	int color[3];
	SDL_Texture* pwinTex;
	SDL_Event* peve;
	clock_t time;
	float FPS;
	int exitbutt;//order
	int titlebutt;//title
	SDL_Rect* pre_title;
};

typedef struct MyUI_font MyUI_font;
struct MyUI_font
{
	int sumfont;
	TTF_Font* pdefaultfont;//"C:\\Windows\\Fonts\\simkai.ttf"
	int* FontListAvaliable;//�����б�ռ��
	TTF_Font* pfont;
};

typedef struct MyUI_set MyUI_set;
struct MyUI_set
{
	SDL_Rect re_win;
	SDL_Rect re_title;
	SDL_Rect re_exitbutt;
	SDL_Color col_win;
	SDL_Color col_title;
	SDL_Color col_exitbutt;
};

typedef struct MyUI MyUI;//SDL Basic UI Collection
struct MyUI
{
	MyUI_win* win;
	MyUI_butt* butt;
	MyUI_font* font;
	MyUI_set* set;//Settings
	List* otherTex;
	List* otherTexRe;
	SDL_Texture* otherTex2[1024];
	SDL_Rect* otherTexRe2[1024];
	MyUI* (*pF_Addfont)(MyUI* pui, int order, const char* ppath);
	MyUI* (*pF_Addbutt)(MyUI* pui, int order, const char* name, int charsize, int charcolor[3], int backcolor[3], int xywh[4], SDL_Surface* back);
	MyUI* (*pF_Delbutt)(MyUI* pui, int order);
	MyUI* (*pF_Checkbutt)(MyUI* pui);
	MyUI* (*pF_Checktitle)(MyUI* pui);
	MyUI* (*pF_Update)(MyUI* pui, bool rendclear, bool copyTex);
	bool (*pF_PollQuit)(MyUI* pui);//do not get quit sig return 0
	void (*pF_PutErr)(MyUI* pui, char* pc);
	void (*pF_GetErr)(MyUI* pui);
};

MyUI* MyUI_Init(const char* titlepc, int winw, int winh, int winflags, int* color);//flag==0 mean defualt
MyUI* MyUI_Addfont(MyUI* pui, int order, const char* ppath);
MyUI* MyUI_Addbutt(MyUI* pui, int order, const char* name, int charsize, int charcolor[3], int backcolor[3], int xywh[4], SDL_Surface* back);//order ��0����
MyUI* MyUI_Delbutt(MyUI* pui, int order);
MyUI* MyUI_Checkbutt(MyUI* pui);
MyUI* MyUI_Checktitle(MyUI* pui);
MyUI* MyUI_Update(MyUI* pui, bool rendclear, bool copyTex);
bool MyUI_PollQuit(MyUI* pui);

//***************************************************************************************************************************Struct MyUI ***End
//************************************************************************************************************************
//*********************************************SDL2******************************************************************End

#endif
