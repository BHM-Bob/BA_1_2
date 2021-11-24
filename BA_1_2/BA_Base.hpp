//BASIC_ALL_1_2
//H
//Writen By BHM
//2021年11月20日
//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_1_2__BASE_H
#define BA_1_2__BASE_H

#define __STDC_WANT_LIB_EXT1__ 1

#include<assert.h>
#include<ctype.h>
#include<conio.h>
#include<direct.h>
#include<locale.h>
#include<limits.h>
#include<math.h>
#include<io.h>
#include<setjmp.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<time.h>
//#include<unistd.h>

#ifdef USE_THREADS
#include<thread>
#endif

#ifdef USE_WINDOWS

#include<Windows.h>
#include<WinBase.h>

#endif // USE_WINDOWS

/*
E:\My_Progs\Cpp\@ExtendLib\SDL2\lib\x64\LIB\SDL2.lib
E:\My_Progs\Cpp\@ExtendLib\SDL2\lib\x64\LIB\SDL2_image.lib
E:\My_Progs\Cpp\@ExtendLib\SDL2\lib\x64\LIB\SDL2_mixer.lib
E:\My_Progs\Cpp\@ExtendLib\SDL2\lib\x64\LIB\SDL2main.lib
E:\My_Progs\Cpp\@ExtendLib\SDL2\lib\x64\LIB\SDL2_ttf.lib
*/
#ifdef USE_SDL2

#include"../../../Cpp/@ExtendLib/SDL2/include/SDL.h"
#include"../../../Cpp/@ExtendLib/SDL2/include/SDL_syswm.h"
#include"../../../Cpp/@ExtendLib/SDL2/include/SDL_image.h"
#include"../../../Cpp/@ExtendLib/SDL2/include/SDL_ttf.h"
#include"../../../Cpp/@ExtendLib/SDL2/include/SDL_mixer.h"
#endif // USE_SDL2

/*
D:\OpenCV\opencv\build\include\opencv2

D:\OpenCV\opencv\build\x64\vc15\lib\opencv_world454d.lib

D:\OpenCV\opencv\build\x64\vc15\lib
*/
#ifdef USE_OPENCV
#include<opencv2/opencv.hpp>
#endif

typedef unsigned long long _ULL;

typedef long long _LL;

void PPIs(int n, ...);

#define PPT() printf("\nTime now : In %s at line %lu ,<%s> <%.10f s>,\n",__func__,__LINE__,Get_Time_Without(),(float)( (float)(clock())/CLOCKS_PER_SEC))
#define PPI(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%d>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPL(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%lld>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPD(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%le>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPF(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%f>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPU(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%llu>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPC(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%c>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPP(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%p>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPS(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%s>,It's size is %llu\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p),strlen(p))
//this str can not contan any ++ opts, because ti will use twice in this define line

#define PPW(p) printf("\nWarning : In %s at line %lu ,<%s>!!!,\n",__func__,__LINE__,(p))
//Push Warning

#define PPWs(p,q) printf("\nWarning : In %s at line %lu ,<%s %s>!!!,\n",__func__,__LINE__,(p),(q))
//Push Warning for a variate string q

/*STOP_IF_SCANF*/
#define _SIS_ {printf("\nStopping Now,Enter Anykey To Comtinue\nAt %s in line %lu  ",__func__,__LINE__);fflush(stdin);_getch();printf("   Scanfed\n");fflush(stdin);}

#define MCALLOC(num,type) (type*)calloc((size_t)(num),sizeof(type))
#define MCALLOCS(type,ret,num) type* ret = (type*)calloc((size_t)(num),sizeof(type))

#define BALLOC_L(num,type) (type*)MyBA_CALLOC_L((size_t)(num),sizeof(type));
#define BALLOC_S(num,type) (type*)MyBA_CALLOC_S((size_t)(num),sizeof(type));

#define BALLOCS_L(type,ret,num,err_ret,err_opts) type* ret = (type*)MyBA_CALLOC_L((size_t)(num),sizeof(type));if((ret) == NULL){err_opts;return err_ret;}
#define BALLOCS_S(type,ret,num,err_ret,err_opts) type* ret = (type*)MyBA_CALLOC_S((size_t)(num),sizeof(type));if((ret) == NULL){err_opts;return err_ret;}

