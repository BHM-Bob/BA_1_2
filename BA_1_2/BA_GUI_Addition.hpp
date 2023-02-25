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
		template<typename baseItemTy>
		class listView : public rect
		{
		public:
			listView_Data data;
			std::deque< baseItemTy> items;
			std::deque< int> events;
			std::deque< int> statue;//0不存在   1存在且显示   2存在不显示

			listView(window* _win, SDL_Rect pos, SDL_Color bgc,
				std::deque< baseItemTy> _items, bool synToOther = false);
			void gen(std::deque< baseItemTy> _items);
			~listView()
			{
				for (auto item : items)
					delete item;
			}

			void clear(void);
			SDL_Texture* getTex(void);
		};
		template<typename baseItemTy>
		inline listView<baseItemTy>::listView(window* _win, SDL_Rect pos, SDL_Color bgc,
			std::deque< baseItemTy> _items, bool synToOther)
			: rect(pos, bgc)
		{
			win = _win;
			rendRect();
			data.re = re;
			if(! synToOther)
				win->addCheckEventFunc(_listView_check, &data);
			gen(_items);
		}
		template<typename baseItemTy>
		inline void listView<baseItemTy>::gen(std::deque<baseItemTy> _items)
		{
			for (baseItemTy item : _items)
			{
				items.emplace_back(item);
				events.emplace_back(0);
				statue.emplace_back(1);
				item->re.y = data.sumHeight;
				data.eachHeight.emplace_back(item->re.h);
				data.pixel2idx.insert(data.pixel2idx.end(), item->re.h, items.size() - 1);
				data.sumHeight += item->re.h;
			}
			data.visPixelRange[1] = min(re.h, data.sumHeight) - 1;
			data.refreshTex = true;
		}
		template<typename baseItemTy>
		inline void listView<baseItemTy>::clear(void)
		{
			data.sumHeight = data.visPixelRange[0] = data.visPixelRange[1] = 0;
			data.eachHeight.clear();
			data.pixel2idx.clear();
			data.synListViewData.clear();
			data.refreshTex = true;
			data.clickIdx = -1;
			for (auto item : items)
				delete item;
			items.clear();
			events.clear();
			statue.clear();
		}
		template<typename baseItemTy>
		inline SDL_Texture* listView<baseItemTy>::getTex(void)
		{
			if (!data.refreshTex)
				return tex;
			data.refreshTex = false;
			if (tex)
				SDL_DestroyTexture(tex);
			_LL firstVisItemIdx = data.pixel2idx[data.visPixelRange[0]];
			_LL lastVisItemIdx = data.pixel2idx[data.visPixelRange[1]];
			_LL firstVisItemVisY = items[firstVisItemIdx]->re.y - data.visPixelRange[0];//<=0
			SDL_Rect reTmp = { 0, firstVisItemVisY, 0, 0};
			SDL_Surface* surTmp = SDL_CreateRGBASurface(re.w, re.h);
			SDL_BlitScaled(sur, NULL, surTmp, NULL);//will make nage y turn 0 !!!
			for (int i = firstVisItemIdx; i <= lastVisItemIdx; i++)
			{
				reTmp.w = items[i]->re.w;
				reTmp.h = items[i]->re.h;
				SDL_BlitScaled(items[i]->sur, NULL, surTmp, &reTmp);//will make nage y turn 0 !!!
				if (i == firstVisItemIdx)
					reTmp.y = firstVisItemVisY + data.eachHeight[i];
				else
					reTmp.y += data.eachHeight[i];
			}
			tex = SDL_CreateTextureFromSurface(win->rend, surTmp);
			SDL_FreeSurface(surTmp);
			return tex;
		}
	}
}

#endif