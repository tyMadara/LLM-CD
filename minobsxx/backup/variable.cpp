#include "variable.h"
#include "debug.h"
Variable::Variable(int numParents, int varId) :
  parentsWithVar(), nParents(numParents),  varId(varId) { }

Variable::Variable() { };

void Variable::addParentSet(ParentSet parentSet) {
  parents.push_back(parentSet);
}

void Variable::clearParentSets() {
  parents.clear();
  nParents = 0;
}

const ParentSet &Variable::getParent(int i) const {
  return parents[i];
}

const ParentSet &Variable::getOriginalParent(int i) const {
  return originalParents[i];
}

void Variable::parentSort() {
  std::sort(parents.begin(), parents.end(), [](ParentSet a, ParentSet b) {
    return a.getScore() < b.getScore() || (a.getScore() == b.getScore() && a.size() < b.size());
  });
}

int Variable::numParents() const {
  return nParents;
}

int Variable::numOriginalParents() const {
  return originalParents.size();
}

void Variable::setNumParents(int n) {
  nParents = n;
}

std::ostream& operator<<(std::ostream &os, const Variable& v) {
  os << "Parents of Variable " << v.varId << ":" << std::endl;
  for (int i = 0; i < v.nParents; i++) {
    os << "Id: " << i << " " << v.parents[i] << std::endl;
  }
  return os;
}

void Variable::resetParentIds() {
  assert(nParents == parents.size());
  for (int j = 0; j < nParents; j++) {
    parents[j].setId(j);
  }
}

int Variable::getId() const {
  return varId;
}

void Variable::hardCopyParents() {

  for (int i = 0; i < parents.size(); i++) {
    const ParentSet &ps = parents[i];
    originalParents.push_back(ParentSet(ps));
  }
}