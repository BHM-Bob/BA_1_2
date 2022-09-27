//BASIC_ALL_1_2 BA_STRING_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_STRING_H
#define BA_STRING_H

#include"BA_Base.hpp"
#include"BA_Math.hpp"

// TODO : add utf-8, unicode, ANSI support
// https://blog.csdn.net/flushhip/article/details/82836867

namespace ba
{

	class str
	{
	public:
		char* pc = NULL;
		_LL len = 0;
		List* mem = List_Init();

		str(void);
		str(const char* _pc);
		//end with a NULL
		str(_LL num, const char* _pc1, ...);

		str Input(int maxLen = 0);
		str ReLoad(const char* _pc);

		str Repeat(_LL times);
		str Concat(str string);
		str Concat(const char* _pc);
		//after pos:[-1, len-1]
		str Insert(_LL pos, str string);
		//after pos:[-1, len-1]
		str Insert(_LL pos, const char* _pc);

		str Replace(str string, str newStr);
		str Replace(const char* _pc, const char* newStr);
		balist<char>* Split(str string);
		balist<char>* Split(const char* _pc);
		//BALLOC_R
		balist<char>* Splitx(str string);
		//BALLOC_R
		balist<char>* Splitx(const char* _pc);
		//return the multi char* of pos
		List* Find(str string);
		//return the multi char* of pos
		List* Find(const char* _pc);

		void Destroy(void);

		// [] 运算符重载
		str* operator()(_LL index1, _LL index2);
	};

	char* Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite);
	/*来源于 VN7 v1.1 项目*//*VN7 v1.2 进行了更新*/
	//当然,这玩意儿可用strstr(pstr1,pstr2)做出来,而且更方便,脑子梗,一时没想起来,唉,浪费一个多小时,更别说CSH.c
	//里的Landing_Account调试时,一个极其明显但又不突出,不好找,关键是没调试器,的BUG找了一个多小时,胃疼,胃胀

	char* Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter, unsigned long long* psite);

	//end with a NULL
	char* StrAdd(List* mem, const char* pstr, ...);



	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	// use MCALLOC when mem is NULL
	template<typename dataType>
	char* Num2Str(dataType num, List* mem = NULL)
	{
		char* preturn = NULL;
		size_t w = 1;
		if (typeid(dataType) == typeid(double))
		{
			if (mem)
			{
				preturn = BALLOC_R(18, char, mem);
			}
			else
			{
				preturn = MCALLOC(181, char);
			}
			sprintf_s(preturn, w + 1, "%e", num);
		}
		else
		{
			for (int a = 10; num / a != 0; w++, a *= 10);
			if (mem)
			{
				preturn = BALLOC_R(w + 1, char, mem);
			}
			else
			{
				preturn = MCALLOC(w + 1, char);
			};
			if (typeid(dataType) == typeid(int))
				sprintf_s(preturn, w + 1, "%d", num);
			else if (typeid(dataType) == typeid(_LL))
				sprintf_s(preturn, w + 1, "%lld", num);
			else if (typeid(dataType) == typeid(_ULL))
				sprintf_s(preturn, w + 1, "%llu", num);
			else
				return (char*)MyBA_Errs(1, "Num_To_Char: type default with ",
					typeid(dataType).name(), ", return NULL", NULL);
		}
		return preturn;
	}
}

#endif
