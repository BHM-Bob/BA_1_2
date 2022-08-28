//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021年11月18日 23点35分

#include"BA_Base.hpp"
#include"BA_UI.hpp"

int* ProduceRainbowCol(int* col, float* i, float* di)// r g b
{
	*i += *di;
	if (*i > 10000.f || *i < -10000.f)
	{
		*di = -*di;
		*i += 100. * (*di);
	}
	col[0] = 127 * sin(*i) + 127;
	col[1] = 127 * cos(*i) + 127;
	col[2] = 127 * sin(0.8f * (*i)) + 127;
	return col;
}

int* ProduceRainbowCol(int* col, float* i, float di)// r g b
{
	*i += di;
	float sh = 5000.f * (0.5 * sin(*i) + 0.7);
	if (*i > sh || *i < -sh)
		*i -= 400. * di;
	col[0] = 127 * sin(*i) + 127;
	col[1] = 127 * cos(*i) + 127;
	col[2] = 127 * sin(0.8f * (*i)) + 127;
	return col;
}


bool SDL_Init_s(void)//ok return 1
{
	int img_f = IMG_INIT_JPG;// | IMG_INIT_PNG;
	if ((SDL_Init(SDL_INIT_EVERYTHING) == -1) || (TTF_Init() == -1) || (IMG_Init(img_f) != (img_f)))/*|| Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID)==0)*/
		return 0;
	return 1;
}

/*
		int r = (color >> 16) & 0xFF;
		int g = (color >> 8) & 0xFF;
		int b = (color >> 0) & 0xFF;
*/
Uint32 SDL_GetPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint32* p = (Uint32*)(surface->pixels);
	return *(p + (Uint32)y * (surface->w) + (Uint32)x);
}

SDL_Texture* SDL_CreateRGBTexture(SDL_Renderer* rend, int w, int h, int r, int g, int b)
{
	SDL_Surface* pSur = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	SDL_FillRect(pSur, NULL, SDL_MapRGB(pSur->format, r, g, b));
	SDL_Texture* pTex = SDL_CreateTextureFromSurface(rend, pSur);
	SDL_FreeSurface(pSur);
	return pTex;
}

SDL_Window* SDL_Pushmess(const char* mess, TTF_Font* pfont, int charsize, int sec)
{
	if (!SDL_Init_s())
		return NULL;
	int len = strlen(mess);
	int w = len * charsize + 2, h = charsize + 5;
	char* title = Num_To_Char(_strdup("d"), sec);
	SDL_Window* pwin = SDL_CreateWindow(title, 0, 0, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	free(title);
	if (pwin == NULL)
		return NULL;
	SDL_Renderer* rend = SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(rend);
	SDL_Color color = { 255,255,255 };

#ifdef USE_WINDOWS
	int i = MultiByteToWideChar(CP_ACP, 0, mess, -1, NULL, 0);
	wchar_t* pte1 = MCALLOC(strlen(mess) + 1, wchar_t);
	MultiByteToWideChar(CP_ACP, 0, mess, -1, pte1, i);
	i = WideCharToMultiByte(CP_UTF8, 0, pte1, -1, NULL, 0, NULL, NULL);
	char* pte2 = MCALLOC(strlen(mess) + 1, char);
	WideCharToMultiByte(CP_UTF8, 0, pte1, -1, pte2, i, NULL, NULL);
	SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, pte2, color);
	free(pte1);
	free(pte2);
#else
	SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, mess, color);
#endif

	if (pte == NULL)
	{
		PPW("Can't render a surface");
		PPS(mess);
		SDL_FreeSurface(pte);
		SDL_DestroyWindow(pwin);
		SDL_DestroyRenderer(rend);
		return NULL;
	}
	SDL_Rect Box = { 1,  1,  w,  h };
	SDL_Texture* pTex = SDL_CreateTextureFromSurface(rend, pte);
	SDL_RenderCopy(rend, pTex, NULL, &Box);
	SDL_RenderPresent(rend);
	SDL_FreeSurface(pte);
	SDL_DestroyTexture(pTex);
	SDL_DestroyRenderer(rend);


	return pwin;
}

SDL_Surface* SDL_GetImage_Sur(const char* ppath)
{
#ifdef USE_WINDOWS
	ppath = GBK_To_UTF8(ppath);
#endif //USE_WINDOWS

	SDL_Surface* pte = IMG_Load(ppath);
	if (!pte)
		return (SDL_Surface*)MyBA_Errs(1, "SDL_GetImage_Sur:Can't load IMG ", ppath, ", return NULL", NULL);
	return pte;
}
SDL_Texture* SDL_GetImage_Tex(SDL_Renderer* renderer, const char* ppath, bool BackColorKeep, int BR, int BG, int BB)
{
	SDL_Surface* pte = IMG_Load(ppath);
	if (!pte)
		return(SDL_Texture*)MyBA_Errs(1, "SDL_GetImage_Tex:Can't load IMG ", ppath, ", return NULL", NULL);
	if (!BackColorKeep)
	{
		Uint32 color = SDL_MapRGB(pte->format, BR, BG, BB);
		SDL_SetColorKey(pte, SDL_TRUE, color);
	}
	return SDL_CreateTextureFromSurface(renderer, pte);
}

SDL_Texture* SDL_Get_Font_Texture(SDL_Renderer* prend, SDL_Surface* pscr, TTF_Font* pfont, int* charcolor, int Size, const char* pc, int x, int y)
//Mem waring: pc does not free in this func
{
	SDL_Color color;//@@@@@@@@@@@@@@@@能不能把这一行安全地,合法地去掉???????????????????????????????????????????????????
	if (charcolor != NULL)
	{
		color.r = charcolor[0];
		color.g = charcolor[1];
		color.b = charcolor[2];
	}
	else
	{
		color.r = 255;
		color.g = 255;
		color.b = 255;
	}
	SDL_Rect Box = { x,  y,  (int)strlen(pc) * Size ,  Size };

	SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, pc, color);
	SDL_Texture* pTex = NULL;
	if (pte == NULL)
		return (SDL_Texture*)MyBA_Errs(0, "SDL_Get_Font_Texture: Can't blended Surface with text:", pc, ", skip", NULL);
	if (pscr != NULL)
	{
		if (pscr->w < Box.w || pscr->h < Box.h)
			SDL_BlitScaled(pte, NULL, pscr, NULL);
		else
			SDL_BlitScaled(pte, NULL, pscr, &Box);
		pTex = SDL_CreateTextureFromSurface(prend, pscr);
	}
	else
	{
		SDL_Surface* pte2 = SDL_CreateRGBSurface(0, Box.w, Box.h, 32, 0, 0, 0, 0);
		SDL_SetColorKey(pte2, 1, SDL_MapRGB(pte2->format, 0, 0, 0));
		Box.x = Box.y = 0;
		SDL_BlitScaled(pte, NULL, pte2, &Box);
		pTex = SDL_CreateTextureFromSurface(prend, pte2);
		SDL_FreeSurface(pte2);
	}
	SDL_FreeSurface(pte);
	return pTex;
}

SDL_Surface* SDL_Get_Font_Surface(SDL_Surface** ppscr, TTF_Font* pfont, int R, int G, int B, int Size, char* pc, int x, int y)
{
	SDL_Color color = { R,G,B };
	SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, pc, color);
	if (pte == NULL)
		return (SDL_Surface*)MyBA_Errs(0, "SDL_Get_Font_Surface: Can't blended Surface with text:", pc, ", skip", NULL);
	SDL_Rect Box = { x, y,  (int)strlen(pc) * Size,  Size };
	SDL_BlitScaled(pte, NULL, *ppscr, &Box);
	SDL_FreeSurface(pte);
	return *ppscr;
}

