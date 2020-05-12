#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "common/solution.h"
#include "basic_local_search.h"

void GenInitialSet(vector<LSSolution> &non_dominated_set);

void HillClimbing(vector<Solution> &non_dominated_set);

#endif // LOCAL_SEARCH_H
