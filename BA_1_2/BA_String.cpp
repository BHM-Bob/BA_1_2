//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021年11月24日 21点13分

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
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

char* StrAdd(List* mem, const char* pstr, ...)
{
	va_list parg;
	va_start(parg, pstr);
	_ULL sumlen = 1;
	List* plist = List_Init();
	sumlen += strlen(pstr);
	plist->Put(plist, (void*)pstr);
	for (char* p = va_arg(parg, char*); p != NULL; p = va_arg(parg, char*))
	{
		sumlen += strlen(p);
		plist->Put(plist, (void*)p);
	}
	char* pret = BALLOC_R(sumlen, char, mem);
	if (!pret)
	{
		PPW("OOM");
		return (char*)1;
	}
	for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
		strcat_s(pret, sumlen, p);
	List_Destroy(plist);
	return pret;
}

BA_String::BA_String(void)
{
	pc = NULL;
	len = 0;
}

BA_String::BA_String(const char* _pc)
{
	pc = NULL;
	len = 0;
	if (_pc == NULL)
	{
		MyBA_Err("BA_String::BA_String(const char* _pc):_pc == NULL,return *this", 1);
	}
	else
	{
		pc = mstrdup(_pc, mem);
		if (pc == NULL)
			MyBA_Err("BA_String::BA_String(const char* _pc):_strdup(_pc) == NULL,return *this", 1);
		else
			len = strlen(pc);
	}
}

BA_String::BA_String(_ULL num,const char* _pc1, ...)
{
	pc = NULL;
	len = 0;

	_ULL js = 0;
	va_list parg;
	va_start(parg, _pc1);
	_ULL sumlen = 1;
	List* plist = List_Init();
	if (plist == NULL)
	{
		MyBA_Err("BA_String::BA_String(const char* _pc1, ...):List_Init() == NULL,return *this", 1);
	}
	else
	{
		sumlen += strlen(_pc1);
		plist->Put(plist, (void*)_pc1);
		for (char* p = va_arg(parg, char*); p != NULL && js<num; p = va_arg(parg, char*))
		{
			sumlen += strlen(p);
			plist->Put(plist, (void*)p);
			++js;
		}

		pc = MCALLOC(sumlen, char);
		if (pc == NULL)
		{
			MyBA_Err("BA_String::BA_String(const char* _pc1, ...):_strdup(_pc) == NULL,return *this", 1);
		}
		else
		{
			len = strlen(pc);
			for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
				strcat_s(pc, sumlen, p);
		}
		free(plist);
	}
	va_end(parg);
}

BA_String BA_String::ReLoad(const char* _pc)
{
	if (_pc == NULL)
	{
		MyBA_Err("BA_String::ReLoad(const char* _pc):_pc == NULL,return *this", 1);
	}
	else
	{
		if(pc != NULL)
			free(pc);
		pc = _strdup(_pc);
		if (pc == NULL)
			MyBA_Err("BA_String::ReLoad(const char* _pc):_strdup(_pc) == NULL,return *this", 1);
		else
			len = strlen(pc);
	}
	return *this;
}

void BA_String::Destroy(void)
{
	MyBA_Free_R(mem, true);
}

BA_String* BA_String::operator()(_LL index1, _LL index2)
{
	BA_String* ret = new BA_String();
	ret->len = index2 - index1;
	ret->pc = BALLOC_R(ret->len + 1, char, ret->mem);
	strncpy_s(ret->pc, ret->len+1, pc + index1, ret->len);
	return ret;
}

BA_String BA_String::Repeat(_ULL times)
{
	_ULL oldLen = len;
	len *= times;
	char* pct = MCALLOC(len+1, char);
	if (pct != NULL)
	{
		for (char* pCharNew = pct, *pEndNew = pct + len; pCharNew != pEndNew; )
			for(char* pChar = pc, *pEnd = pc+oldLen; pChar != pEnd; pChar++, pCharNew++)
				*pCharNew = *pChar;
		free(pc);
		pc = pct;
	}
	else
	{
		PPW("BA_String BA_String::Repeat(_ULL times):MCALLOC NULL,return *this");
	}
	return *this;
}

BA_String BA_String::Concat(BA_String string)
{
	return Concat(string.pc);
}

BA_String BA_String::Concat(const char* _pc)
{
	len += strlen(_pc)+1;
	char* pct = MCALLOC(len, char);
	if (pct != NULL)
	{
		strcat_s(pct, len, pc);
		strcat_s(pct, len, _pc);
		free(pc);
		pc = pct;
	}
	else
	{
		PPW("BA_String BA_String::Repeat(_ULL times):MCALLOC NULL,return *this");
	}
	return *this;
}

BA_String BA_String::Replace(BA_String string, BA_String newStr)
{
	return Replace(string.pc, newStr.pc);
}

