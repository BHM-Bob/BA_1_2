// BA_GUI based on SDL2
// focus on lightweight and simplicity
// from BA_UI and BA_QUI in master branch
#ifndef BA_BA_GUI_H
#define BA_BA_GUI_H
/*
E:\My_Progs\BA\Cpp\BA_1_2\BA_1_2\SDL2\lib\SDL2.lib
E:\My_Progs\BA\Cpp\BA_1_2\BA_1_2\SDL2\lib\SDL2_image.lib
E:\My_Progs\BA\Cpp\BA_1_2\BA_1_2\SDL2\lib\SDL2_mixer.lib
E:\My_Progs\BA\Cpp\BA_1_2\BA_1_2\SDL2\lib\SDL2main.lib
E:\My_Progs\BA\Cpp\BA_1_2\BA_1_2\SDL2\lib\SDL2_ttf.lib
*/
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#include "BA_Base.hpp"
#include "BA_Math.hpp"

// TODO : use threads
// TODO : maybe later : template or func obj of button func
// TODO : some intersting things in SDL2
	//SDL_GetSystemRAM : https://wiki.libsdl.org/SDL2/SDL_GetSystemRAM
	//SDL_GetCPUCount : https://wiki.libsdl.org/SDL2/SDL_GetCPUCount
// TODO : 内存泄露（退出时释放内存）

