#include "basic_ls_mono.h"


bool CompareMakespanMonoSolution(MonoSolution *& l, MonoSolution *& r) //(2)
{
    /*if(*l < *r){
        return true;
    }
    else if (*r < *l){
        return false;
    }
    else{
        if(*l < *r){
            return true;
        }
        else{
            return false;
        }
    }*/

    if(l->makeSpan < r->makeSpan){
        return true;
    }
    else if (l->makeSpan > r->makeSpan){
        return false;
    }
    else{
        if(l->TEC < r->TEC){
            return true;
        }
        else{
            return false;
        }
    }

}

void SortByMakespanMonoSolution(vector<MonoSolution*> &set_solution)
{
    //sort(set_solution.begin(), set_solution.end(), CompareMakespanMonoSolution);
    sort(set_solution.begin(), set_solution.end(), CompareMakespanMonoSolution);
    //sort(set_solution.begin(), set_solution.end());
}

void SetWeights(NDSetSolution<MonoSolution *> non_dominated_set){

    unsigned qtd_solutions;
    qtd_solutions = non_dominated_set.set_solution.size();

    //W.resize(num_weights);

    unsigned x,y;
    x = 0;
    y = qtd_solutions;


    non_dominated_set.set_solution[0]->weights.first = 1-EPS;
    non_dominated_set.set_solution[0]->weights.second = EPS;

    x++;
    y--;

    for(unsigned h=1;h<qtd_solutions-1 ;h++){        
        non_dominated_set.set_solution[h]->weights.first = double(y-1)/double(qtd_solutions-1);
        non_dominated_set.set_solution[h]->weights.second = double(x)/double(qtd_solutions-1);
        x++;
        y--;
    }

    non_dominated_set.set_solution[qtd_solutions-1]->weights.first = EPS;
    non_dominated_set.set_solution[qtd_solutions-1]->weights.second = 1-EPS;
    x++;
    y--;

    /*unsigned qtd_solutions;

    qtd_solutions = non_dominated_set.set_solution.size();

    non_dominated_set.set_solution.front()->weights.first = 0;
    non_dominated_set.set_solution.front()->weights.second = 1;

    non_dominated_set.set_solution.back()->weights.first = 1;
    non_dominated_set.set_solution.back()->weights.second = 0;*/

    /*for(auto it=non_dominated_set.set_solution.begin()+1; it<non_dominated_set.set_solution.end()-1; ++it){
        (*it)->weights.first = 1/(qtd_solutions-1);
        (*it)->weights.second = 1/2;

    }*/
}


void GenerateWeightVector(vector<pair<double, double>> &W, unsigned num_weights){

    W.resize(num_weights);

    unsigned x,y;
    x = 0;
    y = num_weights;
    for(unsigned h=0;h<num_weights ;h++){
        W[h].first = double(x)/double(num_weights-1);
        W[h].second = double(y-1)/double(num_weights-1);
        x++;
        y--;
    }

}

MonoSolution * Destruction(MonoSolution * solution, unsigned level){

    //unsigned job_position, max_ct, machine_max_ct, min_ct, machine_min_ct;
    unsigned machine_makespan;
    unsigned machine, job_position;
    vector<unsigned> removed_jobs;


    //Remover as tarefas da máquina com o makespan
    unsigned i;
    i=0;
    machine_makespan = solution->GetMakespanMachine();
    for (;i < level && solution->scheduling[machine_makespan].size()>0 ;i++ ) {

        //job_position = rand()%solution->scheduling[machine_makespan].size();
        //solution->RemovingJob(machine_makespan, job_position);

        solution->RemovingJob(machine_makespan, solution->scheduling[machine_makespan].size()-1);

    }

    //solution->CalculateShorterTimeHorizon();
    //solution->CalculateObjective();


    for (;i < level;i++ ) {

        do{
            machine = 1 + rand()%Instance::num_machine;
        }while(solution->scheduling[machine].size() == 0);

        job_position = rand()%solution->scheduling[machine].size();

        solution->RemovingJob(machine, job_position);

    }

    solution->CalculateShorterTimeHorizon();
    solution->CalculateObjective();

    return solution;
}

void Construction(MonoSolution * partial_solution, vector<unsigned> removed_jobs){

    MonoSolution *reconstructed_solution;
    MonoSolution *best_reconstructed_solution = new MonoSolution();
    unsigned size;
    reconstructed_solution = new MonoSolution();

    *best_reconstructed_solution = *partial_solution;

    //Para cada tarefa removida
    for(auto it1 : removed_jobs){

        //for(auto it2 : non_dominated_set_partial.set_solution){
        *reconstructed_solution = *best_reconstructed_solution;
        best_reconstructed_solution->makeSpan = best_reconstructed_solution->TEC = INT_MAX;

            //Para cada máquina
            for(unsigned i=1; i<=Instance::num_machine; i++){
                size = reconstructed_solution->scheduling[i].size();
                //Para cada posição
                for(unsigned p=0; p<=size; p++ ){

                    reconstructed_solution->AddJob(it1, i, p, 0);

                    //Para cada modo de operação
                    for(unsigned o = 1; o <= Instance::num_mode_op; o++ ){

                        reconstructed_solution->job_mode_op[it1] = o;
                        //reconstructed_solution->CalculateShorterTimeHorizon();
                        reconstructed_solution->CalculateShorterTimeHorizonMachine(i);
                        //reconstructed_solution->CalculateObjective();

                        reconstructed_solution->CalculateObjectiveMachine(i);
                        //non_dominated_set_partial_next.AddSolution(reconstructed_solution);

                        reconstructed_solution->CalculeMonoObjective();
                        best_reconstructed_solution->CalculeMonoObjective();

                        if(reconstructed_solution->objective_funtion < best_reconstructed_solution->objective_funtion){
                            *best_reconstructed_solution = *reconstructed_solution;
                        }

                    }

                    reconstructed_solution->RemovingJob(i, p);
                }
            }
        //}

    }

    *partial_solution = *best_reconstructed_solution;

    partial_solution->CalculeMonoObjectiveTchebycheff();


    //delete non_dominated_set_partial_b;
    delete reconstructed_solution;
    delete best_reconstructed_solution;

}

bool IntesificationArroyo(MonoSolution *current_solution, unsigned level){

    bool improve = false;
    MonoSolution *partial_solution;
    //unsigned index;
    //NDSetSolution<LSSolution *> *non_dominated_set_i;
    //NDSetSolution<LSSolution *> non_dominated_set_local;

    //index = rand()%non_dominated_set.set_solution.size();
    partial_solution = new MonoSolution();

    *partial_solution = *current_solution;

    partial_solution = Destruction(partial_solution, level);

    vector<unsigned> removed_jobs(partial_solution->removed_jobs);

    partial_solution->removed_jobs.clear();

    //non_dominated_set_local.set_solution.clear();
    //non_dominated_set_local.AddSolution(partial_solution);

    Construction(partial_solution, removed_jobs);

    *current_solution = *partial_solution;

    /*for(auto it : non_dominated_set_local.set_solution){
        if(non_dominated_set.AddSolution(it))
            improve = true;
    }

    for(auto it : non_dominated_set_local.set_solution){
        delete it;
    }*/

    delete partial_solution;

    return improve;

}
