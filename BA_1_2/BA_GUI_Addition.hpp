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
		//低层级的window
		class layout : public virtual rect
		{//虚基类，与子类共享一个基类rect对象。
		public:
			std::deque< rect*> items;
			std::deque< int> events;
			std::deque< int> statue;//0不存在   1存在且显示   2存在不显示
		};

		// 列表视图，baseItemTy为ba::ui::rect子类的指针
		// item需要new来申请，并在listView析构时于其析构函数调用baseItemTy的析构函数
		// 一次性生成，支持不同高度，元素的re.y被修改为在列表总视图中的y
		class listView : public layout
		{
		public:
			_LL visPixelRange[2] = { 0 };//相对于所有列表元素拼起来
			_LL clickIdx = -1;
			_LL sumHeight = 0;
			bool refreshTex = true;// enable first draw
			std::deque<_LL> eachHeight;
			std::deque<_LL> pixel2idx;
			std::deque< listView*> synListView;

			listView(window* _win, SDL_Rect pos, SDL_Color bgc,
				std::deque< rect*> _items = std::deque< rect*>());
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
		_LL _listView_Data_ApplyDy(_LL dy, listView* list);
		int _listView_check(window* _win, void* _self, int mouseEveCode, void* _pData);

		class inputBox : public label
		{
		public:
			int cursorChrPos = 0;//在cursor的后面插入字符或删除处于cursor的字符，相对于第0个字符的位置
			int edgeWidth = 1;
			int charSize = 12;
			SDL_Color ec = { .a = 255 };//edge color
			std::string allText;
			int visCharRange[2] = { 0 };//字符级别
			SDL_Rect realTextRe = { 0 };//像素级别
			rect* cursor = nullptr;//光标的图形表示，re相对于inputBox
			int showCursor = 1;
			clock_t showCursorClock = clock();

			inputBox(window* _win, SDL_Rect pos,
				int charSize, int edgeWidth = 1,
				SDL_Color ec = { .a = 255 }, SDL_Color cc = { .a = 255 }, SDL_Color bgc = { 255 , 255 , 255 , 255 },
				const char* pc = NULL, TTF_Font* font = NULL);
			void addChar(SDL_Keycode key);
		};
		int _inputBox_check(window* _win, void* _self, int mouseEveCode, void* _pData);
		
		// TODO : 多向
		class dragBar : public rect
		{
		public:
			rect handle;
			float per[2] = { 0.f, 0.f };
			dragBar(window* _win, SDL_Rect pos, SDL_Rect handleShape, float per[2] = {},
				SDL_Color bgc = { 255 , 255 , 255 , 255 }, SDL_Color hc = { .a = 255 });
			SDL_Texture* getTex();
		};
		int _dragBar_check(window* _win, void* _self, int mouseEveCode, void* _pData);
	}
}

#endif