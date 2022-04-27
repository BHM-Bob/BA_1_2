#include "BA_1_2.hpp"

mutex m;

void workFunc(_ULL id, MyThreadQueue& getQ, MyThreadQueue& putQ,
    MyThreadQueue& sig, void* data)
{
    _ULL* pData = NULL;
    int js = 0;
    while (true)
    {
        pData = (_ULL*)getQ.Get(&m);
        if (pData == (_ULL*)0x1)
            break;
        BA_Array* a = new BA_Array(BA_Shape(4, 4, 3, 32, 32), "rand");
        putQ.Put((void*)a, &m);
        js++;
    }PPI(js);
    //PPIs(2, (int)id, js);
    sig.Put(NULL, &m);
}

int main(int argc, char** argvs)
{
	MyBA_Init();

    MyThreadsPool tp = MyThreadsPool(8, workFunc, NULL);
    for (_ULL i = 0; i < 9999; i++)
        tp.PutTask((void*)ULLdupS(1, i), &m);
    tp.LoopToQuit(&m, (void*)0x1);

	return MyBA_Quit();
}