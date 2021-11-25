//BASIC_ALL_1_2 BA_STRING_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_STRING_H
#define BA_STRING_H

#include"BA_Base.hpp"

char* Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite);
/*来源于 VN7 v1.1 项目*//*VN7 v1.2 进行了更新*/
//当然,这玩意儿可用strstr(pstr1,pstr2)做出来,而且更方便,脑子梗,一时没想起来,唉,浪费一个多小时,更别说CSH.c
//里的Landing_Account调试时,一个极其明显但又不突出,不好找,关键是没调试器,的BUG找了一个多小时,胃疼,胃胀

char* Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter, unsigned long long* psite);

#endif