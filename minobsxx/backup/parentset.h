#ifndef PARENTSET_H
#define PARENTSET_H

#include<boost/dynamic_bitset.hpp>
#include<vector>
#include "types.h"

class ParentSet {
  public:
    ParentSet(Types::Score score, Types::Bitset parents, int var, int id, std::vector<int> parentsVec);
    Types::Score getScore() const;
    bool hasElement(int k) const;
    bool subsetOf(const Types::Bitset &set) const;
    bool supersetOf(const Types::Bitset &set) const;
    int getVar() const;
    int getId() const;
    Types::Bitset getParents() const;
    int size() const;
    void setId(const int &i);
    friend std::ostream& operator<<(std::ostream &os, const ParentSet& p);
    const std::vector<int> &getParentsVec() const;

  private:
    Types::Score score;
    Types::Bitset parents;
    int var;
    int id;
    std::vector<int> parentsVec;
};
 
#endif /* PARENTSET_H */
