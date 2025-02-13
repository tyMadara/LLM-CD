#include<deque>
#include "ordering.h"
#include "util.h"

#ifndef MOVETABULIST_H
#define MOVETABULIST_H 

class MoveTabuList {
  public:
    int MAX_SIZE;
    void add(int id, int index);
    MoveTabuList(int size, int n);
    bool contains(const Ordering &o);
    bool contains(int i);
  private:
    std::deque<std::pair<int, int>> _list;
    std::vector<int> bucketSizes;
};

#endif /* MOVETABULIST_H */