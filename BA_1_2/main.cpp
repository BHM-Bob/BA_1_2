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
	// TODO : use id to cmp in list and dict

	MyBA_Init();

	BA_Array a = BA_Array(BA_Shape(3, 90, 90, 90), "rand");
	BA_Array b = BA_Array(BA_Shape(3, 90, 90, 90), "l");
	BA_Array c = a * (b + 1LL);

	dict d = dict();
	d["k1"] = 8.9f;
	d["k2"] = mstrdup("GGGKKK");
	d["k3"] = a;
	float k1 = d.Copy<float>("kk");
	char* k2 = d.Copy<char*>("k2");

	QUI ui("TEST");
	ui.AddButt("toShowKey", " ", 20, NULL, NULL, NULL, QUI_WIN_NULLSUR);
	ui.win->pre_title = &(ui.butts->butts["toShowKey"]->re_butt);
	while(ui.PollQuit() == 0)
		ui.Update();

	return MyBA_Quit();
}

