#ifndef SET_SOLUTION_H
#define SET_SOLUTION_H

#include "genetic_algorithm/ga.h"

void SortByMakespan(vector<Solution*> &set_solution);
void SortByTEC(vector<Solution*> &set_solution);

void SortByMakespan(vector<GASolution*> &set_solution);
void SortByTEC(vector<GASolution*> &set_solution);

#endif // SET_SOLUTION_H
