#ifndef MAIN_LS_MONO_H
#define MAIN_LS_MONO_H

#include "mono/neighborhood_ls_mono.h"
#include "common/Timer.h"
#include "../common/algorithm_data.h"

void MOVNS_D(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1);
bool LS_Mono_BI(MonoSolution *my_solution, unsigned op_neighbor);

#endif // MAIN_LS_MONO_H
