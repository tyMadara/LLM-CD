#ifndef PIVOTRESULT_H
#define PIVOTRESULT_H 


#include "ordering.h"
#include "types.h"

class PivotResult {
  public:
    PivotResult(Types::Score score, int swapIdx, Ordering ordering);
    int getSwapIdx() const;
    Types::Score getScore() const;
    const Ordering &getOrdering();
    friend std::ostream& operator<<(std::ostream &os, const PivotResult& v);
  private:
    Types::Score score;
    int swapIdx;
    Ordering ordering;
};

#endif /* PIVOTRESULT_H */