#include "hypervolume.h"

/*
 * Método para calcular o hipervolume para problemas de minimização com dois objetivos
 * Considera que o conjunto está ordenado pelo makespan, crescente
 */
double CalculateHypervolumeMin(vector<pair<unsigned, double>> set_solution, pair<unsigned, double> reference)
{

    double hypervolume, b, h, h_last;

    auto it = set_solution.begin();

    b = reference.first - it->first;
    h = reference.second - it->second;

    hypervolume = b * h;

    h_last = it->second;

    for (++it; it != set_solution.end();++it) {

        b = reference.first - it->first;
        h = h_last - it->second;

        hypervolume += b * h;

        h_last = it->second;
    }

    return hypervolume;
}
