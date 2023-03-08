#include"BA_Base.hpp"
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
	rendRect();
	data.re = re;
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
	item->re.y = data.sumHeight;
	data.eachHeight.emplace_back(item->re.h);
	data.pixel2idx.insert(data.pixel2idx.end(), item->re.h, items.size() - 1);
	data.sumHeight += item->re.h;
	if (isEnd)
	{
		data.visPixelRange[1] = min(re.h, data.sumHeight) - 1;
		data.refreshTex = true;
	}
}
void ba::ui::listView::clear(void)
{
	if(! items.empty())
	{
		data.sumHeight = data.visPixelRange[0] = data.visPixelRange[1] = 0;
		data.eachHeight.clear();
		data.pixel2idx.clear();
		data.synListViewData.clear();
		data.refreshTex = true;
		data.clickIdx = -1;
		for (auto item : items)
			delete item;
		items.clear();
		events.clear();
		statue.clear();
	}
}
SDL_Texture* ba::ui::listView::getTex(void)
{
	if (!data.refreshTex || data.pixel2idx.size()==0)
		return tex;
	data.refreshTex = false;
	if (tex)
		SDL_DestroyTexture(tex);
	_LL firstVisItemIdx = data.pixel2idx[data.visPixelRange[0]];
	_LL lastVisItemIdx = data.pixel2idx[data.visPixelRange[1]];
	_LL firstVisItemVisY = items[firstVisItemIdx]->re.y - data.visPixelRange[0];//<=0
	SDL_Rect reTmp = { 0, firstVisItemVisY, 0, 0 };
	SDL_Surface* surTmp = SDL_CreateRGBASurface(re.w, re.h);
	SDL_BlitScaled(sur, NULL, surTmp, NULL);//will make nage y turn 0 !!!
	for (int i = firstVisItemIdx; i <= lastVisItemIdx; i++)
	{
		reTmp.w = items[i]->re.w;
		reTmp.h = items[i]->re.h;
		SDL_BlitScaled(items[i]->sur, NULL, surTmp, &reTmp);//will make nage y turn 0 !!!
		if (i == firstVisItemIdx)
			reTmp.y = firstVisItemVisY + data.eachHeight[i];
		else
			reTmp.y += data.eachHeight[i];
	}
	tex = SDL_CreateTextureFromSurface(win->rend, surTmp);
	SDL_FreeSurface(surTmp);
	return tex;
}
_LL ba::ui::_listView_Data_ApplyDy(_LL dy, listView_Data* pData)
{
	if (dy > 0 && pData->visPixelRange[0] > 0)//scroll up
		dy = -min(dy, pData->visPixelRange[0]);//请求上滑距离和剩余可上滑距离的最小者
	else if (dy < 0 && pData->visPixelRange[1] < pData->sumHeight - 1)//scroll down
		dy = min(-dy, pData->sumHeight - pData->visPixelRange[1] - 1);//请求下滑距离和剩余可下滑距离的最小者
	else
		return 0;
	pData->visPixelRange[0] += dy;
	pData->visPixelRange[1] += dy;
	pData->refreshTex = true;
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
			PPX(dy);
		}
		if (!_win->winState->wheelY.empty())
			retMouseEveCode = 4;
		else
			retMouseEveCode = 0;
		return dy*(_win->winState->wheelY.size()+1)*4; });//放大
	_listView_Data_ApplyDy(dy, &(self->data));
	for (listView_Data* pD : self->data.synListViewData)
		_listView_Data_ApplyDy(dy, pD);
	// click
	if (_win->winState->checkMouseIn(&(self->data.re)) && mouseEveCode == 2)
	{
		Sint32 y = 0, ry = 0;
		_win->winState->getMousePos(NULL, &y);
		ry = y - self->data.re.y + self->data.visPixelRange[0];
		self->data.clickIdx = ry > self->data.pixel2idx.size() ? -1 : self->data.pixel2idx[ry];
	}
	return retMouseEveCode;
}
