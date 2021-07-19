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
    string s_destruction_factor;
    unsigned u_destruction_factor;
    string s_decomposition_size, s_decomposition_neighboor_size;
    unsigned u_decomposition_size, u_decomposition_neighboor_size;

};

struct algorithm_data{
    double time_limit;
    double elapsed_time_sec;
    vector<pair<unsigned, double>>non_dominated_set;
    bool is_optimal;
    parameters param;
    unsigned qtd_neighbor;
    unsigned max_shake_level;
    unsigned num_weights;
    unsigned num_group;
};

#endif // ALGORITHM_DATA_H
