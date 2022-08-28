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
	win->titlepc = mstrdup(titlepc, mem);
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
	fonts->fonts = new balist< TTF_Font>;
	butts->butts = balist<SDL_MyButton>();
	butts->names = balist<char>();
	butts->events = balist<int>();
	butts->statue = balist<int>();
	butts->eveFunc = badict(true);
	butts->eveFuncData = badict(true);
	keys = new QUI_Keys();

	otherTex = new balist< SDL_Texture>;
	otherTexRe = new balist< SDL_Rect>;

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

		MyBA_AtQuit(QUI_Quit, (void*)this);
	}

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
//bg == (SDL_Surface*)(0x1)), Use MyUI_ColorSur
bool QUI::AddButt(const char* _name, const char* _showWords, int charSize,
	SDL_Color* charCol, SDL_Color* bgc, SDL_Rect* pos, SDL_Surface* bg,
	int (*eveFunc)(void* pData, ...), void* eveFuncData)
{
	char* name = mstrdup(_name, mem);
	if (!_showWords)
		_showWords = _name;
	char* showWords = _strdup(_showWords);
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
	butts->butts(name, pButt, false, true);
	butts->names(name, name, false, true);
	butts->statue(name, TypeDupR(mem, 1, 1), false, true);
	butts->events(name, TypeDupR(mem, 1, 0), false, true);
	if (eveFunc)
	{
		butts->eveFunc[name] = eveFunc;
		butts->eveFuncData[name] = eveFuncData;
	}
	if (iscolor)
		pButt->pct = MyUI_ColorSur_Init(pButt->back1);
	SDL_RenderPresent(win->rend);
	return true;
}

bool QUI::ChangeButtShowWords(const char* _name, const char* _showWords,
	int charSize, SDL_Color* cc, SDL_Color* bgc, const char* fontName)
{
	SDL_MyButton* pbutt = butts->butts.Copy(_name, true);
	free(pbutt->pc);
	pbutt->pc = _strdup(_showWords);
	if (fontName)
		pbutt->pfont = fonts->fonts->Copy(fontName, true);
	pbutt->charsize = charSize;
	int* charcolor = cc ? (int*)TypeDupR(mem, 3, cc->r, cc->g, cc->b) : pbutt->charcolor;
	SDL_Surface* pte = NULL;
	SDL_DestroyTexture(*(pbutt->ppTex));
	if (pbutt->back1 != NULL)
	{
		pte = pbutt->back1;
	}
	else if (bgc != NULL)
	{
		pte = SDL_CreateRGBSurface(0, pbutt->re_butt.w, pbutt->re_butt.h, 32, 0, 0, 0, 0);
		SDL_FillRect(pte, NULL, SDL_MapRGB(pte->format, bgc->r, bgc->g, bgc->b));
	}
	else
	{// null bg, only rewrite pc, get new tex
		SDL_Color color = { charcolor[0],charcolor[1],charcolor[2] };
		SDL_Surface* pte = TTF_RenderUTF8_Blended(pbutt->pfont, pbutt->pc, color);
		*(pbutt->ppTex) = SDL_CreateTextureFromSurface(win->rend, pte);
		SDL_RenderCopy(win->rend, *(pbutt->ppTex), NULL, &(pbutt->re_butt));
		return pbutt;
	}
	*(pbutt->ppTex) = SDL_Get_Font_Texture(win->rend, pte, pbutt->pfont, charcolor, charSize, pbutt->pc, 0, 0);
	SDL_RenderCopy(win->rend, *(pbutt->ppTex), NULL, &(pbutt->re_butt));
	return pbutt;
}

bool QUI::DelButt(const char* _name)
{
	MyBA_Free(butts->events.Get(_name, false, false), mem);
	MyBA_Free(butts->statue.Get(_name, false, false), mem);
	if (butts->eveFunc.HasKey(_name))
	{
		butts->eveFunc.Del(_name, false);
		butts->eveFuncData.Del(_name, false);
	}
	butts->names.Get(_name, false, false);
	SDL_MyButton* pButt = butts->butts.Get(_name, false, false);
	return SDL_Destroy_MyButton(pButt);
}

bool QUI::CheckButt()
{
	SDL_PollEvent(win->peve);
	clock_t st = clock();
	int (*eveFunc)(void* pData) = NULL;
	void* eveFuncData = NULL;
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
		for (balistDot<SDL_MyButton>* dp = butts->butts.pfirst; dp; dp = dp->pnext)
		{
			if (*(butts->statue.Copy(dp->name)) == 1)
			{
				*(butts->events.Copy(dp->name)) = 0;
				x = dp->pdata->re_butt.x;
				y = dp->pdata->re_butt.y;
				w = dp->pdata->re_butt.w;
				h = dp->pdata->re_butt.h;
				if (mx > x && mx<x + w && my>y && my < y + h)
				{
					if (win->peve->button.button == SDL_BUTTON_LEFT)
					{
						*(butts->events.Copy(dp->name)) = 1;
						if (butts->eveFunc.HasKey(dp->name))
						{
							eveFunc = butts->eveFunc.Copy<int (*)(void* pData)>(dp->name, true);
							eveFunc(butts->eveFuncData.Copy<void* > (dp->name, true));
						}
					}
					else if (win->peve->button.button == SDL_BUTTON_RIGHT)
					{
						*(butts->events.Copy(dp->name)) = 2;
					}
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
		(float)((float)clock() - win->time) / CLOCKS_PER_SEC < waitt; SDL_Delay(1));
	win->time = clock();
	keys->Update(win->peve);
	if (rendclear)
		SDL_RenderClear(win->rend);
	if (copyTex)
		SDL_RenderCopy(win->rend, win->pwinTex, NULL, NULL);
	SDL_MyButton* pButt = NULL;
	for (balistDot<SDL_MyButton>* dp = butts->butts.pfirst; dp; dp = dp->pnext)
	{
		if (*(butts->statue.Copy(dp->name)) == 1)
		{
			pButt = butts->butts.Copy(dp->name, true);
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
	if ((win->exitButtName) && *(butts->events.Copy(win->exitButtName)) == 1)
		return 1;
	return SDL_Poll_Quit(win->peve);
}

void QUI_Keys::Update(SDL_Event* pEve)
{
	//SDL_PollEvent(pEve);
	//if (pEve->type == SDL_KEYDOWN)
	//{
	//	if (nowKey != pEve->key.keysym.sym)
	//	{
	//		nowKey = pEve->key.keysym.sym;
	//		changed = true;
	//		PPC(nowKey);
	//	}
	//	
	//}
}
