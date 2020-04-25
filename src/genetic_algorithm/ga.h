#ifndef GA_H
#define GA_H

#include "common/solution.h"
#include <algorithm>    // std::sort

#define POPULATION_SIZE 100
#define NEW_POPULATION_SIZE 100
#define PROBABILITY_CROSSOVER 80
#define TAM_CROSSOVER 98
#define PROBABILITY_MUTATION 1

class GASolution : public Solution{
public:
    double crowding_distance;
    unsigned rank;
    //Usado o nsgaii
    unsigned counter_solution_non_dominated;
    vector<GASolution*> set_solution_dominated;
};

void GenerateInitialPopulation(vector<GASolution> &population);
void PopulationAddIndividual(vector<GASolution> &population, GASolution &individual);
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
void MutationOperatorSwapInside(GASolution &individual);
void MutationOperatorSwapOutside(GASolution &individual);
void MutationOperatorInsertionInside(GASolution &individual);
void MutationOperatorInsertionOutside(GASolution &individual);
void MutationOperatorOperationMode(GASolution &individual);
void MutationOperatorH(GASolution &individual);
void SolutionListToVector(GASolution s, vector<unsigned> &v_solution);

void SortByMakespan(vector<GASolution> &population);
void SortByTEC(vector<GASolution> &population);

#endif // GA_H
