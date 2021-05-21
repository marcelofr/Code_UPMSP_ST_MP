#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "common/solution.h"
#include "basic_local_search.h"
#include "common/Timer.h"
#include "common/set_solution.h"
#include "../common/algorithm_data.h"

//void HillClimbing(NDSetSolution<LSSolution*> &non_dominated_set);
void HillClimbing(NDSetSolution<LSSolution*> &non_dominated_set, algorithm_data alg_data, Timer *t1);
void LS_FI_OP(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out, unsigned op_neighbor);
bool VND_FI(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out);
bool LS_BI_OP(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out, unsigned op_neighbor);
bool VND_BI(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out);
void MORVNS(NDSetSolution<LSSolution*> &non_dominated_set, algorithm_data alg_data, Timer *t1);
void MOVNS(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1);
void MOVNS_Arroyo(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1);
void MOVNS_Eduardo(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1);
#endif // LOCAL_SEARCH_H
