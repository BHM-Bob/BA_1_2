//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021-11-18

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_UI.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"

MyBA* pba;

float MyBA_Ver(void)
{
	return 1.4201f;
	/*
	* 1.1000:2020年08月15日：MyUI； MyDir(in C)
	* 1.2000:2021年03月28日：MyBA; ThreadQue  MyTreads; List
	* 1.3000:2021年08月20日：LOFE(仅支持正向传播);至2021年09月20日左右支持反向传播
	* 1.3011:2021年08月28日：MyUI添加窗口透明选项，修改报错文本
	* 1.3020:2021年08月28日：SDL_GetFontTexture修改逻辑，使得SDL_MyButton选择背景颜色创建时溢出文字可被显示
	* 1.3100:2021年08月29日: 将QQDH项目转化为SDL_ColorSur部件，修改MyUI_Addbutt，添加彩色按钮选项
	* 1.3200:2021年08月29日: 将QQDH项目转化为SDL_ColorText部件
	* 1.3300:2021年11月20日: 升级内存管理机制,支持长时内存和快内存的申请与释放
	* 1.4000:2021年11月25日: 使用C++,增加BA_Array类和BA_Dir类,支持任意形状数组运算,支持文件夹查看
	* 1.4110:2021年12月06日: 增加BA_String,为内存申请增加内存量计数
	* 1.4201:2022年04月10日: 增加生信序列相似度算法;修复BA_String.split的字符串头尾不处理bug
	*/
}

void MyBA_Init(void)
{
	pba = MCALLOC(1, MyBA);
	if (pba == NULL)
	{
		PPW("MyBA_Init:MCALLOC==NULL,pba = NULL");
		PPW("Stop at this moment because pba was not Inited, so it may result in some problems");
		_SIS_;
	}
	else
	{
		pba->BA_Ver = MyBA_Ver();
		printf("\nMyBA (Ver: %10.4f; Build: %s %s) start\n", pba->BA_Ver, __DATE__, __TIME__);

		pba->GUT_t = clock();
		pba->GUT_state = 0;
		pba->JDT_t = 0;

		pba->exepath = _getcwd(NULL, 0);//psd->exeinfo.exepath is <G:\PellesC\My Project\VN7\VN7 v1_3\VN7 1_3>,It's size is 42

		pba->mem = List_Init();
		pba->LTmem = List_Init();
		pba->STmem = List_Init();

#ifdef USE_SDL2
		pba->isSDL2 = 1;
#endif

		MCALLOCS(BALog, plog, 1);
		if (plog == NULL)
		{
			PPW("MyBA_Init:plog == NULL,pass");
		}
		else
		{
			plog->t = clock();
			plog->pdate = Get_Time_Without_L();
			plog->pc = _strdup("MyBA Start");
		}
		pba->pLog = List_Init();
		pba->pLog->Put(pba->pLog, plog);

		pba->PutLog = MyBA_PutLog;
		pba->GUT = MyBA_GetUsedTime;
		pba->Quit = MyBA_Quit;

#ifdef USE_WINDOWS
		Sleep(500);
#else
#ifdef USE_OPENCV
		cv::WaitKey(500);
#endif
		for (clock_t t = clock(); clock() < t + 500;);
#endif

		if (_kbhit() != 0)
		{
			switch (_getch())
			{
			case 'c':case'C':
				MyBA_CMD();
				break;
			case 's':case'S':
				MyBA_SafeMode();
				break;
			default:
				break;
			}
		}
	}
	srand(time(NULL));
}

void MyBA_PutLog(const char* pc)
{
	MCALLOCS(BALog,plog, 1);
	plog->t = clock();
	plog->pdate = Get_Time_Without_L();
	plog->pc = _strdup(pc);
	pba->pLog->Put(pba->pLog, plog);
}

