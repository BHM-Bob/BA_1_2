//BASIC_ALL_1_2
//H
//Writen By BHM
//2021年11月20日
//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_1_2_BASE_H
#define BA_1_2_BASE_H

#define __STDC_WANT_LIB_EXT1__ 1

#include<assert.h>
#include<ctype.h>
#include<conio.h>
#include<direct.h>
#include<locale.h>
#include<limits.h>
#include<math.h>
#include<io.h>
#include<iostream>
#include<setjmp.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<time.h>
//#include<unistd.h>
#include<thread>
#include<mutex>

using namespace std;


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
//#ifdef USE_SDL2

#include"../../../../Cpp/@ExtendLib/SDL2/include/SDL.h"
#include"../../../../Cpp/@ExtendLib/SDL2/include/SDL_syswm.h"
#include"../../../../Cpp/@ExtendLib/SDL2/include/SDL_image.h"
#include"../../../../Cpp/@ExtendLib/SDL2/include/SDL_ttf.h"
#include"../../../../Cpp/@ExtendLib/SDL2/include/SDL_mixer.h"
//#endif // USE_SDL2

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

#define PPT() printf("\nTime now : In %s at line %lu ,<%s> <%.10f s>,\n",__func__,__LINE__,Get_Time_Without_S(),(float)( (float)(clock())/CLOCKS_PER_SEC))
#define PPI(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%d>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPL(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%lld>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPD(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%le>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPF(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%f>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPU(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%llu>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPC(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%c>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPP(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%p>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
#define PPSS(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%s>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p))
//this str can not contan any ++ opts, because ti will use twice in this define line
#define PPS(p) printf("\n <%.10f s>In %s at line %lu , "#p" is <%s>,It's size is %llu\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__func__,__LINE__,(p),strlen(p))

#define PPW(p) printf("\nWarning : In %s at line %lu ,<%s>!!!,\n",__func__,__LINE__,(p))
//Push Warning

#define PPWs(p,q) printf("\nWarning : In %s at line %lu ,<%s %s>!!!,\n",__func__,__LINE__,(p),(q))
//Push Warning for a variate string q

/*STOP_IF_SCANF*/
#define _SIS_ {printf("\nStopping Now,Enter Anykey To Comtinue\nAt %s in line %lu  ",__func__,__LINE__);fflush(stdin);int c = _getch();printf("   Scanfed\n");fflush(stdin);}

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
#define LIST_FORG(type,p,pli) for(type* p = (type*)List_Get(pli); p != NULL; p = (type*)List_Get(pli))
#define LIST_FORR(type,p,pli,opts) for(type* p = (type*)List_Copy(pli); p != NULL; p = (type*)List_Copy(pli),opts)

char* mstrdup(const char* p);
int* intdup(int num, ...);
_LL* lldup(int num, ...);
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

bool Frees(char* ptype, ...);
// ... Must end with a NULL

char* GBK_To_UTF8(const char* pc);

//***********************************************************************************************************************
typedef struct ListDot ListDot;//先进先出
struct ListDot
{
	_ULL idx;//from 0
	_ULL usage;//for some open use
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
ListDot* List_CopyDot(List* plist);
void* List_ReverseCopy(List* plist);
void* List_Get(List* plist);
//Get the index dot content,from 0
void* List_Index(List* plist, _ULL index);
//Get the index dot,from 0
ListDot* List_IndexDot(List* plist, _ULL index);
void* List_IndexGet(List* plist, _ULL index);
List* List_Put(List* plist, void* pdata);
void List_SetVar(List* plist, void* pdata, void* newVar);
// end with a NULL
List* List_Gather(void* pData1, ...);
List* List_Destroy(List* plist);

//******************************************************************
//High level List
template <typename dataType>
class listDot
{
public:
	dataType* pdata = NULL;
	_ULL idx = 0;//from 0
	_ULL usage = 0;//for some open use
	listDot* pnext = NULL;
	listDot* ppre = NULL;

	listDot();
	listDot(dataType* _pdata);
	~listDot();
};
template <typename dataType>
class list
{
public:
	_ULL tik = 0;//标识符，区分List，或者用来储存一些ID信息
	_ULL sumque = 0;
	listDot<dataType>* pfirst = NULL;
	listDot<dataType>* plast = NULL;
	listDot<dataType>* now = NULL;


