//BASIC_ALL_1_2 BA_STRING_H
//H
//Writen By BHM
//2021��11��24�� 21��11��

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_STRING_H
#define BA_STRING_H

#include"BA_Base.hpp"
#include"BA_Math.hpp"

class BA_String
{
public:
	char* pc = NULL;
	_ULL len = 0;
	List* mem = List_Init();

	_ULL queCopyNowIdx = 0;

	BA_String(void);
	BA_String(const char* _pc);
	//end with a NULL
	BA_String(_ULL num,const char* _pc1,...);


	BA_String Repeat(_ULL times);
	BA_String Concat(BA_String string);
	BA_String Concat(const char* _pc);

	BA_String Replace(BA_String string, BA_String newStr);
	BA_String Replace(const char* _pc, const char* newStr);
	List* Split(BA_String string);
	List* Split(const char* _pc);
	List* Splitx(BA_String string);
	List* Splitx(const char* _pc);
	//return the multi char* of pos
	List* Find(BA_String string);
	//return the multi char* of pos
	List* Find(const char* _pc);
};

char* Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite);
/*��Դ�� VN7 v1.1 ��Ŀ*//*VN7 v1.2 �����˸���*/
//��Ȼ,�����������strstr(pstr1,pstr2)������,���Ҹ�����,���ӹ�,һʱû������,��,�˷�һ����Сʱ,����˵CSH.c
//���Landing_Account����ʱ,һ���������Ե��ֲ�ͻ��,������,�ؼ���û������,��BUG����һ����Сʱ,θ��,θ��

char* Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter, unsigned long long* psite);


char* BA_Chars(_ULL num);
char* BA_Chars(_LL num);
char* BA_Chars(int num);
char* BA_Chars(float num);
char* BA_Chars(int num);

char* BA_Chars(BA_Shape s);
char* BA_Chars(BA_Array a);

#endif