#include"BA_Base.hpp"
#include"BA_CMD.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Math.hpp"
#include"BA_Test.hpp"
#include"BA_JSON.hpp"
#include"BA_Jieba.hpp"
#include"BA_glob.hpp"
#include"cppjieba/Jieba.hpp"

using json = nlohmann::json;

std::mutex m1;

void ba::test::_comprehensive::tensor(void)
{
	ba::tensor t = ba::tensor({ 5, 5 }, 0.f);
	float f = t[3](3);

	ba::tensor t2 = ba::tensor({ 5, 5 }, 2.f);
	ba::tensor t3 = t.map(t2, [&](float r, float l) {return r + l; });
	//(t-2.f) - t2*t2;//E0349
	t.map([&](float r) {return r + 2.0; });

	ba::tensor t4 = ba::tensor({ 5, 5 }, (char*)1);
	PPX(t4.cast(0ULL)[3](3));

	PPX(t4.map(0ULL, t, [&](char* p, float f) {return (_ULL)p + (_ULL)&f;})[3](3));
}

void ba::test::_comprehensive::jiebaThreads_SubThr(_LL id, balist<char>& getQ,
	balist<std::pair<char*, std::vector<int>*>>& putQ, balist<float>& proc, balist<bool>& sig, void* data)
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
	std::pair<char*, std::vector<int>*>* pack = NULL;
	char* word = NULL;
	for (char* name = getQ.ThrGet(&m1), *path = NULL, *pc = NULL;
		name && name != (char*)1; name = getQ.ThrGet(&m1), idx++)
	{
		path = StrAdd(NULL, textRoot, "\\", name, NULL);
		ifs.open(path, std::ifstream::in);
		fileCode = ba::detectTextCode(ifs);
		if (fileCode >= 0 && fileCode <= 2)
		{
			pc = ba::read(ifs, (List*)NULL);
			jb.cut2vector(pc, fileCode);
			free(pc);
			result = new std::vector<int>();
			for (std::string w : jb.words)
				if (w2i.contains(w))
					result->emplace_back(w2i[w]);
			pack = new std::pair<char*, std::vector<int>*>(name, result);
			putQ.ThrPut(pack, &m1);
		}
		else
		{
			ifs.close();
		}
		free(path);
	}
	sig.ThrPut(0, &m1);
}

void ba::test::_comprehensive::jiebaThreads(void)
{
	const char* textRoot = R"(D:\AI\DataSet\Seq2ImgFluently\seq\text Seq)";
	ba::dir dir = ba::dir(textRoot, "txt");
	_LL sumThreads = 8;
	MyThreadsPool tp = MyThreadsPool(sumThreads, jiebaThreads_SubThr, NULL, NULL, 5000);
	for (_LL idx = 0, sum = dir.files->sumque; idx < sum; idx++)
		tp.PutTask(dir.files->Copy(), &m1);
	auto result = tp.LoopToQuit(&m1);
	json seq;
	_LL idx = 0;
	for (auto p = result->Copy(); p; p = result->Copy(), idx++)
		seq[p->first] = *(p->second);
	std::ofstream ifs(R"(D:\AI\DataSet\Seq2ImgFluently\w2v\cppjieba.bson)", std::ifstream::trunc);
	auto v = json::to_bson(seq);
	ifs.write((char*)&v[0], sizeof(uint8_t) * v.size());
	ifs.close();
	tp.Destroy(&m1);
	//import bson
	// with open(path, 'rb') as f:
	//   data = bson.decode_all(f.read())
}

void ba::test::_comprehensive::glob(void)
{
	// Match on a single pattern
	for (auto& p : ba::glob(R"(D:\AI\DataSet\_102Flowers\torch\*\*.jpg)"))
	{
		PPX(p.filename());
	}
}
