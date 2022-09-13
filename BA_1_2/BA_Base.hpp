//BASIC_ALL_1_2
//H
//Writen By BHM
//2021年11月20日
//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_1_2_BASE_H
#define BA_1_2_BASE_H

#define __STDC_WANT_LIB_EXT1__ 1

#include <any>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <ctype.h>
#include <conio.h>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <locale.h>
#include <limits.h>
#include <map>
#include <math.h>
#include <io.h>
#include <iostream>
#include <random>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
//#include <unistd.h>
#include <thread>
#include <mutex>

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

#define BA_FREED_PTR (void*)0x1

#define PPT() printf("\nTime now : In %s\n, %s at line %lu ,<%s> <%.10f s>,\n",__FILE__, __func__,__LINE__,Get_Time_Without_S(),(float)( (float)(clock())/CLOCKS_PER_SEC))
#define PPX(p) cout << "\n <" << (float)( (float)(clock())/CLOCKS_PER_SEC) << " s>In " << __FILE__ << ", " << __func__ << " at line " << __LINE__ << " ," << #p << " is <" << p << ">\n" << endl;
// do not cacu it's len
#define PPSS(p) printf("\n <%.10f s>In %s\n, %s at line %lu , "#p" is <%s>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__FILE__,__func__,__LINE__,(p))
//this str can not contan any ++ opts, because ti will use twice in this define line
#define PPS(p) printf("\n <%.10f s>In %s\n, %s at line %lu , "#p" is <%s>,It's size is %llu\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__FILE__,__func__,__LINE__,(p),strlen(p))
//Push Warning
#define PPW(p) printf("\nWarning : In %s\n, %s at line %lu ,<%s>!!!,\n",__FILE__,__func__,__LINE__,(p))
//Push Warning for a variate string q
#define PPWs(p,q) printf("\nWarning : In %s\n, %s at line %lu ,<%s %s>!!!,\n",__FILE__,__func__,__LINE__,(p),(q))

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

#define _Get_File_Size(n,pf) unsigned long long n = 0ULL;{fseek(pf,0,SEEK_END);n = ftell(pf);fseek(pf,0,SEEK_SET);};

#define _CHECKP_(p, opts) {if((p) == NULL){PPW("NULL Pointer");opts;}}

#define LIST_FOR(p,pli) for(void* p = List_Copy(pli); p; p = List_Copy(pli))
#define LIST_FORS(type,p,pli) for(type* p = (type*)List_Copy(pli); p; p = (type*)List_Copy(pli))
#define LIST_FORG(type,p,pli) for(type* p = (type*)List_Get(pli); p; p = (type*)List_Get(pli))
#define LIST_FORR(type,p,pli,opts) for(type* p = (type*)List_Copy(pli); p; p = (type*)List_Copy(pli),opts)

#define FORI(start, end) for(_LL i = start; i < end; i++)

void JDT(_ULL now, _ULL sum);

void SetConsoleCursor(int x, int y);
COORD GetConsoleCursor(void);

char* StringAdd_L(const char* pstr, ...);//end with NULL
char* StringAdd_S(const char* pstr, ...);//end with NULL

int GetDayOfMonth(int year, int month);

char* Get_Time_Without_L(void);
char* Get_Time_Without_S(void);

char* Get_Time_L(void);
char* Get_Time_S(void);

char* Get_Time_For_File_Name(char char_to_replace_unspport_char);

// ... 表示被换的数字变量
char* Num_To_Char(const char* ptype, ...);

// ... Must end with a NULL
template<typename dataType>
bool Frees(dataType * p1, ...);

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
	_ULL tik = 0;//标识符，区分List，或者用来储存一些ID信息
	_ULL sumque = 0;
	ListDot* pfirst = NULL;
	ListDot* plast = NULL;
	ListDot* now = NULL;
	ListDot* reverse_now = NULL;

	List* (*Put)(List* plist, void* pdata);
	void* (*Get)(List* plist);
	void* (*Copy)(List* plist);
	void* (*ReverseCopy)(List* plist);
	void* (*Index)(List* plist, _ULL index);
	ListDot* (*IndexDot)(List* plist, _ULL index);
	void* (*IndexGet)(List* plist, _ULL index);
};
List* List_Init(void);
List* List_Init(void* pdata);
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
//******************************************************************

