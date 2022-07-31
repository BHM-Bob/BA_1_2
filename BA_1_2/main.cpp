/*
 * @Author: BHM-Bob G 2262029386@qq.com
 * @Date: 2021-11-18 22:51:44
 * @LastEditors: BHM-Bob G
 * @LastEditTime: 2022-05-31 00:20:10
 * @Description:
 */
#include "BA_1_2.hpp"
 //mutex m;

int main(int argc, char** argvs)
{
	MyBA_Init();

	dict d = dict();
	d("k1", 8.9f);
	d("k2", mstrdup("GGGKKK"));
	float k1 = d.GetData2Key<float>("kk");
	char* k2 = d.GetData2Key<char*>("k2");

	QUI ui("TEST");
	ui.AddButt("exit", 1, NULL, NULL, NULL, (SDL_Surface*)(0x1));
	while(ui.PollQuit() == 0)
		ui.Update();

	return MyBA_Quit();
}