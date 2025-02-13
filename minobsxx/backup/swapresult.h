#ifndef SWAPRESULT_H
#define SWAPRESULT_H

#include <vector>
#include <utility>
#include <iostream>
#include "types.h"
class SwapResult {
  public:
    SwapResult(Types::Score scoreA, Types::Score scoreB, int parentI, int parentJ);
    friend std::ostream& operator<<(std::ostream &os, const SwapResult& sr);
    Types::Score getScore() const;
    std::pair<int, int> getParentSets() const;
    std::pair<Types::Score, Types::Score> getScores() const;
  private:
    Types::Score scoreB;
    Types::Score scoreA;
    std::pair<int, int> newParentSets;
};

#endif /* SWAPRESULT_H */