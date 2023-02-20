//BASIC_ALL_1_2 BA_STRING_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV

#ifndef BA_STRING_H
#define BA_STRING_H

#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"cppjieba/Jieba.hpp"

// TODO : add utf-8, unicode, ANSI support
// https://blog.csdn.net/flushhip/article/details/82836867

namespace ba
{
	// gbk, utf-8, unicode
	// remain pc unfreed
	char* transferStrCode(const char* pc, const char* ori, const char* to);
	// 0=gbk, 1=utf-8, 2=unicode
	// remain pc unfreed
	char* transferStrCode(const char* pc, int ori, int to);

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
		std::vector<char*>& Split_s(str string);
		std::vector<char*>& Split_s(const char* _pc);
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

	/*来源于 VN7 v1.1 项目*//*VN7 v1.2 进行了更新*/
	char* Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite);

	char* Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter,
		unsigned long long* psite);

	// if mem == NULL, do not record
	char* strdup(const char* p, ba::memRecord* mem = NULL, _LL toBeFreedInStack = 0);
	// end with a NULL
	char* stradd(ba::memRecord* mem, _LL toBeFreedInStack, const char* pstr, ...);


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
			preturn = BALLOC_R(18, char, mem);
			sprintf_s(preturn, w + 1, "%e", (double)num);
		}
		else
		{
			for (int a = 10; num / a != 0; w++, a *= 10);
			preturn = BALLOC_R(w + 1, char, mem);
			if (typeid(dataType) == typeid(int))
				sprintf_s(preturn, w + 1, "%d", (int)num);
			else if (typeid(dataType) == typeid(_LL))
				sprintf_s(preturn, w + 1, "%lld", (_LL)num);
			else if (typeid(dataType) == typeid(_ULL))
				sprintf_s(preturn, w + 1, "%llu", (_ULL)num);
			else
				return (char*)MyBA_Errs(1, "Num_To_Char: type default with ",
					typeid(dataType).name(), ", return NULL", NULL);
		}
		return preturn;
	}
}

#endif
