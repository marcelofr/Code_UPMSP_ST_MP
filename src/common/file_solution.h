#ifndef FILE_SOLUTION_H
#define FILE_SOLUTION_H

/*#include "hypervolume.h"
#include <string>
#include <iostream>*/
#include <filesystem>
#include <vector>
#include <fstream>      // std::fstream

using namespace std;

struct solution_data{
    string file_solution;
    string instance_name;
    string algorithm_name;
    double time_limit;
    unsigned seed;
    double elapsed_time_sec;
    double alpha;
    unsigned population_size;
    double prob_mutation;
    vector<pair<unsigned, double>>non_dominated_set;
    bool is_optimal;
};

void FindFilesInFolder(string folder_solution, vector<string> &files);
void ReadFile(solution_data &sd);

#endif // FILE_SOLUTION_H
