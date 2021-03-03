#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "common/solution.h"
#include "basic_local_search.h"
#include "common/Timer.h"
#include "common/set_solution.h"


void HillClimbing(NDSetSolution<LSSolution*> *non_dominated_set, Timer *t1);

#endif // LOCAL_SEARCH_H
