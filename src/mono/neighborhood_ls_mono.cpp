#include "neighborhood_ls_mono.h"

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool SwapInsideLSMono_FI(MonoSolution *my_solution)
{
    unsigned long num_job_maq;

    MonoSolution *neighbor_sol = new MonoSolution();

    my_solution->CalculeMonoObjectiveTchebycheff();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = my_solution->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_sol->SwapInsideDelta(i, j, k);
                neighbor_sol->CalculeMonoObjectiveTchebycheff();

                my_solution->CalculeMonoObjectiveTchebycheff();

                //Verifica se a solução vizinha é melhor que a solução corrente
                //Caso afirmativo, então retorna true, senão continua a busca
                if(neighbor_sol->objective_funtion < my_solution->objective_funtion){
                    *my_solution = *neighbor_sol;
                    delete neighbor_sol;
                    return true;
                }

                //Defaz a alteração
                neighbor_sol->SwapInsideDelta(i, k, j);

            }

            neighbor_sol->CalculateShorterTimeHorizonMachine(i);
            neighbor_sol->CalculateObjectiveMachine(i);
        }
    //}

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto percorrer toda a vizinhança (best improvement)
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool SwapInsideLSMono_BI(MonoSolution *my_solution)
{
    unsigned long num_job_maq;

    MonoSolution *neighbor_sol = new MonoSolution () ;
    MonoSolution *best_sol = new MonoSolution () ;
    bool improve = false;

    my_solution->CalculeMonoObjectiveTchebycheff();

    //Criar uma cópia da solução
    *best_sol = *neighbor_sol = *my_solution;

    improve = false;
    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();{
        num_job_maq = my_solution->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Criar uma cópia da solução
                //*neighbor_sol = *my_solution;

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_sol->SwapInsideDelta(i, j, k);

                neighbor_sol->CalculeMonoObjectiveTchebycheff();

                best_sol->CalculeMonoObjectiveTchebycheff();

                //Verifica se a solução vizinha é melhor que a solução corrente
                //Caso afirmativo, então retorna true, senão continua a busca
                if(neighbor_sol->objective_funtion < best_sol->objective_funtion){
                    *best_sol = *neighbor_sol;
                }

                neighbor_sol->SwapInsideDelta(i, k, j);
            }

            neighbor_sol->CalculateShorterTimeHorizonMachine(i);
            neighbor_sol->CalculateObjectiveMachine(i);
        }
    }

    my_solution->CalculeMonoObjectiveTchebycheff();
    best_sol->CalculeMonoObjectiveTchebycheff();

    if(best_sol->objective_funtion < my_solution->objective_funtion){
        *my_solution = *best_sol;
        improve=true;
    }


    delete best_sol;
    delete neighbor_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquinas
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool SwapOutsideLSMono_FI(MonoSolution *my_solution)
{
    unsigned long num_job_maq1, num_job_maq2;

    MonoSolution *neighbor_sol = new MonoSolution();

    my_solution->CalculeMonoObjectiveTchebycheff();

    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = my_solution->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        //for (unsigned i2 = i1+1; i2 <= Instance::num_machine; i2++) {
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {

            if(i1 != i2){
                num_job_maq2 = my_solution->scheduling[i2].size();

                //Para cada tarefa j da máquina i1
                for (unsigned j = 0; j < num_job_maq1; j++) {

                    //Para cada tarefa k da máquina i2
                    for (unsigned k = 0; k < num_job_maq2; ++k) {

                        //Gerar vizinho com a troca de tarefas entre máquinas
                        neighbor_sol->SwapOutsideDelta(i1, j, i2, k);
                        neighbor_sol->CalculeMonoObjectiveTchebycheff();

                        my_solution->CalculeMonoObjectiveTchebycheff();

                        //Verifica se a solução vizinha é melhor que a solução corrente
                        //Caso afirmativo, então retorna true, senão continua a busca
                        if(neighbor_sol->objective_funtion < my_solution->objective_funtion){
                            *my_solution = *neighbor_sol;
                            delete neighbor_sol;
                            return true;
                        }

                        neighbor_sol->SwapOutsideDelta(i2, k, i1, j);
                    }

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                    neighbor_sol->CalculateObjectiveMachine(i1);

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                    neighbor_sol->CalculateObjectiveMachine(i2);
                }
            }

        }
    //}

    delete neighbor_sol;
    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquina.
 * Ele gera novos vizinhos e para quanto percorrer toda a vizinhança (best improvement)
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool SwapOutsideLSMono_BI(MonoSolution *my_solution)
{
    unsigned long num_job_maq1, num_job_maq2;

    MonoSolution *neighbor_sol = new MonoSolution();
    MonoSolution *best_sol = new MonoSolution () ;
    bool improve = false;

    my_solution->CalculeMonoObjectiveTchebycheff();

    *best_sol = *neighbor_sol = *my_solution;


    improve = false;
    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine-1; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();{
        num_job_maq1 = my_solution->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = my_solution->scheduling[i2].size();

            if(i1 == i2){
                continue;
            }

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k < num_job_maq2; ++k) {

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_sol->SwapOutsideDelta(i1, j, i2, k);

                    neighbor_sol->CalculeMonoObjectiveTchebycheff();

                    best_sol->CalculeMonoObjectiveTchebycheff();

                    //Verifica se a solução vizinha é melhor que a solução corrente
                    //Caso afirmativo, então retorna true, senão continua a busca
                    if(neighbor_sol->objective_funtion < best_sol->objective_funtion){
                        *best_sol = *neighbor_sol;

                    }

                    neighbor_sol->SwapOutsideDelta(i2, k, i1, j);
                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                neighbor_sol->CalculateObjectiveMachine(i1);

                neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                neighbor_sol->CalculateObjectiveMachine(i2);
            }
        }
    }

    my_solution->CalculeMonoObjectiveTchebycheff();
    best_sol->CalculeMonoObjectiveTchebycheff();

    if(best_sol->objective_funtion < my_solution->objective_funtion){
        *my_solution = *best_sol;
        improve=true;
    }

    delete neighbor_sol;
    delete best_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização de inserção de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool InsertInsideLSMono_FI(MonoSolution *my_solution)
{
    unsigned long num_job_maq;

    MonoSolution *neighbor_sol = new MonoSolution();

    my_solution->CalculeMonoObjectiveTchebycheff();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = my_solution->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_sol->InsertInsideDelta(i, j, k);
                neighbor_sol->CalculeMonoObjectiveTchebycheff();

                my_solution->CalculeMonoObjectiveTchebycheff();

                //Verifica se a solução vizinha é melhor que a solução corrente
                //Caso afirmativo, então retorna true, senão continua a busca
                if(neighbor_sol->objective_funtion < my_solution->objective_funtion){
                    *my_solution = *neighbor_sol;
                    delete neighbor_sol;
                    return true;
                }

                //Defaz a alteração
                neighbor_sol->InsertInsideDelta(i, k, j);

            }

            neighbor_sol->CalculateShorterTimeHorizonMachine(i);
            neighbor_sol->CalculateObjectiveMachine(i);
        }
    //}

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserção de tarefas em uma máquina.
 * Ele gera novos vizinhos e para quanto percorrer toda a vizinhança (best improvement)
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool InsertInsideLSMono_BI(MonoSolution *my_solution)
{
    unsigned long num_job_maq;

    MonoSolution *neighbor_sol = new MonoSolution () ;
    MonoSolution *best_sol = new MonoSolution () ;
    bool improve = false;

    my_solution->CalculeMonoObjectiveTchebycheff();

    //Criar uma cópia da solução
    *best_sol = *neighbor_sol = *my_solution;

    improve = false;
    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_sol->scheduling[i].size();{

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Criar uma cópia da solução
                //*neighbor_sol = *my_solution;

                //Gerar vizinho com a troca de tarefas em uma máquina
                neighbor_sol->InsertInsideDelta(i, j, k);

                neighbor_sol->CalculeMonoObjectiveTchebycheff();

                best_sol->CalculeMonoObjectiveTchebycheff();

                //Verifica se a solução vizinha é melhor que a solução corrente
                //Caso afirmativo, então retorna true, senão continua a busca
                if(neighbor_sol->objective_funtion < best_sol->objective_funtion){
                    *best_sol = *neighbor_sol;

                }

                neighbor_sol->InsertInsideDelta(i, k, j);
            }

            neighbor_sol->CalculateShorterTimeHorizonMachine(i);
            neighbor_sol->CalculateObjectiveMachine(i);
        }
    }

    best_sol->CalculeMonoObjectiveTchebycheff();
    my_solution->CalculeMonoObjectiveTchebycheff();

    if(best_sol->objective_funtion < my_solution->objective_funtion){
        *my_solution = *best_sol;
        improve=true;
    }

    delete neighbor_sol;
    delete best_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização de inserção de tarefas entre máquinas
 * Ele gera novos vizinhos e para quanto encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou quando percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool InsertOutsideLSMono_FI(MonoSolution *my_solution)
{
    unsigned long num_job_maq1, num_job_maq2;

    MonoSolution *neighbor_sol = new MonoSolution();

    my_solution->CalculeMonoObjectiveTchebycheff();

    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = my_solution->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        //for (unsigned i2 = i1+1; i2 <= Instance::num_machine; i2++) {
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {

            if(i1 != i2){
                num_job_maq2 = my_solution->scheduling[i2].size();

                //Para cada tarefa j da máquina i1
                for (unsigned j = 0; j < num_job_maq1; j++) {

                    //Para cada tarefa k da máquina i2
                    for (unsigned k = 0; k <= num_job_maq2; ++k) {

                        //Gerar vizinho com a troca de tarefas entre máquinas
                        neighbor_sol->InsertOutsideDelta(i1, j, i2, k);
                        neighbor_sol->CalculeMonoObjectiveTchebycheff();

                        my_solution->CalculeMonoObjectiveTchebycheff();

                        //Verifica se a solução vizinha é melhor que a solução corrente
                        //Caso afirmativo, então retorna true, senão continua a busca
                        if(neighbor_sol->objective_funtion < my_solution->objective_funtion){
                            *my_solution = *neighbor_sol;
                            delete neighbor_sol;
                            return true;
                        }

                        neighbor_sol->InsertOutsideDelta(i2, k, i1, j);
                    }

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                    neighbor_sol->CalculateObjectiveMachine(i1);

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                    neighbor_sol->CalculateObjectiveMachine(i2);
                }
            }
        }
    //}
    delete neighbor_sol;
    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserção de tarefas entre máquina.
 * Ele gera novos vizinhos e para quanto percorrer toda a vizinhança (best improvement)
 * Ele retorna verdadeiro, caso consiga gerar um vizinho melhor
 */
bool InsertOutsideLSMono_BI(MonoSolution *my_solution)
{
    unsigned long num_job_maq1, num_job_maq2;

    MonoSolution *neighbor_sol = new MonoSolution();
    MonoSolution *best_sol = new MonoSolution();

    bool improve = false;

    my_solution->CalculeMonoObjectiveTchebycheff();

    *best_sol = *neighbor_sol = *my_solution;

    improve = false;
    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine-1; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();{
        num_job_maq1 = my_solution->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = my_solution->scheduling[i2].size();

            if(i1 == i2){
                continue;
            }

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k < num_job_maq2; ++k) {

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_sol->InsertOutsideDelta(i1, j, i2, k);

                    neighbor_sol->CalculeMonoObjectiveTchebycheff();

                    best_sol->CalculeMonoObjectiveTchebycheff();

                    //Verifica se a solução vizinha é melhor que a solução corrente
                    //Caso afirmativo, então retorna true, senão continua a busca
                    if(neighbor_sol->objective_funtion < best_sol->objective_funtion){
                        *best_sol = *neighbor_sol;

                    }

                    neighbor_sol->InsertOutsideDelta(i2, k, i1, j);
                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                neighbor_sol->CalculateObjectiveMachine(i1);

                neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                neighbor_sol->CalculateObjectiveMachine(i2);
            }
        }
    }

    my_solution->CalculeMonoObjectiveTchebycheff();
    best_sol->CalculeMonoObjectiveTchebycheff();

    if(best_sol->objective_funtion < my_solution->objective_funtion){
        *my_solution = *best_sol;
        improve=true;
    }

    delete best_sol;
    delete neighbor_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool ChangeOpModeLSMono_FI(MonoSolution *my_solution)
{
    unsigned long num_job_maq, job, old_op;

    MonoSolution *neighbor_sol = new MonoSolution();

    my_solution->CalculeMonoObjectiveTchebycheff();

    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        num_job_maq = my_solution->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            job = my_solution->scheduling[i][j];

            old_op = neighbor_sol->job_mode_op[job];

            //Para cada modo de operração k
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                //if(old_op != k){

                    neighbor_sol->ChangeModeOpJobDelta(i, j, k);
                    neighbor_sol->CalculeMonoObjectiveTchebycheff();

                    my_solution->CalculeMonoObjectiveTchebycheff();

                    //Verifica se a solução vizinha é melhor que a solução corrente
                    //Caso afirmativo, então retorna true, senão continua a busca
                    if(neighbor_sol->objective_funtion < my_solution->objective_funtion){
                        *my_solution = *neighbor_sol;
                        delete neighbor_sol;
                        return true;
                    }

                    neighbor_sol->ChangeModeOpJobDelta(i, j, old_op);

                //}

            }

            neighbor_sol->CalculateShorterTimeHorizonMachine(i);
            neighbor_sol->CalculateObjectiveMachine(i);
        }
    }

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele gera todos os vizinhos e tenta inserir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool ChangeOpModeLSMono_BI(MonoSolution *my_solution)
{
    unsigned long num_job_maq, job, old_op;

    MonoSolution *neighbor_sol = new MonoSolution();
    MonoSolution *best_sol = new MonoSolution();

    bool improve = false;

    my_solution->CalculeMonoObjectiveTchebycheff();

    //Criar uma cópia da solução
    *best_sol = *neighbor_sol = *my_solution;

    improve = false;
    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();{
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            job = neighbor_sol->scheduling[i][j];

            old_op = neighbor_sol->job_mode_op[job];

            //Para cada tarefa k da máquina i
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                //if(old_op != k){

                    //Criar uma cópia da solução
                    //*neighbor_sol = *my_solution;

                    neighbor_sol->ChangeModeOpJobDelta(i, j, k);

                    neighbor_sol->CalculeMonoObjectiveTchebycheff();

                    best_sol->CalculeMonoObjectiveTchebycheff();

                    //Verifica se a solução vizinha é melhor que a solução corrente
                    //Caso afirmativo, então retorna true, senão continua a busca
                    if(neighbor_sol->objective_funtion < best_sol->objective_funtion){
                        *best_sol = *neighbor_sol;


                    }

                    neighbor_sol->ChangeModeOpJobDelta(i, j, old_op);
                //}

                //neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                //neighbor_sol->CalculateObjectiveMachine(i);

            }
        }
    }


    my_solution->CalculeMonoObjectiveTchebycheff();
    best_sol->CalculeMonoObjectiveTchebycheff();

    if(best_sol->objective_funtion < my_solution->objective_funtion){
        *my_solution = *best_sol;
        improve=true;
    }

    delete best_sol;
    delete neighbor_sol;

    return improve;
}
