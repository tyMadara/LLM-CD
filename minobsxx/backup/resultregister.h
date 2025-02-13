#ifndef RESULTREGISTER_H
#define RESULTREGISTER_H 

#include <sys/time.h>
#include <utility>
#include <vector>
#include <string>
#include "searchresult.h"
#include "ordering.h"
#include "types.h"

class ResultRegister {
  public:
    ResultRegister();
    void record(Types::Score score, const Ordering &o);
    void set();
    void setOrigin();
    void dump(const std::string &outFile);
    Types::Score getBest();
    float check();
    void write(std::ofstream &os);
    void dump(const std::string &outFile, const std::string &instanceTitle);
    void dump(const std::string &outFile, const std::string &instanceTitle, int argc, char* argv[], const SearchResult &sr);
  private:
    long int origin;
    long int checkOrigin;
    Types::Score bestScore;
    std::vector<std::pair<long int, Types::Score>> scores;
    std::vector<std::pair<long int, Types::Score>> bestScores;
    std::vector<std::string> bestOrderings;
};

#endif /* RESULTREGISTER_H */
