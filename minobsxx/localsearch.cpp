#include "localsearch.h"
#include "debug.h"
#include <numeric>
#include <utility>
#include <string>
#include <deque>
#include <fstream>
#include <stack>
#include <chrono>
#include <cmath>
#include "tabulist.h"
#include "util.h"
#include "movetabulist.h"
#include "swaptabulist.h"

LocalSearch::LocalSearch(Instance &instance, ResultRegister &rr) : instance(instance), rr(rr) { 
  alloc_2d(ancestor, descendant, satisfied);
}

LocalSearch::~LocalSearch() {
  dealloc_2d(ancestor, descendant, satisfied);
}

int LocalSearch::climbs = 0;

const ParentSet &LocalSearch::bestParent(const Ordering &ordering, const std::vector<int> & positions, const Types::Bitset &pred, int idx) {
  int current = ordering.get(idx);
  const Variable &v = instance.getVar(current);
  return bestParentVar(pred, v, positions);
}

const ParentSet &LocalSearch::bestParentVar(const Types::Bitset &pred, const Variable &v, const std::vector<int> & positions) {
  int numParents = v.numParents();
  for (int i = 0; i < numParents; i++) {
    const ParentSet &p = v.getParent(i);
    if (p.subsetOf(pred) && (consistentWithUndirected(p, positions))) {
      return p;
    }
  }

  noValidParentFoundFlag = true;

  return v.getParent(0); //Should never happen in THeory
}

bool LocalSearch::consistentWithUndirected(const ParentSet &p, const std::vector<int> &positions) {
  if (!instance.hasUndirectedForNode(p.getVar())) {
    return true;
  }

  const std::vector<int> &undirectedConstraints = instance.getUndirectedExistence(p.getVar());
  for (int i = 0; i < undirectedConstraints.size(); i++) {
    if (positions[undirectedConstraints[i]] < positions[p.getVar()]) {
      if (!p.hasElement(undirectedConstraints[i])) {
        return false;
      }
    }
  }

  return true;
}

bool LocalSearch::consistentWithAncestral(const Ordering &ordering) {
  int n = instance.getN(), m_anc = instance.getM_anc();
  int pos[n];
  for (int i=0; i < n; i++) {
    pos[ordering.get(i)] = i;
  }

  for (int i=0; i < m_anc; i++) {
    int x = instance.getAncestral(i).first, y = instance.getAncestral(i).second;
    if (pos[x] > pos[y]) {
      return false;
    }
  }

  return true;
}

// New code
Types::Score LocalSearch::getBestScoreWithParents(const Ordering &ordering, std::vector<int> &parents, std::vector<Types::Score> &scores) {
  int n = instance.getN(), m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab();
  
  if (!consistentWithAncestral(ordering)) {
    return INF;
  }
  
  Types::Bitset pred(n, 0);
  Types::Score score = 0;

  std::vector<int> positions;

  if (instance.hasUndirectedExistence()) {
    positions = std::vector<int>(n);
    for (int i = 0; i < n; i++) {
      positions[ordering.get(i)] = i;
    }
  }

  for (int i = 0; i < n; i++) {
    const ParentSet &p = bestParent(ordering, positions, pred, i);

    if (noValidParentFoundFlag) {
      noValidParentFoundFlag = false;
      return INF;
    }
    parents[ordering.get(i)] = p.getId();
    scores[ordering.get(i)] = p.getScore();
    score += p.getScore();
    pred[ordering.get(i)] = 1;
  }


  if (m_anc == 0 && m_ancab == 0) {
    if (score < optimalScore) {
      optimalScore = score;
      optimalOrdering = ordering;
      globalOptimum = parents;
      optimalScores = scores;
    }

    return score;
  } else {

    return modifiedDAGScoreWithParents(ordering, parents, scores);

  }
}

void LocalSearch::alloc_2d(bool **&ancestor, bool **&descendant, bool *&satisfied) {
  int n = instance.getN(), m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab();
  ancestor = new bool* [n];
  descendant = new bool* [n];
  satisfied = new bool[m_anc+m_ancab];

  for (int i = 0; i < n; i++) {
    ancestor[i] = new bool[m_anc+m_ancab];
    descendant[i] = new bool[m_anc+m_ancab];
  }
}


