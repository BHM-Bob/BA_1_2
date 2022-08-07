﻿
#include"BA_Base.hpp"
#include"BA_Math.hpp"
#include"BA_UI.hpp"
#include"BA_QUI.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Test.hpp"
#include"BA_Plot.hpp"
#include"BA_BioInfo.hpp"

float BA_Plot_Test_Map(float x,float y)
{
	return 1-x*log(x-y) + y * y;
}

void BA_Plot_Test(void)
{
	PPF(pba->GUT());
	BA_Plot("Test", BA_Plot_Test_Map, 30., 30.).PaintMM().Loop(0);
}


void BA_Dir_Test(void)
{
	BA_Dir dir = BA_Dir("D:\\AI\\DataSet\\AlphaMedia\\rna2img\\culturable", "jpeg");
	LIST_FORS(char, path, dir.files)
	{
		if (remove(StringAdd_S(dir.root, "\\", path, NULL)) != 0)
			MyBA_Errs(1, "BA_Dir_Test:can not remove file:", StringAdd_S(dir.root, "\\", path), " ,do nothing", NULL);
		if (dir.files->now && dir.files->now->idx % 5 == 0)
			JDT(dir.files->now->idx, dir.files->sumque);
	}
}

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
	return pow(*p, 2);
}

float BA_ArrayTest_2(float* pt, void* p)
{
	return pow(*pt-*(float*)p, 2);
}

void BA_Array_Test(void)
{
    PPF(pba->GUT());
    BA_Array t1 = BA_Array(BA_Shape(1, 1000000), "rand");
    float x8 = t1.Sum().dataSumF / t1.dataLen;
    PPF(x8);
    float* px8 = &x8;
    float e1 = t1.Func(BA_ArrayTest_1, 1).Sum().dataSumF / t1.dataLen - x8 * x8;
    float e2 = t1.Maps(BA_ArrayTest_2, (void*)px8, 1).Sum().dataSumF / t1.dataLen;
    PPF(e1);
    PPF(e2);
    PPF(pba->GUT());
}
void BA_Array_Test2(void)
{
    PPF(pba->GUT());
    BA_Array t1 = BA_Array(BA_Shape(1, 1000000), "rand");
    BA_Array t2 = BA_Array(BA_Shape(1, 1000000), "rand");

    BA_Array t3 = t1.Concat(t2, 0);
    PPU(t3.dataLen);
    PPF(t3.dataF[t3.dataLen - 2]);
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

mutex m;

void BA_Test_WordsCount_SubThr(_ULL id, balist<BA_String>& getQ,
    balist<char>& putQ, balist<float>& proc, balist<bool>& sig, void* data)
{
    List* mem = List_Init();
    BA_String* text = (BA_String*)getQ.ThrGet(&m);
    balist<char>* splitResult = text->Splitx(" ,.\n\"'?!;:@`#$%^&*()+=/\r\t()[]{}<>");
    balist<_ULL>* tree = (balist<_ULL>*)data;
    string* str = new string();
    _ULL hashV = 0, nowIdx = 0;
    _ULL* count = NULL;
    std::hash<std::string> strHash;
    for(char* p = splitResult->Copy(); p; p = splitResult->Copy())
    {
        delete str;
        str = new string(p);
        hashV = strHash(*str);
        count = TypeDupR(NULL, 1, 1ULL);
        m.lock();
        tree->Insert(count, hashV, BA_Test_WordsCount_HashCol, p, true);
        m.unlock();

        nowIdx = splitResult->GetNowIndex();
        if (nowIdx % 10000 == 0)
            proc.ThrPut(TypeDupR(mem, 1, 100.f * nowIdx / splitResult->sumque), &m);
    }
    sig.ThrPut(NULL, &m);
    MyBA_Free_R(mem);
}

void BA_Test_WordsCount(void)
{
    _ULL sumThreads = 4;
    balist<_ULL>* tree = new balist<_ULL>();
    MyThreadsPool tp = MyThreadsPool(sumThreads,
        BA_Test_WordsCount_SubThr, (void*)tree);
    BA_String text = BA_String(
        ReadTXT("E:\\My_Progs\\z_Progs_Data_HC\\text\\Harry Potter (complete works).txt"));
    BA_String* subStr = NULL;
    for (_ULL i = 0, stepLen = text.len / sumThreads; i < sumThreads; i++)
    {
        subStr = text(i * stepLen, (i == sumThreads-1) ? text.len : (i + 1) * stepLen);
        tp.PutTask(subStr, &m);
    }
    List* result = tp.LoopToQuit(&m);
    FILE* pf = NULL;
    fopen_s(&pf, "E:\\My_Progs\\z_Progs_Data_HC\\text\\Harry Potter R.csv", "w");
    if (pf)
    {
        for (balistDot<_ULL>* pd = tree->pfirst; pd; pd = pd->pnext)
            fprintf(pf, "%s,%llu\n", pd->name, *(pd->pdata));
        fclose(pf);
    }
    tp.Destroy(&m);
}









