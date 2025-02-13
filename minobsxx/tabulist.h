#include<deque>
#include "ordering.h"

#ifndef TABULIST_H
#define TABULIST_H 

class TabuList {
  public:
    int MAX_SIZE;
    void add(const Ordering &o);
    TabuList(int size);
    bool contains(const Ordering &o);
  private:
    std::deque<Ordering> _list;
};

#endif /* TABULIST_H */