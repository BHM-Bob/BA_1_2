// BA_GUI_Addition based on SDL2
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
			int sumH = 0;
			int singleH = 0;
			int visibleRange[2] = { 0 };
			int sumItems = 0;
			int clickIdx = -1;
			bool moving = true;// enable first draw
			SDL_Rect re;
			std::vector<int> heights;
		};
		int _listView_check(window* _win, void* _pData);
		// 列表视图，baseItemTy为ba::ui::rect子类指针
		// item需要new来申请，并在listView析构时于其析构函数调用baseItemTy的析构函数
		// 一次性生成或动态增加，支持不同高度，支持多列表同步滚动
		template<typename baseItemTy>
		class listView : public rect
		{
		public:
			listView_Data data;
			std::deque< baseItemTy> items;
			std::deque< int> events;
			std::deque< int> statue;//0不存在   1存在且显示   2存在不显示

			listView(window* _win, SDL_Rect pos = {}, SDL_Color bgc = {});
			~listView()
			{
				for (auto item : items)
					delete item;
			}

			void addItem(baseItemTy _item, int _statue = 1);
			void addItems(std::deque< baseItemTy> _items);
			void clear(void);
			SDL_Texture* getTex(void);
		};
		template<typename baseItemTy>
		inline listView<baseItemTy>::listView(window* _win, SDL_Rect pos, SDL_Color bgc)
			: rect(pos, bgc)
		{
			win = _win;
			rendRect();
			data.re = re;
			win->addCheckEventFunc(_listView_check, &data);
		}
		template<typename baseItemTy>
		inline void listView<baseItemTy>::addItem(baseItemTy _item, int _statue)
		{
			items.emplace_back(_item);
			events.emplace_back(0);
			statue.emplace_back(_statue);
			data.singleH = data.singleH == 0 ? _item->re.h : data.singleH;
			data.sumH += _item->re.h;
			++data.sumItems;
			if (data.visibleRange[1] - data.visibleRange[0] < re.h / data.singleH)
				++data.visibleRange[1];
		}
		template<typename baseItemTy>
		inline void listView<baseItemTy>::addItems(std::deque<baseItemTy> _items)
		{
			for (baseItemTy item : _items)
				this->addItem(item);
		}
		template<typename baseItemTy>
		inline void listView<baseItemTy>::clear(void)
		{
			data.sumItems = data.visibleRange[0] = data.visibleRange[1] = 0;
			data.moving = true;
			for (auto item : items)
				delete item;
			items.clear();
			events.clear();
			statue.clear();
		}
		template<typename baseItemTy>
		inline SDL_Texture* listView<baseItemTy>::getTex(void)
		{
			if (!data.moving)
				return tex;
			data.moving = false;
			if (tex)
				SDL_DestroyTexture(tex);
			SDL_Rect reTmp = { 0, 0, 0, data.singleH };
			SDL_Surface* surTmp = SDL_CreateRGBSurface(0, re.w, re.h, 32, 0, 0, 0, 0);
			SDL_BlitScaled(sur, NULL, surTmp, NULL);
			for (int i = data.visibleRange[0]; i < data.visibleRange[1]; i++)
			{
				reTmp.w = items[i]->re.w;
				SDL_BlitScaled(items[i]->sur, NULL, surTmp, &reTmp);
				reTmp.y += data.singleH;
			}
			tex = SDL_CreateTextureFromSurface(win->rend, surTmp);
			SDL_FreeSurface(surTmp);
			return tex;
		}
	}
}

#endif