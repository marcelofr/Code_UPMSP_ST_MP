#include "local_search.h"

void LS_FI_OP(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out, unsigned op_neighbor)
{

    for(auto it : non_dominated_set_in.set_solution){

        switch (op_neighbor) {
        case 0:
            InsertInsideLS_FI(it, non_dominated_set_out);
            break;
        case 1:
            SwapInsideLS_FI(it, non_dominated_set_out);

            break;
        case 2:
            ChangeOpModeLS_FI(it, non_dominated_set_out);
            break;
        case 3:
            InsertOutsideLS_FI(it, non_dominated_set_out);
            break;
        case 4:
            SwapOutsideLS_FI(it, non_dominated_set_out);
            break;
        default:
            break;
        }
    }

}

bool VND_FI(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out)
{

    bool improve=false;

    NDSetSolution<LSSolution *> non_dominated_set_local;
    for(auto it: non_dominated_set_out.set_solution){
        non_dominated_set_local.AddSolution(it);
    }

    for(auto it : non_dominated_set_in.set_solution){
        if(SwapInsideLS_FI(it, non_dominated_set_local)){
            improve = true;
            break;
        }
        if(SwapOutsideLS_FI(it, non_dominated_set_local)){
            improve = true;
            break;
        }
        if(InsertInsideLS_FI(it, non_dominated_set_local)){
            improve = true;
            break;
        }
        if(InsertOutsideLS_FI(it, non_dominated_set_local)){
            improve = true;
            break;
        }
        if(ChangeOpModeLS_FI(it, non_dominated_set_local)){
            improve = true;
            break;
        }
    }

    for(auto it: non_dominated_set_local.set_solution){
        non_dominated_set_out.AddSolution(it);
        delete it;
    }

    return improve;

}

bool LS_BI_OP(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out, unsigned op_neighbor)
{

    NDSetSolution<LSSolution *> non_dominated_set_local;
    bool improve = false;

    for(auto it: non_dominated_set_in.set_solution){
        non_dominated_set_local.AddSolution(it);
    }

    for(auto it : non_dominated_set_in.set_solution){

        switch (op_neighbor) {
        case 0:
            SwapInsideLS_BI(it, non_dominated_set_local);
            break;
        case 1:
            InsertInsideLS_BI(it, non_dominated_set_local);
            break;
        case 2:
            SwapOutsideLS_BI(it, non_dominated_set_local);
            break;
        case 3:
            InsertOutsideLS_BI(it, non_dominated_set_local);
            break;
        case 4:
            ChangeOpModeLS_BI(it, non_dominated_set_local);
            break;
        default:
            break;
        }
    }

    for(auto it: non_dominated_set_local.set_solution){
        if(non_dominated_set_out.AddSolution(it))
            improve = true;
        delete it;
    }

    return improve;
}

bool VND_BI(NDSetSolution<LSSolution *> non_dominated_set_in, NDSetSolution<LSSolution *> &non_dominated_set_out)
{

    NDSetSolution<LSSolution *> non_dominated_set_local;
    for(auto it: non_dominated_set_in.set_solution){
        non_dominated_set_local.AddSolution(it);
    }

    bool improve=false;

    for(auto it : non_dominated_set_in.set_solution){

        if(SwapInsideLS_BI(it, non_dominated_set_local)
                || SwapOutsideLS_BI(it, non_dominated_set_local)
                || InsertInsideLS_BI(it, non_dominated_set_local)
                || InsertOutsideLS_BI(it, non_dominated_set_local)
                || ChangeOpModeLS_BI(it, non_dominated_set_local))
        {
            improve=true;
            break;
        }

    }

    for(auto it: non_dominated_set_local.set_solution){
        non_dominated_set_out.AddSolution(it);
        delete it;
    }

    return improve;
}

