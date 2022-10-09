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

void ba::jieba::cut2vector(char* pc, int inCode)
{
	s.clear();
	result.clear();
	if (inCode == 0 || inCode == 2)
	{
		tmp = ba::transferStrCode(pc, inCode, 1);
		s = tmp;
		free(tmp);
	}
	else
	{
		s = pc;
	}
	jb.Cut(s, words, true);
	s.clear();
}

void ba::jieba::cut2vector(char* pc, const char* inCode)
{
	s.clear();
	result.clear();
	if (strcmp(inCode, "gbk") == 0 || strcmp(inCode, "unicode") == 0)
	{
		tmp = ba::transferStrCode(pc, inCode, "utf-8");
		s = tmp;
		free(tmp);
	}
	else
	{
		s = pc;
	}
	jb.Cut(s, words, true);
	s.clear();
}
std::string& ba::jieba::cut2string(char* pc, const char* spliter,
	const char* inCode, const char* outCode)
{
	cut2vector(pc, inCode);
	if (strcmp(outCode, "gbk") == 0)
	{
		for (int i = 0; i < words.size(); i++)
		{
			tmp = ba::transferStrCode(words[i].c_str(), "utf-8", "gbk");
			words[i].assign(tmp);
			free(tmp);
		}
	}
	result = limonp::Join(words.begin(), words.end(), spliter);
	return result;
}

char* ba::Find_Words(char* pc, const char* ps1, const char* ps2, unsigned long long* psite)
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
char* ba::Mstrtok(char* pc, char* single_delimiters, char* integration_elimiter, unsigned long long* psite)
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
			if (!preturn)
			{
				PPW("OOM");
				return (char*)1;
			}
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

ba::str::str(void)
{
	pc = NULL;
	len = 0;
}

ba::str::str(const char* _pc)
{
	pc = NULL;
	len = 0;
	if (_pc == NULL)
	{
		MyBA_Err("ba::str::str(const char* _pc):_pc == NULL,return *this", 1);
	}
	else
	{
		pc = mstrdup(_pc, mem);
		if (pc == NULL)
			MyBA_Err("ba::str::str(const char* _pc):_strdup(_pc) == NULL,return *this", 1);
		else
			len = strlen(pc);
	}
}

ba::str::str(_LL num,const char* _pc1, ...)
{
	pc = NULL;
	len = 0;

	_LL js = 0;
	va_list parg;
	va_start(parg, _pc1);
	_LL sumlen = 1;
	List* plist = List_Init();
	if (plist == NULL)
	{
		MyBA_Err("ba::str::str(const char* _pc1, ...):List_Init() == NULL,return *this", 1);
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
			MyBA_Err("ba::str::str(const char* _pc1, ...):_strdup(_pc) == NULL,return *this", 1);
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

ba::str ba::str::Input(int maxLen)
{
	if (maxLen > 0)
	{
		pc = BALLOC_R(maxLen, char, mem);
		scanf_s("%s", pc, maxLen);
		len = strlen(pc);
	}
	else
	{
		MyBA_PutLog("str::Input(_LL maxLen)::maxLen < 0");
	}
	return *this;
}

ba::str ba::str::ReLoad(const char* _pc)
{
	if(pc != NULL)
		free(pc);
	pc = _strdup(_pc);
	if (pc == NULL)
		MyBA_Err("ba::str ba::str::ReLoad(const char* _pc):_strdup(_pc) == NULL,return *this", 1);
	else
		len = strlen(pc);
	return *this;
}

void ba::str::Destroy(void)
{
	MyBA_Free_R(mem, true);
}

ba::str* ba::str::operator()(_LL index1, _LL index2)
{
	str* ret = new str();
	ret->len = index2 - index1;
	ret->pc = BALLOC_R(ret->len + 1, char, ret->mem);
	strncpy_s(ret->pc, ret->len+1, pc + index1, ret->len);
	return ret;
}

ba::str ba::str::Repeat(_LL times)
{
	_LL oldLen = len;
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
		PPW("ba::str ba::str::Repeat(_ULL times):MCALLOC NULL,return *this");
	}
	return *this;
}

ba::str ba::str::Concat(str string)
{
	return Concat(string.pc);
}

ba::str ba::str::Concat(const char* _pc)
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
		PPW("ba::str ba::str::Concat(const char* _pc):MCALLOC NULL,return *this");
	}
	return *this;
}