float MyBA_GetUsedTime(void)
{
	if (pba == NULL)
		return -1.;
	if (pba->GUT_state)
	{
		float ret = (float)((float)clock() - pba->GUT_t) / CLOCKS_PER_SEC;
		pba->GUT_t = clock();
		return ret;
	}
	else
	{
		pba->GUT_state = 1;
		return 0.;
	}
}

bool MyBA_WriteLog(bool isquit)
{
	FILE* pf = NULL;
	if (fopen_s(&pf, StringAdd_S(pba->exepath, "\\mba.log", NULL), "a") == 0)
	{
		pba->PutLog(_strdup("mba.log file opened successfully"));
		if (isquit == 1)
			pba->PutLog(_strdup("MyBA Start To Quit"));
	}
	else
	{
		pba->PutLog(_strdup("Unable to open mba.log file"));
		if(pf != NULL)
			fclose(pf);
		return 0;
	}
	for (BALog* p = (BALog*)(List_Copy(pba->pLog)); p != NULL; p = (BALog*)(List_Copy(pba->pLog)))
	{
		fwrite(&(p->t), sizeof(clock_t), 1, pf);
		StringWrite(pf, p->pdate);
		StringWrite(pf, p->pc);
		fwrite(&(p->code), sizeof(int), 1, pf);
	}
	fclose(pf);
	return 1;
}

void* MyBA_Err(const char* pc, bool instance)
{
	if (pba == NULL)
	{
		PPW(pc);
		return NULL;
	}
	else
	{
		if (instance == 1)
		{
			PPW(pc);
		}
		pba->PutLog(StringAdd_L("Err:", pc, NULL));
	}
	return NULL;
}

void* MyBA_Errs(bool instance,...)
//End with a NULL
{
	va_list parg;
	va_start(parg, instance);
	_ULL sumlen = 1;
	List* plist = List_Init();
	for (char* p = va_arg(parg, char*); p != NULL; p = va_arg(parg, char*))
	{
		sumlen += strlen(p);
		plist->Put(plist, (void*)p);
	}
	va_end(parg);
	sumlen += 5;
	BALLOCS_L(char, pret, sumlen, NULL, );
	strcat_s(pret, sumlen, "Err:");
	for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
		strcat_s(pret, sumlen, p);
	free(plist);

	if (instance == 1)
		PPW(pret);
	pba->PutLog(pret);
	return NULL;
}

void* MyBA_CALLOC_S(_ULL count, _ULL size)
{
	void* ret = calloc(count, size);
	if (ret == NULL)
	{
		PPW("MyBA_CALLOC_S: Can not alloc mem, return NULL");//由于可能会引发递不归，所以不使用MyBA_Err
		return NULL;
	}
	List_Put(pba->STmem, ret);
	pba->STmem->plast->usage = size*count;
	pba->STmem->tik += pba->STmem->plast->usage;
	return ret;
}

void* MyBA_CALLOC_L(_ULL count, _ULL size)
{
	void* ret = calloc(count, size);
	if (ret == NULL)
	{
		PPW("MyBA_CALLOC_L: Can not alloc mem, return NULL");//由于可能会引发递不归，所以不使用MyBA_Err
		return NULL;
	}
	List_Put(pba->LTmem, ret);
	pba->LTmem->plast->usage = size* count;
	pba->LTmem->tik += pba->LTmem->plast->usage;
	return ret;
}

void* MyBA_CALLOC_R(_ULL count, _ULL size, List* pli)
{
	void* ret = calloc(count, size);
	if (ret == NULL)
	{
		PPW("MyBA_CALLOC_R: Can not alloc mem, return NULL");//由于可能会引发递不归，所以不使用MyBA_Err
		return NULL;
	}
	List_Put(pli, ret);
	pli->plast->usage = size*count;
	pli->tik += pli->plast->usage;
	return ret;
}

