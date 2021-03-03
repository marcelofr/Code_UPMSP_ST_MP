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

    GASolution();
    GASolution (const GASolution &s);
    ~GASolution();

    GASolution* Create();

    GASolution& operator=(const GASolution &s);
    bool operator <(const Solution& s) ;
    bool operator ==(const Solution& s) ;
};

#endif // GASOLUTION_H
