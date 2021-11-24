//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021年11月18日 23点35分

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_math.hpp"

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

int* MyBA_Zeros_AllocD(int* ret, int* shape, int dims,int nowdim)
{
	ret = (int*)calloc(shape[nowdim], sizeof(int));
	for (int i = 0; i < shape[nowdim] && nowdim < dims-1; i++, ret++)
		MyBA_Zeros_AllocD((int*)ret, shape, dims, nowdim + 1);
	return ret;
}

void* MyBA_ZerosD(int* shape, int dims)
{
	void* ret = NULL;
	return MyBA_Zeros_AllocD((int*)ret,shape, dims,0);
}

//***********************************************************************************************************************
//My Liner Deep Learning Platform (for C or C++)

LOFE_Model* LOFE_CreatNULLModel(const char* name, float lr)
{
	BALLOCS_L(LOFE_Model, pret, 1,  NULL,);
	pret->modelname = mstrdup(name);
	pret->lr = lr;
	pret->sumlayer = 0;

	pret->layers = List_Init();
	pret->mem = List_Init();

	pret->AddLayer = LOFE_AddLayer;
	pret->Read = LOFE_ReadModel;
	pret->Save = LOFE_SaveModel;

	return pret;
}

LOFE_Model* LOFE_AddLayer(LOFE_Model* model, const char* name, unsigned long sumtensor, float dropout, LOFE_Layer* (*Func)(LOFE_Layer*))
{
	model->sumlayer += 1;
	BALLOCS_R(LOFE_Layer, p,1,  model->mem,NULL,);
	p->layerlname = mstrdup(name);
	p->layerid = model->sumlayer;//from 1
	p->bindlayerid = 0;
	p->tensor = BALLOC_R(sumtensor, LOFE_Tensor, model->mem);
	p->sumtensor = sumtensor;
	p->dropout = dropout;

	if (Func == NULL)
		p->Func = LOFE_Relu;
	else
		p->Func = Func;

	model->layers->Put(model->layers, p);

	return model;
}

LOFE_Layer* LOFE_InitTensor(LOFE_Model* model, LOFE_Layer* layer)
{
	LOFE_Tensor* p = layer->tensor;
	float* pw = NULL;
	float* pb = NULL;
	for (_ULL i = 0; i < layer->sumtensor; i++, p++)
	{
		if ((rand() % 100) < (layer->dropout * 100))
			p->isdropout = 1;
		p->w = BALLOC_R(p->bind_num + 1, float, model->mem);
		p->b = BALLOC_R(p->bind_num + 1, float, model->mem);
		pw = p->w;
		pb = p->b;
		int a = 0;
		float b = 0.f;
		for (_ULL js = 0; js < p->bind_num; js++, pw++, pb++)
		{
			*pw = sin(rand());
			*pb = sin(rand());
		}
	}
	return layer;
}

LOFE_Model* LOFE_Train_UpdateDropout(LOFE_Model* model)
{
	LOFE_Tensor* pt = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_Copy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_Copy(model->layers)))
	{
		if (pl->dropout > 0)
		{
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
			{
				if ((rand() % 100) < (pl->dropout * 100))
					pt->isdropout = 1;
				else
					pt->isdropout = 0;
			}
		}
	}
	return model;
}

LOFE_Model* LOFE_BindLayers(LOFE_Model* model, _ULL layer1index, _ULL layer2index)
{
	LOFE_Layer* layer1 = (LOFE_Layer*)(model->layers->Index(model->layers, layer1index - 1));
	LOFE_Layer* layer2 = (LOFE_Layer*)(model->layers->Index(model->layers, layer2index - 1));
	layer2->bindlayerid = layer1->layerid;
	LOFE_Tensor* p = layer2->tensor;
	for (_ULL js = 0; js < layer2->sumtensor; js++, p++)
	{
		p->bind_num = layer1->sumtensor;
		p->bind_tensor = layer1->tensor;
	}
	p = layer1->tensor;
	for (_ULL js = 0; js < layer1->sumtensor; js++, p++)
	{
		p->next_bind_tensor = layer2->tensor;
		p->next_bind_num = layer2->sumtensor;
	}

	LOFE_InitTensor(model, layer2);
	return model;
}

LOFE_Model* LOFE_BindLayersById(LOFE_Model* model)
{
	LOFE_Layer* layer1 = NULL;
	LOFE_Tensor* pt = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_Copy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_Copy(model->layers)))
	{
		if (pl->layerid > 1)
		{
			pl->bindlayerid = pl->layerid - 1;
			layer1 = (LOFE_Layer*)(List_Index(model->layers, pl->bindlayerid - 1));
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
			{
				pt->bind_num = layer1->sumtensor;
				pt->bind_tensor = layer1->tensor;
			}
			pt = layer1->tensor;
			for (_ULL i = 0; i < layer1->sumtensor; pt++, i++)
			{
				pt->next_bind_tensor = pl->tensor;
				pt->next_bind_num = pl->sumtensor;
			}

			LOFE_InitTensor(model, pl);
		}
	}
	return model;
}

