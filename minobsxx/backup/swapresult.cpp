#include "swapresult.h"

SwapResult::SwapResult(Types::Score scoreB, Types::Score scoreA, int parentI, int parentJ) :
  scoreB(scoreB), scoreA(scoreA), newParentSets(parentI, parentJ) { }
 
 std::ostream& operator<<(std::ostream &os, const SwapResult& sr) {
  os << "SwapResult(scoreB="<< sr.scoreB << ", scoreA=" << sr.scoreA << ", first=" << sr.newParentSets.first << ", second=" << sr.newParentSets.second << ")";
  return os;
}
 
Types::Score SwapResult::getScore() const {
  return scoreA + scoreB;
}

std::pair<Types::Score, Types::Score> SwapResult::getScores() const {
  std::pair<Types::Score, Types::Score> ret(scoreB, scoreA);
  return ret;
}

std::pair<int, int> SwapResult::getParentSets() const {
  return newParentSets;
}