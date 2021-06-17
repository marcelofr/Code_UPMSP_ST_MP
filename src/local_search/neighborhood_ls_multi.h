#ifndef NEIGHBORHOOD_LS_MULTI_H
#define NEIGHBORHOOD_LS_MULTI_H

#include "basic_ls_multi.h"

bool SwapInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool SwapInsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set_out);
bool SwapOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool SwapOutsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution*> &non_dominated_set);

bool InsertInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool InsertInsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution *> &non_dominated_set);
bool InsertOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool InsertOutsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set);

bool ChangeOpModeLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set);
bool ChangeOpModeLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set);
bool ChangeHLS(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set);

bool InsertOutsideDuoLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set);

#endif // NEIGHBORHOOD_LS_MULTI_H
