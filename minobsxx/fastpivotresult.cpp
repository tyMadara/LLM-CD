#include "fastpivotresult.h"
#include "debug.h"

FastPivotResult::FastPivotResult(Types::Score score, int swapIdx, Ordering ordering, std::vector<int> parents, std::vector<Types::Score> scores)
  : score(score), swapIdx(swapIdx), parents(parents), scores(scores), ordering(ordering) { }
  
int FastPivotResult::getSwapIdx() const {
  return swapIdx;
}

Types::Score FastPivotResult::getScore() const {
  return score;
}

const Ordering &FastPivotResult::getOrdering() {
  return ordering;
}

std::ostream& operator<<(std::ostream &os, const FastPivotResult& v) {
  os << "FastPivotResult(score=" << v.score << ", swapIdx=" << v.swapIdx << ", ordering=" << v.ordering << ")";
  return os;
}

const std::vector<Types::Score> &FastPivotResult::getScores() {
  return scores;
}

const std::vector<int> &FastPivotResult::getParents() {
  return parents;
}