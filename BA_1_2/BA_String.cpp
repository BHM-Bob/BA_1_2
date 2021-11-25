//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021年11月24日 21点13分

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_String.hpp"


char* Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite)
{
	if ((pc == NULL) && (*pc == '\0'))
	{
		PPW("NULL pc");
		return NULL;
	}
	if (psite == NULL)
	{
		psite = BALLOC_L(1, _ULL);
		*psite = 0;
	}
	size_t ss = strlen(pc);
	size_t ss1 = strlen(ps1);
	size_t ss2 = strlen(ps2);
	_ULL r1 = 0;//被找字符串头字符地址
	_ULL r2 = 0;//被找字符串尾字符地址
	_ULL ste1 = 0;//临时地址1
	_ULL ste2 = 0;//临时地址2
	_ULL qdz = ste1 = *psite;//取字符光标地址
	char* pte1 = BALLOC_S(ss1, char);//临时比对字符串1
	char* pte2 = BALLOC_S(ss2, char);//临时比对字符串2
	for (; ste2 < ss && ste1 < ss; )
	{
		for (_ULL a = 0; a < ss1 && ste1 < ss; *(pte1 + a) = *(pc + ste1), a++, ste1++);
		++qdz;
		if (strncmp(pte1, ps1, strlen(ps1)) == 0)
		{
			ste2 = r2 = r1 = ste1;
			for (; ste2 < ss && ste1 < ss; )
			{
				for (_ULL a = 0; a < ss2 && ste2 < ss; *(pte2 + (a++)) = *(pc + (ste2++)));
				if (strncmp(pte2, ps2, strlen(ps2)) == 0)
				{
					if (r1 < r2)//两个标记字串符间有字符
					{
						BALLOCS_L(char, pret, r2 - r1 + 2, NULL, );
						char* pret_te = pret, * pc_te = pc + r1;
						for (_ULL a = 0; a < r2 - r1; *pret_te = *pc_te, a++, pret_te++, pc_te++);
						*psite = r2;
						return pret;
					}
					if (r1 == r2)//两个标记字串符间没有字符
						return NULL;
				}
				++r2;
				ste2 = r2;
			}
		}
		if (ste1 == ss && ste2 == ss)
			break;
		ste1 = qdz;
	}
	return NULL;//没找到
}
char* Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter, unsigned long long* psite)
{
	if ((single_delimiters != NULL && integration_elimiter != NULL) || (single_delimiters == NULL && integration_elimiter == NULL))
	{
		PPW("Erorr");
		printf("Both single_delimiters and integration_elimiter are %s NULL\n", (single_delimiters == NULL && integration_elimiter == NULL) ? "" : "Not");
		return NULL;
	}
	if (psite == NULL)
	{
		psite = BALLOC_L(1, unsigned long long);
		*psite = 0;
	}
	if (single_delimiters != NULL)
	{
		bool isw = 0;
		bool isde = 0;
		rsize_t s1 = 0ULL;
		rsize_t s2 = 0ULL;
		rsize_t len1 = strlen(pc);
		rsize_t len2 = strlen(single_delimiters);
		if (*psite == len1)
			return NULL;
		for (rsize_t a = *psite, b = 0, c = 0; a < len1 && isw == 0; a++)
		{
			for (b = c = 0; b < len2 && isw == 0; b++)
			{
				if (*(pc + a) != *(single_delimiters + b))
					++c;
				else
					b = len2 + 9;
				if (c == len2)
				{
					s1 = a;
					isw = 1;
				}
			}
			if (a == len1 - 1)
				return NULL;
		}
		for (rsize_t a = s1 + 1, b = 0; a < len1 && isde == 0; a++)
		{
			for (b = 0; b < len2 && isde == 0; b++)
			{
				if (*(pc + a) == *(single_delimiters + b))
				{
					s2 = a;
					isde = 1;
				}
			}
			if (a == len1 - 1)
			{
				a = len1 + 9;
				s2 = len1;
			}
		}
		char* preturn = BALLOC_L(s2 - s1 + 1, char);
		if (memcpy_s(preturn, s2 - s1 + 1, pc + s1, s2 - s1) != 0)
			return NULL;
		*psite = s2;
		return preturn;
	}
	else if (integration_elimiter != NULL)
	{
		rsize_t  a = 0ULL;
		char* pte1 = NULL;
		char* pte2 = NULL;
		pte1 = strstr(pc + (*psite), integration_elimiter);
		if (pte1 != NULL)
		{
			pte2 = pc + (*psite);
			for (a = 0; pte2 + a != pte1; a++);
			char* preturn = MCALLOC(a + 1, char);
			if (strncpy_s(preturn, a + 1, pc + (*psite), a) != 0)
				return NULL;
			*psite = a;
			return preturn;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}


