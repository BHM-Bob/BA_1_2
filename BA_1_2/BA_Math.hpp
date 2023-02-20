//BASIC_ALL_1_2
//H
//Writen By BHM
//2021年11月18日 23点03分

//#define USE_OPENCV

#ifndef BA_MATH_H
#define BA_MATH_H

#include"BA_Base.hpp"
#include"BA_Mem.hpp"

//TODO : think about https://eigen.tuxfamily.org/index.php?title=Main_Page
	//or just using OpenCV


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

		Ty errV = Ty();
	public:
		std::vector<_LL> shape;
		_LL len = 1;
		Ty* data = NULL;
		Ty* lastAddress = NULL;

		tensor(std::vector<_LL> _shape = {0}, Ty defaultValue = Ty());
		~tensor();
		//recreate -> memset

		template<typename toTy>
		toTy sum(toTy ori);

		char* str(bool toStr = false, bool printOut = true);
		tensor<Ty>& sub(_LL from, _LL to);
		//share same data in mem
		tensor<Ty>& refsub(_LL from, _LL to);
		template<typename maskTy>
		tensor<Ty>& masksub(tensor<maskTy> mask);
		tensor<Ty>& concat(std::vector<Ty> other);

		void setErrValue(Ty _errV);

		tensor<Ty>& operator[](_LL idx);
		Ty& operator()(_LL idx);
		void operator=(std::vector<Ty> data);

		//func will be a Lambda as auto func = [&](Ty i){return (toTy)i;};
		template<typename toTy>
		tensor<toTy>& cast(toTy defaultValue);

		//func must be a return of std::bind(callableFunc)
		//    and toTy callableFunc(Ty i){}
		//or func must be a Lambda as auto func = [&](Ty i){return (toTy)i;};
		template<typename toTy, typename funcTy>
		tensor<toTy>& cast(toTy defaultValue, funcTy func);

		//func must be a return of std::bind(callableFunc, std::_PlaceHolder _1)
		//    and Ty callableFunc(Ty* r){}
		//or func must be a Lambda as auto func = [&](Ty* r){}
		template<typename funcTy>
		tensor<Ty>& selfmap(funcTy func);

		//func must be a return of std::bind(callableFunc, std::_PlaceHolder _1)
		//    and Ty callableFunc(Ty r, Ty l){}
		//or func must be a Lambda as auto func = [&](Ty r){other}
		template<typename funcTy>
		tensor<Ty>& map(funcTy func);

		//func must be a return of std::bind(callableFunc)
		//    and Ty callableFunc(Ty* r, otherTy* l){}
		//or func must be a Lambda as auto func = [&](Ty* r, otherTy* l){}
		template<typename otherTy, typename funcTy>
		tensor<Ty>& selfmap(tensor<otherTy>& other, funcTy func);

		//func must be a return of std::bind(callableFunc)
		//    and Ty callableFunc(Ty r, Ty l){}
		//or func must be a Lambda as auto func = [&](Ty r, Ty l){}
		template<typename funcTy>
		tensor<Ty>& map(tensor<Ty>& other, funcTy func);

		//func must be a return of std::bind(callableFunc)
		//    and toTy callableFunc(Ty r, otherTy l){}
		//or func must be a Lambda as auto func = [&](Ty r, otherTy l){}
		template<typename toTy, typename otherTy, typename funcTy>
		tensor<toTy>& map(toTy defaultValue, tensor<otherTy>& other, funcTy func);

		tensor<Ty>& operator+(Ty& other);
		tensor<Ty>& operator+(tensor<Ty>& other);
		tensor<Ty>& operator-(Ty& other);
		tensor<Ty>& operator-(tensor<Ty>& other);
		tensor<Ty>& operator*(Ty& other);
		tensor<Ty>& operator*(tensor<Ty>& other);
		tensor<Ty>& operator/(Ty& other);
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
				lastAddress = data + len;
				Ty* tmp = data;
				for (_LL i = 0; i < len; i++, tmp++)
					*tmp = defaultValue;
			}
		}
	}
	template<typename Ty>
	inline tensor<Ty>::~tensor()
	{
	}
	template<typename Ty>
	inline tensor<Ty>& tensor<Ty>::sub(_LL from, _LL to)
	{
		if (shape.size() != 1 || from >= to || from < 0 || to > len)
		{
			MyBA_Err("tensor<Ty>& tensor<Ty>::sub:shapeLen != 1 || from >= to || from < 0 || to > dataLen, return *this", 1);
			return *this;
		}
		tensor<Ty>* ret = new tensor<Ty>({ to - from });
		memcpy_s(ret->data, ret->len * sizeof(Ty),
			data + from, (to - from) * sizeof(Ty));
		return *ret;
	}
	template<typename Ty>
	inline tensor<Ty>& tensor<Ty>::refsub(_LL from, _LL to)
	{
		if (shape.size() != 1 || from >= to || from < 0 || to > len)
		{
			MyBA_Err("tensor<Ty>& tensor<Ty>::sub:shapeLen != 1 || from >= to || from < 0 || to > dataLen, return *this", 1);
			return *this;
		}
		tensor<Ty>* ret = new tensor<Ty>({ 0 });
		ret->shape = shape;
		ret->len = len;
		ret->data = data + from;
		ret->lastAddress = data + to;
		return *ret;
	}
	template<typename Ty>
	template<typename toTy>
	inline toTy tensor<Ty>::sum(toTy ori)
	{
		for (Ty* pt = data; pt != lastAddress; pt++)
			ori += (toTy)*pt;
		return ori;
	}
	template<typename Ty>
	template<typename maskTy>
	inline tensor<Ty>& tensor<Ty>::masksub(tensor<maskTy> mask)
	{
		if (shape != mask.shape)
		{
			MyBA_Err("tensor<Ty>& tensor<Ty>::masksub(tensor<maskTy> mask)::shape != mask.shape, skip", 1);
			return *this;
		}
		_LL newLen = mask.sum(0);
		tensor<Ty>* ret = new tensor<Ty>({ newLen });
		for (_LL i = 0, j = 0; i < len; i++)
			if ((bool)mask.data[i])
				ret->data[j++] = data[i];
		return *ret;
	}
	template<typename Ty>
	inline tensor<Ty>& tensor<Ty>::concat(std::vector<Ty> other)
	{
		if (shape != other.shape)
		{
			MyBA_Err("tensor<Ty>& tensor<Ty>::concat:shape != other.shape", 1);
			return *this;
		}
		//TODO : imp
	}
	template<typename Ty>
	inline void tensor<Ty>::setErrValue(Ty _errV)
	{
		errV = _errV;
	}
	template<typename Ty>
	inline tensor<Ty>& tensor<Ty>::operator[](_LL idx)
	{
		if ((size_t)nowAxis < shape.size() && idx < shape[nowAxis])
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
	//TODO : 修改为(_LL idx1, ...)
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
	template<typename toTy>
	inline tensor<toTy>& tensor<Ty>::cast(toTy defaultValue)
	{
		tensor<toTy>* pt = new tensor<toTy>(shape);
		Ty* pt1 = data;
		toTy* pt3 = pt->data;
		for (_LL i = 0; i < len; i++, pt1++, pt3++)
			*pt3 = (toTy)(*pt1);
		return *pt;
	}
	template<typename Ty>
	template<typename toTy, typename funcTy>
	inline tensor<toTy>& ba::tensor<Ty>::cast(toTy defaultValue, funcTy func)
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
	inline tensor<Ty>& ba::tensor<Ty>::selfmap(funcTy func)
	{
		Ty* pt1 = data;
		for (_LL i = 0; i < len; i++, pt1++)
			func(pt1, i);
		return *this;
	}
	template<typename Ty>
	template<typename funcTy>
	inline tensor<Ty>& ba::tensor<Ty>::map(funcTy func)
	{
		tensor<Ty>* pt = new tensor<Ty>(shape);
		Ty* pt1 = data;
		Ty* pt3 = pt->data;
		for (_LL i = 0; i < len; i++, pt1++, pt3++)
			*pt3 = func(*pt1);
		return *pt;
	}
	template<typename Ty>
	template<typename otherTy, typename funcTy>
	inline tensor<Ty>& tensor<Ty>::selfmap(tensor<otherTy>& other, funcTy func)
	{
		if (shape == other.shape)
		{
			Ty* pt1 = data;
			otherTy* pt2 = other.data;
			for (_LL i = 0; i < len; i++, pt1++, pt2++)
				func(pt1, pt2, i);
		}
		else
		{
			MyBA_Err("inline void tensor<Ty>::selfmap(tensor<otherTy>& other, funcTy func)::shape != other.shape, skip", 1);
		}
		return *this;
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
	template<typename toTy, typename otherTy, typename funcTy>
	inline tensor<toTy>& ba::tensor<Ty>::map(toTy defaultValue, tensor<otherTy>& other, funcTy func)
	{
		if (shape == other.shape)
		{
			tensor<toTy>* pt = new tensor<toTy>(shape);
			Ty* pt1 = data;
			otherTy* pt2 = other.data;
			toTy* pt3 = pt->data;
			for (_LL i = 0; i < len; i++, pt1++, pt2++, pt3++)
				*pt3 = func(*pt1, *pt2);
			return *pt;
		}
		MyBA_Err("inline tensor<toTy>& ba::tensor<Ty>::map(toTy defaultValue, tensor<otherTy>& other, funcTy func)::shape != other.shape, return defaultValue tensor, shape same as this->shape", 1);
		tensor<toTy>* r = new tensor<toTy>(shape, defaultValue);
		return *r;
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator+(Ty& other)
	{
		return this->map(other, [&](Ty r) {return r + other; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator+(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r + l; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator-(Ty& other)
	{
		return this->map([&](Ty r) {return r - other; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator-(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r - l; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator*(Ty& other)
	{
		return this->map([&](Ty r) {return r * other; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator*(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r * l; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator/(Ty& other)
	{
		return this->map([&](Ty r) {return r / other; });
	}
	template<typename Ty>
	inline tensor<Ty>& ba::tensor<Ty>::operator/(tensor<Ty>& other)
	{
		return this->map(other, [&](Ty r, Ty l) {return r / l; });
	}


	template<typename LessPtrTy, typename BaseTy>
	class array : public BA_Base
	{
	public:
		_LL nowAxis = -1;//read-only
		std::vector<_LL> axis;//read-only
		std::vector<_LL> shape;
		LessPtrTy* data = NULL;

		array(std::vector<_LL> _shape = { 0 });
		array(LessPtrTy* _data, std::vector<_LL> _shape);
		~array();

		// func should be a lambda with tow para : std::vector<_LL> axis, BaseTy dataElement
		// if want to change the value of data, switch to  BaseTy& dataElement
		template<typename FuncTy>
		void map(FuncTy func);
	};


	template<typename LessPtrTy, typename BaseTy>
	inline array<LessPtrTy, BaseTy>::array(std::vector<_LL> _shape)
	{
		data = ba::allocNDArray<LessPtrTy, BaseTy>(_shape, mem);
		shape = _shape;
		axis.resize(shape.size());
	}
	template<typename LessPtrTy, typename BaseTy>
	inline array<LessPtrTy, BaseTy>::array(LessPtrTy* _data, std::vector<_LL> _shape)
	{
		data = _data;
		shape = _shape;
		axis.resize(shape.size());
	}
	template<typename LessPtrTy, typename BaseTy>
	inline array<LessPtrTy, BaseTy>::~array()
	{
	}

	template<typename Ty, typename BaseTy, typename FuncTy>
	inline void arrayMap_loop(Ty* highLeverPtr, std::vector<_LL> shape, int nowDim, FuncTy func,
		BaseTy tmpInducVal, std::vector<_LL> axis)
	{
		if constexpr (std::is_same_v<Ty, BaseTy>)
		{
			for (_ULL i = 0; i < shape[nowDim]; i++)
			{
				axis[nowDim] = i;
				func(axis, highLeverPtr[i]);
			}
		}
		else
		{
			for (_ULL i = 0; i < shape[nowDim]; i++)
			{
				axis[nowDim] = i;
				arrayMap_loop(highLeverPtr[i], shape, nowDim + 1, func, tmpInducVal, axis);
			}
		}
	}
	template<typename LessPtrTy, typename BaseTy>
	template<typename FuncTy>
	inline void array<LessPtrTy, BaseTy>::map(FuncTy func)
	{
		arrayMap_loop<LessPtrTy, BaseTy, FuncTy>(data, shape, 0, func, BaseTy(), axis);
	}
}

#endif