class BA_Base
{
public:


	List* mem = List_Init();

	char* __name__ = NULL;
	_ULL __len__ = 0;
	_ULL __idx__ = 0;

	BA_Base(_ULL idx = 0, const char* name = NULL);

};

 
//******************************************************************
int StrCmpById(const char* ptr1, const char* ptr2);

//NOTICE: is it 100% safe that put lock opt into MyThreadQueue?
// may be there are many threads try to get the putDataQues[quePtr]
// try to get the mem block of putDataQues[quePtr]
//High level List
template <typename dataType>
class balistDot
{
public:
	dataType* pdata = NULL;
	char* name = NULL;
	_LL idx = 0;//from 0
	_LL usage = 0;//for some open use
	balistDot* pnext = NULL;
	balistDot* ppre = NULL;

	balistDot();
	balistDot(dataType* _pdata, const char* _name = NULL, bool justUseNamePtr = false);
	~balistDot();
};
template <typename dataType>
class balist
{
public:
	_LL tik = 0;//标识符，区分List，或者用来储存一些ID信息
	_LL sumque = 0;
	balistDot<dataType>* pfirst = NULL;
	balistDot<dataType>* plast = NULL;
	balistDot<dataType>* now = NULL;
	balistDot<dataType>* medium = NULL;

	//(dataType*)(0x1)
	dataType* errPtr = (dataType*)(0x1);

	int (*strCmpFunc)(const char* ptr1, const char* ptr2) = strcmp;

	balist();
	// end with a NULL
	balist(dataType* data1, ...);

	_LL GetNowIndex();
	_LL Index(dataType* data);
	dataType* Copy();
	dataType* Get();
	//Get the index dot content,from 0
	dataType* Copy(_LL index);
	//Copy the index dot content,from 0
	dataType* Get(_LL index);
	//Get the name dot content
	dataType* Get(const char* name, bool justCmpNameById = false, bool freeName = true);
	//Copy the name dot content
	dataType* Copy(const char* name, bool justCmpNameById = false);
	// put as plast
	void Put(dataType* pdata,
		const char* name = NULL, bool justUseNamePtr = false);
	// use dot::usage as hash key to insert a data between two hash dot
	void Insert(dataType* pdata, _LL hashKey,
		void (*hashCollisionFunc)(balistDot<dataType>* pOriDot, balistDot<dataType>* pNowDot),
		const char* name = NULL, bool justUseNamePtr = false);
	// multi threads
	void ThrPut(dataType* pdata, mutex* m,
		const char* name = NULL, bool justUseNamePtr = false);
	// multi threads
	dataType* ThrGet(mutex* m);
	// multi threads
	_LL ThrSize(mutex* m);
	// end with a NULL
	balist<dataType> Gather(dataType* pData1, ...);
	void Destroy(void);
	~balist();

	// + 运算符重载, join tow balist
	balist<dataType> operator+(balist<dataType>& other);
	// [] 运算符重载, IndexCopy
	dataType* operator[](_LL index);
	// [] 运算符重载, NameCopy
	dataType* operator[](const char* name);
	// () 运算符重载, setVar via data, freeOld[0=No, 1=free(), 2=MyBA_Free
	void operator()(dataType* data, dataType* newVar, bool freeOld = true);
	// () 运算符重载, setVar via index
	void operator()(_LL index, dataType* newVar, bool freeOld = true);
	// () 运算符重载, setVar via name
	void operator()(const char* name, dataType* newVar, bool freeOld = true, bool justUseNamePtr = false);
};
//******************************************************************
//High level badict
class badictPair
{
public:
	char* key = NULL;
	any data;
	_LL idx = 0;//from 0
	_ULL usage = 0;//for some open use
	badictPair* pnext = NULL;
	badictPair* ppre = NULL;

	badictPair();
	badictPair(const char* _key, any _data, bool _justUseKeyPtr = false);
	~badictPair();

	void operator=(any _data);
};
class badict
{
public:
	_LL tik = 0;//标识符，区分List，或者用来储存一些ID信息
	_LL sumque = 0;
	badictPair* pfirst = NULL;
	badictPair* plast = NULL;

	bool justUseKeyPtr = false;

	int (*strCmpFunc)(const char* ptr1, const char* ptr2) = strcmp;

