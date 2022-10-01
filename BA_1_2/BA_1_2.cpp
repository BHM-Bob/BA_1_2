//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021-11-18

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_CMD.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"

MyBA* pba;

float MyBA_Ver(void)
{
	return 1.3621f;
	/*
	* 1.1000 : 1.1.0.0 :2020年08月15日：MyUI, MyDir(in C)
	* 1.2000 : 1.2.0.0 :2021年03月28日：MyBA, List
	* 1.2100 : 1.3.0.0 :2021年08月20日：LOFE(仅支持正向传播);至2021年09月20日左右支持反向传播
	* 1.2111 : 1.3.1.1 :2021年08月28日：MyUI添加窗口透明选项，修改报错文本
	* 1.2120 : 1.3.2.0 :2021年08月28日：SDL_GetFontTexture修改逻辑，使得SDL_MyButton选择背景颜色创建时溢出文字可被显示
	* 1.2200 : 1.3.3.1 :2021年08月29日: 将QQDH项目转化为SDL_ColorSur部件，修改MyUI_Addbutt，添加彩色按钮选项
	* 1.2300 : 1.3.4.0 :2021年08月29日: 将QQDH项目转化为SDL_ColorText部件
	* 1.2400 : 1.4.0.0 :2021年11月20日: 升级内存管理机制,支持长时内存和快内存的申请与释放
	* 1.3000 : 2.1.0.0 :2021年11月25日: 使用C++,增加BA_Array类和BA_Dir类,支持一维数组运算,支持文件夹查看
	* 1.3110 : 2.2.1.0 :2021年12月06日: 增加BA_String,为内存申请增加内存量计数
	* 1.3201 : 2.3.0.1 :2022年04月10日: 增加生信序列相似度算法;修复BA_String.split的字符串头尾不处理bug
	* 1.3310 : 2.4.1.1 :2022年04月28日: 增加线程安全队列和线程池;修复List_Destroy, 增加List_Gather
	* 1.3330 : 2.4.2.0 :2022年06月26日: 增加Array.Concat,Array.Sub,Array.Str
	* 1.3340 : 2.4.2.1 :2022年07月21日: 修复ProduceRainbowCol bug
	* 1.3351 : 2.4.3.1 :2022年07月23日: 添加BA退出时注册函数功能，将MyUI与之挂钩；修改部分注释
	* 1.3401 : 2.5.0.1 :2022年07月26日: 添加list(使用template)；修改部分注释
	* 1.3501 : 2.6.0.1 :2022年07月31日: 添加QUI；修改部分注释
	* 1.3600 : 2.7.0.0 :2022年07月31日: 添加dict(使用any & template), 修改MakeSDLCol和MakeSDLRect参数mem为NULL时行为
	* 1.3621 : 2.7.2.1 :2022年08月07日: 将Que和balist合并；修复BA_String::Splitx bug；修改部分注释
	*/
}