LOFE_Model* LOFE_BuildModel(LOFE_Model* model)
{
	LOFE_Layer** ppl = BALLOC_R(model->sumlayer, LOFE_Layer*, model->mem);
	for (LOFE_Layer* p = (LOFE_Layer*)(List_Get(model->layers)); p != NULL; p = (LOFE_Layer*)(List_Get(model->layers)))
		ppl[p->bindlayerid] = p;
	for (_ULL i = 0; i < model->sumlayer; i++)
		List_Put(model->layers, (void*)ppl[i]);
	model->input_layer = ppl[0];
	model->output_layer = ppl[model->sumlayer - 1];

	_ULL i = 0;
	for (LOFE_Tensor* pt = model->input_layer->tensor; i < model->input_layer->sumtensor; i++)
	{
		pt->w = pt->b = NULL;
		pt->bind_tensor = NULL;
		pt->bind_num = 0;
	}

	model->ynum_layer = BALLOC_R(1, LOFE_Layer, model->mem);
	model->ynum_layer->layerlname = mstrdup("ynum_layer");
	model->ynum_layer->sumtensor = model->output_layer->sumtensor;
	if (model->ynum_layer->sumtensor == 0)
		return (LOFE_Model*)MyBA_Err("LOFE_BuildModel: Got 0 tensor with output layer, return NULL", 1);
	model->ynum_layer->tensor = BALLOC_R(model->ynum_layer->sumtensor, LOFE_Tensor, model->mem);
	return model;
}

LOFE_Model* LOFE_SaveModel(LOFE_Model* model, char* ppath)
{
	FILE* pf = NULL;
	fopen_s(&pf, StringAdd_S(ppath, "model.lofe", NULL), "wb");
	if (pf == NULL)
		return (LOFE_Model*)MyBA_Errs(1,"LOFE_SaveModel:Got a NULL pf with path ", ppath, "model.lofe", NULL);
	fwrite(model, sizeof(LOFE_Model), 1, pf);
	StringWrite(pf, model->modelname);
	fclose(pf);

	fopen_s(&pf, StringAdd_S(ppath, "layers.lofe", NULL), "wb");
	if (pf == NULL)
		return (LOFE_Model*)MyBA_Errs(1,"LOFE_SaveModel:Got a NULL pf with path ", ppath, "layers.lofe", NULL);
	LOFE_Tensor* pt = NULL;
	LIST_FORS(LOFE_Layer,pl,model->layers)
	{
		fwrite(pl, sizeof(LOFE_Layer), 1, pf);
		StringWrite(pf, pl->layerlname);
		fwrite(pl->tensor, sizeof(LOFE_Tensor), pl->sumtensor, pf);
		pt = pl->tensor;
		for (_ULL js = 0; js < pl->sumtensor; js++, pt++)
		{
			if (pt->bind_num > 0)
			{
				fwrite(pt->w, sizeof(float), pt->bind_num, pf);
				fwrite(pt->b, sizeof(float), pt->bind_num, pf);
			}
		}
	}
	fclose(pf);
	return model;
}

LOFE_Model* LOFE_ReadModel(char* ppath)
{
	BALLOCS_L(LOFE_Model, model, 1,  NULL,);
	FILE* pf = NULL;
	fopen_s(&pf, StringAdd_S(ppath, "model.lofe", NULL), "rb");
	if (pf == NULL)
		return (LOFE_Model*)MyBA_Errs(1,"LOFE_ReadModel:Got a NULL pf with path :", ppath, NULL);
	fread(model, sizeof(LOFE_Model), 1, pf);
	model->mem = List_Init();
	model->modelname = StringRead(pf);
	fclose(pf);

	fopen_s(&pf, StringAdd_S(ppath, "layers.lofe", NULL), "rb");
	if (pf == NULL)
		return (LOFE_Model*)MyBA_Errs(1,"LOFE_ReadModel:Got a NULL pf with path :", ppath, NULL);
	model->layers = List_Init();
	LOFE_Layer* pl = NULL;
	LOFE_Tensor* pt = NULL;
	float* ptef = NULL;
	for (_ULL i = 0; i < model->sumlayer; i++)
	{
		pl = BALLOC_R(1, LOFE_Layer, model->mem);
		fread(pl, sizeof(LOFE_Layer), 1, pf);
		pl->layerlname = StringRead(pf);
		pl->tensor = BALLOC_R(pl->sumtensor, LOFE_Tensor, model->mem);
		fread(pl->tensor, sizeof(LOFE_Tensor), pl->sumtensor, pf);
		pt = pl->tensor;
		for (_ULL js = 0; js < pl->sumtensor; js++, pt++)
		{
			if (pt->bind_num > 0)
			{
				pt->w = BALLOC_R(pt->bind_num, float, model->mem);
				pt->b = BALLOC_R(pt->bind_num, float, model->mem);
				pt->wnew = BALLOC_R(pt->bind_num, float, model->mem);
				pt->bnew = BALLOC_R(pt->bind_num, float, model->mem);
				fread(pt->w, sizeof(float), pt->bind_num, pf);
				fread(pt->b, sizeof(float), pt->bind_num, pf);
			}
			else
			{
				pt->w = (float*)0x1;//make a mark
				pt->b = (float*)0x2;//make a mark
			}
		}
		List_Put(model->layers, (void*)pl);
	}
	fclose(pf);
	return LOFE_BuildModel(LOFE_AutoBind(model));
}

LOFE_Model* LOFE_AutoBind(LOFE_Model* model)
{
	LOFE_Layer* layer1 = NULL;
	LOFE_Tensor* pt = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)List_Copy(model->layers); pl != NULL; pl = (LOFE_Layer*)List_Copy(model->layers))
	{
		if (pl->bindlayerid != 0)
		{
			layer1 = (LOFE_Layer*)(List_Index(model->layers, pl->bindlayerid - 1));
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
				pt->bind_tensor = layer1->tensor;
			pt = layer1->tensor;
			for (_ULL i = 0; i < layer1->sumtensor; pt++, i++)
				pt->next_bind_tensor = pl->tensor;
		}
	}
	return model;
}