#define BALLOC_R(num,type,pli) (type*)MyBA_CALLOC_R((size_t)(num),sizeof(type),pli);
#define BALLOCS_R(type,ret,num,pli,err_ret,err_opts) type* ret = (type*)MyBA_CALLOC_R((size_t)(num),sizeof(type),pli);if((ret) == NULL){err_opts;return err_ret;}

#define STOPIF(a,string,...) {if(a){ printf("\nIn %s at line %lu\n, "#string"\n",__func__,__LINE__);__VA_ARGS__;}}

#define _Get_File_Size(n,pf) unsigned long long n = 0ULL;{fseek(pf,0,SEEK_END);n = ftell(pf);fseek(pf,0,SEEK_SET);};

#define _strcat_s(a,b,c,d) {if(strcat_s(a,b,c) != 0){PPW("Can't strcat_s");PPS(a);PPS(c);return d;}}

#define _CHECKP_(p) {if((p) == NULL){PPW("NULL Pointer");_SIS_;}}

#define LIST_FOR(p,pli) for(void* p = List_Copy(pli); p != NULL; p = List_Copy(pli))
#define LIST_FORS(type,p,pli) for(type* p = (type*)List_Copy(pli); p != NULL; p = (type*)List_Copy(pli))
#define LIST_FORR(type,p,pli,opts) for(type* p = (type*)List_Copy(pli); p != NULL; p = (type*)List_Copy(pli),opts)

char* mstrdup(const char* p);
int* intdup(int num, ...);
_ULL* ULLdup(_ULL num, ...);
float* floatdup(_ULL num, ...);
int* intdupS(int num, ...);
_ULL* ULLdupS(_ULL num, ...);
float* floatdupS(_ULL num, ...);

void JDT(_ULL now, _ULL sum);

void gotoxy(int x, int y);

char* StringAdd_L(const char* pstr, ...);//end with NULL
char* StringAdd_S(const char* pstr, ...);//end with NULL

int GetDayOfMonth(int year, int month);

char* Get_Time_Without_L(void);
char* Get_Time_Without_S(void);

char* Get_Time_L(void);
char* Get_Time_S(void);

char* Get_Time_For_File_Name(char char_to_replace_unspport_char);

char* Num_To_Char(const char* ptype, ...);
// ... 表示被换的数字变量

char* Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite);
/*来源于 VN7 v1.1 项目*//*VN7 v1.2 进行了更新*/
//当然,这玩意儿可用strstr(pstr1,pstr2)做出来,而且更方便,脑子梗,一时没想起来,唉,浪费一个多小时,更别说CSH.c
//里的Landing_Account调试时,一个极其明显但又不突出,不好找,关键是没调试器,的BUG找了一个多小时,胃疼,胃胀

char* Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter, unsigned long long* psite);

bool Frees(char* ptype, ...);
// ... Must end with a NULL

char* GBK_To_UTF8(const char* pc);

#ifdef USE_THREADS
//***********************************************************************************************************************
typedef struct MyThread MyThread;
struct MyThread
{
	_ULL sumthreads;
	_ULL numid;
	bool flag;
	bool quit;
	int (*pF)(void*);
	void* pdata;
	int state;//从零开始 thrd_success thrd_timedout thrd_busy thrd_nomem thrd_error
	thrd_t* pid;
	mtx_t* pmtx;          // 一个互斥
	cnd_t* pcndput;       // 两个条件变量
	cnd_t* pcndget;
};
MyThread* MyThread_Init(_ULL sumthreads);//产生sumthreads个MyThread
MyThread* MyThread_Start(MyThread * pth, int (*pF)(void*), void* pdata);
MyThread* MyThread_Get(MyThread * pth);
MyThread* MyThread_Free(MyThread * pth);
MyThread* MyThread_Destroy(MyThread * pth);
//***********************************************************************************************************************

//***********************************************************************************************************************
typedef struct MyThreadQue MyThreadQue;//先进先出
struct MyThreadQue
{
	MyThreadQue* pnext;
	MyThreadQue* ppre;
	int (*pF)(void*);
	void* pdata;
	int state;//从零开始 thrd_success thrd_timedout thrd_busy thrd_nomem thrd_error
};

