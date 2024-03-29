﻿#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_GUI.hpp"
#include"BA_GUI_Addition.hpp"
#include"BA_Test.hpp"

void ba::test::_ui::initSDL2(void)
{
	ba::ui::QUI ui = ba::ui::QUI("corlor surface");

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
		_ShowFPS_();
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
		ui.update(NULL, true, true, false);
	}
	ui.delWindow("corlor surface");
}

void ba::test::_ui::paint(void)
{
	ba::ui::QUI ui = ba::ui::QUI("paint", 1000, 800, 0, {0, 64, 122, 255});

	ui.activeWin->title = new ba::ui::label(ui.activeWin, "paint", 15);
	ui.addButt("exit", "><", 15,
		{ 0, 0,0,0 }, { 0,0,0, 255 }, {}, "tr");
	ui.activeWin->exitButtName = "exit";
	ba::ui::inputBox* inputBox = new ba::ui::inputBox(ui.activeWin, { 200, 0, 200, 15 }, 15, 1);
	ui.addRect("input", inputBox, ba::ui::_inputBox_check, inputBox);
	ba::ui::dragBar* dragBar = new ba::ui::dragBar(ui.activeWin, { 500, 0, 200, 15 }, { .w = 10 });
	ui.addRect("dragBar", dragBar, ba::ui::_dragBar_check, dragBar);
	ba::ui::label* per = new ba::ui::label(ui.activeWin, " ", 20, { 0,0,0,255 }, { 700, 0, 20, 20 });
	ui.addRect("per", per);
	ba::ui::dragBar* dragBar2 = new ba::ui::dragBar(ui.activeWin, { 500, 50, 15, 200 }, { .h = 10 });
	ui.addRect("dragBar2", dragBar2, ba::ui::_dragBar_check, dragBar2);

	ba::ui::rect brush = ba::ui::rect({ 0, 0, 2, 2 }, {});
	brush.win = ui.activeWin;
	brush.rendRect();
	std::string pc;
	for ( ; !ui.pollQuit(); )
	{
		if(ui.activeWin->winState->getMouseEveCode(&(ui.activeWin->re)) ==1)
		{
			ui.activeWin->winState->getMousePos(&(brush.re.x), &(brush.re.y));
			if(brush.re.y > 15)
				ui.addOtherTex2(brush.tex, new SDL_Rect(brush.re.x, brush.re.y, brush.re.w, brush.re.h));
		}

		pc = std::to_string(dragBar2->per[1] * 100);
		per->changeText(pc.substr(0, pc.find(".")).c_str());

		ui.checkEvent();
		ui.update(0, 1, 1, 0);
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
		list->synListView.emplace_back(list2);
		list2->synListView.emplace_back(list);};
	reGenListA();
	ui.addRect("list", list, ba::ui::_listView_check, list);
	ui.addRect("list2", list2, ba::ui::_listView_check, list2);

	for (; !ui.pollQuit(); )
	{
		if (list->clickIdx != -1)
		{
			nowPath = paths[list->clickIdx];
			root = ba::StrAdd(pba->STmem, labels[list->clickIdx]->text.c_str(), "\\*", NULL);
			paths = ba::glob(root);
			reGenListA();
		}
		if (ui.activeWin->butts.getMouseEveCode("return") == 2)
		{
			nowPath = nowPath.parent_path();
			if(nowPath == nowPath.parent_path())// is "D::\\"
				paths = ba::glob(ba::StrAdd(pba->STmem, nowPath.string().c_str(), "*", NULL));
			else
				paths = ba::glob(ba::StrAdd(pba->STmem, nowPath.string().c_str(), "\\*", NULL));
			reGenListA();
		}
		ui.checkEvent();
		ui.update();
	}
	ui.delWindow("fileExplore");
}