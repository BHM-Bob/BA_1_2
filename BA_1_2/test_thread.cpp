
#include"BA_Base.hpp"
#include"BA_CMD.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Test.hpp"
#include"BA_BioInfo.hpp"
#include"BA_JSON.hpp"

std::mutex m;
void ba::test::_thread::WordsCount_SubThr(_LL id, balist<ba::str>& getQ,
    balist<char>& putQ, balist<float>& proc, balist<bool>& sig, void* data)
{
    List* mem = List_Init();
    ba::str* text = (ba::str*)getQ.ThrGet(&m);
    balist<char>* splitResult = text->Splitx(" ,.\n\"'?!;:@`#$%^&*()+=/\r\t()[]{}<>");
    std::map<std::string, _ULL>* tree = (std::map<std::string, _ULL>*)data;
    std::string str;
    _ULL nowIdx = 0;
    for (char* p = splitResult->Copy(); p; p = splitResult->Copy())
    {
        str.clear();
        str.assign(p);
        m.lock();
        (*tree)[str] = (*tree)[str] + 1;
        m.unlock();

        nowIdx = splitResult->GetNowIndex();
        if (nowIdx % 10000 == 0)
            proc.ThrPut(TypeDupR(NULL, 1, 100.f * nowIdx / splitResult->sumque), &m);
    }
    sig.ThrPut(0, &m);
    MyBA_Free_R(mem, true);
}

void ba::test::_thread::WordsCount(void)
{
    _LL sumThreads = 8;
    std::map<std::string, _ULL>* tree = new std::map<std::string, _ULL>();
    MyThreadsPool tp = MyThreadsPool(sumThreads, WordsCount_SubThr, (void*)tree);
    ba::str text = ba::str(
        ReadTXT("E:\\My_Progs\\z_Progs_Data_HC\\text\\Harry Potter (complete works).txt"));
    ba::str* subStr = NULL;
    for (_LL i = 0, stepLen = text.len / sumThreads; i < sumThreads; i++)
    {
        subStr = text(i * stepLen, (i == sumThreads - 1) ? text.len : (i + 1) * stepLen);
        tp.PutTask(subStr, &m);
    }
    auto result = tp.LoopToQuit(&m);
    //FILE* pf = NULL;
    //fopen_s(&pf, "E:\\My_Progs\\z_Progs_Data_HC\\text\\Harry Potter R.csv", "w");
    //if (pf)
    //{
    //    for (auto pd = tree->begin(); pd != tree->end(); pd++)
    //        fprintf(pf, "%s,%llu\n", pd->first.c_str(), pd->second);
    //    fclose(pf);
    //}
    tp.Destroy(&m);
}


//void BA_ThreadPool_Test_workFunc(_ULL id, MyThreadQueue& getQ, MyThreadQueue& putQ,
//    MyThreadQueue& sig, void* data)
//{
//    List* pData = NULL;
//    SequencesSimiCacuer seqCacuer = SequencesSimiCacuer();
//    _ULL* seqAIdx = NULL, * seqBIdx = NULL;
//    BA_String* seqA = NULL, * seqB = NULL;
//    while (true)
//    {
//        //pData = (List*)getQ.Get(&m);
//        if (pData == (List*)0x1)
//            break;
//        seqAIdx = (_ULL*)List_Index(pData, 0);
//        seqA = (BA_String*)List_Index(pData, 1);
//        seqBIdx = (_ULL*)List_Index(pData, 2);
//        seqB = (BA_String*)List_Index(pData, 3);
//        //putQ.Put((void*)lldup(5, *seqAIdx, seqA->len, *seqBIdx, seqB->len,
//        //    seqCacuer.CacuSequencesSimilarity2(seqA, seqB)), &m);
//        free(seqA->pc);
//        free(seqB->pc);
//    }
//    //sig.Put(NULL, &m);
//}
//
//void BA_ThreadPool_Test(void)
//{
//    char* seqPath = mstrdup("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\RNASeqs.txt");
//    char* resultPath = mstrdup("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\result2.txt");
//    List* rnaSeqs = BA_String(ReadTXT(seqPath, 8592246)).Split("\n");
//    BA_String* seqA = NULL, * seqB = NULL;
//    MyThreadsPool tp = MyThreadsPool(8, BA_ThreadPool_Test_workFunc, NULL);
//    _ULL seqAIdx = 0, seqBIdx = 1, mainIndent = 50;
//    _ULL sumLoaded = 0, sumMainSeq = rnaSeqs->sumque / mainIndent + 1,
//        sumWaitseq = rnaSeqs->sumque - sumMainSeq, sumLoad = sumMainSeq * sumWaitseq;
//    while (seqAIdx < rnaSeqs->sumque)
//    {
//        seqA = new BA_String((char*)rnaSeqs->Index(rnaSeqs, seqAIdx));
//        seqB = new BA_String((char*)rnaSeqs->Index(rnaSeqs, seqBIdx));
//        //tp.PutTask((void*)List_Gather((void*)ULLdup(1, seqAIdx), (void*)seqA,
//        //    (void*)ULLdup(1, seqBIdx), (void*)seqB), &m);
//        sumLoaded++;
//        seqBIdx = ((seqBIdx + 1) % mainIndent == 0) ? (seqBIdx + 2) : (seqBIdx + 1);
//        if (seqBIdx >= rnaSeqs->sumque)
//        {
//            seqAIdx += mainIndent;
//            seqBIdx = 1;
//        }
//        if (sumLoaded % 100 == 0)
//            printf("\r%llu / %llu", sumLoaded, sumLoad);
//    }
//    //List* result = tp.LoopToQuit(&m, (void*)0x1);
//    //tp.Destroy(&m);
//    //PPU(result->sumque);
//    FILE* pf = NULL;
//    if (fopen_s(&pf, resultPath, "w") == 0)
//    {
//        //LIST_FORS(_LL, p, result)
//        //{// seqAIdx | seqALen | seqBIdx | seqBLen | Simi
//        //    fprintf(pf, "%4lld | %4lld | %4lld | %4lld | %4lld\n",
//        //        p[0], p[1], p[2], p[3], p[4]);
//        //}
//    }
//    else if (pf)
//    {
//        fclose(pf);
//    }
//}