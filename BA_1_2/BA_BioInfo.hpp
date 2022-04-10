//BASIC_ALL_1_2 BA_BIOINFO_H
//H
//Writen By BHM
//2022年4月9日 23点00分


#ifndef BA_BIOINFO_H
#define BA_BIOINFO_H


#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_String.hpp"

void CacuSimiMatFromFile(const char* seqPath, _ULL loadSize, const char* resultPath);

class SequencesSimiCacuer
{
public:
	int seqALen = 0;
	int seqBLen = 0;
	int rows = seqALen + 1;
	int cols = seqALen + 1;
	int var1 = 0;
	int var2 = 0;
	int var3 = 0;
	int bestAction = 0;
	char tracebackSymbol[4] = {"QA<"};
	BA_Array H = BA_Array(BA_Shape(2, rows, cols), "l");
	BA_Array gapH = BA_Array(BA_Shape(2, rows, cols), "l");
	BA_Array traceback = BA_Array(BA_Shape(2, rows, cols), "l");


	_LL CacuSequencesSimilarity(BA_String seqA, BA_String seqB);
	_LL CacuSequencesSimilarity2(BA_String seqA, BA_String seqB);
	_LL CacuSequencesSimilarityWithTrace(BA_String seqA, BA_String seqB);
	_LL CacuSequencesSimilarityWithTrace2(BA_String seqA, BA_String seqB);
	void VizTraceback(BA_String seqA, BA_String seqB);
	void VizTraceback2(BA_String seqA, BA_String seqB);
};





















#endif