void LocalSearch::dealloc_2d(bool **&ancestor, bool **&descendant, bool *&satisfied) {
  int n = instance.getN(), m_anc = instance.getM_anc();

  for (int i = 0; i < n; i++) {
    delete[] ancestor[i];
    delete[] descendant[i];
  }

  delete[] ancestor;
  delete[] descendant;
  delete[] satisfied;
}

bool LocalSearch::hasDipath(const std::vector<int> &parents, int x, int y) {
  if (x == y) {
    return true;
  }

  const Variable &yVar = instance.getVar(y);
  const ParentSet &p = yVar.getParent(parents[y]);
  const std::vector<int> &pars = p.getParentsVec();

  for (int i=0; i < pars.size(); i++) {
    if (hasDipath(parents, x, pars[i])) {
      return true;
    }
  }

  return false;
}


bool LocalSearch::hasDipathWithOrdering(const std::vector<int> &parents, int x, int y, const std::vector<int> &positions) {
  if (x == y) {
    return true;
  }

  if (positions[y] < positions[x]) {
    return false;
  }

  const Variable &yVar = instance.getVar(y);
  const ParentSet &p = yVar.getParent(parents[y]);
  const std::vector<int> &pars = p.getParentsVec();

  for (int i=0; i < pars.size(); i++) {
    if (hasDipathWithOrdering(parents, x, pars[i], positions)) {
      return true;
    }
  }

  return false;
}

int LocalSearch::numConstraintsSatisfied(const std::vector<int> &parents) {
  int n = instance.getN(), m_anc = instance.getM_anc(), count = 0;

  for (int i=0; i < m_anc; i++) {
    const Ancestral &cons = instance.getAncestral(i);
    if (hasDipath(parents, cons.first, cons.second)) {
      count++;
    }
  }

  return count;
}

int LocalSearch::numConstraintsSatisfied(const std::vector<int> &parents, const std::vector<int> &positions) {
  int n = instance.getN(), m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab(), count = 0;

  for (int i=0; i < m_anc; i++) {
    const Ancestral &cons = instance.getAncestral(i);
    if (hasDipathWithOrdering(parents, cons.first, cons.second, positions)) {
      count++;
    }
  }

  for (int i=m_anc; i < m_anc + m_ancab; i++) {
    const Ancestral &cons = instance.getAncestral(i);
    if (!hasDipathWithOrdering(parents, cons.first, cons.second, positions)) {
      count++;
    }
  }

  return count;
}

// bty mod
int LocalSearch::numConstraintsSatisfied(const std::vector<int> &newParents, bool **ancestor, bool **descendant, bool *satisfied, int cur, const std::vector<int> &positions) {
  int n = instance.getN(), m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab(), count = 0, par = newParents[cur];

  const Variable &var = instance.getVar(cur);
  const ParentSet &p = var.getParent(par); 

  for (int i = 0; i < m_anc+m_ancab; i++) {

    // Check if a connected ancestral is now unconnected
    if ((i<m_anc && satisfied[i]) || (i>=m_anc && !satisfied[i])) {
      if (!(positions[instance.getAncestral(i).first] <= positions[cur] && positions[cur] <= positions[instance.getAncestral(i).second])) {
        if (i<m_anc)
          count++;
      } else if (ancestor[cur][i] && descendant[cur][i]) {
        // Maybe do a check here later for if cur has a parent who has X_i has an ancestor
        const std::vector<int> &curPars = p.getParentsVec();
        bool connected = false;
        for (int j = 0; j < curPars.size(); j++) {
          if (ancestor[curPars[j]][i]) {
            connected = true;
            break;
          }
        }

        if (connected) {
          if (i < m_anc)
            count++;
        } else if (hasDipathWithOrdering(newParents, instance.getAncestral(i).first, instance.getAncestral(i).second, positions)) {
          if (i < m_anc)
            count++;
        }
        else
        {
          if (i>= m_anc)
            count++;
        }
      } else {
        if (i < m_anc)
          count++;
      }
    } 

    // Check if an unconnected ancestral is now connected.
    else {
      if (positions[instance.getAncestral(i).first] <= positions[cur] && positions[cur] <= positions[instance.getAncestral(i).second] && descendant[cur][i]) {
        const std::vector<int> &curPars = p.getParentsVec();
        bool connected = false;
        for (int j = 0; j < curPars.size(); j++) {
          if (ancestor[curPars[j]][i]) {
            connected = true;
            break;
          }
        }

        if (connected) {
          if (i<m_anc)
            count++;
        }
        else
        {
          if (i>=m_anc)
            count++;
        }
      }
      else
      {
        if (i>=m_anc)
          count++;
      }
    }
  }
  
  return count;
}

