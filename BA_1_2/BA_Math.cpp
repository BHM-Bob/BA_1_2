//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021年11月18日 23点35分

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"

float abs_f(float a)
{
	if (a < 0)
		return -a;
	return a;
}

double abs_d(double a)
{
	if (a < 0)
		return -a;
	return a;
}

// [start, end)
_LL BA_Rand(_LL start, _LL end)
{
	return start + rand() % (end - start);
}

// [start, end)
_LL* BA_Rand(_LL start, _LL end, _LL len, List* mem)
{
	mem = mem ? mem : pba->LTmem;
	BALLOCS_R(_LL, ret, len, mem, NULL, );
	for (_LL i = 0; i < len; i++)
		ret[i] = BA_Rand(start, end);
	return ret;
}

int* MyBA_Zeros_AllocD(int* ret, int* shape, int dims, int nowdim)
{
	ret = (int*)calloc(shape[nowdim], sizeof(int));
	for (int i = 0; i < shape[nowdim] && nowdim < dims - 1; i++, ret++)
		MyBA_Zeros_AllocD((int*)ret, shape, dims, nowdim + 1);
	return ret;
}

void* MyBA_ZerosD(int* shape, int dims)
{
	void* ret = NULL;
	return MyBA_Zeros_AllocD((int*)ret, shape, dims, 0);
}

BA_Shape::BA_Shape(_LL len, ...)
{
	shapeLen = len;
	shape = BALLOC_L(len, _LL);
	if (shape == NULL)
	{
		MyBA_Err("BA_Shape::BA_Shape(_LL len, ...): BALLOC_L(len,int)==NULL, make shape = NULL", 1);
	}
	else
	{
		va_list parg;
		va_start(parg, len);
		for (int a = 0; a < len; a++)
			shape[a] = va_arg(parg, int);
		va_end(parg);
	}
}

BA_Shape::BA_Shape(_LL len, _LL* _shape)
{
	shapeLen = len;
	shape = BALLOC_L(len, _LL);
	if (shape == NULL)
	{
		MyBA_Err("BA_Shape::BA_Shape(_LL len, ...): BALLOC_L(len,int)==NULL, make shape = NULL", 1);
	}
	else
	{
		for (int a = 0; a < len; a++)
			shape[a] = _shape[a];
	}
}

BA_Shape::BA_Shape(_LL* _shape, _LL len)
{
	shapeLen = len;
	shape = BALLOC_L(len, _LL);
	if (shape == NULL)
	{
		MyBA_Err("BA_Shape::BA_Shape(_LL len, ...): BALLOC_L(len,int)==NULL, make shape = NULL", 1);
	}
	else
	{
		for (int a = 0; a < len; a++)
			shape[a] = _shape[a];
	}
}

char* BA_Shape::Str(bool toStr, bool printOut)
{
	char* pc = NULL;
	printf("[");
	for (_LL idx = 0; idx < shapeLen; idx++)
	{
		printf("%llu, ", shape[idx]);
	}
	printf("]");
	return nullptr;
}

BA_Array::BA_Array(BA_Shape _shape, _LL content)
{
	_LL* pt = _shape.shape;
	dataShape = BALLOC_R(_shape.shapeLen, _LL, mem);
	shapeLen = _shape.shapeLen;
	dataLen = 1;
	type = 'l';

	for (_LL i = 0; i < _shape.shapeLen; i++, pt++)
	{
		dataLen *= (*pt);
		dataShape[i] = *pt;
	}

	dataL = BALLOC_R(dataLen, _LL, mem);
	if (content != 0)
	{
		_LL* pt2 = dataL;
		for (_LL i = 0; i < dataLen; i++, pt2++)
			*pt2 = content;
	}
}

