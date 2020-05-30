#ifndef GA_H
#define GA_H

#include "common/solution.h"
#include <algorithm>    // std::sort

#define POPULATION_SIZE 100
#define NEW_POPULATION_SIZE 100
//#define PROBABILITY_CROSSOVER 80
//#define TAM_CROSSOVER 98
#define PROBABILITY_MUTATION 5

class GASolution : public Solution{
public:
    double crowding_distance;
    unsigned rank;
    //Usado o nsgaii
    unsigned counter_solution_non_dominated;
    vector<GASolution*> set_solution_dominated;

    GASolution() : Solution(){}
    ~GASolution(){
        set_solution_dominated.clear();
    }

    GASolution& operator=(GASolution &s){

        this->crowding_distance = s.crowding_distance;
        this->rank = s.rank;
        this->counter_solution_non_dominated = s.counter_solution_non_dominated;

        this->set_solution_dominated.clear();
        copy(s.set_solution_dominated.begin(), s.set_solution_dominated.end(), back_inserter(this->set_solution_dominated));

        return (GASolution&)(Solution::operator =(s));
    }

    bool operator <(const Solution& s) {

        return Solution::operator <(s);
    }

    bool operator ==(const Solution& s) {

        return Solution::operator ==(s);
    }
};

void GenerateInitialPopulation(vector<GASolution*> &population);
void PopulationAddIndividual(vector<GASolution> &population, GASolution &individual);
void PrintPopulation(vector<GASolution*> &population);
void Crossover(vector<GASolution*> &population, vector<GASolution*> &new_population);
void Mutation(vector<GASolution*> &population, vector<GASolution*> &new_population);
void Selection(vector<GASolution> &population, vector<GASolution> &new_population);
void GenerateOffspring1(GASolution parent1, GASolution parent2,
                       GASolution & offspring1, GASolution & offspring2);
void GenerateOffspring2(GASolution parent1, GASolution parent2,
                       GASolution &offspring1, GASolution &offspring2);
void GenerateOffspring3(GASolution *parent1, GASolution *parent2,
                       GASolution *offspring1, GASolution *offspring2);
void MutationSwapInside(GASolution &individual);
void MutationSwapOutside(GASolution &individual);
void MutationInsertInside(GASolution &individual);
void MutationInsertOutside(GASolution &individual);
void MutationChangeOpMode(GASolution &individual);
void MutationChangeH(GASolution &individual);
void SolutionListToVector(GASolution s, vector<unsigned> &v_solution);

/*void SortByMakespan(vector<GASolution*> &population);
void SortByTEC(vector<GASolution*> &population);*/

void UnionPopulation(vector<GASolution*> &R, vector<GASolution*> &P, vector<GASolution*> &Q);

#endif // GA_H
