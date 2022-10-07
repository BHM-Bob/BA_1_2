//BASIC_ALL_1_2
//H
//Writen By BHM
//2021年11月18日 23点03分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_MATH_H
#define BA_MATH_H

#include"BA_Base.hpp"

void* MyBA_ZerosD(int* shape, int dims);

class BA_Shape
{
public:
	_LL shapeLen;
	_LL* shape;

	BA_Shape(_LL len, ...);
	BA_Shape(_LL len, _LL* _shape);
	BA_Shape(_LL* _shape,_LL len);
	char* Str(bool toStr = false, bool printOut = true);
};

class BA_Array
{
public:
	_LL* dataL = NULL;
	float* dataF = NULL;

	_LL dataSumL = 0;
	float dataSumF = 0.f;

	char type = 'f';//f = float, l = _LL
	_LL dataLen = 0;

	_LL* dataShape = NULL;
	_LL shapeLen = 0;

	List* mem = List_Init();

	BA_Array(BA_Shape _shape, _LL content);
	BA_Array(BA_Shape _shape, float content);
	BA_Array(BA_Shape _shape, float* content);
	//way=="rand" item is 0~1 randomly,
	//way==type just create the type of array
	BA_Array(BA_Shape _shape, const char* way);

	BA_Array ReCreate(BA_Shape _shape, const char* way);

	BA_Array To(char toType, bool aNew);

	//as pytorch, dims can be [2,1,4,1]
	BA_Array Repeat(BA_Shape dims, bool aNew);
	BA_Array Concat(BA_Array a, int _dim);
	BA_Array Sub(_LL from, _LL to);

	BA_Array Add(BA_Array other, bool aNew);
	BA_Array Add(_LL other, bool aNew);
	BA_Array Add(float other, bool aNew);

	BA_Array Reduc(BA_Array other, bool aNew);
	BA_Array Reduc(_LL other, bool aNew);
	BA_Array Reduc(float other, bool aNew);

	BA_Array Mul(BA_Array other, bool aNew);
	BA_Array Mul(_LL other, bool aNew);
	BA_Array Mul(float other, bool aNew);

	BA_Array Devide(BA_Array other, bool aNew);
	BA_Array Devide(_LL other, bool aNew);
	BA_Array Devide(float other, bool aNew);

	BA_Array MatMul(BA_Array other);

	BA_Array Eq(float other, bool aNew);
	BA_Array Ge(float other, bool aNew);

	BA_Array Seq(float from, float by);
	BA_Array Func(float (*Func)(float* pt), bool aNew);
	BA_Array Maps(float (*MapFunc)(float* pt, void* p), void* p, bool aNew);

	BA_Array Sum(void);

	BA_Array Reshape(BA_Shape newShape,bool aNew);

	char* Str(bool toStr = false, bool printOut = true);

	template<typename dataType>
	BA_Array operator+(dataType other);
	template<typename dataType>
	BA_Array operator-(dataType other);
	template<typename dataType>
	BA_Array operator*(dataType other);
	template<typename dataType>
	BA_Array operator/(dataType other);
	std::any operator[](_LL idx);
	std::any operator[](_ULL idx);
	std::any operator()(_LL idx1, ...);

};
//***********************************************************************************************************************


template<typename dataType>
inline BA_Array BA_Array::operator+(dataType other)
{
	return this->Add(other, true);
}

template<typename dataType>
inline BA_Array BA_Array::operator-(dataType other)
{
	return this->Reduc(other, true);
}

template<typename dataType>
inline BA_Array BA_Array::operator*(dataType other)
{
	return this->Mul(other, true);
}

template<typename dataType>
inline BA_Array BA_Array::operator/(dataType other)
{
	return this->Devide(other, true);
}

#endif

