#ifndef UTIL_H
#define UTIL_H 
#include "searchresult.h"
#include <utility>
#include "types.h"
class Util {
  public:
    static bool isOpt(const SearchResult &sr, const Types::Score &opt);
    static std::pair<int, int> getUniquePair(int n);
};

#endif /* UTIL_H */