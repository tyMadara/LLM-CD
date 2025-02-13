#include "instance.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include "debug.h"
#include<boost/dynamic_bitset.hpp>

Instance::Instance(std::string fileName, std::string constraintsFileName) {
  const int SCORE_SCALE = -1000000;
  int countParents = 0, pruned = 0;
  std::ifstream file(fileName);
  std::ifstream constraintsFile(constraintsFileName);
  this->fileName = fileName;
  this->constraintFileName = constraintsFileName;

  // Assume the input file name is of the format {instance}_{dataSize}.BIC
  // Otherwise, hard code the value of dataSize here.

  int underscoreIdx = fileName.find('_'), underscore2Idx = fileName.find('_', underscoreIdx+1);

  if (underscoreIdx == std::string::npos || underscore2Idx == std::string::npos || underscoreIdx > underscore2Idx) {
    std::cout << "Error: the parent scores file is not of the format {instance}_{dataSize}.{scoreType}. Rename the file or modify instance.cpp." << std::endl;
    exit(0);
  }

  std::string sizeStr = fileName.substr(underscoreIdx + 1, underscore2Idx - underscoreIdx - 1);
  dataSize = stoi(sizeStr);
  std::cout << "Number of data points: " << dataSize << std::endl;

  if (file.is_open()) {
    file >> n;

    orderConstraints = std::vector<std::vector<bool>>(n);
    ancestralPairs = std::vector<std::vector<bool>>(n);

    for (int i = 0; i < n; i++) {
      orderConstraints[i].resize(n);
      ancestralPairs[i].resize(n);

      for (int j = 0; j < n; j++) {
        orderConstraints[i][j] = false;
        ancestralPairs[i][j] = false;
      }
    }

    // Read directed arc existence constraints:
    mustHaveParent.resize(n);
    constraintsFile >> m_dae;
    for (int i = 0; i < m_dae; i++) {
      int a, b;
      constraintsFile >> a >> b;
      mustHaveParent[b].push_back(a);
      deConstraints.push_back(std::make_pair(a,b));
      orderConstraints[a][b] = true;
    }


    // Read undirected arc existence constraints:
    constraintsFile >> m_uae;
    undirectedArcExistence = std::vector<std::vector<int>>(n);

    for (int i = 0; i < m_uae; i++) {
      int a, b;
      constraintsFile >> a >> b;
      undirectedArcExistence[a].push_back(b);
      undirectedArcExistence[b].push_back(a);
      ueConstraints.push_back(std::make_pair(a,b));
    }

    // Read arc absence constraints:
    constraintsFile >> m_aa;
    mustNotHaveParent.resize(n);
    for (int i = 0; i < m_aa; i++) {
      int a, b;
      constraintsFile >> a >> b;
      mustNotHaveParent[b].push_back(a);
      absConstraints.push_back(std::make_pair(a,b));
    }

    // Read ordering constraints:

    constraintsFile >> m_ord;
    for (int i = 0; i < m_ord; i++) {
      int a, b;
      constraintsFile >> a >> b;
      orderConstraints[a][b] = true;
      ordConstraints.push_back(std::make_pair(a,b));
    }

    // Read ancestral constraints:
    constraintsFile >> m_anc;
    
    ancestralConstraints.resize(m_anc);
    for (int i=0; i < m_anc; i++) {
      int a, b;
      constraintsFile >> a >> b;
      ancestralConstraints[i] = std::make_pair(a, b);
      ancestralPairs[a][b] = true;
      orderConstraints[a][b] = true; // We can infer that a < b from the ancestral constraint.
    }


    // Read ancestral absence constraints:
    constraintsFile >> m_ancab;

    ancestralConstraints.resize(m_ancab+m_anc);
    for (int i=m_anc; i < m_anc + m_ancab; i++) {
      int a, b;
      constraintsFile >> a >> b;
      ancestralConstraints[i] = std::make_pair(a, b);
    }

    // bty

    // Bellman-Ford-esque algorithm to generate all possible inferred ordering constraints.
    for (int iters = 0; iters < n; iters++) {
      bool improvement = false;

      for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
          if (orderConstraints[i][j]) {
            for (int k = j+1; k < n; k++) {
              if (orderConstraints[j][k]) {
                if (! orderConstraints[i][k]) {
                  improvement = true;
                  orderConstraints[i][k] = true;
                } 
              }
            }
          }
        }
      }

      if (!improvement) break;
    }

    // Read parent set scores.
    vars.resize(n);
    for (int i = 0; i < n; i++) {
      DBG("Working on var: " << i);
      int varId;
      int numParents;
      file >> varId >> numParents;
      Variable v(numParents, varId);

      for (int j = 0; j < numParents; j++) {
        double doubleScore;
        int parentSize;
        Types::Bitset set(n, 0);
        std::vector<int> parentsVec;
        file >> doubleScore >> parentSize;
        Types::Score score = (Types::Score)(doubleScore * SCORE_SCALE);

        for (int k = 0; k < parentSize; k++) {
          int parentVar;
          file >> parentVar;
          assert(parentVar < n);
          set[parentVar] = 1;
          parentsVec.push_back(parentVar);
        }

        ParentSet parentSet(score, set, varId, j, parentsVec);
        v.addParentSet(parentSet);
      }

      countParents += numParents;

      v.setNumParents(numParents);
      v.parentSort();
      v.resetParentIds();
      vars[varId] = v;
    }
  } else {
    throw "Could not open file";
  }

  std::cout << countParents << std::endl;

  pruneFactor = initialPruneFactor();
  pruned += pruneParentSetsLossless();


  // Make a hard copy of all the parent sets at this point.
  for (int i = 0; i < n; i++) {
    vars[i].hardCopyParents();
  }


  pruned += pruneParentSetsHeuristic();

  // Initialize the allParentSets array
  for (int i = 0; i < n; i++) {
    const Variable &v = getVar(i);
    for (int j=0; j < v.numParents(); j++) {
      allParentSets.push_back(std::make_pair(i, j));  
    }
  }

  std::cout << "Number of candidate parents: " << allParentSets.size() << std::endl;
  std::cout << "Pruned parent sets: " << pruned << std::endl;
  std::cout << "Number of ancestral constraints: " << m_anc << std::endl;
  std::cout << "Pruning factor: " << pruneFactor << std::endl;
  sortAllParents();
}

