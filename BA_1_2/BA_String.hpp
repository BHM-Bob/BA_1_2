//BASIC_ALL_1_2 BA_STRING_H
//H
//Writen By BHM
//2021��11��24�� 21��11��

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_STRING_H
#define BA_STRING_H

#include"BA_Base.hpp"

char* Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite);
/*��Դ�� VN7 v1.1 ��Ŀ*//*VN7 v1.2 �����˸���*/
//��Ȼ,�����������strstr(pstr1,pstr2)������,���Ҹ�����,���ӹ�,һʱû������,��,�˷�һ����Сʱ,����˵CSH.c
//���Landing_Account����ʱ,һ���������Ե��ֲ�ͻ��,������,�ؼ���û������,��BUG����һ����Сʱ,θ��,θ��

char* Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter, unsigned long long* psite);

#endif