float* LOFE_Norm(LOFE_Model* model, _ULL sum, float* num, bool is0center)// xi = xi / max ;return the max
{
	float* pret = BALLOC_R(sum, float, model->mem);
	if ((sum == 1) || (sum == 0))
		return (float*)MyBA_Err("LOFE_Norm: Got sum < 2,retrun (float)0x1", 1);
	float t = *num;
	float* pt = num + 1;
	for (_ULL i = 1; i < sum; i++, pt++)//find max
		if (t < (*pt))
			t = (*pt);
	if (t == 0)
	{
		MyBA_Err("LOFE_Norm: Got a max value with 0, try to use the -min", 1);
		t = *num;
		pt = num + 1;
		for (_ULL i = 1; i < sum; i++, pt++)//find min
			if (t > (*pt))//找到绝对值最大的
				t = (*pt);
		if (t == 0)
			return (float*)MyBA_Err("LOFE_Norm: All values are 0, return (float)0x2", 1);
		t = -t;//make the origin value still be Negative
	}
	pt = num;
	float* pte = pret;
	if (is0center)
	{
		for (_ULL i = 0; i < sum; i++, pt++, pte++)//do the Division
			(*pte) = (*pt) / t - 0.5;
	}
	else
	{
		for (_ULL i = 0; i < sum; i++, pt++, pte++)//do the Division
			(*pte) = (*pt) / t;
	}
	return pret;
}

LOFE_Layer* LOFE_BatchNorm(LOFE_Layer* layer)
{
	return layer;
}

LOFE_Layer* LOFE_Relu(LOFE_Layer* layer)
{
	_ULL sum = layer->sumtensor;
	for (LOFE_Tensor* p = layer->tensor; p != NULL; p++)
		if (p->num < 0.)
			p->num = 0.;
	return layer;
}

/*
	long i = 0;
	long j = (0xffffffff >> 1);
	for (LOFE_Tensor* p = layer->tensor; p != NULL; p = p->pnext)
	{
		i = *(long*)&(p->num);
		i = i & j;
		p->num += *(float*)&i;//By now is (float)(x + |x|)
	}
	return layer;
*/

LOFE_Layer* LOFE_LeakyRelu(LOFE_Layer* layer)
{
	return layer;
}

LOFE_Layer* LOFE_Softmax(LOFE_Layer* layer)
{
	return layer;
}

LOFE_Model* LOFE_CreateTrDS(LOFE_Model* model, _ULL index, _LL sum, ...)
{
	if (sum == 0)
		return (LOFE_Model*)MyBA_Err("LOFE_CreateTrDS: sum==0, return NULL", 1);
	va_list parg;
	va_start(parg, sum);
	if (model->train_ds == NULL)
		model->train_ds = BALLOC_R(model->input_layer->sumtensor + model->output_layer->sumtensor, float*, model->mem);
	if (sum > 0)
	{
		model->train_ds[index] = BALLOC_R(sum, float, model->mem);
		for (_ULL i = 0; i < sum; i++)
			model->train_ds[index][i] = va_arg(parg, double);
	}
	else if (sum < 0)
	{
		sum = -sum;
		model->train_ds[index] = BALLOC_R(sum, float, model->mem);
		float* p = va_arg(parg, float*);
		for (_LL i = 0; i < sum; i++)
			model->train_ds[index][i] = p[i];
	}
	va_end(parg);
	return model;
}

LOFE_Model* LOFE_CreateTeDS(LOFE_Model* model, _ULL index, _LL sum, ...)
{
	if (sum == 0)
		return (LOFE_Model*)MyBA_Err("LOFE_CreateTeDS: sum==0, return NULL", 1);
	va_list parg;
	va_start(parg, sum);
	if (model->test_ds == NULL)
		model->test_ds = BALLOC_R(model->input_layer->sumtensor + model->output_layer->sumtensor, float*, model->mem);
	if (sum > 0)
	{
		model->test_ds[index] = BALLOC_R(sum, float, model->mem);
		for (_ULL i = 0; i < sum; i++)
			model->test_ds[index][i] = va_arg(parg, double);
	}
	else if (sum < 0)
	{
		sum = -sum;
		model->test_ds[index] = BALLOC_R(sum, float, model->mem);
		float* p = va_arg(parg, float*);
		for (_LL i = 0; i < sum; i++)
			model->test_ds[index][i] = p[i];
	}
	va_end(parg);
	return model;
}

LOFE_Layer* LOFE_FillLayer(LOFE_Layer* pl, _LL sum, ...)
{
	va_list parg;
	va_start(parg, sum);
	LOFE_Tensor* pt = pl->tensor;
	if (sum > 0)
	{
		if (sum != pl->sumtensor)
			return (LOFE_Layer*)MyBA_Errs(1,"LOFE_FillLayer: sum != pl->sumtensor with layer ", pl->layerlname, "return NULL", NULL);
		for (_ULL i = 0; i < sum; i++, pt++)
			pt->num = va_arg(parg, double);
	}
	else if (sum < 0)
	{
		sum = -sum;
		if (sum != pl->sumtensor)
			return (LOFE_Layer*)MyBA_Errs(1,"LOFE_FillLayer: -sum != pl->sumtensor with layer ", pl->layerlname, "return NULL", NULL);
		float* p = va_arg(parg, float*);
		for (_LL i = 0; i < sum; i++, pt++)
			pt->num = p[i];
	}
	else
	{
		va_end(parg);
		return (LOFE_Layer*)MyBA_Errs(1,"LOFE_FillLayer : Got 0 with sum with layer ", pl->layerlname, "return NULL", NULL);
	}
	va_end(parg);
	return pl;
}

