#include<iostream>
#include <string>
#include<fstream>
#include <sys/time.h>
#include<stdlib.h>
#include "instance.h"
#include "ordering.h"
#include "localsearch.h"
#include "debug.h"
#include "resultregister.h"
#include <unistd.h>
#include "util.h"
#include "types.h"
#include "math.h"

void usage() {
  std::cerr <<
    "Command without optinal arguemnts\n\n" <<
    "\t./search <instance-file> <constraints-file> <cutofftime (seconds)> <seed> <output file>\n\n" <<
    "If <seed> is -1, the current time will be used as the seed.\n" <<
    "Full command (with all optional arguments): \n\n" <<
    "\t./search  <instance-file> <cutofftime> <seed> <output file> -populationsize <pop size>\n\t-crossover <# of crossovers> -nummutation <# of mutations>\n\t-divlookahead <check paper> -numkeep <check paper>\n\t-crossovertype <check paper> -powerfactor <check paper>\n\n" <<
    "By default, the tuned parameters in the paper are used.\n" <<
    "The result is printed to std::out at the end and a file with progress is dumped.\n\n" <<
    "For more information, feel free to contact me at cdlee@edu.uwaterloo.ca.\n";
} 

int main(int argc, char* argv[]) {
  if (argc < 6) {
    usage();
    return 0;
  }
  Types::Score opt;
  std::string fileName = argv[1];
  std::string constraintsFileName = argv[2];
  int cutoffGenerations = atoi(argv[3]);
  int seed = atoi(argv[4]);
  std::string outFile = argv[5];
  seed = seed == -1 ? time(NULL) : seed;
  ResultRegister rr;
  rr.setOrigin();
  rr.set();
  srand(seed);
  Instance instance(fileName, constraintsFileName);

  LocalSearch localSearch(instance, rr);
  int n = instance.getN();
  int initPopulationSize = 20;
  int numCrossovers = 20;
  int numMutations = 6;
  int mutationPower = ceil(n*0.01);
  int divLookahead = 32;
  int numKeep = 4;
  float divTolerance = 0.001;
  int greediness = -1;
  CrossoverType crossoverType = CrossoverType::RK;
  for (int i = 6; i < 23 && i < argc; i++) {
    std::string param(argv[i]);
    DBG(argv[i]);
    if (param == "-populationsize") {
      initPopulationSize = atoi(argv[i+1]);
    } else if (param == "-crossover") {
      numCrossovers = atoi(argv[i+1]);
    } else if (param == "-nummutation") {
      numMutations = atoi(argv[i+1]);
    } else if (param == "-divlookahead") {
      divLookahead = atoi(argv[i+1]);
    } else if (param == "-numkeep") {
      numKeep = atoi(argv[i+1]);
    } else if (param == "-divtolerance") {
      divTolerance = atof(argv[i+1]);
    } else if (param == "-greediness") {
      greediness = atoi(argv[i+1]);
    } else if (param == "-crossovertype") {
      std::string crossoverTypeString = argv[i+1];
      if (crossoverTypeString == "OB") {
        crossoverType = CrossoverType::OB;
      } else if (crossoverTypeString == "RK") {
        crossoverType = CrossoverType::RK;
      } else {
        crossoverType = CrossoverType::CX;
      }
    } else if (param == "-powerfactor") {
      float powerfactor = atof(argv[i+1]);
      mutationPower = ceil(n*powerfactor);
    }
  }
  
  SearchResult sr = localSearch.genetic(cutoffGenerations, initPopulationSize, numCrossovers, numMutations, mutationPower, divLookahead, numKeep, divTolerance, crossoverType, greediness, opt, rr);
  localSearch.checkSolution();

  rr.dump(outFile, fileName, argc, argv, sr);
  return 0;
}