void MyBA_Init(int argc, char** argvs, bool safeMode)
{
	pba = MCALLOC(1, MyBA);
	if (pba == NULL)
	{
		PPW("MyBA_Init:MCALLOC==NULL,pba = NULL");
		PPW("Stop at this moment because pba was not Inited, it may result in some problems");
		_SIS_;
	}
	else
	{
		pba->BA_Ver = MyBA_Ver();
		if(argc ==1 )
			printf("\nMyBA (Ver: %10.4f; Build: %s %s) start\n", pba->BA_Ver, __DATE__, __TIME__);

		pba->GUT_t = clock();
		pba->GUT_state = 0;
		pba->JDT_t = 0;

		pba->exepath = _getcwd(NULL, 0);//psd->exeinfo.exepath is <G:\PellesC\My Project\VN7\VN7 v1_3\VN7 1_3>,It's size is 42

		pba->isSAFEMODE = safeMode;

		pba->mem = List_Init();
		pba->LTmem = List_Init();
		pba->STmem = List_Init();

		pba->stacks = new ba::stack();
		ba::singleStack* ps = new ba::singleStack("MyBA_Init", NULL);
		pba->stacks->stacks.emplace_front(ps);

		pba->exitFunc = List_Init();
		pba->exitFuncData = List_Init();

		BALog* plog = MCALLOC(1, BALog);
		if (plog == NULL)
		{
			PPW("MyBA_Init:plog == NULL,pass");
		}
		else
		{
			plog->t = clock();
			plog->pdate = GetTimeWithout(pba->LTmem);
			plog->pc = _strdup("MyBA Start");
		}
		pba->pLog = List_Init();
		pba->pLog->Put(pba->pLog, plog);

		pba->GUT = MyBA_GetUsedTime;

#ifdef USE_OPENCV
		cv::WaitKey(500);
#else
		Sleep(500);
#endif

	srand((unsigned int)time(NULL));
	pba->randomEngine.seed((unsigned int)time(NULL));

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
	if(argc > 1 && argvs)
		MyBA_CMD(argc, argvs);
}

//BALLOC_L
void MyBA_PutLog(const char* pc, const char* head)
{
	if(pba)
	{
		if (!head)
			head = mstrdup("Normal Log:", pba->LTmem);
		BALLOCS_L(BALog, plog, 1, , );
		plog->t = clock();
		plog->pdate = GetTimeWithout(pba->LTmem);
		_ULL sumlen = strlen(pc) + strlen(head);
		plog->pc = ba::StrAdd(pba->LTmem, head, pc, NULL);
		pba->pLog->Put(pba->pLog, plog);
	}
}

//BALLOC_L
void MyBA_PutLogs(const char* head, ...)
{
	if(pba)
	{
		if (!head)
			head = mstrdup("Normal Log:", pba->LTmem);
		va_list parg;
		va_start(parg, head);
		_ULL sumlen = strlen(head) + 1;
		List* plist = List_Init();
		for (char* p = va_arg(parg, char*); p != NULL; p = va_arg(parg, char*))
		{
			sumlen += strlen(p);
			plist->Put(plist, (void*)p);
		}
		va_end(parg);
		BALLOCS_L(char, pret, sumlen, , );
		strcat_s(pret, sumlen, head);
		for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
			strcat_s(pret, sumlen, p);

		free(plist);

		BALLOCS_L(BALog, plog, 1, , );
		plog->t = clock();
		plog->pdate = GetTimeWithout(pba->LTmem);
		plog->pc = pret;
		pba->pLog->Put(pba->pLog, plog);
	}
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
	if (fopen_s(&pf, ba::StrAdd(pba->STmem, pba->exepath, "\\mba.log", NULL), "a") == 0)
	{
		MyBA_PutLog(_strdup("mba.log file opened successfully"));
		if (isquit == 1)
			MyBA_PutLog(_strdup("MyBA Start To Quit"));
	}
	else
	{
		MyBA_PutLog(_strdup("Unable to open mba.log file"));
		if (pf != NULL)
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
		MyBA_PutLog(pc, "Err:");
	}
	return NULL;
}

//End with a NULL
void* MyBA_Errs(bool instance, ...)
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
	for (char* p = (char*)(plist->Get(plist)); p != NULL; p = (char*)plist->Get(plist))
		strcat_s(pret, sumlen, p);
	free(plist);

	if (instance == 1)
		PPW(pret);
	MyBA_PutLog(pret, "Err:");
	return NULL;
}

_ULL MyBA_AtQuit(int(*exitFunc)(void* data, int code, ...), void* data)
{
	List_Put(pba->exitFunc, (void*)exitFunc);
	List_Put(pba->exitFuncData, (void*)data);
	return pba->exitFunc->plast->idx;
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
	pba->STmem->plast->usage = size * count;
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
	pba->LTmem->plast->usage = size * count;
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
	if(pli)
	{
		List_Put(pli, ret);
		pli->plast->usage = size * count;
		pli->tik += pli->plast->usage;
	}
	return ret;
}

