#ifndef SOLUTION_H
#define SOLUTION_H

#include "instance.h"
#include <vector>
#include <limits.h>
#include "utils.h"
#include <math.h>       /* ceil */

using namespace std;

class Solution
{
public:
    //Objetivos
    size_t makeSpan;
    double TEC;

    //Sequenciamento
    vector<vector<size_t>> scheduling;

    //Instante de início de cada tarefa
    vector<size_t> job_start_time;

    //Instante de termino de cada tarefa
    vector<size_t> job_end_time;

    //Modo de operação de cada tarefa
    vector<size_t> job_mode_op;

    //Instante de término de cada máquina
    vector<size_t> completion_time;

    //double PEC_on, PEC_off;

    Solution();
    ~Solution();


    void Print();
    void DummyInitialSolution();
    void RandomInitialSolution();
    void GreedyInitialSolution();

    void CalculateInitialTimeMin();
    void CalculateObjective();

    void Check();

    bool operator <(const Solution& s) {
        if(makeSpan < s.makeSpan && TEC < s.TEC) {
            return true;
        }
        return false;
    }

};

#endif // SOLUTION_H