typedef struct MyThreadQueue MyThreadQueue;//先进先出
struct MyThreadQueue
{
	_ULL sumque;
	MyThreadQue* pfirst;
	MyThreadQue* plast;
	int (*pF)(void*);
	MyThreadQueue* (*Put)(MyThreadQueue* pque, void* pdata);
	MyThreadQue* (*Get)(MyThreadQueue* pqueue);
	int state;//从零开始 thrd_success thrd_timedout thrd_busy thrd_nomem thrd_error
	thrd_t* pid;
	mtx_t* pmtx;          // 一个互斥
	cnd_t* pcndput;       // 两个条件变量
	cnd_t* pcndget;
};
MyThreadQueue* MyThreadQueue_Init(void* pdata);
MyThreadQue* MyThreadQueue_Get(MyThreadQueue * pqueue);
MyThreadQueue* MyThreadQueue_Put(MyThreadQueue * pque, void* pdata);
MyThreadQueue* MyThreadQueue_Destroy(MyThreadQueue * pque);
//***********************************************************************************************************************
#endif


//***********************************************************************************************************************
typedef struct ListDot ListDot;//先进先出
struct ListDot
{
	_ULL idx;//from 0
	_ULL usage;//for same open use
	ListDot* pnext;
	ListDot* ppre;
	void* pdata;
};

typedef struct List List;//先进先出
struct List
{
	_ULL tik;//标识符，区分List，或者用来储存一些ID信息
	_ULL sumque;
	ListDot* pfirst;
	ListDot* plast;
	ListDot* now;
	ListDot* reverse_now;

	List* (*Put)(List* plist, void* pdata);
	void* (*Get)(List* plist);
	void* (*Copy)(List* plist);
	void* (*ReverseCopy)(List* plist);
	void* (*Index)(List* plist, _ULL index);
	ListDot* (*IndexDot)(List* plist, _ULL index);
	void* (*IndexGet)(List* plist, _ULL index);
};
List* List_Init(void);
void* List_Copy(List* plist);
void* List_ReverseCopy(List* plist);
void* List_Get(List* plist);
void* List_Index(List* plist, _ULL index);//Get the index dot content,from 0
ListDot* List_IndexDot(List* plist, _ULL index);//Get the index dot,from 0
void* List_IndexGet(List* plist, _ULL index);
List* List_Put(List* plist, void* pdata);
List* List_Destroy(List* plist);

//***********************************************************************************************************************
typedef struct BALog BALog;
struct BALog
{
	clock_t t;
	char* pdate;
	char* pc;
	int code;
};

typedef struct MyBA MyBA;
struct MyBA
{
	float BA_Ver;
	char BuildDate[12];
	char BuildTime[9];

	char* exepath;//psd->exeinfo.exepath is <G:\\PellesC\\My Project\\VN7\\VN7 v1_3\\VN7 1_3>,It's size is 42

	List* mem;//BA Self Mem
	List* LTmem;//Long Term Mem
	List* STmem;//Short Term Mem

	clock_t GUT_t;
	bool GUT_state;
	clock_t JDT_t;

	bool isSDL2;
	bool isSAFEMODE;

	List* pLog;

	int (*Quit)(void);
	float (*GUT)(void);
	void (*PutLog)(const char* pc);
};
extern MyBA* pba;
float MyBA_GetUsedTime(void);
void MyBA_Init(void);
void MyBA_PutLog(const char* pc);
bool MyBA_WriteLog(bool isquit);
void* MyBA_Err(const char* pc, bool instance);
void* MyBA_Errs(bool instance, ...);
int MyBA_Quit(void);

void MyBA_Free(void* p, List* mem);
void MyBA_Free_R(List* pli);
void MyBA_FreeInstance(void);
void* MyBA_CALLOC_R(_ULL count, _ULL size, List* pli);
void* MyBA_CALLOC_L(_ULL count, _ULL size);
void* MyBA_CALLOC_S(_ULL count, _ULL size);

void MyBA_CMD(void);
int MyBA_CMD_SearchCom(char* pc);
int MyBA_CMD_ShowLog(void);
void MyBA_SafeMode(void);
//***********************************************************************************************************************
#endif