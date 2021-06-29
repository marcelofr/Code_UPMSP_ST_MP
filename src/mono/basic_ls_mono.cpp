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

}


void GenerateWeightVector(vector<pair<double, double>> &Weigths, unsigned num_weights){

    Weigths.resize(num_weights);

    unsigned x,y;
    x = 0;
    y = num_weights;


    Weigths[0].first = 1-EPS;
    Weigths[0].second = EPS;

    x++;
    y--;

    for(unsigned h=1;h<num_weights-1 ;h++){
        Weigths[h].first = double(y-1)/double(num_weights-1);
        Weigths[h].second = double(x)/double(num_weights-1);
        x++;
        y--;
    }

    Weigths[num_weights-1].first = EPS;
    Weigths[num_weights-1].second = 1-EPS;
    x++;
    y--;

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

MonoSolution *GenNeighborSol(MonoSolution *my_solution, unsigned op_neighbor)
{

    unsigned i1, i2, j1, j2, mode_op;

    //op_mov
    switch (op_neighbor) {
        //Troca na mesma máquina
        case 0:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            //do{
                j2 = rand()%my_solution->scheduling[i1].size();
            //}while(j1==j2);

            //Realizar troca
            my_solution->SwapInside(i1, j1, j2);

            break;

        //Troca em máquinas diferentes
        case 1:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(my_solution->scheduling[i2].size() < 1);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%my_solution->scheduling[i2].size();

            //Realizar troca
            my_solution->SwapOutside(i1, j1, i2, j2);

            break;

        //Inserção na mesma máquina
        case 2:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            //do{
                j2 = rand()%my_solution->scheduling[i1].size();
            //}while(j1==j2);

            //Realizar inserção
            my_solution->InsertInside(i1, j1, j2);

            break;

        //Inserção em máquinas diferentes
        case 3:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(my_solution->scheduling[i2].size() < 1);

            //Selecionar a primeira tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%my_solution->scheduling[i2].size();

            //Realizar inserção
            if(i1 != i2)
                my_solution->InsertOutside(i1, j1, i2, j2);
            else
                my_solution->InsertInside(i1, j1, j2);

            break;

        //Troca de modo de operação
        case 4:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (my_solution->scheduling[i1].size() < 1);

            //Selecionar uma tarefa
            j1 = rand()%my_solution->scheduling[i1].size();

            //Selecionar modo de operação
            mode_op = 1+rand()%Instance::num_mode_op;

            //Realizar troca do modo de operação
            my_solution->ChangeModeOpJob(i1, j1, mode_op);

            break;
    }

    my_solution->CalculateShorterTimeHorizon();
    my_solution->CalculateObjective();
    my_solution->CalculeMonoObjectiveTchebycheff();

    return my_solution;
}


MonoSolution * Shaking(MonoSolution *cur_solution, unsigned op_neighbor, unsigned level)
{

    for(unsigned i=0; i<level; i++){
        //Gerar uma solução vizinha aleatoriamente
        cur_solution = GenNeighborSol(cur_solution, op_neighbor);
    }

    return cur_solution;
}
