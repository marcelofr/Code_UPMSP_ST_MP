#ifndef BASIC_LS_MONO_H
#define BASIC_LS_MONO_H

#include "common/set_solution.h"
#include "mono/monosolution.h"
#include <algorithm>

bool CompareMakespanMonoSolution(MonoSolution *& l, MonoSolution *& r);
void SortByMakespanMonoSolution(vector<MonoSolution*> &set_solution);

void SetWeights(NDSetSolution<MonoSolution *> non_dominated_set);
void GenerateWeightVector(vector<pair<double, double>> &W, unsigned num_weights);

MonoSolution * Destruction(MonoSolution * solution, unsigned level);
void Construction(MonoSolution * partial_solution, vector<unsigned> removed_jobs);
bool IntesificationArroyo(MonoSolution *current_solution, unsigned level);

MonoSolution * Shaking(MonoSolution *cur_solution, unsigned op_neighbor, unsigned level);
MonoSolution *GenNeighborSol(MonoSolution *my_solution, unsigned op_neighbor);

#endif // BASIC_LS_MONO_H
