#ifndef TYPES_H
#define TYPES_H

#include <boost/dynamic_bitset.hpp>
#include <climits>

class Types {
  public:
    typedef int64_t Score;
    typedef boost::dynamic_bitset<uint64_t> Bitset;
    static const Score SCORE_MAX = 9223372036854775807LL;
};
 
#endif /* TYPES_H */
