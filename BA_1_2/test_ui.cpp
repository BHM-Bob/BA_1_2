﻿#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_GUI.hpp"
#include"BA_Test.hpp"

void ba::test::_ui::initSDL2(void)
{
	ba::ui::QUI ui = ba::ui::QUI("win1");

	ba::ui::colorSur* cs = new ba::ui::colorSur(ui.activeWin, NULL, ui.activeWin->re);
	ui.addOtherTex("cst", NULL, &cs->re);
	ba::ui::colorText* ct = new ba::ui::colorText(ui.activeWin, "test");
	ui.addOtherTex("ctt", NULL, &ui.activeWin->re);
	ui.activeWin->title = new ba::ui::label(ui.activeWin, "test", 15);
	ui.activeWin->butts->add("exit", "><", 15, {0,0,0, 255 }, {},
		{30, 0,0,0}, "tr", (SDL_Surface*)1);
	ui.activeWin->exitButtName = "exit";

	char* pc = mstrdup("w");
	for (SDL_Keycode keyboard = ui.activeWin->winState->getKeyboardEve().first; ! ui.pollQuit(); )
	{
		//PPX(pba->GUT(0));
		keyboard = ui.activeWin->winState->getKeyboardEve().first;
		if (keyboard != 0)
		{
			*pc = (char)keyboard;
			PPX(pc);
			ui.delButt("key");
			ui.addButt("key", pc, 20, { 100, 100, 20, 20 });
		}
		ui.updateOtherTex("cst", cs->getTex());
		ui.updateOtherTex("ctt", ct->getTex());
		ui.checkEvent();
		ui.update();


	}		
	ui.addWindow("tmp");
	ui.delWindow("win1");
}

void ba::test::_ui::paint(void)
{
	ba::ui::QUI ui = ba::ui::QUI("paint", 1000, 800, 0, {0, 64, 122, 255});

	ui.activeWin->title = new ba::ui::label(ui.activeWin, "paint", 15);
	ui.activeWin->butts->add("exit", "><", 15, { 0,0,0, 255 }, {},
		{ 30, 0,0,0 }, "tr");
	ui.activeWin->exitButtName = "exit";

	ba::ui::rect brush = ba::ui::rect({ 0, 0, 2, 2 }, {});
	brush.win = ui.activeWin;
	brush.rendRect();
	for ( ; !ui.pollQuit(); )
	{
		ui.activeWin->winState->getMousePos(&(brush.re.x), &(brush.re.y));
		SDL_RenderCopy(ui.activeWin->rend, brush.tex, NULL, &(brush.re));

		ui.checkEvent();
		ui.update(0, 0, 0, 0);
	}
	ui.delWindow("paint");
}

void ba::test::_ui::fileExplore(void)
{
	char* name = mstrdup("fileExplore  ", pba->LTmem);
	char* titleStr = ba::StrAdd(pba->STmem, name, "D:\\", NULL);
	ba::ui::QUI ui = ba::ui::QUI(titleStr, 1000, 800, 0, {0, 64, 122, 255});

	ui.activeWin->title = new ba::ui::label(ui.activeWin, titleStr, 15);
	ui.activeWin->butts->add("exit", "><", 15, { 0,0,0, 255 }, {},
		{ 30, 0,0,0 }, "tr");
	ui.activeWin->exitButtName = "exit";

	auto paths = ba::glob("D:\\*");
	ba::ui::listView<ba::ui::label*> list(ui.activeWin, {0, 20, 800, 600}, {94, 59, 63, 255});
	for (auto& path : paths)
		list.addItem(new ba::ui::label(ui.activeWin, path.string().c_str(),
			20, {255, 255, 255, 255}));
	ui.addOtherTex("list", list.getTex(), &list.re);

	for (; !ui.pollQuit(); )
	{
		ui.updateOtherTex("list", list.getTex());
		ui.checkEvent();
		ui.update();
	}
	ui.delWindow("fileExplore");
}