#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_String.hpp"

#include"BA_GUI.hpp"
#include"BA_GUI_Addition.hpp"

// TODO : 增加constListView，一次性生成列表，支持动态渲染
// TODO : 增加输入框、拖动条等


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

int ba::ui::_listView_check(window* _win, void* _pData)
{
	listView_Data* pData = (listView_Data*)_pData;
	if (! _win->winState->getVar(true, [=]() {return checkDotInRect(
		_win->winState->mouseEndPos[0], _win->winState->mouseEndPos[1], &(pData->re)); }))
		return -1;
	// scroll
	_LL dy = _win->winState->getVar((Sint32)0, [=]() {
		Sint32 dy = 0;
		if (_win->winState->wheelY.size() > 0)
		{
			dy = _win->winState->wheelY.back();
			_win->winState->wheelY.pop_back();
		}
		return dy*(_win->winState->wheelY.size()+1)*3; });//放大
	_listView_Data_ApplyDy(dy, pData);
	for (listView_Data* pD : pData->synListViewData)
		_listView_Data_ApplyDy(dy, pD);
	// click
	if (_win->winState->checkMouseIn(&(pData->re)) && _win->winState->getMouseEveCode(&(pData->re)) == 2)
	{
		_win->winState->_mutexSafeWrapper([&]() {_win->winState->mouseEveCode = 0; });
		Sint32 y = 0;
		_win->winState->getMousePos(NULL, &y);
		pData->clickIdx = pData->pixel2idx[y - pData->re.y + pData->visPixelRange[0]];
	}
	return 0;
}
