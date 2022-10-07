//BASIC_ALL_1_2 Thread
//H
//Writen By BHM
//2022年9月25日

#ifndef BA_1_2_THREAD_H
#define BA_1_2_THREAD_H

#include"BA_BASE.HPP"

//void (*_pF)(_LL, balist<dataTypePut>&, balist<dataTypeGet>&, balist<float>&, balist<bool>&, void*)
//id, getQ, putQ, process, sig, data
template<typename dataTypePut, typename dataTypeGet>
class MyThreadsPool
{
public:
	_LL sumThreads = 0;
	_LL sumTasks = 0;
	balist<bool> sig = balist<bool>();
	balist<dataTypePut>* putDataQues = NULL;
	balist<dataTypeGet>* getDataQues = NULL;
	balist<float>* procQues = NULL;
	_LL quePtr = 0;
	char* name = NULL;
	std::thread** ppThs = NULL;
	COORD pos = COORD();

	List* mem = List_Init();

	MyThreadsPool(_LL _sumThreads,
		void (*_pF)(_LL, balist<dataTypePut>&, balist<dataTypeGet>&, balist<float>&, balist<bool>&, void*),
		void* otherData = NULL, const char* _name = NULL);

	void PutTask(dataTypePut* pData, std::mutex* m);
	balist<dataTypeGet>* LoopToQuit(std::mutex* m);
	void Destroy(std::mutex* m);
};

template<typename dataTypePut, typename dataTypeGet>
MyThreadsPool<dataTypePut, dataTypeGet>::MyThreadsPool(_LL _sumThreads,
	void(*_pF)(_LL, balist<dataTypePut>&, balist<dataTypeGet>&,
		balist<float>&, balist<bool>&, void*), void* otherData, const char* _name)
{
	sumThreads = _sumThreads;
	ppThs = BALLOC_R(_sumThreads, std::thread*, mem);
	putDataQues = new balist< dataTypePut>[_sumThreads];
	getDataQues = new balist< dataTypeGet>[_sumThreads];
	procQues = new balist< float>[_sumThreads];
	//pF = _pF;
	if (!_name)
		name = mstrdup("MyThreadsPool", mem);
	else
		name = mstrdup(_name, mem);
	for (_LL i = 0; i < sumThreads; i++)
	{
		ppThs[i] = new std::thread(_pF, i, std::ref(putDataQues[i]),
			std::ref(getDataQues[i]), std::ref(procQues[i]), std::ref(sig), otherData);
	}
}

template<typename dataTypePut, typename dataTypeGet>
inline void MyThreadsPool<dataTypePut, dataTypeGet>::PutTask(dataTypePut* pData, std::mutex* m)
{
	putDataQues[quePtr].ThrPut(pData, m);
	quePtr = ((quePtr + 1) < sumThreads) ? (quePtr + 1) : 0;
	sumTasks += 1;
}

//be sure that all tasks sended, this func will
//send 'wait to quit' signal to every que,
//and start to loop waiting
template<typename dataTypePut, typename dataTypeGet>
inline balist<dataTypeGet>* MyThreadsPool<dataTypePut, dataTypeGet>::LoopToQuit(std::mutex* m)
{
	balist<dataTypeGet>* retList = new balist<dataTypeGet>();
	for (_LL idx = 0; idx < sumThreads; idx++)
		putDataQues[idx].ThrPut((dataTypePut*)0x1, m);
	_LL sumTasksTillNow = 0;
	float st = (float)clock();
	float* hisProc = BALLOC_R(sumThreads, float, mem);
	float* procTemp = NULL;
	pos = GetConsoleCursor();
	while (sig.ThrSize(m) < sumThreads)
	{
		sumTasksTillNow = 0;
		for (_LL idx = 0; idx < sumThreads; idx++)
			sumTasksTillNow += putDataQues[idx].ThrSize(m);

		SetConsoleCursor(pos.X, pos.Y);
		printf("%20s: subThreads working: %5llu / %5llu  --  %8.3f sec\n",
			name, sumTasksTillNow, sumTasks, (float)(clock() - st) / CLOCKS_PER_SEC);
		for (_LL idx = 0; idx < sumThreads; idx++)
		{
			for (; procQues[idx].ThrSize(m) > 1; free(procQues[idx].ThrGet(m)));
			procTemp = procQues[idx].ThrSize(m) > 0 ? procQues[idx].ThrGet(m) : NULL;
			hisProc[idx] = procTemp ? *procTemp : hisProc[idx];
			printf("subThreads %llu : %6.3f %%        \n", idx, hisProc[idx]);
			if (procTemp)
				free(procTemp);
		}

		for (_LL idx = 0; idx < sumThreads; idx++)
			while (getDataQues[idx].ThrSize(m) > 0)
				retList->Put(getDataQues[idx].ThrGet(m));
		Sleep(1000);
	}
	for (_LL idx = 0; idx < sumThreads; idx++)
	{
		while (getDataQues[idx].ThrSize(m) > 0)
			retList->Put(getDataQues[idx].ThrGet(m));
		ppThs[idx]->join();
		printf("\r%s: waiting to join subThreads: %llu / %llu  --  %8.3f sec",
			name, sumTasksTillNow, sumTasks,
			(float)(clock() - st) / CLOCKS_PER_SEC);
	}
	return retList;
}

template<typename dataTypePut, typename dataTypeGet>
inline void MyThreadsPool<dataTypePut, dataTypeGet>::Destroy(std::mutex* m)
{
	MyBA_Free_R(mem);
	List_Destroy(mem);
	delete[] getDataQues;
	delete[] putDataQues;
	delete[] procQues;
}

#endif