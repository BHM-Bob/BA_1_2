//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2022年4月9日 23点00分

#include "BA_Base.hpp"
#include "BA_File.hpp"
#include "BA_String.hpp"
#include "BA_BioInfo.hpp"
#include "BA_JSON.hpp"

//BA_String seqA = BA_String("GGATCGA");
//BA_String seqB = BA_String("GAATTCAGTTA");
//SequencesSimiCacuer seqCacuer = SequencesSimiCacuer();
//PPS(seqA.pc);
//PPS(seqB.pc);
//PPL(seqCacuer.CacuSequencesSimilarityWithTrace2(seqA, seqB));
//seqCacuer.VizTraceback2(seqA, seqB);
//CacuSimiMatFromFile(
//	"D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\RNASeqs.txt",
//	8592246,
//	"D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\result2.txt");

void CacuSimiMatFromFile(const char* seqPath, _ULL loadSize, const char* resultPath)
{
	List* mem = List_Init();
	ba::str seqA = ba::str("GCG");
	ba::str seqB = ba::str("UGC");
	balist<char>* rnaSeqs = NULL;
	if (loadSize == 0)
		rnaSeqs = ba::str(ReadTXT(seqPath)).Split("\n");
	else
		rnaSeqs = ba::str(ReadTXT(seqPath, loadSize)).Split("\n");
	balist<_LL>* result = new balist<_LL>();
	SequencesSimiCacuer seqCacuer = SequencesSimiCacuer();

	for (_LL seqAIdx = 0; seqAIdx < rnaSeqs->sumque; seqAIdx += 100)
	{
		seqA.ReLoad(rnaSeqs->Copy(seqAIdx));
		for (balistDot<char>* seqBDot = rnaSeqs->CopyDot(seqAIdx + 1); seqBDot; seqBDot = seqBDot->pnext)
		{
			if (seqBDot->idx % 50 == 0)
				printf("\rseqAIdx:%4llu seqBIdx:%4llu sumQue:%4llu  %5.4f s / seq-seq",
					seqAIdx, seqBDot->idx, rnaSeqs->sumque, pba->GUT(0) / 50.0);
			seqB.ReLoad(seqBDot->pdata);
			result->Put(TypeDupR(mem, 5, seqAIdx, seqA.len, seqBDot->idx, seqB.len,
				seqCacuer.CacuSequencesSimilarity2(seqA, seqB)));
		}
	}
	FILE* pf = NULL;
	if (fopen_s(&pf, resultPath, "w") == 0)
	{
		for(_LL* p = result->Copy(); p; p = result->Copy())
		{// seqAIdx | seqALen | seqBIdx | seqBLen | Simi
			fprintf(pf, "%4lld | %4lld | %4lld | %4lld | %4lld\n",
				p[0], p[1], p[2], p[3], p[4]);
		}
	}
	else
	{
		if (pf)
			fclose(pf);
	}
	MyBA_Free_R(mem);
}