BA_Array::BA_Array(BA_Shape _shape, float content)
{
	_LL* pt = _shape.shape;
	dataShape = BALLOC_R(_shape.shapeLen, _LL, mem);
	shapeLen = _shape.shapeLen;
	dataLen = 1;
	type = 'f';

	for (_LL i = 0; i < _shape.shapeLen; i++, pt++)
	{
		dataLen *= (*pt);
		dataShape[i] = *pt;
	}

	dataF = BALLOC_R(dataLen, float, mem);
	if (content != 0)
	{
		float* pt2 = dataF;
		for (_LL i = 0; i < dataLen; i++, pt2++)
			*pt2 = content;
	}
}

BA_Array::BA_Array(BA_Shape _shape, float* content)
{
	_LL* pt = _shape.shape;
	dataShape = BALLOC_R(_shape.shapeLen, _LL, mem);
	shapeLen = _shape.shapeLen;
	dataL = NULL;
	type = 'f';
	dataLen = 1;

	for (_LL i = 0; i < _shape.shapeLen; i++, pt++)
	{
		dataLen *= (*pt);
		dataShape[i] = *pt;
	}

	dataF = BALLOC_R(dataLen, float, mem);
	if (content != NULL)
	{
		float* pt1 = content, * pt2 = dataF;
		for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
			*pt2 = *pt1;
	}
}

BA_Array::BA_Array(BA_Shape _shape, const char* way)
{
	dataShape = BALLOC_R(_shape.shapeLen, _LL, mem);
	shapeLen = _shape.shapeLen;
	dataL = NULL;
	dataF = NULL;
	type = 'f';
	dataLen = 1;

	for (_LL i = 0; i < _shape.shapeLen; i++)
	{
		dataLen = dataLen * _shape.shape[i];
		dataShape[i] = _shape.shape[i];
	}

	if (strcmp(way, "rand") == 0)
	{
		dataF = RandNum(0.f, 1.f, dataLen, mem);
	}
	else if (strcmp(way, "range") == 0)
	{
		float js = 0.;
		dataF = BALLOC_R(dataLen, float, mem);
		float* te = dataF;
		for (_LL i = 0; i < dataLen; i++, te++, js += 1.0)
			*te = js;
	}
	else if (strcmp(way, "l") == 0)
	{
		type = 'l';
		dataL = BALLOC_R(dataLen, _LL, mem);
	}
	else if (strcmp(way, "f") == 0)
	{
		type = 'f';
		dataF = BALLOC_R(dataLen, float, mem);
	}
	//else//strcmp(way, "xy") == 0
	//{
	//	if (_shapeLen < 2)
	//	{
	//		MyBA_Err("BA_Array::BA_Array(_LL* _dataShape, _LL _shapeLen, char* way): _shapeLen < 2 but way==\"xy\" which need least 2 dims,return 0s", 1);
	//	}
	//	else
	//	{

	//	}
	//}
}

BA_Array BA_Array::ReCreate(BA_Shape _shape, const char* way)
{
	MyBA_Free_R(mem);
	free(mem);
	mem = List_Init();

	dataShape = BALLOC_R(_shape.shapeLen, _LL, mem);
	shapeLen = _shape.shapeLen;
	dataL = NULL;
	dataF = NULL;
	type = 'f';
	dataLen = 1;
	dataSumF = 0.;
	dataSumL = 0;

	for (_LL i = 0; i < _shape.shapeLen; i++)
	{
		dataLen = dataLen * _shape.shape[i];
		dataShape[i] = _shape.shape[i];
	}

	if (strcmp(way, "rand") == 0)
	{
		dataF = BALLOC_R(dataLen, float, mem);
		float* te = dataF;
		for (_LL i = 0; i < dataLen; i++, te++)
			*te = (float)(rand() % 10000) / (float)10000.;
	}
	else if (strcmp(way, "range") == 0)
	{
		float js = 0.;
		dataF = BALLOC_R(dataLen, float, mem);
		float* te = dataF;
		for (_LL i = 0; i < dataLen; i++, te++, js += 1.0)
			*te = js;
	}
	else if (strcmp(way, "l") == 0)
	{
		type = 'l';
		dataL = BALLOC_R(dataLen, _LL, mem);
	}
	else if (strcmp(way, "f") == 0)
	{
		type = 'f';
		dataF = BALLOC_R(dataLen, float, mem);
	}

	return *this;
}

