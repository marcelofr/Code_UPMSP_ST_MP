#include "basic_ls_multi.h"



/*
 * Método para adicionar uma nova solução ao conjunto não-dominado, caso possíveis:
 * 1 - A nova solução é dominada então ela não será adicionada
 * 2 - A nova solução é não-dominada então ela será adicionada
 * 3 - A solução domina então ela será adicionada e as outras serão removidas
 * Se conseguiu adicionar my_solution, então retorna verdadeiro
 */
/*bool AddSolution(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set)
{

    //caso 1
    //bool is_dominated = false;
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end(); ++it_sol){
        //Se my_solution é dominada por alguma solução do conjunto
        if(*it_sol < my_solution || *it_sol == my_solution){
            return false;
        }
    }

    //Caso 3
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end();){
        //Se my_solution domina alguma solução do conjunto
        if(my_solution < *it_sol){
            //Remover essa solução do conjunto
            it_sol = non_dominated_set.erase(it_sol);
        }
        else{
            ++it_sol;
        }
    }
    //Caso 2 e 3
    non_dominated_set.push_back(my_solution);
    return true;
}*/

#include<numeric>

LSSolution *SelectNonDomintatedSolution(NDSetSolution<LSSolution *> &non_dominated_set)
{

    unsigned qtd_nao_visitada = 0;

    LSSolution *cur_solution = NULL;

    vector<int> v(non_dominated_set.set_solution.size());
    iota (std::begin(v), std::end(v), 0);
    random_shuffle( v.begin(), v.end() );

    //for(auto it_sol = non_dominated_set->set_solution.begin(); it_sol != non_dominated_set->set_solution.end();it_sol++){
    //for(auto it_sol : non_dominated_set.set_solution){
    for(auto it: v){
        //if(!((it_sol)->was_visited)){
        if(!(non_dominated_set.set_solution[it]->was_visited)){
            //cout << "Solução não visitada" << endl;

            //Selecionar solução não-visitada
            //cur_solution = (LSSolution*)(*it_sol);
            //cur_solution = it_sol;
            cur_solution = non_dominated_set.set_solution[it];
            break;

        }
        else{
            //cout << "Solução visitada" << endl;
            qtd_nao_visitada++;
        }
    }

    //Se todas as solução já foram visitadas
    if(qtd_nao_visitada == non_dominated_set.set_solution.size()){
        //Marcar todas como não-visitadas
        for(auto it_sol = non_dominated_set.set_solution.begin(); it_sol != non_dominated_set.set_solution.end();it_sol++){
            (*it_sol)->was_visited = false;
        }
        //Selecionar a primeira para continuar a busca
        //cur_solution = *(non_dominated_set.set_solution.begin());
        cur_solution = non_dominated_set.set_solution[v[0]];
    }

    return cur_solution;
}



unsigned SelectNonVisitedSolution(NDSetSolution<LSSolution *> non_dominated_set)
{

    vector<int> v(non_dominated_set.set_solution.size());
    iota (std::begin(v), std::end(v), 0);
    srand(Instance::seed);
    random_shuffle( v.begin(), v.end() );
    for(auto it: v){
        if(!(non_dominated_set.set_solution[it]->was_visited)){
            return it;
        }
    }

    return non_dominated_set.set_solution.size()+1;
}


unsigned ExistNonDomintatedSolution(NDSetSolution<LSSolution *> non_dominated_set)
{

    unsigned qtd_nao_visitada = 0;

    for(auto it_sol : non_dominated_set.set_solution){
        if(!it_sol->was_visited){

            return ++qtd_nao_visitada;

        }
    }

    return qtd_nao_visitada;
}

LSSolution *GenRandomNeighborSol(LSSolution *my_solution)
{

    LSSolution *neighbor_sol = new LSSolution();

    *neighbor_sol = *my_solution;

    unsigned op_mov;
    unsigned i1, i2, j1, j2, mode_op;

    op_mov = rand()%5;

    switch (op_mov) {
        //Troca na mesma máquina
        case 0:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            do{
                j2 = rand()%neighbor_sol->scheduling[i1].size();
            }while(j1==j2);

            //Realizar troca
            neighbor_sol->SwapInside(i1, j1, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Troca em máquinas diferentes
        case 1:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(i1==i2 || neighbor_sol->scheduling[i2].size() < 1);

            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%neighbor_sol->scheduling[i2].size();

            //Realizar troca
            neighbor_sol->SwapOutside(i1, j1, i2, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Inserção na mesma máquina
        case 2:

            //Selecionar a máquina i1 com pelo menos 2 tarefas
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 2);

            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa, diferente da primeira
            do{
                j2 = rand()%neighbor_sol->scheduling[i1].size();
            }while(j1==j2);

            //Realizar inserção
            neighbor_sol->InsertInside(i1, j1, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Inserção em máquinas diferentes
        case 3:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 1);

            //Selecionar máquina i2 com pelo menos 1 tarefa
            do{
                i2 = 1+rand()%Instance::num_machine;
            }while(i1==i2 || neighbor_sol->scheduling[i2].size() < 1);


            //Selecionar a primeira tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar a segunda tarefa
            j2 = rand()%neighbor_sol->scheduling[i2].size();

            //Realizar inserção
            neighbor_sol->InsertOutside(i1, j1, i2, j2);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;

        //Troca de modo de operação
        case 4:

            //Selecionar máquina i1 com pelo menos 1 tarefa
            do {
                i1 = 1+rand()%Instance::num_machine;
            } while (neighbor_sol->scheduling[i1].size() < 1);

            //Selecionar uma tarefa
            j1 = rand()%neighbor_sol->scheduling[i1].size();

            //Selecionar modo de operação
            mode_op = 1+rand()%Instance::num_mode_op;

            //Realizar troca do modo de operação
            neighbor_sol->ChangeModeOpJob(i1, j1, mode_op);

            neighbor_sol->CalculateShorterTimeHorizon();
            neighbor_sol->CalculateObjective();

            break;
    }

    return neighbor_sol;
}

LSSolution *GenNeighborSol(LSSolution *my_solution, unsigned op_neighbor)
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

    return my_solution;
}