	list();
	// end with a NULL
	list(dataType* data1, ...);


	_ULL lastIndex = 0;
	listDot<dataType>* lastIndexDot = NULL;

	_ULL GetNowIndex();
	dataType* Copy();
	dataType* Get();
	//Get the index dot content,from 0
	dataType* IndexCopy(_ULL index);
	//Copy the index dot content,from 0
	dataType* IndexGet(_ULL index);
	list<dataType> Put(dataType* pdata);
	// end with a NULL
	list<dataType> Gather(dataType* pData1, ...);
	void Destroy(void);
	~list();

	// + 运算符重载, join tow list
	list<dataType> operator+(list<dataType>& other);
	// [] 运算符重载, IndexCopy
	dataType* operator[](_ULL index);
};



//***********************************************************************************************************************
//NOTICE: is it 100% safe that put lock opt into MyThreadQueue?
// may be there are many threads try to get the putDataQues[quePtr]
// try to get the mem block of putDataQues[quePtr]
// ?????????????????????????????????????????????
typedef struct MyThreadQue MyThreadQue;
struct MyThreadQue//FIFO node
{
	_ULL idx = 0;//from 0
	_ULL usage = 0;//for some open use
	MyThreadQue* pnext = NULL;
	MyThreadQue* ppre = NULL;
	void* pdata = NULL;
	int state = 0;//from 0, thrd_success thrd_timedout thrd_busy thrd_nomem thrd_error
};

class MyThreadQueue//FIFO
{
public:
	_ULL sumque = 0;
	MyThreadQue* now = NULL;
	MyThreadQue* pfirst = NULL;
	MyThreadQue* plast = NULL;
	int state = 0;//from 0, thrd_success thrd_timedout thrd_busy thrd_nomem thrd_error

	MyThreadQueue(void);
	bool Put(void* _pData, mutex* m);
	void* Get(mutex* m);
	_ULL Size(mutex* m);
};
////***********************************************************************************************************************

//***********************************************************************************************************************
//void (*_pF)(_ULL, MyThreadQueue&, MyThreadQueue&, MyThreadQueue&, void*)
//id, getQ, putQ, sig, data
class MyThreadsPool
{
public:
	_ULL sumThreads = 0;
	_ULL sumTasks = 0;
	MyThreadQueue sig = MyThreadQueue();
	MyThreadQueue* putDataQues = NULL;
	MyThreadQueue* getDataQues = NULL;
	_ULL quePtr = 0;
	char* name = NULL;
	thread** ppThs = NULL;

	List* mem = List_Init();

	//void (*pF)(MyThreadQueue*, MyThreadQueue*, MyThreadQueue, void*) = NULL;

	MyThreadsPool(void);
	MyThreadsPool(_ULL _sumThreads,
		void (*_pF)(_ULL, MyThreadQueue&, MyThreadQueue&, MyThreadQueue&, void*),
		void* otherData, const char* _name = NULL);

	void PutTask(void* pData, mutex* m);
	List* LoopToQuit(mutex* m, void* quitSig);
	void* Destroy(mutex* m);
};

//***********************************************************************************************************************



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

	List* exitFuncData;
	List* exitFunc;

