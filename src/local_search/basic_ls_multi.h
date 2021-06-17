#ifndef BASIC_LOCAL_SEARCH_H
#define BASIC_LOCAL_SEARCH_H

#include "local_search/lssolution.h"

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

//bool AddSolution(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set);

LSSolution *SelectNonDomintatedSolution(NDSetSolution<LSSolution*> &non_dominated_set);
unsigned SelectNonVisitedSolution(NDSetSolution<LSSolution *> non_dominated_set);
unsigned ExistNonDomintatedSolution(NDSetSolution<LSSolution *> non_dominated_set);
LSSolution* GenRandomNeighborSol(LSSolution *my_solution);
LSSolution *GenNeighborSol(LSSolution *my_solution, unsigned op_neighbor);

//void Shaking(NDSetSolution<LSSolution *> *non_dominated_set_in, NDSetSolution<LSSolution *> *non_dominated_set_out, unsigned op_neighbor, unsigned level);
LSSolution * Shaking(LSSolution *solution, unsigned op_neighbor, unsigned level);
//void Shaking_Eduardo(NDSetSolution<LSSolution *> *non_dominated_set_in, NDSetSolution<LSSolution *> *non_dominated_set_out, unsigned op_neighbor, unsigned level);

LSSolution * Destruction(LSSolution * solution, unsigned level);
bool Construction(LSSolution * partial_solution,NDSetSolution<LSSolution *> *non_dominated_set_partial_a, vector<unsigned> removed_jobs);
bool IntesificationArroyo(LSSolution *current_solution, NDSetSolution<LSSolution *> &non_dominated_set, unsigned level);

void SortByMakespanLSSolution(vector<LSSolution*> &set_solution);

#endif // BASIC_LOCAL_SEARCH_H
