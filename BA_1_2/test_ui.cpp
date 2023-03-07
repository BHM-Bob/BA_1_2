#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_GUI.hpp"
#include"BA_GUI_Addition.hpp"
#include"BA_Test.hpp"

void ba::test::_ui::initSDL2(void)
{
	ba::ui::QUI ui = ba::ui::QUI("win1");

	ba::ui::colorSur* cs = new ba::ui::colorSur(ui.activeWin, NULL, ui.activeWin->re);
	ui.addOtherTex("cst", NULL, &cs->re);
	ba::ui::colorText* ct = new ba::ui::colorText(ui.activeWin, "test");
	ui.addOtherTex("ctt", NULL, &ui.activeWin->re);
	ui.activeWin->title = new ba::ui::label(ui.activeWin, "test", 15);
	ui.addButt("exit", "><", 15,
		{0,0,0,0}, {0,0,0, 255 }, {}, "tr", (SDL_Surface*)1);
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
			ui.addButt("key", pc, 20, { 100, 100, 20, 20 }, {255, 255, 255, 255}, {0});
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
	ui.addButt("exit", "><", 15,
		{ 30, 0,0,0 }, { 0,0,0, 255 }, {}, "tr");
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

	ui.addButt("exit", "><", 15,
		{ 0,0,0,0 }, { 0,0,0, 255 }, { 0, 200, 100, 255 }, "tr");
	ui.activeWin->exitButtName = "exit";
	ui.addButt("return", "return", 15,
		{ 0,15,0,0 }, { 0,0,0, 255 }, {0, 200, 100, 255}, "tl");

	char* root = mstrdup("D:\\*", pba->STmem);
	ui.activeWin->title = new ba::ui::label(ui.activeWin, titleStr, 15);
	auto paths = ba::glob(root);
	std::filesystem::path nowPath;
	std::deque<ba::ui::label*> labels;
	auto reGenList = [&](ba::ui::listView* list) {
		list->clear();
		labels.clear();
		for (auto& path : paths)
		{
			labels.emplace_back(new ba::ui::label(ui.activeWin, path.string().c_str(), 20));
			list->gen1(labels.back(), list->items.size() == labels.size() - 1);
		}
		return list; };
	ba::ui::listView* list = new ba::ui::listView(ui.activeWin, { 0, 40, 800, 600 }, { 94, 59, 63, 255 });
	ba::ui::listView* list2 = new ba::ui::listView(ui.activeWin, { 900, 40, 100, 600 }, { 0, 59, 63, 255 });
	auto reGenListA = [&]() {
		list = reGenList(list);
		list2 = reGenList(list2);
		list->data.synListViewData.emplace_back(&(list2->data));
		list2->data.synListViewData.emplace_back(&(list->data));};
	reGenListA();
	ui.addRect("list", list, ba::ui::_listView_check, list);
	ui.addRect("list2", list2, ba::ui::_listView_check, list2);

	for (; !ui.pollQuit(); )
	{
		if (list->data.clickIdx != -1)
		{
			nowPath = paths[list->data.clickIdx];
			root = ba::StrAdd(pba->STmem, labels[list->data.clickIdx]->text.c_str(), "\\*", NULL);
			paths = ba::glob(root);
			reGenListA();
		}
		if (ui.activeWin->butts.events["return"] == 2)
		{
			ui.activeWin->butts.events["return"] = 0;
			nowPath = nowPath.parent_path();
			if(nowPath == nowPath.parent_path())// is "D::\\"
				paths = ba::glob(ba::StrAdd(pba->STmem, nowPath.string().c_str(), "*", NULL));
			else
				paths = ba::glob(ba::StrAdd(pba->STmem, nowPath.string().c_str(), "*", NULL));
			reGenListA();
		}
		ui.checkEvent();
		ui.update();
	}
	ui.delWindow("fileExplore");
}