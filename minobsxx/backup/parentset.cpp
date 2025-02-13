#include "parentset.h"
#include "debug.h"
ParentSet::ParentSet(Types::Score score, Types::Bitset parents, int var, int id, std::vector<int> parentsVec) :
  score(score), parents(parents), var(var), id(id), parentsVec(parentsVec) { }

bool ParentSet::hasElement(int k) const {
  return parents[k];
}

int ParentSet::getVar() const {
  return var;
}

std::ostream& operator<<(std::ostream &os, const ParentSet& p) {
  os << "Score : " << p.score << " ParentSet: " << p.parents << " Child: " << p.var << " Id: " << p.id;
  return os;
}

Types::Score ParentSet::getScore() const {
  return score;
}

Types::Bitset ParentSet::getParents() const {
  return parents;
}

bool ParentSet::subsetOf(const Types::Bitset &set) const {
  // for (int i = 0; i < parentsVec.size(); i++) {
  //   if (!set.test(parentsVec[i])) {
  //     return false;
  //   }
  // }
  // return true;

  return (set & parents) == parents;
}

bool ParentSet::supersetOf(const Types::Bitset &set) const {
  return (set & parents) == set;
}

void ParentSet::setId(const int &i) {
  id = i;
}

int ParentSet::getId() const {
  return id;
}

int ParentSet::size() const {
  return parentsVec.size();
}

const std::vector<int> &ParentSet::getParentsVec() const {
  return parentsVec;
}
