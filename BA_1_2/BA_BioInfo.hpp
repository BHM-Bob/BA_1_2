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
	_LL seqALen = 0;
	_LL seqBLen = 0;
	_LL rows = seqALen + 1;
	_LL cols = seqALen + 1;
	int var1 = 0;
	int var2 = 0;
	int var3 = 0;
	int bestAction = 0;
	char tracebackSymbol[4] = {"QA<"};
	ba::tensor<int> H = ba::tensor({ rows, cols }, 0);
	ba::tensor<int> gapH = ba::tensor({ rows, cols }, 0);
	ba::tensor<int> traceback = ba::tensor({ rows, cols }, 0);


	_LL CacuSequencesSimilarity(ba::str seqA, ba::str seqB);
	_LL CacuSequencesSimilarity2(ba::str seqA, ba::str seqB);
	_LL CacuSequencesSimilarity2(ba::str* seqA, ba::str* seqB);
	_LL CacuSequencesSimilarityWithTrace(ba::str seqA, ba::str seqB);
	_LL CacuSequencesSimilarityWithTrace2(ba::str seqA, ba::str seqB);
	void VizTraceback(ba::str seqA, ba::str seqB);
	void VizTraceback2(ba::str seqA, ba::str seqB);
};





















#endif