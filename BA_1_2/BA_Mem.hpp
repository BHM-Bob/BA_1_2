//BASIC_ALL_1_2 MEM
//H
//Writen By BHM
//2022年9月25日

#ifndef BA_1_2_MEM_H
#define BA_1_2_MEM_H

#include"BA_BASE.HPP"

#define _balloc(Ty,count,mem) ba::balloc<Ty>(count,mem)
#define _ballocs(Ty,varName,count,mem,fs,errRetCodeAsPtr); Ty*varName=ba::balloc<Ty>(count,mem,fs);if(!varName){PPW("OOM");return (Ty*)errRetCodeAsPtr;}

namespace ba
{
	class memRecord;//前置声明

	class singleStack
	{
	public:
		char* funcName = NULL;
		memRecord* mem = NULL;
		singleStack* up = NULL;
		singleStack* next = NULL;

		singleStack(const char* _funcName, singleStack* _up);
	};

	class stack//向外提供服务，BA内部函数不使用
	{
	public:
		std::deque<singleStack*> stacks;

		stack(){}
	};

	class memRecordDot
	{
	public:
		_LL size = 0;
		_LL toBeFreedInStack = 0;
		memRecordDot* next = NULL;
		void* data = NULL;

		memRecordDot(_LL _size, _LL _toBeFreedInStack,
			memRecordDot* _next, void* _data);
	};

	class memRecord
	{
	public:
		_ULL sumDots = 0;
		memRecordDot* base = new memRecordDot(-1, 0, NULL, NULL);
		memRecordDot* top = base;

		singleStack* stack = NULL;

		memRecord();
		template<typename Ty>
		inline void put(Ty* p, _LL size, _LL toBeFreedInStack = 0)
		{
			top->next = new memRecordDot(size, toBeFreedInStack, NULL, (void*)p);
		}
		int destroy(void);
	};



	void stackInit(const char* _funcName);
	//ba::_stackInit();
#define _stackInit() stackInit(__func__)
	template<typename Ty>
	Ty stackQuit(Ty retValue)
	{
		ba::singleStack* ps = pba->stacks->stacks[0];
		pba->stacks->stacks.pop_front();
		ps->mem->destroy();
		delete ps->mem;
		return retValue;
	}

	template<typename Ty, typename BaseTy>
	Ty* allocNDArray_AllocD(Ty* highLeverPtr, std::vector<_LL> shape, int nowDim, BaseTy baseV, List* mem)
	{
		highLeverPtr = BALLOC_R(shape[nowDim], Ty, mem);
		if (!highLeverPtr || (_ULL)nowDim + 1 == shape.size())
			return highLeverPtr;
		if constexpr (!std::is_same_v<Ty, BaseTy>)
			for (_ULL i = 0 ; i < shape[nowDim] ; i++)
				highLeverPtr[i] = allocNDArray_AllocD(highLeverPtr[i], shape, nowDim + 1, BaseTy(), mem);
		return highLeverPtr;
	}
	//int*** p2 = ba::allocNDArray<int**, int>({ 5, 6, 9});
	template<typename LessPtrTy, typename BaseTy>
	inline LessPtrTy* allocNDArray(std::vector<_LL> shape, List* mem = NULL)
	{
		if (shape.size() == 0)
			return nullptr;
		LessPtrTy* ret = nullptr;
		return allocNDArray_AllocD<LessPtrTy, BaseTy>(ret, shape, 0, BaseTy(), mem);
	}


	template<typename Ty>
	Ty* balloc(_LL count, memRecord* mem = NULL, _LL toBeFreedInStack = 0,
		const char* errWaring = "OOM", _ULL errRetCodeAsPtr = 1);
	// if mem == NULL, use MCALLOC
	float* typedup(ba::memRecord* mem, _LL toBeFreedInStack,
		_ULL num, float firstData, ...);
	// if mem == NULL, use MCALLOC
	template<typename dataType>
	dataType* typedup(ba::memRecord* mem, _LL toBeFreedInStack,
		_ULL num, dataType firstData, ...);



	template<typename Ty>
	Ty* balloc(_LL count, memRecord* mem, _LL toBeFreedInStack,
		const char* errWaring, _ULL errRetCodeAsPtr)
	{
		Ty* ret = (Ty*)calloc(count, sizeof(Ty));
		if (!ret)
		{
			PPWs("ba::balloc::Waring: ", errWaring);
			return (Ty*)errRetCodeAsPtr;
		}
		if (mem)
		{
			mem->put(ret, sizeof(Ty), toBeFreedInStack);
		}
		return ret;
	}

	template<typename dataType>
	inline dataType* typedup(ba::memRecord* mem, _LL toBeFreedInStack, _ULL num, dataType firstData, ...)
	{
		_ballocs(dataType, pret, num, mem, toBeFreedInStack, 1);
		pret[0] = firstData;
		va_list parg;
		va_start(parg, firstData);
		for (_ULL a = 1; a < num; a++)
			pret[a] = (dataType)va_arg(parg, dataType);
		va_end(parg);
		return pret;
	}
}

#endif