void SDL_printf_Texture(SDL_Renderer* prenderer, int x, int y, int w, int h, int point_x, int point_y, int angel, SDL_Texture* pmess)
{
	SDL_Point center = { point_x,  point_y };
	SDL_Rect Box = { x,  y,  w,  h };
	SDL_RenderCopyEx(prenderer, pmess, NULL, &Box, angel, &center, SDL_FLIP_NONE);
}

void SDL_printf(char* pc, SDL_Renderer* prenderer, bool Prensent_Now, int x, int y, int Size, int R, int G, int B, TTF_Font* pfont)
{
	if (pc == NULL || strlen(pc) == 0)
	{
		PPW("pc==NULL || strlen(pc)==0");
		return;
	}
	SDL_Color color = { R,G,B };
#ifdef USE_WINDOWS
	int i = MultiByteToWideChar(CP_ACP, 0, pc, -1, NULL, 0);
	wchar_t* pte1 = MCALLOC(strlen(pc) + 1, wchar_t);
	MultiByteToWideChar(CP_ACP, 0, pc, -1, pte1, i);
	i = WideCharToMultiByte(CP_UTF8, 0, pte1, -1, NULL, 0, NULL, NULL);
	char* pte2 = MCALLOC(strlen(pc) + 1, char);
	WideCharToMultiByte(CP_UTF8, 0, pte1, -1, pte2, i, NULL, NULL);
	SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, pte2, color);
	free(pte1);
	free(pte2);
#endif

	/*	iconv_t cd;
		char* ptechar = MCALLOC(strlen(pc+1),char);
		size_t in = strlen(pc),out = strlen(ptechar);
		if( (cd = iconv_open("UTF-8","GB2312")) == 0)
			return ;
		if( iconv(cd,(const char**)(&pc),&in,&ptechar,&out) == -1)
			return ;
		iconv_close(cd);*/
#ifndef USE_WINDOWS
	SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, pc, color);
#endif
	if (pte == NULL)
	{
		PPW("pte==NULL");
		return;
	}
	SDL_Rect Box = { x,  y,  (int)strlen(pc) * Size,  Size };
	SDL_Texture* pTex = SDL_CreateTextureFromSurface(prenderer, pte);
	SDL_RenderCopy(prenderer, pTex, NULL, &Box);
	if (Prensent_Now)
		SDL_RenderPresent(prenderer);
	SDL_FreeSurface(pte);
	SDL_DestroyTexture(pTex);
}

bool SDL_Poll_Quit(SDL_Event* pevent)
{
	SDL_PollEvent(pevent);
	if ((pevent->type == SDL_QUIT) || ((pevent->type == SDL_KEYUP) && (pevent->key.keysym.sym == SDLK_ESCAPE)))//KEYUP 防止上一次多按
		return 1;
	return 0;
}

char* SDL_Poll_Scnaf(SDL_Event* peve, char** pc)
{
	return NULL;
}

SDL_MyButton* SDL_Create_MyButton(SDL_Renderer* rend, Uint32 Format, SDL_Rect re_butt_in, int backcolor[3], const char* pc, TTF_Font* pfont, unsigned int charsize, int charcolor[3], SDL_Surface* back)
{
	SDL_MyButton* pbutt = MCALLOC(1, SDL_MyButton);
	if (Format == 0)
		Format = SDL_PIXELFORMAT_RGBA8888;//默认
	SDL_Texture** ppTex = MCALLOC(1, SDL_Texture*);//从栈内存跳出去的跳板@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	pbutt->ppTex = ppTex;
	if (re_butt_in.w < 0 || re_butt_in.h < 0)//用户此行为,则要求自行根据字符串大小计算按钮大小
	{
		if (charsize == 0 || strlen(pc) == 0)
			return NULL;
		if (re_butt_in.w < 0)
			pbutt->re_butt.w = re_butt_in.w = strlen(pc) * charsize;
		if (re_butt_in.h < 0)
			pbutt->re_butt.h = re_butt_in.h = charsize;
	}
	pbutt->re_butt = re_butt_in;
	for (int a = 0; a < 3; a++)
	{
		if (backcolor != NULL)
			pbutt->backcolor[a] = backcolor[a];
		pbutt->charcolor[a] = charcolor[a];
	}
	pbutt->Format = Format;
	pbutt->pc = _strdup(pc);
	pbutt->pfont = pfont;
	pbutt->charsize = charsize;

	SDL_Surface* pte = NULL;
	if (back != NULL)
	{
		pbutt->back1 = back;
		*ppTex = SDL_Get_Font_Texture(rend, back, pfont, TypeDupR(NULL, 3, charcolor[0], charcolor[1], charcolor[2]), charsize, pc, 0, 0);
	}
	else if (backcolor != NULL)
	{
		pte = SDL_CreateRGBSurface(0, (pbutt->re_butt).w, (pbutt->re_butt).h, 32, 0, 0, 0, 0);
		SDL_FillRect(pte, NULL, SDL_MapRGB(pte->format, backcolor[0], backcolor[1], backcolor[2]));
		*ppTex = SDL_Get_Font_Texture(rend, pte, pfont, TypeDupR(NULL, 3, charcolor[0], charcolor[1], charcolor[2]), charsize, pc, 0, 0);
	}
	else
	{
		SDL_Color color = { charcolor[0],charcolor[1],charcolor[2] };
		SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, pc, color);
		*ppTex = SDL_CreateTextureFromSurface(rend, pte);
	}
	SDL_FreeSurface(pte);
	pbutt->ppTex = ppTex;
	SDL_RenderCopy(rend, *ppTex, NULL, &(pbutt->re_butt));
	return pbutt;
}


bool SDL_Destroy_MyButton(SDL_MyButton* pbutt)
{
	SDL_DestroyTexture(*(pbutt->ppTex));
	free(pbutt->pc);
	if (pbutt->back1 != NULL)
		SDL_FreeSurface(pbutt->back1);
	if (pbutt->back2 != NULL)
		SDL_FreeSurface(pbutt->back2);
	if (pbutt->back3 != NULL)
		SDL_FreeSurface(pbutt->back3);
	free(pbutt);
	pbutt = NULL;
	return 1;
}

bool SDL_Destroy_MyButtons(SDL_MyButton* pbutt, ...)//End with a NULL
{
	va_list parg;
	va_start(parg, pbutt);
	for (SDL_MyButton* pte = NULL; (pte = va_arg(parg, SDL_MyButton*)) != NULL; )
		SDL_Destroy_MyButton(pte);
	va_end(parg);
	return 1;
}

SDL_MyButton* SDL_Move_MyButton(SDL_MyButton* pbutt, SDL_Renderer* rend, int x, int y)
{
	pbutt->re_butt.x = x;
	pbutt->re_butt.y = y;
	SDL_Rect re = { x,  y,  pbutt->re_butt.w,  pbutt->re_butt.h };
	SDL_RenderCopy(rend, *(pbutt->ppTex), NULL, &re);
	return pbutt;
}

