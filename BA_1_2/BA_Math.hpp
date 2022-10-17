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

namespace ba
{
	//shape as [n, m] mean first(top) axis is n, last(bottom) is m
	template<typename Ty>
	class tensor
	{
	private:
		List* mem = List_Init();
		std::vector<_LL> axis;
		_LL nowAxis = -1;
		_LL len = 1;

		Ty errV = Ty();
	public:
		std::vector<_LL> shape;
		Ty* data = NULL;

		tensor(std::vector<_LL> _shape, Ty defaultValue = Ty());
		void setErrValue(Ty _errV);

		tensor<Ty>& operator[](_LL idx);
		Ty& operator()(_LL idx);
		void operator=(std::vector<Ty> data);

		//func must be a return of std::bind(callableFunc)
		//    and toTy callableFunc(Ty i){}
		//or func must be a Lambda as auto func = [&](Ty i){return (toTy)i;};
		template<typename toTy, typename funcTy>
		tensor<toTy>& toType(toTy defaultValue, funcTy func);

		//func must be a return of std::bind(callableFunc)
		//    and Ty callableFunc(Ty r, Ty l){}
		//or func must be a Lambda as auto func = [&](Ty r, Ty l){}
		template<typename funcTy>
		tensor<Ty>& map(tensor<Ty>& other, funcTy func);

		//func must be a return of std::bind(callableFunc)
		//    and Ty callableFunc(Ty r, Ty l){}
		//or func must be a Lambda as auto func = [&](Ty r, Ty l){}
		template<typename funcTy, typename otherTy, typename toTy>
		tensor<toTy>& map(toTy defaultValue, tensor<otherTy>& other, funcTy func);

		tensor<Ty>& operator+(tensor<Ty>& other);
		tensor<Ty>& operator-(tensor<Ty>& other);
		tensor<Ty>& operator*(tensor<Ty>& other);
		tensor<Ty>& operator/(tensor<Ty>& other);
	};
	template<typename Ty>
	inline tensor<Ty>::tensor(std::vector<_LL> _shape, Ty defaultValue)
	{
		if (_shape.size() == 0)
		{
			MyBA_Err("tensor<Ty>::tensor(std::vector<_LL> _shape, Ty defaultValue)::null shape", 1);
		}
		else
		{
			nowAxis = 0;
			shape = _shape;
			axis = std::vector<_LL>(shape.size(), -1);
			for (_LL i : shape)
				len *= (i > 0 ? i : 0);
			if (len == 0)
			{
				MyBA_Err("tensor<Ty>::tensor(std::vector<_LL> _shape, Ty defaultValue)::negative shape", 1);
			}
			else
			{
				data = new Ty[len];
				Ty* tmp = data;
				for (_LL i = 0; i < len; i++, tmp++)
					*tmp = defaultValue;
			}
		}
	}
	template<typename Ty>
	inline void tensor<Ty>::setErrValue(Ty _errV)
	{
		errV = _errV;
	}
	template<typename Ty>
	inline tensor<Ty>& tensor<Ty>::operator[](_LL idx)
	{
		if (nowAxis < shape.size() && idx < shape[nowAxis])
		{
			axis[nowAxis] = idx;
			++nowAxis;
		}
		else
		{
			MyBA_Err("tensor<Ty>& tensor<Ty>::operator[](_LL idx)::can not apply [] in Ty, use ()", 1);
		}
		return *this;
	}
	template<typename Ty>
	inline Ty& tensor<Ty>::operator()(_LL idx)
	{
		if (nowAxis == shape.size()-1)
		{
			axis[nowAxis] = idx;
			_LL len1 = 0, axisSize = 1;
			for (_LL i  = shape.size()-1; i >= 0; i--)
			{
				len1 += ((axis[i]-1) * axisSize);
				axisSize *= shape[i];
			}
			nowAxis = 0;
			std::fill(std::begin(axis), std::end(axis), -1);
			return data[len1];
		}
		else
		{
			MyBA_Err("tensor<Ty>& tensor<Ty>::operator[](_LL idx)::can apply [] in Ty, use ()", 1);
			return errV;
		}
	}
	template<typename Ty>
	inline void tensor<Ty>::operator=(std::vector<Ty> data)
	{
		nowAxis = 0;
		shape = {data.size()};
		axis = std::vector<_LL>(shape.size(), -1);
		len = shape[0];
		data = new Ty[len];
		Ty* tmp = data;
		for (Ty t : data)
		{
			*tmp = t;
			++tmp;
		}
	}
	template<typename Ty>
	template<typename toTy, typename funcTy>
	inline tensor<toTy>& ba::tensor<Ty>::toType(toTy defaultValue, funcTy func)
	{
		tensor<toTy>* pt = new tensor<toTy>(shape);
		Ty* pt1 = data;
		toTy* pt3 = pt->data;
		for (_LL i = 0; i < len; i++, pt1++, pt3++)
			*pt3 = func(*pt1);
		return *pt;
	}
	template<typename Ty>
	template<typename funcTy>
	inline tensor<Ty>& ba::tensor<Ty>::map(tensor<Ty>& other, funcTy func)
	{
		if (shape == other.shape)
		{
			tensor<Ty>* pt = new tensor<Ty>(shape);
			Ty* pt1 = data, * pt2 = other.data, * pt3 = pt->data;
			for (_LL i = 0; i < len; i++, pt1++, pt2++, pt3++)
				*pt3 = func(*pt1, *pt2);
			return *pt;
		}
		MyBA_Err("inline tensor<Ty>& ba::tensor<Ty>::map(tensor<Ty>& other, funcTy func):: shape != other.shape, return *this", 1);
		return *this;
	}
	template<typename Ty>
	template<typename funcTy, typename otherTy, typename toTy>
	inline tensor<toTy>& ba::tensor<Ty>::map(toTy defaultValue, tensor<otherTy>& other, funcTy func)
	{
		if (shape == other.shape)
		{
			tensor<toTy>* pt = new tensor<toTy>(shape);
			Ty* pt1 = data;
			otherTy* pt2 = other.data;
			toTy *pt3 = pt->data;
			for (_LL i = 0; i < len; i++, pt1++, pt2++, pt3++)
				*pt3 = func(*pt1, *pt2);
			return *pt;
		}
		MyBA_Err("inline tensor<Ty>& ba::tensor<Ty>::map(tensor<Ty>& other, funcTy func)::shape != other.shape, return defaultValue tensor, shape same as this->shape", 1);
		tensor<toTy>* r = new tensor<toTy>(this->shape, defaultValue);
		return *r;
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator+(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r + l; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator-(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r - l; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator*(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r * l; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator/(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r / l; });
	}
}

#endif

