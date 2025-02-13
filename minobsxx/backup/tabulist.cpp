#include "tabulist.h"
#include "debug.h"

TabuList::TabuList(int size) : MAX_SIZE(size) { }

void TabuList::add(const Ordering &o) {
  _list.push_back(o);
  if (_list.size() > MAX_SIZE) {
    _list.pop_front();
  }
}

bool TabuList::contains(const Ordering &o) {
  int n = _list.size();
  for (int i = 0; i < n; i++) {
    if (o.equals(_list[i])) {
      return true;
    }
  }
  return false;
}