ba::str ba::str::Insert(_LL pos, str string)
{
	return Insert(pos, string.pc);
}

ba::str ba::str::Insert(_LL pos, const char* _pc)
{
	if (-1 <= pos && pos <= len-1)
	{
		++pos;//[0, len]
		char* p1 = pc;
		_LL l1 = pos;
		char* p2 = mstrdup(_pc);
		_LL l2 = strlen(p2);
		char* p3 = pc + pos;
		_LL l3 = len - pos;
		char* p = BALLOC_R(l1 + l2 + l3 + 1, char, mem);
		strncat_s(p, l1 + l2 + l3 + 1, p1, l1);
		strncat_s(p, l1 + l2 + l3 + 1, p2, l2);
		strncat_s(p, l1 + l2 + l3 + 1, p3, l3);
		free(pc);
		pc = p;
		len = l1 + l2 + l3;
	}
	else
	{
		MyBA_Err("ba::str ba::str::Insert(_LL pos, const char* _pc): not -1 <= pos && pos <= len-1, do nothing", 1);
	}
	return *this;
}

ba::str ba::str::Replace(str string, str newStr)
{
	return Replace(string.pc, newStr.pc);
}

ba::str ba::str::Replace(const char* _pc, const char* newStr)
{
	if (pc == NULL)
	{
		MyBA_Err("str str::Replace(const char* pc, const char* newStr):pc==NULL,return *this",1);
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

balist<char>* ba::str::Split(str string)
{
	return Split(string.pc);
}

balist<char>* ba::str::Split(const char* _pc)
{
	if (_pc == NULL || *_pc == '\0' || pc == NULL || len == 0 || *pc == '\0')
		return (balist<char>*)MyBA_Err("List* str::Splitx(const char* _pc): _pc == NULL || pc == NULL,return NULL", 1);

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
_LL* str_Splitx_FindOnce(char* pc, const char* delimiters)
{
	_LL* psite = MCALLOC(1, _LL);
	if(psite)
	{
		for (*psite = 0; *pc != '\0'; (*psite)++, pc++)
			if (strchr(delimiters, *pc) != NULL)
				return psite;
	}
	return NULL;
}

balist<char>* ba::str::Splitx(const char* _pc)
{
	if (_pc == NULL || *_pc == '\0' || pc == NULL || len == 0 || *pc == '\0')
		return (balist<char>*)MyBA_Err("List* str::Splitx(const char* _pc): _pc == NULL || pc == NULL,return NULL", 1);

	_LL* psite = NULL;
	char* pte = pc;
	balist<_LL>* pli = new balist<_LL>();
	for (psite = str_Splitx_FindOnce(pte, _pc); psite != NULL; psite = str_Splitx_FindOnce(pte, _pc))
	{
		//if the first chr of pte is in delimiters, *psite is 0
		pte += *psite + 1;
		pli->Put(psite);
	}
	pli->Put(TypeDupR(NULL, 1, len));
	balist<char>* pret = new balist<char>();
	char* pcte = NULL;
	pte = pc;
	for(_LL* p = pli->Copy(); p; p = pli->Copy())
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
	pli->Destroy();
	delete pli;
	return pret;
}

balist<char>* ba::str::Splitx(str string)
{
	return Splitx(string.pc);
}

List* ba::str::Find(str string)
{
	return Find(string.pc);
}

List* ba::str::Find(const char* _pc)
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

char* ba::StrAdd(List* mem, const char* pstr, ...)
{
	va_list parg;
	va_start(parg, pstr);
	_ULL sumlen = 1;
	List* plist = List_Init();
	sumlen += (pstr ? strlen(pstr) : 0);// handle that if the head ptr is NULL
	plist->Put(plist, (pstr ? (void*)pstr : BA_FREED_PTR));// handle that if the head ptr is NULL
	for (char* p = va_arg(parg, char*); p != NULL; p = va_arg(parg, char*))
	{
		sumlen += (p ? strlen(p) : 0);
		plist->Put(plist, p);
	}
	char* pret = BALLOC_R(sumlen, char, mem);
	if (!pret)
	{
		PPW("OOM");
		return (char*)1;
	}
	for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
		if (p != BA_FREED_PTR)
			strcat_s(pret, sumlen, p);
	List_Destroy(plist);
	return pret;
}

char* ba::stradd(ba::memRecord* mem, _LL toBeFreedInStack, const char* pstr, ...)
{
	va_list parg;
	va_start(parg, pstr);
	_ULL sumlen = 1;
	List* plist = List_Init();
	sumlen += (pstr ? strlen(pstr) : 0);// handle that if the head ptr is NULL
	plist->Put(plist, (pstr ? (void*)pstr : BA_FREED_PTR));// handle that if the head ptr is NULL
	for (char* p = va_arg(parg, char*); p != NULL; p = va_arg(parg, char*))
	{
		sumlen += (p ? strlen(p) : 0);
		plist->Put(plist, p);
	}
	char* pret = balloc<char>(sumlen, mem, toBeFreedInStack);
	if (!pret)
	{
		PPW("OOM");
		return (char*)1;
	}
	for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
		if (p != BA_FREED_PTR)
			strcat_s(pret, sumlen, p);
	List_Destroy(plist);
	return pret;
}

// if mem == NULL, do not record
char* ba::strdup(const char* p, ba::memRecord* mem, _LL toBeFreedInStack)
{
	_ballocs(char, pret, strlen(p), mem, toBeFreedInStack, 1);
	return pret;
}

char* ba::transferStrCode(const char* pc, const char* ori, const char* to)
{
	int len = 0;
	wchar_t* wstr = NULL;
	char* str = NULL;
	// from -> unicode(wstr)
	if(pc && ori && to)
	{
		if (strcmp(ori, "gbk") == 0)
		{// gbk(char) -> Unicode(wchar)
			len = MultiByteToWideChar(CP_ACP, 0, pc, -1, NULL, 0);
			wstr = new wchar_t[len + 1];
			memset(wstr, 0, len + 1);
			MultiByteToWideChar(CP_ACP, 0, pc, -1, wstr, len);
		}
		else if (strcmp(ori, "utf-8") == 0)
		{// UTF-8(char) -> Unicode(wchar)
			len = MultiByteToWideChar(CP_UTF8, 0, pc, -1, NULL, 0);
			wstr = new wchar_t[len + 1];
			memset(wstr, 0, len + 1);
			MultiByteToWideChar(CP_UTF8, 0, pc, -1, wstr, len);
		}
		else if (strcmp(ori, "unicode") == 0)
		{// Unicode(char) -> Unicode(wchar)
			len = mbstowcs(NULL, pc, 0);
			wstr = new wchar_t[len + 1];
			memset(wstr, 0, len + 1);
		}
	}
	// unicode(wstr) -> to
	if(wstr)
	{
		if (strcmp(to, "gbk") == 0)
		{//Unicode(wchar) -> gbk(char)
			len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
			str = new char[len + 1];
			memset(str, 0, len + 1);
			WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
			if (wstr) delete[] wstr;
			return str;
		}
		else if (strcmp(to, "utf-8") == 0)
		{//Unicode(wchar) -> utf-8(char)
			len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
			str = new char[len + 1];
			memset(str, 0, len + 1);
			WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
			if (wstr) delete[] wstr;
			return str;
		}
		else if (strcmp(to, "unicode") == 0)
		{//Unicode(wchar) -> Unicode(char)
			len = wcstombs(NULL, wstr, 0);
			str = new char[len + 1];
			memset(str, 0, len + 1);
			wcstombs(str, wstr, len);
			if (wstr) delete[] wstr;
			return str;
		}
	}
	PPW("unable to tansfer code");
	PPWs(ori, to);
	return (char*)1;
}

char* ba::transferStrCode(const char* pc, int ori, int to)
{
	char code2str[3][10] =
	{
		"gbk",
		"utf-8",
		"unicode",
	};
	if (ori <= 2 && to <= 2 && ori >= 0 && to >= 0)
		return transferStrCode(pc, code2str[ori], code2str[to]);
	PPW("unable to tansfer code");
	PPX(ori);
	PPX(to);
	return (char*)1;
}