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

    my_solution->CalculeMonoObjectiveTchebycheff();

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
            IntesificationArroyo(current_solution, ceil(Instance::num_jobs/10));
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

    alg_data.num_weights = non_dominated_set.set_solution.size();

    NDSetSolution<MonoSolution *> B;

    Z_STAR::makespan = INT_MAX;
    Z_STAR::TEC = INT_MAX;

    for(auto it:non_dominated_set.set_solution){
        it->CalculeMonoObjectiveTchebycheff();
    }

    //Calcular as distâncias entre as soluções e os pesos
    vector<vector<unsigned>>distance_solution_weighted(alg_data.num_weights, vector<unsigned>(alg_data.num_weights));
    pair<double, double> point;

    for(unsigned i=0; i<alg_data.num_weights; i++){
        for(unsigned j=0; j<alg_data.num_weights; j++){
            point.first = non_dominated_set.set_solution[j]->makeSpan;
            point.second = non_dominated_set.set_solution[j]->TEC;
            distance_solution_weighted[i][j] = CalcDistancePointToLine(point, non_dominated_set.set_solution[i]->weights);
        }
    }

    //Associar as soluções mais próximas para cada peso
    unsigned num_neighboor=1;
    unsigned best_distance, best_index;
    vector<vector<unsigned>>solution_neighboor(alg_data.num_weights, vector<unsigned>(num_neighboor));


    for(unsigned i=0; i<alg_data.num_weights; i++){
        for(unsigned j=0; j<num_neighboor; j++){
            best_distance = INT_MAX;
            best_index = 0;
            for(unsigned k=0; k<alg_data.num_weights; k++){
                if(distance_solution_weighted[i][k] < best_distance){
                    best_distance = distance_solution_weighted[i][k];
                    best_index = k;
                }
            }
            solution_neighboor[i][j] = best_index;
            distance_solution_weighted[i][best_index] = INT_MAX;
        }
    }

    //Em solution_neighboor, temos os n vizinhos mais próximos para cada peso

    current_solution = new MonoSolution ();

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        for(unsigned i=0; i<alg_data.num_weights;i++){

            //Selecionar aleatoriamente um vizinho e B_i
            random_shuffle( solution_neighboor[i].begin(), solution_neighboor[i].end() );

            for(auto it_i:solution_neighboor[i]){
                if(non_dominated_set.set_solution[it_i]->was_visited){
                    it_i++;
                }
                else{

                    //ILS
                    unsigned r = 0;
                    unsigned op_neighboor;
                    bool improve = false;
                    unsigned level = 2;
                    unsigned max_level = 5;
                    while(r <= alg_data.qtd_neighbor && t1->getElapsedTimeInMilliSec() < alg_data.time_limit){

                        *current_solution = *non_dominated_set.set_solution[it_i];

                        op_neighboor = rand()%alg_data.qtd_neighbor;
                        current_solution = Shaking(current_solution, op_neighboor, level);

                        if(LS_Mono_FI(current_solution, r)){
                        //if(LS_Mono_BI(current_solution, r)){

                            non_dominated_set.set_solution[it_i]->CalculeMonoObjectiveTchebycheff();

                            if(current_solution->objective_funtion < non_dominated_set.set_solution[it_i]->objective_funtion){
                                *non_dominated_set.set_solution[it_i] = *current_solution;
                                improve = true;
                            }

                        }
                        r++;
                        t1->stop();
                    }

                    if(!improve){
                        level++;
                        if(level > max_level){
                            level = 2;
                        }
                    }
                    else{
                        level = 2;
                    }

                    for(auto it_j:solution_neighboor[i]){
                        auto w_j = non_dominated_set.set_solution[it_j]->weights;
                        auto obj_i = max(w_j.first*(non_dominated_set.set_solution[it_i]->makeSpan - Z_STAR::makespan),
                                        w_j.second*(non_dominated_set.set_solution[it_i]->TEC - Z_STAR::TEC));
                        auto obj_j = max(w_j.first*(non_dominated_set.set_solution[it_j]->makeSpan - Z_STAR::makespan),
                                        w_j.second*(non_dominated_set.set_solution[it_j]->TEC - Z_STAR::TEC));


                        if(obj_i < obj_j){
                            *non_dominated_set.set_solution[it_j] = *non_dominated_set.set_solution[it_i];
                        }
                    }

                    non_dominated_set.set_solution[it_i]->was_visited = true;

                    break;
                }

            }
        }

        for(unsigned i=0; i<non_dominated_set.set_solution.size();i++){
            non_dominated_set.set_solution[i]->was_visited = false;
        }

        t1->stop();
    }

    delete current_solution;

}