// bty mod
bool LocalSearch::improving(const std::vector<int> &newParents, bool **ancestor, bool **descendant, bool *satisfied, int cur, const std::vector<int> &positions, int curNumSat, int oldPar, int &numSat) {
 
  int n = instance.getN(), m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab(), count = 0, par = newParents[cur];

  const Variable &var = instance.getVar(cur);
  const ParentSet &p = var.getParent(par); 
  const ParentSet &oldP = var.getParent(oldPar); 

  for (int i = 0; i < m_anc; i++) {

    // Check if an unsatisfied anc existence constraint is now satisfied.
    if (!satisfied[i]) {
      if (positions[instance.getAncestral(i).first] <= positions[cur] && positions[cur] <= positions[instance.getAncestral(i).second] && descendant[cur][i]) {
        const std::vector<int> &curPars = p.getParentsVec();
        bool connected = false;
        for (int j = 0; j < curPars.size(); j++) {
          if (ancestor[curPars[j]][i]) {
            connected = true;
            break;
          }
        }

        if (connected) {
          count++;
        }
      }
    } else {
      count ++;
    }
  }

  for (int i = m_anc; i < m_anc + m_ancab; i++) {

    // Check if an unsatisfied anc absence constraint is now satisfied.
  
    if (!satisfied[i]) {
        if ( ancestor[cur][i] && descendant[cur][i] && !hasDipathWithOrdering(newParents, instance.getAncestral(i).first, instance.getAncestral(i).second, positions)) {
        count++;
      }
      
    } else {
      count ++;
    }
  }

  // Count now is a theoretical upper bound on the number of satisfied constraints.

  for (int i = 0; i < m_anc; i++) {
    // Check if a satisfied constraint is now unsatisfied

    if (count < curNumSat ||
      (count == curNumSat && p.getScore() > oldP.getScore()) || 
      (count == curNumSat && p.getScore() == oldP.getScore() && p.size() >= oldP.size())) {
      numSat = 0;
      return false;
    }


    if (satisfied[i]) {
      if (ancestor[cur][i] && descendant[cur][i]) {

        if (!(hasDipathWithOrdering(newParents, instance.getAncestral(i).first, instance.getAncestral(i).second, positions))) {
          count--;
        }
      }
    } 

    
  }

  for (int i = m_anc; i < m_anc+m_ancab; i++) {
    // Check if a satisfied absence constraint is now unsatisfied

    if (satisfied[i]) {
        if (positions[instance.getAncestral(i).first] <= positions[cur] && positions[cur] <= positions[instance.getAncestral(i).second] && descendant[cur][i]) {
          const std::vector<int> &curPars = p.getParentsVec();
          bool connected = false;
          for (int j = 0; j < curPars.size(); j++) {
            if (ancestor[curPars[j]][i]) {
              connected = true;
              break;
            }
          }

          if (connected) {
            count--;
          }
        }
    }  
  }
  
  numSat = count;

  return (count > curNumSat) ||
      (count == curNumSat && p.getScore() < oldP.getScore()) ||
      (count == curNumSat && p.getScore() == oldP.getScore() && p.size() < oldP.size());

}

// bty mod
void LocalSearch::computeAncestralGraph(const std::vector<int> &parents, bool **ancestor, bool **descendant, bool *satisfied, const std::vector<int> &positions) {

  int n = instance.getN(), m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab();
  
  for (int j = 0; j < m_anc + m_ancab; j++) {
    if (j < m_anc)
    satisfied[j] = false;
    else
    satisfied[j] = true;
  }
  
  for (int i = 0; i < n; i++) {
  
    for (int j = 0; j < m_anc + m_ancab; j++) {
 
      ancestor[i][j] = hasDipathWithOrdering(parents, instance.getAncestral(j).first, i, positions);
    
      descendant[i][j] = hasDipathWithOrdering(parents, i, instance.getAncestral(j).second, positions);
   
      if (ancestor[i][j] && descendant[i][j]) {
        if (j < m_anc)
        satisfied[j] = true;
        else
        satisfied[j] = false;
      }
    }
  }
}