BA_Array BA_Array::To(char toType, bool aNew)
{
	if (aNew)
	{
		if (toType == 'l')
		{
			BA_Array te(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* te1 = te.dataL;
			if (type == 'l')
			{
				_LL* te2 = dataL;
				for (_LL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = *te2;
			}
			else
			{
				float* te2 = dataF;
				for (_LL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = (_LL)(*te2);
			}
			return te;
		}
		else//toType=='f'
		{
			BA_Array te(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* te1 = te.dataF;
			if (type == 'l')
			{
				_LL* te2 = dataL;
				for (_LL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = (float)(*te2);
			}
			else//selfType=='f'
			{
				float* te2 = dataF;
				for (_LL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = (*te2);
			}
			return te;
		}
	}
	else
	{
		if (toType == 'l')
		{
			if (type == 'f')//f->l
			{
				type = 'l';
				dataL = BALLOC_R(dataLen, _LL, mem);
				float* te1 = dataF;
				_LL* te2 = dataL;
				for (_LL i = 0; i < dataLen; i++, te1++, te2++)
					*te2 = (_LL)(*te1);
			}
		}
		else//toType=='f'
		{
			if (type == 'l')//l->f
			{
				type = 'f';
				dataF = BALLOC_R(dataLen, float, mem);
				float* te1 = dataF;
				_LL* te2 = dataL;
				for (_LL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = (float)(*te2);
			}
		}
	}
	return *this;
}

BA_Array BA_Array::Repeat(BA_Shape dims, bool aNew)
{
	if (shapeLen != dims.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Repeat(BA_Shape dims, bool aNew):shapeLen != dims.shapeLen, return *this", 1);
		return *this;
	}

	return *this;
}

BA_Array BA_Array::Concat(BA_Array a, int _dim)
{
	if (shapeLen != a.shapeLen || _dim > shapeLen || _dim > a.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Concat(BA_Array a, bool aNew):shapeLen err, return *this", 1);
		return *this;
	}
	if (shapeLen > 1)
	{
		for (int dim = 0; dim < shapeLen; dim++)
		{
			if (dataShape[dim] != a.dataShape[dim] && dim != _dim)
			{
				MyBA_Err("BA_Array BA_Array::Concat(BA_Array a, bool aNew):shape not matching eith other dims, return *this", 1);
				return *this;
			}
		}
	}
	BA_Shape newShape = BA_Shape(dataShape, shapeLen);
	newShape.shape[_dim] = dataShape[_dim] + a.dataShape[_dim];
	BA_Array ret = BA_Array(newShape, &type);
	// TODO: support axis
	if (type == 'f')
	{
		memcpy((void*)ret.dataF, (void*)dataF, dataLen * 4);
		memcpy((void*)(ret.dataF + dataLen), (void*)a.dataF, a.dataLen * 4);
	}
	else
	{
		memcpy((void*)ret.dataL, (void*)dataL, dataLen * 8);
		memcpy((void*)(ret.dataL + dataLen), (void*)a.dataL, a.dataLen * 8);
	}
	return ret;
}

BA_Array BA_Array::Sub(_LL from, _LL to)
{
	if (shapeLen != 1 || from >= to || from < 0 || to > dataLen)
	{
		MyBA_Err("BA_Array BA_Array::Sub(_LL from, _LL to):shapeLen != 1 || from >= to || from < 0 || to > dataLen, return *this", 1);
		return *this;
	}

	BA_Array ret = BA_Array(BA_Shape(1, to - from), 0.f);
	float* p1 = dataF + from, * p2 = ret.dataF;
	for (_LL idxR = 0; idxR < ret.dataLen; p1++, p2++, idxR++)
		*p2 = *p1;
	return ret;
}

BA_Array BA_Array::Add(BA_Array other, bool aNew)
{
	if (shapeLen != other.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Add(BA_Array other, bool aNew): shapeLen != other.GetShapeLen(),return", 1);
		return *this;
	}
	for (_LL i = 0; i < other.shapeLen; i++)
	{
		if (dataShape[i] != other.dataShape[i])
		{
			MyBA_Err("BA_Array BA_Array::Add(BA_Array other, bool aNew): dataShape[i] != other.dataShape[i],return", 1);
			return *this;
		}
	}
	if (type == 'l')
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt2 = other.dataL, * pt3 = ret.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 + *pt2;
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 + *pt2;
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 + (_LL)(*pt2);
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 + (_LL)(*pt2);
			}
		}
	}
	else//selfType == 'f'
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt3 = ret.dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 + (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 + (float)(*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt2 = other.dataF, * pt3 = ret.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 + (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 + (*pt2);
			}
		}
	}
	return *this;
}

BA_Array BA_Array::Reduc(BA_Array other, bool aNew)
{
	if (shapeLen != other.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Reduc(BA_Array other, bool aNew): shapeLen != other.GetShapeLen(),return", 1);
		return *this;
	}
	for (_LL i = 0; i < other.shapeLen; i++)
	{
		if (dataShape[i] != other.dataShape[i])
		{
			MyBA_Err("BA_Array BA_Array::Reduc(BA_Array other, bool aNew): dataShape[i] != other.dataShape[i],return", 1);
			return *this;
		}
	}
	if (type == 'l')
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt2 = other.dataL, * pt3 = ret.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - *pt2;
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - *pt2;
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - (_LL)(*pt2);
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - (_LL)(*pt2);
			}
		}
	}
	else//selfType == 'f'
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt3 = ret.dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - (float)(*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt2 = other.dataF, * pt3 = ret.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - (*pt2);
			}
		}
	}
	return *this;
}

