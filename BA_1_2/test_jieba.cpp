// from https://github.com/yanyiwu/cppjieba

#include"BA_Base.hpp"
#include"BA_CMD.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Test.hpp"
#include"BA_JSON.hpp"
#include"BA_Jieba.hpp"
#include"cppjieba/Jieba.hpp"

using namespace std;

const char* const DICT_PATH = "D:/AI/PreTrainModel/jieba/jieba.dict.utf8";
const char* const HMM_PATH = "D:/AI/PreTrainModel/jieba/hmm_model.utf8";
const char* const USER_DICT_PATH = "D:/AI/PreTrainModel/jieba/user.dict.utf8";
const char* const IDF_PATH = "D:/AI/PreTrainModel/jieba/idf.utf8";
const char* const STOP_WORD_PATH = "D:/AI/PreTrainModel/jieba/stop_words.utf8";

void ba::test::_jieba::jiebaDemo(void)
{
    cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);
    vector<string> words;
    vector<cppjieba::Word> jiebawords;
    string s;
    string result;

    s = ba::transferStrCode("�����������׺��д���", "gbk", "utf-8");
    cout << ba::transferStrCode(s.c_str(), "utf-8", "gbk") << endl;
    cout << "[demo] Cut With HMM" << endl;
    jieba.Cut(s, words, true);
    //cout << limonp::Join(words.begin(), words.end(), "/") << endl;
    for (int i = 0; i < words.size(); i++)
        cout << ba::transferStrCode(words[i].c_str(), "utf-8", "gbk") << "/";

    cout << "[demo] Cut Without HMM " << endl;
    jieba.Cut(s, words, false);
    //cout << limonp::Join(words.begin(), words.end(), "/") << endl;
    for (int i = 0; i < words.size(); i++)
        cout << ba::transferStrCode(words[i].c_str(), "utf-8", "gbk") << "/";

    s = "�����������廪��ѧ";
    cout << s << endl;
    cout << "[demo] CutAll" << endl;
    jieba.CutAll(s, words);
    cout << limonp::Join(words.begin(), words.end(), "/") << endl;

    s = "С��˶ʿ��ҵ���й���ѧԺ�������������ձ�������ѧ����";
    cout << s << endl;
    cout << "[demo] CutForSearch" << endl;
    jieba.CutForSearch(s, words);
    cout << limonp::Join(words.begin(), words.end(), "/") << endl;

    cout << "[demo] Insert User Word" << endl;
    jieba.Cut("��ĬŮ��", words);
    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
    jieba.InsertUserWord("��ĬŮ��");
    jieba.Cut("��ĬŮ��", words);
    cout << limonp::Join(words.begin(), words.end(), "/") << endl;

    cout << "[demo] CutForSearch Word With Offset" << endl;
    jieba.CutForSearch(s, jiebawords, true);
    cout << jiebawords << endl;

    cout << "[demo] Lookup Tag for Single Token" << endl;
    const int DemoTokenMaxLen = 32;
    char DemoTokens[][DemoTokenMaxLen] = { "������", "CEO", "123", "��" };
    vector<pair<string, string> > LookupTagres(sizeof(DemoTokens) / DemoTokenMaxLen);
    vector<pair<string, string> >::iterator it;
    for (it = LookupTagres.begin(); it != LookupTagres.end(); it++) {
        it->first = DemoTokens[it - LookupTagres.begin()];
        it->second = jieba.LookupTag(it->first);
    }
    cout << LookupTagres << endl;

    cout << "[demo] Tagging" << endl;
    vector<pair<string, string> > tagres;
    s = "����������ѧԺ�ַ�������רҵ�ġ����ö�ã��Ҿͻ���ְ��н������CEO�����������۷塣";
    jieba.Tag(s, tagres);
    cout << s << endl;
    cout << tagres << endl;

    cout << "[demo] Keyword Extraction" << endl;
    const size_t topk = 5;
    vector<cppjieba::KeywordExtractor::Word> keywordres;
    jieba.extractor.Extract(s, keywordres, topk);
    cout << s << endl;
    cout << keywordres << endl;
}

void ba::test::_jieba::jiebaDealFile(void)
{
    cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);
    vector<string> words;
    vector<cppjieba::Word> jiebawords;
    string s;
    string result;

    std::ifstream ifs(R"(D:\AI\DataSet\Seq2ImgFluently\seq\text Seq\������.txt)",
        std::ifstream::in);
    char* pc = ba::read(ifs, (List*)NULL);
    if (ba::detectTextCode(ifs) == 0)//gbk
        s = ba::transferStrCode(pc, "gbk", "utf-8");
    else
        s = pc;
    jieba.Cut(s, words, true);
    for (int i = 0; i < 100; i++)
        cout << ba::transferStrCode(words[i].c_str(), "utf-8", "gbk") << "/";
}

void ba::test::_jieba::jiebaClass(void)
{
    ba::jieba jb = ba::jieba();
    std::ifstream ifs(R"(D:\AI\DataSet\Seq2ImgFluently\seq\text Seq\������.txt)",
        std::ifstream::in);
    char* pc = ba::read(ifs, (List*)NULL);
    std::cout << jb.cut2string(pc) << std::endl;
}