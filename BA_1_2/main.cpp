#include "BA_1_2.hpp"

int main(int argc, char** argvs)
{
	MyBA_Init();

	//BA_String seqA = BA_String("GGATCGA");
	//BA_String seqB = BA_String("GAATTCAGTTA");
	//SequencesSimiCacuer seqCacuer = SequencesSimiCacuer();
	//PPS(seqA.pc);
	//PPS(seqB.pc);
	//PPL(seqCacuer.CacuSequencesSimilarityWithTrace2(seqA, seqB));
	//seqCacuer.VizTraceback2(seqA, seqB);
	CacuSimiMatFromFile(
		"D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\RNASeqs.txt",
		8592246,
		"D:\\AI\\DataSet\\AlphaMedia\\rna2img\\seq\\result2.txt");

	return MyBA_Quit();
}