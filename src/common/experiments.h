#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <string>
#include <sstream>
#include "utils.h"
#include "instance.h"
#include "genetic_algorithm/nsga_ii.h"
#include "math_model/run_model.h"
#include "common/solution.h"
#include "local_search/main_ls_multi.h"
#include "common/set_solution.h"
#include "metric/hypervolume.h"
#include "file_solution.h"
#include <map>
#include "metric/metric.h"
#include "algorithm_data.h"
#include "mono/main_ls_mono.h"
#include "mono/basic_ls_mono.h"

using namespace std;

void RunAlgorithm(algorithm_data alg_data);

void SalveFileSolution(algorithm_data alg_data);
//void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set, string folder_solution, string instance_name, string algorithm_name);
void SelectOnlyValidSolutions(vector<Solution*> non_dominated_set);

//void GenerateReferenceSet(string folder_solution, map<string, map<string, vector<pair<unsigned, double>>>> sets);
void CalculateMetric(string folder_solution, string folder_instance);

void Discretize(unsigned factor);

void RunAlgorithmNSGAII(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmExact(algorithm_data alg_data, vector<Solution*> &non_dominated_set);
//void RunAlgorithmHillClimbing(vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmHillClimbing(vector<Solution*> &non_dominated_set, algorithm_data alg_data, Timer *t1);
void RunAlgorithmMOVNS(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmMOVNSArroyo(algorithm_data alg_data, vector<Solution*> &nd_set_solution, Timer *t1);
void RunAlgorithmMOVNSEduardo(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);
void RunAlgorithmMono(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1);

#endif // EXPERIMENTS_H
