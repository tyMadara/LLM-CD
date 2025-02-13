#include "searchresult.h"

SearchResult::SearchResult(Types::Score score, Ordering ordering) :
  score(score), ordering(ordering) { }
  
Types::Score SearchResult::getScore() const {
  return score;
}

Ordering SearchResult::getOrdering() const {
  return ordering;
}

std::ostream& operator<<(std::ostream &os, const SearchResult& v) {
  os << "SearchResult(score=" << v.score << ", ordering=" << v.ordering << ")";
  return os;
}

 Ordering &SearchResult::getOrderingRef() {
   return ordering;
 }