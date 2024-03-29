﻿#include "BA_BASE.HPP"
#include "BA_Mem.hpp"
#include "BA_String.hpp"


ba::singleStack::singleStack(const char* _funcName, singleStack* _up)
{
	mem = new memRecord();
	mem->stack = this;
	funcName = strdup(_funcName, mem, 0);
	up = _up;
	if (up)
		up->next = this;
}

ba::memRecordDot::memRecordDot(_LL _size, _LL _toBeFreedInStack,
	memRecordDot* _next, void* _data)
{
	size = _size;
	toBeFreedInStack = _toBeFreedInStack;
	next = _next;
	data = _data;
}

ba::memRecord::memRecord()
{
}

int ba::memRecord::destroy(void)
{
	for (memRecordDot* pd = base, *pdt = NULL; pd; )
	{
		if (pd->toBeFreedInStack >= 0 && pd->data != BA_FREED_PTR)
			free(pd->data);
		else// to be freed in upstream funcs
			stack->up->mem->put(pd->data, pd->size, pd->toBeFreedInStack + 1);
		pdt = pd->next;
		delete pd;
		pd = pdt;
	}
	return 0;
}

//when it comes to the first stack, init and mount it to a nullptr
//when it comes to the other, simply init and mount it to the last of the queue
void ba::stackInit(const char* _funcName)
{
	singleStack* up = pba->stacks->stacks.size() == 0 ? nullptr : *(pba->stacks->stacks.end());
	ba::singleStack* ps = new singleStack(_funcName, up);
	pba->stacks->stacks.emplace_front(ps);
}

// if mem == NULL, use MCALLOC
float* ba::typedup(ba::memRecord* mem, _LL toBeFreedInStack, _ULL num, float firstData, ...)
{
	_ballocs(float, pret, num, mem, toBeFreedInStack, 1);
	pret[0] = firstData;
	va_list parg;
	va_start(parg, firstData);
	for (_ULL a = 1; a < num; a++)
		pret[a] = (float)va_arg(parg, double);
	va_end(parg);
	return pret;
}
