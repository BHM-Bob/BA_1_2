//BASIC_ALL_1_2
//H
//Writen By BHM
//2021年11月20日
//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_1_2_BASE_H
#define BA_1_2_BASE_H

#define __STDC_WANT_LIB_EXT1__ 1

// C
#include <assert.h>
#include <ctype.h>
#include <direct.h>
#include <locale.h>
#include <limits.h>
#include <setjmp.h>
// std
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
// STL
#include <algorithm>
#include <numeric>
#include <functional>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
// type
#include <any>
#include <tuple>
#include <string>
#include <sys/types.h>
// IO
#include <conio.h>
#include <io.h>
#include <iostream>
#include <filesystem>
#include <fstream>
// thread
#include <mutex>
#include <thread>
// windows platform for console cursor OP and string code transfer
#include <Windows.h>
#include <WinBase.h>

/*
D:\soft\opencv\opencv\build\include\opencv2
D:\soft\opencv\opencv\build\x64\vc15\lib\opencv_world454d.lib
D:\soft\opencv\opencv\build\x64\vc15\lib
*/
#ifdef USE_OPENCV
#include<opencv2/opencv.hpp>
#endif

typedef unsigned long long _ULL;
typedef long long _LL;

#define BA_FREED_PTR (void*)0x1

#define PPT() printf("\nTime now : In %s\n, %s at line %lu ,<%s> <%.10f s>,\n",__FILE__, __func__,__LINE__,GetTimeWithout(pba->STmem),(float)( (float)(clock())/CLOCKS_PER_SEC))
#define PPX(p) std::cout << "\n <" << (float)( (float)(clock())/CLOCKS_PER_SEC) << " s>In " << __FILE__ << ", " << __func__ << " at line " << __LINE__ << " ," << #p << " is <" << p << ">\n" << std::endl;
// do not cacu it's len
#define PPSS(p) printf("\n <%.10f s>In %s\n, %s at line %lu , "#p" is <%s>\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__FILE__,__func__,__LINE__,(p))
//this str can not contan any ++ opts, because ti will use twice in this define line
#define PPS(p) printf("\n <%.10f s>In %s\n, %s at line %lu , "#p" is <%s>,It's size is %llu\n",(float)( (float)(clock())/CLOCKS_PER_SEC),__FILE__,__func__,__LINE__,(p),strlen(p))
//Push Warning
#define PPW(p) printf("\nWarning : In %s\n, %s at line %lu ,<%s>!!!,\n",__FILE__,__func__,__LINE__,(p))
//Push Warning for a variate string q
#define PPWs(p,q) printf("\nWarning : In %s\n, %s at line %lu ,<%s %s>!!!,\n",__FILE__,__func__,__LINE__,(p),(q))

/*STOP_IF_SCANF*/
#define _SIS_ {printf("\nStopping Now,Enter Any Key To Continue\nAt %s in line %lu  ",__func__,__LINE__);fflush(stdin);int c = _getch();printf("   Scanfed\n");fflush(stdin);}

#define MCALLOC(num,type) (type*)calloc((size_t)(num),sizeof(type))

#define BALLOC_L(num,type) (type*)MyBA_CALLOC_L((size_t)(num),sizeof(type))
#define BALLOC_S(num,type) (type*)MyBA_CALLOC_S((size_t)(num),sizeof(type))

#define BALLOCS_L(type,ret,num,err_ret,err_opts) type* ret = (type*)MyBA_CALLOC_L((size_t)(num),sizeof(type));if((ret) == NULL){err_opts;return err_ret;}
#define BALLOCS_S(type,ret,num,err_ret,err_opts) type* ret = (type*)MyBA_CALLOC_S((size_t)(num),sizeof(type));if((ret) == NULL){err_opts;return err_ret;}

#define BALLOC_R(num,type,pli) (type*)MyBA_CALLOC_R((size_t)(num),sizeof(type),pli)

