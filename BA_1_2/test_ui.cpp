﻿// from 

#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_JSON.hpp"
#include"BA_GUI.hpp"
#include"BA_Test.hpp"

void ba::test::_ui::initSDL2(void)
{
	ba::ui::QUI ui = ba::ui::QUI();
	ba::ui::colorSur* cs = new ba::ui::colorSur(&ui, NULL, ui.win->re);
	ui.otherTex["cst"] = new std::pair((SDL_Texture*)NULL, &(ui.win->re));
	ba::ui::colorText* ct = new ba::ui::colorText(&ui, "test");
	ui.otherTex["ctt"] = new std::pair((SDL_Texture*)NULL, &(ui.win->re));
	ui.win->title = new ba::ui::label(&ui, "test", 15);
	ui.butts->add("exit", "><", 15, { 255,255, 255, 255 }, {},
		{300, 10,0,0}, "tr", (SDL_Surface*)1);
	ui.win->exitButtName = "exit";
	for (; ! ui.pollQuit(); )
	{
		ui.otherTex["cst"]->first = cs->getTex();
		ui.otherTex["ctt"]->first = ct->getTex();

		ui.checkTitle();
		ui.update();
	}
}