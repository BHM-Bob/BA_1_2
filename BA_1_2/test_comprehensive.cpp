#include"BA_Base.hpp"
#include"BA_CMD.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Test.hpp"
#include"BA_JSON.hpp"
#include"cppjieba/Jieba.hpp"

using json = nlohmann::json;

std::mutex m1;

void ba::test::_comprehensive::jiebaThreads_SubThr(_LL id, balist<char>& getQ,
	balist<std::vector<int>>& putQ, balist<float>& proc, balist<bool>& sig, void* data)
{
	const char* wordsDict = R"(D:\AI\DataSet\Seq2ImgFluently\w2v\words.json)";
	const char* textRoot = R"(D:\AI\DataSet\Seq2ImgFluently\seq\text Seq)";
	ba::jieba jb = ba::jieba();
	std::ifstream ifs(wordsDict, std::ifstream::in);
	json w2i;
	ifs >> w2i;
	ifs.close();
	int idx = 0, sum = getQ.ThrSize(&m1), fileCode = -1;
	std::vector<int>* result = NULL;
	char* word = NULL;
	for (char* name = getQ.ThrGet(&m1), *path = NULL, *pc = NULL;
		name && name != (char*)1; name = getQ.ThrGet(&m1), idx++)
	{
		path = StrAdd(NULL, textRoot, "\\", name, NULL); PPX(pba->GUT(1));
		ifs.open(path, std::ifstream::in);
		fileCode = ba::detectTextCode(ifs);
		pc = ba::read(ifs, (List*)NULL);
		free(path);
		jb.cut2vector(pc, fileCode == 0 ? "gbk" : "utf-8");
		free(pc);
		result = new std::vector<int>(); PPX(pba->GUT(0));
		for (std::string w : jb.words)
		{
			if (w2i.contains(w))
				result->emplace_back(w2i[w]);
		}PPX(pba->GUT(0));
		putQ.ThrPut(result, &m1);
		proc.ThrPut(TypeDupR(NULL, 1, 100.f * (float)idx / sum), &m1);
	}
	sig.ThrPut(0, &m1);;
}

void ba::test::_comprehensive::jiebaThreads(void)
{
	// read texts, almost ANSI
	const char* textRoot = R"(D:\AI\DataSet\Seq2ImgFluently\seq\text Seq)";
	ba::dir dir = ba::dir(textRoot, "txt");
	PPX(dir.files->Index((char*)1));
	_LL sumThreads = 1;
	MyThreadsPool tp = MyThreadsPool(sumThreads, jiebaThreads_SubThr, NULL, NULL, 10000);
	tp.logMode = true;
	for (_LL idx = 0, sum = 4; idx < sum; idx++)
		tp.PutTask(dir.files->Copy(), &m1);
	auto result = tp.LoopToQuit(&m1);
	tp.Destroy(&m1);
}