LOFE_Layer* LOFE_Predict(LOFE_Model* model)
{
	float* pw = NULL;
	float* pb = NULL;
	LOFE_Tensor* pt = NULL;
	LOFE_Tensor* pt2 = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_Copy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_Copy(model->layers)))
	{
		if (pl->bindlayerid != 0)
		{
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
			{
				if (pt->isdropout == 0)
				{
					pt->num = 0.;
					pw = pt->w;
					pb = pt->b;
					pt2 = pt->bind_tensor;
					for (_ULL j = 0; j < pt->bind_num; pt2++, pw++, pb++, j++)
						pt->num += (pt2->num) * (*pw) + (*pb);
				}
			}
		}
	}
	return model->output_layer;
}

LOFE_Model* LOFE_Loss(LOFE_Model* model)
{
	model->loss = 0.;
	LOFE_Tensor* pt1 = model->output_layer->tensor;
	LOFE_Tensor* pt2 = model->ynum_layer->tensor;
	for (_ULL i = 0; i < model->output_layer->sumtensor; i++, pt1++, pt2++)
		model->loss += 0.5 * (pt1->num - pt2->num) * (pt1->num - pt2->num);
	return model;
}


LOFE_Model* LOFE_Train_CacuChain(LOFE_Model* model)//计算所有链的值
{
	LOFE_Chain* pc = NULL;
	for (size_t k = 0; k < model->output_layer->sumtensor; k++)
	{
		pc = model->chain + k;
		for (size_t i = 0; i < pc->sumchaindot; i++)
		{
			pc->pdif[i] = 1.;
			for (float* pwt = (float*)List_Copy(pc->pchain[i]); pwt != NULL; pwt = (float*)List_Copy(pc->pchain[i]))
				pc->pdif[i] *= *pwt;
		}
	}
	return model;
}

float LOFE_Train_CacuSubChain(LOFE_Model* model, LOFE_Tensor* pt, float* p, bool isw, _ULL k)//计算dL/dw,dL/db
{
	float ret = 0.f, te = 0.f;
	_ULL i = 0;
	if (*p != 0)
	{
		if (isw)
		{
			for (size_t t = 0; t < model->output_layer->sumtensor; t++)
			{
				te = 0.;
				i = 0;
				for (_ULL* pwt = (_ULL*)List_Copy(pt->subchain[t]->wchain[k]); /*i< pt->subchain[t]->wchain[k]->sumque*/pwt != NULL;/*i++*/ pwt = (_ULL*)List_Copy(pt->subchain[t]->wchain[k]))
				{
					te += ((model->chain + t)->pdif[*pwt]);
				}
				ret += (model->pd[t]) * te / (*p);
			}
		}
		else
		{
			for (size_t t = 0; t < model->output_layer->sumtensor; t++)
			{
				te = 0.;
				i = 0;
				for (_ULL* pwt = (_ULL*)List_Copy(pt->subchain[t]->bchain[k]);  /*i < pt->subchain[t]->bchain[k]->sumque*/pwt != NULL;/*i++*/ pwt = (_ULL*)List_Copy(pt->subchain[t]->bchain[k]))
				{
					te += ((model->chain + t)->pdif[*pwt]);
				}
				ret += (model->pd[t]) * te / (*p);
			}
		}
	}
	return ret;
}

LOFE_Model* LOFE_Train_ApllyG(LOFE_Model* model)
{
	_ULL i = 0;
	float* pw = NULL, * pb = NULL, * pwnew = NULL, * pbnew = NULL;
	LOFE_Tensor* pt = NULL, * pt2 = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)))
	{
		if (pl->bindlayerid != 0)
		{
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
			{
				pw = pt->w;
				pb = pt->b;
				pwnew = pt->wnew;
				pbnew = pt->bnew;
				for (_ULL j = 0; j < pt->bind_num; pw++, pb++, pbnew++, pwnew++, j++)
				{
					if (pt->isdropout == 0)
					{
						*pwnew = *pw - (model->lr) * LOFE_Train_CacuSubChain(model, pt, pw, 1, j);
						*pbnew = *pb - (model->lr) * LOFE_Train_CacuSubChain(model, pt, pb, 0, j);
						if (isnan(*pwnew))
							*pwnew = sin(rand());
						if (isnan(*pbnew))
							*pbnew = sin(rand());
					}
				}
			}
		}
	}

	for (LOFE_Layer* pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)))
	{
		if (pl->bindlayerid != 0)
		{
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
			{
				if (pt->isdropout == 0)
				{
					pw = pt->w;
					pb = pt->b;
					pwnew = pt->wnew;
					pbnew = pt->bnew;
					for (_ULL j = 0; j < pt->bind_num; pw++, pb++, pbnew++, pwnew++, j++)
					{
						*pw = *pwnew;
						*pb = *pbnew;
					}
				}
			}
		}
	}
	return model;
}

LOFE_Layer* LOFE_Train(LOFE_Model* model)
{
	LOFE_Predict(model);//正向传播
	LOFE_Loss(model);//计算loss
	LOFE_Train_CacuChain(model);//计算所有反向传播关系式的值

	LOFE_Tensor* pt = model->output_layer->tensor;
	LOFE_Tensor* pt2 = model->ynum_layer->tensor;
	for (_ULL i = 0; i < model->output_layer->sumtensor; i++, pt++, pt2++)//计算pd
		model->pd[i] = pt->num - pt2->num;

	LOFE_Train_ApllyG(model);//计算新的权值和偏置并应用

	LOFE_Train_UpdateDropout(model);

	return model->output_layer;
}