int Instance::pruneParentSetsLossless() {
  int pruned = 0;

  for (int i = 0; i < n; i++) {
    Variable &var = getVar(i);
    std::vector<ParentSet> validParents;

    for (int j = 0; j < var.numParents(); j++) {
      if (!canPruneParentLossless(i, j)) {
        validParents.push_back(var.getParent(j));
      } else {
        pruned++;
      }
    }
 
    var.clearParentSets();

    for (int l = 0; l < validParents.size(); l++) {
      var.addParentSet(validParents[l]);
    }
 
    var.setNumParents(validParents.size());
    //var.parentSort();
    var.resetParentIds();
  
  return pruned;
  }

  return pruned;
}


bool Instance::canPruneParentLossless(int node, int j) {
  // Pruning rules-------------
  // 1) If X < Y, P is a parent set of X containing Y, P can be eliminated.
  // 2) If X--->Y, P is a parent set of Y and each W in P precedes X in every ordering, P can be eliminated.
  // 3) If X -> Y, P is a parent set of Y not containing X, P can be eliminated.
  // 4) If X -/> Y, P is a parent set of Y containing X, P can be eliminated.

  const Variable &var = getVar(node);
  const ParentSet &ps = var.getParent(j);
  const std::vector<int> &parents = ps.getParentsVec();

  // Rule 1
  for (int i = 0; i < parents.size(); i++) {
    if (orderConstraints[node][parents[i]]) {
      return true;
    }
  }


  // Rule 2
  for (int x = 0; x < n; x++) {
    if (ancestralPairs[x][node]) {
      bool allPrecedes = true;

      for (int i = 0; i < parents.size(); i++) {
        if (!orderConstraints[parents[i]][x]) {
          allPrecedes = false;
          break;
        }
      }

      if (allPrecedes) {
        return true;
      }
    }
  }

  // Rule 3

  for (int i = 0; i < mustHaveParent[node].size(); i++) {
    if (!ps.hasElement(mustHaveParent[node][i])) {
      return true;
    }
  }

  // Rule 4

  for (int i = 0; i < mustNotHaveParent[node].size(); i++) {
    if (ps.hasElement(mustNotHaveParent[node][i])) {
      return true;
    }
  }

  return false;
}