	badict(bool _justUseKeyPtr = false);
	// end with a NULL
	badict(const char* key, any data, bool _justUseKeyPtr = false);

	bool HasKey(const char* key, bool justCmpKeyById = false);
	//Get the data to key
	template <typename dataType> dataType Copy(const char* key, bool justCmpKeyById = false);
	badict Put(const char* key, any data, bool _justUseKeyPtr = false);
	// del
	bool Del(const char* key, bool freeKey = true);
	void Destroy(void);
	~badict();

	// + 运算符重载, join tow badict
	badict operator+(badict& other);
	// [] 运算符重载, 获取键值对的引用，便于对键值对赋值
	//use in obj justUseKeyPtr when create pair
	badictPair& operator[](const char* key);
	// () 运算符重载, setVar via key
	badict operator()(const char* _key, any _data);
};



//***********************************************************************************************************************
//void (*_pF)(_ULL, MyThreadQueue&, MyThreadQueue&, MyThreadQueue&, void*)
//id, getQ, putQ, sig, data

template<typename dataTypePut, typename dataTypeGet>
class MyThreadsPool
{
public:
	_ULL sumThreads = 0;
	_ULL sumTasks = 0;
	balist<bool> sig = balist<bool>();
	balist<dataTypePut>* putDataQues = NULL;
	balist<dataTypeGet>* getDataQues = NULL;
	balist<float>* procQues = NULL;
	_ULL quePtr = 0;
	char* name = NULL;
	thread** ppThs = NULL;
	COORD pos = COORD();

	List* mem = List_Init();

	MyThreadsPool(_ULL _sumThreads,
		void (*_pF)(_ULL, balist<dataTypePut>&, balist<dataTypeGet>&, balist<float>&, balist<bool>&, void*),
		void* otherData = NULL, const char* _name = NULL);

	void PutTask(dataTypePut* pData, mutex* m);
	List* LoopToQuit(mutex* m);
	void Destroy(mutex* m);
};

//***********************************************************************************************************************


char* GetTimeWithout(List* mem = NULL);


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

	default_random_engine randomEngine;

	bool isSDL2;
	bool isSAFEMODE;

	List* pLog;

	List* exitFuncData;
	List* exitFunc;

	int (*Quit)(int retVal);
	float (*GUT)(void);
	// BALLOC_L
	void (*PutLog)(const char* pc, const char* head);
};
extern MyBA* pba;
void MyBA_Init(void);
void MyBA_Context(const char* nowFuncName);
float MyBA_GetUsedTime(void);
// BALLOC_L
void MyBA_PutLog(const char* pc, const char* head = "Normal Log:");
// end with NULL & BALLOC_L
void MyBA_PutLogs(const char* head = "Normal Log:", ...);
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

// if mem == NULL, do not record
char* mstrdup(const char* p, List* mem = NULL);
//_ULL* pi = TypeDupR(mem, 2, 99ULL, 99ULL);
// if mem == NULL, use MCALLOC
template<typename dataType>
dataType* TypeDupR(List* mem, _ULL num, dataType firstData, ...);
// random, [start, end) when type is int
template<typename dataType>
dataType RandNum(dataType start, dataType end);
// random, [start, end) for int, [start, end] for float
// if mem is NULL, using MCALLOC
template<typename dataType>
dataType* RandNum(dataType start, dataType end, _LL len, List* mem = NULL);


//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************
//***********************************************************************************************************************


template<typename dataType>
bool Frees(dataType* p1, ...)
{
	va_list parg;
	va_start(parg, p1);
	dataType* pte = p1;
	for (; pte; pte = va_arg(parg, dataType*))
		free(pte);
	va_end(parg);
	return 0;
};


//_ULL* pi = TypeDupR(mem, 2, 99ULL, 99ULL);
// if mem == NULL, use MCALLOC
template<typename dataType>
dataType* TypeDupR(List* mem, _ULL num, dataType firstData, ...)
{
	dataType* pret = NULL;
	if (mem)
	{
		pret = BALLOC_R(num, dataType, mem);
	}
	else
	{
		pret = MCALLOC(num, dataType);
	}
	if (pret)
	{
		pret[0] = firstData;
		va_list parg;
		va_start(parg, firstData);
		if(typeid(dataType) == typeid(float))
			for (_ULL a = 1; a < num; a++)
				pret[a] = (dataType)va_arg(parg, double);
		else
			for (_ULL a = 1; a < num; a++)
				pret[a] = (dataType)va_arg(parg, dataType);
		va_end(parg);
	}
	return pret;
}

