#include "BA_1_2.hpp"
mutex m;

void workFunc(_ULL id, MyThreadQueue& getQ, MyThreadQueue& putQ,
    MyThreadQueue& sig, void* data)
{
    List* pData = NULL;
    SequencesSimiCacuer seqCacuer = SequencesSimiCacuer();
    _ULL* seqAIdx = NULL, *seqBIdx = NULL;
    BA_String* seqA = NULL, * seqB = NULL;
    while (true)
    {
        pData = (List*)getQ.Get(&m);
        if (pData == (List*)0x1)
            break;
        seqAIdx = (_ULL*)List_Index(pData, 0);
        seqA = (BA_String*)List_Index(pData, 1);
        seqBIdx = (_ULL*)List_Index(pData, 2);
        seqB = (BA_String*)List_Index(pData, 3);
        putQ.Put((void*)lldup(5, *seqAIdx, seqA->len, *seqBIdx, seqB->len,
            seqCacuer.CacuSequencesSimilarity2(seqA, seqB)), &m);
        free(seqA->pc);
        free(seqB->pc);
    }
    sig.Put(NULL, &m);
}

int main(int argc, char** argvs)
{
	MyBA_Init();

    char* seqPath = mstrdup("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\RNASeqs.txt");
    char* resultPath = mstrdup("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\result2.txt");
    List* rnaSeqs = BA_String(ReadTXT(seqPath,8592246)).Split("\n");
    BA_String* seqA = NULL, * seqB = NULL;
    MyThreadsPool tp = MyThreadsPool(8, workFunc, NULL);
    _ULL seqAIdx = 0, seqBIdx = 1, mainIndent = 50;
    _ULL sumLoaded = 0,sumMainSeq = rnaSeqs->sumque / mainIndent + 1,
        sumWaitseq = rnaSeqs->sumque - sumMainSeq, sumLoad = sumMainSeq * sumWaitseq;
    while (seqAIdx < rnaSeqs->sumque)
    {
        seqA = new BA_String((char*)rnaSeqs->Index(rnaSeqs, seqAIdx));
        seqB = new BA_String((char*)rnaSeqs->Index(rnaSeqs, seqBIdx));
        tp.PutTask((void*)List_Gather( (void*)ULLdup(1, seqAIdx), (void*)seqA,
            (void*)ULLdup(1, seqBIdx), (void*)seqB), &m);
        sumLoaded++;
        seqBIdx = ((seqBIdx + 1) % mainIndent == 0) ? (seqBIdx + 2) : (seqBIdx + 1);
        if (seqBIdx >= rnaSeqs->sumque)
        {
            seqAIdx += mainIndent;
            seqBIdx = 1;
        }
        if(sumLoaded % 100 == 0)
            printf("\r%llu / %llu", sumLoaded, sumLoad);
    }
    List* result = tp.LoopToQuit(&m, (void*)0x1);
    tp.Destroy(&m);
    PPU(result->sumque);
    FILE* pf = NULL;
    if (fopen_s(&pf, resultPath, "w") == 0)
    {
        LIST_FORS(_LL, p, result)
        {// seqAIdx | seqALen | seqBIdx | seqBLen | Simi
            fprintf(pf, "%4lld | %4lld | %4lld | %4lld | %4lld\n",
                p[0], p[1], p[2], p[3], p[4]);
        }
    }
    else if (pf)
    {
        fclose(pf);
    }

	return MyBA_Quit();
}