_LL SequencesSimiCacuer::CacuSequencesSimilarity(ba::str seqA, ba::str seqB)
{
	seqALen = seqA.len;
	seqBLen = seqB.len;
	rows = seqALen + 1;
	cols = seqBLen + 1;
	H.ReCreate(BA_Shape(2, rows, cols), "l");
	gapH.ReCreate(BA_Shape(2, rows, cols), "l");
	for (int i = 1; i < cols; i++)
		H.dataL[i] = i;//H[0, :] = np.arange(start=0, stop=cols)
	for (int i = 1; i < rows; i++)
		H.dataL[i * cols] = i;//H[:, 0] = np.arange(start=0, stop=rows)
	var1 = var2 = var3 = bestAction = 0;
	for (int row = 1; row < rows; row++)
	{
		for (int col = 1; col < cols; col++)
		{
			// leave_or_replace_letter = H[row - 1, col - 1] + score_func(a, b)
			var1 = H.dataL[(row - 1) * cols + col - 1] +
				(seqA.pc[row - 1] == seqB.pc[col - 1] ? 2 : -1);
			// delete_indel = H[row - 1, col] + score
			var2 = H.dataL[(row - 1) * cols + col] +
				(gapH.dataL[(row - 1) * cols + col] == 0 ? -2 : 0);
			// insert_indel = H[row, col - 1] + score
			var3 = H.dataL[row * cols + (col - 1)] +
				(gapH.dataL[row * cols + (col - 1)] == 0 ? -2 : 0);
			// bestAction = argmax([var1, var2, var3])
			bestAction = (var1 >= var2 && var1 >= var3) ? 0 : (var2 >= var3 ? 1 : 2);
			// if best_action in [1, 2]: gapH[row, col] = True
			if (bestAction != 0)
				gapH.dataL[row * cols + col] = 1;
			// H[row, col] = max([var1, var2, var3])
			H.dataL[row * cols + col] = bestAction == 0 ? var1 : (bestAction == 1 ? var2 : var3);
		}
	}
	//for (int i = 0; i < H.dataShape[0]; i++)
	//{
	//	for (int j = 0; j < H.dataShape[1]; j++)
	//		printf("%3d ", H.dataL[i * H.dataShape[1] + j]);
	//	printf("\n");
	//}
	return H.dataL[H.dataLen - 1];
}

_LL SequencesSimiCacuer::CacuSequencesSimilarityWithTrace(ba::str seqA, ba::str seqB)
{
	seqALen = seqA.len;
	seqBLen = seqB.len;
	rows = seqALen + 1;
	cols = seqBLen + 1;
	H.ReCreate(BA_Shape(2, rows, cols), "l");
	gapH.ReCreate(BA_Shape(2, rows, cols), "l");
	traceback.ReCreate(BA_Shape(2, rows, cols), "l");
	for (int i = 1; i < cols; i++)
		H.dataL[i] = i;//H[0, :] = np.arange(start=0, stop=cols)
	for (int i = 1; i < rows; i++)
		H.dataL[i * cols] = i;//H[:, 0] = np.arange(start=0, stop=rows)
	var1 = var2 = var3 = bestAction = 0;
	for (int row = 1; row < rows; row++)
	{
		for (int col = 1; col < cols; col++)
		{
			// up&left | leave_or_replace_letter = H[row - 1, col - 1] + score_func(a, b)
			var1 = H.dataL[(row - 1) * cols + col - 1] +
				(seqA.pc[row - 1] == seqB.pc[col - 1] ? 2 : -1);
			// up | delete_indel = H[row - 1, col] + score
			var2 = H.dataL[(row - 1) * cols + col] +
				(gapH.dataL[(row - 1) * cols + col] == 0 ? -2 : 0);
			// left | insert_indel = H[row, col - 1] + score
			var3 = H.dataL[row * cols + (col - 1)] +
				(gapH.dataL[row * cols + (col - 1)] == 0 ? -2 : 0);
			// bestAction = argmax([var1, var2, var3])
			bestAction = (var1 >= var2 && var1 >= var3) ? 0 : (var2 >= var3 ? 1 : 2);
			// if best_action in [1, 2]: gapH[row, col] = True
			if (bestAction != 0)
				gapH.dataL[row * cols + col] = 1;
			// H[row, col] = max([var1, var2, var3])
			H.dataL[row * cols + col] = bestAction == 0 ? var1 : (bestAction == 1 ? var2 : var3);
			traceback.dataL[row * cols + col] = bestAction;
		}
	}
	//for (int i = 0; i < H.dataShape[0]; i++)
	//{
	//	for (int j = 0; j < H.dataShape[1]; j++)
	//		printf("%3d ", H.dataL[i * H.dataShape[1] + j]);
	//	printf("\n");
	//}
	return H.dataL[H.dataLen - 1];
}