// GREEDY HILL-CLIMBING METHOD (First Improvement)
Types::Score LocalSearch::modifiedDAGScoreWithParents(const Ordering &ordering, std::vector<int> &parents, std::vector<Types::Score> &scores) {
  int n = instance.getN(), m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab();
  std::vector<int> bestGraph(parents);

  std::vector<int> positions(n);

  for (int i = 0; i < n; i++) {
    positions[ordering.get(i)] = i;
  }

  computeAncestralGraph(bestGraph, ancestor, descendant, satisfied, positions);

  int curNumSat = 0;

  for (int i = 0; i < m_anc+m_ancab; i++) {
    curNumSat += (satisfied[i]);
  }

  std::vector<Types::Bitset> pred(n);
  Types::Bitset curPred(n, 0);

  for (int i = 0; i < n; i++) {
    pred[ordering.get(i)] = curPred;
    curPred[ordering.get(i)] = 1;
  }

  int lastRandomWalk[n];

  for (int i = 0; i < n; i++) {
    lastRandomWalk[i] = -tabuTenure-1;
  }

  int iters = 0;
  while(true) {

    bool foundImproving = false;

    for (int i = 0; i < instance.getParentList().size(); i++) {
      int cur = instance.getParentList()[i].first, par = instance.getParentList()[i].second;
      int oldPar = bestGraph[cur];
      const Variable &var = instance.getVar(cur);
      const ParentSet &p = var.getParent(par);
      const ParentSet &oldP = var.getParent(oldPar);
      Types::Score sc = p.getScore();
       
      //
      if (curNumSat == m_anc + m_ancab && sc > oldP.getScore()) {
        continue;
      }
      // bty

      // std::cout << p  <<std::endl;
      // for(int i = 0; i < p.getParentsVec().size(); i++)
      // {
      //   std::cout<< p.getParentsVec()[i] << " ";
      // }
      // std::cout <<  std::endl;
      if (par != bestGraph[cur] && (iters - lastRandomWalk[cur] > tabuTenure) && p.subsetOf(pred[cur]) && consistentWithUndirected(p, positions)) {

        bestGraph[cur] = par;
        int numSat;
        
        bool improv = improving(bestGraph, ancestor, descendant, satisfied, cur, positions, curNumSat, oldPar, numSat);
    
        bestGraph[cur] = oldPar;

        if (improv) {
          
          bestGraph[cur] = par;
           
          curNumSat = numSat;
          foundImproving = true;
          break;
        }
      }
    }

    if (!foundImproving) {

      // Take a random walk.
      if ((double)rand() / RAND_MAX < walkProb) {

        Types::Score finalSc = 0LL;
        for (int i=0; i<n; i++) {
            const Variable &v = instance.getVar(i);
            scores[i] = v.getParent(bestGraph[i]).getScore();
            finalSc += scores[i];
        }

        // Add a penalty for each broken constraint.
        finalSc += PENALTY * (m_anc + m_ancab - curNumSat);

        if (finalSc < optimalScore) {
          optimalScore = finalSc;
          optimalScores = scores;
          optimalOrdering = ordering;
          globalOptimum = bestGraph;
        }

        while (true) {
          int i = rand() % instance.getParentList().size();

          int cur = instance.getParentList()[i].first, par = instance.getParentList()[i].second;
          const Variable &var = instance.getVar(cur);
          const ParentSet &p = var.getParent(par);

          if (p.subsetOf(pred[cur]) && consistentWithUndirected(p, positions)) {
            lastRandomWalk[cur] = iters;
            bestGraph[cur] = par;
            curNumSat = numConstraintsSatisfied(bestGraph, ancestor, descendant, satisfied, cur, positions);
            foundImproving = true;
            break;
          }
        }
      } else {
        break;
      } 
    }

    computeAncestralGraph(bestGraph, ancestor, descendant, satisfied, positions);

    iters++;
  }


  Types::Score finalSc = 0LL;
  for (int i=0; i<n; i++) {
      const Variable &v = instance.getVar(i);
      scores[i] = v.getParent(bestGraph[i]).getScore();
      finalSc += scores[i];
  }

  parents = bestGraph;

  // Add a penalty for each broken constraint.
  finalSc += PENALTY * (m_anc + m_ancab - curNumSat);

  if (finalSc < optimalScore) {
    optimalScore = finalSc;
    optimalScores = scores;
    optimalOrdering = ordering;
    globalOptimum = bestGraph;
  }

  return finalSc;
}