int MyBA_Quit(int retVal)
{
	MyBA_WriteLog(true);

	int (*exitFunc)(void* data, int code, ...) = (int (*)(void* data, int code, ...))List_Copy(pba->exitFunc);
	for (void* data = List_Copy(pba->exitFuncData); exitFunc;
		data = List_Copy(pba->exitFuncData),
		exitFunc = (int (*)(void* data, int code, ...))List_Copy(pba->exitFunc))
	{
		if (data != (void*)0x1)
		{
			if (exitFunc(data, 0) != 0)
			{
				MyBA_Errs(1, "int MyBA_Quit(int retVal): exitFunc(pdata, 0) != 0, func No.",
					ba::Num2Str(pba->exitFunc->now ? pba->exitFunc->now->idx-1 : pba->exitFunc->sumque),
					" err!", NULL);
			}
		}
	}

	MyBA_Free_R(pba->mem);
	MyBA_Free_R(pba->LTmem);
	MyBA_Free_R(pba->STmem);
	List_Destroy(pba->mem);
	List_Destroy(pba->LTmem);
	List_Destroy(pba->STmem);
	free(pba->exepath);

	free(pba);
	printf("\nMyBA Going to Quit\n");
	return retVal;
}

void MyBA_Free_R(List* pli, bool destoryList)
{
	if (pba->isSAFEMODE)
		return;// do not free mem manually
	if (pli != NULL)
	{
		for (void* pm = List_Get(pli); pm != NULL; pm = List_Get(pli))
			if (pm != (void*)0x1)
				free(pm);
		pli->now = pli->pfirst;
		if (destoryList)
			free(pli);
	}
	else
	{
		PPW("MyBA_Free_R: pli==NULL,return");
	}
}

void MyBA_Free(void* p, List* mem)
{
	if (mem != NULL)
	{
		ListDot* pd = NULL;
		_ULL js = 0;
		// from last to first
		for (pd = mem->plast; pd; pd = pd->ppre)
		{
			if (pd->pdata == p)
			{
				free(pd->pdata);
				pd->pdata = BA_FREED_PTR;
				break;
			}
		}
		mem->now = mem->pfirst;
	}
	else
	{
		PPW("MyBA_Free: mem==NULL,return");
	}
}

void MyBA_FreeInstance(void)
{
	if (pba->STmem != NULL)
	{
		for (void* pm = List_Get(pba->STmem); pm != NULL; pm = List_Get(pba->STmem))
			if (pm != (void*)0x1)
				free(pm);
		pba->STmem->now = pba->STmem->pfirst;
	}
	else
	{
		PPW("MyBA_FreeInstance: pba->STmem==NULL,return");
	}
}

void MyBA_SafeMode(void)
{
	pba->isSAFEMODE = true;
}

ba::singleStack::singleStack(const char* _funcName, singleStack* _up)
{
	mem = new memRecord();
	mem->stack = this;
	funcName = strdup(_funcName, mem, 0);
	up = _up;
}

ba::stack::stack()
{

}

void JDT(_ULL now, _ULL sum)
{
	if (pba->JDT_t == 0)
		pba->JDT_t = clock();
	float a = (float)(now / ((float)sum)) * 100.0f;
	float sec = (float)((clock() - pba->JDT_t) / (float)CLOCKS_PER_SEC);
	printf("\r[-Done %5.1f%%,%7.2f s--Need%7.2f s-]         ", a, sec, (float)((sec / a) * (100.0f - a)));
}

float* floatDup(List* mem, _ULL num, ...)
{
	float* pret = NULL;
	if (mem)
	{
		pret = BALLOC_R(num, float, mem);
	}
	else
	{
		pret = MCALLOC(num, float);
	}
	if (pret)
	{
		va_list parg;
		va_start(parg, num);
		for (_ULL a = 0; a < num; a++)
			pret[a] = (float)va_arg(parg, double);
		va_end(parg);
	}
	return pret;
}

//https://blog.csdn.net/radjedef/article/details/79028329
void SetConsoleCursor(int x, int y)
{
	COORD pos = { (SHORT)x, (SHORT)y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
}

//https://blog.csdn.net/m0_51819222/article/details/118099067
COORD GetConsoleCursor(void)
{
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		coordScreen.X = csbi.dwCursorPosition.X;
		coordScreen.Y = csbi.dwCursorPosition.Y;
	}
	return coordScreen; //光标位置
}