int MyBA_Quit(void)
{
	MyBA_WriteLog(1);

	MyBA_Free_R(pba->mem);
	MyBA_Free_R(pba->LTmem);
	MyBA_Free_R(pba->STmem);

	free(pba);
	printf("\nMyBA Going to Quit\n");
	return 1;
}

void MyBA_Free_R(List* pli)
{
	if (pli != NULL)
	{
		for (void* pm = List_Copy(pli); pm != NULL; pm = List_Copy(pli))
			if (pm != (void*)0x1)
				free(pm);
	}
	else
	{
		PPW("MyBA_Free_R: pli==NULL,return");
	}
	pli->now = pli->pfirst;
}

void MyBA_Free(void* p,List* mem)
{
	if (mem != NULL)
	{
		ListDot* pd = NULL;
		_ULL js = 0;
		for (void* pm = List_Copy(mem); pm != NULL; pm = List_Copy(mem),js++)
		{
			if (pm == p)
			{
				pd = List_IndexDot(mem, js);//because the now could be a NULL and is actually the next
				free(pd->pdata);
				pd->pdata = (void*)0x1;
				break;
			}
		}
	}
	else
	{
		PPW("MyBA_Free: mem==NULL,return");
	}
	mem->now = mem->pfirst;
}

void MyBA_FreeInstance(void)
{
	if (pba->STmem != NULL)
	{
		for (void* pm = List_Copy(pba->STmem); pm != NULL; pm = List_Copy(pba->STmem))
			if (pm != (void*)0x1)
				free(pm);
	}
	else
	{
		PPW("MyBA_FreeInstance: pba->STmem==NULL,return");
	}
	pba->STmem->now = pba->STmem->pfirst;
}

void MyBA_CMD(void)
{
	bool conti = 1;
	List* pcl = List_Init();
	char* pc = NULL;
	do
	{
		pc = MCALLOC(1024 * 1024, char);
		printf("\nMyBA CMD (Ver:1.0):\n");
		fgets(pc, 1024 * 1024 - 1, stdin);

		switch (MyBA_CMD_SearchCom(pc))
		{
		case 0:
			printf("\nQuit MyBA CMD\n");
			conti = 0;
			break;
		case 1:
			MyBA_CMD_ShowLog();
			break;
		default:
			break;
		}
		free(pc);
	} while (conti);
}

int MyBA_CMD_SearchCom(char* pc)
{
	if (strstr(pc, "quit") != NULL)
		return 0;
	return 1;
}

int MyBA_CMD_ShowLog(void)
{
	FILE* pf = NULL;
	if (fopen_s(&pf, StringAdd_S(pba->exepath, "\\mba.log", NULL), "r") == 0)
	{
		pba->PutLog(_strdup("mba.log file opened successfully"));
	}
	else
	{
		pba->PutLog(_strdup("Unable to open mba.log file"));
		fclose(pf);
		return 0;
	}
	BALog* pl = MCALLOC(1, BALog);
	fseek(pf, 0L, SEEK_END);//文件指针置于结尾
	long len1 = ftell(pf);//获取结尾指针值
	fseek(pf, 0L, SEEK_SET);//文件指针至于开头
	long len2 = ftell(pf);//获取开头指针值
	while (len2 < len1)//循环判断
	{
		fread(&(pl->t), sizeof(clock_t), 1, pf);
		pl->pdate = StringRead(pf);
		pl->pc = StringRead(pf);
		fread(&(pl->code), sizeof(int), 1, pf);
		printf("\nBoot:%10.5fs | Date:%s | Code:%d\nContent:<%s>\n", (float)((float)(pl->t) / CLOCKS_PER_SEC), pl->pdate, pl->code, pl->pc);
		free(pl->pc);
		free(pl->pdate);

		len2 = ftell(pf);
	}
	fclose(pf);
	free(pl);
	return 0;
}

void MyBA_SafeMode(void)
{
}

