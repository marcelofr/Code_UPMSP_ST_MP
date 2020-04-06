#ifndef GA_H
#define GA_H

#include "common/solution.h"
#include <algorithm>    // std::sort

#define POPULATION_SIZE 5
#define NEW_POPULATION_SIZE 5
#define NUM_GENERATIONS 5

class GASolution : public Solution{
public:
    unsigned crowding_distance;
    //Usado o nsgaii
    unsigned n;
    vector<GASolution*> set_solution_dominated;
};

void GenerateInitialPopulation(vector<GASolution> &population);
void PrintPopulation(vector<GASolution> &population);
void Crossover(vector<GASolution> &population, vector<GASolution> &new_population);
void Mutation(vector<GASolution> &population, vector<GASolution> &new_population);
void Selection(vector<GASolution> &population, vector<GASolution> &new_population);
void GenerateOffspring1(GASolution parent1, GASolution parent2,
                       GASolution & offspring1, GASolution & offspring2);
void GenerateOffspring2(GASolution parent1, GASolution parent2,
                       GASolution &offspring1, GASolution &offspring2);
void GenerateOffspring3(GASolution parent1, GASolution parent2,
                       GASolution &offspring1, GASolution &offspring2);
void SolutionListToVector(GASolution s, vector<unsigned> &v_solution);

void SortByMakespan(vector<GASolution> &population);

#endif // GA_H
