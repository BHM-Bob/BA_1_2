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

		class rect : public BA_Base
		{
		public:
			SDL_Rect re = { 0 };
			SDL_Color col = { 0 };
			SDL_Surface* sur = nullptr;
			SDL_Texture* tex = nullptr;
			QUI* ui = nullptr;

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
			bool checkPressOn(SDL_Event* peve);
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

			colorSur(QUI* _ui, SDL_Surface* _distSur, SDL_Rect pos = { 0,0,0,0 }, int _sumdot = 4);
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

			colorText(QUI* _ui, const char* pc);
		};
		class label : public rect
		{
		public:
			std::string text;

			label(QUI* _ui, const char* pc, int charSize, SDL_Color charCol = { 0,0,0,255 },
				SDL_Rect pos = { 0,0,0,0 }, SDL_Color bgc = { 255,255,255,0 });
		};
		class button : public label
		{
		public:
			button(QUI* _ui, const char* pc, int charSize, SDL_Color charCol = { 0,0,0,0 },
				SDL_Rect pos = { 0,0,0,0 }, SDL_Color bgc = { 255,255,255,0 });
		};

		class buttons
		{
		public:
			std::map<const char*, int> events;//1 left ; 2 right
			std::map<const char*, int> statue;//按钮列表占用 0不存在   1存在且显示   2存在不显示
			std::map<const char*, button*> butts;//按钮列表
			badict eveFunc = badict(true);// int (*)(void* pData);
			badict eveFuncData = badict(true);// void*
		};

		class window : public rect
		{
		public:
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

			window(QUI* _ui, const char* title, SDL_Rect _re, SDL_Color _col);
		};

		// TODO : support multi window
		class QUI : public BA_Base
		{
		public:
			SDL_Renderer* rend = nullptr;
			TTF_Font* defaultFont = nullptr;
			window* win = nullptr;
			buttons* butts = new buttons();

			std::map<const char*, std::pair<SDL_Texture*, SDL_Rect*>*> otherTex;


			QUI(const char* titlepc = "QUI", int winw = 800, int winh = 500,
				int winflags = 0, SDL_Color* bgc = NULL);

			// name, _showWords 会mstrdup, 其余实参指针直接利用，外部代码申请内存时需要使用QUI的mem
			//bg == (SDL_Surface*)(0x1)), Use MyUI_ColorSur
			bool addButt(const char* _name, const char* _showWords, int charSize,
				SDL_Color* charCol, SDL_Color* bgc, SDL_Rect* pos, SDL_Surface* bg,
				int (*eveFunc)(void* pData, ...) = NULL, void* eveFuncData = NULL);
			// _showWords 会mstrdup
			bool changeButtShowWords(const char* _name, const char* _showWords,
				int charSize, SDL_Color* cc = NULL, SDL_Color* bgc = NULL,
				const char* fontName = NULL);
			bool delButt(const char* _name);
			bool checkButt();
			bool checkTitle(bool rendclear = true, bool copyTex = true);
			bool update(bool rendclear = true, bool copyTex = true);
			bool pollQuit();
			int Quit(int code, ...);
			friend int QUI_Quit(void* pui_, int code, ...);
		};
	}
}


#endif //BA_BA_GUI_H