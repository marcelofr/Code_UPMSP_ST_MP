#include "main_ls_mono.h"

/*
 * Local search monobjective with best improvement
 */
bool LS_Mono_BI(MonoSolution *my_solution, unsigned op_neighbor){

    MonoSolution *current_solution = new MonoSolution();
    MonoSolution *best_solution = new MonoSolution();

    *best_solution = *current_solution = *my_solution;

    switch (op_neighbor) {
        case 0:
            SwapInsideLSMono_BI(current_solution);
            best_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < best_solution->objective_funtion){
                *best_solution = *current_solution;
            }
        break;
        case 1:
            SwapOutsideLSMono_BI(current_solution);
            best_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < best_solution->objective_funtion){
                *best_solution = *current_solution;
            }
        break;
        case 2:
            InsertInsideLSMono_BI(current_solution);
            best_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < best_solution->objective_funtion){
                *best_solution = *current_solution;
            }
        break;
        case 3:
            InsertOutsideLSMono_BI(current_solution);
            best_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < best_solution->objective_funtion){
                *best_solution = *current_solution;
            }
        break;
        case 4:
            ChangeOpModeLSMono_BI(current_solution);
            best_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < best_solution->objective_funtion){
                *best_solution = *current_solution;
            }
        break;
        case 5:
            IntesificationArroyo(current_solution, Instance::num_jobs/10);
            best_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < best_solution->objective_funtion){
                *best_solution = *current_solution;
            }
        break;

    }

    best_solution->CalculeMonoObjectiveTchebycheff();
    my_solution->CalculeMonoObjectiveTchebycheff();
    if(best_solution->objective_funtion < my_solution->objective_funtion){
        *my_solution = *best_solution;
        return true;
    }
    else{
        return false;
    }

}

/*
 * Local search monobjective with first improvement
 */
bool LS_Mono_FI(MonoSolution *my_solution, unsigned op_neighbor){

    MonoSolution *current_solution = new MonoSolution();
    *current_solution = *my_solution;

    switch (op_neighbor) {
        case 0:
            SwapInsideLSMono_FI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 1:
            SwapOutsideLSMono_FI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 2:
            InsertInsideLSMono_FI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 3:
            InsertOutsideLSMono_FI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 4:
            ChangeOpModeLSMono_FI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;
        case 5:
            IntesificationArroyo(current_solution, Instance::num_jobs/10);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                return true;
            }
        break;

    }

    return false;

}

void MOVNS_D(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{

    MonoSolution * current_solution;
    //unsigned index;
    vector<pair<double, double>> W;

    alg_data.num_weights = non_dominated_set.set_solution.size();

    //GenerateWeightVector(W, alg_data.num_weights);

    NDSetSolution<MonoSolution *> B;

    Z_STAR::makespan = UINT_MAX;
    Z_STAR::TEC = INT_MAX;

    for(auto it:non_dominated_set.set_solution){

        //B.set_solution.push_back(it);

        if(it->makeSpan < Z_STAR::makespan)
            Z_STAR::makespan = it->makeSpan;

        if(it->TEC < Z_STAR::TEC)
            Z_STAR::TEC = it->TEC;

    }

    for(auto it:non_dominated_set.set_solution){
        it->CalculeMonoObjectiveTchebycheff();
    }

    current_solution = new MonoSolution ();

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {


        for(unsigned i=0; i<non_dominated_set.set_solution.size();){

            if(non_dominated_set.set_solution[i]->was_visited){
                i++;
            }
            else{

                unsigned r = 0;
                while(r <= alg_data.qtd_neighbor && t1->getElapsedTimeInMilliSec() < alg_data.time_limit){

                    *current_solution = *non_dominated_set.set_solution[i];

                    if(LS_Mono_FI(current_solution, r)){
                    //if(LS_Mono_BI(current_solution, r)){
                        *non_dominated_set.set_solution[i] = *current_solution;
                    }
                    r++;
                }

                non_dominated_set.set_solution[i]->was_visited = true;

            }

        }

        for(unsigned i=0; i<non_dominated_set.set_solution.size();i++){
            non_dominated_set.set_solution[i]->was_visited = false;
        }

        t1->stop();
    }

    delete current_solution;

}
