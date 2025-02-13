#ifndef POPULATION_H
#define POPULATION_H 

#include "localsearch.h"
#include "ordering.h"
#include "instance.h"
#include "searchresult.h"
#include "types.h"

class LocalSearch;

enum class CrossoverType;

class Population {
  public:
    Population(LocalSearch &localSearch);
    void addSpecimen(const SearchResult &o);
    int getSize() const;
    SearchResult getSpecimen(int i) const;
    friend std::ostream& operator<<(std::ostream &os, const Population& sr);
    void addCrossovers(int n, CrossoverType crossoverType, std::vector<SearchResult> &offspring);
    Ordering crossoverOB(const Ordering &o1, const Ordering &o2);
    Ordering crossoverCX(const Ordering &o1, const Ordering &o2);
    void mutate(int NUM_MUTATIONS, int MUTATION_POWER, std::vector<SearchResult> &offspring, const Instance &instance);
    void filterBest(int n);
    Types::Score getAverageFitness();
    void diversify(int numKeep, Instance &instance);
    Ordering crossoverRK(const Ordering &o1, const Ordering &o2);
    void append(const std::vector<SearchResult> &offspring);
  private:
    std::vector<SearchResult> specimens;
    LocalSearch &localSearch;
};

#endif /* POPULATION_H */