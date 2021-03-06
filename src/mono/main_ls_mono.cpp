#include "main_ls_mono.h"

/*
 * Local search monobjective with best improvement
 */
bool LS_Mono_BI(MonoSolution *my_solution, unsigned op_neighbor){

    MonoSolution *current_solution = new MonoSolution();

    *current_solution = *my_solution;

    bool improve = false;

    switch (op_neighbor) {
        case 0:
            SwapInsideLSMono_BI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 1:
            SwapOutsideLSMono_BI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 2:
            InsertInsideLSMono_BI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 3:
            InsertOutsideLSMono_BI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;
        case 4:
            ChangeOpModeLSMono_BI(current_solution);
            my_solution->CalculeMonoObjectiveTchebycheff();
            current_solution->CalculeMonoObjectiveTchebycheff();
            if(current_solution->objective_funtion < my_solution->objective_funtion){
                *my_solution = *current_solution;
                improve = true;
            }
        break;

    }

    return improve;

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

        break;

    }

    return false;

}

void MOVNS_D(NDSetSolution<MonoSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{

    MonoSolution * current_solution;

    vector<pair<double, double>> Weights;
    GenerateWeightVector(Weights, alg_data.num_weights);

    Z_STAR::makespan = INT_MAX;
    Z_STAR::TEC = INT_MAX;

    for(auto it:non_dominated_set.set_solution){
        if(it->makeSpan < Z_STAR::makespan){
            Z_STAR::makespan = it->makeSpan;
        }

        if(it->TEC < Z_STAR::TEC){
            Z_STAR::TEC = it->TEC;
        }
    }

    for(auto it:non_dominated_set.set_solution){
        it->CalculeMonoObjectiveTchebycheff();
    }

    //Calcular as distâncias entre os pesos par a par
    vector<vector<double>>distance_weighted(alg_data.num_weights, vector<double>(alg_data.num_weights));
    for(unsigned i=0; i<alg_data.num_weights; i++){
        for(unsigned j=0; j<alg_data.num_weights; j++){
            distance_weighted[i][j] = CalcDistanceWeightedToWeighted(Weights[i], Weights[j]);
        }
    }

    //Associar as soluções mais próximas para cada peso
    unsigned best_index;
    double best_distance;
    vector<vector<unsigned>>solution_neighboor(alg_data.num_weights, vector<unsigned>(alg_data.num_group));

    //Associar cada peso a um conjunto j de pesos
    for(unsigned i=0; i<alg_data.num_weights; i++){
        for(unsigned k=0; k<alg_data.num_group; k++){
            best_distance = INT_MAX;
            best_index = 0;
            for(unsigned j=0; j<alg_data.num_weights; j++){
                if(distance_weighted[i][j] < best_distance){
                    best_distance = distance_weighted[i][j];
                    best_index = j;
                }
            }
            solution_neighboor[i][k] = best_index;
            distance_weighted[i][best_index] = INT_MAX;
        }
    }

    //Em solution_neighboor, temos os n vizinhos mais próximos para cada peso

    current_solution = new MonoSolution ();

    unsigned index, op;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        for(unsigned i=0; i<alg_data.num_weights;i++){

            //Selecionar aleatoriamente um vizinho e B_i
            //random_shuffle( solution_neighboor[i].begin(), solution_neighboor[i].end() );

            index = rand()%solution_neighboor[i].size();
            op = solution_neighboor[i][index];

            //ILS
            unsigned r = 0;
            unsigned op_neighboor;
            bool improve = false;
            unsigned level = 2;
            unsigned max_level = 5;

            //*current_solution = *non_dominated_set.set_solution[it_i];
            *current_solution = *non_dominated_set.set_solution[op];

            op_neighboor = rand()%alg_data.qtd_neighbor;
            current_solution = Shaking(current_solution, op_neighboor, level);

            while(r <= alg_data.qtd_neighbor && t1->getElapsedTimeInMilliSec() < alg_data.time_limit){

                if(r < alg_data.qtd_neighbor){

                    //if(LS_Mono_FI(current_solution, r)){
                    if(LS_Mono_BI(current_solution, r)){

                        non_dominated_set.set_solution[op]->CalculeMonoObjectiveTchebycheff();

                        if(current_solution->objective_funtion - non_dominated_set.set_solution[op]->objective_funtion < -0.01){
                        //if(current_solution < non_dominated_set.set_solution[op]){
                            *non_dominated_set.set_solution[op] = *current_solution;
                            improve = true;
                            //r = 0;
                        }

                    }
                }
                else{

                    IntesificationArroyo(current_solution, ceil(double(Instance::num_jobs)/double(10)));

                    non_dominated_set.set_solution[op]->CalculeMonoObjectiveTchebycheff();

                    //Se current_solution é melhor que non_dominated_set.set_solution[op]
                    if(current_solution->objective_funtion - non_dominated_set.set_solution[op]->objective_funtion < -EPS){
                    //if(current_solution < non_dominated_set.set_solution[op]){
                        *non_dominated_set.set_solution[op] = *current_solution;
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
                auto obj_i = max(w_j.first*(non_dominated_set.set_solution[op]->makeSpan - Z_STAR::makespan),
                                w_j.second*(non_dominated_set.set_solution[op]->TEC - Z_STAR::TEC));
                auto obj_j = max(w_j.first*(non_dominated_set.set_solution[it_j]->makeSpan - Z_STAR::makespan),
                                w_j.second*(non_dominated_set.set_solution[it_j]->TEC - Z_STAR::TEC));


                //if(obj_i < obj_j){
                if(obj_i - obj_j < -EPS){
                    *non_dominated_set.set_solution[it_j] = *non_dominated_set.set_solution[op];
                    non_dominated_set.set_solution[it_j]->weights = w_j;
                }
            }

        }

        t1->stop();
    }

    delete current_solution;

}
