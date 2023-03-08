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
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_CreateRGBASurface(w,h) SDL_CreateRGBSurface(0,w,h,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF)
#else
#define SDL_CreateRGBASurface(w,h) SDL_CreateRGBSurface(0,w,h,32,0x000000FF,0x0000FF00,0x00FF0000,0xFF000000)
#endif
		SDL_Texture* createRGBTexture(SDL_Renderer* rend, int w, int h, int r, int g, int b);
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
			virtual SDL_Texture* getTex();
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
		// name, _showWords 会mstrdup, 其余实参指针直接利用，外部代码申请内存时需要使用QUI的mem
		// bg == (SDL_Surface*)(0x1)), Use MyUI_ColorSur
		class button : public label
		{
		public:
			colorSur* cs = NULL;
			button(window* _win, const char* _name, const char* _showWords, int charSize,
				SDL_Color charCol, SDL_Color bgc, SDL_Rect pos, const char* align, SDL_Surface* bg);
			// _showWords 会mstrdup
			bool changeButtShowWords(const char* _showWords,
				int charSize, SDL_Color* cc = NULL, SDL_Color* bgc = NULL,
				const char* fontName = NULL);
			SDL_Texture* getTex();
		};

		// baseItemTy is a ptr based on ba::ui::rect
		// eveFunc return a int which will be set as mouseEveCode
		template<typename baseItemTy>
		class namedItems : public rect
		{
		public:
			window* win = NULL;
			std::map<std::string, int> events;// 鼠标事件代码：0=None；-1=Push；1=Drag；2=LEFT；3=RIGHT; 4=wheel
			std::map<std::string, int> statue;//占用 0不存在   1存在且显示   2存在不显示
			std::map<std::string, baseItemTy> items;//列表
			std::map<std::string, int (*)(window* _win, void* _self, int mouseEveCode, void* pData)> eveFunc;// int (*)(void* pData);
			std::map<std::string, void*> eveFuncSelfData;// void*
			std::map<std::string, void*> eveFuncData;// void*

			namedItems() {};
			namedItems(window* _win) { win = _win; };
			~namedItems();
			bool add(const char* _name, baseItemTy item,
				int (*_eveFunc)(window* _win, void* _self, int mouseEveCode, void* pData) = NULL, void* _self = NULL, void* _eveFuncData = NULL);
			bool del(const char* _name);
			void check(void);
			// will reset events to 0 if it has a event
			int getMouseEveCode(const char* _name);
			void update(void);
			bool update(const char* _name, baseItemTy newItem, bool delOldOne = false);
		};
		template<typename baseItemTy>
		inline namedItems<baseItemTy>::~namedItems()
		{
				for (auto p : items)
					delete p.second;
		}
		template<typename baseItemTy>
		inline bool namedItems<baseItemTy>::add(const char* _name, baseItemTy item,
			int(*_eveFunc)(window* _win, void* _self, int mouseEveCode, void* pData), void* _self, void* _eveFuncData)
		{
			char* name = mstrdup(_name, mem);
			items[name] = item;
			statue[name] = 1;
			events[name] = false;
			if (_eveFunc)
			{
				eveFunc[name] = _eveFunc;
				eveFuncSelfData[name] = _self;
				eveFuncData[name] = _eveFuncData;
			}
			return true;
		}
		template<typename baseItemTy>
		inline bool namedItems<baseItemTy>::del(const char* _name)
		{
			if (items.contains(_name))
			{
				baseItemTy item = items[_name];
				events.erase(_name);
				statue.erase(_name);
				items.erase(_name);
				if (eveFunc.contains(_name))
				{
					eveFunc.erase(_name);
					eveFuncData.erase(_name);
				}
				delete item;
				return true;
			}
			return false;
		}
		template<typename baseItemTy>
		inline void namedItems<baseItemTy>::check(void)
		{
			int mouseEveCode = 0;
			for (auto p = items.begin(); p != items.end(); p++)
			{
				mouseEveCode = 0;
				if (statue[p->first])
				{
					events[p->first] = win->winState->getMouseEveCode(&(p->second->re));
					if (events[p->first] != 0)
					{
						if(eveFunc[p->first])//如果槽函数执行过程中发生了事件更新，那么事件代码不再接受槽函数返回值的赋值
							mouseEveCode = eveFunc[p->first](win, eveFuncSelfData[p->first], events[p->first], eveFuncData[p->first]);
						win->winState->_mutexSafeWrapper([&]() {
							win->winState->mouseEveCode = win->winState->mouseEveCode == events[p->first] ? mouseEveCode : win->winState->mouseEveCode;});
					}
				}
			}
		}
		template<typename baseItemTy>
		inline int namedItems<baseItemTy>::getMouseEveCode(const char* _name)
		{
			int ret = 0;
			if (statue.find(_name) != statue.end() && statue[_name])
			{
				ret = events[_name];
				events[_name] = 0;
			}
			return ret;
		}
		template<typename baseItemTy>
		inline void namedItems<baseItemTy>::update(void)
		{
			for (auto p = items.begin(); p != items.end(); p++)
			{
				if (statue[p->first])
					SDL_RenderCopy(win->rend, items[p->first]->getTex(), NULL, &(items[p->first]->re));
			}
		}
		template<typename baseItemTy>
		inline bool namedItems<baseItemTy>::update(const char* _name, baseItemTy newItem, bool delOldOne)
		{
			return false;
		}

		class event : public BA_Base
		{
		public:

		};

		class windowState : public BA_Base
		{
		public:
			//protect
			SDL_mutex* _locker = nullptr;
			SDL_Event* _eve = BALLOC_R(1, SDL_Event, mem);
			//private
			Sint32 winW = 0, winH = 0;
			bool isQuit = false;
			char* dropFilePath = nullptr;
			char* dropText = nullptr;
			Sint32 mousePos[2] = { 0 };// 按下鼠标时光标位置			
			Sint32 mouseEndPos[2] = { 0 };// 事件进行时实时光标位置
			Sint32 dMouseMove[2] = { 0 };// 鼠标位移
			int mouseEveCode = 0;// 鼠标事件代码：0=None；-1=Push；1=Drag；2=LEFT；3=RIGHT; 4=wheel
			std::list < std::pair<Sint32, Uint32>> wheelY;//鼠标滚轮（滑动值，时间戳）
			Uint32 timestamp = 0;//timestamp of the event
			std::deque<std::pair<SDL_Keycode, clock_t>> keys;// 键盘事件缓存队列，每个事件附带时间戳
			//public
			windowState() {};
			windowState(SDL_mutex* locker) { _locker = locker; };

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

			namedItems<button*> butts = namedItems<button*>(this);
			namedItems<rect*> rects = namedItems<rect*>(this);
			// will be abandoned
			std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect*>*> otherTex;
			std::deque<int (*)(window* _win, void* pData)> checkEventFunc;
			std::deque<void*> checkEventFuncData;


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
		class winCreatePara
		{
		public:
			const char* titlepc;
			int winw;
			int winh;
			int winflags;
			SDL_Color bgc;
			winCreatePara(const char* _titlepc, int _winw, int _winh, int _winflags, SDL_Color _bgc)
			{
				titlepc = _titlepc;
				winw = _winw;
				winh = _winh;
				winflags = _winflags;
				bgc = _bgc;
			}
		};
		class QUIEventThread : public windowState
		{
		public:
			SDL_Window* winTmp = NULL;
			std::mutex* condMutex = new std::mutex();
			balist<winCreatePara>* winNeedSig = new balist<winCreatePara>();
			balist<SDL_Window>* winPipline = new balist<SDL_Window>();
			std::unordered_map<_LL, window*> winId2Ptr;
			QUIEventThread(SDL_mutex* locker = SDL_CreateMutex()) { _locker = locker; };

			void _setMouseEve(Sint32 mx, Sint32 my, Sint32 emx, Sint32 emy, Sint32 dx, Sint32 dy, int code);
		};
		/*QUIEventThread的线程
		* mouse : 0=None；-1=Push；1=Drag；2=LEFT；3=RIGHT; 4=wheel
		* keyboard : 直接返回std::pair<SDL_Keycode, clock_t>
		*/
		int _QUIEvent_checkAll(void* _s);
		// TODO : 一个QUI拥有多个窗口，但构造QUI时在一个子线程中SDL_Init以及轮询事件，主线程可创建并管理多个窗口
		class QUI : public BA_Base
		{
		public:
			int sysSumRAM = SDL_GetSystemRAM();
			int sysSumCPU = SDL_GetCPUCount();
			QUIEventThread* eveThread;
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
				const char* win = NULL, int (*eveFunc)(window* _win, void* _self, int mouseEveCode, void* pData) = NULL, void* eveFuncData = NULL)
			{
				return getWindow(win)->butts.add(_name, new button(getWindow(win), _name, _showWords, charSize, charCol, bgc, pos, align, bg),
					eveFunc, eveFuncData);
			}
			inline bool delButt(const char* _name, const char* win = NULL)
			{
				return getWindow(win)->delButt(_name);
			}
			inline bool addRect(const char* name, rect* re, int (*eveFunc)(window* _win, void* _self, int mouseEveCode, void* pData) = NULL, void* eveFuncSelfData = NULL, void* eveFuncData = NULL, const char* win = NULL)
			{
				return getWindow(win)->rects.add(name, re, eveFunc, eveFuncSelfData, eveFuncData);
			}
			inline bool delRect(const char* _name, const char* win = NULL)
			{
				return getWindow(win)->rects.del(_name);
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
}
}


#endif //BA_BA_GUI_H