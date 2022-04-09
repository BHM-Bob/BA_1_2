//BASIC_ALL_1_2 BA_BIOINFO_H
//H
//Writen By BHM
//2022年4月9日 23点00分


#ifndef BA_BIOINFO_H
#define BA_BIOINFO_H


#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_String.hpp"

class SequencesSimiCacuer
{
public:
	int seqALen = 0;
	int seqBLen = 0;
	int rows = seqALen + 1;
	int cols = seqALen + 1;
	BA_Array H = BA_Array(BA_Shape(2, rows, cols), "l");
	BA_Array gapH = BA_Array(BA_Shape(2, rows, cols), "l");


	_LL CacuSequencesSimilarity(BA_String seqA, BA_String seqB);
};





















#endif