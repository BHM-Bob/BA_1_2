#include "BA_BASE.HPP"
#include "BA_CMD.hpp"
#include "BA_String.hpp"

void MyBA_CMD(int argc, char** argvs)
{
	if (argc == 1)//maual entry
	{
		auto s = ba::str().Input(1024 * 1024);
		if (!s.Find(" "))//only one cmd
		{
			argc = 2;
			argvs = BALLOC_S(2, char*);
			argvs[1] = s.pc;
		}
		else
		{
			balist<char>* l = s.Split(" ");
			argc = (int)(l->sumque) + 1;
			//unknown err: C2440:"类型强制转换"：无法从"double"转换为"char"
			//argvs = BALLOC_R(argc, char*, pba->STmem);
			argvs = BALLOC_S(argc, char*);
			for (auto pd = l->pfirst; pd; pd++)
				argvs[pd->idx+1] = pd->pdata;
		}
	}
	ba::command cmd = ba::command(argc-1, argvs+1);
	cmd.runStack().endWork();
}

ba::command::command(int _argc, char** _argvs)
{
	argc = _argc;
	argvs = _argvs;
	char* chrTmp = NULL, *chrTmp2 = NULL;
	if (argc > 0 && argvs)
	{
		// make args list
		FORI(0, argc)
		{
			chrTmp = mstrdup(argvs[i], mem);
			args.Put(chrTmp);
		}

		//build stack
		while (!cmdLineEnd)
		{
			while (!cmdEnd)
			{
				chrTmp = args[nowCmdIdx++];
				if (chrTmp == args.errPtr)
				{
					cmdEnd = cmdLineEnd = true;
				}
				else
				{
					switch (chrTmp[0])
					{
					case '=':
						if (cmdBegin)// meet next cmd
						{
							cmdBegin = false;
							cmdEnd = true;
							--nowCmdIdx;//wait for next loop to handle
							break;
						}
						else// new cmd begin
						{
							cmdBegin = true;
							PutFunc(chrTmp + 1);
						}
						break;

					case '-':// arg : -i a.txt
						chrTmp2 = args[nowCmdIdx++];
						if (chrTmp2 == args.errPtr || strchr("=->:", *chrTmp2))
						{
							PPW(StrAdd(mem, "\nba::command::command(int _argc, char** _argvs): arg after",
								chrTmp, " has err.\nIgnore this arg.", NULL));
							--nowCmdIdx;
						}
						else
						{
							PutArg(chrTmp + 1, chrTmp2);
						}
						break;

					case '>':// data flow target rebind
						break;

					default:// ?
						break;
					}
				}
			}
		}
	}
}

ba::command ba::command::runStack(void)
{
	if(! isErr)
	{
		//run stack
		std::string funcNames;
		int (*func)(List * mem, std::any & data, char* dataType, cmdStack* stack) = NULL;
		for (auto d = name2func.begin(); d != name2func.end(); d++)
			funcNames += d->first;
		cmdStack* st = NULL;
		for (auto pd = stack.pfirst; pd; pd = pd->pnext)
		{
			st = pd->pdata;
			if (funcNames.find(st->funcObjName) != std::string::npos)
			{
				func = name2func[st->funcObjName];
				func(mem, std::ref(data), &dataType, st);
			}
			else
			{
				//???
			}
		}
	}
	return *this;
}

ba::command ba::command::endWork(void)
{
	if (!isErr)
	{
		//output data flow
		char* chrTmp = NULL, * chrTmp2 = NULL;
		switch (dataFlow)
		{
		case 's':
			if (dataType = 'c')
			{
				chrTmp = std::any_cast<char*>(data);
				printf("%s", chrTmp);
			}
			else if (dataType = 'l')
			{
				//???
			}
			break;

		default:
			break;
		}
	}
	return *this;
}

ba::cmdStack::cmdStack(char* _funcObjName)
{
	funcObjName.assign(_funcObjName);
}

