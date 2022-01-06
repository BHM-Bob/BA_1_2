//BASIC_ALL_1_2
//H
//Writen By BHM
//2021��11��20��
//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_1_2__PLOT_H
#define BA_1_2__PLOT_H

#include"BA_Base.hpp"
#include"BA_UI.hpp"
#include"BA_String.hpp"
#include"BA_Math.hpp"

class BA_Plot
{
public:
	BA_String name = BA_String("BA_Plot:");
	MyUI* pui = NULL;
	//���ڻ������������볤�����
	int _H, _W, _H2, _W2;
	//Ϊ���᳤��
	float ylim = 0., xlim = 0.;
	//������xlim:_W2 = 1:scaleX
	float scaleX, scaleY;

	float (*MapFunc)(float x, float y) = NULL;
	List* mem = List_Init();

	BA_Plot(const char* _name, float (*_MapFunc)(float x, float y));
	BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _l);
	BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _xlim, float _ylim);
	BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _xlim, float _ylim, int* backg);
	void Make_BA_Plot(const char* _name, float (*_MapFunc)(float x, float y), float _xlim, float _ylim,int flag, int* backg);

	BA_Plot Paint(void);
	BA_Plot PaintMM(void);

	BA_Plot Loop(bool renderClear);
};

#endif