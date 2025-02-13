#ifndef FASTPIVOTRESULT_H
#define FASTPIVOTRESULT_H

#include "ordering.h"
#include "types.h"

class FastPivotResult {
  public:
    FastPivotResult(Types::Score score, int swapIdx, Ordering ordering, std::vector<int> parents, std::vector<Types::Score> scores);
    int getSwapIdx() const;
    Types::Score getScore() const;
    const Ordering &getOrdering();
    friend std::ostream& operator<<(std::ostream &os, const FastPivotResult& v);
    const std::vector<Types::Score> &getScores();
    const std::vector<int> &getParents();
    
  private:
    Types::Score score;
    int swapIdx;
    std::vector<int> parents;
    std::vector<Types::Score> scores;
    Ordering ordering;
};

#endif /* FASTPIVOTRESULT_H */