SDL_MyButton* SDL_Rewrite_MyButton(SDL_MyButton* pbutt, SDL_Renderer* rend, int backcolor[3], const char* pc, TTF_Font* pfont, int charsize, int charcolor[3])
{
	free(pbutt->pc);
	pbutt->pc = _strdup(pc);
	if (backcolor != NULL)
		for (int a = 0; a < 3; pbutt->backcolor[a] = backcolor[a], pbutt->charcolor[a] = charcolor[a], a++);
	pbutt->pfont = pfont;
	pbutt->charsize = charsize;
	SDL_Surface* pte = NULL;
	SDL_DestroyTexture(*(pbutt->ppTex));
	if (pbutt->back1 != NULL)
	{
		pte = pbutt->back1;
	}
	else if (backcolor != NULL)
	{
		pte = SDL_CreateRGBSurface(0, pbutt->re_butt.w, pbutt->re_butt.h, 32, 0, 0, 0, 0);
		SDL_FillRect(pte, NULL, SDL_MapRGB(pte->format, backcolor[0], backcolor[1], backcolor[2]));
	}
	else
	{
		SDL_Color color = { charcolor[0],charcolor[1],charcolor[2] };
		SDL_Surface* pte = TTF_RenderUTF8_Blended(pfont, pc, color);
		*(pbutt->ppTex) = SDL_CreateTextureFromSurface(rend, pte);//SDL_printf_Texture(rend,0,0,100,100,0,0,0,*(pbutt->ppTex));
		SDL_RenderCopy(rend, *(pbutt->ppTex), NULL, &(pbutt->re_butt));
		return pbutt;
	}
	*(pbutt->ppTex) = SDL_Get_Font_Texture(rend, pte, pfont, TypeDupR(NULL, 3, charcolor[0], charcolor[1], charcolor[2]), charsize, pc, 0, 0);
	SDL_RenderCopy(rend, *(pbutt->ppTex), NULL, &(pbutt->re_butt));
	return pbutt;
}

SDL_MyButton* SDL_ChangeBack_MyButton(SDL_MyButton* pbutt, SDL_Renderer* rend, int backcolor[3], SDL_Surface** back)
{
	SDL_Surface* pte = NULL;
	SDL_DestroyTexture(*(pbutt->ppTex));
	if (back != NULL && *back != NULL)
	{
		pte = pbutt->back1 = *back;
		*(pbutt->ppTex) = SDL_Get_Font_Texture(rend, pte, pbutt->pfont, TypeDupR(NULL, 3, pbutt->charcolor[0], pbutt->charcolor[1], pbutt->charcolor[2]), pbutt->charsize, pbutt->pc, 0, 0);

	}
	else if (backcolor != NULL)
	{
		pte = SDL_CreateRGBSurface(0, pbutt->re_butt.w, pbutt->re_butt.h, 32, 0, 0, 0, 0);
		for (int a = 0; a < 3; pbutt->backcolor[a] = backcolor[a], a++);
		SDL_FillRect(pte, NULL, SDL_MapRGB(pte->format, pbutt->backcolor[0], pbutt->backcolor[1], pbutt->backcolor[2]));
		*(pbutt->ppTex) = SDL_Get_Font_Texture(rend, pte, pbutt->pfont, TypeDupR(NULL, 3, pbutt->charcolor[0], pbutt->charcolor[1], pbutt->charcolor[2]), pbutt->charsize, pbutt->pc, 0, 0);

	}
	else
	{
		for (int a = 0; a < 3; pbutt->backcolor[a] = 0, a++);
		SDL_Color color = { pbutt->charcolor[0],pbutt->charcolor[1],pbutt->charcolor[2] };
		SDL_Surface* pte = TTF_RenderUTF8_Blended(pbutt->pfont, pbutt->pc, color);
		*(pbutt->ppTex) = SDL_CreateTextureFromSurface(rend, pte);
	}
	SDL_RenderCopy(rend, *(pbutt->ppTex), NULL, &(pbutt->re_butt));
	return pbutt;
}

bool SDL_CheckOver_MyButton(SDL_Rect* re_butt, SDL_Event* peve)
{
	Sint32 mx = peve->motion.x;
	Sint32 my = peve->motion.y;
	if (mx > re_butt->x && mx<re_butt->x + re_butt->w && my>re_butt->y && my < re_butt->y + re_butt->h)
		return 1;
	return 0;
}

bool SDL_CheckPressOn_MyButton(SDL_Rect* re_butt, SDL_Event* peve)
{
	SDL_PollEvent(peve);
	if (peve->type == SDL_MOUSEBUTTONDOWN)
	{
		Sint32 mx = peve->motion.x;
		Sint32 my = peve->motion.y;
		if (mx > re_butt->x && mx<re_butt->x + re_butt->w && my>re_butt->y && my < re_butt->y + re_butt->h)
			return 1;
	}
	return 0;
}

int SDL_CheckClick_MyButton(SDL_Rect* re_butt, SDL_Event* peve)
{
	if (peve->type == SDL_MOUSEBUTTONDOWN)
	{
		Sint32 mx1 = peve->motion.x;
		Sint32 my1 = peve->motion.y;
		for (Sint32 mx2 = mx1, my2 = my1; (mx2 > re_butt->x && mx2<re_butt->x + re_butt->w && my2>re_butt->y && my2 < re_butt->y + re_butt->h); SDL_PollEvent(peve))
		{
			mx2 = peve->motion.x;
			my2 = peve->motion.y;
			if (peve->type == SDL_MOUSEBUTTONUP)
			{
				if (mx2 > re_butt->x && mx2<re_butt->x + re_butt->w && my2>re_butt->y && my2 < re_butt->y + re_butt->h)
				{
					if (peve->button.button == SDL_BUTTON_LEFT)
						return 1;
					else if (peve->button.button == SDL_BUTTON_RIGHT)
						return 2;
				}
			}
		}
		return 0;//鼠标有点击,但非本按钮
	}
	return 0;
}

MyTextInput* SDL_Create_MyTextInput(SDL_Renderer* rend, Uint32 Format, SDL_Rect re_text_in, int edgingwidth, int edgingwidthcolor[3], int backcolor[3], TTF_Font* pfont, int charsize, int charcolor[3], SDL_Surface* back)
{
	MyTextInput* ptext = MCALLOC(1, MyTextInput);
	if (Format == 0)
		Format = SDL_PIXELFORMAT_RGBA8888;//默认
	SDL_Texture** ppTex = MCALLOC(1, SDL_Texture*);//从栈内存跳出去的跳板@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	*ppTex = SDL_CreateTexture(rend, Format, SDL_TEXTUREACCESS_TARGET, 0, 0);
	SDL_DestroyTexture(*ppTex);
	ptext->ppTex = ppTex;
	if (re_text_in.h == 0)
		re_text_in.h = charsize + 2 * edgingwidth;
	ptext->re_text = re_text_in;
	for (int a = 0; a < 3; ptext->backcolor[a] = backcolor[a], ptext->charcolor[a] = charcolor[a], ptext->edgingwidthcolor[a] = edgingwidthcolor[a], a++);
	ptext->pfont = pfont;
	ptext->charsize = charsize;
	ptext->edgingwidth = edgingwidth;
	ptext->back1 = back;
	SDL_Rect Box = { edgingwidth,  edgingwidth,  re_text_in.w - 2 * edgingwidth,  re_text_in.h - 2 * edgingwidth };

	SDL_Surface* pte = SDL_CreateRGBSurface(0, (ptext->re_text).w, (ptext->re_text).h, 32, 0, 0, 0, 0);
	SDL_FillRect(pte, NULL, SDL_MapRGB(pte->format, edgingwidthcolor[0], edgingwidthcolor[1], edgingwidthcolor[2]));
	if (ppTex != NULL && *ppTex != NULL)
		SDL_DestroyTexture(*ppTex);
	if (back != NULL)
	{
		SDL_BlitScaled(back, NULL, pte, &Box);
		*ppTex = SDL_CreateTextureFromSurface(rend, back);
	}
	else
	{
		SDL_FillRect(pte, &Box, SDL_MapRGB(pte->format, backcolor[0], backcolor[1], backcolor[2]));
		*ppTex = SDL_CreateTextureFromSurface(rend, pte);
	}
	SDL_RenderCopy(rend, *ppTex, NULL, &re_text_in);
	return ptext;
}

