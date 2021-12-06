
#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_UI.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include "BA_Test.hpp"




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

BA_Array BA_Array_GetTest()
{
	BA_Array t1 = BA_Array(BA_Shape(1, 10000000), "range");
	return t1;
}

float BA_ArrayTest_1(float* p)
{
	return 2.0 * (*p) / powf(3.0, *p);
}

void BA_String_Test(void)
{
	PPSS(BA_String("GGGKKtKHHHtGGGKKKBtBBHHeHJJJ").Replace("HHH", NULL).pc);
}

void BA_Array_Test(void)
{
	PPF(pba->GUT());
	BA_Array t1 = BA_Array_GetTest();
	PPF(t1.Func(BA_ArrayTest_1, 0).Sum().dataSumF);
	PPF(pba->GUT());
}