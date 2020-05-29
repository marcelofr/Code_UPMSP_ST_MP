#ifndef BASIC_LOCAL_SEARCH_H
#define BASIC_LOCAL_SEARCH_H

#include "common/solution.h"

class LSSolution : public Solution{
public:
    bool was_visited;
};

bool SwapInsideLS_FI(LSSolution my_solution, vector<LSSolution> &non_dominated_set);
bool SwapOutsideLS_FI(LSSolution my_solution, vector<LSSolution> &non_dominated_set);

bool InsertInsideLS_FI(LSSolution my_solution, vector<LSSolution> &non_dominated_set);
bool InsertOutsideLS_FI(LSSolution my_solution, vector<LSSolution> &non_dominated_set);

bool ChangeOpModeLS(LSSolution my_solution, vector<LSSolution> &non_dominated_set);
bool ChangeHLS(LSSolution my_solution, vector<LSSolution> &non_dominated_set);

bool AddSolution(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set);

#endif // BASIC_LOCAL_SEARCH_H
