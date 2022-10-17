#ifndef CPPJIBEA_HMMSEGMENT_H
#define CPPJIBEA_HMMSEGMENT_H

#include <iostream>
#include <fstream>
#include <memory.h>
#include <cassert>
#include "HMMModel.hpp"
#include "SegmentBase.hpp"

namespace cppjieba {
class HMMSegment: public SegmentBase {
 public:
  HMMSegment(const string& filePath)
  : model_(new HMMModel(filePath)), isNeedDestroy_(true) {
  }
  HMMSegment(const HMMModel* model) 
  : model_(model), isNeedDestroy_(false) {
  }
  ~HMMSegment() {
    if (isNeedDestroy_) {
      delete model_;
    }
  }

  void Cut(const string& sentence, vector<string>& words) const;
  void Cut(const string& sentence, vector<Word>& words) const;
  void Cut(RuneStrArray::const_iterator begin,
      RuneStrArray::const_iterator end, vector<WordRange>& res) const;
 private:
  // sequential letters rule
  RuneStrArray::const_iterator SequentialLetterRule(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const {
    Rune x = begin->rune;
    if (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z')) {
      begin ++;
    } else {
      return begin;
    }
    while (begin != end) {
      x = begin->rune;
      if (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z') || ('0' <= x && x <= '9')) {
        begin ++;
      } else {
        break;
      }
    }
    return begin;
  }
  //
  RuneStrArray::const_iterator NumbersRule(RuneStrArray::const_iterator begin, RuneStrArray::const_iterator end) const {
    Rune x = begin->rune;
    if ('0' <= x && x <= '9') {
      begin ++;
    } else {
      return begin;
    }
    while (begin != end) {
      x = begin->rune;
      if ( ('0' <= x && x <= '9') || x == '.') {
        begin++;
      } else {
        break;
      }
    }
    return begin;
  }

  void Viterbi(RuneStrArray::const_iterator begin,
      RuneStrArray::const_iterator end,
      vector<size_t>& status) const;

  void InternalCut(RuneStrArray::const_iterator begin,
      RuneStrArray::const_iterator end, vector<WordRange>& res) const;


  const HMMModel* model_;
  bool isNeedDestroy_;
}; // class HMMSegment

} // namespace cppjieba

#endif
