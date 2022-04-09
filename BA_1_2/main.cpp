#include "BA_1_2.hpp"

int main(int argc, char** argvs)
{
	MyBA_Init();
	BA_String seqA = BA_String("GCG");
	BA_String seqB = BA_String("UGC");
	List* rnaSeqs = BA_String(ReadTXT("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\RNASeqs.txt")).Split("\n");
	List* result = List_Init();
	SequencesSimiCacuer seqCacuer = SequencesSimiCacuer();

	seqA.ReLoad((char*)rnaSeqs->pfirst->pdata);
	LIST_FORS(char, pStrRNA, rnaSeqs)
	{		
		if (strlen(pStrRNA) < 100)
			continue;
		if (rnaSeqs->now && rnaSeqs->now->idx % 50 == 0)
			printf("\r%4llu / %4llu   %5.4f s / seq-seq", rnaSeqs->now->idx, rnaSeqs->sumque, pba->GUT() / 50.0);
		seqB.ReLoad(pStrRNA);
		result->Put(result, (void*)lldup(1, seqCacuer.CacuSequencesSimilarity(seqA, seqB)));
	}
	LIST_FORS(_LL, pSimi, result)
	{
		printf("\n%4lld", *pSimi);
	}
	return MyBA_Quit();
}