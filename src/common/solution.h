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

    //Tempo de processamento em cada máquina
    vector<unsigned> machine_processing_time;

    //Tempo de preparação em cada máquina
    vector<unsigned> machine_setup_time;

    //Instance de término de cada bloco em cada máquina
    //Serão 3 blocos em cada máquina (antes, durante e depois do horário de pico)
    vector<vector<unsigned>> block_end_time;

    //double PEC_on, PEC_off;

    Solution();
    ~Solution();


    void Print();
    void DummyInitialSolution();
    void RandomInitialSolution();
    void GreedyInitialSolutionMakespan();
    void GreedyInitialSolutionTEC();

    void CalculateInitialTimeMin();
    void CalculateInitialTimeAvoidPeak();
    void CalculateObjective();

    double CalcPECToJob(unsigned machine, unsigned job, unsigned h);

    void Check();

    unsigned FindJobBestPosMacMakespan(unsigned new_job, unsigned machine);
    unsigned FindJobBestPosMacTEC(unsigned new_job, unsigned machine, unsigned &best_op);

    //Solução inicial
    void AddJobGreedyMakespan(unsigned new_job, unsigned mode_op);
    void AddJobGreedyTEC(unsigned new_job, unsigned mode_op);

    //Mutação
    void AddJobGreedyMakespanMachine(unsigned machine, unsigned new_job, unsigned mode_op);
    void AddJobGreedyTECMachine(unsigned machine, unsigned new_job, unsigned mode_op);

    void InsertRandomPosition(unsigned new_job);

    void SwapInside(unsigned machine, unsigned job1, unsigned job2);

    bool operator <(const Solution& s) {
        if((makeSpan < s.makeSpan && TEC < s.TEC)
                || (makeSpan < s.makeSpan && TEC == s.TEC) ||
                (makeSpan == s.makeSpan && TEC < s.TEC)) {
            return true;
        }
        return false;
    }

};

#endif // SOLUTION_H
