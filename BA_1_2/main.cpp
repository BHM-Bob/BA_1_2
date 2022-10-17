#include "BA_1_2.hpp"
 //mutex m;

int main(int argc, char** argvs)
{
	MyBA_Init(argc, argvs);

	//ba::test::_thread::WordsCount();
	//ba::test::_cmd::openlog();
	//ba::test::_comprehensive::jiebaThreads();
	ba::test::_comprehensive::tensor();

	return MyBA_Quit();
}

