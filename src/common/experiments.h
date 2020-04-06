#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <string>
#include <sstream>
#include "utils.h"
#include "instance.h"
#include "genetic_algorithm/nsga_ii.h"

using namespace std;

struct Parameters{
    string instanceFile;
    string seed;
    string algorithm;
    string fileSolution;
    unsigned timeLimit;
};

void RunAlgorithm(Parameters Param);



#endif // EXPERIMENTS_H
