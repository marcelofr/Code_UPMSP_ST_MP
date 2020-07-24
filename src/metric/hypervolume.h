#ifndef HYPERVOLUME_H
#define HYPERVOLUME_H

#include<common/solution.h>

#define REFERENCE 15000

double CalculateHypervolumeMin(vector<pair<unsigned, double>> set_solution, pair<unsigned, double> reference);

#endif // HYPERVOLUME_H