void LocalSearch::bestSwapForward(
  int pivot,
  Ordering o,
  const std::vector<int> &parents,
  Ordering &bestOrdering,
  std::vector<int> &bestParents, 
  Types::Score &bestSc
)
{
  int n = instance.getN();
  std::vector<int> tmpParents = parents;
  Types::Bitset pred(n, 0);

  for (int i = 0; i < pivot; i++) {
    pred[o.get(i)] = 1;
  }

  std::vector<int> positions;

  if (instance.hasUndirectedExistence()) {
    positions = std::vector<int>(n);
    for (int i = 0; i < n; i++) {
      positions[o.get(i)] = i;
    }
  }

  for (int j = pivot; j < n-1; j++) {
    // First check that the swap results in a valid ordering.
    // It is valid iff O[j] -> O[j+1] is NOT an ancestral constraint.
    // Furthermore, if O[j] -> O[j+1] IS an ancestral constraint all further forward swaps are invalid.

    if (instance.isConstraint(o.get(j), o.get(j+1))) {
      break;
    }

    pred[o.get(j+1)] = 1;

    // Check if O_{j+1} still has a feasible parent set (feasible iff it does not contain O_j)
    const Variable &v = instance.getVar(o.get(j+1));
    const ParentSet &ps = v.getParent(tmpParents[o.get(j+1)]);

    
    if (ps.hasElement(o.get(j))) {
      // Replace the current parent set of O_{j+1} with some valid replacement.
      // Note that here pred[o.get(j)] = 0.
      int pid = bestParentVar(pred, v, positions).getId();
      if (noValidParentFoundFlag) {
        noValidParentFoundFlag = false;
        break;
      }
      tmpParents[o.get(j+1)] = pid;
    }

    o.swap(j, j+1);

    std::vector<Types::Score> scores(n); // We don't actually use this.
    Types::Score sc = modifiedDAGScoreWithParents(o, tmpParents, scores);
    if (sc < bestSc) {
      bestSc = sc;
      bestParents = tmpParents;
      bestOrdering = o;
    }
  }
}

void LocalSearch::bestSwapBackward(
  int pivot,
  Ordering o,
  const std::vector<int> &parents,
  Ordering &bestOrdering,
  std::vector<int> &bestParents, 
  Types::Score &bestSc
)
{
  int n = instance.getN();
  std::vector<int> tmpParents = parents;

  Types::Bitset pred(n, 0);

  for (int i = 0; i < pivot; i++) {
    pred[o.get(i)] = 1;
  }

  std::vector<int> positions;

  if (instance.hasUndirectedExistence()) {
    positions = std::vector<int>(n);
    for (int i = 0; i < n; i++) {
      positions[o.get(i)] = i;
    }
  }

  for (int j = pivot-1; j >= 0; j--) {
    // First check that the swap results in a valid ordering.
    // It is valid iff O[j] -> O[j+1] is NOT an ancestral constraint.
    // Furthermore, if O[j] -> O[j+1] IS an ancestral constraint all further forward swaps are invalid.


    if (instance.isConstraint(o.get(j), o.get(j+1))) {
      break;
    }


    pred[o.get(j)] = 0;

    // Check if O_{j+1} still has a feasible parent set (feasible iff it does not contain O_j)
    const Variable &v = instance.getVar(o.get(j+1));
    const ParentSet &ps = v.getParent(tmpParents[o.get(j+1)]);

    if (ps.hasElement(o.get(j))) {
      // Replace the current parent set of O_{j+1} with some valid replacement.
      // Note that here pred[o.get(j)] = 0.

      int pid = bestParentVar(pred, v, positions).getId();
      if (noValidParentFoundFlag) {
        noValidParentFoundFlag = false;
        break;
      }
      tmpParents[o.get(j+1)] = pid;

    }

    o.swap(j, j+1);

    pred[o.get(j)] = 0;

    std::vector<Types::Score> scores(n); // We don't actually use this.
    Types::Score sc = modifiedDAGScoreWithParents(o, tmpParents, scores);

    if (sc < bestSc) {
      bestSc = sc;
      bestParents = tmpParents;
      bestOrdering = o;
    }
  }
}

