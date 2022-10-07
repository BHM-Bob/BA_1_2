
#include"BA_Base.hpp"
#include"BA_CMD.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Test.hpp"
#include"BA_BioInfo.hpp"
#include"BA_JSON.hpp"

void ba::test::_cmd::open(void)
{
	int argc = 4;
	char** argvs = MCALLOC(argc, char*);
	argvs[1] = mstrdup("=open");
	argvs[2] = mstrdup("-i");
	argvs[3] = mstrdup("[D:\\Bio\\BioInfoLessonSTU\\Class4\\data\\dreb2_AtOs.fasta,D:\\Bio\\BioInfoLessonSTU\\Class4\\data\\dreb2_FtAtOs.fasta]");
	MyBA_CMD(argc, argvs);
}

void ba::test::_cmd::openlog(void)
{
	int argc = 2;
	char** argvs = MCALLOC(argc, char*);
	argvs[1] = mstrdup("=openlog");
	MyBA_CMD(argc, argvs);
}

void ba::test::_cmd::_missMatch(void)
{
}