int Instance::pruneParentSetsHeuristic() {
  int pruned = 0;
  for (int i = 0; i < n; i++) {
    Variable &var = getVar(i);
    std::vector<ParentSet> validParents;

    for (int j = 0; j < var.numOriginalParents(); j++) {
      if (!canPruneParentHeuristic(i, j)) {
        validParents.push_back(var.getOriginalParent(j));
      } else {
        pruned++;
      }
    }

    var.clearParentSets();

    for (int l = 0; l < validParents.size(); l++) {
      var.addParentSet(validParents[l]);
    }

    var.setNumParents(validParents.size());
    //var.parentSort(); (Don't need this?)
    var.resetParentIds();
  }
  return pruned;
}


double Instance::initialPruneFactor() const {
  double omegaFactor = (double) 50 * n / dataSize;
  double constraintDensity = (double)m_anc / (n * (n-1));

  return 1 + omegaFactor * (1 - (1-constraintDensity)*(1-constraintDensity));
}


bool Instance::canPruneParentHeuristic(int node, int j) {
  // Pruning rules-------------
  // If P \subset P' are parent sets for Y and 2*sc(P) < sc(P'), prune P'. This is a heuristic.

  const Variable &var = getVar(node);
  const ParentSet &ps = var.getOriginalParent(j);
  const std::vector<int> &parents = ps.getParentsVec();

  // Rule 3
  Types::Bitset pred(n, 0);
  for (int i = 0; i < parents.size(); i++) {
    pred[parents[i]] = 1;
  }

  for (int i = 0; i < var.numParents(); i++) {
    const ParentSet &other = var.getOriginalParent(i);

    if (i != j && pruneFactor * other.getScore() < ps.getScore() && other.subsetOf(pred)) {
      return true;
    }
  }

  return false;
}


void Instance::sortAllParents() {
  std::sort(allParentSets.begin(), allParentSets.end(), [&](const std::pair<int,int> a, const std::pair<int,int> b) -> bool {
      const Variable &aVar = getVar(a.first), &bVar = getVar(b.first);
      return (aVar.getParent(a.second).getScore() < bVar.getParent(b.second).getScore()
        || (aVar.getParent(a.second).getScore() == bVar.getParent(b.second).getScore() && 
                aVar.getParent(a.second).size() < bVar.getParent(b.second).size()));
    });
}

// Return the number of parent sets pruned by the heuristic
int Instance::restartWithLessPrune(int multiplier) {
  // Too much pruning was done last time. 
  // Increase the pruning factor, and re-try.

  pruneFactor = (pruneFactor - 1) * multiplier + 1;
  
  int pruned = pruneParentSetsHeuristic();

  allParentSets.clear();

  for (int i = 0; i < n; i++) {
    const Variable &v = getVar(i);
    for (int j=0; j < v.numParents(); j++) {
      allParentSets.push_back(std::make_pair(i, j));  
    }
  }

  std::cout << "Pruning factor: " << pruneFactor << std::endl;
  sortAllParents();
  return pruned;
}

std::vector< std::pair<int, int> > &Instance::getParentList() {
  return allParentSets;
}

int Instance::getN() const {
  return n;
}

int Instance::getM_anc() const {
  return m_anc;
}

int Instance::getM_dae() const {
  return m_dae;
}

int Instance::getM_uae() const {
  return m_uae;
}
//
int Instance::getM_ancab() const {
  return m_ancab;
}
// bty
int Instance::getM_aa() const {
  return m_aa;
}

int Instance::getM_ord() const {
  return m_ord;
}

std::string Instance::getFileName() const {
  return fileName;
}

std::string Instance::getConstraintFileName() const {
  return constraintFileName;
}

bool Instance::isConstraint(int a, int b) const {
  return (orderConstraints[a][b]);
}

Variable &Instance::getVar(int i) {
  return vars[i];
}

const Ancestral &Instance::getAncestral(int i) const {
  return ancestralConstraints[i];
}
//
const Ancestral &Instance::getAncestralAbsence(int i) const {
  return ancestralAbsenceConstraints[i];
}
// bty
const std::vector<int> &Instance::getUndirectedExistence(int i) const {
  return undirectedArcExistence[i];
}

const std::vector<std::vector<bool>> &Instance::getOrderConstraints() const {
  return orderConstraints;
}

bool Instance::hasUndirectedForNode(int i) const {
  return (undirectedArcExistence[i].size() > 0);
}

bool Instance::hasUndirectedExistence() const {
  return (m_uae > 0);
}

std::ostream& operator<<(std::ostream &os, const Instance& I) {
  os << "Printing Instance: " << std::endl;
  for (int i = 0; i < I.n; i++) {
    os << I.vars[i] << std::endl;
  }
  return os;
}
