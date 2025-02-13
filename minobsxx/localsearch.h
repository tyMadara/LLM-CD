#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H 

#include <boost/dynamic_bitset.hpp>
#include "instance.h"
#include "ordering.h"
#include "pivotresult.h"
#include "searchresult.h"
#include "population.h"
#include "resultregister.h"
#include "swapresult.h"
#include "fastpivotresult.h"
#include "types.h"

enum class Neighbours {
  SWAP,
  INSERT
};

enum class CrossoverType {
  CX,
  OB,
  RK
};

enum class SelectType {
  FIRSTV1,
  FIRSTV2,
  BEST,
  HYBRID,
  OLDHYBRID
};

const Types::Score INF = 223372036854775807LL;
const Types::Score PENALTY = 100000000000000LL;
const int tabuTenure = 3;


class LocalSearch {
  public:
    LocalSearch(Instance &instance, ResultRegister &rr);
    ~LocalSearch();
    const ParentSet &bestParent(const Ordering &ordering, const std::vector<int> & positions, const Types::Bitset &pred, int idx);
    const ParentSet &bestParentVar(const Types::Bitset &pred, const Variable &v, const std::vector<int> & positions);
    bool consistentWithUndirected(const ParentSet &p, const std::vector<int> &positions);

    Types::Score getBestScoreWithParents(const Ordering &ordering, std::vector<int> &parents, std::vector<Types::Score> &scores);

    int numConstraintsSatisfied(const std::vector<int> &parents);
    int numConstraintsSatisfied(const std::vector<int> &parents, const std::vector<int> &positions);
    int numConstraintsSatisfied(const std::vector<int> &newParents, bool **ancestor, bool **descendant, bool *satisfied, int cur, const std::vector<int> &positions);
    bool improving(const std::vector<int> &newParents, bool **ancestor, bool **descendant, bool *satisfied, int cur, const std::vector<int> &positions, int curNumSat, int oldPar, int &numSat);
    bool hasDipath(const std::vector<int> &parents, int x, int y);
    bool hasDipathWithOrdering(const std::vector<int> &parents, int x, int y, const std::vector<int> &positions);
    void alloc_2d(bool **&ancestor, bool **&descendant, bool *&satisfied);
    void dealloc_2d(bool **&ancestor, bool **&descendant, bool *&satisfied);
    void computeAncestralGraph(const std::vector<int> &parents, bool **ancestor, bool **descendant, bool *satisfied,  const std::vector<int> &positions);

    Types::Score modifiedDAGScoreWithParents(const Ordering &ordering, std::vector<int> &parents, std::vector<Types::Score> &scores);

    void bestSwapBackward(int pivot, Ordering o, const std::vector<int> &parents, Ordering &bestOrdering, std::vector<int> &bestParents, Types::Score &bestSc);
    void bestSwapForward(int pivot, Ordering o, const std::vector<int> &parents, Ordering &bestOrdering, std::vector<int> &bestParents, Types::Score &bestSc);
    SearchResult hillClimb(const Ordering &ordering);
    void tunePruningFactor();
    SearchResult genetic(int cutoffGenerations, int INIT_POPULATION_SIZE, int NUM_CROSSOVERS, int NUM_MUTATIONS, int MUTATION_POWER, int DIV_LOOKAHEAD, int NUM_KEEP, float DIV_TOLERANCE, CrossoverType crossoverType, int greediness, Types::Score opt, ResultRegister &rr);
    void checkSolution();
    bool consistentWithAncestral(const Ordering &ordering);
    std::pair<int, int> constraintRange(const Ordering &ordering);
    bool consistentWithOrdering(const Ordering &o, const std::vector<int> &parents);
    bool allConstraintsSatisfied(const std::vector<int> &parents, const Ordering &o);
    void printModelString(const std::vector<int> &parents, bool valid, Types::Score score);
    
  private:
    Instance &instance;
    ResultRegister &rr;
    static int climbs;
    bool **ancestor, **descendant, *satisfied;

    std::vector<int> globalOptimum;
    Types::Score optimalScore = INF;
    Ordering optimalOrdering;
    std::vector<Types::Score> optimalScores;

    double walkProb = 0.075;
    double transposeProb = 0;

    bool noValidParentFoundFlag = false;
};

#endif /* LOCALSEARCH_H */
  