#define _BA_ERR_(pc) MyBA_Errs(1, __FILE__, " : ", __func__, " : ", __LINE__, "\n: ", pc, NULL)

#define LIST_FORG(type,p,pli) for(type* p = (type*)List_Get(pli); p; p = (type*)List_Get(pli))
#define FORI(start, end) for(_LL i = start; i < end; i++)

void JDT(_ULL now, _ULL sum);

// TODO : remove windows dependence
void SetConsoleCursor(int x, int y);
COORD GetConsoleCursor(void);

int GetDayOfMonth(int year, int month);

char* Get_Time_For_File_Name(char char_to_replace_unspport_char);

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

	List* (*Put)(List* plist, void* pdata);
	void* (*Get)(List* plist);
	void* (*Copy)(List* plist);
	void* (*Index)(List* plist, _ULL index);
};
List* List_Init(void* pdata = NULL);
void* List_Copy(List* plist);
void* List_Get(List* plist);
//Get the index dot content,from 0
void* List_Index(List* plist, _ULL index);
List* List_Put(List* plist, void* pdata);
void List_SetVar(List* plist, void* pdata, void* newVar);
List* List_Destroy(List* plist);

#define BA_LIST_PTR (List*)0x1

// func that use List
//******************************************************************


char* GetTimeWithout(List* mem = NULL);
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
	balistDot< dataType>* CopyDot(_LL index);
	//Copy the index dot content,from 0
	dataType* Get(_LL index);
	//Copy the name dot content
	dataType* Copy(const char* name, bool justCmpNameById = false);
	//Get the name dot content
	dataType* Get(const char* name, bool justCmpNameById = false, bool freeName = true);
	// put as plast
	void Put(dataType* pdata,
		const char* name = NULL, bool justUseNamePtr = false);
	// use dot::usage as hash key to insert a data between two hash dot
	void Insert(dataType* pdata, _LL hashKey,
		void (*hashCollisionFunc)(balistDot<dataType>* pOriDot, balistDot<dataType>* pNowDot),
		const char* name = NULL, bool justUseNamePtr = false);
	// multi threads
	void ThrPut(dataType* pdata, std::mutex* m,
		const char* name = NULL, bool justUseNamePtr = false);
	// multi threads
	dataType* ThrGet(std::mutex* m, _LL wait = 100, _LL block = -1);
	// multi threads
	_LL ThrSize(std::mutex* m);
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
	std::any data;
	_LL idx = 0;//from 0
	_ULL usage = 0;//for some open use
	badictPair* pnext = NULL;
	badictPair* ppre = NULL;

	badictPair();
	badictPair(const char* _key, std::any _data, bool _justUseKeyPtr = false);
	~badictPair();

	void operator=(std::any _data);
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
	badict(const char* key, std::any data, bool _justUseKeyPtr = false);

	bool HasKey(const char* key, bool justCmpKeyById = false);
	//Get the data to key
	template <typename dataType> dataType Copy(const char* key, bool justCmpKeyById = false);
	badict Put(const char* key, std::any data, bool _justUseKeyPtr = false);
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
	badict operator()(const char* _key, std::any _data);
};

//***********************************************************************************************************************
typedef struct BALog BALog;
struct BALog
{
	clock_t t;
	char* pdate;
	char* pc;
	int code;
};

namespace ba
{
	class stack;

	// end with a NULL
	char* StrAdd(List* mem, const char* pstr, ...);
}

