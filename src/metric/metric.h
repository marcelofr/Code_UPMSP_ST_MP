#ifndef METRIC_H
#define METRIC_H

#include "hypervolume.h"
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>      // std::fstream

using namespace std;

struct instance_result{
    string algorithm_name;
    unsigned time_limit;
    double elapsed_time_sec;
    string instance_name;
    unsigned seed;
    vector<pair<unsigned, double>>non_dominated_set;
    bool is_optimal;
};


void CalculateMetric(string folder_solution);

void ReadFilesInFolder(string folder_solution, vector<string> &files);
void ReadFile(string file_name, instance_result &ir, vector<vector<pair<unsigned, double>>>&non_dominated_sets);

#endif // METRIC_H