void JDT(_ULL now, _ULL sum)
{
	if (pba->JDT_t == 0)
		pba->JDT_t = clock();
	float a = (float)(now / ((float)sum)) * 100.0;
	float sec = (float)((clock() - pba->JDT_t) / (float)CLOCKS_PER_SEC);
	printf("\r[-Done %5.1f%%,%7.2f s--Need%7.2f s-]         ", a, sec, (float)((sec / a) * (100.0 - a)));
}

void PPIs(int n, ...)
{
	printf("\n");
	va_list parg;
	va_start(parg, n);
	for (int a = 0, b = 0; b < n; printf(" %d ", a), b++)
		a = va_arg(parg, int);
	va_end(parg);
	printf("\n");
}

#ifdef USE_WINDOWS

void gotoxy(int x, int y)//from https://blog.csdn.net/radjedef/article/details/79028329
{
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
}

#endif // USE_WINDOWS


char* mstrdup(const char* p)
{
	char* pret = _strdup(p);
	List_Put(pba->mem, (void*)pret);
	return pret;
}

int* intdup(int num, ...)
{
	BALLOCS_L(int, pret, num, NULL, );
	va_list parg;
	va_start(parg, num);
	for (int a = 0; a < num; a++)
		pret[a] = va_arg(parg, int);
	va_end(parg);
	return pret;
}

_LL* lldup(int num, ...)
{
	BALLOCS_L(_LL, pret, num, NULL, );
	va_list parg;
	va_start(parg, num);
	for (int a = 0; a < num; a++)
		pret[a] = va_arg(parg, _LL);
	va_end(parg);
	return pret;
}

_ULL* ULLdup(_ULL num, ...)
{
	BALLOCS_L(_ULL, pret, num, NULL, );
	va_list parg;
	va_start(parg, num);
	for (int a = 0; a < num; a++)
		pret[a] = va_arg(parg, _ULL);
	va_end(parg);
	return pret;
}

float* floatdup(_ULL num, ...)
{
	BALLOCS_L(float, pret, num, NULL, );
	va_list parg;
	va_start(parg, num);
	for (_ULL a = 0; a < num; a++)
		pret[a] = va_arg(parg, double);
	va_end(parg);
	return pret;
}

int* intdupS(int num, ...)
{
	BALLOCS_S(int, pret, num, NULL, );
	va_list parg;
	va_start(parg, num);
	for (int a = 0; a < num; a++)
		pret[a] = va_arg(parg, int);
	va_end(parg);
	return pret;
}

_ULL* ULLdupS(_ULL num, ...)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!! IT HAS PROBLEMS !!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	BALLOCS_S(_ULL, pret, num, NULL, );
	va_list parg;
	va_start(parg, num);
	_ULL j = 0;
	for (_ULL a = 0; a < num; a++)
		pret[a] = va_arg(parg, _ULL);
	va_end(parg);
	return pret;
}

float* floatdupS(_ULL num, ...)
{
	BALLOCS_S(float, pret, num, NULL, );
	va_list parg;
	va_start(parg, num);
	for (_ULL a = 0; a < num; a++)
		pret[a] = va_arg(parg, double);
	va_end(parg);
	return pret;
}

char* Get_Time_Without_L(void)
{
	BALLOCS_L(char,p,26,NULL,PPW("Get_Time_Without_L:BALLOCS_L Faliue,return NULL"));
	time_t tim = time(NULL);
	if (ctime_s(p, 26, &tim) != 0)
		return NULL;
	*(p + 24) = (char)' ';
	return p;
}

char* Get_Time_Without_S(void)
{
	BALLOCS_S(char, p, 26, NULL, PPW("Get_Time_Without_L:BALLOCS_S Faliue,return NULL"));
	time_t tim = time(NULL);
	if (ctime_s(p, 26, &tim) != 0)
		return NULL;
	*(p + 24) = (char)' ';
	return p;
}

