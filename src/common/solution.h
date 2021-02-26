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
    //void GreedyInitialSolutionTEC();
    //void GreedyInitialSolutionTEC2();
    void GreedyInitialSolutionTEC3();

    void CalculateShorterTimeHorizon();
    void CalculateHorizonAvoidingPeak();
    void CalculateObjective();

    double CalcPECToJob(unsigned machine, unsigned job, unsigned h);

    void Check();

    unsigned FindJobBestPosMacMakespan(unsigned new_job, unsigned machine);
    unsigned FindJobBestPosMacTEC(unsigned new_job, unsigned machine, unsigned &best_op);
    unsigned FindJobBestPosMacTEC3(unsigned new_job, unsigned machine);

    void SelectBestModeOpJob(unsigned machine, unsigned position, unsigned new_mode_op);

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
    void SwapOutside(unsigned machine1, unsigned pos_job1, unsigned machine2, unsigned pos_job2);
    void InsertInside(unsigned machine, unsigned pos1, unsigned pos2);
    void InsertOutside(unsigned machine1, unsigned pos1, unsigned machine2, unsigned pos2);
    void ChangeModeOpJob(unsigned machine, unsigned position, unsigned new_mode_op);
    void ChangeHJob(unsigned machine, unsigned position, unsigned add_h);

    Solution& operator=(Solution &s){

        if( &s != this ) {

            Solution* ptr_object = NULL;
            if(&s != ptr_object){
                this->makeSpan = s.makeSpan;
                this->TEC = s.TEC;
                this->is_optimal = s.is_optimal;


                for(unsigned i = 0; i < this->scheduling.size(); i++){
                    this->scheduling[i].clear();
                }

                for(unsigned i = 0; i < this->scheduling.size(); i++){
                    copy(s.scheduling[i].begin(), s.scheduling[i].end(), back_inserter(this->scheduling[i]));
                }



                this->H1.clear();
                copy(s.H1.begin(), s.H1.end(), back_inserter(this->H1));
                this->job_end_time1.clear();
                copy(s.job_end_time1.begin(), s.job_end_time1.end(), back_inserter(this->job_end_time1));
                this->job_start_time1.clear();
                copy(s.job_start_time1.begin(), s.job_start_time1.end(), back_inserter(this->job_start_time1));
                this->job_mode_op.clear();
                copy(s.job_mode_op.begin(), s.job_mode_op.end(), back_inserter(this->job_mode_op));
                this->machine_completion_time.clear();
                copy(s.machine_completion_time.begin(), s.machine_completion_time.end(), back_inserter(this->machine_completion_time));
                this->machine_TEC.clear();
                copy(s.machine_TEC.begin(), s.machine_TEC.end(), back_inserter(this->machine_TEC));
            }



        }
        return *this;
    }

    bool operator <(const Solution& s) {
            return (this->makeSpan < s.makeSpan && this->TEC < s.TEC)
                    || (this->makeSpan < s.makeSpan && abs(this->TEC - s.TEC) < EPS) ||
                    (this->makeSpan == s.makeSpan && this->TEC < s.TEC);
    }

    bool operator ==(const Solution& s) {
            return (this->makeSpan == s.makeSpan && abs(this->TEC - s.TEC) < EPS);
    }

};

bool CompareMakespan(Solution * l, Solution * r);
bool CompareTEC(Solution * l, Solution * r);

#endif // SOLUTION_H