template<typename dataType>
inline dataType RandNum(dataType start, dataType end)
{
	if (typeid(dataType) == typeid(float))
	{
		uniform_real_distribution<float> dis(start, end);
		return dis(pba->randomEngine);
	}
	else
	{
		uniform_int_distribution<int> dis(start, end - 1);
		return dis(pba->randomEngine);
	}
}

template<typename dataType>
inline dataType* RandNum(dataType start, dataType end, _LL len, List* mem)
{
	dataType* ret = NULL;
	if (mem)
	{
		ret = BALLOC_R(len, dataType, mem);
	}
	else
	{
		ret = MCALLOC(len, dataType);
	}
	if (typeid(dataType) == typeid(float))
	{
		uniform_real_distribution<float> dis(start, end);
		for (_LL i = 0; i < len; i++)
			ret[i] = dis(pba->randomEngine);
	}
	else
	{
		uniform_int_distribution<int> dis(start, end-1);
		for (_LL i = 0; i < len; i++)
			ret[i] = dis(pba->randomEngine);
	}
	return ret;
}

//***********************************************************************************************************************
//***********************************************************************************************************************


template<typename dataType>
balistDot<dataType>::balistDot()
{
}

template<typename dataType>
inline balistDot<dataType>::balistDot(dataType* _pdata, const char* _name, bool justUseNamePtr)
{
	pdata = _pdata;
	if (_name)
	{
		if (justUseNamePtr)
		{
			_ULL id = (_ULL)_name;
			name = (char*)_name;
		}
		else
		{
			name = _strdup(_name);
		}
	}
}

template<typename dataType>
balistDot<dataType>::~balistDot()
{
}

template<typename dataType>
balist<dataType>::balist()
{
}

template<typename dataType>
balist<dataType>::balist(dataType* data1, ...)
{
	va_list parg;
	va_start(parg, data1);
	this->Put(data1);
	for (dataType* p = va_arg(parg, dataType*); p != NULL; p = va_arg(parg, dataType*))
		this->Put(p);
	va_end(parg);
}

template<typename dataType>
_LL balist<dataType>::GetNowIndex()
{
	if (now == NULL)
		return sumque;
	else
		return now->idx;
}

template<typename dataType>
_LL balist<dataType>::Index(dataType* data)
{
	balistDot<dataType>* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (pd->pdata == data)
			return (_LL)(pd->idx);
	return -1;
}

template<typename dataType>
dataType* balist<dataType>::Copy()
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
dataType* balist<dataType>::Get()
{
	balistDot<dataType>* pret = pfirst;
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
		balistDot< dataType>* pte = pfirst->pnext;
		pte->ppre = NULL;
		pfirst = pte;
	}
	--sumque;
	dataType* pdata = pret->pdata;
	delete pret;
	return pdata;
}

template<typename dataType>
dataType* balist<dataType>::Copy(_LL index)
{
	if (index > sumque-1 || index < -(sumque))
		return (dataType*)(0x1);
	if (index < 0 && index >= -(sumque))
		index = sumque + index;
	balistDot<dataType>* pd = pfirst;
	for (_LL i = 0; (i < index) && (p != NULL); i++, pd = pd->pnext);
	return pd->pdata;
}

template<typename dataType>
dataType* balist<dataType>::Get(_LL index)
{
	balistDot<dataType>* p = pfirst;
	for (_LL i = 0; (i < index) && (p != NULL); i++, p = p->pnext);
	if (p == pfirst)
	{
		return this->Get();
	}
	else if (p == plast)
	{
		plast = p->ppre;
		p->ppre->pnext = NULL;
	}
	else
	{
		p->ppre->pnext = p->pnext;
		p->pnext->ppre = p->ppre;
	}
	dataType* pret = p->pdata;
	delete p;
	return pret;
}

