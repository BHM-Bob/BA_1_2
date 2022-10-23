// from 

#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_JSON.hpp"
#include"BA_GUI.hpp"
#include"BA_Test.hpp"

void ba::test::_ui::initSDL2(void)
{
	ba::ui::QUI ui = ba::ui::QUI();
	ui.win->title = new ba::ui::label(&ui, "test", 30);
	ba::ui::colorSur* cs = new ba::ui::colorSur(&ui, NULL, ui.win->re);
	ui.otherTex["cst"] = new std::pair((SDL_Texture*)NULL, &(ui.win->re));
	ba::ui::colorText* ct = new ba::ui::colorText(ui.rend, ui.defaultFont, "TEST");
	ui.otherTex["ctt"] = new std::pair((SDL_Texture*)NULL, &(ui.win->re));
	for (auto p = ui.otherTex["cst"]; ! ui.pollQuit(); )
	{
		p = ui.otherTex["cst"];
		if (p->first)
			SDL_DestroyTexture(p->first);
		p->first = cs->getTex();

		p = ui.otherTex["ctt"];
		if(p->first)
			SDL_DestroyTexture(p->first);
		p->first = ct->getTex();

		ui.checkTitle();
		ui.update();
	}
}