// if mem == NULL, do not record
char* mstrdup(const char* p, List* mem)
{
	char* pret = _strdup(p);
	if (mem)
		List_Put(mem, (void*)pret);
	return pret;
}

float* TypeDupR(List* mem, _ULL num, float firstData, ...)
{
	float* pret = BALLOC_R(num, float, mem);
	if (pret)
	{
		pret[0] = firstData;
		va_list parg;
		va_start(parg, firstData);
		for (_ULL a = 1; a < num; a++)
			pret[a] = (float)va_arg(parg, double);
		va_end(parg);
	}
	return pret;
}

////!!!!!!!!!!!!!!!!!!!!!!!!!!!
////!!! IT HAS PROBLEMS !!!!!!!
////!!!!!!!!!!!!!!!!!!!!!!!!!!!
//_ULL* ULLdupS(_ULL num, ...)
//{
//	BALLOCS_S(_ULL, pret, num, NULL, );
//	va_list parg;
//	va_start(parg, num);
//	_ULL j = 0;
//	for (_ULL a = 0; a < num; a++)
//		pret[a] = va_arg(parg, _ULL);
//	va_end(parg);
//	return pret;
//}

char* GetTimeWithout(List* mem)
{
	char* p = NULL;
	if (!mem)
		p = MCALLOC(26, char);
	else
		p = BALLOC_R(26, char, mem);
	time_t tim = time(NULL);
	if (!p || ctime_s(p, 26, &tim) != 0)
		return NULL;
	*(p + 24) = (char)' ';
	return p;
}

//GetTimeWithout(pba->STmem)
char* Get_Time_For_File_Name(char char_to_replace_unspport_char)
{
	char* p = GetTimeWithout(pba->STmem);
	*(p + 13) = *(p + 16) = char_to_replace_unspport_char;
	return p;
}