MyTextInput* SDL_Input_MyTextInput(SDL_Renderer* rend, MyTextInput* ptext)
{//PPI(ptext->re_text.x);PPI(ptext->re_text.y);PPI(ptext->re_text.w);PPI(ptext->re_text.h);
	SDL_Event eve;
	time_t st = clock(), ed;
	bool KeyDowned = 0;
	Uint8 te = '\0';
	int cx = 0;//待输入字符偏移量
	int mc = ((ptext->re_text).w - 2 * ptext->edgingwidth) / (ptext->charsize);//最大显示量
	int gx = 0;//光标,相对于TextInput
	SDL_Rect Box = { ptext->edgingwidth,  ptext->edgingwidth,  ptext->re_text.w - 2 * (ptext->edgingwidth),  ptext->re_text.h - 2 * (ptext->edgingwidth) };
	if (ptext->pc != NULL)
		cx = strlen(ptext->pc);//待输入字符偏移量
	if (cx == 0)
	{
		free(ptext->pc);
		ptext->pc = MCALLOC(1, char);
		*(ptext->pc) = '\0';
	}
	SDL_Surface* sur = SDL_CreateRGBSurface(0, (ptext->re_text).w, (ptext->re_text).h, 32, 0, 0, 0, 0);
	do
	{
		SDL_PollEvent(&eve);
		if (eve.type == SDL_KEYDOWN)
		{
			if (KeyDowned == 1)
			{
				ed = clock();
				if ((double)((double)(ed - st) / CLOCKS_PER_SEC) >= 0.1)
				{
					st = clock();
					te = eve.key.keysym.sym;
					if ((te == SDLK_BACKSPACE) && (cx > 0))
					{
						--cx;
						*(ptext->pc + cx) = (char)' ';
						goto A;
					}
					if ((te >= (Uint8)' ') || (te <= (Uint8)'`'))
					{
						ptext->pc = (char*)realloc(ptext->pc, cx + 1);
						*(ptext->pc + (cx++)) = te;
					}
					if ((te == SDLK_RETURN) || (SDL_CheckClick_MyButton(&(ptext->re_text), &eve) == -1))
					{
						*(ptext->pc + cx) = '\0';
						return ptext;
					}
				A:;		    	SDL_FillRect(sur, &Box, SDL_MapRGB(sur->format, ptext->backcolor[0], ptext->backcolor[1], ptext->backcolor[2]));
					*(ptext->ppTex) = SDL_Get_Font_Texture(rend, sur, ptext->pfont, TypeDupR(NULL, 3, ptext->charcolor[0], ptext->charcolor[1], ptext->charcolor[2]), ptext->charsize, ptext->pc + (cx >= mc ? cx - mc : 0), ptext->edgingwidth, ptext->edgingwidth);
					SDL_RenderCopy(rend, *(ptext->ppTex), NULL, &(ptext->re_text));
					SDL_RenderPresent(rend);
				}
			}
			else if (KeyDowned == 0)
			{
				st = clock();
				KeyDowned = 1;
				te = eve.key.keysym.sym;
				if ((te == SDLK_BACKSPACE) && (cx > 0))
				{
					--cx;
					*(ptext->pc + cx) = (char)' ';
					goto B;
				}
				if ((te >= (Uint8)' ') || (te <= (Uint8)'`'))
				{
					ptext->pc = (char*)realloc(ptext->pc, cx + 1);
					*(ptext->pc + (cx++)) = te;
					*(ptext->pc + cx) = '\0';
				}
				if ((te == SDLK_RETURN) || (SDL_CheckClick_MyButton(&(ptext->re_text), &eve) == -1))
				{
					*(ptext->pc + cx - 1) = '\0';
					return ptext;
				}
			B:;		    	SDL_FillRect(sur, &Box, SDL_MapRGB(sur->format, ptext->backcolor[0], ptext->backcolor[1], ptext->backcolor[2]));
				*(ptext->ppTex) = SDL_Get_Font_Texture(rend, sur, ptext->pfont, TypeDupR(NULL, 3, ptext->charcolor[0], ptext->charcolor[1], ptext->charcolor[2]), ptext->charsize, ptext->pc + (cx >= mc ? cx - mc : 0), ptext->edgingwidth, ptext->edgingwidth);
				gx = (cx >= mc ? mc : cx) * (ptext->charsize) + ptext->edgingwidth;
				SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
				SDL_RenderDrawLine(rend, gx + ptext->re_text.x, ptext->re_text.y, gx + ptext->re_text.x, ptext->re_text.y + ptext->re_text.h);
				SDL_RenderCopy(rend, *(ptext->ppTex), NULL, &(ptext->re_text));
				SDL_RenderPresent(rend);
			}
		}
		else if (eve.type == SDL_KEYUP)
			KeyDowned = 0;
		if (eve.type == SDL_MOUSEBUTTONDOWN)
		{
		}
	} while (1);
}

MyTextInput* SDL_Move_MyTextInput(MyTextInput* ptext, SDL_Renderer* rend, int x, int y)
{
	ptext->re_text.x = x;
	ptext->re_text.y = y;
	SDL_Rect re = { x,  y,  ptext->re_text.w,  ptext->re_text.h };
	SDL_RenderCopy(rend, *(ptext->ppTex), NULL, &re);
	return ptext;
}

MyTextInput* SDL_Rewrite_MyTextInput(MyTextInput* ptext, SDL_Renderer* rend, int backcolor[3], char* pc, TTF_Font* pfont, int charsize, int charcolor[3])
{
	for (int a = 0; a < 3; ptext->backcolor[a] = backcolor[a], ptext->charcolor[a] = charcolor[a], a++);
	ptext->pfont = pfont;
	ptext->charsize = charsize;
	SDL_Surface* pte = NULL;
	SDL_DestroyTexture(*(ptext->ppTex));
	if (ptext->back1 != NULL)
	{
		pte = ptext->back1;
		*(ptext->ppTex) = SDL_CreateTextureFromSurface(rend, ptext->back1);
	}
	else
	{
		pte = SDL_CreateRGBSurface(0, ptext->re_text.w, ptext->re_text.h, 32, 0, 0, 0, 0);
		SDL_FillRect(pte, NULL, SDL_MapRGB(pte->format, backcolor[0], backcolor[1], backcolor[2]));
	}
	*(ptext->ppTex) = SDL_Get_Font_Texture(rend, pte, pfont, TypeDupR(NULL, 3, charcolor[0], charcolor[1], charcolor[2]), charsize, pc, 0, 0);
	SDL_RenderCopy(rend, *(ptext->ppTex), NULL, &(ptext->re_text));
	return ptext;
}

