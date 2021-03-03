#include "basic_local_search.h"


/*
 * Método para imprimir um vetor de soluções
 */
void PrintNonDominatedSet(vector<Solution*> &non_dominated_set)
{
    for(auto it = non_dominated_set.begin(); it != non_dominated_set.end(); ++it){
        (*it)->Print();
    }
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool SwapInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> *non_dominated_set)
{
    unsigned long num_job_maq;

    LSSolution *neighbor_sol = new LSSolution();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_sol->SwapInside(i, j, k);

                neighbor_sol->CalculateShorterTimeHorizon();
                neighbor_sol->CalculateObjective();

                //Tentar adicionar o vizinho gerado no conjunto não-dominado
                //Se conseguiu, então retorna true, senão continua a busca
                //if(AddSolution(neighbor_sol, non_dominated_set)){
                if(non_dominated_set->AddSolution(neighbor_sol)){

                    return true;
                }
                else{
                    *neighbor_sol = *my_solution;
                }
            }
        }
    }

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool SwapOutsideLS_FI(LSSolution* my_solution, NDSetSolution<LSSolution*> *non_dominated_set)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_sol = new LSSolution();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = i1+1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k < num_job_maq2; ++k) {

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_sol->SwapOutside(i1, j, i2, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();

                    //Tentar adicionar o vizinho gerado no conjunto não-dominado
                    //Se conseguiu, então retorna true
                    //if(AddSolution(neighbor_sol, non_dominated_set)){
                    if(non_dominated_set->AddSolution(neighbor_sol)){

                        return true;
                    }
                    else{
                        *neighbor_sol = *my_solution;
                    }

                }
            }
        }
    }

    delete neighbor_sol;
    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas em uma máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool InsertInsideLS_FI(LSSolution* my_solution, NDSetSolution<LSSolution *> *non_dominated_set)
{
    unsigned long num_job_maq;

    LSSolution *neighbor_sol = new LSSolution();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = 0; k < num_job_maq; ++k) {

                if(j != k){
                    //Realizar a inserção
                    neighbor_sol->InsertInside(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();

                    //Tentar adicionar o vizinho gerado no conjunto não-dominado
                    //Se conseguiu, então retorna true
                    //if(AddSolution(neighbor_sol, non_dominated_set)){
                    if(non_dominated_set->AddSolution(neighbor_sol)){

                        return true;
                    }
                    else{
                        *neighbor_sol = *my_solution;
                    }
                }

            }
        }
    }

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas entre máquinas.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool InsertOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution *> *non_dominated_set)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_sol = new LSSolution();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            if(i1 != i2){
                //Para cada tarefa j da máquina i1
                for (unsigned j = 0; j < num_job_maq1; j++) {

                    //Para cada tarefa k da máquina i2
                    for (unsigned k = 0; k <= num_job_maq2; ++k) {

                        //Realizar a troca
                        neighbor_sol->InsertOutside(i1, j, i2, k);

                        neighbor_sol->CalculateShorterTimeHorizon();
                        neighbor_sol->CalculateObjective();

                        //Tentar adicionar o vizinho gerado no conjunto não-dominado
                        //Se conseguiu, então retorna true
                        //if(AddSolution(neighbor_sol, non_dominated_set)){
                        if(non_dominated_set->AddSolution(neighbor_sol)){

                            return true;
                        }
                        else{
                            *neighbor_sol = *my_solution;
                        }
                    }
                }
            }
        }
    }

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool ChangeOpModeLS(LSSolution *my_solution, NDSetSolution<LSSolution *> *non_dominated_set)
{
    unsigned long num_job_maq, job;

    LSSolution *neighbor_sol = new LSSolution();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            job = neighbor_sol->scheduling[i][j];

            //Para cada tarefa k da máquina i
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                if(neighbor_sol->job_mode_op[job] != k){

                    neighbor_sol->ChangeModeOpJob(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();

                    //Tentar adicionar o vizinho gerado no conjunto não-dominado
                    //Se conseguiu, então retorna true
                    //if(AddSolution(neighbor_sol, non_dominated_set)){
                    if(non_dominated_set->AddSolution(neighbor_sol)){

                        return true;
                    }
                    else{
                        *neighbor_sol = *my_solution;
                    }
                }

            }
        }
    }

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

bool ChangeHLS(LSSolution my_solution, vector<LSSolution> &non_dominated_set)
{
    return false;
}

/*
 * Método para adicionar uma nova solução ao conjunto não-dominado, caso possíveis:
 * 1 - A nova solução é dominada então ela não será adicionada
 * 2 - A nova solução é não-dominada então ela será adicionada
 * 3 - A solução domina então ela será adicionada e as outras serão removidas
 * Se conseguiu adicionar my_solution, então retorna verdadeiro
 */
bool AddSolution(LSSolution *my_solution, vector<LSSolution*> &non_dominated_set)
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
}
