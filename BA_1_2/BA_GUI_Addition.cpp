#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"

#include"BA_GUI.hpp"
#include"BA_GUI_Addition.hpp"

// TODO : 增加constListView，一次性生成列表，支持动态渲染
// TODO : 增加输入框、拖动条等

int ba::ui::_listView_check(window* _win, void* _pData)
{
	listView_Data* pData = (listView_Data*)_pData;
	// scroll
	Sint32 dy = _win->winState->getVar((Sint32)0, [=]() {
		Sint32 dy = 0;
		if (_win->winState->wheelY.size() > 0)
		{
			dy = _win->winState->wheelY.back();
			_win->winState->wheelY.pop_back();
		}
		return dy; });
	if (dy)
	{//dy>0 : scroll up
		if (pData->visibleRange[0] - dy >= 0 && pData->visibleRange[1] - dy < pData->sumItems)
		{
			pData->visibleRange[0] -= dy;
			pData->visibleRange[1] -= dy;
			pData->moving = true;
		}
	}
	// click
	if (_win->winState->checkMouseIn(&(pData->re)) && _win->winState->getMouseEveCode(&(pData->re)) == 2)
	{
		_win->winState->_mutexSafeWrapper([&]() {_win->winState->mouseEveCode = 0; });
		Sint32 y = 0;
		_win->winState->getMousePos(NULL, &y);
		pData->clickIdx = (y - pData->re.y) / pData->singleH + pData->visibleRange[0];
	}
	return 0;
}