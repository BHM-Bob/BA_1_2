#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_File.hpp"
#include"BA_JSON.hpp"
#include"BA_String.hpp"
#include"BA_GUI.hpp"
#include"BA_Test.hpp"

// TODO : 有什么更优雅的方法简化windowState中各方法函数中的加解锁
// lambda似乎有点性能损耗，宏似乎不方便

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

bool ba::ui::checkDotInRect(Sint32 x, Sint32 y, SDL_Rect* re)
{
	if ((x > re->x) && (x < re->x + re->w) && (y > re->y) && (y < re->y + re->h))
		return true;
	return false;
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
bool ba::ui::rect::checkMouseIn(Sint32 x, Sint32 y)
{
	return checkDotInRect(x, y, &re);
}

ba::ui::rect::~rect()
{
	MyBA_Free_R(mem, true);
	if(sur)
		SDL_FreeSurface(sur);
	if (tex)
		SDL_DestroyTexture(tex);
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
		mask = ba::allocNDArray<bool*, bool>({ re.h, re.w }, mem);
}
ba::ui::colorSur* ba::ui::colorSur::cacu(void)
{
	float sumlen = 0.f;
	for (long i = 0, dx = 0, dy = 0; i < sumdot; i++)
	{
		dx = re_paint.x - dots->data[i].x;
		dy = re_paint.y - dots->data[i].y;
		len->data[i] = (float)(dx * dx + dy * dy);
		if (len->data[i] == 0)
			len->data[i] = 1;
		sumlen += len->data[i];
	}
	for (long i = 0; i < sumdot; i++)
		lv->data[i] = len->data[i] / sumlen;
	col[0] = (int)(lv->data[0] * dots->data[0].col[0]) + (int)(lv->data[1] * dots->data[1].col[0]) + (int)(lv->data[2] * dots->data[2].col[0]) + (int)(lv->data[3] * dots->data[3].col[0]);
	col[1] = (int)(lv->data[0] * dots->data[0].col[1]) + (int)(lv->data[1] * dots->data[1].col[1]) + (int)(lv->data[2] * dots->data[2].col[1]) + (int)(lv->data[3] * dots->data[3].col[1]);
	col[2] = (int)(lv->data[0] * dots->data[0].col[2]) + (int)(lv->data[1] * dots->data[1].col[2]) + (int)(lv->data[2] * dots->data[2].col[2]) + (int)(lv->data[3] * dots->data[3].col[2]);
	//col[3] = (int)(lv->data[0] * dots->data[0].col[3]) + (int)(lv->data[1] * dots->data[1].col[3]) + (int)(lv->data[2] * dots->data[2].col[3]) + (int)(lv->data[3] * dots->data[3].col[3]);
	return this;
}
ba::ui::colorSur* ba::ui::colorSur::update(void)
{
	for (colorSurDot* pd = dots->data; pd != dots->lastAddress; pd++)
	{
		pd->b += 0.01f;
		ProduceRainbowCol(pd->col, &(pd->b));
		pd->x += (int)(2.0f * (pd->fx));
		if (pd->x < 1)
			pd->fx = -pd->fx;
		else if (pd->x > re.w)
			pd->fx = -pd->fx;
		pd->y += (int)(2.0f * (pd->fy));
		if (pd->y < 1)
			pd->fy = -pd->fy;
		else if (pd->y > re.h)
			pd->fy = -pd->fy;
	}

	for (re_paint.y = 0; (re_paint.y) < (re.h); (re_paint.y)++)
	{
		for (re_paint.x = 0; (re_paint.x) < (re.w); (re_paint.x)++)
		{
			if(!mask || mask[re_paint.y][re_paint.x])
			{
				cacu();
				SDL_FillRect(sur, &(re_paint),
					SDL_MapRGB(sur->format, col[0], col[1], col[2]));
			}
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
ba::ui::colorSur::~colorSur()
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

ba::ui::buttons::~buttons()
{
	for (auto p : butts)
		delete p.second;
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
bool ba::ui::buttons::del(const char* _name)
{
	if(butts.contains(_name))
	{
		button* pbutt = butts[_name];
		events.erase(_name);
		statue.erase(_name);
		butts.erase(_name);
		if (eveFunc.contains(_name))
		{
			eveFunc.erase(_name);
			eveFuncData.erase(_name);
		}
		delete pbutt;
		return true;
	}
	return false;
}

int ba::ui::_windowState_checkAll(void* _s)
{
	ba::ui::windowState* s = (ba::ui::windowState*)_s;
	SDL_Event* eveTmp = NULL;
	Sint32 x = -1, y = -1, oriX = -1, oriY = -1, wx = 0, wy = 0, winW = 0, winH = 0;
	Uint32 keyTimestamp = SDL_GetTicks();//This value wraps if the program runs for more than ~49 days.
	for ( ; ! s->getVar(false, [=]() {return s->isQuit;}) ; SDL_Delay(20))
	{
		eveTmp = s->getUpdatedEveCopy(eveTmp);
		if (eveTmp->type == SDL_MOUSEBUTTONDOWN)
		{//鼠标按下后的一些事件（按下后移动/不移动，按下后松开）
			// 检测拖动事件，一旦有鼠标按下后移动，进入循环不断检测，松开后退出循环
			for (oriX = eveTmp->motion.x, oriY = eveTmp->motion.y; eveTmp->type != SDL_MOUSEBUTTONUP; )
			{// loop quit: SDL_MOUSEBUTTONUP(1026)
				eveTmp = s->getUpdatedEveCopy(eveTmp);
				x = eveTmp->motion.x;		y = eveTmp->motion.y;
				// 发送信号：1: 鼠标按下后移动; -1: 鼠标按下不移动
				if (eveTmp->type == SDL_MOUSEMOTION || eveTmp->type == SDL_WINDOWEVENT)
					s->_setMouseEve(oriX, oriY, x, y, eveTmp->motion.xrel, eveTmp->motion.yrel, 1);
				else
					s->_setMouseEve(oriX, oriY, x, y, eveTmp->motion.xrel, eveTmp->motion.yrel, -1);
			}
			// 检测单击事件: 发送信号：2 for LEFT; 3 for RIGHT
			s->_setMouseEve(oriX, oriY, x, y, 0, 0,
				eveTmp->button.button == SDL_BUTTON_LEFT ? 2 : (eveTmp->button.button == SDL_BUTTON_RIGHT ? 3 : 0));
		}
		else if (eveTmp->type == SDL_DROPFILE)
		{//检测拖拽文件
			s->_mutexSafeWrapper([&]() {s->dropFilePath = mstrdup(eveTmp->drop.file, s->mem); });
			SDL_free(eveTmp->drop.file);
		}
		else if (eveTmp->type == SDL_DROPTEXT)
		{//检测拖拽文本
			s->_mutexSafeWrapper([&]() {s->dropText = mstrdup(eveTmp->drop.file, s->mem); });
			SDL_free(eveTmp->drop.file);
		}
		else if (eveTmp->key.state > 11 && eveTmp->key.timestamp - keyTimestamp > 30)//最快30ms捕捉一次
		{//普通键盘事件,ASCII字母。11是因为鼠标在窗口外时state为11。TODO : SDL_KEYDOWN无效，是SDL_TEXTINPUT，但是也有问题
			keyTimestamp = eveTmp->key.timestamp;
			s->_mutexSafeWrapper([&]() {s->keys.emplace_back(std::pair<SDL_Keycode, Uint32>(eveTmp->key.state, keyTimestamp)); });
		}
		else if (eveTmp->key.keysym.sym == SDLK_ESCAPE || eveTmp->type == SDL_QUIT)
		{//"退出"事件
			s->_mutexSafeWrapper([&]() {s->isQuit = true; });
		}
		else if (eveTmp->type == SDL_FINGERDOWN)
		{//触屏按压事件
			// 检测拖动事件，一旦有按下后移动，进入循环不断检测，松开后退出循环
			winW = s->getVar(winW, [&]() {return s->winW; });
			winH = s->getVar(winH, [&]() {return s->winH; });
			// 检测拖动事件，一旦有按下后移动，进入循环不断检测，松开后退出循环
			for (oriX = (Sint32)(s->winW * eveTmp->tfinger.x), oriY = (Sint32)(s->winH * eveTmp->tfinger.y);
				eveTmp->type != SDL_FINGERUP; )
			{
				eveTmp = s->getUpdatedEveCopy(eveTmp);
				x = (Sint32)(s->winW * eveTmp->tfinger.x);
				y = (Sint32)(s->winH * eveTmp->tfinger.y);
				// 发送信号：1: 鼠标按下后移动; -1: 鼠标按下不移动
				if (eveTmp->type == SDL_FINGERMOTION || eveTmp->type == SDL_WINDOWEVENT)
					s->_setMouseEve(oriX, oriY, x, y, eveTmp->motion.xrel, eveTmp->motion.yrel, 1);
				else
					s->_setMouseEve(oriX, oriY, x, y, eveTmp->motion.xrel, eveTmp->motion.yrel, -1);
			}
			s->_setMouseEve(x, y, x, y, 0, 0, 2);
		}
	}
	SDL_DestroyMutex(s->_locker);
	if (eveTmp)
		free(eveTmp);
	delete s;
	return 0;
}

void ba::ui::windowState::_setMouseEve(Sint32 mx, Sint32 my, Sint32 emx, Sint32 emy,
	Sint32 dx, Sint32 dy, int code)
{
	_mutexSafeWrapper([&]() {
		mousePos[0] = mx;
		mousePos[1] = my;
		mouseEndPos[0] = emx;
		mouseEndPos[1] = emy;
		dMouseMove[0] = dx;
		dMouseMove[1] = dy;
		mouseEveCode = code; });
}
void ba::ui::windowState::getMousePos(Sint32* x, Sint32* y, Sint32* orix, Sint32* oriy, Sint32* dx, Sint32* dy)
{
#define _ba_ui_windowState_getMousePos_assign_(p, v) if(p){*p=v;}
	_mutexSafeWrapper([&]() {
		_ba_ui_windowState_getMousePos_assign_(x, mouseEndPos[0]);
		_ba_ui_windowState_getMousePos_assign_(y, mouseEndPos[1]);
		_ba_ui_windowState_getMousePos_assign_(orix, mousePos[0]);
		_ba_ui_windowState_getMousePos_assign_(oriy, mousePos[1]);
		_ba_ui_windowState_getMousePos_assign_(dx, dMouseMove[0]);
		_ba_ui_windowState_getMousePos_assign_(dy, dMouseMove[1]); });
}
std::pair<SDL_Keycode, Uint32> ba::ui::windowState::getKeyboardEve(void)
{
	std::pair<SDL_Keycode, Uint32> ret = std::pair<SDL_Keycode, Uint32>(0, 0);
	return getVar(ret, [&]() {
		if (keys.size() > 0)
		{
			ret = keys.front();
			keys.pop_front();
		}
		return ret; });
}

ba::ui::window::window(QUI* _ui, const char* _titlepc, int winw, int winh,
	int winflags, SDL_Color bgc)
	: rect(SDL_Rect(0, 0, winw, winh), bgc)
{
	ui = _ui;
	win = this;
	titlepc = mstrdup(_titlepc, mem);
	FPS = 25.f;

	if (winflags == 0)
		winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;
	else if (winflags == 1)
		winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	pwin = SDL_CreateWindow(titlepc, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		re.w, re.h, winflags);
	SDL_GetWindowPosition(pwin, winPos, winPos+1);
	rend = SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	SDL_SetRenderDrawColor(rend, 200, 200, 200, 200);

	defaultFont = TTF_OpenFont("C:\\Windows\\Fonts\\simkai.ttf", 128);

	if (bgc.r == bgc.g == bgc.b == bgc.a == 0)
	{
		col = { 0,0,0,0 };
		SDL_VERSION(&(info.version));
		if (SDL_GetWindowWMInfo(pwin, &(info)))
		{
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
			MyBA_Err("ba::ui::window::window: unable to get window info via SDL_GetWindowWMInfo, set bgc to 0", 1);
			rendRect();
		}
	}
	else
	{
		rendRect();
	}

	winState = new windowState();
	winState->winW = re.w;
	winState->winH = re.h;
	SDL_CreateThread(ba::ui::_windowState_checkAll, "events server", (void*)winState);
	
	SDL_RenderCopy(rend, tex, NULL, NULL);
	SDL_RenderPresent(rend);
	time = clock();
}
ba::ui::window::~window()
{
	if (title)
		delete title;
	delete butts;
	TTF_CloseFont(defaultFont);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(pwin);
}
ba::ui::QUI& ba::ui::window::addOtherTex(std::string name, SDL_Texture* tex, SDL_Rect* re)
{
	std::pair<SDL_Texture*, SDL_Rect*>* p = new std::pair<SDL_Texture*, SDL_Rect*>(tex, re);
	otherTex[name] = p;
	return *ui;
}
ba::ui::QUI& ba::ui::window::updateOtherTex(std::string name, SDL_Texture* tex, bool destroyOld)
{
	if (destroyOld)
		SDL_DestroyTexture(otherTex[name]->first);
	otherTex[name]->first = tex;
	return *ui;
}
bool ba::ui::window::checkButt()
{
	// make ui window look like responsible to sys
	// TODO : deep effect remains unkown
	winState->pollEvent();

	int (*eveFunc)(void* pData, ...) = NULL;
	void* eveFuncData = NULL;
	if (winState->getMouseEveCode(&(this->re)) != 0)
	{
		if(title && winState->getMouseEveCode(&(title->re)) == 1 && winState->checkMouseIn(&(title->re)))
			return this->checkTitle();
		for (auto p = butts->butts.begin(); p != butts->butts.end(); p++)
		{
			if (butts->statue[p->first])
			{
				butts->events[p->first] = winState->getMouseEveCode(&(p->second->re));
				if (butts->events[p->first] == 2)
				{
					if (butts->eveFunc.find(p->first) != butts->eveFunc.end())
					{
						eveFunc = butts->eveFunc[p->first];
						eveFunc(butts->eveFuncData[p->first]);
					}
				}
			}
		}
	}
	return true;
}
bool ba::ui::window::checkTitle(bool rendclear, bool copyTex)
{
	// TODO : 速度不能太快：不能捕捉到窗口外的鼠标
	if (title && winState->getMouseEveCode(&(title->re)) == 1 && winState->checkMouseIn(&(title->re)))
	{
		Sint32 x = 0, y = 0, oriX = 0, oriY = 0;
		winState->getMousePos(&x, &y, &oriX, &oriY);
		if (x != 0 && y != 0)
		{
			SDL_SetWindowPosition(pwin, winPos[0] + x - oriX, winPos[1] + y - oriY);
			winPos[0] += (x - oriX);		winPos[1] += (y - oriY);
		}
	}
	return true;
}
bool ba::ui::window::update(bool rendclear, bool copyTex, bool limitFPS)
{
	if(limitFPS)
		for (float waitt = 1.f / FPS; (float)((float)clock() - time) < waitt* CLOCKS_PER_SEC; SDL_Delay(10));
	time = clock();
	winState->_mutexSafeWrapper([=]() {SDL_PollEvent(winState->_eve); });
	if (rendclear)
		SDL_RenderClear(rend);
	if (copyTex)
		SDL_RenderCopy(rend, tex, NULL, NULL);
	for (auto p = otherTex.begin(); p != otherTex.end(); p++)
		SDL_RenderCopy(rend, p->second->first,
			NULL, p->second->second);
	button* pb = NULL;
	for (auto p = butts->butts.begin(); p != butts->butts.end(); p++)
	{
		if (butts->statue[p->first])
		{
			pb = butts->butts[p->first];
			if (pb->cs)
			{
				pb->cs->getTex();
				SDL_RenderCopy(rend, pb->cs->tex, NULL, &(pb->re));
			}
			SDL_RenderCopy(rend, pb->tex, NULL, &(pb->re));
		}
	}
	if (title && title->tex)
		SDL_RenderCopy(rend, title->tex, NULL, &(title->re));
	SDL_RenderPresent(rend);
	return true;
}
bool ba::ui::window::pollQuit()
{
	this->checkButt();
	if ((exitButtName) && butts->events[exitButtName] == 2)
		return 1;
	return winState->getVar(false, [&]() {return winState->isQuit; });
}
bool ba::ui::window::delButt(const char* _name)
{
	return butts->del(_name);
}

// TODO : why should put this func before ba::ui::QUI::QUI
	// C2065 if this put after ba::ui::QUI::QUI
int ba::ui::QUI_Quit(void* pui_, int code, ...)
{
	// TODO : if ba::ui::QUI was inited as a local var(not a ptr in heap memory),
		// this will cause mem err
	ba::ui::QUI* pui = (ba::ui::QUI*)pui_;
	return 0;
}
ba::ui::QUI::QUI(const char* titlepc, int winw, int winh, int winflags, SDL_Color bgc)
{
	int img_f = IMG_INIT_JPG;// | IMG_INIT_PNG;
	if ((SDL_Init(SDL_INIT_EVERYTHING) == -1) || (TTF_Init() == -1) || (IMG_Init(img_f) != (img_f)))/*|| Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID)==0)*/
	{
		MyBA_Err("MyUI_Init: Can't Init SDL2", 1);
	}
	else
	{
		window* win = new window(this, titlepc, winw, winh, winflags, bgc);
		windows[titlepc] = win;
		activeWin = win;

		MyBA_AtQuit(QUI_Quit, (void*)this);
	}
}
ba::ui::QUI& ba::ui::QUI::addWindow(const char* titlepc, int winw, int winh, int winflags, SDL_Color bgc)
{
	window* win = new window(this, titlepc, winw, winh, winflags, bgc);
	windows[titlepc] = win;
	return *this;
}
bool ba::ui::QUI::delWindow(const char* titlepc)
{
	if (windows.contains(titlepc))
	{
		if (activeWin == windows[titlepc])
		{
			if (windows.size() == 1)
				return false;//在只有一个窗口时禁止销毁该窗口
			activeWin = windows.begin()->second;//回到第一个窗口
		}
		window* pwin = windows[titlepc];
		pwin->winState->_mutexSafeWrapper([&]() {pwin->winState->isQuit = true; });
		delete windows[titlepc];
		windows.erase(titlepc);
	}
	return false;
}
ba::ui::QUI& ba::ui::QUI::setActiveWindow(const char* title)
{
	if (title && windows.contains(title))
		activeWin = windows[title];
	return *this;
}
int ba::ui::QUI::Quit(int code, ...)
{
	window* win = activeWin;
	for (auto p : windows)
	{
		win = p.second;
		delete win;
	}
	TTF_Quit();
	//MyBA_Free_R(mem);
	List_SetVar(pba->exitFunc, (void*)QUI_Quit, (void*)0x1);
	return 0;
}