LOFE_Model* LOFE_TrainDS(LOFE_Model* model)
{
	float* pw = NULL;
	float* pb = NULL;
	LOFE_Tensor* pt = NULL;
	LOFE_Tensor* pt2 = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_Copy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_Copy(model->layers)))
	{
		if (pl->bindlayerid != 0)
		{
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
			{
				if (pt->isdropout == 0)
				{
					pt->num = 0.;
					pw = pt->w;
					pb = pt->b;
					pt2 = pt->bind_tensor;
					for (_ULL j = 0; j < pt->bind_num; pt2++, pw++, pb++, j++)
						pt->num += (pt2->num) * (*pw) + (*pb);
				}
			}
		}
	}//正向传播

	LOFE_Loss(model);//计算loss
	LOFE_Train_CacuChain(model);//计算所有反向传播关系式的值

	pt = model->output_layer->tensor;
	pt2 = model->ynum_layer->tensor;
	for (_ULL i = 0; i < model->output_layer->sumtensor; i++, pt++, pt2++)//计算pd
		model->pd[i] = pt->num - pt2->num;

	LOFE_Train_ApllyG(model);//计算新的权值和偏置并应用

	LOFE_Train_UpdateDropout(model);

	return model;
}

_LL LOFE_CompileModel_QueryAdr(List* chain, void* adr)
{
	_LL js = 0;
	for (void* pte = List_Copy(chain); pte != NULL; pte = List_Copy(chain), js++)
	{
		if (pte == adr)
		{
			chain->now = chain->pfirst;
			return js;
		}
	}
	return -1;
}

LOFE_Model* LOFE_CompileModel_PutChain(LOFE_Model* model, LOFE_Chain* chain, _ULL t)
{
	bool wuse = 0, buse = 0;
	LOFE_Tensor* pt = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)))
	{
		if (pl->bindlayerid != 0)
		{
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; pt++, i++)
			{
				for (_ULL j = 0; j < pt->bind_num; j++)
				{
					pt->subchain[t]->wchain[j] = List_Init();
					pt->subchain[t]->bchain[j] = List_Init();
					for (_ULL k = 0; k < chain->sumchaindot; k++)
					{
						if (LOFE_CompileModel_QueryAdr(chain->pchain[k], (void*)(pt->w + j)) != -1)
							List_Put(pt->subchain[t]->wchain[j], (void*)(ULLdup(1, k)));
						if (LOFE_CompileModel_QueryAdr(chain->pchain[k], (void*)(pt->b + j)) != -1)
							List_Put(pt->subchain[t]->bchain[j], (void*)(ULLdup(1, k)));
					}
				}
			}
		}
	}
	return model;
}

LOFE_Chain* LOFE_CompileModel_CacuSum(LOFE_Model* model, LOFE_Chain* chain)
{
	_ULL sumw = 1;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)))
		sumw *= (pl->sumtensor);

	_ULL sumb = 0;
	_ULL a = 1;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)))
	{
		if (pl != model->input_layer)
		{
			a *= (pl->tensor->bind_num);
			sumb += a;
		}
	}
	sumb *= (model->output_layer->sumtensor);

	_ULL sum = sumw + sumb;

	chain->pdif = BALLOC_R(sum, float, model->mem);

	chain->pchain = BALLOC_R(sum, List*, model->mem);
	for (_ULL i = 0; i < sum; i++)
		chain->pchain[i] = List_Init();
	chain->sumbchaindot = sumb;
	chain->sumwchaindot = sumw;
	chain->sumchaindot = sum;
	return chain;
}

LOFE_Model* LOFE_CompileModel_W(LOFE_Model* model, LOFE_Chain* chain)
{
	LOFE_Tensor* pt = NULL;
	BALLOCS_S(_ULL, psum, 1,  NULL,);
	bool reseti = 0;
	_ULL times = 0;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)))
	{//写入w
		if (pl != model->input_layer)
		{
			for (_ULL i = 0, js = 0; js < chain->sumwchaindot; )//i 所有tensor
			{
				pt = pl->tensor + i;
				for (_ULL j = 0; j < pt->bind_num; j++)//一个tensor的w
				{
					*psum = 0;
					times = LOFE_CompileModel_W_CacuTimes(pt->bind_tensor + j, psum);
					//if (times == 1)
					//	reseti = 1;
					//else
					//	reseti = 0;
					for (_ULL k = 0; k < times; k++, *psum = 0/*, printf("%3f ", *(pt->w + j))*/, js++)//w重复
						List_Put(chain->pchain[js], (void*)(pt->w + j));
				}
				if (i == pl->sumtensor - 1/* && reseti*/)
					i = 0;
				else
					i++;
			}
		}
		else
		{
			for (_ULL i = 0, b = 0; i < chain->sumwchaindot; i++)
			{
				List_Put(chain->pchain[i], (void*)(&((pl->tensor + b)->num)));/*printf("%3f ", (pl->tensor + b)->num);*/
				if (b >= pl->sumtensor)
					b = 0;
			}
		}
	}
	return model;
}

LOFE_Model* LOFE_CompileModel_B(LOFE_Model* model, LOFE_Chain* chain)
{
	_ULL* bias = ULLdup(1, chain->sumwchaindot);
	LOFE_Tensor* pt = NULL;
	float* pb = NULL;
	float* pw = NULL;

	for (_ULL i = 0; i < model->output_layer->sumtensor; i++)//先写入output层的b
	{
		pt = model->output_layer->tensor + i;
		for (_ULL j = 0; j < pt->bind_num; j++, (*bias)++)
			List_Put(chain->pchain[*bias], (void*)(pt->b + j));
	}

	for (LOFE_Layer* pl = (LOFE_Layer*)(List_Copy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_Copy(model->layers)))
	{//写入b
		if (pl != model->output_layer)
		{
			pt = pl->tensor;
			for (_ULL i = 0; i < pl->sumtensor; i++, pt++)//所有tensor
			{
				pb = pt->b;
				pw = pt->w;
				for (_ULL j = 0; j < pt->bind_num; j++, pb++, pw++)//所有b
				{
					LOFE_CompileModel_B_PutB(chain, pw, pb, model->sumlayer - pl->layerid, bias);
				}
			}
		}
	}
	return model;
}