template<typename dataType>
inline dataType* balist<dataType>::Get(const char* name, bool justCmpNameById, bool freeName)
{
	if (justCmpNameById)
		strCmpFunc = StrCmpById;
	else
		strCmpFunc = strcmp;
	balistDot<dataType>* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (pd->name && strCmpFunc(pd->name, name) == 0)
		{
			if (pd == pfirst)
			{
				pfirst = pd->pnext;
				if (pd->pnext)
					pd->pnext->ppre = NULL;
			}
			else if (pd == plast)
			{
				plast = pd->ppre;
				if (pd->ppre)
					pd->ppre->pnext = NULL;
			}
			else
			{
				pd->ppre->pnext = pd->pnext;
				pd->pnext->ppre = pd->ppre;
			}
			if(pd->name && freeName)
				free(pd->name);
			dataType* pret = pd->pdata;
			delete pd;
			--sumque;
			return pret;
		}
	return NULL;
}

template<typename dataType>
inline dataType* balist<dataType>::Copy(const char* name, bool justCmpNameById)
{
	if (justCmpNameById)
		strCmpFunc = StrCmpById;
	else
		strCmpFunc = strcmp;
	balistDot<dataType>* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (pd->name && strCmpFunc(pd->name, name) == 0)
			return pd->pdata;
	return (dataType*)0x1;
}

template<typename dataType>
void balist<dataType>::Put(dataType* pdata, const char* name, bool justUseNamePtr)
{
	++sumque;
	if (sumque == 1)
	{
		pfirst = new balistDot<dataType>(pdata, name, justUseNamePtr);
		if (pfirst == NULL)
		{
			MyBA_Err("List* List_Put(List* pbalist, void* pdata): MCALLOC(1, ListDot) == NULL, return pbalist", 1);
		}
		else
		{
			now = plast = pfirst;
			pfirst->pnext = pfirst->ppre = NULL;
			pfirst->idx = 0;
		}
	}
	else
	{
		balistDot<dataType>* pte = new balistDot<dataType>(pdata, name);
		if (pte == NULL)
		{
			MyBA_Err("List* List_Put(List* pbalist, void* pdata): MCALLOC(1, ListDot) == NULL, return pbalist", 1);
		}
		else
		{
			pte->ppre = plast;
			pte->pnext = NULL;
			plast->pnext = pte;
			plast = pte;
			pte->idx = plast->idx + 1;
		}
	}
}

template<typename dataType>
void balist<dataType>::Insert(dataType* pdata, _LL hashKey,
	void (*hashCollisionFunc)(balistDot<dataType>* pOriDot, balistDot<dataType>* pNowDot),
	const char* name, bool justUseNamePtr)
{
	balistDot<dataType>* pOriDot = NULL;
	balistDot<dataType>* pNowDot = new balistDot<dataType>(pdata, name, justUseNamePtr);
	pNowDot->usage = hashKey;
	++sumque;
	if (sumque == 1)// init as first
	{
		medium = now = plast = pfirst = pNowDot;
		pfirst->pnext = pfirst->ppre = NULL;
	}
	else if (hashKey < pfirst->usage)// insert as first
	{
		pNowDot->pnext = pfirst;
		pfirst->ppre = pNowDot;
		pfirst = pNowDot;
		if (medium->ppre)// move to first
			medium = medium->ppre;
	}
	else if (plast->usage < hashKey)// insert as last
	{
		pNowDot->ppre = plast;
		plast->pnext = pNowDot;
		plast = pNowDot;
		if (medium->pnext)// move to last
			medium = medium->pnext;
	}
	else// insert as medium
	{
		if (hashKey >= medium->usage)// insert as medium->next
		{
			for (pOriDot = medium; pOriDot; pOriDot = pOriDot->pnext)
			{
				if (pOriDot->usage < hashKey && hashKey < pOriDot->pnext->usage)
					break;
				else if (pOriDot->usage == hashKey)
					return hashCollisionFunc(pOriDot, pNowDot);
				// NO else;
			}
			if (medium->pnext)// move to last
				medium = medium->pnext;
		}
		else// if(hashKey < medium->usage)// insert as medium->pre
		{
			for (pOriDot = medium; pOriDot; pOriDot = pOriDot->ppre)
			{
				if (pOriDot->usage < hashKey && hashKey < pOriDot->pnext->usage)
					break;
				else if (pOriDot->usage == hashKey)
					return hashCollisionFunc(pOriDot, pNowDot);
				// NO else;
			}
			if (medium->ppre)// move to first
				medium = medium->ppre;
		}
		// now <=> ori->pnext
		if(pOriDot)
			pOriDot->pnext->ppre = pNowDot;
		pNowDot->pnext = pOriDot->pnext;
		// ori <=> now
		pOriDot->pnext = pNowDot;
		pNowDot->ppre = pOriDot;
	}
}