int ba::command::PutFunc(char* name)
{
	cmdStack* ps = new cmdStack(name);
	stack.Put(ps);
	return 0;
}

int ba::command::PutArg(char* argType, char* argName)
{
	cmdStack* ps = stack.plast->pdata;
	ps->args[argType] = argName;
	return 0;
}

int ba::cmd::version(List* mem, std::any& data, char* dataType, cmdStack* stack)
{
	char* ver = BALLOC_R(100, char, mem);
	sprintf_s(ver, 100, "MyBA (Ver: %10.4f; Build: %s %s)\n", pba->BA_Ver, __DATE__, __TIME__);
	data = ver;
	*dataType = 'c';
	return 0;
}

int ba::cmd::openlog(List* mem, std::any& data, char* dataType, cmdStack* stack)
{
	FILE* pf = NULL;
	char* root = _getcwd(NULL, 0);
	if (fopen_s(&pf, ba::StrAdd(pba->STmem, root, "\\mba.log", NULL), "r"))
	{
		PPW("Unable to open mba.log file");
		if (pf)
			fclose(pf);
		return -1;
	}
	BALog* pl = MCALLOC(1, BALog);
	if (!pl)
	{
		PPW("MCALLOC(1, BALog) == NULL, return -1");
		return -2;
	}
	fseek(pf, 0L, SEEK_END);//文件指针置于结尾
	long len1 = ftell(pf);//获取结尾指针值
	fseek(pf, 0L, SEEK_SET);//文件指针至于开头
	long len2 = ftell(pf);//获取开头指针值
	char* strTmp = MCALLOC(len1 - len2 + 1, char);
	char* str = BALLOC_R(2 * (len1 - len2) + 1, char, mem);
	if (!strTmp || !str)
	{
		PPW("OOM");
		return -3;
	}
	_LL idx = 0;
	while (len2 < len1)//循环判断
	{
		fread(&(pl->t), sizeof(clock_t), 1, pf);
		pl->pdate = StringRead(pf);
		pl->pc = StringRead(pf);
		fread(&(pl->code), sizeof(int), 1, pf);
		sprintf_s(strTmp, len1 - len2,
			"\nBoot:%10.5fs | Date:%s | Code:%d\nContent:<%s>\n",
			(float)((float)(pl->t) / CLOCKS_PER_SEC), pl->pdate, pl->code, pl->pc);
		memcpy(str + idx, strTmp, strlen(strTmp));
		idx += strlen(strTmp);
		free(strTmp);
		strTmp = MCALLOC(len1 - len2 + 1, char);
		if (!strTmp)
		{
			PPW("OOM");
			return -4;
		}
		len2 = ftell(pf);
	}
	fclose(pf);
	free(pl);
	str[idx + 1] = '\0';
	data = str;
	*dataType = 'c';
	return 0;
}

int ba::cmd::open(List* mem, std::any& data, char* dataType, cmdStack* stack)
{
	std::string ipath = stack->args["i"];
	balist<char>* paths = NULL;
	ba::str* strT = new ba::str();
	char* root = _getcwd(NULL, 0), *pt = NULL, *r = NULL;
	if (ipath.size() == 0)
	{
		PPW("ipath.size() == 0");
		return -1;
	}
	if (ipath[0] == '[')//list args
	{
		if (ipath.find(']') == std::string::npos)
		{
			PPW("err list without \']\', do noting");
			return -2;
		}
		std::string subStr = ipath.substr(1, ipath.find(']') - 1);
		ba::str* newIPath = new ba::str(subStr.c_str());
		paths = newIPath->Split(",");
	}
	else
	{
		paths = new balist(ipath.data(), NULL);
	}
	for (char* path = paths->Copy(); path; path = paths->Copy())
	{
		//Relative path => abs path
		if (!strchr(path, ':'))
			path = StrAdd(mem, root, "\\", path, NULL);
		pt = ReadTXT(path, 0, mem);
		r = ba::StrAdd(mem, r, pt, NULL);
	}
	data = r;
	*dataType = 'c';
	return 0;
}