BA_String BA_String::Replace(const char* _pc, const char* newStr)
{
	if (pc == NULL)
	{
		MyBA_Err("BA_String BA_String::Replace(const char* pc, const char* newStr):pc==NULL,return *this",1);
		return *this;
	}
	_ULL trgStringLen = strlen(_pc);
	_ULL newSubStringLen = 0;
	if (newStr != NULL)
		newSubStringLen = strlen(newStr);
	List* pli = List_Init();
	if (pli != NULL)
	{
		for (char* pte = strstr(pc, _pc); pte != NULL; pte = strstr(pte, _pc))
		{
			List_Put(pli, (void*)pte);
			pte += trgStringLen;
		}
		if (pli->sumque > 0)
		{
			_ULL maxDist = len + (pli->sumque) * (newSubStringLen - trgStringLen) + 1;
			char* ptm = MCALLOC(maxDist, char);
			if (ptm != NULL)
			{
				len = strlen(ptm);
				char* st = pc;
				LIST_FORG(char, p, pli)
				{
					strncat_s(ptm, maxDist, st, (_ULL)(p - st));//NOT st - p
					if (newSubStringLen > 0)
						strncat_s(ptm, maxDist, newStr, newSubStringLen);
					st = p + trgStringLen;
				}
				strncat_s(ptm, maxDist, st, strlen(st));//NOT st - p
			}
			else
			{
				PPW("MCALLOC==NULL,return *this");
			}
			free(pc);
			free(pli);
			pc = ptm;
		}
		else
		{
			return *this;
		}
	}
	else
	{
		PPW("List_Init()==NULL,return *this");
	}
	return *this;
}

balist<char>* BA_String::Split(BA_String string)
{
	return Split(string.pc);
}

balist<char>* BA_String::Split(const char* _pc)
{
	if (_pc == NULL || *_pc == '\0' || pc == NULL || len == 0 || *pc == '\0')
		return (balist<char>*)MyBA_Err("List* BA_String::Splitx(const char* _pc): _pc == NULL || pc == NULL,return NULL", 1);

	List* pli = this->Find(_pc);
	balist<char>* pret = new balist<char>();
	char* pcte = pc;
	char* ptm = NULL;
	_ULL lens = 0;
	_ULL trgStrLen = strlen(_pc);
	LIST_FORG(char, p, pli)
	{
		lens = p - pcte;
		if (lens == 0)
			continue;
		ptm = BALLOC_R(lens +1, char, mem);
		strncat_s(ptm, lens + 1, pcte, lens);
		pret->Put(ptm);
		pret->plast->usage = lens;
		pcte += (lens + trgStrLen);
	}
	lens = pc + len - pcte;
	if (lens <= 0)
		return pret;
	ptm = BALLOC_R(lens + 1, char, mem);
	strncat_s(ptm, lens + 1, pcte, lens);
	pret->Put(ptm);
	pret->plast->usage = lens;
	return pret;
}

//返回pc中出现delimiters的字符的偏移量,from 0
//会对pc进行地址偏移操作，修改pc值
_ULL* BA_String_Splitx_FindOnce(char* pc, const char* delimiters)
{
	MCALLOCS(_ULL, psite, 1);
	if(psite)
	{
		for (*psite = 0; *pc != '\0'; (*psite)++, pc++)
			if (strchr(delimiters, *pc) != NULL)
				return psite;
	}
	return NULL;
}

balist<char>* BA_String::Splitx(const char* _pc)
{
	if (_pc == NULL || *_pc == '\0' || pc == NULL || len == 0 || *pc == '\0')
		return (balist<char>*)MyBA_Err("List* BA_String::Splitx(const char* _pc): _pc == NULL || pc == NULL,return NULL", 1);

	_ULL* psite = NULL;
	char* pte = pc;
	List* pli = List_Init();
	for (psite = BA_String_Splitx_FindOnce(pte, _pc); psite != NULL; psite = BA_String_Splitx_FindOnce(pte, _pc))
	{
		//if the first chr of pte is in delimiters, *psite is 0
		pte += *psite + 1;
		List_Put(pli, (void*)psite);
	}
	List_Put(pli, TypeDupR(NULL, 1, len));
	balist<char>* pret = new balist<char>();
	char* pcte = NULL;
	pte = pc;
	LIST_FORG(_ULL, p, pli)
	{
		if (*p > 0)
		{
			pcte = BALLOC_R(*p + 1, char, mem);
			strncpy_s(pcte, *p + 1, pte, *p);
			pret->Put(pcte);
		}
		pte += *p + 1;
		free(p);
	}
	List_Destroy(pli);
	return pret;
}

balist<char>* BA_String::Splitx(BA_String string)
{
	return Splitx(string.pc);
}

List* BA_String::Find(BA_String string)
{
	return Find(string.pc);
}

List* BA_String::Find(const char* _pc)
{
	_ULL trgStringLen = strlen(_pc);
	List* pli = List_Init();
	if (pli != NULL)
	{
		for (char* pte = strstr(pc, _pc); pte != NULL; pte = strstr(pte, _pc))
		{
			List_Put(pli, (void*)pte);
			pte += trgStringLen;
		}
		if (pli->sumque > 0)
		{
			return pli;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		PPW("List_Init()==NULL,return *this");
	}
	return NULL;
}