template<typename dataType>
void balist<dataType>::ThrPut(dataType* pdata, mutex* m, const char* name, bool justUseNamePtr)
{
	m->lock();
	this->Put(pdata, name, justUseNamePtr);
	m->unlock();
}

template<typename dataType>
dataType* balist<dataType>::ThrGet(mutex* m)
{
	dataType* ret = NULL;
balist_Label_A:
	m->lock();
	if (sumque == 0 || (!pfirst))
	{
		m->unlock();
		Sleep(100);
		goto balist_Label_A;
	}
	else
	{
		ret = this->Get();
	}
	m->unlock();
	return ret;
}

template<typename dataType>
_LL balist<dataType>::ThrSize(mutex* m)
{
	m->lock();
	_LL ret = sumque;
	m->unlock();
	return ret;
}

template<typename dataType>
balist<dataType> balist<dataType>::Gather(dataType* pData1, ...)
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
void balist<dataType>::Destroy(void)
{
	for (balistDot<dataType>* p = pfirst, *pn = NULL; p; p = pn)
	{
		pn = p->pnext;
		free(p->name);
		delete p;
	}
}

template<typename dataType>
inline balist<dataType>::~balist()
{

}

template<typename dataType>
inline balist<dataType> balist<dataType>::operator+(balist<dataType>& other)
{
	balist<dataType>* ret = new balist<dataType>;
	this->now = this->pfirst;
	for (dataType* p = this.Copy(); p; p = other.Copy())
		ret->Put(p);
	other.now = other.pfirst;
	for (dataType* p = other.Copy(); p; p = other.Copy())
		ret->Put(p);
	return *ret;
}

template<typename dataType>
inline dataType* balist<dataType>::operator[](_LL index)
{
	return this->Copy(index);
}

template<typename dataType>
inline dataType* balist<dataType>::operator[](const char* name)
{
	balistDot<dataType>* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (pd->name && strcmp(pd->name, name) == 0)
			return pd->pdata;
	return (dataType*)0x1;
}

template<typename dataType>
inline void balist<dataType>::operator()(dataType* data, dataType* newVar, bool freeOld)
{
	balistDot<dataType>* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (pd->pdata == data)
		{
			if (freeOld)
				free(pd->pdata);
			pd->pdata = newVar;
			return ;
		}
	this->Put(newVar);
}

template<typename dataType>
inline void balist<dataType>::operator()(_LL index, dataType* newVar, bool freeOld)
{
	balistDot< dataType>* pd = pfirst;
	for (_LL i = 0; (i < index) && (pd != NULL); i++, pd = pd->pnext);
	if (pd)
	{
		if (freeOld)
			free(pd->pdata);
		pd->pdata = newVar;
	}
	this->Put(newVar);
}

template<typename dataType>
inline void balist<dataType>::operator()(const char* name, dataType* newVar, bool freeOld, bool justUseNamePtr)
{
	balistDot<dataType>* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (pd->name && strcmp(pd->name, name) == 0)
		{
			if (freeOld)
				free(pd->pdata);
			pd->pdata = newVar;
			return;
		}
	this->Put(newVar, name, justUseNamePtr);
}


//***********************************************************************************************************************
//***********************************************************************************************************************



template<typename dataType>
inline dataType badict::Copy(const char* key, bool justCmpKeyById)
{
	badictPair* pd = pfirst;
	if (justCmpKeyById)
		strCmpFunc = StrCmpById;
	else
		strCmpFunc = strcmp;
	for (; pd; pd = pd->pnext)
		if (!strCmpFunc(pd->key, key))
			return any_cast<dataType>(pd->data);
	PPWs("No Such Key: ", key);
	// dataType NULL retrun
	// if there is no func, C2512 will be caused
	return dataType();
}

