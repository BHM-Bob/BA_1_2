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
	mba -version
	mba -contimode
	mba -open vi.txt
	mba -open [a.txt,b.file,c.obj] > r.txt

args:
	mba: run mba.exe
	-*: stands a func obj
	* after -*: arg for func obj
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
int MyBA_CMD_ShowLog(void);

namespace ba {

	class cmdStack : BA_Base
	{
	public:
		char* funcObjName = NULL;
		balist<char> args = balist<char>();

		cmdStack(char* _funcObjName);
	};

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

		balist<cmdStack> stack = balist<cmdStack>();//actually not the stack

		std::any data;
		//s=stdout, n=nextFuncObj
		char dataFlow = 's';
		//c=char*, b=binary
		char dataType = ' ';

		command(int _argc, char** _argvs);
		int PutFunc(char* name);
		int PutArg(char* name);
	};

}


#endif