BA_Array BA_Array::Mul(BA_Array other, bool aNew)
{
	if (shapeLen != other.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Mul(BA_Array other, bool aNew): shapeLen != other.GetShapeLen(),return", 1);
		return *this;
	}
	for (_LL i = 0; i < other.shapeLen; i++)
	{
		if (dataShape[i] != other.dataShape[i])
		{
			MyBA_Err("BA_Array BA_Array::Mul(BA_Array other, bool aNew): dataShape[i] != other.dataShape[i],return", 1);
			return *this;
		}
	}
	if (type == 'l')
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt2 = other.dataL, * pt3 = ret.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (*pt2);
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = (*pt1) * (*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (_LL)(*pt2);
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = (*pt1) * (_LL)(*pt2);
			}
		}
	}
	else//selfType == 'f'
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt3 = ret.dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = (*pt1) * (float)(*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt2 = other.dataF, * pt3 = ret.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = (*pt1) * (*pt2);
			}
		}
	}
	return *this;
}

BA_Array BA_Array::Devide(BA_Array other, bool aNew)
{
	if (shapeLen != other.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Devide(BA_Array other, bool aNew): shapeLen != other.GetShapeLen(),return", 1);
		return *this;
	}
	for (_LL i = 0; i < other.shapeLen; i++)
	{
		if (dataShape[i] != other.dataShape[i])
		{
			MyBA_Err("BA_Array BA_Array::Devide(BA_Array other, bool aNew): dataShape[i] != other.dataShape[i],return", 1);
			return *this;
		}
	}
	if (type == 'l')
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt2 = other.dataL, * pt3 = ret.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if (*pt2 != 0)
						*pt3 = (*pt1) / (*pt2);
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					if (*pt2 != 0)
						*pt1 = (*pt1) / (*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
				_LL* pt1 = dataL, * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if (*pt2 != 0)
						*pt3 = (*pt1) / (_LL)(*pt2);
				return ret;
			}
			else
			{
				_LL* pt1 = dataL, * pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					if (*pt2 != 0)
						*pt1 = (*pt1) / (_LL)(*pt2);
			}
		}
	}
	else//selfType == 'f'
	{
		if (other.type == 'l')
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt3 = ret.dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if (*pt2 != 0)
						*pt3 = (*pt1) / (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_LL* pt2 = other.dataL;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					if (*pt2 != 0)
						*pt1 = (*pt1) / (float)(*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt2 = other.dataF, * pt3 = ret.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if (*pt2 != 0)
						*pt3 = (*pt1) / (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
					if (*pt2 != 0)
						*pt1 = (*pt1) / (*pt2);
			}
		}
	}
	return *this;
}

