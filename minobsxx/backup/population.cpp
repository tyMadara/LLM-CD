#include "population.h"
#include "assert.h"
#include <map>
#include "debug.h"
#include <numeric>
#include <algorithm>
Population::Population(LocalSearch &localSearch) :
  localSearch(localSearch) { }

int Population::getSize() const {
  return specimens.size();
}

void Population::addSpecimen(const SearchResult &o) {
  specimens.push_back(o);
}

SearchResult Population::getSpecimen(int i) const {
  return specimens[i];
}

std::ostream& operator<<(std::ostream &os, const Population& v) {
  int n = v.getSize();
  for (int i = 0; i < n; i++) {
    if (i != 0) {
      os << std::endl;
    }
    os << v.getSpecimen(i);
  }
  return os;
}

void Population::addCrossovers(int n, CrossoverType crossoverType, std::vector<SearchResult> &offspring) {
  for (int i = 0; i < n; i++) {
    int numOrderings = getSize();
    int a = rand()%numOrderings;
    int b = rand()%(numOrderings - 1);
    if (b >= a) {
      b += 1;
    }
    const Ordering &o1 = specimens[a].getOrdering();
    const Ordering &o2 = specimens[b].getOrdering();
    DBG("Crossing: (" << specimens[a] << "), (" << specimens[b] << ")");
    Ordering crossed(o1.getSize());
    if (crossoverType == CrossoverType::OB) {
      crossed = crossoverOB(o1, o2);
    } else if (crossoverType == CrossoverType::CX) {
      crossed = crossoverCX(o1, o2);
    } else {
      crossed = crossoverRK(o1, o2);
    }
    
    SearchResult sr = localSearch.hillClimb(crossed);
    DBG("Crossed: " << crossed);
    DBG("Crossed Result: " << sr);
    offspring.push_back(sr);
  }
}

Ordering Population::crossoverOB(const Ordering &o1, const Ordering &o2) {
  assert(o1.getSize() == o2.getSize());
  int n = o1.getSize();
  Ordering crossed = Ordering(n);
  Types::Bitset seen(n, 0);
  Types::Bitset seenO1(n, 0);
  for (int i = 0; i < n; i++) {
    int roll = rand()%2;
    if (roll) {
      seen[i] = 1;
      seenO1[o1.get(i)] = 1;
      crossed.set(i, o1.get(i));
    }
  }
  int o2Idx = 0;
  for (int i = 0; i < n; i++) {
    if (!(seen[i])) {
      while(seenO1[o2.get(o2Idx)] != 0) {
        o2Idx++;
      }
      crossed.set(i, o2.get(o2Idx));
      seenO1[o2.get(o2Idx)] = 1;
    }
  }
  return crossed;
}

void Population::mutate(int NUM_MUTATIONS, int MUTATION_POWER, std::vector<SearchResult> &offspring, const Instance &instance) {
  assert(specimens.size() > 0);
  for (int i = 0; i < NUM_MUTATIONS; i++) {
    Ordering mutated = specimens[rand()%getSize()].getOrderingRef();
    DBG(mutated);
    mutated.perturb(MUTATION_POWER, instance);
    DBG(mutated);
    SearchResult climbed = localSearch.hillClimb(mutated);
    DBG("Mutated: " << climbed);
    offspring.push_back(climbed);
  }
}

void Population::filterBest(int n) {
  std::sort(specimens.begin(), specimens.end(), [](const SearchResult &a, const SearchResult &b) {
    return a.getScore() < b.getScore();
  });
  int i = 0;
  while (i + 1 < specimens.size() && getSize() > n) {
    Types::Score current = getSpecimen(i).getScore();
    if (getSpecimen(i+1).getScore() == current) {
      specimens.erase(specimens.begin() + i);
    } else {
      i++;
    }
  }
  while (getSize() > n) {
    specimens.pop_back();
  }
}

Types::Score Population::getAverageFitness() {
  int n = getSize();
  Types::Score sum = 0;
  for (int i = 0; i < n; i++) {
    sum += specimens[i].getScore();
  }
  return sum/n;
}

Ordering Population::crossoverCX(const Ordering &o1, const Ordering &o2) {
  int n = o1.getSize();
  std::vector<int> o1Inv(n);
  std::vector<int> o2Inv(n);
  for (int i = 0; i < n; i++) {
    o1Inv[o1.get(i)] = i;
    o2Inv[o2.get(i)] = i;
  }
  Ordering crossed(n);
  std::vector<int> notCrossed;
  for (int i = 0; i < n; i++) {
    if (o1.get(i) == o2.get(i)) {
      crossed.set(i, o1.get(i));
    } else {
      notCrossed.push_back(i);
    }
  }
  while (!notCrossed.empty()) {
    int numNotCrossed = notCrossed.size();
    int idx = notCrossed[rand()%numNotCrossed];
    int coinToss = rand()%2;
    Ordering p1(n);
    Ordering p2(n);
    std::vector<int> p1Inv, p2Inv;
    if (coinToss) {
      p1 = o1;
      p1Inv = o1Inv;
      p2 = o2;
      p2Inv = o2Inv;
    } else {
      p1 = o2;
      p1Inv = o2Inv;
      p2 = o1;
      p2Inv = o1Inv;
    }

    int initIdx = idx;
    //DBG("CYCLED " << crossed << "numrossed" << numNotCrossed);
    do  {
      //DBG(idx << " p1: " << p1.get(idx) << " p2: " << p2.get(idx) << " crossed: " << crossed);
      crossed.set(idx, p1.get(idx));
      notCrossed.erase(std::remove(notCrossed.begin(), notCrossed.end(), idx), notCrossed.end());
      int next = p1Inv[p2.get(idx)];
      idx = next;

    } while (idx != initIdx);
  }
  return crossed;
}



void Population::diversify(int numKeep, Instance &instance) {
  std::vector<SearchResult> diversified;
  int size = getSize();
  numKeep = numKeep <= size ? numKeep : size;
  assert(numKeep <= size);
  for (int i = 0; i < numKeep; i++) {
    diversified.push_back(specimens[i]);
  }
  for (int i = 0; i < size - numKeep; i++) {
    diversified.push_back(localSearch.hillClimb(Ordering::randomOrdering(instance)));
  }
  specimens = diversified;
}

void Population::append(const std::vector<SearchResult> &offspring) {
  specimens.insert(specimens.end(), offspring.begin(), offspring.end());
}

Ordering Population::crossoverRK(const Ordering &o1, const Ordering &o2) {
  std::map<int, std::vector<int>> rankMap;
  DBG("test");  
  int n = o1.getSize();
  std::vector<int> o1Inv(n);
  std::vector<int> o2Inv(n);
  for (int i = 0; i < n; i++) {
    o1Inv[o1.get(i)] = i;
    o2Inv[o2.get(i)] = i;
  }
  assert(o1.getSize() == o2.getSize());
  for (int i = 0 ; i < o1.getSize(); i++) {
    rankMap[o1Inv[i]+o2Inv[i]].push_back(i);
  }
  Ordering crossed(n);
  int cur = 0;
  for (auto iterator : rankMap) {
    std::vector<int> &currentBucket = iterator.second;
    if (currentBucket.size() > 1) {
      std::random_shuffle(currentBucket.begin(), currentBucket.end());
    }
    for (int i = 0; i < currentBucket.size(); i++) {
      crossed.set(cur, (currentBucket)[i]);
      cur++;
    }
  }
  return crossed;
}