SearchResult LocalSearch::hillClimb(const Ordering &ordering) {
  bool improving = false;
  int n = instance.getN();
  std::vector<int> parents(n), initialDAG(n);
  std::vector<Types::Score> scores(n);
  int steps = 0;
  std::vector<int> positions(n);
  Ordering cur(ordering);

  Types::Score curScore = getBestScoreWithParents(cur, parents, scores);

  initialDAG = parents;

  if (curScore == INF) {
    return SearchResult(curScore, cur);
  }



  std::iota(positions.begin(), positions.end(), 0);

  DBG("Inits: " << cur);
  do {
    improving = false;
    std::random_shuffle(positions.begin(), positions.end());
    for (int s = 0; s < n && !improving; s++) {
      int pivot = positions[s];

      std::vector<int> bestDAG(n);
      Ordering bestOrdering;
      Types::Score bestSc = INF;
      bestSwapForward(pivot, cur, initialDAG, bestOrdering, bestDAG, bestSc);
      bestSwapBackward(pivot, cur, initialDAG, bestOrdering, bestDAG, bestSc);

      if (bestSc < curScore) {
        steps += 1;
        improving = true;
        cur = bestOrdering;
        curScore = bestSc;
        initialDAG = bestDAG;
      }
    }
    DBG("Cur Score: " << curScore);
  } while(improving);
  DBG("Total Steps: " << steps);

  return SearchResult(curScore, cur);
}

void LocalSearch::tunePruningFactor() {

  SearchResult o1 = hillClimb(Ordering::randomOrdering(instance));

  SearchResult o2 = hillClimb(Ordering::randomOrdering(instance));
  SearchResult o3 = hillClimb(Ordering::randomOrdering(instance));
  
  // Success.
  if (o1.getScore() < PENALTY && o2.getScore() < PENALTY && o3.getScore() < PENALTY) {
    optimalScore = INF; // This resets the best score found so far.
    return;
  }

  // Restart with a higher pruning factor and continue tuning.
  // Exception: if nothing was pruned there's no point in restarting.
  int pruned = instance.restartWithLessPrune(2);

  if (pruned > 0) {
    tunePruningFactor();
  } else {
    optimalScore = INF;
    return;
  }
  
}

SearchResult LocalSearch::genetic(int cutoffGenerations, int INIT_POPULATION_SIZE, int NUM_CROSSOVERS, int NUM_MUTATIONS,
    int MUTATION_POWER, int DIV_LOOKAHEAD, int NUM_KEEP, float DIV_TOLERANCE, CrossoverType crossoverType, int greediness, Types::Score opt, ResultRegister &rr) {
  int n = instance.getN();

  tunePruningFactor();

  SearchResult best(Types::SCORE_MAX, Ordering(n));
  std::deque<Types::Score> fitnesses;
  Population population(*this);
  int numGenerations = 1, nonImprovingGenerations = 0;

  std::cout << "Time: " << rr.check() << " Generating initial population" << std::endl;
  for (int i = 0; i < INIT_POPULATION_SIZE; i++) {
    SearchResult o;

    int triesLeft = 10000;
    do {
      o = hillClimb(Ordering::randomOrdering(instance));
      triesLeft--;

      if (triesLeft == 0) {
        std::cout << "Infeasible..." << std::endl;
        exit(1);
      }
    } while (o.getScore() == INF);


    std::cout << "Time: " << rr.check() <<  " i = " << i << " The score is: " << o.getScore() << std::endl;
    std::vector<int> parents(n);
    std::vector<Types::Score> scores(n);

    rr.record(o.getScore(), o.getOrdering());
    population.addSpecimen(o);
  }
  std::cout << "Done generating initial population" << std::endl;

  do {

    Types::Score initialSc = optimalScore;

    std::vector<SearchResult> offspring;

    population.addCrossovers(NUM_CROSSOVERS, crossoverType, offspring);
    //DBG(population);
    population.mutate(NUM_MUTATIONS, MUTATION_POWER, offspring, instance);
    //DBG(population);
    population.append(offspring);
    population.filterBest(INIT_POPULATION_SIZE);
    DBG(population);
    Types::Score fitness = population.getAverageFitness();
    fitnesses.push_back(fitness);
    if (fitnesses.size() > DIV_LOOKAHEAD) {
      Types::Score oldFitness = fitnesses.front();
      fitnesses.pop_front();
      float change = std::abs(((float)fitness-(float)oldFitness)/(float)oldFitness);
      if (change < DIV_TOLERANCE && DIV_TOLERANCE != -1) {
        DBG("Diversification Step. Change: " << change << " Old: " << oldFitness << " New: " << fitness);
        population.diversify(NUM_KEEP, instance);
        fitnesses.clear();
      }
    }
    //std::cout << "Fitness: " << population.getAverageFitness() << std::endl;
    SearchResult curBest = population.getSpecimen(0);
    Types::Score curScore = curBest.getScore();
    if (curScore < best.getScore()) {
      std::cout << "Time: " << rr.check() <<  " The best score at this iteration is: " << curScore << std::endl;
      rr.record(curBest.getScore(), curBest.getOrdering());
      best = curBest;
    }
    numGenerations++;


    if (optimalScore < initialSc) {
      walkProb = std::max(0.05, walkProb - 0.025);
      nonImprovingGenerations = 0;
    } else {
      walkProb = std::min(0.20, walkProb + 0.025);
      nonImprovingGenerations++;
    }

    std::cout << "Finished generation: " << numGenerations << std::endl;

  } while (numGenerations < cutoffGenerations);
  std::cout << "Generations: " << numGenerations << std::endl;
  return best;
}


