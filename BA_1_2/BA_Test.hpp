//BASIC_ALL_1_2 BA_STRING_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_TEST_H
#define BA_TEST_H

#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"

void BA_String_Test(void);
void BA_Array_Test(void);
void BA_Array_Test2(void);
float BA_ArrayTest_1(float* p);
BA_Array BA_Array_GetTest();
void BA_Dir_Test(void);
void BA_ThreadPool_Test(void);
void BA_Test_WordsCount_HashCol(balistDot<_ULL>* p1, balistDot<_ULL>* p2);
void BA_Test_WordsCount_SubThr(_ULL id, balist<ba::str>& getQ,
    balist<char>& putQ, balist<float>& proc, balist<bool>& sig, void* data);
void BA_Test_WordsCount(void);

#endif