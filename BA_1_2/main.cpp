#include "BA_1_2.hpp"

int main(int argc, char** argvs)
{
	MyBA_Init();

	BA_Array_Test();

	int a[9][8] = { 0 };
	PPU(sizeof(char));
	PPU(sizeof(int(*)[9]));
	PPU(sizeof(a + 1));

	return MyBA_Quit();
}