char* StringWrite(FILE* pf, char* pc);
//BALLOC_S
char* StringRead(FILE* pf, List* mem = NULL);

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

	ba::stack* stacks;//stacks for next generation of mem control and log

	clock_t GUT_t;
	bool GUT_state;
	clock_t JDT_t;

	std::default_random_engine randomEngine;
	std::random_device randomDevice;
	std::mt19937 fastRandom = std::mt19937(randomDevice);

	bool isSAFEMODE;

	List* pLog;

	List* exitFuncData;
	List* exitFunc;

	float (*GUT)(bool reset);

	std::map<std::string, bool> moudleState;
};
extern MyBA* pba;
void MyBA_Init(int argc = 1, char** argvs = NULL, bool safeMode = false);
float MyBA_GetUsedTime(bool reset = false);
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
List* MyBA_Free_R(List* pli, bool destoryList = false);
void MyBA_FreeInstance(void);
void* MyBA_CALLOC_R(_ULL count, _ULL size, List* pli);
void* MyBA_CALLOC_L(_ULL count, _ULL size);
void* MyBA_CALLOC_S(_ULL count, _ULL size);

void MyBA_CMD_Warrper(int argc = 1, char** argvs = NULL);
void MyBA_SafeMode(void);
//***********************************************************************************************************************

// if mem == NULL, do not record
char* mstrdup(const char* p, List* mem = NULL);
//_ULL* pi = TypeDupR(mem, 2, 99ULL, 99ULL);
// if mem == NULL, use MCALLOC
float* TypeDupR(List* mem, _ULL num, float firstData, ...);
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

template<typename dataType>
dataType* TypeDupR(List* mem, _ULL num, dataType firstData, ...)
{
	dataType* pret = BALLOC_R(num, dataType, mem);
	if (pret)
	{
		pret[0] = firstData;
		va_list parg;
		va_start(parg, firstData);
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
		std::uniform_real_distribution<float> dis(start, end);
		return dis(pba->randomEngine);
	}
	else
	{
		std::uniform_int_distribution<int> dis(start, end - 1);
		return dis(pba->randomEngine);
	}
}

template<typename dataType>
inline dataType* RandNum(dataType start, dataType end, _LL len, List* mem)
{
	dataType* ret = BALLOC_R(len, dataType, mem);
	if(ret)
	{
		if (typeid(dataType) == typeid(float))
		{
			std::uniform_real_distribution<float> dis((float)start, (float)end);
			for (_LL i = 0; i < len; i++)
				ret[i] = (float)dis(pba->randomEngine);
		}
		else
		{
			std::uniform_int_distribution<int> dis((int)start, (int)end - 1);
			for (_LL i = 0; i < len; i++)
				ret[i] = (int)dis(pba->randomEngine);
		}
		return ret;
	}
	else
	{
		PPW("OOM Err");
		return (dataType*)1;
	}
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
	for (_LL i = 0; (i < index) && (pd != NULL); i++, pd = pd->pnext);
	return pd ? pd->pdata : errPtr;
}

template<typename dataType>
inline balistDot<dataType>* balist<dataType>::CopyDot(_LL index)
{
	if (index > sumque - 1 || index < -(sumque))
		return (balistDot<dataType>*)(0x1);
	if (index < 0 && index >= -(sumque))
		index = sumque + index;
	balistDot<dataType>* pd = pfirst;
	for (_LL i = 0; (i < index) && (pd != NULL); i++, pd = pd->pnext);
	return pd;
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
	--sumque;
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
			pte->idx = plast->idx + 1;
			pte->ppre = plast;
			pte->pnext = NULL;
			plast->pnext = pte;
			plast = pte;
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
	else if (hashKey <= pfirst->usage)// insert as first
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
void balist<dataType>::ThrPut(dataType* pdata, std::mutex* m, const char* name, bool justUseNamePtr)
{
	m->lock();
	this->Put(pdata, name, justUseNamePtr);
	m->unlock();
}

template<typename dataType>
dataType* balist<dataType>::ThrGet(std::mutex* m, _LL wait, _LL block)
{
	dataType* ret = NULL;
	_LL sumWait = 0;
balist_Label_A:
	m->lock();
	if (sumque == 0 || (!pfirst))
	{
		m->unlock();
		Sleep((DWORD)wait);
		sumWait += wait;
		if (block > 0 && block < sumWait)
			return (dataType*)0;
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
_LL balist<dataType>::ThrSize(std::mutex* m)
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

#endif