char* Get_Time_L(void)
{
	BALLOCS_L(char, p, 26, NULL, PPW("Get_Time_Without_L:BALLOCS_L Faliue,return NULL"));
	time_t tim = time(NULL);
	if (ctime_s(p, 26, &tim) != 0)
		return NULL;
	return p;
}

char* Get_Time_S(void)
{
	BALLOCS_S(char, p, 26, NULL, PPW("Get_Time_Without_L:BALLOCS_S Faliue,return NULL"));
	time_t tim = time(NULL);
	if (ctime_s(p, 26, &tim) != 0)
		return NULL;
	return p;
}

char* Get_Time_For_File_Name(char char_to_replace_unspport_char)
{
	char* p = Get_Time_Without_L();
	*(p + 13) = *(p + 16) = char_to_replace_unspport_char;
	return p;
}



char* Num_To_Char(const char* ptype, ...)
{
	char* preturn = NULL;
	va_list parg;
	va_start(parg, ptype);
	size_t w = 1;
	if (*ptype == 'd')
	{
		int te = va_arg(parg, int);
		for (int a = 10; te / a != 0; w++, a *= 10);
		preturn = BALLOC_L(w + 1, char);
		sprintf_s(preturn, w + 1, "%d", te);
	}
	else if (*ptype == 'u')
	{
		unsigned int te = va_arg(parg, unsigned int);
		for (int a = 10; te / a != 0; w++, a *= 10);
		preturn = BALLOC_L(w + 1, char);
		sprintf_s(preturn, w + 1, "%u", te);
	}
	else if (strncmp(ptype, "ld", 2) == 0)
	{
		long te = va_arg(parg, long);
		for (int a = 10; te / a != 0; w++, a *= 10);
		preturn = BALLOC_L(w + 1, char);
		sprintf_s(preturn, w + 1, "%ld", te);
	}
	else if (strncmp(ptype, "lu", 2) == 0)
	{
		unsigned long te = va_arg(parg, unsigned long);
		for (int a = 10; te / a != 0; w++, a *= 10);
		preturn = BALLOC_L(w + 1, char);
		sprintf_s(preturn, w + 1, "%lu", te);
	}
	else if (strncmp(ptype, "lld", 3) == 0)
	{
		long long te = va_arg(parg, long long);
		for (int a = 10; te / a != 0; w++, a *= 10);
		preturn = BALLOC_L(w + 1, char);
		sprintf_s(preturn, w + 1, "%lld", te);
	}
	else if (strncmp(ptype, "llu", 3) == 0)
	{
		unsigned long long te = va_arg(parg, unsigned long long);
		for (int a = 10; te / a != 0; w++, a *= 10);
		preturn = BALLOC_L(w + 1, char);
		sprintf_s(preturn, w + 1, "%llu", te);
	}
	/*	else if(*ptype=='f')
		{
			float te = va_arg(parg,float);
			preturn = BALLOC(10,char);
			sprintf_s(preturn,w+1,"%f",te);
		}*/
	else if (*ptype == 'e')
	{
		double te = va_arg(parg, double);
		preturn = BALLOC_L(18, char);
		sprintf_s(preturn, w + 1, "%e", te);
	}
	else
	{
		va_end(parg);
		return (char*)MyBA_Errs(1,"Num_To_Char: type default with", ptype,"return NULL",NULL);
	}
	va_end(parg);
	return preturn;
}

