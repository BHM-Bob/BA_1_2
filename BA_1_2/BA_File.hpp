﻿//BASIC_ALL_1_2 BA_FILE_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_FILE_H
#define BA_FILE_H

#include"BA_Base.hpp"

char* StringWrite(FILE* pf, char* pc);

//BALLOC_S
char* StringRead(FILE* pf, List* mem = NULL);

unsigned long long  Get_File_Size(FILE* pf);

char* ReadTXT(const char* path, _ULL loadSize = 0);

char* Get_File_Type(char* ppath);

bool Check_File_Exist(char* path);

bool Encrypt_File(char* path, char* Passwords);

typedef struct TextIni TextIni;
struct TextIni
{
	FILE* pf;
	List* name;
	List* data;
};
TextIni* TextIni_Create(void);
TextIni* TextIni_Add(TextIni* p, const char* name, const char* data);
TextIni* TextIni_Write(TextIni* p, const char* path);
TextIni* TextIni_Del(TextIni* p, const char* name);
TextIni* TextIni_Read(const char* path);
char* TextIni_Query(TextIni* p, const char* name);

class BA_Dir : public BA_Base
{
public:
	balist<char>* dirs = new balist<char>;//char* as a dot,not full path, don't contain "." and ".."
	balist<char>* files = new balist<char>;//char* as a dot,not full path
	char* root = NULL;//root as "D:\\A_DIR"

	bool isErr = false;

	//_root as "D:\\A_DIR"
	BA_Dir(const char* _root);
	//_root as "D:\\A_DIR", _type as "txt"
	BA_Dir(const char* _root,const char* _type);

	void Print(void);
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// return errCode
template<typename dataType>
int WriteType(FILE* pf, dataType* data, _LL num = 1);
// return data, use MCALLOC if data is NULL
template<typename dataType>
dataType* ReadType(FILE* pf, dataType* data = NULL);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template<typename dataType>
int WriteType(FILE* pf, dataType* data, _LL num)
{
	if (fwrite(&num, sizeof(_LL), 1, pf) != 1)
		return -1;
	if (fwrite(data, sizeof(dataType), num, pf) != num)
		return -2;
	return 0;
}

template<typename dataType>
dataType* ReadType(FILE* pf, dataType* data)
{
	_LL* num = MCALLOC(1, _LL);
	fread(num, sizeof(_LL), 1, pf);
	if (!data)
	{
		data = MCALLOC(*num, dataType);
		if (!data)
		{
			free(num);
			return (dataType*)(0x1);
		}
	}
	if(fread(data, sizeof(dataType), *num, pf) != *num)
	{
		free(num);
		return (dataType*)(0x2);
	}
	return data;
}

#endif