#include "swaptabulist.h"
#include "debug.h"

SwapTabuList::SwapTabuList(int size) : MAX_SIZE(size) { }

void SwapTabuList::add(int a, int b) {
  if (b < a) {
    std::swap(a, b);
  }
  _list.push_back(std::make_pair(a, b));
  if (_list.size() > MAX_SIZE) {
    _list.pop_front();
  }
}

bool SwapTabuList::contains(int a, int b) {
  int lsize = _list.size();
  if (b < a) {
    std::swap(a, b);
  }
  for (int i = 0; i < lsize; i++) {
    const std::pair<int, int> &p = _list[i];
    if (p.first == a && p.second == b) {
      return true;
    }
  }
  return false;
}

void SwapTabuList::print() {
  DBG("////////////////////////");
  DBG("/Printing Tabu List:");
  int lsize = _list.size();
  for (int i = 0; i < lsize; i++) {
    DBG("/List element: " << _list[i].first << " " << _list[i].second);
  }
}