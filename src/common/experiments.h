#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <string>
#include <sstream>
#include "utils.h"
#include "instance.h"
#include "genetic_algorithm/nsga_ii.h"
#include "math_model/run_model.h"
#include "common/solution.h"
#include "local_search/local_search.h"
#include "common/set_solution.h"
#include "metric/hypervolume.h"
#include "file_solution.h"
#include <map>
#include "metric/metric.h"
#include "algorithm_data.h"

using namespace std;

void RunAlgorithm(algorithm_data alg_data);

void SalveFileSolution(algorithm_data alg_data);
//void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set, string folder_solution, string instance_name, string algorithm_name);
void SelectOnlyValidSolutions(vector<Solution*> non_dominated_set);

//void GenerateReferenceSet(string folder_solution, map<string, map<string, vector<pair<unsigned, double>>>> sets);
void CalculateMetric(string folder_solution);

#endif // EXPERIMENTS_H