bool Frees(char* ptype, ...)
{
	va_list parg;
	va_start(parg, ptype);
	if (strcmp(ptype, "int") == 0)
	{
		int* pte = NULL;
		for (; (pte = va_arg(parg, int*)) != NULL; pte = NULL)
			free(pte);
	}
	else if (strcmp(ptype, "long") == 0)
	{
		long* pte = NULL;
		for (; (pte = va_arg(parg, long*)) != NULL; pte = NULL)
			free(pte);
	}
	else if (strcmp(ptype, "unsigned long long") == 0)
	{
		unsigned long long* pte = NULL;
		for (; (pte = va_arg(parg, unsigned long long*)) != NULL; pte = NULL)
			free(pte);
	}
	else if (strcmp(ptype, "float") == 0)
	{
		float* pte = NULL;
		for (; (pte = va_arg(parg, float*)) != NULL; pte = NULL)
			free(pte);
	}
	else if (strcmp(ptype, "double") == 0)
	{
		double* pte = NULL;
		for (; (pte = va_arg(parg, double*)) != NULL; pte = NULL)
			free(pte);
	}
	else if (strcmp(ptype, "char") == 0)
	{
		char* pte = NULL;
		for (; (pte = va_arg(parg, char*)) != NULL; pte = NULL)
			free(pte);
	}
	else if (strcmp(ptype, "void") == 0)
	{
		void* pte = NULL;
		for (; (pte = va_arg(parg, void*)) != NULL; pte = NULL)
			free(pte);
	}
	/*	else if(strcmp(ptype,"int") == 0 )
		{
			int* pte = va_arg(parg,int*);
				for( ; (pte = va_arg(parg,int*)) != NULL ; )
					free(pte);
		}
		}*/
	else
	{
		va_end(parg);
		MyBA_Errs(1,"Frees:Can't match the type of", ptype,NULL);
		return 1;
	}
	va_end(parg);
	return 0;
};

char* StringAdd_L(const char* pstr, ...)//end with NULL
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
	BALLOCS_L(char,pret, sumlen, NULL,);
	for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
		strcat_s(pret, sumlen, p);
	va_end(parg);
	return pret;
}

char* StringAdd_S(const char* pstr, ...)//end with NULL
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
	BALLOCS_S(char, pret, sumlen, NULL, );
	for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
		strcat_s(pret, sumlen, p);
	free(plist);
	va_end(parg);
	return pret;
}

