#include"BA_Base.hpp"
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