BA_Array BA_Array::Add(_LL other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) + (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) + (float)(other);
		}
	}
	return *this;
}

BA_Array BA_Array::Reduc(_LL other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) - (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) - (float)(other);
		}
	}
	return *this;
}

BA_Array BA_Array::Mul(_LL other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) * (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) * (float)(other);
		}
	}
	return *this;
}

BA_Array BA_Array::Devide(_LL other, bool aNew)
{
	if (other == 0)
	{
		MyBA_Err("BA_Array BA_Array::Devide(_LL other, bool aNew): other == 0,return *this", 1);
		return *this;
	}
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) / (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) / (float)(other);
		}
	}
	return *this;
}

BA_Array BA_Array::Add(float other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (_LL)(other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) + (_LL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) + (other);
		}
	}
	return *this;
}

BA_Array BA_Array::Reduc(float other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (_LL)(other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) - (_LL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) - (other);
		}
	}
	return *this;
}

BA_Array BA_Array::Mul(float other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (_LL)(other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) * (_LL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) * (other);
		}
	}
	return *this;
}

BA_Array BA_Array::Devide(float other, bool aNew)
{
	if (other == 0)
	{
		MyBA_Err("BA_Array BA_Array::Devide(float other, bool aNew): other == 0,return *this", 1);
		return *this;
	}
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (_LL)(other);
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) / (_LL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) / (other);
		}
	}
	return *this;
}

BA_Array BA_Array::MatMul(BA_Array other)
{
	if (shapeLen != other.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::MatMul(BA_Array other, bool aNew): shapeLen != other.shapeLen,return *this", 1);
		return *this;
	}
	if (shapeLen <= 2 || other.shapeLen <= 2)
	{
		MyBA_Err("BA_Array BA_Array::MatMul(BA_Array other, bool aNew): shapeLen <= 2 || other.shapeLen <= 2,return *this", 1);
		return *this;
	}
	if (dataShape[shapeLen - 1] != other.dataShape[other.dataLen - 2])
	{
		MyBA_Err("BA_Array BA_Array::Add(BA_Array other, bool aNew): dataShape[shapeLen-1] != other.dataShape[other.dataLen-2],return *this", 1);
		return *this;
	}
	for (_LL i = 0; i < shapeLen - 2; i++)
	{
		if (dataShape[i] != other.dataShape[i])
		{
			MyBA_Err("BA_Array BA_Array::Add(BA_Array other, bool aNew): uncompatibale shape,return *this", 1);
			return *this;
		}
	}
	float* pt1 = dataF, * pt2 = other.dataF;
	for (_LL i = 0; i < dataLen; i++, pt1++, pt2++)
		*pt1 = (*pt1) * (*pt2);
	return *this;
}

BA_Array BA_Array::Eq(float other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) == (_LL)(other) ? 1 : 0;
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) == (_LL)(other) ? 1 : 0;
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) == (other) ? 1.f : 0.f;
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) == (other) ? 1.f : 0.f;
		}
	}
	return *this;
}

BA_Array BA_Array::Sum(void)
{
	if (type == 'l')
	{
		_LL* pt = dataL;
		dataSumL = 0;
		for (_LL i = 0; i < dataLen; i++, pt++)
			dataSumL += *pt;
	}
	else//selfType == 'f'
	{
		float* pt = dataF;
		dataSumF = 0.;
		for (_LL i = 0; i < dataLen; i++, pt++)
			dataSumF += *pt;
	}
	return *this;
}

