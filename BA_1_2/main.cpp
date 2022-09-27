/*
 * @Author: BHM-Bob G 2262029386@qq.com
 * @Date: 2021-11-18 22:51:44
 * @LastEditors: BHM-Bob G
 * @LastEditTime: 2022-05-31 00:20:10
 * @Description:
 */
#include "BA_1_2.hpp"
 //mutex m;

int main(int argc, char** argvs)
{
	argc = 4;
	char** ppc = MCALLOC(argc, char*);
	ppc[0] = argvs[0];
	ppc[1] = mstrdup("=open");
	ppc[2] = mstrdup("-i");
	ppc[3] = mstrdup("D:\\Bio\\DataBase\\Silva\\SILVA_138_SSU_tax_silva.fasta");
	MyBA_Init(argc, ppc);

	ba::str s = ba::str("666");
	s.Insert(-1, "000");
	s.Insert(3, "333");
	s.Insert(8, "999");
	PPS(s.pc);

	if(argc == 1)//run test
	{
		BA_Array a = BA_Array(BA_Shape(3, 90, 90, 90), "rand");
		BA_Array b = BA_Array(BA_Shape(3, 90, 90, 90), "l");
		BA_Array c = a * (b + 1LL);

		badict d = badict();
		d["k1"] = 8.9f;
		d["k2"] = mstrdup("GGGKKK");
		d["k3"] = a;
		char* k2 = d.Copy<char*>("k2");

		BA_Test_WordsCount();
	}

	return MyBA_Quit();
}

