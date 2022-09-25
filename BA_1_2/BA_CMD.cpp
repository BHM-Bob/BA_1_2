#include "BA_BASE.HPP"
#include "BA_CMD.hpp"

void MyBA_CMD(int argc, char** argvs)
{
	bool conti = 1;
	List* pcl = List_Init();
	char* pc = NULL;
	ba::command cmd = ba::command(argc, argvs);
}

ba::command::command(int _argc, char** _argvs)
{
	argc = _argc;
	argvs = _argvs;
	char* chrTmp = NULL;
	if (argc > 2 && argvs)
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
					cmdLineEnd = true;
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
		for (auto pd = stack.pfirst; pd; pd++)
		{
			st = pd->pdata;
			if (strcmp(st->funcObjName, "version"))
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
