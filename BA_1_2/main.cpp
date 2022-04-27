#include "BA_1_2.hpp"

void proc(List* pLi)
{
}

int main(int argc, char** argvs)
{
	MyBA_Init();
    cout << "我是主线程" << endl;
    int a = 9;
    thread th2(proc, a);//第一个参数为函数名，第二个参数为该函数的第一个参数。此时线程开始执行。
    th2.join();//此时主线程被阻塞直至子线程执行结束。

	return MyBA_Quit();
}