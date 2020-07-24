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
    string alpha;
    string prob_mutation;
    string tam_population;
};

void RunAlgorithm(Parameters Param);

void SalveFileSolution(solution_data sd);
//void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set, string folder_solution, string instance_name, string algorithm_name);
void SelectOnlyValidSolutions(vector<Solution*> non_dominated_set);

//void GenerateReferenceSet(string folder_solution, map<string, map<string, vector<pair<unsigned, double>>>> sets);
void CalculateMetric(string folder_solution);

#endif // EXPERIMENTS_H
