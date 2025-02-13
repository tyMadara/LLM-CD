#include "util.h"
#include "debug.h"


bool Util::isOpt(const SearchResult &sr, const Types::Score &opt) {
  double diff = sr.getScore() - opt;
  const float EPSILON = 0.005;
  return 100*(diff/(double)opt) < EPSILON;
}

std::pair<int, int> Util::getUniquePair(int n) {
  int i = rand()%n;
  int j = rand()%(n-1);
  if (j >= i) {
    j += 1;
  }
  return std::make_pair(i, j);
}