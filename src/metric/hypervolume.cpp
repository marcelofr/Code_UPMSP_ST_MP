#include "hypervolume.h"

/*
 * Método para calcular o hipervolume para problemas com dois objetivos
 */
double CalculateHypervolume(vector<pair<unsigned, double>> set_solution, pair<unsigned, double> reference)
{

    double hypervolume, b, h, b_last;

    auto it = set_solution.begin();

    b = it->first - reference.first;
    h = it->second - reference.second;

    hypervolume = b * h;

    b_last = it->first;

    for (++it; it != set_solution.end();++it) {

        b = it->first - b_last;
        h = it->second - reference.second;

        hypervolume += b * h;

        b_last = it->first;
    }

    return hypervolume;
}
