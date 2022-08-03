//BASIC_ALL_1_2
//H
//Writen By BHM
//2021年11月18日 23点03分

//#define USE_OPENCV
//#define USE_WINDOWS

#ifndef BA_MATH_H
#define BA_MATH_H

#include"BA_Base.hpp"

float abs_f(float a);
double abs_d(double a);

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
	_LL* dataL;
	float* dataF;

	_LL dataSumL;
	float dataSumF;

	char type;//f = float, l = _LL
	_LL dataLen;

	_LL* dataShape;
	_LL shapeLen;

	List* mem;

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
	any operator[](_LL idx);
	any operator()(_LL idx1, ...);

};

//void* mymax(char* ptype, int sum, ...);//配合atoi等
//stata == 0 未被激活

//***********************************************************************************************************************
//My Full Conect Net Deep Learning Platform (in C or C++)

typedef struct LOFE_SubChain LOFE_SubChain;
struct LOFE_SubChain//每个output层的tensor所对应的chain
{
	List** wchain;//一共有pt的bind_tensor个List*
	List** bchain;
};

typedef struct LOFE_Tensor LOFE_Tensor;
struct LOFE_Tensor
{
	float num;
	float* w;
	float* b;
	float* wnew;
	float* bnew;
	LOFE_SubChain** subchain;//一级是output->tensor个
	_ULL bind_num;
	_ULL next_bind_num;
	LOFE_Tensor* bind_tensor;//multi tensors in the pre layer
	LOFE_Tensor* next_bind_tensor;//multi tensors in the next layer
	bool isdropout;//Dropout
};

typedef struct LOFE_Layer LOFE_Layer;
struct LOFE_Layer
{
	char* layerlname;
	_ULL layerid;//from 1
	_ULL bindlayerid;//from 1
	float dropout;
	_ULL sumtensor;
	LOFE_Tensor* tensor;

	LOFE_Layer* (*Func)(LOFE_Layer*);//Activation,maybe should has it
	void (*Bind)(LOFE_Layer*, LOFE_Layer*);//Bind two layers
};

typedef struct LOFE_Chain LOFE_Chain;
struct LOFE_Chain//每个output层的tensor所对应的chain
{
	_ULL sumwchaindot;
	_ULL sumbchaindot;
	_ULL sumchaindot;
	List** pchain;//为输出层单tensor对应总数chain个List*,每个List是一个关系式的容器
	float* pdif;//每个chain的计算值
};

typedef struct LOFE_Model LOFE_Model;
struct LOFE_Model
{
	char* modelname;
	float lr;
	_ULL sumlayer;
	float loss;
	LOFE_Layer* input_layer;
	LOFE_Layer* output_layer;
	LOFE_Layer* ynum_layer;
	List* layers;

	float* pd;//output - ylabel

	LOFE_Chain* chain;

	_ULL trlen;
	_ULL telen;
	float** train_ds;
	float** test_ds;

	_ULL trained_epoch;

	List* mem;

	LOFE_Model* (*AddLayer)(LOFE_Model*, const char*, unsigned long, float, LOFE_Layer* (*)(LOFE_Layer*));
	LOFE_Model* (*Save)(LOFE_Model*, char*);
	LOFE_Model* (*Read)(char*);
};

LOFE_Model* LOFE_CreatNULLModel(const char* name, float lr);
LOFE_Model* LOFE_AddLayer(LOFE_Model* model, const char* name, unsigned long sumtensor, float dropout, LOFE_Layer* (*Func)(LOFE_Layer*));
LOFE_Layer* LOFE_InitTensor(LOFE_Model* model, LOFE_Layer* layer);
//from1  layer1->layer2
LOFE_Model* LOFE_BindLayers(LOFE_Model* model, _ULL layer1index, _ULL layer2index);

LOFE_Model* LOFE_SaveModel(LOFE_Model* model, char* ppath);
LOFE_Model* LOFE_ReadModel(char* ppath);

LOFE_Model* LOFE_AutoBind(LOFE_Model* model);
LOFE_Model* LOFE_BindLayersById(LOFE_Model* model);
LOFE_Model* LOFE_BuildModel(LOFE_Model* model);

LOFE_Model* LOFE_CompileModel(LOFE_Model* model);
LOFE_Chain* LOFE_CompileModel_CacuSum(LOFE_Model* model, LOFE_Chain* chain);
LOFE_Model* LOFE_CompileModel_W(LOFE_Model* model, LOFE_Chain* chain);
LOFE_Model* LOFE_CompileModel_B(LOFE_Model* model, LOFE_Chain* chain);
_ULL LOFE_CompileModel_W_CacuTimes(LOFE_Tensor* pt, _ULL* psum);
//k为sumlayer-层序号(from1)
LOFE_Chain* LOFE_CompileModel_B_PutB(LOFE_Chain* chain, float* pw, float* pb, _ULL k, _ULL* bias);
bool LOFE_CompileModel_B_PutB_IsEd(List** chain, _ULL his, _ULL bias, _LL adr, _ULL j);
_LL LOFE_CompileModel_QueryAdr(List* chain, void* adr);
LOFE_Model* LOFE_CompileModel_PutChain(LOFE_Model* model, LOFE_Chain* chain, _ULL t);

// xi = xi / max ;return the max
float* LOFE_Norm(LOFE_Model* model, _ULL sum, float* num, bool is0center);
LOFE_Layer* LOFE_BatchNorm(LOFE_Layer* layer);

//  or /*y = x + |x|*/
LOFE_Layer* LOFE_Relu(LOFE_Layer* layer);
LOFE_Layer* LOFE_LeakyRelu(LOFE_Layer* layer);
LOFE_Layer* LOFE_Softmax(LOFE_Layer* layer);

LOFE_Model* LOFE_CreateTrDS(LOFE_Model* model, _ULL index, _LL sum, ...);
LOFE_Model* LOFE_CreateTeDS(LOFE_Model* model, _ULL index, _LL sum, ...);

//sum<0 means a float array which is size of -sum
LOFE_Layer* LOFE_FillLayer(LOFE_Layer* pl, _LL sum, ...);

//仅前向传播
LOFE_Layer* LOFE_Predict(LOFE_Model* model);

//计算loss
LOFE_Model* LOFE_Loss(LOFE_Model* model);

//将各函数集合，计算单个样本并进行梯度下降
LOFE_Layer* LOFE_Train(LOFE_Model* model);
//应用梯度
LOFE_Model* LOFE_Train_ApllyG(LOFE_Model* model);
//计算所有梯度
LOFE_Model* LOFE_Train_CacuChain(LOFE_Model* model);
//引用并返回单个w or b涉及的梯度
float LOFE_Train_CacuSubChain(LOFE_Model* model, LOFE_Tensor* pt, float* p, bool isw, _ULL k);
LOFE_Model* LOFE_Train_UpdateDropout(LOFE_Model* model);

LOFE_Model* LOFE_TrainDS(LOFE_Model* model);

bool LOFE_FreeModel(LOFE_Model* model);
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