MyTextInput* SDL_ChangeBack_MyTextInput(MyTextInput* ptext, SDL_Renderer* rend, int edgingwidth, int edgingwidthcolor[3], int backcolor[3], SDL_Surface* back)
{
	SDL_DestroyTexture(*(ptext->ppTex));
	SDL_Rect Box = { edgingwidth,  edgingwidth,  (ptext->re_text).w - 2 * edgingwidth,  (ptext->re_text).h - 2 * edgingwidth };
	SDL_Surface* pte = SDL_CreateRGBSurface(0, (ptext->re_text).w, (ptext->re_text).h, 32, 0, 0, 0, 0);
	SDL_FillRect(pte, NULL, SDL_MapRGB(pte->format, edgingwidthcolor[0], edgingwidthcolor[1], edgingwidthcolor[2]));
	if (back != NULL)
	{
		ptext->back1 = back;
		SDL_BlitScaled(back, NULL, pte, &Box);
		*(ptext->ppTex) = SDL_CreateTextureFromSurface(rend, pte);
	}
	else
	{
		pte = SDL_CreateRGBSurface(0, ptext->re_text.w, ptext->re_text.h, 32, 0, 0, 0, 0);
		for (int a = 0; a < 3; ptext->backcolor[a] = backcolor[a], ptext->edgingwidthcolor[a] = edgingwidthcolor[a], a++);
		SDL_FillRect(pte, &Box, SDL_MapRGB(pte->format, ptext->backcolor[0], ptext->backcolor[1], ptext->backcolor[2]));
		*(ptext->ppTex) = SDL_CreateTextureFromSurface(rend, pte);
	}
	SDL_RenderCopy(rend, *(ptext->ppTex), NULL, &(ptext->re_text));
	return ptext;
}

//End With a NULL
bool SDL_Quit_s(SDL_Window** win, SDL_Renderer** renderer, SDL_Surface** ppSurface1, ...)
{
	va_list parg;
	va_start(parg, ppSurface1);
	for (SDL_Surface** pte = NULL; (pte = va_arg(parg, SDL_Surface**)) != NULL; )
		SDL_FreeSurface(*pte);
	va_end(parg);
	if (renderer != NULL)
		SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*win);
	va_end(parg);
	return 1;
}

