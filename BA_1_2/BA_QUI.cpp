//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2022年7月30日

#include"BA_Base.hpp"
#include"BA_UI.hpp"
#include"BA_QUI.hpp"

SDL_Color* SetSDLCol(SDL_Color* col, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (col)
	{
		col->r = r;
		col->g = g;
		col->b = b;
		col->a = a;
	}
	return col;
}

SDL_Color* MakeSDLCol(List* mem, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color* col = NULL;
	if (mem)
	{
		col = BALLOC_R(1, SDL_Color, mem);
	}
	else
	{
		col = BALLOC_L(1, SDL_Color);
	}
	return SetSDLCol(col, r, g, b, a);
}

SDL_Rect* SetSDLRect(SDL_Rect* pos, int w, int h, int x, int y)
{
	if (pos)
	{
		pos->w = w;
		pos->h = h;
		pos->x = x;
		pos->y = y;
	}
	return pos;
}

SDL_Rect* MakeSDLRect(List* mem, int w, int h, int x, int y)
{
	SDL_Rect* pos = NULL;
	if (mem)
	{
		pos = BALLOC_R(1, SDL_Rect, mem);
	}
	else
	{
		pos = BALLOC_L(1, SDL_Rect);
	}
	return SetSDLRect(pos, w, h, x, y);
}

int QUI_Quit(void* pui_, int code, ...)
{
	QUI* pui = (QUI*)pui_;
	MyBA_Free_R(pui->mem);
	return 0;
}

