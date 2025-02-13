#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H 

#include "ordering.h"
#include "types.h"

class SearchResult {
  public:
    SearchResult() {}
    SearchResult(Types::Score score, Ordering ordering);
    friend std::ostream& operator<<(std::ostream &os, const SearchResult& sr);
    Types::Score getScore() const;
    Ordering getOrdering() const;
    Ordering &getOrderingRef();
  private:
    Types::Score score;
    Ordering ordering;
};

#endif /* SEARCHRESULT_H */