bool LocalSearch::allConstraintsSatisfied(const std::vector<int> &parents, const Ordering &o) {
  int n = instance.getN();
  if (numConstraintsSatisfied(parents) != instance.getM_anc()) {
    return false;
  }

  std::vector< std::vector<int> > adj(n);

  for (int i = 0; i < n; i++) {
    adj[i] = std::vector<int> (n);
    for (int j = 0; j < n; j++) {
      adj[i][j] = 0;
    }
  }

  for (int i = 0; i < n; i++) {
    const Variable &var = instance.getVar(i);
    const ParentSet &ps = var.getParent(parents[i]);
    const std::vector<int> &parentsVec = ps.getParentsVec();

    for (int j = 0; j < parentsVec.size(); j++) {
      adj[parentsVec[j]][i] = 1;
    }
  }

  // Check all directed arc existence

  for (int i = 0; i < instance.getM_dae(); i++) {
    std::pair<int, int> cons = instance.deConstraints[i];
    if (!adj[cons.first][cons.second]) {
      std::cout << "Directed arc existence constraint broken: " << cons.first << " -> " << cons.second << std::endl;
      return false;
    }
  }

  //Check undirected existence
  for (int i = 0; i < instance.getM_uae(); i++) {
    std::pair<int, int> cons = instance.ueConstraints[i];
    if (!adj[cons.first][cons.second] && !adj[cons.second][cons.first]) {
      std::cout << "Undirected arc existence constraint broken: " << cons.first << " -- " << cons.second << std::endl;
      return false;
    }
  }

  //Check arc absence

  for (int i = 0; i < instance.getM_aa(); i++) {
    std::pair<int, int> cons = instance.absConstraints[i];
    if (adj[cons.first][cons.second]) {
      std::cout << "Arc absence constraint broken: " << cons.first << " -/-> " << cons.second << std::endl;
      return false;
    }
  }

  // Check ordering
  std::vector< int > positions(n);

  for (int i = 0; i < n; i++) {
    positions[o.get(i)] = i;
  }

  for (int i = 0; i < instance.getM_ord(); i++) {
    std::pair<int, int> cons = instance.ordConstraints[i];
    if (positions[cons.first] >= positions[cons.second]) {
      std::cout << "Ordering constraint broken: " << cons.first << " < " << cons.second << std::endl;
      return false;
    }
  }

  if (numConstraintsSatisfied(parents) != instance.getM_anc()) {
    std::cout << "Ancestral constraint broken" << std::endl;
    return false;
  }

  int m_anc = instance.getM_anc(), m_ancab = instance.getM_ancab(), count = 0;

  for (int i=m_anc; i < m_anc+m_ancab; i++) {
    const Ancestral &cons = instance.getAncestral(i);
    if (hasDipath(parents, cons.first, cons.second)) {
      std::cout << "Ancestral absence constraint broken" << std::endl;
      return false;
    }
  }
  return true;
}