	int (*Quit)(int retVal);
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
_ULL MyBA_AtQuit(int (*exitFunc)(void* data, int code, ...), void* data);
int MyBA_Quit(int retVal = 0);

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




//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************






template<typename dataType>
listDot<dataType>::listDot()
{
}

template<typename dataType>
inline listDot<dataType>::listDot(dataType* _pdata)
{
	pdata = _pdata;
}

template<typename dataType>
listDot<dataType>::~listDot()
{
}

template<typename dataType>
list<dataType>::list()
{
}

template<typename dataType>
list<dataType>::list(dataType* data1, ...)
{
	va_list parg;
	va_start(parg, data1);
	this->Put(data1);
	for (dataType* p = va_arg(parg, dataType*); p != NULL; p = va_arg(parg, dataType*))
		this->Put(p);
	va_end(parg);
}

template<typename dataType>
_ULL list<dataType>::GetNowIndex()
{
	if (now == NULL)
		return sumque;
	else
		return now->idx;
}

template<typename dataType>
dataType* list<dataType>::Copy()
{
	if (now == NULL)//Get the signal
	{
		now = pfirst;//ReSet
		return NULL;
	}
	dataType* pret = now->pdata;
	now = now->pnext;//If it means the end, it will report a NULL to rise a signal
	return pret;
}

template<typename dataType>
dataType* list<dataType>::Get()
{
	listDot<dataType>* pret = pfirst;
	if (sumque == 1)
	{
		now = pfirst = plast = NULL;
	}
	else if (sumque == 0)
	{
		return NULL;
	}
	else
	{
		if (now == pfirst)
			now = pfirst->pnext;
		ListDot* pte = pfirst->pnext;
		pte->ppre = NULL;
		pfirst = pte;
	}
	--sumque;
	dataType* pdata = pret->pdata;
	delete pret;
	return pdata;
}

template<typename dataType>
dataType* list<dataType>::IndexCopy(_ULL index)
{
	listDot<dataType>* p = pfirst;
	for (_ULL i = 0; (i < index) && (p != NULL); i++, p = p->pnext);
	return p->pdata;
}

template<typename dataType>
dataType* list<dataType>::IndexGet(_ULL index)
{
	listDot<dataType>* p = pfirst;
	for (_ULL i = 0; (i < index) && (p != NULL); i++, p = p->pnext);
	p->ppre->pnext = p->pnext;
	p->pnext->ppre = p->ppre;
	dataType* pret = p->pdata;
	delete p;
	return pret;
}

template<typename dataType>
list<dataType> list<dataType>::Put(dataType* pdata)
{
	++sumque;
	if (sumque == 1)
	{
		pfirst = new listDot<dataType>(pdata);
		if (pfirst == NULL)
		{
			MyBA_Err("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist", 1);
		}
		else
		{
			plast = pfirst;
			pfirst->pnext = pfirst->ppre = NULL;
			pfirst->idx = sumque - 1;
		}
	}
	else
	{
		listDot<dataType>* pte = new listDot<dataType>(pdata);
		if (pte == NULL)
		{
			MyBA_Err("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist", 1);
		}
		else
		{
			pte->ppre = plast;
			pte->pnext = NULL;
			plast->pnext = pte;
			plast = pte;
			pte->pdata = pdata;
			pte->idx = sumque - 1;
		}
	}
	return *this;
}

template<typename dataType>
list<dataType> list<dataType>::Gather(dataType* pData1, ...)
{
	va_list parg;
	va_start(parg, pData1);
	this->Put(pData1);
	for (dataType* p = va_arg(parg, dataType*); p != NULL; p = va_arg(parg, dataType*))
		this->Put(p);
	va_end(parg);
	return *this;
}

template<typename dataType>
void list<dataType>::Destroy(void)
{
	for (listDot<dataType>* p = pfirst, *pn = NULL; p; p = pn)
	{
		pn = p->pnext;
		delete p;
	}
}

template<typename dataType>
list<dataType>::~list()
{

}

template<typename dataType>
list<dataType> list<dataType>::operator+(list<dataType>& other)
{
	list<dataType>* ret = new list<dataType>;
	this->now = this->pfirst;
	for (dataType* p = this.Copy(); p; p = other.Copy())
		ret->Put(p);
	other.now = other.pfirst;
	for (dataType* p = other.Copy(); p; p = other.Copy())
		ret->Put(p);
	return *ret;
}

template<typename dataType>
dataType* list<dataType>::operator[](_ULL index)
{
	if (!lastIndexDot)
		lastIndexDot = pfirst;
	listDot<dataType>* pd = lastIndexDot;
	if (index > lastIndex)
		for (_ULL i = lastIndex; (i < index) && (pd != NULL); i++, pd = pd->pnext);
	else if (index < lastIndex)
		for (_ULL i = lastIndex; (i > index) && (pd != NULL); i--, pd = pd->ppre);
	//else//(index == lastIndex);
	lastIndex = index;
	lastIndexDot = pd;
	return pd->pdata;
}























#endif