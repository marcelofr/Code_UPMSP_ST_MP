#ifndef RUN_MODEL_H
#define RUN_MODEL_H

#include "gurobi_c++.h"
#include "model.h"

#include <string>

using namespace std;

void RunMathModel(double max_time, double alpha, Solution * my_solution);

#endif // RUN_MODEL_H
