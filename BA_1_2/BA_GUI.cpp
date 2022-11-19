﻿#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_JSON.hpp"
#include"BA_String.hpp"
#include"BA_GUI.hpp"
#include"BA_Test.hpp"


int* ba::ui::ProduceRainbowCol(int* col, float* i, float* di)// r g b
{
	*i += *di;
	float sh = 5000.f;
	if (*i > sh || *i < -sh)
	{
		*di = -*di;
	}
	col[0] = (int)(127.f * sin(*i)) + 127;
	col[1] = (int)(127.f * cos(*i)) + 127;
	col[2] = (int)(127.f * sin(0.8f * (*i))) + 127;
	return col;
}
int* ba::ui::ProduceRainbowCol(int* col, float* i, float di)// r g b
{
	*i += di;
	float sh = 5000.f;
	if (*i > sh || *i < -sh)
		*i = 0;
	col[0] = (int)(127.f * sin(*i)) + 127;
	col[1] = (int)(127.f * cos(*i)) + 127;
	col[2] = (int)(127.f * sin(0.8f * (*i))) + 127;
	return col;
}
SDL_Color* ba::ui::SetSDLCol(SDL_Color* col, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
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
SDL_Color* ba::ui::MakeSDLCol(List* mem, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
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
SDL_Rect* ba::ui::SetSDLRect(SDL_Rect* pos, int w, int h, int x, int y)
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
SDL_Rect* ba::ui::MakeSDLRect(List* mem, int w, int h, int x, int y)
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

/*
		int r = (color >> 16) & 0xFF;
		int g = (color >> 8) & 0xFF;
		int b = (color >> 0) & 0xFF;
*/
Uint32 ba::ui::getPixle(SDL_Surface* surface, int x, int y)
{
	if(x >= 0 && y >= 0)
	{
		int bpp = surface->format->BytesPerPixel;
		Uint32* p = (Uint32*)(surface->pixels);
		return *(p + (Uint32)y * (surface->w) + (Uint32)x);
	}
	else
	{
		MyBA_Err("x or y is negative, return -1", 1);
		return 0;
	}
}
int ba::ui::formatPixle(Uint32 pixle, char channle)
{
	switch (channle)
	{
	case 'r':
		return (pixle >> 16) & 0xFF;
	case 'g':
		return (pixle >> 8) & 0xFF;
	case 'b':
		return (pixle >> 0) & 0xFF;
	}
	MyBA_Err("Unable to handle channle, return -1", 1);
	PPX(channle);
	return -1;
}
SDL_Texture* ba::ui::createRGBTexture(SDL_Renderer* rend, int w, int h, int r, int g, int b)
{
	SDL_Surface* pSur = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	SDL_FillRect(pSur, NULL, SDL_MapRGB(pSur->format, r, g, b));
	SDL_Texture* pTex = SDL_CreateTextureFromSurface(rend, pSur);
	SDL_FreeSurface(pSur);
	return pTex;
}
SDL_Surface* ba::ui::getImageSur(const char* path)
{
	SDL_Surface* pte = IMG_Load(path);
	if (!pte)
		return (SDL_Surface*)MyBA_Errs(1, "getImageSur::Can't load IMG from ", path,
			", return NULL", NULL);
	return pte;
}
SDL_Texture* ba::ui::getImageTex(SDL_Renderer* renderer, const char* path)
{
	SDL_Surface* pte = getImageSur(path);
	if (!pte)
		return (SDL_Texture*)MyBA_Errs(1, "getImageTex::Can't load IMG from ",
			path, ", return NULL", NULL);
	return SDL_CreateTextureFromSurface(renderer, pte);
}

void ba::ui::rect::rendRect(void)
{
	if (win)
	{
		sur = SDL_CreateRGBSurface(0, re.w, re.h, 32, 0, 0, 0, 0);
		SDL_FillRect(sur, NULL, SDL_MapRGBA(sur->format, col.r, col.g, col.b, col.a));
		tex = SDL_CreateTextureFromSurface(win->rend, sur);
	}
	else
	{
		MyBA_Err("ba::ui::rect::rendRect: win is not assigned", 1);
	}
}
bool ba::ui::rect::checkPressOn(SDL_Event* peve)
{
	SDL_PollEvent(peve);
	if (peve->type == SDL_MOUSEBUTTONDOWN)
	{
		Sint32 mx = peve->motion.x;
		Sint32 my = peve->motion.y;
		if (mx > re.x && mx<re.x + re.w && my>re.y && my < re.y + re.h)
			return 1;
	}
	return 0;
}


ba::ui::colorSur::colorSur(window* _win, SDL_Surface* _distSur, SDL_Rect pos,
	bool alloc0Mask, int _sumdot)
	: rect(pos, {0,0,0,0})
{
	if(_distSur)
	{
		sur = _distSur;
		re.w = _distSur->w;
		re.h = _distSur->h;
	}
	else
	{
		sur = SDL_CreateRGBSurface(0, re.w, re.h, 32, 0, 0, 0, 0);
	}
	win = _win;
	sumdot = _sumdot;
	dots = new tensor<colorSurDot>({ _sumdot });
	lv = new tensor<float>({ _sumdot });
	len = new tensor<float>({ _sumdot });
	dots->selfmap([&](colorSurDot* r, _LL i) {
		r->b = 500.f / (float)(1.f + rand() % 200);
	r->x = 1 + rand() % (re.w - 1);
	r->y = 1 + rand() % (re.h - 1);
	r->fx = r->fy = 1.0;
	ProduceRainbowCol(r->col, &(r->b));
	//r->col[3] = (int)(127.f * sin(0.8f * r->b)) + 0;
		});
	if (alloc0Mask)
	{
		mask = BALLOC_R(re.h, bool*, mem);
		for (re_paint.y = 0; (re_paint.y) < (re.h); (re_paint.y)++)
			mask[re_paint.y] = BALLOC_R(re.w, bool, mem);
	}
}
ba::ui::colorSur* ba::ui::colorSur::cacu(void)
{
	float sumlen = 0.f;
	float sumlength = 0.f;
	long dx = 0;
	long dy = 0;
	long j = 0;
	float k = 0;
	float k2 = 0;
	for (long i = 0; i < sumdot; i++)
	{
		dx = re_paint.x - dots->data[i].x;
		dy = re_paint.y - dots->data[i].y;
		k = (float)(dx * dx + dy * dy);
		if (k <= 0)
			k = -k + 1;
		j = *(long*)&k;                        // evil floating point bit level hacking
		j = 0x5f3759df - (j >> 1);             // what the fuck? 
		k2 = *(float*)&j;
		len->data[i] = k2 * k2 * (1.5f - (0.5f * k * k2 * k2));//originaly is k2 * ( 1.5f - ( 0.5*k * k2 * k2 ) ) but to use plen[i] = plen[i]*plen[i];
		sumlen += len->data[i];
	}
	for (long i = 0; i < sumdot; i++)
		lv->data[i] = len->data[i] / sumlen;
	col[0] = (int)(lv->data[0] * dots->data[0].col[0]) + (int)(lv->data[1] * dots->data[1].col[0]) + (int)(lv->data[2] * dots->data[2].col[0]) + (int)(lv->data[3] * dots->data[3].col[0]);
	col[1] = (int)(lv->data[0] * dots->data[0].col[1]) + (int)(lv->data[1] * dots->data[1].col[1]) + (int)(lv->data[2] * dots->data[2].col[1]) + (int)(lv->data[3] * dots->data[3].col[1]);
	col[2] = (int)(lv->data[0] * dots->data[0].col[2]) + (int)(lv->data[1] * dots->data[1].col[2]) + (int)(lv->data[2] * dots->data[2].col[2]) + (int)(lv->data[3] * dots->data[3].col[2]);
	col[3] = 255;
	return this;
}
ba::ui::colorSur* ba::ui::colorSur::update(void)
{
	for (colorSurDot* pd = dots->data; pd != dots->lastAddress; pd++)
	{
		pd->b += 0.05f;
		ProduceRainbowCol(pd->col, &(pd->b));
		pd->x += (int)(2.0f * (pd->fx));
		if (pd->x < 1)
		{
			pd->fx = -pd->fx;
			pd->x = 1;
		}
		if (pd->x > re.w)
		{
			pd->fx = -pd->fx;
			pd->x = re.w - 1;
		}
		pd->y += (int)(2.0f * (pd->fy));
		if (pd->y < 1)
		{
			pd->fy = -pd->fy;
			pd->y = 1;
		}
		if (pd->y > re.h)
		{
			pd->fy = -pd->fy;
			pd->y = re.h - 1;
		}
	}

	for (re_paint.y = 0; (re_paint.y) < (re.h); (re_paint.y)++)
	{
		for (re_paint.x = 0; (re_paint.x) < (re.w); (re_paint.x)++)
		{
			if(!mask || mask[re_paint.y][re_paint.x])
			{
				cacu();
				SDL_FillRect(sur, &(re_paint),
					SDL_MapRGBA(sur->format, col[0], col[1], col[2], col[3]));
			}
			//else
			//{
			//	SDL_FillRect(sur, &(re_paint),
			//		SDL_MapRGBA(sur->format, 0, 0, 0, 0));
			//}
		}
	}
	return this;
}
SDL_Texture* ba::ui::colorSur::getTex(void)
{
	update();
	if (tex)
		SDL_DestroyTexture(tex);
	tex = SDL_CreateTextureFromSurface(win->rend, sur);
	return tex;
}
void ba::ui::colorSur::destroy(void)
{
	delete dots;
	delete len;
	delete lv;
	MyBA_Free_R(mem);
	SDL_FreeSurface(sur);
	SDL_DestroyTexture(tex);
}

//bool ba::ui::MyUI_ColorText_Destroy(MyUI_ColorText* pct)
//{
//	MyBA_Free_R(mem);
//	SDL_FreeSurface(fontSur);
//	SDL_FreeSurface(pSur);
//	free(pct);
//	return true;
//}


ba::ui::colorText::colorText(window* _win, const char* pc)
	: colorSur(_win, TTF_RenderUTF8_Blended(_win->defaultFont, pc, { .r = 0 , .g = 255 , .b = 0, .a = 255 }),
		{ 0 }, true)
{
	font = win->defaultFont;
	rend = win->rend;
	pc = mstrdup(pc, mem);
	if (re.w == 0)
	{
		MyBA_Errs(1, "MyUI_ColorText_Init: Can't blended Surface with text:", pc, ", skip", NULL);
	}
	else
	{
		// TODO : why tmp is necessary???
		// 为什么直接从fontSur中读取像素是失败的？？？？(get 255 0 0 0)
		SDL_Surface* tmp = SDL_CreateRGBSurface(0, re.w, re.h, 32, 0, 0, 0, 255);
		SDL_SetColorKey(tmp, 1, SDL_MapRGB(sur->format, 0, 0, 0));
		SDL_BlitSurface(sur, NULL, tmp, NULL);

		Uint8 r, g, b;
		SDL_LockSurface(tmp);
		for (re_paint.y = 0; (re_paint.y) < (re.h); (re_paint.y)++)
		{
			for (re_paint.x = 0; (re_paint.x) < (re.w); (re_paint.x)++)
			{
				SDL_GetRGB(getPixle(tmp, re_paint.x, re_paint.y), tmp->format, &r, &g, &b);
				if (g == 255)
					mask[re_paint.y][re_paint.x] = 1;
			}
		}
		SDL_UnlockSurface(tmp);
		SDL_FreeSurface(tmp);
	}
}

ba::ui::label::label(window* _win, const char* pc, int charSize, SDL_Color charCol,
	SDL_Rect pos, SDL_Color bgc)
	: rect(pos, bgc)
{
	win = _win;
	text = pc;
	if (pos.w <= 0 || pos.h <= 0)//自行根据字符串大小计算label大小
	{
		if (pos.w <= 0)
			re.w = pos.w = (int)text.size() * charSize;
		if (pos.h <= 0)
			re.h = charSize;
		if (charSize == 0 || (int)text.size() == 0)
		{
			MyBA_Err("ba::ui::label::label: pos.w <= 0 || pos.h <= 0 and charSize == 0 || str.size() == 0, create a NULL rect", 1);
		}
	}

	sur = TTF_RenderUTF8_Blended(win->defaultFont, text.c_str(), charCol);
	if (sur == NULL)
	{
		MyBA_Errs(1, "ba::ui::label::label: Can't blended Surface with text:",
			text.c_str(), ", skip", NULL);
	}
	else
	{
		SDL_Surface* bgs = SDL_CreateRGBSurface(0, re.w, re.h, 32, 0, 0, 0, 0);
		SDL_FillRect(bgs, NULL, SDL_MapRGBA(bgs->format, bgc.r, bgc.g, bgc.b, bgc.a));
		SDL_BlitScaled(sur, NULL, bgs, NULL);
		SDL_FreeSurface(bgs);
		tex = SDL_CreateTextureFromSurface(win->rend, sur);
	}
}

ba::ui::button::button(window* _win, const char* pc, int charSize, SDL_Color charCol,
	SDL_Rect pos, SDL_Color bgc)
	: label(_win, pc, charSize, charCol, pos, bgc)
{
}

ba::ui::buttons::buttons(window* _win)
{
	win = _win;
}

bool ba::ui::buttons::add(const char* _name, const char* _showWords, int charSize,
	SDL_Color charCol, SDL_Color bgc, SDL_Rect pos, const char* align, SDL_Surface* bg,
	int(*_eveFunc)(void* pData, ...), void* _eveFuncData)
{
	char* name = mstrdup(_name, mem);
	if (!_showWords)
		_showWords = _name;
	char* showWords = _strdup(_showWords);
	if (pos.w <= 0 || pos.h <= 0)//自行根据字符串大小计算按钮大小
	{
		if (charSize == 0 || strlen(showWords) == 0)
			return 0;
		if (pos.w < 0)
			pos.w = (int)strlen(showWords) * charSize;
		if (pos.h < 0)
			pos.h = charSize;
		if (!strcmp(align, "tr"))//top && right
		{
			pos.x = win->re.w - pos.x;
		}
		else if (!strcmp(align, "bl"))//bottom && left
		{
			pos.h = win->re.h - pos.h;
		}
		else if (!strcmp(align, "br"))//bottom && right
		{
			pos.h = win->re.h - pos.h;
			pos.x = win->re.w - pos.x;
		}
	}
	button* pb = new button(win, showWords, charSize, charCol, pos, bgc);
	if (bg == (SDL_Surface*)(0x1))//Use colorSur
		pb->cs = new colorSur(win, NULL, pb->re);

	// just use char ptr
	butts[name] = pb;
	statue[name] = 1;
	events[name] = false;
	if (_eveFunc)
	{
		eveFunc[name] = _eveFunc;
		eveFuncData[name] = _eveFuncData;
	}
	return true;
}

ba::ui::window::window(QUI* _ui, const char* _titlepc, int winw, int winh,
	int winflags, SDL_Color* bgc)
	: rect(SDL_Rect(0, 0, winw, winh), bgc ? *bgc : SDL_Color(0, 0, 0, 0))
{
	ui = _ui;
	titlepc = mstrdup(_titlepc, mem);
	peve = BALLOC_R(1, SDL_Event, mem);
	FPS = 25.f;

	if (winflags == 0)
		winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;
	else if (winflags == 1)
		winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	pwin = SDL_CreateWindow(titlepc, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		re.w, re.h, winflags);
	rend = SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	SDL_SetRenderDrawColor(rend, 200, 200, 200, 200);

	defaultFont = TTF_OpenFont("C:\\Windows\\Fonts\\simkai.ttf", 128);

	if (!bgc)
	{
		col = { 0,0,0,0 };
		SDL_VERSION(&(info.version));
		if (SDL_GetWindowWMInfo(pwin, &(info)))
			hwnd = info.info.win.window;
		/*设置窗口colorkey*/
		SetWindowLongW(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
		/*设置窗口为悬浮窗 */
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		sur = SDL_GetWindowSurface(pwin);
		UINT32 keyColor = SDL_MapRGB(sur->format, 255, 255, 255);
		SDL_SetSurfaceBlendMode(sur, SDL_BLENDMODE_NONE);
		SDL_FillRect(sur, NULL, keyColor);
		tex = SDL_CreateTextureFromSurface(rend, sur);
	}
	else
	{
		rendRect();
	}

	SDL_RenderCopy(rend, tex, NULL, NULL);
	SDL_RenderPresent(rend);
	time = clock();
}

// TODO : why should put this func before ba::ui::QUI::QUI
	// C2065 if this put after ba::ui::QUI::QUI
int ba::ui::QUI_Quit(void* pui_, int code, ...)
{
	// TODO : if ba::ui::QUI was inited as a local var(not a ptr in heap memory),
		// this will cause mem err
	ba::ui::QUI* pui = (ba::ui::QUI*)pui_;
	//SDL_FreeSurface(pui->win->sur);
	//SDL_DestroyTexture(pui->win->tex);
	//SDL_DestroyRenderer(pui->win->rend);
	//SDL_DestroyWindow(pui->win->pwin);
	return 0;
}
ba::ui::QUI::QUI(const char* titlepc, int winw, int winh, int winflags, SDL_Color* bgc)
{
	int img_f = IMG_INIT_JPG;// | IMG_INIT_PNG;
	if ((SDL_Init(SDL_INIT_EVERYTHING) == -1) || (TTF_Init() == -1) || (IMG_Init(img_f) != (img_f)))/*|| Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID)==0)*/
	{
		MyBA_Err("MyUI_Init: Can't Init SDL2", 1);
	}
	else
	{
		window* win = new window(this, titlepc, winw, winh, winflags, bgc);
		windows.push_back(win);
		activeWindow = 0;
		activeWin = windows[activeWindow];

		MyBA_AtQuit(QUI_Quit, (void*)this);
	}
}
ba::ui::QUI& ba::ui::QUI::addOtherTex(std::string name, SDL_Texture* tex, SDL_Rect* re)
{
	std::pair<SDL_Texture*, SDL_Rect*>* p = new std::pair<SDL_Texture*, SDL_Rect*>(tex, re);
	windows[activeWindow]->otherTex[name] = p;
	return *this;
}
ba::ui::QUI& ba::ui::QUI::updateOtherTex(std::string name, SDL_Texture* tex)
{
	windows[activeWindow]->otherTex[name]->first = tex;
	return *this;
}
bool ba::ui::QUI::delButt(const char* _name)
{
	return false;
}
bool ba::ui::QUI::checkButt()
{
	window* win = windows[activeWindow];

	SDL_PollEvent(win->peve);
	clock_t st = clock();
	int (*eveFunc)(void* pData, ...) = NULL;
	void* eveFuncData = NULL;
	if (win->peve->type == SDL_MOUSEBUTTONDOWN)
	{
		for (SDL_PollEvent(win->peve); win->peve->type != SDL_MOUSEBUTTONUP;
			SDL_PollEvent(win->peve), SDL_Delay(1))
		{
			if ((float)(clock() - st) / CLOCKS_PER_SEC > 0.2)
				return this->checkTitle();
		}
		Sint32 mx = win->peve->motion.x;
		Sint32 my = win->peve->motion.y;
		int x, y, w, h;
		for (auto p = win->butts->butts.begin(); p != win->butts->butts.end(); p++)
		{
			if (win->butts->statue[p->first])
			{
				win->butts->events[p->first] = 0;
				x = p->second->re.x;
				y = p->second->re.y;
				w = p->second->re.w;
				h = p->second->re.h;
				if (mx > x && mx<x + w && my>y && my < y + h)
				{
					if (win->peve->button.button == SDL_BUTTON_LEFT)
					{
						win->butts->events[p->first] = 1;
						if (win->butts->eveFunc.find(p->first) != win->butts->eveFunc.end())
						{
							eveFunc = win->butts->eveFunc[p->first];
							eveFunc(win->butts->eveFuncData[p->first]);
						}
					}
					else if (win->peve->button.button == SDL_BUTTON_RIGHT)
					{
						win->butts->events[p->first] = 2;
					}
				}
			}
		}
	}
	return true;
}
bool ba::ui::QUI::checkTitle(bool rendclear, bool copyTex)
{
	window* win = windows[activeWindow];

	Sint32 wx = 0, wy = 0, bx = 0, by = 0;
	if (win->title && win->title->checkPressOn(win->peve))
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
			//update(rendclear, copyTex);
		}
	}
	return true;
}
bool ba::ui::QUI::update(bool rendclear, bool copyTex)
{
	window* win = windows[activeWindow];

	for (float waitt = 1.f / win->FPS;
		(float)((float)clock() - win->time) / CLOCKS_PER_SEC < waitt; SDL_Delay(1));
	win->time = clock();
	if (rendclear)
		SDL_RenderClear(win->rend);
	if (copyTex)
		SDL_RenderCopy(win->rend, win->tex, NULL, NULL);
	for (auto p = win->otherTex.begin(); p != win->otherTex.end(); p++)
		SDL_RenderCopy(win->rend, p->second->first,
			NULL, p->second->second);
	button* pb = NULL;
	for (auto p = win->butts->butts.begin(); p != win->butts->butts.end(); p++)
	{
		if (win->butts->statue[p->first])
		{
			pb = win->butts->butts[p->first];
			if (pb->cs)
			{
				pb->cs->getTex();
				SDL_RenderCopy(win->rend, pb->cs->tex, NULL, &(pb->re));
			}
			SDL_RenderCopy(win->rend, pb->tex, NULL, &(pb->re));
		}
	}
	if (win->title && win->title->tex)
		SDL_RenderCopy(win->rend, win->title->tex, NULL, &(win->title->re));
	SDL_RenderPresent(win->rend);
	return true;
}
bool ba::ui::QUI::pollQuit()
{
	window* win = windows[activeWindow];
	this->checkButt();
	if ((win->exitButtName) && win->butts->events[win->exitButtName] == 1)
		return 1;
	if ((win->peve->type == SDL_QUIT) || ((win->peve->type == SDL_KEYUP) && (win->peve->key.keysym.sym == SDLK_ESCAPE)))//KEYUP 防止上一次多按
		return 1;
	return 0;
}
int ba::ui::QUI::Quit(int code, ...)
{
	window* win = windows[activeWindow];
	SDL_FreeSurface(win->sur);
	SDL_DestroyTexture(win->tex);
	SDL_DestroyRenderer(win->rend);
	SDL_DestroyWindow(win->pwin);
	//MyBA_Free_R(mem);
	List_SetVar(pba->exitFunc, (void*)QUI_Quit, (void*)0x1);
	return 0;
}