BA_Array BA_Array::Reshape(BA_Shape newShape, bool aNew)
{
	_LL newShapeDataLen = 1;
	for (_LL i = 0; i < newShape.shapeLen; i++)
		newShapeDataLen *= newShape.shape[i];
	if (newShapeDataLen != dataLen)
	{
		MyBA_Err("BA_Array BA_Array::Reshape(BA_Shape newShape, bool aNew):uncompatibable shape,return *this", 1);
	}
	else
	{
		if (aNew)
		{
			return BA_Array(newShape, &type);
		}
		else
		{
			MyBA_Free(dataShape, mem);
			dataShape = BALLOC_R(newShape.shapeLen, _LL, mem);
			shapeLen = newShape.shapeLen;
			for (_LL i = 0; i < newShape.shapeLen; i++)
				dataShape[i] = newShape.shape[i];
		}
	}
	return *this;
}

BA_Array BA_Array::Ge(float other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_LL)0);
			_LL* pt1 = dataL, * pt3 = ret.dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) >= (_LL)(other) ? 1 : 0;
			return ret;
		}
		else
		{
			_LL* pt1 = dataL;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) >= (_LL)(other) ? 1 : 0;
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) >= (other) ? 1.f : 0.f;
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) >= (other) ? 1.f : 0.f;
		}
	}
	return *this;
}

BA_Array BA_Array::Seq(float from, float by)
{
	if (type == 'f')
	{
		float* pt1 = dataF;
		float nowVal = from;
		for (_LL i = 0; i < dataLen; i++, pt1++, nowVal += by)
			*pt1 = nowVal;
	}
	else
	{
		MyBA_Err("BA_Array BA_Array::Seq(float from, float by): unsupport for type = 'l',do nothing", 1);
	}
	return *this;
}

BA_Array BA_Array::Func(float (*Func)(float* pt), bool aNew)
{
	if (type == 'f')
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = Func(pt1);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = Func(pt1);
		}
	}
	else
	{
		MyBA_Err("BA_Array BA_Array::Func(float (*Func)(float* pt), bool aNew): unsupport for type = 'l',do nothing", 1);
	}
	return *this;
}

BA_Array BA_Array::Maps(float(*MapFunc)(float* pt, void* p), void* p, bool aNew)
{
	if (type == 'f')
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = MapFunc(pt1, p);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_LL i = 0; i < dataLen; i++, pt1++)
				*pt1 = MapFunc(pt1, p);
		}
	}
	else
	{
		MyBA_Err("BA_Array BA_Array::Func(float (*Func)(float* pt), bool aNew): unsupport for type = 'l',do nothing", 1);
	}
	return *this;
}

char* BA_Array::Str(bool toStr, bool printOut)
{
	char* pc = NULL;
	printf("BA_Array: Len=%llu, Shape=[%s]\n", dataLen, "-");
	for (_LL idx = 0; idx < dataLen; idx++)
	{
		printf("%10.5f, ", dataF[idx]);
	}
	printf("\n");
	return nullptr;
}

std::any BA_Array::operator[](_LL idx)
{
	if (idx < 0 && idx >= (-dataLen))
	{
		idx = dataLen + idx;
	}
	else if (idx < (-dataLen))
	{
		MyBA_Err("BA_Array BA_Array::operator[](_LL idx): idx < (-dataLen),return NAN;", 1);
		return NAN;
	}

	if (type == 'f')
		return dataF[idx];
	else
		return dataL[idx];
}

std::any BA_Array::operator[](_ULL idx)
{
	if (type == 'f')
		return dataF[idx];
	else
		return dataL[idx];
}

std::any BA_Array::operator()(_LL idx1, ...)
{
	return std::any();
}
