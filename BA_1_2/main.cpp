﻿#include "BA_1_2.hpp"
 //mutex m;

int main(int argc, char** argvs)
{
	MyBA_Init(argc, argvs);

	//ba::test::_thread::WordsCount();
	//ba::test::_cmd::openlog();
	//ba::test::_comprehensive::jiebaThreads();
	//ba::test::_comprehensive::glob();
	ba::test::_ui::initSDL2();
	ba::test::_ui::paint();
	ba::test::_ui::fileExplore();
	//ba::test::_comprehensive::range();

	return MyBA_Quit();
}

