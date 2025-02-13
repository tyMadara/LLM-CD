#include "movetabulist.h"
#include "debug.h"

MoveTabuList::MoveTabuList(int size, int n) : MAX_SIZE(size), bucketSizes(n) { }

void MoveTabuList::add(int id, int index) {
  _list.push_back(std::make_pair(id, index));
  if (_list.size() > MAX_SIZE) {
    std::pair<int, int> popped = _list.front();
    --bucketSizes[popped.first];
    _list.pop_front();
  }
  ++bucketSizes[id];
  /*
  for (int i = 0; i < _list.size(); i++) {
    DBG(_list[i].first << " " << _list[i].second);
  }
  */
  for (int i = 0; i < bucketSizes.size(); i++) {
    DBG(bucketSizes[i]);
  }
}

bool MoveTabuList::contains(const Ordering &o) {
  int n = o.getSize();
  int lsize = _list.size();
  for (int i = 0; i < n; i++) {
    int num = o.get(i);
    if (bucketSizes[num] > 0) {
      for (int j = 0; j < lsize; j++) {
        if (_list[j].first == num && _list[j].second == i) {
          return true;
        }
      }
    }
  }
  return false;
}

bool MoveTabuList::contains(int i) {
  return bucketSizes[i] > 0;
}