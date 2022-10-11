#pragma once
#include"BA_Base.hpp"
#include"cppjieba/Jieba.hpp"


namespace ba
{
	class jieba
	{
	public:
		const char* DICT_PATH = "D:/AI/PreTrainModel/jieba/jieba.dict.utf8";
		const char* HMM_PATH = "D:/AI/PreTrainModel/jieba/hmm_model.utf8";
		const char* USER_DICT_PATH = "D:/AI/PreTrainModel/jieba/user.dict.utf8";
		const char* IDF_PATH = "D:/AI/PreTrainModel/jieba/idf.utf8";
		const char* STOP_WORD_PATH = "D:/AI/PreTrainModel/jieba/stop_words.utf8";
		cppjieba::Jieba jb = cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
		std::vector<std::string> words;
		std::vector<cppjieba::Word> jiebawords;
		std::string s;
		std::string result;
		char* tmp = NULL;

		// remain pc unfreed
		void cut2vector(char* pc, int inCode = 0);
		// remain pc unfreed
		void cut2vector(char* pc, const char* inCode = "gbk");
		// remain pc unfreed
		std::string& cut2string(char* pc, const char* spliter = "/",
			const char* inCode = "gbk", const char* outCode = "gbk");
	};
}