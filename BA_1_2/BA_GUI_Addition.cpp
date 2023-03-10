﻿#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_String.hpp"

#include"BA_GUI.hpp"
#include"BA_GUI_Addition.hpp"

// TODO : 增加输入框、拖动条等


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
