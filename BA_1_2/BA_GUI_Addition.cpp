#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_String.hpp"

#include"BA_GUI.hpp"
#include"BA_GUI_Addition.hpp"

ba::ui::listView::listView(window* _win, SDL_Rect pos, SDL_Color bgc,
	std::deque< rect*> _items)
	: rect(pos, bgc)
{
	win = _win;
	re = pos;
	rendRect();
	if( !_items.empty())
		gen(_items);
}
void ba::ui::listView::gen(std::deque<rect*> _items)
{
	for (rect* item : _items)
		gen1(item, items.size() == _items.size() - 1);
}
void ba::ui::listView::gen1(rect* item, bool isEnd)
{
	items.emplace_back(item);
	events.emplace_back(0);
	statue.emplace_back(1);
	item->re.y = sumHeight;
	eachHeight.emplace_back(item->re.h);
	pixel2idx.insert(pixel2idx.end(), item->re.h, items.size() - 1);
	sumHeight += item->re.h;
	if (isEnd)
	{
		visPixelRange[1] = min(re.h, sumHeight) - 1;
		refreshTex = true;
	}
}
void ba::ui::listView::clear(void)
{
	if(! items.empty())
	{
		sumHeight = visPixelRange[0] = visPixelRange[1] = 0;
		eachHeight.clear();
		pixel2idx.clear();
		synListView.clear();
		refreshTex = true;
		clickIdx = -1;
		for (auto item : items)
			delete item;
		items.clear();
		events.clear();
		statue.clear();
	}
}
SDL_Texture* ba::ui::listView::getTex(void)
{
	if (!refreshTex || pixel2idx.size()==0)
		return tex;
	refreshTex = false;
	if (tex)
		SDL_DestroyTexture(tex);
	_LL firstVisItemIdx = pixel2idx[visPixelRange[0]];
	_LL lastVisItemIdx = pixel2idx[visPixelRange[1]];
	_LL firstVisItemVisY = items[firstVisItemIdx]->re.y - visPixelRange[0];//<=0
	SDL_Rect reTmp = { 0, firstVisItemVisY, 0, 0 };
	SDL_Surface* surTmp = SDL_CreateRGBASurface(re.w, re.h);
	SDL_BlitScaled(sur, NULL, surTmp, NULL);//will make nage y turn 0 !!!
	for (int i = firstVisItemIdx; i <= lastVisItemIdx; i++)
	{
		reTmp.w = items[i]->re.w;
		reTmp.h = items[i]->re.h;
		SDL_BlitScaled(items[i]->sur, NULL, surTmp, &reTmp);//will make nage y turn 0 !!!
		if (i == firstVisItemIdx)
			reTmp.y = firstVisItemVisY + eachHeight[i];
		else
			reTmp.y += eachHeight[i];
	}
	tex = SDL_CreateTextureFromSurface(win->rend, surTmp);
	SDL_FreeSurface(surTmp);
	return tex;
}
_LL ba::ui::_listView_Data_ApplyDy(_LL dy, listView* list)
{
	if (dy > 0 && list->visPixelRange[0] > 0)//scroll up
		dy = -min(dy, list->visPixelRange[0]);//请求上滑距离和剩余可上滑距离的最小者
	else if (dy < 0 && list->visPixelRange[1] < list->sumHeight - 1)//scroll down
		dy = min(-dy, list->sumHeight - list->visPixelRange[1] - 1);//请求下滑距离和剩余可下滑距离的最小者
	else
		return 0;
	list->visPixelRange[0] += dy;
	list->visPixelRange[1] += dy;
	list->refreshTex = true;
	return dy;
}

int ba::ui::_listView_check(window* _win, void* _self, int mouseEveCode, void* _pData)
{
	int retMouseEveCode = 0;
	ba::ui::listView* self = (ba::ui::listView*)_self;
	// scroll
	_LL dy = _win->winState->getVar((_LL)0, [&]() {
		Sint32 dy = 0;
		if ( !_win->winState->wheelY.empty())
		{
			if(SDL_GetTicks() - _win->winState->wheelY.front().second < 300)
				dy = _win->winState->wheelY.front().first;
			_win->winState->wheelY.pop_front();
		}
		if (!_win->winState->wheelY.empty())
			retMouseEveCode = 4;
		else
			retMouseEveCode = 0;
		return dy*(_win->winState->wheelY.size()+1)*4; });//放大
	_listView_Data_ApplyDy(dy, self);
	for (listView* pD : self->synListView)
		_listView_Data_ApplyDy(dy, pD);
	// click
	if (_win->winState->checkMouseIn(&(self->re)) && mouseEveCode == 2)
	{
		Sint32 y = 0, ry = 0;
		_win->winState->getMousePos(NULL, &y);
		ry = y - self->re.y + self->visPixelRange[0];
		self->clickIdx = ry > self->pixel2idx.size() ? -1 : self->pixel2idx[ry];
	}
	return retMouseEveCode;
}

