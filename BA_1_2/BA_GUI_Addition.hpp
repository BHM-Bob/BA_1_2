﻿// BA_GUI_Addition based on SDL2
// focus on lightweight and simplicity
// addition for BA_GUI
#ifndef BA_BA_GUI_ADDITION_H
#define BA_BA_GUI_ADDITION_H

#include "BA_Base.hpp"
#include "BA_Math.hpp"
#include "BA_GUI.hpp"

namespace ba
{
	namespace ui
	{
		class listView_Data
		{
		public:
			_LL visPixelRange[2] = { 0 };//相对于所有列表元素拼起来
			_LL clickIdx = -1;
			_LL sumHeight = 0;
			bool refreshTex = true;// enable first draw
			SDL_Rect re;//listView.re
			std::deque<_LL> eachHeight;
			std::deque<_LL> pixel2idx;
			std::deque< listView_Data*> synListViewData;

		};
		_LL _listView_Data_ApplyDy(_LL dy, listView_Data* pData);
		int _listView_check(window* _win, void* _pData);
		// 列表视图，baseItemTy为ba::ui::rect子类的指针
		// item需要new来申请，并在listView析构时于其析构函数调用baseItemTy的析构函数
		// 一次性生成，支持不同高度，元素的re.y被修改为在列表总视图中的y
		// TODO : 支持多列表同步滚动
		class listView : public rect
		{
		public:
			listView_Data data;
			std::deque< rect*> items;
			std::deque< int> events;
			std::deque< int> statue;//0不存在   1存在且显示   2存在不显示

			listView(window* _win, SDL_Rect pos, SDL_Color bgc,
				std::deque< rect*> _items = std::deque< rect*>(), bool synToOther = false);
			void gen(std::deque< rect*> items);
			void gen1(rect* _item, bool isEnd = false);
			~listView()
			{//TODO : no need, give it outside
				for (auto item : items)
					delete item;
			}

			void clear(void);
			SDL_Texture* getTex(void);
		};
	}
}

#endif