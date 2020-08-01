#ifndef ALGORITHM_DATA_H
#define ALGORITHM_DATA_H

#include <string>
#include <vector>

using namespace std;

struct parameters{
    string instance_folder;
    string instance_file;
    string instance_extension;
    string instance_name;
    string algorithm_name;
    string file_solution;
    string folder_solution;
    unsigned u_seed;
    unsigned u_max_time_factor;
    double d_alpha;
    unsigned u_prob_mutation;
    unsigned u_population_size;
    string s_seed, s_max_time_factor, s_alpha, s_prob_mutation, s_population_size;
};

struct algorithm_data{
    double time_limit;
    double elapsed_time_sec;
    vector<pair<unsigned, double>>non_dominated_set;
    bool is_optimal;
    parameters param;
};

#endif // ALGORITHM_DATA_H
