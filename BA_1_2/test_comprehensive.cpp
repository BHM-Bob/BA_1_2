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

void ba::test::_comprehensive::jiebaThreads_SubThr(_LL id, balist<char>& getQ, balist<char>& putQ, balist<float>& proc, balist<bool>& sig, void* data)
{
	const char* wordsDict = R"(D:\AI\DataSet\Seq2ImgFluently\w2v\words.json)";
	const char* textRoot = R"(D:\AI\DataSet\Seq2ImgFluently\seq\text Seq)";
	// jieba
	ba::jieba jb = ba::jieba();
	// read wordsDict
	std::ifstream ifs(wordsDict, std::ifstream::in);
	json j4;
	ifs >> j4;
	int idx = 0, sum = 100;
	for (char* name = getQ.ThrGet(&m1), *path = NULL, *pc = NULL;
		name && idx < sum; name = getQ.ThrGet(&m1), idx++)
	{
		path = StrAdd(NULL, textRoot, "\\", name, NULL);
		pc = ReadTXT(path);
		free(path);
		jb.cut(pc, "/", "gbk", "utf-8");
		putQ.ThrPut(pc, &m1);
		JDT(idx, sum);
	}
}

void ba::test::_comprehensive::jiebaThreads(void)
{
	_LL sumThreads = 8;
	MyThreadsPool tp = MyThreadsPool(sumThreads, jiebaThreads_SubThr, NULL, NULL, 20000);
	// read texts, almost ANSI
	const char* textRoot = R"(D:\AI\DataSet\Seq2ImgFluently\seq\text Seq)";
	ba::dir dir = ba::dir(textRoot, "txt");
	for (_LL idx = 0, sum = 100; idx < sum; idx++)
		tp.PutTask(dir.files->Copy(), &m1);
	auto result = tp.LoopToQuit(&m1);
	tp.Destroy(&m1);
}