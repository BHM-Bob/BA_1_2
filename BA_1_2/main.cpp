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

	QUI ui("TEST");
	ui.AddButt("exit", 1, NULL, NULL, NULL, (SDL_Surface*)(0x1));
	while(ui.PollQuit() == 0)
		ui.Update();

	//MyUI* pui = MyUI_Init("TEST", 800, 800, 0, NULL);
	//pui->pF_Addbutt(pui, 0, "exit", 1, intdup(3, 0, 0, 0), intdup(3, 0, 0, 0), intdup(4, 0, 0, 800, 800), (SDL_Surface*)(0x1));
	//while(pui->pF_PollQuit(pui) == 0)
	//	pui->pF_Update(pui, 1, 1);

	return MyBA_Quit();
}