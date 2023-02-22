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
		ui.checkButt();
		ui.update();


	}		
	ui.addWindow("tmp");
	ui.delWindow("win1");
}

void ba::test::_ui::paint(void)
{
	ba::ui::QUI ui = ba::ui::QUI("paint", 1000, 800, 0, {0, 64, 122, 255});

	ui.activeWin->title = new ba::ui::label(ui.activeWin, "test", 15);
	ui.activeWin->butts->add("exit", "><", 15, { 0,0,0, 255 }, {},
		{ 30, 0,0,0 }, "tr");
	ui.activeWin->exitButtName = "exit";

	ba::ui::rect brush = ba::ui::rect({ 0, 0, 2, 2 }, {});
	brush.win = ui.activeWin;
	brush.rendRect();
	for (Sint32 x = 0, y = 0; !ui.pollQuit(); )
	{
		ui.activeWin->winState->getMousePos(&(brush.re.x), &(brush.re.y));
		SDL_RenderCopy(ui.activeWin->rend, brush.tex, NULL, &(brush.re));

		ui.checkButt();
		ui.update(0, 0, 0, 0);
	}
	ui.delWindow("win1");
}