namespace ba
{
	namespace ui
	{
		int* ProduceRainbowCol(int* col, float* i, float* di);
		int* ProduceRainbowCol(int* col, float* i, float di = 0.05f);
		SDL_Color* SetSDLCol(SDL_Color* col, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		SDL_Color* MakeSDLCol(List* mem, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
		SDL_Rect* SetSDLRect(SDL_Rect* pos, int w, int h, int x, int y);
		SDL_Rect* MakeSDLRect(List* mem, int w, int h, int x, int y);
		Uint32 getPixle(SDL_Surface* surface, int x, int y);
		int formatPixle(Uint32 pixle, char channle = 'r');
		SDL_Texture* createRGBTexture(SDL_Renderer* rend,
			int w, int h, int r, int g, int b);
		// make sure that path is utf-8 coded
		SDL_Surface* getImageSur(const char* path);
		// make sure that path is utf-8 coded
		SDL_Texture* getImageTex(SDL_Renderer* renderer, const char* path);
		inline void SDL_print_Texture(SDL_Renderer* rend, SDL_Rect Box, SDL_Point center, int angel, SDL_Texture* tex)
		{
			SDL_RenderCopyEx(rend, tex, NULL, &Box, angel, &center, SDL_FLIP_NONE);
		}

		bool checkDotInRect(Sint32 x, Sint32 y, SDL_Rect* re);

		typedef struct colorSurDot colorSurDot;
		struct colorSurDot
		{
			int x = 0;
			int y = 0;
			int col[4] = { 0 };
			float fx = 0.f;
			float fy = 0.f;
			float b = 0.f;
		};

		// 前置声明
		class QUI;
		class window;

		class rect : public BA_Base
		{
		public:
			SDL_Rect re = { 0 };
			SDL_Color col = { 0 };
			SDL_Surface* sur = nullptr;
			SDL_Texture* tex = nullptr;
			window* win = nullptr;

			int mouseHistory = 0;
			clock_t mouseHistoryTime = 0;

			rect() {}
			rect(SDL_Rect _re, SDL_Color _col)
			{
				re = _re;
				col = _col;
			}
			template<typename numTyPos, typename numTyCol>
			rect(numTyPos x, numTyPos y, numTyPos w, numTyPos h,
				numTyCol r, numTyCol g, numTyCol b, numTyCol a)
			{
				re.x = (int)x;
				re.y = (int)y;
				re.w = (int)w;
				re.h = (int)h;
				col.r = (int)r;
				col.g = (int)g;
				col.b = (int)b;
				col.a = (int)a;
			}
			//must use after ui is assigned
			void rendRect(void);
			bool checkMouseIn(Sint32 x, Sint32 y);
			~rect();
		};
		class colorSur : public rect
		{
		public:
			int sumdot = 0;
			tensor<colorSurDot>* dots = NULL;
			tensor<float>* len = NULL;
			tensor<float>* lv = NULL;
			bool** mask = 0;
			int col[4] = { 0 };
			SDL_Rect re_paint = { 0, 0, 1, 1 };

			colorSur(window* _win, SDL_Surface* _distSur, SDL_Rect pos = { 0,0,0,0 },
				bool alloc0Mask = false, int _sumdot = 4);
			colorSur* cacu(void);
			colorSur* update(void);
			SDL_Texture* getTex(void);
			~colorSur();
		};
		class colorText : public colorSur
		{
		public:
			char* pc = nullptr;
			TTF_Font* font = nullptr;
			SDL_Renderer* rend = nullptr;

			colorText(window* _win, const char* pc);
		};
		class label : public rect
		{
		public:
			std::string text;
			// TODO : can not rend a white font ???
			label(window* _win, const char* pc, int charSize, SDL_Color charCol = { 0,0,0,255 },
				SDL_Rect pos = {  }, SDL_Color bgc = { });
		};
		class button : public label
		{
		public:
			colorSur* cs = NULL;
			button(window* _win, const char* pc, int charSize, SDL_Color charCol = { 0,0,0,255 },
				SDL_Rect pos = {  }, SDL_Color bgc = {  });
			// _showWords 会mstrdup
			bool changeButtShowWords(const char* _showWords,
				int charSize, SDL_Color* cc = NULL, SDL_Color* bgc = NULL,
				const char* fontName = NULL);
		};

		template<typename baseItemTy>
		class namedItems : public BA_Base
		{

		};
		class buttons : public BA_Base
		{
		public:
			window* win = NULL;
			std::map<std::string, int> events;//1 left ; 2 right
			std::map<std::string, int> statue;//按钮列表占用 0不存在   1存在且显示   2存在不显示
			std::map<std::string, button*> butts;//按钮列表
			std::map<std::string, int (*)(void* pData, ...)> eveFunc;// int (*)(void* pData);
			std::map<std::string, void*> eveFuncData;// void*

			buttons(window* _win) {	win = _win;}
			~buttons();
			// name, _showWords 会mstrdup, 其余实参指针直接利用，外部代码申请内存时需要使用QUI的mem
			// bg == (SDL_Surface*)(0x1)), Use MyUI_ColorSur
			bool add(const char* _name, const char* _showWords, int charSize,
				SDL_Color charCol = {}, SDL_Color bgc = {.r = 255, .g = 255, .b = 255, .a = 255},
				SDL_Rect pos = {}, const char* align = "tl", SDL_Surface * bg = NULL,
				int (*eveFunc)(void* pData, ...) = NULL, void* eveFuncData = NULL);
			bool del(const char* _name);
		};

		class event : public BA_Base
		{
		public:

		};

		class windowState : public BA_Base
		{
		public:
			//protect
			SDL_mutex* _locker = SDL_CreateMutex();
			SDL_Event* _eve = BALLOC_R(1, SDL_Event, mem);
			void _setMouseEve(Sint32 mx, Sint32 my, Sint32 emx, Sint32 emy, Sint32 dx, Sint32 dy, int code);
			//private
			Sint32 winW = 0, winH = 0;
			bool isQuit = false;
			char* dropFilePath = nullptr;
			char* dropText = nullptr;
			Sint32 mousePos[2] = { 0 };// 按下鼠标时光标位置			
			Sint32 mouseEndPos[2] = { 0 };// 事件进行时实时光标位置
			Sint32 dMouseMove[2] = { 0 };// 鼠标位移
			int mouseEveCode = 0;// 鼠标事件代码：0=None；-1=Push；1=Drag；2=LEFT；3=RIGHT
			std::deque < Sint32> wheelY;//鼠标滚轮
			Uint32 timestamp = 0;//timestamp of the event
			std::deque<std::pair<SDL_Keycode, clock_t>> keys;// 键盘事件缓存队列，每个事件附带时间戳
			//public
			windowState() { };

			inline void pollEvent(void)
			{
				_mutexSafeWrapper([&]() {SDL_PollEvent(_eve); });
			}
			// if tmp is not nullptr, free will be called in-func
			inline SDL_Event* getUpdatedEveCopy(SDL_Event* tmp = NULL)
			{// TODO : 如果有要处理的事件，则返回1，否则返回0。
				if (tmp)
					free(tmp);
				tmp = MCALLOC(1, SDL_Event);
				if(tmp)
					*tmp = *(getVar(tmp, [&]() {SDL_PollEvent(_eve); return _eve; }));
				return tmp;
			}
			inline bool checkMouseIn(SDL_Rect* re)
			{
				return getVar(false, [&]() {
					return checkDotInRect(mouseEndPos[0], mouseEndPos[1], re) && checkDotInRect(mousePos[0], mousePos[1], re); });
			}
			inline int getMouseEveCode(SDL_Rect* re)
			{
				if (this->checkMouseIn(re))
					return getVar(0, [&]() {return mouseEveCode; });
				return 0;
			}
			void getMousePos(Sint32* x = NULL, Sint32* y = NULL, Sint32* orix = NULL, Sint32* oriy = NULL, Sint32* dx = NULL, Sint32* dy = NULL);
			std::pair<SDL_Keycode, Uint32> getKeyboardEve(void);

			template<typename funcTy>
			inline void _mutexSafeWrapper(funcTy func)
			{
				SDL_LockMutex(_locker);
				func();
				SDL_UnlockMutex(_locker);
			}
			template<typename funcTy, typename retTy>
			inline retTy getVar(retTy typeExampleValue, funcTy func)
			{
				SDL_LockMutex(_locker);
				retTy ret = func();
				SDL_UnlockMutex(_locker);
				return ret;
			}
		};
		/*windowState的线程，单独在windowState所属的windowState初始化时，合适的时候launch
		* mouse : 1:drag	 2:LEFT		 3:RIGHT
		* keyboard : 直接返回std::pair<SDL_Keycode, clock_t>
		*/
		int _windowState_checkAll(void* _s);

		/*
		* 以单独的事件监听线程监听并更新
		* TODO ：每个window有单独的线程以供渲染使用
		* _titlepc会内部复制
		*/
		class window : public rect
		{
		private:
		public:
			// GUI事件处理。除_windowState_checkAll，外部只可调用非_开头的方法
			windowState* winState = nullptr;
			QUI* ui = nullptr;
			TTF_Font* defaultFont = nullptr;

			char* titlepc = nullptr;
			Sint32 winPos[2] = { -1 };
			SDL_Window* pwin = nullptr;
			SDL_SysWMinfo info = SDL_SysWMinfo();
			HWND hwnd = HWND();
			SDL_Renderer* rend = nullptr;
			clock_t time = clock();
			float FPS = 0.f;
			const char* exitButtName = nullptr;
			label* title = nullptr;
			buttons* butts = new buttons(this);
			std::deque<int (*)(window* _win, void* pData)> checkEventFunc;
			std::deque<void*> checkEventFuncData;

			std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect*>*> otherTex;

			window(QUI* _ui, const char* _titlepc = "QUI", int winw = 800, int winh = 500,
				int winflags = 0, SDL_Color bgc = {});
			~window();

			QUI& addOtherTex(std::string name, SDL_Texture* tex, SDL_Rect* re);
			//colorSur::getTex has SDL_DestroyTexture builtin
			QUI& updateOtherTex(std::string name, SDL_Texture* tex, bool destroyOld = false);
			bool checkTitle(bool rendclear = true, bool copyTex = true);
			inline void addCheckEventFunc(int (*func)(window* _win, void* _pData), void* _pData)
			{
				checkEventFunc.emplace_back(func);
				checkEventFuncData.emplace_back(_pData);
			}
			bool checkEvent();
			bool update(bool rendclear = true, bool copyTex = true, bool limitFPS = true);
			bool pollQuit();
			bool delButt(const char* _name);
		};

		// TODO : support multi window
		class QUI : public BA_Base
		{
		public:
			window* activeWin = NULL;
			std::unordered_map<std::string, window*> windows;


			QUI(const char* titlepc = "QUI", int winw = 800, int winh = 500,
				int winflags = 0, SDL_Color bgc = {});

			QUI& addWindow(const char* titlepc = "QUI", int winw = 800, int winh = 500,
				int winflags = 0, SDL_Color bgc = {});
			bool delWindow(const char* titlepc);
			QUI& setActiveWindow(const char* title = NULL);
			inline window* getWindow(const char* win)
			{
				if (win && windows.find(win) != windows.end())
					return windows[win];
				return activeWin;
			}

			inline QUI& addOtherTex(std::string name, SDL_Texture* tex, SDL_Rect* re, const char* win = NULL)
			{
				return getWindow(win)->addOtherTex(name, tex, re);
			}
			inline QUI& updateOtherTex(std::string name, SDL_Texture* tex, const char* win = NULL)
			{
				return getWindow(win)->updateOtherTex(name, tex);
			}
			inline bool addButt(const char* _name, const char* _showWords, int charSize, SDL_Rect pos = {},
				SDL_Color charCol = {}, SDL_Color bgc = { .r = 255, .g = 255, .b = 255, .a = 255 },
				const char* align = "tl", SDL_Surface* bg = NULL,
				const char* win = NULL, int (*eveFunc)(void* pData, ...) = NULL, void* eveFuncData = NULL)
			{
				return getWindow(win)->butts->add(_name, _showWords, charSize, charCol,bgc, pos, align, bg, eveFunc, eveFuncData);
			}
			inline bool delButt(const char* _name, const char* win = NULL)
			{
				return getWindow(win)->delButt(_name);
			}
			inline bool checkTitle(bool rendclear = true, bool copyTex = true, const char* win = NULL)
			{
				return getWindow(win)->checkTitle(rendclear, copyTex);
			}
			inline bool checkEvent(const char* win = NULL)
			{
				return getWindow(win)->checkEvent();
			}
			inline bool update(const char* win = NULL, bool rendclear = true, bool copyTex = true, bool limitFPS = true)
			{
				return getWindow(win)->update(rendclear, copyTex, limitFPS);
			}
			inline bool pollQuit(const char* win = NULL)
			{
				return getWindow(win)->pollQuit();
			}

			int Quit(int code, ...);
			friend int QUI_Quit(void* pui_, int code, ...);
		};


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
		};
		// 列表视图，baseItemTy为ba::ui::rect子类指针
		// item需要new来申请，并在listView析构时于其析构函数调用baseItemTy的析构函数
		// TODO : 支持不同高度，支持多列表同步滚动
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
		int _listView_check(window* _win, void* _pData);
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
			data.visibleRange[0] = data.visibleRange[1] = 0;
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


#endif //BA_BA_GUI_H