ba::ui::inputBox::inputBox(window* _win, SDL_Rect pos, int charSize, int edgeWidth, SDL_Color ec, SDL_Color cc, SDL_Color bgc, const char* pc, TTF_Font* font)
{
	this->win = _win;
	this->pfont = font ? font : win->defaultFont;
	this->re = pos;
	if (pos.h == 0)
		this->re.h = charSize + 2 * edgeWidth;
	realTextRe = { 0, 0, charSize * (visCharRange[1] - visCharRange[0]), re.h };
	this->col = bgc;
	this->bgc = bgc;
	this->cc = cc;
	this->ec = ec;
	this->charSize = charSize;
	this->edgeWidth = edgeWidth;
	this->cursor = new rect(0, 0, 1, re.h, cc.r, cc.g, cc.b, cc.a);
	this->cursor->win = win;
	this->cursor->rendRect();
	rendRect();
}
void ba::ui::inputBox::addChar(SDL_Keycode key)
{
	if (key == 8 || key == 127)
	{//Backspace or Delete
		if ((cursorChrPos == 0 && key == 8) || (cursorChrPos == visCharRange[1] && key == 127))
			return;
		allText.erase(key == 8 ? cursorChrPos -1 : cursorChrPos, 1);
		//if [.| ] pass
		if (visCharRange[0] > 0)//...[..|] || ...[.|..]
			visCharRange[0]--;
		if (cursorChrPos <= visCharRange[1])//[.|..] || [.|. ]
			visCharRange[1]--;
		if(key == 8)
			cursorChrPos--;
	}
	else if (key == 13)
	{// Enter
		keepEveAlive = false;//按下Enter，退出输入模式
	}
	else if (key == -1)
	{// 无字符更新，仅根据参数刷新画面
	}
	else
	{// 正常插入字符
		allText.insert(cursorChrPos, 1, (char)key);
		visCharRange[1] ++;
		cursorChrPos++;
		if (charSize * (visCharRange[1] - visCharRange[0]) > re.w)
			visCharRange[0] ++;
	}
	realTextRe.w = charSize * (visCharRange[1] - visCharRange[0]);
	text = allText.substr(visCharRange[0], visCharRange[1]);
	rendText(false);
	SDL_Surface* surTmp = SDL_CreateRGBASurface(re.w, re.h);
	SDL_FillRect(surTmp, NULL, SDL_MapRGBA(surTmp->format, bgc.r, bgc.g, bgc.b, bgc.a));
	if(sur)
		SDL_BlitScaled(sur, NULL, surTmp, &realTextRe);
	cursor->re.x = charSize * (cursorChrPos - visCharRange[0]);
	std::cout << cursorChrPos << " | " << visCharRange[0] << std::endl;
	SDL_BlitScaled(cursor->sur, NULL, surTmp, &cursor->re);
	tex = SDL_CreateTextureFromSurface(win->rend, surTmp);
	SDL_FreeSurface(surTmp);
}
int ba::ui::_inputBox_check(window* _win, void* _self, int mouseEveCode, void* _pData)
{
	inputBox* self = (inputBox*)_self;
	if (self->win->winState->getMouseEveCode(&(self->win->re)) != 0 && self->win->winState->getMouseEveCode(&(self->re)) == 0)
		self->keepEveAlive = false;//窗口内有点击事件并且不在本输入框内
	else
		self->keepEveAlive = true;
	if (self->win->winState->isQuit)
		self->keepEveAlive = false;//按下ESC
	if (self->keepEveAlive)
	{
		std::pair<SDL_Keycode, clock_t> key = self->win->winState->getKeyboardEve();
		if (key.second && SDL_GetTicks() - key.second < 100)
			self->addChar(key.first);
		if (self->win->winState->getMouseEveCode(&(self->win->re)) == 2)
		{// 鼠标点击改变光标位置
			Sint32 x = 0;
			self->win->winState->getMousePos(&x);
			if ((x - self->re.x) / self->charSize <= self->visCharRange[1])
			{
				self->cursorChrPos = (x - self->re.x) / self->charSize + self->visCharRange[0];
				self->cursor->re.x = (self->cursorChrPos - self->visCharRange[0]) * self->charSize;
				self->addChar(-1);//无字符更新，仅根据参数刷新画面
			}
		}
	}
	return 0;
}

int ba::ui::_dragBar_check(window* _win, void* _self, int mouseEveCode, void* _pData)
{
	dragBar* self = (dragBar*)_self;
	if (_win->winState->getMouseEveCode(&(self->re)) == 1)
	{
		Sint32 x = 0;
		_win->winState->getMousePos(&x);
		if(x > self->re.x && x < self->re.x + self->re.w)
		{
			self->handle.re.x = x;
			self->per = (float)(self->handle.re.x - self->re.x) / self->re.w;
		}
	}
	return mouseEveCode;
}

ba::ui::dragBar::dragBar(window* _win, SDL_Rect pos, SDL_Rect handleShape, float per,
	SDL_Color bgc, SDL_Color hc)
	: rect(pos, bgc)
{
	this->win = _win;
	rendRect();
	this->per = per;
	handleShape.x = pos.x + (int)(per*pos.w);
	handleShape.y = pos.y;
	handleShape.h = pos.h;
	handle = rect(handleShape, hc);
	handle.win = _win;
	handle.rendRect();
	getTex();
}
SDL_Texture* ba::ui::dragBar::getTex()
{
	if (tex)
	{
		SDL_DestroyTexture(tex);
		tex = NULL;
	}
	SDL_Surface* surTmp = SDL_CreateRGBASurface(re.w, re.h);
	SDL_FillRect(surTmp, NULL, SDL_MapRGBA(surTmp->format, col.r, col.g, col.b, col.a));
	SDL_Rect reTmp = { (int)(per*(float)re.w), handle.re.y, handle.re.w, handle.re.h };
	SDL_BlitScaled(handle.sur, NULL, surTmp, &reTmp);
	tex = SDL_CreateTextureFromSurface(win->rend, surTmp);
	SDL_FreeSurface(surTmp);
	return tex;
}
