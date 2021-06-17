#ifndef MONOSOLUTION_H
#define MONOSOLUTION_H

#include "local_search/lssolution.h"

class MonoSolution : public LSSolution{
public:
    double objective_funtion;
    pair<double, double> weights;

    MonoSolution():LSSolution(){

    }

    MonoSolution (const MonoSolution &s):LSSolution(s){
        this->objective_funtion = s.objective_funtion;
    }

    ~MonoSolution(){}

    MonoSolution* Create(){
        return new MonoSolution();
    }

    MonoSolution& operator=(const MonoSolution &s){

        LSSolution::operator=(s);

        this->objective_funtion = s.objective_funtion;

        this->weights = s.weights;

        return *this;
    }

    bool operator <(const MonoSolution& s){

        return LSSolution::operator <(s);
    }

    bool operator ==(const MonoSolution& s){

        return LSSolution::operator ==(s);
    }

    void CalculeMonoObjective(){

        this->objective_funtion = this->weights.first*this->makeSpan + this->weights.second*this->TEC;
    }

    void CalculeMonoObjectiveTchebycheff(){

        if(this->makeSpan < Z_STAR::makespan){
            Z_STAR::makespan = this->makeSpan;
        }

        if(this->TEC < Z_STAR::TEC){
            Z_STAR::TEC = this->TEC;
        }

        this->objective_funtion = max(this->weights.first*(this->makeSpan-Z_STAR::makespan), this->weights.second*(this->TEC-Z_STAR::TEC));
    }

    void GenerateGreedySolutionWeigth()
    {
        vector<unsigned> jobs(Instance::num_jobs+1);
        unsigned machine, position, diff_time;
        unsigned best_machine, best_position, best_diff_time, best_op_mode;
        double obj_job, best_obj_job;
        vector<unsigned>::iterator it2;

        for(unsigned i = 0; i <= Instance::num_jobs; i++){
            jobs[i] = i;
        }

        //Gerar um vetor com números aleatórios
        srand(Instance::seed);
        random_shuffle(jobs.begin()+1, jobs.end());

        //Inserir cada tarefa através de uma estratégia gulosa
        while (jobs.size() > 1) {
            best_obj_job = INT_MAX;
            best_machine = 1;
            best_position = 0;
            best_diff_time = 0;
            best_op_mode = 1;
            for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

                for(unsigned o=1; o<=Instance::num_mode_op; o++){
                    obj_job = 0;
                    machine = 1;
                    GreedyChoiceWeigth(*it, o, machine, position, diff_time, obj_job, weights);
                    if(obj_job < best_obj_job){
                        best_machine = machine;
                        best_position = position;
                        best_diff_time = diff_time;
                        best_obj_job = obj_job;
                        best_op_mode = o;
                        it2 = it;
                    }
                }

            }

            AddJob(*it2, best_machine, best_position, best_diff_time);

            //Definir o modo de operação da nova tarefa
            job_mode_op[*it2] = best_op_mode;

            jobs.erase(it2);

        }

        //CalculateInitialTimeMin();
        CalculateShorterTimeHorizon();
        CalculateObjective();

    #ifdef DEBUG
        Check();
    #endif
    }

};


#endif // MONOSOLUTION_H
