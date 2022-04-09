//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2022年4月9日 23点00分

#include"BA_Base.hpp"
#include"BA_String.hpp"
#include "BA_BioInfo.hpp"


_LL SequencesSimiCacuer::CacuSequencesSimilarity(BA_String seqA, BA_String seqB)
{
	int seqALen = seqA.len, seqBLen = seqB.len;
	int rows = seqALen + 1, cols = seqBLen + 1;
	H.ReCreate(BA_Shape(2, rows, cols), "l");
	gapH.ReCreate(BA_Shape(2, rows, cols), "l");
	for (int i = 1; i < cols; i++)
		H.dataL[i] = i;//H[0, :] = np.arange(start=0, stop=cols)
	for (int i = 1; i < rows; i++)
		H.dataL[i * cols] = i;//H[:, 0] = np.arange(start=0, stop=rows)
	int var1, var2, var3, bestAction;
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
	return H.dataL[H.dataLen - 1];
}
