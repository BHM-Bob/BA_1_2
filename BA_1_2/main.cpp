#include "BA_1_2.hpp"

int main(int argc, char** argvs)
{
	MyBA_Init();

	BA_String seqA = BA_String("GCG");
	BA_String seqB = BA_String("UGC");
	List* rnaSeqs = BA_String(ReadTXT("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\RNASeqs.txt",
		8592246)).Split("\n");
	List* result = List_Init();
	SequencesSimiCacuer seqCacuer = SequencesSimiCacuer();

	for (_ULL seqAIdx = 0; seqAIdx < rnaSeqs->sumque; seqAIdx += 100)
	{
		seqA.ReLoad((char*)rnaSeqs->Index(rnaSeqs, seqAIdx));		
		for(ListDot* seqBDot = rnaSeqs->IndexDot(rnaSeqs, seqAIdx + 1); seqBDot; seqBDot = seqBDot->pnext)
		{
			if (seqBDot->idx % 50 == 0)
				printf("\rseqAIdx:%4llu seqBIdx:%4llu sumQue:%4llu  %5.4f s / seq-seq",
					seqAIdx, seqBDot->idx, rnaSeqs->sumque, pba->GUT() / 50.0);
			seqB.ReLoad((char*)seqBDot->pdata);
			result->Put(result, (void*)lldup(5, seqAIdx, seqA.len, seqBDot->idx, seqB.len,
				seqCacuer.CacuSequencesSimilarity(seqA, seqB)));
		}
	}
	FILE* pf = NULL;
	if (fopen_s(&pf, "D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\result.txt", "w") == 0)
	{
		LIST_FORS(_LL, p, result)
		{// seqAIdx | seqALen | seqBIdx | seqBLen | Simi
			fprintf(pf, "%4lld | %4lld | %4lld | %4lld | %4lld\n",
				p[0], p[1], p[2], p[3], p[4]);
		}
	}
	else
	{
		if (!pf)
			fclose(pf);
	}
	return MyBA_Quit();
}