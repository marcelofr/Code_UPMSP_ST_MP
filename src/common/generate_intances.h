#ifndef GENERATE_INTANCES_H
#define GENERATE_INTANCES_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "utils.h"


#define PEAK_START 108
#define PEAK_END 125
#define RATE_IN_PEAK 0.47753
#define RATE_OFF_PEAK 0.32282
#define PLAN_HORIZON_SIZE 143



using namespace std;

struct InstanceParam{
    unsigned num_jobs, num_machines, num_op_mode;
    vector<double> speed_factor, consumption_factor;
    vector<unsigned> machine_potency;
    string destination;
    pair<unsigned, unsigned>range_setup, range_processing, range_potency;
};

void GenerateSmallInstances();
void GenerateLargeInstances();

void CreateNewIntance(InstanceParam ip);

#endif // GENERATE_INTANCES_H
