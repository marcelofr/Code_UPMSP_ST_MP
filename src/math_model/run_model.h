#ifndef RUN_MODEL_H
#define RUN_MODEL_H

#include "gurobi_c++.h"
#include "model.h"

#include <string>

using namespace std;

void RunMathModel(string instance_file, unsigned max_time, double alpha);

#endif // RUN_MODEL_H