void HillClimbing(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
//void HillClimbing(NDSetSolution<LSSolution *> &non_dominated_set)
{

    //NonDominatedSetSolution cur_non_dominated_set;
    LSSolution *current_solution = new LSSolution();
    unsigned index_solution = 0;

    while(t1->getElapsedTimeInMilliSec() < alg_data.time_limit){
    //while(true){

        t1->stop();

        index_solution = SelectNonVisitedSolution(non_dominated_set);

        if(index_solution < non_dominated_set.set_solution.size()){
            *current_solution = *non_dominated_set.set_solution[index_solution];
        }
        else{
            break;
        }

        //*current_solution = *select_solution;

        //Realiza busca local na solução corrente (inserção de tarefas na mesma máquina)
        //Se houve melhora
        //if(InsertInsideLS_FI(current_solution, non_dominated_set)){
        if(InsertInsideLS_BI(current_solution, non_dominated_set)){
            continue;
        }

        //Realiza busca local na solução corrente (troca de tarefas na mesma máquina)
        //Se houve melhora
        //else if(SwapInsideLS_FI(current_solution, non_dominated_set)){
        else if(SwapInsideLS_BI(current_solution, non_dominated_set)){
            continue;
        }

        //Realiza busca local na solução corrente (troca de tarefas entre máquinas)
        //Se houve melhora
        //else if(SwapOutsideLS_FI(current_solution, non_dominated_set)){
        else if(SwapOutsideLS_BI(current_solution, non_dominated_set)){
            continue;
        }

        //Realiza busca local na solução corrente (troca do modo de operação da tarefa)
        //Se houve melhora
        //else if(ChangeOpModeLS_FI(current_solution, non_dominated_set)){
        else if(ChangeOpModeLS_BI(current_solution, non_dominated_set)){
            continue;
        }


        //Realiza busca local na solução corrente (inserção de tarefas entre máquinas)
        //Se houve melhora
        else if(InsertOutsideLS_BI(current_solution, non_dominated_set)){ //1355 s
        //else if(InsertOutsideLS_FI(current_solution, non_dominated_set)){
            continue;
        }

        else if(IntesificationArroyo(current_solution, non_dominated_set, int(Instance::num_jobs*0.1))){
            continue;
        }

        else{
            //current_solution->was_visited = true;
            non_dominated_set.set_solution[index_solution]->was_visited = true;
        }

        t1->stop();

    }

    delete current_solution;

}

void MORVNS(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{

    /*LSSolution *cur_solution, *nei_solution;

    unsigned op_neighbor;
    bool improve;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        improve = false;

        //Selecionar uma solução s não-visitada
        cur_solution = SelectNonDomintatedSolution(non_dominated_set);

        //Selecionar uma estrutura de vizinhança
        op_neighbor=rand()%alg_data.qtd_neighbor;

        //Perturbar a solução corrente, com dois movimentos aleatórios
        nei_solution = Shaking(cur_solution, op_neighbor, 2);

        //Fazer busca local na estrutura de vizinhança selecionada
        switch (op_neighbor) {
            case 0:
                improve = SwapInsideLS_FI(nei_solution, non_dominated_set);
                break;
            case 1:
                improve = SwapOutsideLS_FI(nei_solution, non_dominated_set);
                break;
            case 2:
                improve = InsertInsideLS_FI(nei_solution, non_dominated_set);
                break;
            case 3:
                improve = InsertOutsideLS_FI(nei_solution, non_dominated_set);
                break;
            case 4:
                improve = ChangeOpModeLS_FI(nei_solution, non_dominated_set);
                break;
        }

        if(!improve){
            //Marcar s como visitada
            cur_solution->was_visited = true;
        }

        t1->stop();
    }*/
}

void MOVNS(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{
    NDSetSolution<LSSolution *> nd_set_solution_shaked;
    unsigned op_neighbor;
    LSSolution *current_solution;
    LSSolution *shaked_solution;

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        //Selecionar aleatoriamente uma estrutura de vizinhança
        op_neighbor=rand()%alg_data.qtd_neighbor;

        //Shaking
        current_solution = SelectNonDomintatedSolution(non_dominated_set);
        current_solution->was_visited = true;

        shaked_solution = new LSSolution ();
        *shaked_solution = *current_solution;
        shaked_solution = Shaking(shaked_solution, op_neighbor, 1);
        /*shaked_solution->CalculateShorterTimeHorizon();
        shaked_solution->CalculateObjective();*/

        //Local search
        for(auto it : nd_set_solution_shaked.set_solution){
            delete it;
        }
        nd_set_solution_shaked.set_solution.clear();
        nd_set_solution_shaked.AddSolution(shaked_solution);
        delete shaked_solution;

        LS_BI_OP(nd_set_solution_shaked, non_dominated_set, op_neighbor);

        t1->stop();

    }
}

void MOVNS_Arroyo(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{

    NDSetSolution<LSSolution *> nd_set_solution_shaked;
    unsigned op_neighbor, index;
    LSSolution *current_solution;
    LSSolution *shaked_solution;

    shaked_solution = new LSSolution ();

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        //Selecionar aleatoriamente uma estrutura de vizinhança
        op_neighbor=rand()%alg_data.qtd_neighbor;

        //Shaking
        current_solution = SelectNonDomintatedSolution(non_dominated_set);
        current_solution->was_visited = true;

        *shaked_solution = *current_solution;
        shaked_solution = Shaking(shaked_solution, op_neighbor, 1);

        //Local search
        for(auto it : nd_set_solution_shaked.set_solution){
            delete it;
        }
        nd_set_solution_shaked.set_solution.clear();
        nd_set_solution_shaked.AddSolution(shaked_solution);


        LS_BI_OP(nd_set_solution_shaked, non_dominated_set, op_neighbor);
        //LS_FI_OP(nd_set_solution_shaked, non_dominated_set, op_neighbor);

        //Intensification
        index = rand()%non_dominated_set.set_solution.size();
        *shaked_solution = *non_dominated_set.set_solution[index];
        IntesificationArroyo(shaked_solution, non_dominated_set, int(Instance::num_jobs*1.0));

        t1->stop();

    }

    delete shaked_solution;

}

void MOVNS_Eduardo(NDSetSolution<LSSolution *> &non_dominated_set, algorithm_data alg_data, Timer *t1)
{

    NDSetSolution<LSSolution *> nd_set_solution_shaked;
    NDSetSolution<LSSolution *> non_dominated_set_local;
    unsigned op_neighbor, shake_level, index, r;
    LSSolution * shaked_solution;
    bool improve;

    shake_level = 2;

    shaked_solution = new LSSolution ();

    while (t1->getElapsedTimeInMilliSec() < alg_data.time_limit) {

        //Selecionar uma estrutura de vizinhança
        op_neighbor=rand()%alg_data.qtd_neighbor;

        //Shaking
        for(auto it : non_dominated_set.set_solution){
            *shaked_solution = *it;

            shaked_solution = Shaking(shaked_solution, op_neighbor, shake_level);

            nd_set_solution_shaked.AddSolution(shaked_solution);
        }


        /*//Local search
        //improve = VND_FI(nd_set_solution_shaked, non_dominated_set);
        //improve = VND_BI(nd_set_solution_shaked, non_dominated_set);
        for(unsigned i=0; i<alg_data.qtd_neighbor; i++){
            improve = LS_BI_OP(nd_set_solution_shaked, non_dominated_set_local, i);
        }*/
        r = 0;
        //improve = false;
        while(r < alg_data.qtd_neighbor+1){
            if(r < alg_data.qtd_neighbor){
                LS_BI_OP(nd_set_solution_shaked, non_dominated_set_local, r);
                //LS_FI_OP(nd_set_solution_shaked, non_dominated_set_local, r);
            }
            else{
                //Intensification
                index = rand()%nd_set_solution_shaked.set_solution.size();
                *shaked_solution = *nd_set_solution_shaked.set_solution[index];
                //IntesificationArroyo(shaked_solution, non_dominated_set, int(Instance::num_jobs*0.1));
                IntesificationArroyo(shaked_solution, non_dominated_set_local, int(Instance::num_jobs*0.1));
            }

            improve = false;
            for(auto it:non_dominated_set_local.set_solution){
                if(non_dominated_set.AddSolution(it))
                    improve = true;
            }

            if(improve){
                r=0;
            }
            else{
                r++;
            }
        }


        for(auto it : nd_set_solution_shaked.set_solution){
            delete it;
        }
        nd_set_solution_shaked.set_solution.clear();

        //Level control
        /*if(!improve){

            //Intensification
            index = rand()%non_dominated_set.set_solution.size();
            *shaked_solution = *non_dominated_set.set_solution[index];
            //IntesificationArroyo(shaked_solution, non_dominated_set, int(Instance::num_jobs*0.1));
            improve = IntesificationArroyo(shaked_solution, non_dominated_set, int(Instance::num_jobs*0.1));*/

            //if(!improve){
                shake_level++;
                if(shake_level >= alg_data.max_shake_level){
                    shake_level = 2;
                }
            //}
        //}

        t1->stop();
    }

    delete shaked_solution;
}
