#ifndef SOLUTION_H
#define SOLUTION_H

#include "instance.h"
#include <vector>
#include <limits.h>
#include "utils.h"
#include <math.h>       // ceil
#include <algorithm>    // std::random_shuffle

using namespace std;

class Solution
{
public:
    //Objetivos
    unsigned makeSpan;
    double TEC;
    bool is_optimal;

    //Sequenciamento
    vector<vector<unsigned>> scheduling;

    //Instante de início de cada tarefa
    vector<unsigned> H1;

    //Instante de início de cada tarefa
    vector<unsigned> job_end_time1;

    //Instante de início de cada tarefa
    vector<unsigned> job_start_time1;

    //Modo de operação de cada tarefa
    vector<unsigned> job_mode_op;

    //Instante de conclusão de cada máquina
    vector<unsigned> machine_completion_time;

    //Custo de energia de cada máquina
    vector<unsigned> machine_TEC;

    //double PEC_on, PEC_off;

    Solution();
    ~Solution();

    void Print();
    void DummyInitialSolution();
    void RandomInitialSolution();
    void GreedyInitialSolutionMakespan();
    void GreedyInitialSolutionTEC();
    void GreedyInitialSolutionTEC2();
    void GreedyInitialSolutionTEC3();

    void CalculateShorterTimeHorizon();
    void CalculateHorizonAvoidingPeak();
    void CalculateObjective();

    double CalcPECToJob(unsigned machine, unsigned job, unsigned h);

    void Check();

    unsigned FindJobBestPosMacMakespan(unsigned new_job, unsigned machine);
    unsigned FindJobBestPosMacTEC(unsigned new_job, unsigned machine, unsigned &best_op);
    unsigned FindJobBestPosMacTEC3(unsigned new_job, unsigned machine);

    void ChangeModeOpJob(unsigned machine, unsigned position, unsigned new_mode_op);

    //Solução inicial
    void AddJobGreedyMakespan(unsigned new_job, unsigned mode_op);
    //void AddJobGreedyTEC(unsigned new_job, unsigned mode_op);
    void AddJobGreedyTEC2(unsigned new_job, unsigned mode_op);
    void AddJobGreedyTEC3(unsigned new_job, unsigned mode_op);
    void AddJobGreedyTECMachine3(unsigned machine, unsigned new_job, unsigned mode_op);

    //Cruzamento
    void AddJobGreedyMakespanMachine(unsigned machine, unsigned new_job, unsigned mode_op);
    //void AddJobGreedyTECMachine(unsigned machine, unsigned new_job, unsigned mode_op);

    //Mutação
    void InsertRandomPosition(unsigned new_job);

    void SwapInside(unsigned machine, unsigned job1, unsigned job2);

    bool operator <(const Solution& s) {
            return (makeSpan < s.makeSpan && TEC < s.TEC)
                    || (makeSpan < s.makeSpan && abs(TEC - s.TEC)<EPS) ||
                    (makeSpan == s.makeSpan && TEC < s.TEC);
    }

};

#endif // SOLUTION_H
