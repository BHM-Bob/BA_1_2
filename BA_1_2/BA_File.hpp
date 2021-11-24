//BASIC_ALL_1_2 BA_FILE_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_FILE_H
#define BA_FILE_H

#include"BA_Base.hpp"

class BA_Dir
{
public:
	List* dirs;//char* as a dot,not full path, don't contain "." and ".."
	List* files;//char* as a dot,not full path
	char* root;//root as "D:\\A_DIR"

	List* mem;

	//root as "D:\\A_DIR"
	BA_Dir(const char* _root);
	void Print(void);
};

#endif