LOFE_Chain* LOFE_CompileModel_B_PutB(LOFE_Chain* chain, float* pw, float* pb, _ULL k, _ULL* bias)
{
	_LL adr = 0;
	for (_ULL j = 0, his = *bias; j < chain->sumwchaindot; j++)
	{
		adr = LOFE_CompileModel_QueryAdr(chain->pchain[j], pw);
		if ((adr != -1) && (LOFE_CompileModel_B_PutB_IsEd(chain->pchain, his, *bias, adr, j) == 0))
		{
			chain->pchain[*bias] = List_Init();
			for (_ULL a = 0; a < k; a++)//其实k也就是adr
				List_Put(chain->pchain[*bias], List_Copy(chain->pchain[j]));
			chain->pchain[j]->now = chain->pchain[j]->pfirst;//reset now pointer manually
			List_Put(chain->pchain[*bias], (void*)pb);
			(*bias)++;
		}
	}
	return chain;
}

bool LOFE_CompileModel_B_PutB_IsEd(List** chain, _ULL his, _ULL bias, _LL adr, _ULL j)
//his是当前pb最开始的链索引
//bias是pb当前的链索引
//j是当前怀疑链的索引
//adr是当前怀疑链的pw节点索引
{
	void* p1 = NULL, * p2 = NULL;
	for (_ULL i = his, js = 0; i < bias; i++, js = 0)//从历史历遍到现在
	{
		for (_ULL k = 0; k < adr; k++)//K是当前检查链的节点索引
		{
			p1 = List_Copy(chain[i]);
			p2 = List_Copy(chain[j]);
			if (p1 != p2)
			{
				chain[i]->now = chain[i]->pfirst;
				chain[j]->now = chain[j]->pfirst;
				k = adr;//直接去继续比对下一个
			}
			else
			{
				++js;
			}
		}
		if (js == adr)
		{
			//printf("\n%llu  %llu   %lld   %llu\n", his, bias, adr, j);
			chain[i]->now = chain[i]->pfirst;
			chain[j]->now = chain[j]->pfirst;
			return 1;
		}
	}
	return 0;
}

LOFE_Model* LOFE_CompileModel(LOFE_Model* model)
{
	model->pd = BALLOC_R(model->output_layer->sumtensor, float, model->mem);//为train做一些变量申请准备
	model->chain = BALLOC_R(model->output_layer->sumtensor, LOFE_Chain, model->mem);

	LOFE_Tensor* pt = NULL;
	LOFE_SubChain* psub = NULL;
	for (LOFE_Layer* pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)); pl != NULL; pl = (LOFE_Layer*)(List_ReverseCopy(model->layers)))
	{//
		if (pl != model->input_layer)
		{
			for (_ULL i = 0; i < pl->sumtensor; i++)//i 所有tensor
			{
				pt = pl->tensor + i;
				pt->subchain = BALLOC_R(model->output_layer->sumtensor, LOFE_SubChain*, model->mem);
				for (_ULL j = 0; j < model->output_layer->sumtensor; j++)//一个tensor的w
				{
					pt->subchain[j] = BALLOC_R(1, LOFE_SubChain, model->mem);
					pt->subchain[j]->wchain = BALLOC_R(pt->bind_num, List*, model->mem);
					pt->subchain[j]->bchain = BALLOC_R(pt->bind_num, List*, model->mem);
				}
			}
		}
	}
	for (_ULL i = 0; i < model->output_layer->sumtensor; i++)
	{
		LOFE_CompileModel_CacuSum(model, model->chain + i);
		LOFE_CompileModel_W(model, model->chain + i);
		LOFE_CompileModel_B(model, model->chain + i);
		LOFE_CompileModel_PutChain(model, model->chain + i, i);
	}
	return model;
}

_ULL LOFE_CompileModel_W_CacuTimes(LOFE_Tensor* pt, _ULL* psum)
{
	if (pt->bind_num == 0)
	{
		(*psum)++;
		return *psum;
	}
	for (_ULL i = 0; i < pt->bind_num; i++)//inputlayer-pt->bind_num == 0
	{
		LOFE_CompileModel_W_CacuTimes(pt->bind_tensor + i, psum);
	}
	return *psum;
}

bool LOFE_FreeModel(LOFE_Model* model)
{
	MyBA_Free_R(model->mem);
	free(model);
	return 1;
}
//***********************************************************************************************************************

//***********************************************************************************************************************

//***********************************************************************************************************************

//***********************************************************************************************************************

//***********************************************************************************************************************

//***********************************************************************************************************************

