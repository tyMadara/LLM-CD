#include "pivotresult.h"
#include "debug.h"

PivotResult::PivotResult(Types::Score score, int swapIdx, Ordering ordering)
  : score(score), swapIdx(swapIdx), ordering(ordering) { }
  
int PivotResult::getSwapIdx() const {
  return swapIdx;
}

Types::Score PivotResult::getScore() const {
  return score;
}

const Ordering &PivotResult::getOrdering() {
  return ordering;
}

std::ostream& operator<<(std::ostream &os, const PivotResult& v) {
  os << "PivotResult(score=" << v.score << ", swapIdx=" << v.swapIdx << ", ordering=" << v.ordering << ")";
  return os;
}