//flag==0 means defualt
//color==NULL means translucent BG
MyUI* MyUI_Init(const char* titlepc, int winw, int winh, int winflags, int* color)
{
	MyUI* pui = MCALLOC(1, MyUI);
	pui->win = MCALLOC(1, MyUI_win);
	pui->win->titlepc = _strdup(titlepc);
	pui->win->peve = MCALLOC(1, SDL_Event);
	pui->win->winw = winw;
	pui->win->winh = winh;
	pui->win->pre_win = MCALLOC(1, SDL_Rect);
	pui->win->pre_win->w = winw;
	pui->win->pre_win->h = winh;
	pui->win->pre_win->x = pui->win->pre_win->y = 0;
	pui->win->FPS = 25.f;
	pui->win->exitbutt = -1;
	pui->win->titlebutt = -1;
	pui->font = MCALLOC(1, MyUI_font);
	pui->butt = MCALLOC(1, MyUI_butt);
	pui->butt->sumbutt = 1024;
	pui->butt->buttava = MCALLOC(pui->butt->sumbutt, int);
	pui->butt->eventbutt = MCALLOC(pui->butt->sumbutt, int);

	pui->otherTex = List_Init();
	pui->otherTexRe = List_Init();

	pui->pF_Addfont = MyUI_Addfont;
	pui->pF_Addbutt = MyUI_Addbutt;
	pui->pF_Delbutt = MyUI_Delbutt;
	pui->pF_Checkbutt = MyUI_Checkbutt;
	pui->pF_Checktitle = MyUI_Checktitle;
	pui->pF_Update = MyUI_Update;
	pui->pF_PollQuit = MyUI_PollQuit;
	int img_f = IMG_INIT_JPG;// | IMG_INIT_PNG;
	if ((SDL_Init(SDL_INIT_EVERYTHING) == -1) || (TTF_Init() == -1) || (IMG_Init(img_f) != (img_f)))/*|| Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID)==0)*/
		return (MyUI*)MyBA_Err("MyUI_Init: Can't Init SDL2", 1);
	if (winflags == 0)
		winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE;
	else if (winflags == 1)
		winflags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	pui->win->pwin = SDL_CreateWindow(pui->win->titlepc, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pui->win->winw, pui->win->winh, winflags);
	if (pui->win->pwin == NULL)
		return (MyUI*)MyBA_Err("MyUI_Init: Can't create win", 1);
	pui->win->rend = SDL_CreateRenderer(pui->win->pwin, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(pui->win->rend);
	SDL_SetRenderDrawColor(pui->win->rend, 200, 200, 200, 200);

	if (color != NULL)
	{
		pui->win->color[0] = color[0];
		pui->win->color[1] = color[1];
		pui->win->color[2] = color[2];
		pui->win->pwinSur = SDL_GetWindowSurface(pui->win->pwin);
		if (pui->win->pwinSur == NULL)
			return (MyUI*)MyBA_Err("MyUI_Init: Can't create sur", 1);
		pui->win->format = pui->win->pwinSur->format;
		SDL_FillRect(pui->win->pwinSur, NULL, SDL_MapRGB(pui->win->format, pui->win->color[0], pui->win->color[1], pui->win->color[2]));

	}
	else
	{
		SDL_VERSION(&(pui->win->info.version));
		if (SDL_GetWindowWMInfo(pui->win->pwin, &(pui->win->info)))
			pui->win->hwnd = pui->win->info.info.win.window;
		/*设置窗口colorkey*/
		SetWindowLong(pui->win->hwnd, GWL_EXSTYLE, GetWindowLong(pui->win->hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(pui->win->hwnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
		/*设置窗口为悬浮窗 */
		SetWindowPos(pui->win->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		pui->win->pwinSur = SDL_GetWindowSurface(pui->win->pwin);
		UINT32 keyColor = SDL_MapRGB(pui->win->pwinSur->format, 255, 255, 255);
		SDL_SetSurfaceBlendMode(pui->win->pwinSur, SDL_BLENDMODE_NONE);
		SDL_FillRect(pui->win->pwinSur, NULL, keyColor);
	}

	pui->win->pwinTex = SDL_CreateTextureFromSurface(pui->win->rend, pui->win->pwinSur);
	if (pui->win->pwinTex == NULL)
		return (MyUI*)MyBA_Err("MyUI_Init: Can't create Tex", 1);
	SDL_RenderCopy(pui->win->rend, pui->win->pwinTex, NULL, NULL);
	pui->font->pdefaultfont = TTF_OpenFont("C:\\Windows\\Fonts\\simkai.ttf", 128);
	if (pui->font->pdefaultfont == NULL)
		return (MyUI*)MyBA_Err("MyUI_Init: Can't open defaultfont:C:\\Windows\\Fonts\\simkai.ttf", 1);
	SDL_RenderPresent(pui->win->rend);
	pui->win->time = clock();

	MyBA_AtQuit(MyUI_Quit, (void*)pui);

	return pui;
}

MyUI* MyUI_Addfont(MyUI* pui, int order, const char* ppath)
{
	return pui;
}

//back: 0x1 means to use MyUI_ColorSur
MyUI* MyUI_Addbutt(MyUI* pui, int order, const char* name, int charsize, int charcolor[3], int backcolor[3], int xywh[4], SDL_Surface* back)
{
	if (order < (pui->butt->sumbutt))
	{
		bool iscolor = 0;
		SDL_Rect re = { xywh[0],  xywh[1],  xywh[2],  xywh[3] };
		if (back == (SDL_Surface*)(0x1))//Use MyUI_ColorSur
		{
			iscolor = 1;
			if (re.w < 0 || re.h < 0)//用户此行为,则要求自行根据字符串大小计算按钮大小
			{
				if (charsize == 0 || strlen(name) == 0)
					return NULL;
				if (re.w < 0)
					re.w = strlen(name) * charsize;
				if (re.h < 0)
					re.h = charsize;
			}
			back = SDL_CreateRGBSurface(0, re.w, re.h, 32, 0, 0, 0, 255);
		}
		(pui->butt->pbutt)[order] = SDL_Create_MyButton(pui->win->rend, 0, re, backcolor, name, pui->font->pdefaultfont, charsize, charcolor, back);
		if (iscolor)
			((pui->butt->pbutt)[order])->pct = MyUI_ColorSur_Init(((pui->butt->pbutt)[order])->back1);
		(pui->butt->buttava)[order] = 1;
		SDL_RenderPresent(pui->win->rend);
	}
	else
	{
		return (MyUI*)MyBA_Err("MyUI_Addbutt: order is out of range", 1);
	}
	return pui;
}

MyUI* MyUI_Delbutt(MyUI* pui, int order)
{
	if (pui == NULL)
		return (MyUI*)MyBA_Err("MyUI_Delbutt: pui==NULL, return NULL", 1);
	if ((order > (pui->butt->sumbutt)) || (order < (pui->butt->sumbutt) && pui->butt->pbutt[order] == NULL))
		return (MyUI*)MyBA_Err("MyUI_Delbutt: the butt asked to del is not exist, return NULL", 0);
	SDL_Destroy_MyButton(pui->butt->pbutt[order]);
	pui->butt->pbutt[order] = NULL;
	if (order == pui->win->exitbutt)
		pui->win->exitbutt = -1;
	if (order == pui->win->titlebutt)
		pui->win->titlebutt = -1;
	pui->butt->buttava[order] = 0;
	pui->butt->eventbutt[order] = 0;
	return pui;
}

MyUI* MyUI_Checkbutt(MyUI* pui)
{
	SDL_PollEvent(pui->win->peve);
	clock_t st = clock();
	/*	for(int a = 0;a<pui->butt->sumbutt;a++)
			if((pui->butt->buttava)[a] == 1)
				pui->butt->eventbutt[a] = SDL_CheckClick_MyButton(&((pui->butt->pbutt[a])->re_butt),pui->win->peve);*/
	if (pui->win->peve->type == SDL_MOUSEBUTTONDOWN)
	{
		for (SDL_PollEvent(pui->win->peve); pui->win->peve->type != SDL_MOUSEBUTTONUP; SDL_PollEvent(pui->win->peve), SDL_Delay(5))
		{
			if ((float)(clock() - st) / CLOCKS_PER_SEC > 0.2)
				return pui->pF_Checktitle(pui);
		}
		Sint32 mx = pui->win->peve->motion.x;
		Sint32 my = pui->win->peve->motion.y;
		int x, y, w, h;
		for (int a = 0; a < pui->butt->sumbutt; a++)
		{
			if ((pui->butt->buttava)[a] == 1)
			{
				pui->butt->eventbutt[a] = 0;//清空
				x = ((pui->butt->pbutt[a])->re_butt).x;
				y = ((pui->butt->pbutt[a])->re_butt).y;
				w = ((pui->butt->pbutt[a])->re_butt).w;
				h = ((pui->butt->pbutt[a])->re_butt).h;
				if (mx > x && mx<x + w && my>y && my < y + h)
				{
					if (pui->win->peve->button.button == SDL_BUTTON_LEFT)
					{//PPI(a);
						pui->butt->eventbutt[a] = 1;
					}
					else if (pui->win->peve->button.button == SDL_BUTTON_RIGHT)
					{
						pui->butt->eventbutt[a] = 2;
					}
				}
			}
		}
	}
	return pui;
}

MyUI* MyUI_Checktitle(MyUI* pui)
{
	SDL_Rect* pre = pui->win->pre_title;
	if ((pre == NULL) && (pui->win->titlebutt != -1))
		pre = &(pui->butt->pbutt[pui->win->titlebutt]->re_butt);
	Sint32 wx = 0, wy = 0, bx = 0, by = 0;
	if ((pre != NULL) && (SDL_CheckPressOn_MyButton(pre, pui->win->peve)))
	{
		SDL_GetWindowPosition(pui->win->pwin, &wx, &wy);
		bx = pui->win->peve->button.x;
		by = pui->win->peve->button.y;
		for (Sint32 x, y = SDL_PollEvent(pui->win->peve); pui->win->peve->type != SDL_MOUSEBUTTONUP; )
		{
			SDL_PollEvent(pui->win->peve);
			x = pui->win->peve->button.x;
			y = pui->win->peve->button.y;
			if (pui->win->peve->type == SDL_MOUSEMOTION && x != 0 && y != 0)
			{
				SDL_SetWindowPosition(pui->win->pwin, wx + x - bx, wy + y - by);
				wx = wx + x - bx;
				wy = wy + y - by;
			}
		}
	}
	return pui;
}

MyUI* MyUI_Update(MyUI* pui, bool rendclear, bool copyTex)
{
	for (float waitt = 1. / pui->win->FPS; (float)((float)clock() - pui->win->time) / CLOCKS_PER_SEC < waitt; SDL_Delay(10));
	pui->win->time = clock();
	if (rendclear)
		SDL_RenderClear(pui->win->rend);
	if (copyTex)
		SDL_RenderCopy(pui->win->rend, pui->win->pwinTex, NULL, NULL);
	for (int a = 0; a < pui->butt->sumbutt; a++)
	{
		if ((pui->butt->buttava)[a] == 1)
		{
			if (((pui->butt->pbutt)[a])->pct != NULL)
			{
				MyUI_ColorSur_Update(((pui->butt->pbutt)[a])->pct);
				SDL_DestroyTexture(*(((pui->butt->pbutt)[a])->ppTex));
				*(((pui->butt->pbutt)[a])->ppTex) = SDL_Get_Font_Texture(pui->win->rend, ((pui->butt->pbutt)[a])->back1, ((pui->butt->pbutt)[a])->pfont, ((pui->butt->pbutt)[a])->charcolor, ((pui->butt->pbutt)[a])->charsize, ((pui->butt->pbutt)[a])->pc, 0, 0);
			}
			SDL_RenderCopy(pui->win->rend, *(((pui->butt->pbutt)[a])->ppTex), NULL, &(((pui->butt->pbutt)[a])->re_butt));
		}
	}
	SDL_Rect* pre = (SDL_Rect*)List_Copy(pui->otherTexRe);
	for (SDL_Texture* pTex = (SDL_Texture*)List_Copy(pui->otherTex); pTex != NULL; pTex = (SDL_Texture*)List_Copy(pui->otherTex))
	{
		SDL_RenderCopy(pui->win->rend, pTex, NULL, pre);
		pre = (SDL_Rect*)List_Copy(pui->otherTexRe);
	}

	SDL_RenderPresent(pui->win->rend);
	return pui;
}

//do not get quit sig return 0
bool MyUI_PollQuit(MyUI* pui)
{
	pui->pF_Checkbutt(pui);
	if ((pui->win->exitbutt != -1) && ((pui->butt->eventbutt)[pui->win->exitbutt] == 1))
		return 1;
	return SDL_Poll_Quit(pui->win->peve);
}

int MyUI_Quit(void* pui_, int code, ...)
{
	MyUI* pui = (MyUI*)pui_;

	free(pui->win->titlepc);
	SDL_FreeSurface(pui->win->pwinSur);
	SDL_DestroyTexture(pui->win->pwinTex);
	SDL_DestroyWindow(pui->win->pwin);
	//free(pui->win->pre_win);
	SDL_DestroyRenderer(pui->win->rend);
	SDL_FreeFormat(pui->win->format);
	free(pui->win->peve);
	free(pui->win->pre_title);

	free(pui->win);
	free(pui->font);
	free(pui->set);

	return 0;
}

MyUI_ColorSur* MyUI_ColorSur_Init(SDL_Surface* distSur)
{
	MCALLOCS(MyUI_ColorSur, pcs, 1);
	pcs->pre = MCALLOC(1, SDL_Rect);
	pcs->pre->x = pcs->pre->y = 0;
	pcs->pre->w = distSur->w;
	pcs->pre->h = distSur->h;
	pcs->sumdot = 4;
	pcs->pdot = MCALLOC(pcs->sumdot, MyUI_ColorSur_dot);
	pcs->plv = MCALLOC(pcs->sumdot, float);
	pcs->plen = MCALLOC(pcs->sumdot, float);
	for (size_t i = 0; i < pcs->sumdot; i++)
	{
		pcs->pdot[i].b = 500.0 / (float)(1.0 + rand() % 200);
		pcs->pdot[i].x = 1 + rand() % (pcs->pre->w - 1);
		pcs->pdot[i].y = 1 + rand() % (pcs->pre->h - 1);//PPU(pdot[xlh].x);PPU(pdot[xlh].y);
		pcs->pdot[i].fx = pcs->pdot[i].fy = 1.0;
		ProduceRainbowCol(pcs->pdot[i].color, &(pcs->pdot[i].b));
	}
	pcs->distSur = distSur;
	pcs->re_paint.w = pcs->re_paint.h = 1;

	pcs->pF_Update = MyUI_ColorSur_Update;
	return pcs;
}

void MyUI_ColorSur_CacuCol(MyUI_ColorSur* pcs)
{
	float sumlen = 0.f;
	float sumlength = 0.f;
	long dx = 0;
	long dy = 0;
	long j = 0;
	float k = 0;
	float k2 = 0;
	for (long i = 0; i < pcs->sumdot; i++)
	{
		dx = pcs->re_paint.x - pcs->pdot[i].x;
		dy = pcs->re_paint.y - pcs->pdot[i].y;
		k = (float)(dx * dx + dy * dy);
		if (k <= 0)
			k = -k + 1;
		j = *(long*)&k;                        // evil floating point bit level hacking
		j = 0x5f3759df - (j >> 1);             // what the fuck? 
		k2 = *(float*)&j;
		pcs->plen[i] = k2 * k2 * (1.5f - (0.5 * k * k2 * k2));//originaly is k2 * ( 1.5f - ( 0.5*k * k2 * k2 ) ) but to use plen[i] = plen[i]*plen[i];
		sumlen += pcs->plen[i];
	}
	for (long i = 0; i < pcs->sumdot; i++)
		pcs->plv[i] = pcs->plen[i] / sumlen;
	pcs->col[0] = pcs->plv[0] * pcs->pdot[0].color[0] + pcs->plv[1] * pcs->pdot[1].color[0] + pcs->plv[2] * pcs->pdot[2].color[0] + pcs->plv[3] * pcs->pdot[3].color[0];
	pcs->col[1] = pcs->plv[0] * pcs->pdot[0].color[1] + pcs->plv[1] * pcs->pdot[1].color[1] + pcs->plv[2] * pcs->pdot[2].color[1] + pcs->plv[3] * pcs->pdot[3].color[1];
	pcs->col[2] = pcs->plv[0] * pcs->pdot[0].color[2] + pcs->plv[1] * pcs->pdot[1].color[2] + pcs->plv[2] * pcs->pdot[2].color[2] + pcs->plv[3] * pcs->pdot[3].color[2];
	pcs->col[3] = 255. * sumlen;
}

MyUI_ColorSur* MyUI_ColorSur_Update(MyUI_ColorSur* pcs)
{
	if (pcs->distSur == NULL)
		return (MyUI_ColorSur*)MyBA_Err("MyUI_ColorSur_Update: distSur == NULL, return NULL", 1);
	for (long a = 0; a < pcs->sumdot; a++)
	{
		pcs->pdot[a].b += 0.05f;
		ProduceRainbowCol(pcs->pdot[a].color, &(pcs->pdot[a].b));
		pcs->pdot[a].x += 2.0f * (pcs->pdot[a].fx);//PPD((pdot[a].fx)*cos(0.01*(pdot[a].b)));
		if (pcs->pdot[a].x < 1)
		{
			pcs->pdot[a].fx = -pcs->pdot[a].fx;
			pcs->pdot[a].x = 1;
		}
		if (pcs->pdot[a].x > pcs->pre->w)
		{
			pcs->pdot[a].fx = -pcs->pdot[a].fx;
			pcs->pdot[a].x = pcs->pre->w - 1;
		}
		pcs->pdot[a].y += 2.0f * (pcs->pdot[a].fy);//PPD((pcs->pdot[a].fy)*sin(0.01*(pcs->pdot[a].b)));
		if (pcs->pdot[a].y < 1)
		{
			pcs->pdot[a].fy = -pcs->pdot[a].fy;
			pcs->pdot[a].y = 1;
		}
		if (pcs->pdot[a].y > pcs->pre->h)
		{
			pcs->pdot[a].fy = -pcs->pdot[a].fy;
			pcs->pdot[a].y = pcs->pre->h - 1;
		}
	}

	for (pcs->re_paint.y = 0; (pcs->re_paint.y) < (pcs->pre->h); (pcs->re_paint.y)++)
	{
		for (pcs->re_paint.x = 0; (pcs->re_paint.x) < (pcs->pre->w); (pcs->re_paint.x)++)
		{
			MyUI_ColorSur_CacuCol(pcs);
			SDL_FillRect(pcs->distSur, &(pcs->re_paint), SDL_MapRGBA(pcs->distSur->format, pcs->col[0], pcs->col[1], pcs->col[2], pcs->col[3]));
		}
	}

	return pcs;
}

MyUI_ColorText* MyUI_ColorText_Init(SDL_Renderer* rend, TTF_Font* font, const char* pc)
//为什么直接从fontSur中读取像素是失败的？？？？(get 255 0 0 0)
{
	MCALLOCS(MyUI_ColorText, pct, 1);
	pct->mem = List_Init();
	pct->font = font;
	pct->rend = rend;
	pct->pre = BALLOC_R(1, SDL_Rect, pct->mem);
	pct->pc = _strdup(pc);
	SDL_Color color = { .r = 255 , .g = 0 , .b = 0, .a = 255 };
	pct->fontSur = TTF_RenderUTF8_Blended(pct->font, pct->pc, color);
	if (pct->fontSur == NULL)
		return (MyUI_ColorText*)MyBA_Errs(0, "MyUI_ColorText_Init: Can't blended Surface with text:", pc, ", skip", NULL);
	pct->pre->w = pct->fontSur->w;
	pct->pre->h = pct->fontSur->h;
	pct->pSur = SDL_CreateRGBSurface(0, pct->pre->w, pct->pre->h, 32, 0, 0, 0, 255);
	SDL_SetColorKey(pct->pSur, 1, SDL_MapRGB(pct->pSur->format, 0, 0, 0));
	SDL_BlitSurface(pct->fontSur, NULL, pct->pSur, NULL);

	pct->sumdot = 4;
	pct->pdot = BALLOC_R(pct->sumdot, MyUI_ColorSur_dot, pct->mem);
	pct->plv = BALLOC_R(pct->sumdot, float, pct->mem);
	pct->plen = BALLOC_R(pct->sumdot, float, pct->mem);
	for (size_t i = 0; i < pct->sumdot; i++)
	{
		pct->pdot[i].b = 500.0 / (float)(1.0 + rand() % 200);
		pct->pdot[i].x = 1 + rand() % (pct->pre->w - 1);
		pct->pdot[i].y = 1 + rand() % (pct->pre->h - 1);//PPU(pdot[xlh].x);PPU(pdot[xlh].y);
		pct->pdot[i].fx = pct->pdot[i].fy = 1.0;
		ProduceRainbowCol(pct->pdot[i].color, &(pct->pdot[i].b));
	}
	pct->re_paint.w = pct->re_paint.h = 1;
	Uint8 r, g, b;
	pct->ppb = BALLOC_R(pct->pre->h, bool*, pct->mem);
	SDL_LockSurface(pct->pSur);
	for (pct->re_paint.y = 0; (pct->re_paint.y) < (pct->pre->h); (pct->re_paint.y)++)
	{
		pct->ppb[pct->re_paint.y] = BALLOC_R(pct->pre->w, bool, pct->mem);
		for (pct->re_paint.x = 0; (pct->re_paint.x) < (pct->pre->w); (pct->re_paint.x)++)
		{
			SDL_GetRGB(SDL_GetPixel(pct->pSur, pct->re_paint.x, pct->re_paint.y), pct->pSur->format, &r, &g, &b);
			if (r == 255)
				pct->ppb[pct->re_paint.y][pct->re_paint.x] = 1;
			//printf("%5d %5d | %3d  %3d  %3d\n", pct->re_paint.y, pct->re_paint.x,r, g, b);
		}
	}
	//unsigned char* p = (unsigned char*)(pct->fontSur->pixels);
	//for (_ULL i = 0; i < (pct->pre->h) * (pct->pre->w) * 4; i++, p++)
	//	printf("%u\n", *p);
	SDL_UnlockSurface(pct->pSur);
	return pct;
}

void MyUI_ColorText_CacuCol(MyUI_ColorText* pcs)
{
	float sumlen = 0.f;
	float sumlength = 0.f;
	long dx = 0;
	long dy = 0;
	long j = 0;
	float k = 0;
	float k2 = 0;
	for (long i = 0; i < pcs->sumdot; i++)
	{
		dx = pcs->re_paint.x - pcs->pdot[i].x;
		dy = pcs->re_paint.y - pcs->pdot[i].y;
		k = (float)(dx * dx + dy * dy);
		if (k <= 0)
			k = -k + 1;
		j = *(long*)&k;                        // evil floating point bit level hacking
		j = 0x5f3759df - (j >> 1);             // what the fuck? 
		k2 = *(float*)&j;
		pcs->plen[i] = k2 * k2 * (1.5f - (0.5 * k * k2 * k2));//originaly is k2 * ( 1.5f - ( 0.5*k * k2 * k2 ) ) but to use plen[i] = plen[i]*plen[i];
		sumlen += pcs->plen[i];
	}
	for (long i = 0; i < pcs->sumdot; i++)
		pcs->plv[i] = pcs->plen[i] / sumlen;
	pcs->col[0] = pcs->plv[0] * pcs->pdot[0].color[0] + pcs->plv[1] * pcs->pdot[1].color[0] + pcs->plv[2] * pcs->pdot[2].color[0] + pcs->plv[3] * pcs->pdot[3].color[0];
	pcs->col[1] = pcs->plv[0] * pcs->pdot[0].color[1] + pcs->plv[1] * pcs->pdot[1].color[1] + pcs->plv[2] * pcs->pdot[2].color[1] + pcs->plv[3] * pcs->pdot[3].color[1];
	pcs->col[2] = pcs->plv[0] * pcs->pdot[0].color[2] + pcs->plv[1] * pcs->pdot[1].color[2] + pcs->plv[2] * pcs->pdot[2].color[2] + pcs->plv[3] * pcs->pdot[3].color[2];
	pcs->col[3] = 255. * sumlen;
}

SDL_Surface* MyUI_ColorText_Get(MyUI_ColorText* pct)
{
	for (long a = 0; a < pct->sumdot; a++)
	{
		pct->pdot[a].b += 0.05f;
		ProduceRainbowCol(pct->pdot[a].color, &(pct->pdot[a].b));
		pct->pdot[a].x += 2.0f * (pct->pdot[a].fx);//PPD((pdot[a].fx)*cos(0.01*(pdot[a].b)));
		if (pct->pdot[a].x < 1)
		{
			pct->pdot[a].fx = -pct->pdot[a].fx;
			pct->pdot[a].x = 1;
		}
		if (pct->pdot[a].x > pct->pre->w)
		{
			pct->pdot[a].fx = -pct->pdot[a].fx;
			pct->pdot[a].x = pct->pre->w - 1;
		}
		pct->pdot[a].y += 2.0f * (pct->pdot[a].fy);//PPD((pct->pdot[a].fy)*sin(0.01*(pct->pdot[a].b)));
		if (pct->pdot[a].y < 1)
		{
			pct->pdot[a].fy = -pct->pdot[a].fy;
			pct->pdot[a].y = 1;
		}
		if (pct->pdot[a].y > pct->pre->h)
		{
			pct->pdot[a].fy = -pct->pdot[a].fy;
			pct->pdot[a].y = pct->pre->h - 1;
		}
	}
	for (pct->re_paint.y = 0; (pct->re_paint.y) < (pct->pre->h); (pct->re_paint.y)++)
	{
		for (pct->re_paint.x = 0; (pct->re_paint.x) < (pct->pre->w); (pct->re_paint.x)++)
		{
			if (pct->ppb[pct->re_paint.y][pct->re_paint.x])
			{
				MyUI_ColorText_CacuCol(pct);
				SDL_FillRect(pct->pSur, &(pct->re_paint), SDL_MapRGB(pct->fontSur->format, pct->col[0], pct->col[1], pct->col[2]));
				//PPIs(3, pct->col[0], pct->col[1], pct->col[2], pct->col[3]);
			}
		}
	}
	return pct->pSur;
}
bool MyUI_ColorText_Destroy(MyUI_ColorText* pct)
{
	MyBA_Free_R(pct->mem);
	SDL_FreeSurface(pct->fontSur);
	SDL_FreeSurface(pct->pSur);
	free(pct);
	return true;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*********************************************SDL2*****************************************************************End
