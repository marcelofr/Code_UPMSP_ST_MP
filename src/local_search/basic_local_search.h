#ifndef BASIC_LOCAL_SEARCH_H
#define BASIC_LOCAL_SEARCH_H

#include "common/solution.h"
#include "common/set_solution.h"

class LSSolution : public Solution{
public:
    bool was_visited;

    void PrintNonDominatedSet(vector<LSSolution*> &non_dominated_set){
        PrintNonDominatedSet(non_dominated_set);
    }

    LSSolution* Create(){
        return new LSSolution();
    }
};


void PrintNonDominatedSet(vector<Solution*> &non_dominated_set);

bool SwapInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> *non_dominated_set);
bool SwapOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> *non_dominated_set);

bool InsertInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> *non_dominated_set);
bool InsertOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> *non_dominated_set);

bool ChangeOpModeLS(LSSolution *my_solution, NDSetSolution<LSSolution*> *non_dominated_set);
bool ChangeHLS(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set);

bool AddSolution(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set);

#endif // BASIC_LOCAL_SEARCH_H