//use in obj justUseKeyPtr when create pair
inline badictPair& badict::operator[](const char* key)
{
	badictPair* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (!strcmp(pd->key, key))
		{// has key, return the pair
			badictPair& d = *pd;
			return d;
		}
	// do not has the key, create the pair with 0LL data
	// work with badictPair = operator func
	this->Put(key, 0LL, justUseKeyPtr);
	//return new pair
	badictPair& d = *plast;
	return d;
}



//***********************************************************************************************************************
//***********************************************************************************************************************



template<typename dataTypePut, typename dataTypeGet>
MyThreadsPool<dataTypePut, dataTypeGet>::MyThreadsPool(_ULL _sumThreads,
	void(*_pF)(_ULL, balist<dataTypePut>&, balist<dataTypeGet>&,
		balist<float>&, balist<bool>&, void*), void* otherData, const char* _name)
{
	sumThreads = _sumThreads;
	ppThs = BALLOC_R(_sumThreads, thread*, mem);
	putDataQues = new balist< dataTypePut>[_sumThreads];
	getDataQues = new balist< dataTypeGet>[_sumThreads];
	procQues = new balist< float>[_sumThreads];
	//pF = _pF;
	if (!_name)
		name = mstrdup("MyThreadsPool", mem);
	else
		name = mstrdup(_name, mem);
	for (_ULL i = 0; i < sumThreads; i++)
	{
		ppThs[i] = new thread(_pF, i, ref(putDataQues[i]),
			ref(getDataQues[i]), ref(procQues[i]), ref(sig), otherData);
	}
}

template<typename dataTypePut, typename dataTypeGet>
inline void MyThreadsPool<dataTypePut, dataTypeGet>::PutTask(dataTypePut* pData, mutex* m)
{
	putDataQues[quePtr].ThrPut(pData, m);
	quePtr = ((quePtr + 1) < sumThreads) ? (quePtr + 1) : 0;
	sumTasks += 1;
}

//be sure that all tasks sended, this func will
//send 'wait to quit' signal to every que,
//and start to loop waiting
template<typename dataTypePut, typename dataTypeGet>
inline List* MyThreadsPool<dataTypePut, dataTypeGet>::LoopToQuit(mutex* m)
{
	List* retList = List_Init();
	for (_ULL idx = 0; idx < sumThreads; idx++)
		putDataQues[idx].ThrPut((dataTypePut*)0x1, m);
	_ULL sumTasksTillNow = 0;
	float st = (float)clock();
	float* hisProc = BALLOC_R(sumThreads, float, mem);
	pos = GetConsoleCursor();
	while (sig.ThrSize(m) < sumThreads)
	{
		sumTasksTillNow = 0;
		for (_ULL idx = 0; idx < sumThreads; idx++)
			sumTasksTillNow += putDataQues[idx].ThrSize(m);

		SetConsoleCursor(pos.X, pos.Y);
		printf("%20s: subThreads working: %5llu / %5llu  --  %8.3f sec\n",
			name, sumTasksTillNow, sumTasks, (float)(clock() - st) / CLOCKS_PER_SEC);
		for (_ULL idx = 0; idx < sumThreads; idx++)
		{
			hisProc[idx] = procQues[idx].ThrSize(m) > 0 ?
				*(procQues[idx].ThrGet(m)) : hisProc[idx];
			printf("subThreads %llu : %6.3f %%        \n", idx, hisProc[idx]);
		}

		for (_ULL idx = 0; idx < sumThreads; idx++)
			while (getDataQues[idx].ThrSize(m) > 0)
				List_Put(retList, getDataQues[idx].ThrGet(m));
		Sleep(1000);
	}
	for (_ULL idx = 0; idx < sumThreads; idx++)
	{
		while (getDataQues[idx].ThrSize(m) > 0)
			List_Put(retList, getDataQues[idx].ThrGet(m));
		ppThs[idx]->join();
		printf("\r%s: waiting to join subThreads: %llu / %llu  --  %8.3f sec",
			name, sumTasksTillNow, sumTasks,
			(float)(clock() - st) / CLOCKS_PER_SEC);
	}
	return retList;
}

template<typename dataTypePut, typename dataTypeGet>
inline void MyThreadsPool<dataTypePut, dataTypeGet>::Destroy(mutex* m)
{
	MyBA_Free_R(mem);
	List_Destroy(mem);
	delete[] getDataQues;
	delete[] putDataQues;
	delete[] procQues;
}









#endif

