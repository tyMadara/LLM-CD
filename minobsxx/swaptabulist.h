#include<deque>
#include "ordering.h"

#ifndef SWAPTABULIST_H
#define SWAPTABULIST_H 

class SwapTabuList {
  public:
    int MAX_SIZE;
    void add(int a, int b);
    SwapTabuList(int size);
    bool contains(int a, int b);
    void print();
  private:
    std::deque<std::pair<int, int>> _list;
};

#endif /* SWAPTABULIST_H */