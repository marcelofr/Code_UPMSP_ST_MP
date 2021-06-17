#ifndef GASOLUTION_H
#define GASOLUTION_H

#include "common/solution.h"

class GASolution : public Solution{
public:
    double crowding_distance;
    unsigned rank;
    //Usado o nsgaii
    unsigned counter_solution_non_dominated;
    vector<GASolution*> set_solution_dominated;


    GASolution() : Solution(){

    }

    GASolution (const GASolution &s):Solution(s){

        this->crowding_distance = s.crowding_distance;
        this->rank = s.rank;
        this->counter_solution_non_dominated = s.counter_solution_non_dominated;

        copy(s.set_solution_dominated.begin(), s.set_solution_dominated.end(), back_inserter(this->set_solution_dominated));
    }

    ~GASolution(){
        set_solution_dominated.clear();
    }

    GASolution* Create(){
        return new GASolution();
    }

    GASolution& operator=(const GASolution &s){

        Solution::operator=(s);

        this->crowding_distance = s.crowding_distance;
        this->rank = s.rank;
        this->counter_solution_non_dominated = s.counter_solution_non_dominated;

        this->set_solution_dominated.clear();
        copy(s.set_solution_dominated.begin(), s.set_solution_dominated.end(), back_inserter(this->set_solution_dominated));

        return *this;
    }

    bool operator <(const Solution& s) {

        return Solution::operator <(s);
    }

    bool operator ==(const Solution& s) {

        return Solution::operator ==(s);
    }
};

#endif // GASOLUTION_H