QUI::QUI(const char* titlepc, int winw, int winh, int winflags, SDL_Color* bgc)
{
	mem = List_Init();

	win = BALLOC_R(1, QUI_win, mem);
	win->titlepc = _strdup(titlepc);
	mem->Put(mem, (void*)(win->titlepc));
	win->peve = BALLOC_R(1, SDL_Event, mem);
	win->winw = winw;
	win->winh = winh;
	win->pre_win = BALLOC_R(1, SDL_Rect, mem);
	win->pre_win->w = winw;
	win->pre_win->h = winh;
	win->pre_win->x = win->pre_win->y = 0;
	win->FPS = 25.f;
	fonts = BALLOC_R(1, QUI_fonts, mem);
	butts = BALLOC_R(1, QUI_butts, mem);
	fonts->fonts = new list< TTF_Font>;
	butts->butts = dict(true);
	butts->events = dict(true);
	butts->statue = dict(true);
	butts->names = list<char>();

	otherTex = new list< SDL_Texture>;
	otherTexRe = new list< SDL_Rect>;

	int img_f = IMG_INIT_JPG;// | IMG_INIT_PNG;
	if ((SDL_Init(SDL_INIT_EVERYTHING) == -1) || (TTF_Init() == -1) || (IMG_Init(img_f) != (img_f)))/*|| Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID)==0)*/
	{
		MyBA_Err("MyUI_Init: Can't Init SDL2", 1);
	}
	else
	{
		if (winflags == 0)
			winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;
		else if (winflags == 1)
			winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
		win->pwin = SDL_CreateWindow(win->titlepc, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win->winw, win->winh, winflags);
		win->rend = SDL_CreateRenderer(win->pwin, -1, SDL_RENDERER_ACCELERATED);
		SDL_RenderClear(win->rend);
		SDL_SetRenderDrawColor(win->rend, 200, 200, 200, 200);

		fonts->pdefaultfont = TTF_OpenFont("C:\\Windows\\Fonts\\simkai.ttf", 128);

		if (bgc != NULL)
		{
			win->bgc = bgc;
			win->pwinSur = SDL_GetWindowSurface(win->pwin);
			win->format = win->pwinSur->format;
			SDL_FillRect(win->pwinSur, NULL, SDL_MapRGB(win->format, win->bgc->r, win->bgc->g, win->bgc->b));

		}
		else
		{
			win->bgc = MakeSDLCol(mem, 0, 0, 0, 0);
			SDL_VERSION(&(win->info.version));
			if (SDL_GetWindowWMInfo(win->pwin, &(win->info)))
				win->hwnd = win->info.info.win.window;
			/*设置窗口colorkey*/
			SetWindowLongW(win->hwnd, GWL_EXSTYLE, GetWindowLong(win->hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			SetLayeredWindowAttributes(win->hwnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
			/*设置窗口为悬浮窗 */
			SetWindowPos(win->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			win->pwinSur = SDL_GetWindowSurface(win->pwin);
			UINT32 keyColor = SDL_MapRGB(win->pwinSur->format, 255, 255, 255);
			SDL_SetSurfaceBlendMode(win->pwinSur, SDL_BLENDMODE_NONE);
			SDL_FillRect(win->pwinSur, NULL, keyColor);
		}

		win->pwinTex = SDL_CreateTextureFromSurface(win->rend, win->pwinSur);
		SDL_RenderCopy(win->rend, win->pwinTex, NULL, NULL);
		SDL_RenderPresent(win->rend);
		win->time = clock();

		List_Put(pba->exitFunc, (void*)QUI_Quit);
		List_Put(pba->exitFuncData, (void*)this);
	}

}

QUI::QUI()
{
	mem = List_Init();
	SDL_Color* bgc = MakeSDLCol(mem, 255, 255, 255, 255);
	*this = QUI("QUI", 800, 800, 0, bgc);
}

QUI::~QUI()
{
}

bool QUI::AddFont(const char* ppath, const char* name)
{
	if (ppath && name)
	{
		TTF_Font* pFont = TTF_OpenFont(ppath, 128);
		if (pFont)
		{
			fonts->fonts->Put(pFont, name);
			return true;
		}
	}
	return false;
}

//name, showWords 会内部mstrdup, 其余实参指针直接利用，外部代码申请内存时需要使用QUI的mem
bool QUI::AddButt(const char* _name, const char* _showWords, int charSize, SDL_Color* charCol, SDL_Color* bgc, SDL_Rect* pos, SDL_Surface* bg)
{
	char* name = mstrdup(_name, mem);
	if (!_showWords)
		_showWords = _name;
	char* showWords = mstrdup(_showWords, mem);
	bool iscolor = 0;
	if (!pos)
		pos = MakeSDLRect(mem, win->pre_win->w, win->pre_win->h, 0, 0);
	if (bg == (SDL_Surface*)(0x1))//Use MyUI_ColorSur
	{
		iscolor = 1;
		if (pos->w < 0 || pos->h < 0)//自行根据字符串大小计算按钮大小
		{
			if (charSize == 0 || strlen(showWords) == 0)
				return 0;
			if (pos->w < 0)
				pos->w = strlen(showWords) * charSize;
			if (pos->h < 0)
				pos->h = charSize;
		}
		bg = SDL_CreateRGBSurface(0, pos->w, pos->h, 32, 0, 0, 0, 255);
	}

	if (!charCol)
		charCol = MakeSDLCol(mem, 0, 0, 0, 255);
	if (!bgc)
		bgc = MakeSDLCol(mem, 255, 255, 255, 255);
	int* _bgc = TypeDupR<int>(mem, 3, bgc->r, bgc->g, bgc->b);
	int* _charCol = TypeDupR< int>(mem, 3, charCol->r, charCol->g, charCol->b);

	SDL_MyButton* pButt = SDL_Create_MyButton(win->rend, 0, *pos, _bgc, showWords, fonts->pdefaultfont, charSize, _charCol, bg);
	// just use char ptr
	butts->butts[name] = pButt;
	butts->statue[name] = 1;
	butts->events[name] = 0;
	butts->names.Put(name, name, true);
	if (iscolor)
		pButt->pct = MyUI_ColorSur_Init(pButt->back1);
	SDL_RenderPresent(win->rend);
	return true;
}

bool QUI::DelButt(const char* _name)
{
	butts->butts.Del(_name);
	butts->events.Del(_name);
	butts->statue.Del(_name);
	butts->names.Get(_name);
	SDL_MyButton* pButt = butts->butts.Copy<SDL_MyButton*>(_name);
	return SDL_Destroy_MyButton(pButt);
}

bool QUI::CheckButt()
{
	SDL_PollEvent(win->peve);
	clock_t st = clock();
	if (win->peve->type == SDL_MOUSEBUTTONDOWN)
	{
		for (SDL_PollEvent(win->peve); win->peve->type != SDL_MOUSEBUTTONUP;
			SDL_PollEvent(win->peve), SDL_Delay(1))
		{
			if ((float)(clock() - st) / CLOCKS_PER_SEC > 0.2)
				return this->CheckTitle();
		}
		Sint32 mx = win->peve->motion.x;
		Sint32 my = win->peve->motion.y;
		int x, y, w, h;
		for (dictPair* dp = butts->butts.pfirst; dp; dp = dp->pnext)
		{
			if (butts->statue.Copy<int>(dp->key) == 1)
			{
				butts->statue[dp->key] = 0;
				SDL_Rect re = butts->butts.Copy<SDL_MyButton*>(dp->key)->re_butt;
				x = re.x;
				y = re.y;
				w = re.w;
				h = re.h;
				if (mx > x && mx<x + w && my>y && my < y + h)
				{
					if (win->peve->button.button == SDL_BUTTON_LEFT)
						butts->events[dp->key] = 1;
					else if (win->peve->button.button == SDL_BUTTON_RIGHT)
						butts->events[dp->key] = 2;
				}
			}
		}
	}
	return true;
}

bool QUI::CheckTitle()
{
	SDL_Rect* pre = win->pre_title;
	Sint32 wx = 0, wy = 0, bx = 0, by = 0;
	if ((pre != NULL) && (SDL_CheckPressOn_MyButton(pre, win->peve)))
	{
		SDL_GetWindowPosition(win->pwin, &wx, &wy);
		bx = win->peve->button.x;
		by = win->peve->button.y;
		for (Sint32 x, y = SDL_PollEvent(win->peve); win->peve->type != SDL_MOUSEBUTTONUP; )
		{
			SDL_PollEvent(win->peve);
			x = win->peve->button.x;
			y = win->peve->button.y;
			if (win->peve->type == SDL_MOUSEMOTION && x != 0 && y != 0)
			{
				SDL_SetWindowPosition(win->pwin, wx + x - bx, wy + y - by);
				wx = wx + x - bx;
				wy = wy + y - by;
			}
		}
	}
	return true;
}

bool QUI::Update(bool rendclear, bool copyTex)
{
	for (float waitt = 1. / win->FPS;
		(float)((float)clock() - win->time) / CLOCKS_PER_SEC < waitt; SDL_Delay(2));
	win->time = clock();
	if (rendclear)
		SDL_RenderClear(win->rend);
	if (copyTex)
		SDL_RenderCopy(win->rend, win->pwinTex, NULL, NULL);
	SDL_MyButton* pButt = NULL;
	for (dictPair* dp = butts->butts.pfirst; dp; dp = dp->pnext)
	{
		if (butts->statue.Copy<int>(dp->key) == 1)
		{
			pButt = butts->butts.Copy<SDL_MyButton*>(dp->key);
			if (pButt->pct != NULL)
			{
				MyUI_ColorSur_Update(pButt->pct);
				SDL_DestroyTexture(*(pButt->ppTex));
				*(pButt->ppTex) = SDL_Get_Font_Texture(win->rend,
					pButt->back1, pButt->pfont,
					pButt->charcolor, pButt->charsize, pButt->pc, 0, 0);
			}
			SDL_RenderCopy(win->rend, *(pButt->ppTex),
				NULL, &(pButt->re_butt));
		}
	}
	for (int a = 0; a < otherTex->sumque; a++)
		SDL_RenderCopy(win->rend, otherTex->Copy(a),
			NULL, otherTexRe->Copy(a));
	SDL_RenderPresent(win->rend);
	return true;
}

bool QUI::PollQuit()
{
	this->CheckButt();
	if ((win->exitButtName) && butts->events.Copy<int>(win->exitButtName) == 1)
		return 1;
	return SDL_Poll_Quit(win->peve);
}
