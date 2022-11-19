// from 

#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_JSON.hpp"
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

	ui.addWindow("win2");
	ba::ui::colorSur* cs2 = new ba::ui::colorSur(ui.windows["win2"], NULL, ui.activeWin->re);
	ui.addOtherTex("cst", NULL, &cs2->re, "win2");

	for (; ! ui.pollQuit(); )
	{
		ui.updateOtherTex("cst", cs->getTex());
		ui.updateOtherTex("ctt", ct->getTex());
		ui.checkTitle();
		ui.update();

		ui.updateOtherTex("cst", cs2->getTex(), "win2");
		ui.update("win2");
	}
}