void LocalSearch::checkSolution() {
  int n = instance.getN(), m_anc = instance.getM_anc();

  const Ordering &o = optimalOrdering;
  const std::vector<int> &parents = globalOptimum;
  const std::vector<Types::Score> scores = optimalScores;

  long long scoreFromScores = 0;
  long long scoreFromParents = 0;
  std::vector<int> inverse(n);

  for (int i = 0; i < n; i++) {
    inverse[o.get(i)] = i;
  }
  bool valid = true;
  for (int i = 0; i < n; i++) {
    int var = o.get(i);
    const Variable &v = instance.getVar(var);
    const ParentSet &p = v.getParent(parents[var]);
    const std::vector<int> &parentVars = p.getParentsVec();
    std::string parentsStr = "";
    bool before = true;
    for (int j = 0; j < parentVars.size(); j++) {
      parentsStr += std::to_string(parentVars[j]) + " ";
      before = before && (inverse[parentVars[j]] < i);
    }
    valid &= before;
    scoreFromParents += p.getScore();
    scoreFromScores += scores[var];
    std::cout << "Ordering[" << i << "]\t= "<< var << "\tScore:\t" << scores[var] << "\tParents:\t{ " << parentsStr << "}\tValid: " << before << std::endl;
  }

  std::cout << "Total Score: " << scoreFromScores << " " << scoreFromParents << " " << optimalScore << std::endl;
  std::string validStr = valid ? "Good" : "Bad";
  std::cout << "Validity Check: " << validStr << std::endl;

  bool constraintsSatisfied = allConstraintsSatisfied(parents, o);

  if (constraintsSatisfied) {
    std::cout << "Constraints check: Good" << std::endl;
  } else {
    std::cout << "Constraints check: Bad" << std::endl;
  }

  printModelString(parents, (constraintsSatisfied && valid && (scoreFromParents == scoreFromScores && scoreFromScores == optimalScore)), optimalScore);
}

bool LocalSearch::consistentWithOrdering(const Ordering &o, const std::vector<int> &parents) {
  int n = instance.getN();
  Types::Bitset pred(n, 0);

  for (int i = 0; i < n; i++) {
    int cur = o.get(i);
    const Variable &v = instance.getVar(cur);
    const ParentSet &par = v.getParent(parents[cur]);

    if (!par.subsetOf(pred)) {
      return false;
    }

    pred[cur] = 1;
  }

  return true;
}

void LocalSearch::printModelString(const std::vector<int> &parents, bool valid, Types::Score score) {
  int n = instance.getN();
  std::ifstream file;
  std::ofstream outF;

  std::string instanceFile = instance.getFileName();


  int periodIdx = instanceFile.find('.');

  if (periodIdx == std::string::npos) {
    std::cout << "Warning: the parent scores file is not of the format {instance}_{dataSize}.{scoreType}. Rename the file or modify instance.cpp." << std::endl;
    return;
  }

  std::string instanceName = instanceFile.substr(0, periodIdx);

  if (instance.getFileName().find("asia") != std::string::npos) {
    file = std::ifstream("data/mappings/asia.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("alarm") != std::string::npos) {
    file = std::ifstream("data/mappings/alarm.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("hailfinder") != std::string::npos) {
    file = std::ifstream("data/mappings/hailfinder.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("child") != std::string::npos) {
    file = std::ifstream("data/mappings/child.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("sachs") != std::string::npos) {
    file = std::ifstream("data/mappings/sachs.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("insurance") != std::string::npos) {
    file = std::ifstream("data/mappings/insurance.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("water") != std::string::npos) {
    file = std::ifstream("data/mappings/water.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("barley") != std::string::npos) {
    file = std::ifstream("data/mappings/barley.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("cancer") != std::string::npos) {
    file = std::ifstream("data/mappings/cancer.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("earthquake") != std::string::npos) {
    file = std::ifstream("data/mappings/earthquake.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("survey") != std::string::npos) {
    file = std::ifstream("data/mappings/survey.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } else if (instance.getFileName().find("mildew") != std::string::npos) {
    file = std::ifstream("data/mappings/mildew.mapping");
    outF.open(instanceName + "_results", std::ios_base::app);
  } 
  else {
    std::cout << "No suitable mapping found!" << std::endl;
    exit(0);
  }


  std::vector<std::string> vars(n);

  for (int i=0; i < n; i++) {
    getline(file, vars[i]);
  }


  // Print the network.
  outF << instance.getM_anc() + instance.getM_ord() + instance.getM_aa() + instance.getM_uae() + instance.getM_dae() << std::endl;

  if (!valid) {
    outF << "INVALID" << std::endl;
  }

  for (int i = 0; i < n; i++) {
    outF << "[" + vars[i];

    const Variable &varI = instance.getVar(i);
    const ParentSet &par = varI.getParent(parents[i]);
    const std::vector<int> parVec = par.getParentsVec();

    if (parVec.size() != 0) {
      outF << "|";
    }

    for (int j = 0; j < parVec.size(); j++) {
      outF << vars[parVec[j]];
      if (j < parVec.size() - 1) {
        outF << ":";
      }
    }

    outF << "]";
  }

  outF << std::endl;

  outF << score << std::endl;
}