int GetDayOfMonth(int year, int month)
{
	//month:1~12
	if (month < 1 || month >12)
	{
		MyBA_Errs(1,"GetDayOfMonth: month is out of range with input:",Num_To_Char("d",month), ",return -1");
		return -1;
	}
	int a[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (year % 4 == 0)
		a[1] = 29;
	return a[month - 1];
}
//***********************************************************************************************************************
void* List_Get(List* plist)
{
	if (plist == NULL)
		return MyBA_Err("get a NULL que", 1);
	ListDot* pret = plist->pfirst;
	if (plist->sumque == 1)
	{
		plist->now = plist->reverse_now = plist->pfirst = plist->plast = NULL;
	}
	else if (plist->sumque == 0)
	{
		return NULL;
	}
	else
	{
		if (plist->now == plist->pfirst)
			plist->now = plist->pfirst->pnext;
		ListDot* pte = plist->pfirst->pnext;
		pte->ppre = NULL;
		plist->pfirst = pte;
	}
	--(plist->sumque);
	void* pdata = pret->pdata;
	free(pret);
	return pdata;
}

void* List_Copy(List* plist)
{
	if (plist == NULL)
		return MyBA_Err("get a NULL que", 1);
	if (plist->now == NULL)//Get the signal
	{
		plist->now = plist->pfirst;//ReSet
		return NULL;
	}
	void* pret = plist->now->pdata;
	plist->now = plist->now->pnext;//If it means the end, it will report a NULL to rise a signal
	return pret;
}

void* List_ReverseCopy(List* plist)
{
	if (plist == NULL)
		return MyBA_Err("get a NULL que", 1);
	if (plist->reverse_now == NULL)
	{
		plist->reverse_now = plist->plast;//ReSet
		return NULL;
	}
	void* pret = plist->reverse_now->pdata;
	plist->reverse_now = plist->reverse_now->ppre;//If it means the end, it will report a NULL to rise a signal
	return pret;
}

void* List_Index(List* plist, _ULL index)
/*
* Description: Get the index dot content,from 0
*/
{
	if (plist == NULL)
		return MyBA_Err("get a NULL plist pram", 1);
	ListDot* p = plist->pfirst;
	for (_ULL i = 0; (i < index) && (p != NULL); i++, p = p->pnext);
	return p->pdata;
}

ListDot* List_IndexDot(List* plist, _ULL index)
{
	if (plist == NULL)
		return (ListDot*)MyBA_Err("get a NULL plist pram", 1);
	ListDot* p = plist->pfirst;
	for (_ULL i = 0; (i < index) && (p != NULL); i++, p = p->pnext);
	return p;
}

void* List_IndexGet(List* plist, _ULL index)
{
	if (plist == NULL)
		return MyBA_Err("get a NULL plist pram", 1);
	ListDot* p = plist->pfirst;
	for (_ULL i = 0; (i < index) && (p != NULL); i++, p = p->pnext);
	if (p == plist->pfirst)
	{
		return List_Get(plist);
	}
	else if (p == plist->plast)
	{
		plist->plast = p->ppre;
		p->ppre->pnext = NULL;
	}
	else
	{
		p->ppre->pnext = p->pnext;
		p->pnext->ppre = p->ppre;
	}
	void* pret = p->pdata;
	free(p);
	return pret;
}

List* List_Put(List* plist, void* pdata)
{
	++(plist->sumque);
	if (plist->sumque == 1)
	{
		plist->pfirst = MCALLOC(1, ListDot);
		if (plist->pfirst == NULL)
		{
			MyBA_Err("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist", 1);
		}
		else
		{
			plist->plast = plist->pfirst;
			plist->pfirst->pnext = plist->pfirst->ppre = NULL;
			plist->pfirst->pdata = pdata;
			plist->pfirst->idx = plist->sumque - 1;
			plist->reverse_now = plist->now = plist->pfirst;
		}
		return plist;
	}
	ListDot* pte = MCALLOC(1, ListDot);
	if (pte == NULL)
	{
		MyBA_Err("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist", 1);
	}
	else
	{
		pte->ppre = plist->plast;
		pte->pnext = NULL;
		plist->plast->pnext = pte;
		plist->plast = pte;
		if (plist->reverse_now == plist->plast->ppre)
			// make sure put opt do not influence rverseCopy opt
			plist->reverse_now = plist->plast;
		pte->pdata = pdata;
		pte->idx = plist->sumque - 1;
	}
	return plist;
}

List* List_Destroy(List* plist)
{
	for (ListDot* pte = plist->pfirst; pte != NULL; pte = pte->pnext)
		free(pte);
	free(plist);
	plist = NULL;
	return NULL;
}

List* List_Init(void)//产生sumthreads个List
{
	MCALLOCS(List,plist,1);
	if (plist == NULL)
	{
		PPW("List * List_Init(void):plist == NULL,return NULL");
		return (List*)0x1;
	}
	else
	{
		plist->sumque = 0;
		plist->Put = List_Put;
		plist->Get = List_Get;
		plist->Copy = List_Copy;
		plist->ReverseCopy = List_ReverseCopy;
		plist->Index = List_Index;
		plist->IndexDot = List_IndexDot;
		plist->IndexGet = List_IndexGet;
		return plist;
	}
}
//***********************************************************************************************************************

//***********************************************************************************************************************
//MyThread* MyThread_Init(_ULL sumthreads)//产生sumthreads个MyThread
//{
//	MyThread* pth = MCALLOC(sumthreads, MyThread);
//	for (_ULL a = 0; a < sumthreads; a++)
//	{
//		pth[a].flag = 1;
//		pth[a].numid = a;
//		pth[a].sumthreads = sumthreads;
//		pth[a].pmtx = MCALLOC(1, mtx_t);
//	}
//	return pth;
//}
//MyThread* MyThread_Start(MyThread* pth, int (*pF)(void*), void* pdata)
//{
//	if (thrd_create(pth->pid, pF, pdata) != thrd_success)
//	{
//		pth->state = 4;
//		PPWs(Num_To_Char("u", pth->numid), "   This thread is in error due to thrd_create");
//		return NULL;
//	}
//	return pth;
//}
////***********************************************************************************************************************
//MyThreadQue* MyThreadQueue_Get(MyThreadQueue* pque)
//{
//	if (pque == NULL)
//	{
//		PPW("get a NULL que");
//		return NULL;
//	}
//	MyThreadQue* pret = NULL;
//	MyThreadQue* pte = NULL;
//	mtx_lock(pque->pmtx);
//	if (cnd_wait(pque->pcndput, pque->pmtx) != thrd_success)
//		return NULL;
//	pret = pque->pfirst;
//	pte = pque->pfirst->pnext;
//	pte->ppre = NULL;
//	pque->pfirst = pte;
//	--pque->sumque;
//	mtx_unlock(pque->pmtx);
//	cnd_signal(pque->pcndget);
//	return pret;
//}
//MyThreadQueue* MyThreadQueue_Put(MyThreadQueue* pque, void* pdata)//TODO递归可一次性加多个data变que
//{
//	mtx_lock(pque->pmtx);
//	if (cnd_wait(pque->pcndget, pque->pmtx) != thrd_success)
//		return NULL;
//	MyThreadQue* pte = MCALLOC(1, MyThreadQue);
//	++pque->sumque;
//	pte->ppre = pque->plast;
//	pte->pnext = NULL;
//	pque->plast->pnext = pte;
//	pque->plast = pte;
//	pte->pdata = pdata;
//	mtx_unlock(pque->pmtx);
//	cnd_signal(pque->pcndput);
//	return pque;
//}
//MyThreadQueue* MyThreadQueue_Destroy(MyThreadQueue* pque)
//{
//	for (MyThreadQue* pte = pque->pfirst; pte != NULL; pte = pte->pnext)
//		free(pte);
//	free(pque);
//	return pque;
//}
//MyThreadQueue* MyThreadQueue_Init(void* pdata)
//{
//	MyThreadQueue* pque = MCALLOC(1, MyThreadQueue);
//	pque->sumque = 1;
//	pque->pfirst = MCALLOC(1, MyThreadQue);
//	pque->plast = pque->pfirst;
//	pque->pfirst->pnext = pque->pfirst->ppre = NULL;
//	pque->pfirst->pdata = pdata;
//	pque->Put = MyThreadQueue_Put;
//	pque->Get = MyThreadQueue_Get;
//	pque->pmtx = MCALLOC(1, mtx_t);
//	pque->pcndput = MCALLOC(1, cnd_t);
//	pque->pcndget = MCALLOC(1, cnd_t);
//	if (!(mtx_init(pque->pmtx, mtx_plain) == thrd_success && cnd_init(pque->pcndput) == thrd_success && cnd_init(pque->pcndget) == thrd_success))
//	{
//		PPW("This queue is in error due to mtx or cnd init");
//		return NULL;
//	}
//	return pque;
//}
//***********************************************************************************************************************

#ifdef USE_WINDOWS
char* GBK_To_UTF8(const char* pc)
{
	int i = MultiByteToWideChar(CP_ACP, 0, pc, -1, NULL, 0);
	wchar_t* pte1 = MCALLOC(strlen(pc) + 1, wchar_t);
	MultiByteToWideChar(CP_ACP, 0, pc, -1, pte1, i);
	i = WideCharToMultiByte(CP_UTF8, 0, pte1, -1, NULL, 0, NULL, NULL);
	char* pte2 = MCALLOC(strlen(pc) + 1, char);
	WideCharToMultiByte(CP_UTF8, 0, pte1, -1, pte2, i, NULL, NULL);
	free(pte1);
	return pte2;
}
#endif
//***********************************************************************************************************************