
#include"BA_Base.hpp"
#include"BA_Jieba.hpp"
#include"BA_String.hpp"

void ba::jieba::cut2vector(char* pc, int inCode)
{
	s.clear();
	result.clear();
	if (inCode == 0 || inCode == 2)
	{
		tmp = ba::transferStrCode(pc, inCode, 1);
		s = tmp;
		free(tmp);
	}
	else
	{
		s = pc;
	}
	jb.Cut(s, words, true);
	s.clear();
}

void ba::jieba::cut2vector(char* pc, const char* inCode)
{
	s.clear();
	result.clear();
	if (strcmp(inCode, "gbk") == 0 || strcmp(inCode, "unicode") == 0)
	{
		tmp = ba::transferStrCode(pc, inCode, "utf-8");
		s = tmp;
		free(tmp);
	}
	else
	{
		s = pc;
	}
	jb.Cut(s, words, true);
	s.clear();
}
std::string& ba::jieba::cut2string(char* pc, const char* spliter,
	const char* inCode, const char* outCode)
{
	cut2vector(pc, inCode);
	if (strcmp(outCode, "gbk") == 0)
	{
		for (int i = 0; i < words.size(); i++)
		{
			tmp = ba::transferStrCode(words[i].c_str(), "utf-8", "gbk");
			words[i].assign(tmp);
			free(tmp);
		}
	}
	result = limonp::Join(words.begin(), words.end(), spliter);
	return result;
}