void SequencesSimiCacuer::VizTraceback(ba::str seqA, ba::str seqB)
{
	if (seqA.len + 1 != traceback.dataShape[0] || seqB.len + 1 != traceback.dataShape[1])
	{
		MyBA_Err("void SequencesSimiCacuer::VizTraceback(ba::str seqA, ba::str seqB) : seqA.len+1 != traceback.dataShape[0] || seqB.len+1 != traceback.dataShape[1], return none", 1);
		return;
	}
	for (int i = 0; i < traceback.dataShape[0]; i++)
	{
		for (int j = 0; j < traceback.dataShape[1]; j++)
			printf("%2c", tracebackSymbol[traceback.dataL[i * traceback.dataShape[1] + j]]);
		printf("\n");
	}
	printf("\nseqA:");
	for (int i = 0, j = 0; i < seqA.len; i++, j++)
	{
		switch (traceback.dataL[(i + 1) * traceback.dataShape[1] + j + 1]) {
		case 0:case 1:
			printf("%c", seqA.pc[i]);
			break;
		case 2:
			printf("_");
			--i;
			++j;
			break;
		}
	}
	printf("\nseqB:");
	for (int i = 0, j = 0; i < seqA.len && j < seqB.len; i++)
	{
		switch (traceback.dataL[(i + 1) * traceback.dataShape[1] + j + 1]) {
		case 0:case 2:
			printf("%c", seqB.pc[j]);
			break;
		case 1:
			printf("_");
			++i;
			--j;
			break;
		}
	}
}




_LL SequencesSimiCacuer::CacuSequencesSimilarity2(ba::str seqA, ba::str seqB)
{
	seqALen = seqA.len;
	seqBLen = seqB.len;
	rows = seqALen + 1;
	cols = seqBLen + 1;
	H.ReCreate(BA_Shape(2, rows, cols), "l");
	var1 = var2 = var3 = bestAction = 0;
	for (int row = 1; row < rows; row++)
	{
		for (int col = 1; col < cols; col++)
		{
			// up&left | leave_or_replace_letter = H[row - 1, col - 1] + score_func(a, b)
			var1 = H.dataL[(row - 1) * cols + col - 1] +
				(seqA.pc[row - 1] == seqB.pc[col - 1] ? 1 : 0);
			// up | delete_indel = H[row - 1, col] + score
			var2 = H.dataL[(row - 1) * cols + col];
			// left | insert_indel = H[row, col - 1] + score
			var3 = H.dataL[row * cols + (col - 1)];
			// H[row, col] = max([var1, var2, var3])
			H.dataL[row * cols + col] = (var1 >= var2 && var1 >= var3) ? var1 : (var2 >= var3 ? var2 : var3);
		}
	}
	return H.dataL[H.dataLen - 1];
}
_LL SequencesSimiCacuer::CacuSequencesSimilarity2(ba::str* seqA, ba::str* seqB)
{
	seqALen = seqA->len;
	seqBLen = seqB->len;
	rows = seqALen + 1;
	cols = seqBLen + 1;
	H.ReCreate(BA_Shape(2, rows, cols), "l");
	var1 = var2 = var3 = bestAction = 0;
	for (int row = 1; row < rows; row++)
	{
		for (int col = 1; col < cols; col++)
		{
			// up&left | leave_or_replace_letter = H[row - 1, col - 1] + score_func(a, b)
			var1 = H.dataL[(row - 1) * cols + col - 1] +
				(seqA->pc[row - 1] == seqB->pc[col - 1] ? 1 : 0);
			// up | delete_indel = H[row - 1, col] + score
			var2 = H.dataL[(row - 1) * cols + col];
			// left | insert_indel = H[row, col - 1] + score
			var3 = H.dataL[row * cols + (col - 1)];
			// H[row, col] = max([var1, var2, var3])
			H.dataL[row * cols + col] = (var1 >= var2 && var1 >= var3) ? var1 : (var2 >= var3 ? var2 : var3);
		}
	}
	return H.dataL[H.dataLen - 1];
}

