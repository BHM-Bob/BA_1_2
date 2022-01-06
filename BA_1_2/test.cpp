
#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_UI.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include "BA_Test.hpp"
#include"BA_Plot.hpp"

float BA_Plot_Test_Map(float x,float y)
{
	return 1-x*log(x-y) + y * y;
}

void BA_Plot_Test(void)
{
	PPF(pba->GUT());
	BA_Plot("Test", BA_Plot_Test_Map, 30., 30.).PaintMM().Loop(0);
}


void BA_Dir_Test(void)
{
	BA_Dir dir = BA_Dir("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\culturable", "jpeg");
	LIST_FORS(char, path, dir.files)
	{
		if (remove(StringAdd_S(dir.root, "\\", path, NULL)) != 0)
			MyBA_Errs(1, "BA_Dir_Test:can not remove file:", StringAdd_S(dir.root, "\\", path), " ,do nothing", NULL);
		if (dir.files->now && dir.files->now->idx % 5 == 0)
			JDT(dir.files->now->idx, dir.files->sumque);
	}
}

void BA_String_Test(void)
{
	PPSS(BA_String("GGGKKtKHHHtGGGKKKBtBBHHeHJJJ").Replace("HHH", NULL).pc);
}

float BA_ArrayTest_1(float* p)
{
	return pow(*p, 2);
}

float BA_ArrayTest_2(float* pt, void* p)
{
	return pow(*pt-*(float*)p, 2);
}

void BA_Array_Test(void)
{
	PPF(pba->GUT());
	BA_Array t1 = BA_Array(BA_Shape(1, 1000000), "rand");
	float x8 = t1.Sum().dataSumF / t1.dataLen;
	PPF(x8);
	float* px8 = &x8;
	float e1 = t1.Func(BA_ArrayTest_1, 1).Sum().dataSumF / t1.dataLen - x8*x8;
	float e2 = t1.Maps(BA_ArrayTest_2, (void*)px8, 1).Sum().dataSumF / t1.dataLen;
	PPF(e1);
	PPF(e2);
	PPF(pba->GUT());
}