int GetDayOfMonth(int year, int month)
{
	//month:1~12
	if (month < 1 || month >12)
	{
		MyBA_Errs(1, "GetDayOfMonth: month is out of range with input:", ba::Num2Str(month), ",return -1");
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
		return MyBA_Err("void* List_Get(List* plist)::get a NULL que, return NULL", 1);
	ListDot* pret = plist->pfirst;
	if (plist->sumque == 1)
	{
		plist->now = plist->pfirst = plist->plast = NULL;
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

//Get the index dot content,from 0
void* List_Index(List* plist, _ULL index)
{
	if (plist == NULL)
		return MyBA_Err("get a NULL plist pram", 1);
	ListDot* p = plist->pfirst;
	for (_ULL i = 0; (i < index) && p; i++, p = p->pnext);
	if (p)
		return p->pdata;
	else
		return BA_FREED_PTR;
}

List* List_Put(List* plist, void* pdata)
{
	++(plist->sumque);
	if (plist->sumque == 1)
	{
		plist->pfirst = MCALLOC(1, ListDot);
		if (plist->pfirst == NULL)
		{
			PPW("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist");
		}
		else
		{
			plist->plast = plist->pfirst;
			plist->pfirst->pnext = plist->pfirst->ppre = NULL;
			plist->pfirst->pdata = pdata;
			plist->pfirst->idx = plist->sumque - 1;
			plist->now = plist->pfirst;
		}
		return plist;
	}
	ListDot* pte = MCALLOC(1, ListDot);
	if (pte == NULL)
	{
		PPW("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist");
	}
	else
	{
		pte->ppre = plist->plast;
		pte->pnext = NULL;
		plist->plast->pnext = pte;
		plist->plast = pte;
		pte->pdata = pdata;
		pte->idx = plist->sumque - 1;
	}
	return plist;
}
void List_SetVar(List* plist, void* pdata, void* newVar)
{
	if (plist == NULL)
	{
		MyBA_Err("get a NULL plist pram", 1);
		return ;
	}
	ListDot* p = plist->pfirst;
	for ( ; (p != NULL) && (p->pdata != pdata); p = p->pnext);
	if (p)
		p->pdata = newVar;
}


List* List_Destroy(List* plist)
{
	for (ListDot* p = plist->pfirst, *pn = NULL; p; p = pn)
	{
		pn = p->pnext;
		free(p);
	}
	free(plist);
	return NULL;
}

List* List_Init(void* pdata)
{
	List* plist = MCALLOC(1, List);
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
		plist->Index = List_Index;

		if(pdata)
			List_Put(plist, pdata);
		return plist;
	}
}
//***********************************************************************************************************************


BA_Base::BA_Base(_ULL idx, const char* name)
{
	__idx__ = idx;
	if (name)
		__name__ = mstrdup(name, mem);
}

//***********************************************************************************************************************

int StrCmpById(const char* ptr1, const char* ptr2)
{
	return (ptr1 == ptr2) ? 0 : 1;
}

badictPair::badictPair()
{
}

badictPair::badictPair(const char* _key, std::any _data, bool _justUseKeyPtr)
{
	if (_justUseKeyPtr)
	{
		_ULL id = (_ULL)_key;
		key = (char*)_key;
	}
	else
	{
		key = _strdup(_key);
	}
	data = _data;
}

badictPair::~badictPair()
{
}

void badictPair::operator=(std::any _data)
{
	data.reset();
	data = _data;
}

badict::badict(bool _justUseKeyPtr)
{
	justUseKeyPtr = _justUseKeyPtr;
}

badict::badict(const char* key, std::any data, bool _justUseKeyPtr)
{
	this->Put(key, data, _justUseKeyPtr);
}

bool badict::HasKey(const char* key, bool justCmpKeyById)
{
	if (justCmpKeyById)
		strCmpFunc = StrCmpById;
	badictPair* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (!strCmpFunc(pd->key, key))
			return true;
	return false;
}

badict badict::Put(const char* key, std::any data, bool _justUseKeyPtr)
{
	++sumque;
	if (sumque == 1)
	{
		pfirst = new badictPair(key, data, _justUseKeyPtr);
		if (pfirst == NULL)
		{
			MyBA_Err("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist", 1);
		}
		else
		{
			plast = pfirst;
			pfirst->pnext = pfirst->ppre = NULL;
			pfirst->idx = sumque - 1;
		}
	}
	else
	{
		badictPair* pte = new badictPair(key, data);
		if (pte == NULL)
		{
			MyBA_Err("List* List_Put(List* plist, void* pdata): MCALLOC(1, ListDot) == NULL, return plist", 1);
		}
		else
		{
			pte->ppre = plast;
			pte->pnext = NULL;
			plast->pnext = pte;
			plast = pte;
			pte->idx = sumque - 1;
		}
	}
	return *this;
}

bool badict::Del(const char* key, bool freeKey)
{
	badictPair* pd = pfirst;
	for (; pd; pd = pd->pnext)
		if (!strcmp(pd->key, key))
		{
			if (pd == pfirst)
			{
				pfirst = pd->pnext;
				if(pd->pnext)
					pd->pnext->ppre = NULL;
			}
			else if (pd == plast)
			{
				plast = pd->ppre;
				if(pd->ppre)
					pd->ppre->pnext = NULL;
			}
			else
			{
				pd->ppre->pnext = pd->pnext;
				pd->pnext->ppre = pd->ppre;
			}
			if(freeKey)
				free(pd->key);
			delete pd;
			--sumque;
			return true;
		}
	PPWs("No Such Key: ", key);
	return false;
}

void badict::Destroy(void)
{
	for (badictPair* p = pfirst, *pn = NULL; p; p = pn)
	{
		free(p->key);
		delete p;
	}
}

badict::~badict()
{
}

//dict dict::operator+(dict& other)
//{
//	dict* ret = new dict();
//	this->now = this->pfirst;
//	for (dataType* p = this.Copy(); p; p = other.Copy())
//		ret->Put(p);
//	other.now = other.pfirst;
//	for (dataType* p = other.Copy(); p; p = other.Copy())
//		ret->Put(p);
//	return *ret;
//}



//***********************************************************************************************************************
//*******************************************************************************************************************