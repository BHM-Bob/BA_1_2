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
}

ba::command::command(int _argc, char** _argvs)
{
	argc = _argc;
	argvs = _argvs;
	char* chrTmp = NULL;
	if (argc > 0 && argvs)
	{
		FORI(0, argc)
		{
			chrTmp = mstrdup(argvs[i], mem);
			args.Put(chrTmp);
		}

		//struct the cmd
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
					case '-':
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

					case '>':// data flow target rebind
						break;

					default:// arg
						PutArg(chrTmp);
						break;
					}
				}
			}
		}

		//run stack
		cmdStack* st = NULL;
		for (auto pd = stack.pfirst; pd; pd = pd->pnext)
		{
			st = pd->pdata;
			if (strcmp(st->funcObjName, "version") == 0)
			{
				char* ver = BALLOC_R(100, char, mem);
				sprintf_s(ver, 100, "MyBA (Ver: %10.4f; Build: %s %s)\n", pba->BA_Ver, __DATE__, __TIME__);
				data = ver;
				dataType = 'c';
			}
			else
			{

			}
		}

		//output data flow
		switch (dataFlow)
		{
		case 's':
			if (dataType = 'c')
			{
				chrTmp = std::any_cast<char*>(data);
				printf("%s", chrTmp);
			}
			else
			{

			}
			break;

		default:
			break;
		}
	}
}

int ba::command::PutFunc(char* name)
{
	cmdStack* ps = new cmdStack(name);
	stack.Put(ps);
	return 0;
}

int ba::command::PutArg(char* name)
{
	cmdStack* ps = stack.plast->pdata;
	ps->args.Put(name);
	return 0;
}

ba::cmdStack::cmdStack(char* _funcObjName)
{
	funcObjName = _funcObjName;
}

int MyBA_CMD_ShowLog(void)
{
	FILE* pf = NULL;
	if (fopen_s(&pf, ba::StrAdd(pba->STmem, pba->exepath, "\\mba.log", NULL), "r") == 0)
	{
		MyBA_PutLog(_strdup("mba.log file opened successfully"));
	}
	else
	{
		MyBA_PutLog(_strdup("Unable to open mba.log file"));
		if (pf)
			fclose(pf);
		return 0;
	}
	BALog* pl = MCALLOC(1, BALog);
	if (!pl)
	{
		PPW("MCALLOC(1, BALog) == NULL, return -1");
		return -1;
	}
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
		//free(pl->pc);
		//free(pl->pdate);

		len2 = ftell(pf);
	}
	fclose(pf);
	free(pl);
	return 0;
}
