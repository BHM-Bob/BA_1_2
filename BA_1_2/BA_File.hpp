//BASIC_ALL_1_2 BA_FILE_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_FILE_H
#define BA_FILE_H

#include "BA_Base.hpp"
#include "BA_Mem.hpp"

namespace ba
{
	_LL getFileSize(std::ifstream& ifs);
	char* read(std::ifstream& ifs, List* mem = NULL);
	char* read(std::ifstream& ifs, memRecord* mem = NULL);


	class dir : public BA_Base
	{
	public:
		balist<char>* dirs = new balist<char>;//char* as a dot,not full path, don't contain "." and ".."
		balist<char>* files = new balist<char>;//char* as a dot,not full path
		char* root = NULL;//root as "D:\\A_DIR"

		bool isErr = false;

		//_root as "D:\\A_DIR"
		dir(const char* _root);
		//_root as "D:\\A_DIR", _type as "txt"
		dir(const char* _root, const char* _type);

		void Print(void);
	};
}

char* StringWrite(FILE* pf, char* pc);

//BALLOC_S
char* StringRead(FILE* pf, List* mem = NULL);

unsigned long long  Get_File_Size(FILE* pf);

char* ReadTXT(const char* path, _ULL loadSize = 0, List* mem = NULL);

char* GetFileType(char* ppath, List* mem = NULL);

bool Check_File_Exist(char* path);

bool Encrypt_File(char* path, char* Passwords);

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