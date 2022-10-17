
#include "cppjieba/HMMSegment.hpp"

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

void cppjieba::HMMSegment::Cut(const string& sentence, vector<string>& words)
const
{
	vector<Word> tmp;
	Cut(sentence, tmp);
	GetStringsFromWords(tmp, words);
}

void cppjieba::HMMSegment::Cut(const string& sentence, vector<Word>& words)
const
{
	PreFilter pre_filter(symbols_, sentence);
	PreFilter::Range range;
	vector<WordRange> wrs;
	wrs.reserve(sentence.size() / 2);
	while (pre_filter.HasNext()) {
		range = pre_filter.Next();
		Cut(range.begin, range.end, wrs);
	}
	words.clear();
	words.reserve(wrs.size());
	GetWordsFromWordRanges(sentence, wrs, words);
}

void cppjieba::HMMSegment::Cut(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<WordRange>& res)
const
{
	RuneStrArray::const_iterator left = begin;
	RuneStrArray::const_iterator right = begin;
	while (right != end) {
		if (right->rune < 0x80) {
			if (left != right) {
				InternalCut(left, right, res);
			}
			left = right;
			do {
				right = SequentialLetterRule(left, end);
				if (right != left) {
					break;
				}
				right = NumbersRule(left, end);
				if (right != left) {
					break;
				}
				right++;
			} while (false);
			WordRange wr(left, right - 1);
			res.push_back(wr);
			left = right;
		}
		else {
			right++;
		}
	}
	if (left != right) {
		InternalCut(left, right, res);
	}
}

void cppjieba::HMMSegment::Viterbi(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<size_t>& status)
const
{
	size_t Y = HMMModel::STATUS_SUM;
	size_t X = end - begin;

	size_t XYSize = X * Y;
	size_t now, old, stat;
	double tmp, endE, endS;

	vector<int> path(XYSize);
	vector<double> weight(XYSize);

	//start
	for (size_t y = 0; y < Y; y++) {
		weight[0 + y * X] = model_->startProb[y] + model_->GetEmitProb(model_->emitProbVec[y], begin->rune, MIN_DOUBLE);
		path[0 + y * X] = -1;
	}

	double emitProb;

	for (size_t x = 1; x < X; x++) {
		for (size_t y = 0; y < Y; y++) {
			now = x + y * X;
			weight[now] = MIN_DOUBLE;
			path[now] = HMMModel::E; // warning
			emitProb = model_->GetEmitProb(model_->emitProbVec[y], (begin + x)->rune, MIN_DOUBLE);
			for (size_t preY = 0; preY < Y; preY++) {
				old = x - 1 + preY * X;
				tmp = weight[old] + model_->transProb[preY][y] + emitProb;
				if (tmp > weight[now]) {
					weight[now] = tmp;
					path[now] = preY;
				}
			}
		}
	}

	endE = weight[X - 1 + HMMModel::E * X];
	endS = weight[X - 1 + HMMModel::S * X];
	stat = 0;
	if (endE >= endS) {
		stat = HMMModel::E;
	}
	else {
		stat = HMMModel::S;
	}

	status.resize(X);
	for (int x = X - 1; x >= 0; x--) {//warning C4267
		status[x] = stat;
		stat = path[x + stat * X];
	}
}

void cppjieba::HMMSegment::InternalCut(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end, vector<WordRange>& res) const
const
 {
    vector<size_t> status;
    Viterbi(begin, end, status);

    RuneStrArray::const_iterator left = begin;
    RuneStrArray::const_iterator right;
    for (size_t i = 0; i < status.size(); i++) {
      if (status[i] % 2) { //if (HMMModel::E == status[i] || HMMModel::S == status[i])
        right = begin + i + 1;
        WordRange wr(left, right - 1);
        res.push_back(wr);
        left = right;
      }
    }
  }
