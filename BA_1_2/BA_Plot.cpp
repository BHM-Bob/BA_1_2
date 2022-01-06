#include "BA_Plot.hpp"

BA_Plot::BA_Plot(const char* _name, float (*_MapFunc)(float x, float y))
{
	this->Make_BA_Plot(_name, _MapFunc, 10, 10, 2, intdup(3, 255, 255, 255));
}

BA_Plot::BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _l)
{
	this->Make_BA_Plot(_name, _MapFunc, _l, _l, 2, intdup(3, 255, 255, 255));
}

BA_Plot::BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _xlim, float _ylim)
{
	this->Make_BA_Plot(_name, _MapFunc, _xlim, _ylim, 2, intdup(3, 255, 255, 255));
}

BA_Plot::BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _xlim, float _ylim, int* backg)
{
	this->Make_BA_Plot(_name, _MapFunc, _xlim, _ylim, 2, backg);
}

void BA_Plot::Make_BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _xlim, float _ylim, int flag, int* backg)
{
	_W = 500;
	_H = ((float)_W * _ylim) / _xlim;
	_W2 = _W / 2;
	_H2 = _H / 2;
	name.Concat(_name);
	pui = MyUI_Init(name.pc, _W, _H, flag, backg);
	MapFunc = _MapFunc;
	ylim = _xlim;
	xlim = _ylim;
	scaleX = (float)_W / 2.0 / xlim;
	scaleY = (float)_H / 2.0 / ylim;
}

BA_Plot BA_Plot::Paint(void)
{
	int newx = 0, newy = 0, oldx = -1, oldy = -1;
	SDL_SetRenderDrawColor(pui->win->rend, 0, 200, 0, 255);
	for (float y = ylim; y > -ylim; y -= 0.01)
	{
		for (float x = -xlim; x < xlim; x += 0.01)
		{
			newx = scaleX * x + _W2;
			newy = _H2 - scaleY * y;
			if (newx != oldx || newy != oldy)
			{
				if (MapFunc(x, y) > 0)
					SDL_RenderDrawPoint(pui->win->rend, scaleX * x + _W2, _H2 - scaleY * y);

				oldx = newx;
				oldy = newy;
			}
		}
	}

	SDL_SetRenderDrawColor(pui->win->rend, 0, 0, 0, 255);
	SDL_RenderDrawLine(pui->win->rend, 0, _H2, _W, _H2);
	SDL_RenderDrawLine(pui->win->rend, _W2, 0, _W2, _H);

	pui->win->pre_title = pui->win->pre_win;

	return *this;
}

BA_Plot BA_Plot::PaintMM(void)
{
	BA_Array data(BA_Shape(1,_H*_W), "f");
	float min = 0., max = 0.,tmp = 0.;
	int newx = 0, newy = 0, oldx = -1, oldy = -1;

	for (float y = ylim; y > -ylim; y -= 0.01)
	{
		for (float x = -xlim; x < xlim; x += 0.01)
		{
			*(data.dataF+ (int)(scaleX * x) + _W2+ (_H2 - (int)(scaleY * y))*_W ) = tmp = MapFunc(x, y);
			if (tmp > max)
				max = tmp;
			else if (tmp < min)
				min = tmp;
		}
	}
	for (float y = ylim; y > -ylim; y -= 0.01)
	{
		for (float x = -xlim; x < xlim; x += 0.01)
		{
			newx = scaleX * x + _W2;
			newy = _H2 - scaleY * y;
			if (newx != oldx || newy != oldy)
			{
				tmp = *(data.dataF + newx + (newy) * _W);
				if (tmp > 0)
					SDL_SetRenderDrawColor(pui->win->rend, 255* tmp/max, 0, 255, 255);
				else//tmp <= 0
					SDL_SetRenderDrawColor(pui->win->rend, 0, 255 * tmp / min, 255, 255);

				oldx = newx;
				oldy = newy;
				SDL_RenderDrawPoint(pui->win->rend, newx, newy);
			}
		}
	}

	SDL_SetRenderDrawColor(pui->win->rend, 0, 0, 0, 255);
	SDL_RenderDrawLine(pui->win->rend, 0, _H2, _W, _H2);
	SDL_RenderDrawLine(pui->win->rend, _W2, 0, _W2, _H);
	pui->win->pre_title = pui->win->pre_win;

	MyBA_Free_R(data.mem);

	return *this;
}

BA_Plot BA_Plot::Loop(bool renderClear)
{
	PPF(pba->GUT());
	for (; pui->pF_PollQuit(pui)==0 ; )
		pui->pF_Update(pui, renderClear, 0);
	return *this;
}
