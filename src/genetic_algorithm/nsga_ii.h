#ifndef NSGA_II_H
#define NSGA_II_H

#include "common/solution.h"
#include "common/set_solution.h"
#include "ga.h"
#include "common/Timer.h"
#include <limits>

void nsga_ii(unsigned time_limit, vector<Solution*> &non_dominated_set);
void FastNonDominatedSort(vector<vector<GASolution*>> &F, vector<GASolution*> &P);
void ComputeCrowdingDistance(vector<GASolution*> &F_i);
void Sort(vector<GASolution*> F_i);

#endif // NSGA_II_H
