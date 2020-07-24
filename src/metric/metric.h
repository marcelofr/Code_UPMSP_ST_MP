#ifndef METRIC_H
#define METRIC_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include "common/file_solution.h"
#include "hypervolume.h"

using namespace std;

void CalculateHypervolume(map<string, map<string, vector<pair<unsigned, double>>>> sets, map<string, map<string, double>> &hypervolume, map<string, pair<unsigned, double>> reference_points);
void GenerateReferenceSet(string folder_solution, map<string, map<string, vector<pair<unsigned, double>>>> &sets, map<string, map<string, double>> &hypervolume, map<string, pair<unsigned, double>> &reference_points);
void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set, string folder_solution, string instance_name, string algorithm_name, pair<unsigned, double> reference_point);
void ReadFiles(vector<string> files, map<string, map<string, vector<pair<unsigned, double>>>> &sets);

#endif // METRIC_H
