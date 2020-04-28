#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <string>
#include <sstream>
#include "utils.h"
#include "instance.h"
#include "genetic_algorithm/nsga_ii.h"
#include "metric/metric.h"
#include "math_model/run_model.h"

using namespace std;

struct Parameters{
    string instance_folder;
    string instance_file;
    string instance_extension;
    string instance_name;
    string seed;
    string algorithm;
    string file_solution;
    string folder_solution;
    string max_time_factor;
};

void RunAlgorithm(Parameters Param);

void SalveSolution(vector<Solution> non_dominated_set, Parameters Param, instance_result ir);

#endif // EXPERIMENTS_H
