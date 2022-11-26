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

namespace ba
{
	namespace ui
	{
		int* ProduceRainbowCol(int* col, float* i, float* di);
		int* ProduceRainbowCol(int* col, float* i, float di = 0.05f);
		SDL_Color* SetSDLCol(SDL_Color* col, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		SDL_Color* MakeSDLCol(List* mem, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
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
			SDL_Event* _checkEveAvaliable(SDL_Event* peve);
			int _setMouseHistory(int code);
			bool _checkMouseIn(bool updateEve = false, SDL_Event* peve = NULL);
			int checkMouse(bool updateEve = false, SDL_Event* peve = NULL);
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
			void destroy(void);
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

		class buttons : public BA_Base
		{
		public:
			window* win = NULL;
			std::map<std::string, int> events;//1 left ; 2 right
			std::map<std::string, int> statue;//按钮列表占用 0不存在   1存在且显示   2存在不显示
			std::map<std::string, button*> butts;//按钮列表
			std::map<std::string, int (*)(void* pData, ...)> eveFunc;// int (*)(void* pData);
			std::map<std::string, void*> eveFuncData;// void*

			buttons(window* _win);
			// name, _showWords 会mstrdup, 其余实参指针直接利用，外部代码申请内存时需要使用QUI的mem
			// bg == (SDL_Surface*)(0x1)), Use MyUI_ColorSur
			bool add(const char* _name, const char* _showWords, int charSize,
				SDL_Color charCol = {}, SDL_Color bgc = {.r = 255, .g = 255, .b = 255, .a = 255},
				SDL_Rect pos = {}, const char* align = "tl", SDL_Surface * bg = NULL,
				int (*eveFunc)(void* pData, ...) = NULL, void* eveFuncData = NULL);
		};

		class event : public BA_Base
		{
		public:


		};

		class window : public rect
		{
		private:
			void _maintainGUI(balist<ba::ui::event> inEve, balist <ba::ui::event> outEve);
		public:
			std::mutex locker;
			QUI* ui = NULL;
			TTF_Font* defaultFont = nullptr;

			char* titlepc = nullptr;
			SDL_Window* pwin = nullptr;
			SDL_SysWMinfo info = SDL_SysWMinfo();
			HWND hwnd = HWND();
			SDL_Renderer* rend = nullptr;
			SDL_PixelFormat* format = nullptr;
			SDL_Event* peve = nullptr;
			clock_t time = clock();
			float FPS = 0.f;
			const char* exitButtName = nullptr;
			label* title = nullptr;
			buttons* butts = new buttons(this);

			std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect*>*> otherTex;

			window(QUI* _ui, const char* _titlepc = "QUI", int winw = 800, int winh = 500,
				int winflags = 0, SDL_Color* bgc = NULL);

			QUI& addOtherTex(std::string name, SDL_Texture* tex, SDL_Rect* re);
			QUI& updateOtherTex(std::string name, SDL_Texture* tex);
			bool checkButt();
			bool checkTitle(bool rendclear = true, bool copyTex = true);
			bool update(bool rendclear = true, bool copyTex = true);
			bool pollQuit();
			bool delButt(const char* _name);
		};

		// TODO : support multi window
		class QUI : public BA_Base
		{

		public:
			_LL activeWindow = -1;
			window* activeWin = NULL;
			std::unordered_map<std::string, window*> windows;


			QUI(const char* titlepc = "QUI", int winw = 800, int winh = 500,
				int winflags = 0, SDL_Color* bgc = NULL);

			QUI& addWindow(const char* titlepc = "QUI", int winw = 800, int winh = 500,
				int winflags = 0, SDL_Color* bgc = NULL);
			QUI& setActiveWindow(const char* title = NULL, _LL idx = -1);

			QUI& addOtherTex(std::string name, SDL_Texture* tex, SDL_Rect* re,
				const char* win = NULL);
			QUI& updateOtherTex(std::string name, SDL_Texture* tex,
				const char* win = NULL);
			bool checkButt(const char* win = NULL);
			bool checkTitle(bool rendclear = true, bool copyTex = true,
				const char* win = NULL);
			bool update(const char* win = NULL, bool rendclear = true, bool copyTex = true);
			bool pollQuit(const char* win = NULL);
			bool delButt(const char* _name, const char* win = NULL);

			int Quit(int code, ...);
			friend int QUI_Quit(void* pui_, int code, ...);
		};
	}
}


#endif //BA_BA_GUI_H