//BASIC_ALL_1_2 BA_FILE_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_FILE_H
#define BA_FILE_H

#include"BA_Base.hpp"


char* StringWrite(FILE* pf, char* pc);

char* StringRead(FILE* pf);

unsigned long long  Get_File_Size(FILE* pf);

char* ReadTXT(char* path);

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

class BA_Dir
{
public:
	List* dirs;//char* as a dot,not full path, don't contain "." and ".."
	List* files;//char* as a dot,not full path
	char* root;//root as "D:\\A_DIR"

	List* mem;

	//_root as "D:\\A_DIR"
	BA_Dir(const char* _root);
	//_root as "D:\\A_DIR", _type as "txt"
	BA_Dir(const char* _root,const char* _type);

	void Print(void);
};

#endif