BA_Shape::BA_Shape(_ULL len, ...)
{
	shapeLen = len;
	shape = BALLOC_L(len,int);
	if (shape == NULL)
	{
		MyBA_Err("BA_Shape::BA_Shape(_ULL len, ...): BALLOC_L(len,int)==NULL, make shape = NULL", 1);
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

BA_Shape::BA_Shape(_ULL len, int* _shape)
{
	shapeLen = len;
	shape = BALLOC_L(len, int);
	if (shape == NULL)
	{
		MyBA_Err("BA_Shape::BA_Shape(_ULL len, ...): BALLOC_L(len,int)==NULL, make shape = NULL", 1);
	}
	else
	{
		for (int a = 0; a < len; a++)
			shape[a] = _shape[a];
	}
}

BA_Shape::BA_Shape(int* _shape, _ULL len)
{
shapeLen = len;
	shape = BALLOC_L(len, int);
	if (shape == NULL)
	{
		MyBA_Err("BA_Shape::BA_Shape(_ULL len, ...): BALLOC_L(len,int)==NULL, make shape = NULL", 1);
	}
	else
	{
		for (int a = 0; a < len; a++)
			shape[a] = _shape[a];
	}
}

BA_Array::BA_Array(BA_Shape _shape, _ULL content)
{
	mem = List_Init();
	int* pt = _shape.shape;
	dataShape = BALLOC_R(_shape.shapeLen, int, mem);
	shapeLen = _shape.shapeLen;
	dataF = NULL;
	type = 'l';
	dataLen = 1;
	dataSumF = dataSumL = 0;

	for (_ULL i = 0; i < _shape.shapeLen; i++, pt++)
	{
		dataLen *= (*pt);
		dataShape[i] = *pt;
	}

	dataL = BALLOC_R(dataLen, _ULL, mem);
	if (content != 0)
	{
		_ULL* pt2 = dataL;
		for (_ULL i = 0; i < dataLen; i++, pt2++)
			*pt2 = content;
	}
}

BA_Array::BA_Array(BA_Shape _shape, float content)
{
	mem = List_Init();
	int* pt = _shape.shape;
	dataShape = BALLOC_R(_shape.shapeLen, int, mem);
	shapeLen = _shape.shapeLen;
	dataL = NULL;
	type = 'f';
	dataLen = 1;
	dataSumF = dataSumL = 0;

	for (_ULL i = 0; i < _shape.shapeLen; i++, pt++)
	{
		dataLen *= (*pt);
		dataShape[i] = *pt;
	}

	dataF = BALLOC_R(dataLen, float, mem);
	if (content != 0)
	{
		float* pt2 = dataF;
		for (_ULL i = 0; i < dataLen; i++, pt2++)
			*pt2 = content;
	}
}

BA_Array::BA_Array(BA_Shape _shape, const char* way)
{
	mem = List_Init();
	dataShape = BALLOC_R(_shape.shapeLen, int, mem);
	shapeLen = _shape.shapeLen;
	dataL = NULL;
	type = 'f';
	dataLen = 1;
	dataSumF = dataSumL = 0;

	for (_ULL i = 0; i < _shape.shapeLen; i++)
	{
		dataLen = dataLen * _shape.shape[i];
		dataShape[i] = _shape.shape[i];
	}

	dataF = BALLOC_R(dataLen, float, mem);
	if (strcmp(way, "rand") == 0)
	{
		float* te = dataF;
		for (_ULL i = 0; i < dataLen; i++, te++)
			*te = (float)(rand()%10000) / (float)10000.;
	}
	//else//strcmp(way, "xy") == 0
	//{
	//	if (_shapeLen < 2)
	//	{
	//		MyBA_Err("BA_Array::BA_Array(_ULL* _dataShape, _ULL _shapeLen, char* way): _shapeLen < 2 but way==\"xy\" which need least 2 dims,return 0s", 1);
	//	}
	//	else
	//	{

	//	}
	//}
}

BA_Array BA_Array::To(char toType, bool aNew)
{
	if (aNew)
	{
		if (toType == 'l')
		{
			BA_Array te(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* te1 = te.dataL;
			if (type == 'l')
			{
				_ULL* te2 = dataL;
				for (_ULL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = *te2;
			}
			else
			{
				float* te2 = dataF;
				for (_ULL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = (_ULL)(*te2);
			}
			return te;
		}
		else//toType=='f'
		{
			BA_Array te(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* te1 = te.dataF;
			if (type == 'l')
			{
				_ULL* te2 = dataL;
				for (_ULL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = (float)(*te2);
			}
			else//selfType=='f'
			{
				float* te2 = dataF;
				for (_ULL i = 0; i < dataLen; i++, te1++, te2++)
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
				dataL = BALLOC_R(dataLen, _ULL, mem);
				float* te1 = dataF;
				_ULL* te2 = dataL;
				for (_ULL i = 0; i < dataLen; i++, te1++, te2++)
					*te2 = (_ULL)(*te1);
			}
		}
		else//toType=='f'
		{
			if (type == 'l')//l->f
			{
				type = 'f';
				dataF = BALLOC_R(dataLen, float, mem);
				float* te1 = dataF;
				_ULL* te2 = dataL;
				for (_ULL i = 0; i < dataLen; i++, te1++, te2++)
					*te1 = (float)(*te2);
			}
		}
	}
	return *this;
}

BA_Array BA_Array::Add(BA_Array other, bool aNew)
{
	if (shapeLen != other.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Add(BA_Array other, bool aNew): shapeLen != other.GetShapeLen(),return", 1);
		return *this;
	}
	for (_ULL i = 0; i < other.shapeLen; i++)
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
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL, * pt2 = other.dataL, *pt3 = ret.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++,pt3++)
					*pt3 = *pt1 + *pt2;
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 + *pt2;
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL,  * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 + (_ULL) (*pt2);
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 + (_ULL)(*pt2);
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
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 + (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 + (float)(*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt2 = other.dataF, * pt3 = ret.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 + (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
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
	for (_ULL i = 0; i < other.shapeLen; i++)
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
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL, * pt2 = other.dataL, * pt3 = ret.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - *pt2;
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - *pt2;
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL, * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - (_ULL)(*pt2);
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - (_ULL)(*pt2);
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
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - (float)(*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt2 = other.dataF, * pt3 = ret.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = *pt1 - (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = *pt1 - (*pt2);
			}
		}
	}
}

BA_Array BA_Array::Mul(BA_Array other, bool aNew)
{
	if (shapeLen != other.shapeLen)
	{
		MyBA_Err("BA_Array BA_Array::Mul(BA_Array other, bool aNew): shapeLen != other.GetShapeLen(),return", 1);
		return *this;
	}
	for (_ULL i = 0; i < other.shapeLen; i++)
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
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL, * pt2 = other.dataL, * pt3 = ret.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (*pt2);
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = (*pt1) * (*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL, * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (_ULL)(*pt2);
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = (*pt1) * (_ULL)(*pt2);
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
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					*pt1 = (*pt1) * (float)(*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
				float* pt1 = dataF, * pt2 = other.dataF, * pt3 = ret.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					*pt3 = (*pt1) * (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
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
	for (_ULL i = 0; i < other.shapeLen; i++)
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
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL, * pt2 = other.dataL, * pt3 = ret.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if(*pt2 != 0)
						*pt3 = (*pt1) / (*pt2);
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					if (*pt2 != 0)
						*pt1 = (*pt1) / (*pt2);
			}
		}
		else//other.type == 'f'
		{
			if (aNew)
			{
				BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
				_ULL* pt1 = dataL, * pt3 = ret.dataL;
				float* pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if (*pt2 != 0)
						*pt3 = (*pt1) / (_ULL)(*pt2);
				return ret;
			}
			else
			{
				_ULL* pt1 = dataL, * pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					if (*pt2 != 0)
						*pt1 = (*pt1) / (_ULL)(*pt2);
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
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if (*pt2 != 0)
						*pt3 = (*pt1) / (float)(*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF;
				_ULL* pt2 = other.dataL;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
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
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++, pt3++)
					if (*pt2 != 0)
						*pt3 = (*pt1) / (*pt2);
				return ret;
			}
			else
			{
				float* pt1 = dataF, * pt2 = other.dataF;
				for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
					if (*pt2 != 0)
						*pt1 = (*pt1) / (*pt2);
			}
		}
	}
	return *this;
}

BA_Array BA_Array::Add(_ULL other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) + (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) + (float)(other);
		}
	}
	return *this;
}

BA_Array BA_Array::Reduc(_ULL other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) - (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) - (float)(other);
		}
	}
	return *this;
}

BA_Array BA_Array::Mul(_ULL other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) * (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) * (float)(other);
		}
	}
	return *this;
}

