#ifndef NSGA_II_H
#define NSGA_II_H

#include "common/solution.h"
#include "common/set_solution.h"
#include "ga.h"
#include "common/Timer.h"
#include <limits>

struct ParametersGA{
    double time_limit;
    unsigned tam_population;
    unsigned prob_mutation;
};

void nsga_ii(ParametersGA param, vector<Solution*> &non_dominated_set);
void FastNonDominatedSort(vector<vector<GASolution*>> &F, vector<GASolution*> &P);
void ComputeCrowdingDistance(vector<GASolution*> &F_i);
void Sort(vector<GASolution*> F_i);

#endif // NSGA_II_H
