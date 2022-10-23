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
	ba::ui::colorSur* cs = new ba::ui::colorSur(ui.win->sur);
	ba::ui::colorText* ct = new ba::ui::colorText(ui.rend, ui.defaultFont, "TEST");
	ui.otherTex["ctt"] = new std::pair((SDL_Texture*)NULL, &(ui.win->re));
	auto p = ui.otherTex["ctt"];
	while (! ui.pollQuit())
	{
		cs->update();
		SDL_DestroyTexture(ui.win->tex);
		ui.win->tex = SDL_CreateTextureFromSurface(ui.rend, ui.win->sur);

		p = ui.otherTex["ctt"];
		if(p->first)
			SDL_DestroyTexture(p->first);
		p->first = ct->getTex();

		ui.checkTitle();
		ui.update();
	}
}