BA_Array BA_Array::Devide(_ULL other, bool aNew)
{
	if (other == 0)
	{
		MyBA_Err("BA_Array BA_Array::Devide(_ULL other, bool aNew): other == 0,return *this", 1);
		return *this;
	}
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) / (other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (float)(other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
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
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (_ULL)(other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) + (_ULL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) + (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
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
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (_ULL)(other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) - (_ULL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) - (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
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
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (_ULL)(other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) * (_ULL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) * (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
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
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (_ULL)(other);
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) / (_ULL)(other);
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) / (other);
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
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
	if (dataShape[shapeLen-1] != other.dataShape[other.dataLen-2])
	{
		MyBA_Err("BA_Array BA_Array::Add(BA_Array other, bool aNew): dataShape[shapeLen-1] != other.dataShape[other.dataLen-2],return *this", 1);
		return *this;
	}
	if (type == 'l' || other.type == 'l')
	{
		MyBA_Err("BA_Array BA_Array::Add(BA_Array other, bool aNew): type == 'l' || other.type == 'l' and it is not support now,return *this", 1);
		return *this;
	}
	else//selfType == 'f' && other.type == 'f'
	{
		float* pt1 = dataF, * pt2 = other.dataF;
		for (_ULL i = 0; i < dataLen; i++, pt1++, pt2++)
			*pt1 = (*pt1) * (*pt2);
	}
	return *this;
}

BA_Array BA_Array::Eq(float other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) == (_ULL)(other) ? 1:0;
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) == (_ULL)(other) ? 1 : 0;
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) == (other) ? 1 : 0;
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) == (other) ? 1 : 0;
		}
	}
	return *this;
}

BA_Array BA_Array::Sum(void)
{
	if (type == 'l')
	{
		_ULL* pt = dataL;
		dataSumL = 0;
		for (_ULL i = 0; i < dataLen; i++, pt++)
			dataSumL += *pt;
	}
	else//selfType == 'f'
	{
		float* pt = dataF;
		dataSumF = 0.;
		for (_ULL i = 0; i < dataLen; i++, pt++)
			dataSumF += *pt;
	}
	return *this;
}

BA_Array BA_Array::Ge(float other, bool aNew)
{
	if (type == 'l')
	{

		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (_ULL)0);
			_ULL* pt1 = dataL, * pt3 = ret.dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) > (_ULL)(other) ? 1 : 0;
			return ret;
		}
		else
		{
			_ULL* pt1 = dataL;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) > (_ULL)(other) ? 1 : 0;
		}
	}
	else//selfType == 'f'
	{
		if (aNew)
		{
			BA_Array ret = BA_Array(BA_Shape(dataShape, shapeLen), (float)0.f);
			float* pt1 = dataF, * pt3 = ret.dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++, pt3++)
				*pt3 = (*pt1) > (other) ? 1 : 0;
			return ret;
		}
		else
		{
			float* pt1 = dataF;
			for (_ULL i = 0; i < dataLen; i++, pt1++)
				*pt1 = (*pt1) > (other) ? 1 : 0;
		}
	}
	return *this;
}

