
#include"BA_Base.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Test.hpp"
#include"BA_BioInfo.hpp"

void BA_String_Test(void)
{
	PPSS(BA_String("GGGKKtKHHHtGGGKKKBtBBHHeHJJJ").Replace("HHH", NULL).pc);

    balist<char>* result = BA_String("1234...56789").Splitx("2.");
    for (char* p = result->Copy(); p; p = result->Copy())
        PPS(p);

    char nts1[] = "Test";
    char nts2[] = "Test";
    std::string str1(nts1);
    std::string str2(nts2);
}

float BA_ArrayTest_1(float* p)
{
	return (float)pow(*p, 2);
}

float BA_ArrayTest_2(float* pt, void* p)
{
	return (float)pow(*pt-*(float*)p, 2);
}

void BA_Array_Test(void)
{
    PPX(pba->GUT());
    BA_Array t1 = BA_Array(BA_Shape(1, 1000000), "rand");
    float x8 = t1.Sum().dataSumF / t1.dataLen;
    PPX(x8);
    float* px8 = &x8;
    float e1 = t1.Func(BA_ArrayTest_1, 1).Sum().dataSumF / t1.dataLen - x8 * x8;
    float e2 = t1.Maps(BA_ArrayTest_2, (void*)px8, 1).Sum().dataSumF / t1.dataLen;
    PPX(e1);
    PPX(e2);
    PPX(pba->GUT());
}
void BA_Array_Test2(void)
{
    PPX(pba->GUT());
    BA_Array t1 = BA_Array(BA_Shape(1, 1000000), "rand");
    BA_Array t2 = BA_Array(BA_Shape(1, 1000000), "rand");

    BA_Array t3 = t1.Concat(t2, 0);
    PPX(t3.dataLen);
    PPX(t3.dataF[t3.dataLen - 2]);
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

void BA_Test_WordsCount_HashCol(balistDot<_ULL>* p1, balistDot<_ULL>* p2)
{
    (*(p1->pdata))++;
    delete p2;
}

std::mutex m;

void BA_Test_WordsCount_SubThr(_LL id, balist<BA_String>& getQ,
    balist<char>& putQ, balist<float>& proc, balist<bool>& sig, void* data)
{
    List* mem = List_Init();
    BA_String* text = (BA_String*)getQ.ThrGet(&m);
    balist<char>* splitResult = text->Splitx(" ,.\n\"'?!;:@`#$%^&*()+=/\r\t()[]{}<>");
    std::map<std::string, _ULL>* tree = (std::map<std::string, _ULL>*)data;
    std::string str;
    _ULL nowIdx = 0;
    for(char* p = splitResult->Copy(); p; p = splitResult->Copy())
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
    sig.ThrPut(NULL, &m);
    MyBA_Free_R(mem, true);
}

void BA_Test_WordsCount(void)
{
    _LL sumThreads = 8;
    std::map<std::string, _ULL>* tree = new std::map<std::string, _ULL>();
    MyThreadsPool tp = MyThreadsPool(sumThreads,
        BA_Test_WordsCount_SubThr, (void*)tree);
    BA_String text = BA_String(
        ReadTXT("E:\\My_Progs\\z_Progs_Data_HC\\text\\Harry Potter (complete works).txt"));
    BA_String* subStr = NULL;
    for (_LL i = 0, stepLen = text.len / sumThreads; i < sumThreads; i++)
    {
        subStr = text(i * stepLen, (i == sumThreads-1) ? text.len : (i + 1) * stepLen);
        tp.PutTask(subStr, &m);
    }
    List* result = tp.LoopToQuit(&m);
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









