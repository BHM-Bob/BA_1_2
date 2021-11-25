#include "BA_1_2.hpp"

BA_Array BA_Array_GetTest();
void BA_Array_Test(void);

void BA_Dir_Test(void);

int main(int argc, char** argvs)
{
	MyBA_Init();

	BA_Array_Test();

	return MyBA_Quit();
}

void BA_Dir_Test(void)
{
	BA_Dir("D:\\^SYSTEM\\SETTING\\Wallpear Standered","png").Print();
}

BA_Array BA_Array_GetTest()
{
	BA_Array t1 = BA_Array(BA_Shape(3, 100, 100, 100), "rand");
	return t1;
}

void BA_Array_Test(void)
{
	PPF(pba->GUT());
	BA_Array t1 = BA_Array_GetTest();
	BA_Array t2 = t1.Reshape(BA_Shape(4, 10, 10, 100, 100), 0).Ge(0., 1).To('l', 0).Sum();
	_ULL old = 0;
	for (float i = 1.; i > 0; i -= 0.001)
	{
		t2 = t1.Ge(i, 1).To('l', 0).Sum();
		PPU(t2.dataSumL - old);
		old = t2.dataSumL;
		MyBA_Free_R(t2.mem);
	}
	PPF(pba->GUT());
}