_LL SequencesSimiCacuer::CacuSequencesSimilarityWithTrace2(ba::str seqA, ba::str seqB)
{
	seqALen = seqA.len;
	seqBLen = seqB.len;
	rows = seqALen + 1;
	cols = seqBLen + 1;
	H.ReCreate(BA_Shape(2, rows, cols), "l");
	traceback.ReCreate(BA_Shape(2, rows, cols), "l");
	var1 = var2 = var3 = bestAction = 0;
	for (int row = 1; row < rows; row++)
	{
		for (int col = 1; col < cols; col++)
		{
			// up&left | leave_or_replace_letter = H[row - 1, col - 1] + score_func(a, b)
			var1 = H.dataL[(row - 1) * cols + col - 1] +
				(seqA.pc[row - 1] == seqB.pc[col - 1] ? 1 : 0);
			// up | delete_indel = H[row - 1, col] + score
			var2 = H.dataL[(row - 1) * cols + col];
			// left | insert_indel = H[row, col - 1] + score
			var3 = H.dataL[row * cols + (col - 1)];
			// bestAction = argmax([var1, var2, var3])
			bestAction = (var1 >= var2 && var1 >= var3) ? 0 : (var2 >= var3 ? 1 : 2);
			// H[row, col] = max([var1, var2, var3])
			H.dataL[row * cols + col] = bestAction == 0 ? var1 : (bestAction == 1 ? var2 : var3);
			traceback.dataL[row * cols + col] = bestAction;
		}
	}
	//for (int i = 0; i < H.dataShape[0]; i++)
	//{
	//	for (int j = 0; j < H.dataShape[1]; j++)
	//		printf("%3d ", H.dataL[i * H.dataShape[1] + j]);
	//	printf("\n");
	//}
	return H.dataL[H.dataLen - 1];
}

void SequencesSimiCacuer::VizTraceback2(ba::str seqA, ba::str seqB)
{
	if (seqA.len + 1 != traceback.dataShape[0] || seqB.len + 1 != traceback.dataShape[1])
	{
		MyBA_Err("void SequencesSimiCacuer::VizTraceback(ba::str seqA, ba::str seqB) : seqA.len+1 != traceback.dataShape[0] || seqB.len+1 != traceback.dataShape[1], return none", 1);
		return;
	}
	for (int i = 0; i < H.dataShape[0]; i++)
	{
		for (int j = 0; j < H.dataShape[1]; j++)
			printf("%3lld ", H.dataL[i * H.dataShape[1] + j]);
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < H.dataShape[0]; i++)
	{
		for (int j = 0; j < H.dataShape[1]; j++)
			printf("%3lld ", traceback.dataL[i * H.dataShape[1] + j]);
		printf("\n");
	}
	_ULL maxLen = seqA.len > seqB.len ? seqA.len : seqB.len;
	ba::str seqAs = ba::str("_").Repeat(maxLen);
	ba::str seqBs = ba::str("_").Repeat(maxLen);
	for (_LL seqAIdx = seqA.len, seqBIdx = seqB.len, seqIdx = maxLen - 1; seqAIdx >= 0 && seqBIdx >= 0 && seqIdx >= 0; seqIdx--)
	{
		switch (traceback.dataL[(seqAIdx) * traceback.dataShape[1] + seqBIdx]) {
		case 0:
			seqAs.pc[seqIdx] = seqA.pc[seqAIdx - 1];
			seqBs.pc[seqIdx] = seqB.pc[seqBIdx - 1];
			--seqAIdx;
			--seqBIdx;
			break;
		case 1:
			seqAs.pc[seqIdx] = seqA.pc[seqAIdx - 1];
			--seqAIdx;
			break;
		case 2:
			seqBs.pc[seqIdx] = seqB.pc[seqBIdx - 1];
			--seqBIdx;
			break;
		}
	}
	printf("\nseqA:%s\nseqB:%s\n", seqAs.pc, seqBs.pc);
}
