//BASIC_ALL_1_2 CMD
//H
//Writen By BHM
//2022年9月25日

#ifndef BA_1_2_CMD_H
#define BA_1_2_CMD_H

#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_BioInfo.hpp"

/*
examples for cmd.exe to run:
	mba =version
	mba =contimode
	mba =open -i vi.txt
	mba =open -i [a.txt,b.file,c.obj] > r.txt

args:
	mba: run mba.exe
	=*: stands a func obj
	-*: stands a arg name
	* after -*: arg for func obj
	:*: a var 
	>: data flow output, defualt there is no '>' and is out to stdout
	[*,*,]:list

doc:
	data flow: Command supports multi func obj to call in a line,
		so the output of pre func will store in the data flow.
		When command meets the end of the command line,
		data flow will put the data its stored into stdout(printf).
		If there is a '>',
		the output od data flow trun to the target after '>'.
*/

void MyBA_CMD(int argc = 1, char** argvs = NULL);

namespace ba {

	class cmdStack : BA_Base
	{
	public:
		std::string funcObjName;
		// argTy - arg
		std::map<std::string, std::string> args;

		cmdStack(char* _funcObjName);
	};

	namespace cmd {
		int version(List* mem, std::any & data, char* dataType, cmdStack* stack);
		int openlog(List* mem, std::any& data, char* dataType, cmdStack* stack);
		int open(List* mem, std::any& data, char* dataType, cmdStack* stack);
	}

	class command : BA_Base
	{
	public:
		int argc = 0;
		char** argvs = NULL;

		balist<char> args = balist<char>();
		bool cmdBegin = false;
		bool cmdEnd = false;
		bool cmdLineEnd = false;
		_LL nowCmdIdx = 0;

		bool isErr = false;

		balist<cmdStack> stack = balist<cmdStack>();//actually not the stack

		std::any data;
		//s=stdout, n=nextFuncObj
		char dataFlow = 's';
		//c=char*, b=binary
		char dataType = ' ';

		command(int _argc, char** _argvs);
		command runStack(void);
		command endWork(void);

		int PutFunc(char* name);
		int PutArg(char* argType, char* argName);

		std::map<std::string,
			int (*)(List* mem, std::any& data,
				char* dataType, cmdStack* stack)> name2func = {
			{"version", cmd::version},
			{"openlog", cmd::openlog},
			{"open", cmd::open},
		};

	};
}


#endif