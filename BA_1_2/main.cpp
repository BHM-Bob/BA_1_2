#include "BA_1_2.hpp"

int main(int argc, char** argvs)
{
	MyBA_Init();

	BA_Array t1 = BA_Array(ULLdup(4, 25, 3, 128, 128), 4, (_ULL)0);
	BA_Array t2 = t1.To('f', 1);


	return MyBA_Quit();
}
