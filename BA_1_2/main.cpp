/*
 * @Author: BHM-Bob G 2262029386@qq.com
 * @Date: 2021-11-18 22:51:44
 * @LastEditors: BHM-Bob
 * @LastEditTime: 2022-10-06 17:09:31
 * @Description:
 */
#include "BA_1_2.hpp"
 //mutex m;

int main(int argc, char** argvs)
{
	MyBA_Init(argc, argvs);

	//ba::test::_thread::WordsCount();
	ba::test::_cmd::openlog();

	return MyBA_Quit();
}