LSSolution * Shaking(LSSolution *cur_solution, unsigned op_neighbor, unsigned level)
{

    for(unsigned i=0; i<level; i++){
        //Gerar uma solução vizinha aleatoriamente
        cur_solution = GenNeighborSol(cur_solution, op_neighbor);
    }

    return cur_solution;
}

LSSolution * Destruction(LSSolution * solution, unsigned level){

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

bool Construction(LSSolution * partial_solution, NDSetSolution<LSSolution *> &non_dominated_set, vector<unsigned> removed_jobs){

    LSSolution *reconstructed_solution;
    unsigned size;
    bool improve;
    NDSetSolution<LSSolution *> non_dominated_set_partial;
    NDSetSolution<LSSolution *> non_dominated_set_partial_next;
    reconstructed_solution = new LSSolution();

    non_dominated_set_partial.AddSolution(partial_solution);

    //Para cada tarefa removida
    for(auto it1 : removed_jobs){        
        //Para cada solução parcial
        for (auto it : non_dominated_set_partial_next.set_solution) {
            delete it;
        }
        non_dominated_set_partial_next.set_solution.clear();
        for(auto it2 : non_dominated_set_partial.set_solution){
            //Para cada máquina
            for(unsigned i=1; i<=Instance::num_machine; i++){
                size = it2->scheduling[i].size();
                //Para cada posição
                for(unsigned p=0; p<=size; p++ ){

                    *reconstructed_solution = *it2;
                    reconstructed_solution->AddJob(it1, i, p, 0);

                    //Para cada modo de operação
                    for(unsigned o = 1; o <= Instance::num_mode_op; o++ ){

                        reconstructed_solution->job_mode_op[it1] = o;
                        //reconstructed_solution->CalculateShorterTimeHorizon();
                        reconstructed_solution->CalculateShorterTimeHorizonMachine(i);
                        //reconstructed_solution->CalculateObjective();
                        reconstructed_solution->CalculateObjectiveMachine(i);
                        non_dominated_set_partial_next.AddSolution(reconstructed_solution);

                    }
                }
            }
        }

        for (auto it : non_dominated_set_partial.set_solution) {
            delete it;
        }
        non_dominated_set_partial.set_solution.clear();

        for(auto it: non_dominated_set_partial_next.set_solution){
            non_dominated_set_partial.AddSolution(it);
        }
        // Copying vector by copy function
        //copy(nd_set_partial_temp.set_solution.begin(), nd_set_partial_temp.set_solution.end(), back_inserter(nd_set_partial.set_solution));
        //nd_set_partial.set_solution.insert(nd_set_partial.set_solution.end(), nd_set_partial_temp.set_solution.begin(), nd_set_partial_temp.set_solution.end());
    }

    improve=false;
    for(auto it: non_dominated_set_partial.set_solution){
        if(non_dominated_set.AddSolution(it))
            improve=true;
    }

    for (auto it : non_dominated_set_partial.set_solution) {
        delete it;
    }
    non_dominated_set_partial.set_solution.clear();

    //delete non_dominated_set_partial_b;
    delete reconstructed_solution;

    return improve;

}

bool IntesificationArroyo(LSSolution *current_solution, NDSetSolution<LSSolution *> &non_dominated_set, unsigned level){

    bool improve = false;
    LSSolution *partial_solution;
    //unsigned index;
    //NDSetSolution<LSSolution *> *non_dominated_set_i;
    //NDSetSolution<LSSolution *> non_dominated_set_local;

    //index = rand()%non_dominated_set.set_solution.size();
    partial_solution = new LSSolution();

    *partial_solution = *current_solution;

    partial_solution = Destruction(partial_solution, level);

    vector<unsigned> removed_jobs(partial_solution->removed_jobs);

    partial_solution->removed_jobs.clear();

    //non_dominated_set_local.set_solution.clear();
    //non_dominated_set_local.AddSolution(partial_solution);

    improve=Construction(partial_solution, non_dominated_set, removed_jobs);

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

void SortByMakespanLSSolution(vector<LSSolution*> &set_solution)
{
    //sort(set_solution.begin(), set_solution.end(), CompareMakespanMonoSolution);
    sort(set_solution.begin(), set_solution.end(), CompareMakespan);
    //sort(set_solution.begin(), set_solution.end());
}
