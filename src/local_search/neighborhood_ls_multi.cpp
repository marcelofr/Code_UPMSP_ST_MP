#include "neighborhood_ls_multi.h"
/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool SwapInsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set)
{
    unsigned long num_job_maq;

    LSSolution *neighbor_sol = new LSSolution();

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Criar uma cópia da solução
                //*neighbor_sol = *my_solution;

                //Gerar vizinho com a troca de tarefas em uma máquina
                //neighbor_sol->SwapInsideDelta(i, j, k);
                neighbor_sol->SwapInside(i, j, k);


                //Calcula os objetivos considerando apenas a máquina modificada
                neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                neighbor_sol->CalculateObjectiveMachine(i);

                //Tentar adicionar o vizinho gerado no conjunto não-dominado
                //Se conseguiu, então retorna true, senão continua a busca
                //if(AddSolution(neighbor_sol, non_dominated_set)){
                if(non_dominated_set.AddSolution(neighbor_sol)){
                    return true;
                }

                /*if(neighbor_sol->machine_completion_time[i] > Instance::v_peak_start[0]){
                    neighbor_sol->CalculateHorizonAvoidingPeakMachine(i);
                    neighbor_sol->CalculateObjectiveMachine(i);

                    //Tentar adicionar o vizinho gerado no conjunto não-dominado
                    //Se conseguiu, então retorna true, senão continua a busca
                    //if(AddSolution(neighbor_sol, non_dominated_set)){
                    if(non_dominated_set.AddSolution(neighbor_sol)){
                        return true;
                    }
                }*/

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
 * Método que gera vizinhos através da realização de  trocas de tarefas em uma máquina.
 * Ele gera todos os vizinhos e tentar o melhor no conjunto não-dominado
 * (best improvement)
 */
bool SwapInsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution*> &non_dominated_set_out)
{
    unsigned long num_job_maq;

    LSSolution *neighbor_sol = new LSSolution () ;

    bool improve = false;

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq-1; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = j+1; k < num_job_maq; ++k) {

                //Criar uma cópia da solução
                //*neighbor_sol = *my_solution;

                //Gerar vizinho com a troca de tarefas em uma máquina
                //neighbor_sol->SwapInsideDelta(i, j, k);
                neighbor_sol->SwapInside(i, j, k);

                neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                neighbor_sol->CalculateObjectiveMachine(i);

                if(non_dominated_set_out.AddSolution(neighbor_sol)){
                //if(neighbor_sol < best_solution){
                    //*best_solution = *neighbor_sol;
                    improve=true;
                }

                neighbor_sol->SwapInsideDelta(i, k, j);
            }

            neighbor_sol->CalculateShorterTimeHorizonMachine(i);
            neighbor_sol->CalculateObjectiveMachine(i);
        }
    //}

    /*if(non_dominated_set_out.AddSolution(best_solution)){
        improve=true;
    }*/

    delete neighbor_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool SwapOutsideLS_FI(LSSolution* my_solution, NDSetSolution<LSSolution*> &non_dominated_set)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_sol = new LSSolution();

    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine-1; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = i1+1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k < num_job_maq2; ++k) {

                    //Criar uma cópia da solução
                    //*neighbor_sol = *my_solution;

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_sol->SwapOutsideDelta(i1, j, i2, k);
                    /*neighbor_sol->SwapOutside(i1, j, i2, k);


                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    //Tentar adicionar o vizinho gerado no conjunto não-dominado
                    //Se conseguiu, então retorna true
                    //if(AddSolution(neighbor_sol, non_dominated_set)){
                    if(non_dominated_set.AddSolution(neighbor_sol)){
                        return true;
                    }

                    /*if(neighbor_sol->machine_completion_time[i1] > Instance::v_peak_start[0] || neighbor_sol->machine_completion_time[i2] > Instance::v_peak_start[0]){

                        neighbor_sol->CalculateHorizonAvoidingPeakMachine(i1);
                        neighbor_sol->CalculateObjectiveMachine(i1);

                        neighbor_sol->CalculateHorizonAvoidingPeakMachine(i2);
                        neighbor_sol->CalculateObjectiveMachine(i2);

                        //Tentar adicionar o vizinho gerado no conjunto não-dominado
                        //Se conseguiu, então retorna true, senão continua a busca
                        //if(AddSolution(neighbor_sol, non_dominated_set)){
                        if(non_dominated_set.AddSolution(neighbor_sol)){
                            return true;
                        }
                    }*/

                    neighbor_sol->SwapOutsideDelta(i2, k, i1, j);
                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                neighbor_sol->CalculateObjectiveMachine(i1);

                neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                neighbor_sol->CalculateObjectiveMachine(i2);
            }
        }
    //}

    delete neighbor_sol;
    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}

/*
 * Método que gerar vizinhos através da realização de  trocas de tarefas entre máquina.
 * Ele gera todos os vizinhos e tenta incluir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool SwapOutsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution*> &non_dominated_set_out)
{
    unsigned long num_job_maq1, num_job_maq2;
    LSSolution *neighbor_sol = new LSSolution();
    bool improve = false;

    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine-1; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = i1+1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            //Para cada tarefa j da máquina i1
            for (unsigned j = 0; j < num_job_maq1; j++) {

                //Para cada tarefa k da máquina i2
                for (unsigned k = 0; k < num_job_maq2; ++k) {

                    //*neighbor_sol = *my_solution;

                    //Gerar vizinho com a troca de tarefas entre máquinas
                    neighbor_sol->SwapOutsideDelta(i1, j, i2, k);
                    /*neighbor_sol->SwapOutside(i1, j, i2, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    if(non_dominated_set_out.AddSolution(neighbor_sol)){
                    //if(neighbor_sol < best_solution){
                        //*best_solution = *neighbor_sol;
                        improve = true;
                    }

                    neighbor_sol->SwapOutsideDelta(i2, k, i1, j);

                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                neighbor_sol->CalculateObjectiveMachine(i1);

                neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                neighbor_sol->CalculateObjectiveMachine(i2);
            }
        }
    //}

    /*if(non_dominated_set_out.AddSolution(best_solution)){
        improve=true;
    }*/

    delete neighbor_sol;
    //delete best_solution;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas em uma máquina.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool InsertInsideLS_FI(LSSolution* my_solution, NDSetSolution<LSSolution *> &non_dominated_set)
{
    unsigned long num_job_maq;

    LSSolution *neighbor_sol = new LSSolution();

    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = 0; k < num_job_maq; ++k) {

                if(j != k){

                    //Criar uma cópia da solução
                    //*neighbor_sol = *my_solution;

                    //Realizar a inserção
                    neighbor_sol->InsertInsideDelta(i, j, k);
                    /*neighbor_sol->InsertInside(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    //Tentar adicionar o vizinho gerado no conjunto não-dominado
                    //Se conseguiu, então retorna true
                    //if(AddSolution(neighbor_sol, non_dominated_set)){
                    if(non_dominated_set.AddSolution(neighbor_sol)){
                        return true;
                    }

                    /*if(neighbor_sol->machine_completion_time[i] > Instance::v_peak_start[0]){

                        neighbor_sol->CalculateHorizonAvoidingPeakMachine(i);
                        neighbor_sol->CalculateObjectiveMachine(i);

                        if(non_dominated_set.AddSolution(neighbor_sol)){
                            return true;
                        }
                    }*/

                    neighbor_sol->InsertInsideDelta(i, k, j);
                }

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
 * Método que gerar vizinhos através da realização de inserções de tarefas em uma máquina.
 * Ele gera todos os vizinhos e tenta inserir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool InsertInsideLS_BI(LSSolution* my_solution, NDSetSolution<LSSolution *> &non_dominated_set_out)
{
    unsigned long num_job_maq;
    LSSolution *neighbor_sol = new LSSolution();
    //LSSolution *best_solution = new LSSolution();
    bool improve = false;

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {
    unsigned i = neighbor_sol->GetMakespanMachine();
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            //Para cada tarefa k da máquina i
            for (unsigned k = 0; k < num_job_maq; ++k) {

                if(j != k){

                    //Criar uma cópia da solução
                    //*neighbor_sol = *my_solution;

                    //Realizar a inserção
                    neighbor_sol->InsertInsideDelta(i, j, k);
                    /*neighbor_sol->InsertInside(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    if(non_dominated_set_out.AddSolution(neighbor_sol)){
                    //if(neighbor_sol < best_solution){
                        //*best_solution = *neighbor_sol;
                        improve = true;
                    }

                    neighbor_sol->InsertInsideDelta(i, k, j);
                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                neighbor_sol->CalculateObjectiveMachine(i);

            }
        }
    //}

    //delete best_solution;
    delete neighbor_sol;

    return improve;

}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas entre máquinas.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool InsertOutsideLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_sol = new LSSolution();


    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            if(i1 != i2){
                //Para cada tarefa j da máquina i1
                for (unsigned j = 0; j < num_job_maq1; j++) {

                    //Para cada tarefa k da máquina i2
                    for (unsigned k = 0; k <= num_job_maq2; ++k) {

                        //Criar uma cópia da solução
                        //*neighbor_sol = *my_solution;

                        //Realizar a troca
                        neighbor_sol->InsertOutsideDelta(i1, j, i2, k);
                        /*neighbor_sol->InsertOutside(i1, j, i2, k);

                        neighbor_sol->CalculateShorterTimeHorizon();
                        neighbor_sol->CalculateObjective();*/

                        //Tentar adicionar o vizinho gerado no conjunto não-dominado
                        //Se conseguiu, então retorna true
                        //if(AddSolution(neighbor_sol, non_dominated_set)){


                        if(non_dominated_set.AddSolution(neighbor_sol)){

                            return true;
                        }


                        /*if(neighbor_sol->machine_completion_time[i1] > Instance::v_peak_start[0] || neighbor_sol->machine_completion_time[i2] > Instance::v_peak_start[0]){

                            neighbor_sol->CalculateHorizonAvoidingPeakMachine(i1);
                            neighbor_sol->CalculateObjectiveMachine(i1);

                            neighbor_sol->CalculateHorizonAvoidingPeakMachine(i2);
                            neighbor_sol->CalculateObjectiveMachine(i2);

                            //Tentar adicionar o vizinho gerado no conjunto não-dominado
                            //Se conseguiu, então retorna true, senão continua a busca
                            //if(AddSolution(neighbor_sol, non_dominated_set)){
                            if(non_dominated_set.AddSolution(neighbor_sol)){
                                return true;
                            }
                        }*/

                        neighbor_sol->InsertOutside(i2, k, i1, j);
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
 * Método que gerar vizinhos através da realização de inserções de tarefas entre máquinas.
 * Ele gera todos os vizinhos e tenta inserir o melhor no conjunto não-dominado
 * (best improvement)
 */
bool InsertOutsideLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set_out)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_sol = new LSSolution();
    //LSSolution *best_solution = new LSSolution();
    bool improve = false;

    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            if(i1 != i2){
                //Para cada tarefa j da máquina i1
                for (unsigned j = 0; j < num_job_maq1; j++) {

                    //Para cada tarefa k da máquina i2
                    for (unsigned k = 0; k <= num_job_maq2; ++k) {

                        //Criar uma cópia da solução
                        //*neighbor_sol = *my_solution;

                        //Realizar a troca
                        neighbor_sol->InsertOutsideDelta(i1, j, i2, k);
                        /*neighbor_sol->InsertOutside(i1, j, i2, k);

                        neighbor_sol->CalculateShorterTimeHorizon();
                        neighbor_sol->CalculateObjective();*/

                        if(non_dominated_set_out.AddSolution(neighbor_sol)){
                        //if(neighbor_sol < best_solution){
                            //*best_solution = *neighbor_sol;
                            improve = true;
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

    /*if(non_dominated_set_out.AddSolution(best_solution)){
        improve=true;
    }*/

    //delete best_solution;
    delete neighbor_sol;

    return improve;
}

/*
 * Método que gerar vizinhos através da realização da troca do modo de operação das tarefas
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool ChangeOpModeLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set)
{
    unsigned long num_job_maq, job, old_op;

    LSSolution *neighbor_sol = new LSSolution();

    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            job = neighbor_sol->scheduling[i][j];

            old_op = neighbor_sol->job_mode_op[job];

            //Para cada modo de operração k
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {


                //Criar uma cópia da solução
                //*neighbor_sol = *my_solution;


                if(old_op != k){

                    neighbor_sol->ChangeModeOpJobDelta(i, j, k);
                    /*neighbor_sol->ChangeModeOpJob(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    //Tentar adicionar o vizinho gerado no conjunto não-dominado
                    //Se conseguiu, então retorna true
                    //if(AddSolution(neighbor_sol, non_dominated_set)){
                    if(non_dominated_set.AddSolution(neighbor_sol)){

                        return true;
                    }

                    /*if(neighbor_sol->machine_completion_time[i] > Instance::v_peak_start[0]){

                        neighbor_sol->CalculateHorizonAvoidingPeakMachine(i);
                        neighbor_sol->CalculateObjectiveMachine(i);

                        if(non_dominated_set.AddSolution(neighbor_sol)){
                            return true;
                        }
                    }*/

                    neighbor_sol->ChangeModeOpJobDelta(i, j, old_op);

                }



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
bool ChangeOpModeLS_BI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set_out)
{
    unsigned long num_job_maq, job, old_op;

    LSSolution *neighbor_sol = new LSSolution();
    //LSSolution *best_solution = new LSSolution();
    bool improve = false;

    //Criar uma cópia da solução
    *neighbor_sol = *my_solution;

    //Para cada máquina i de 1 à n
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        num_job_maq = neighbor_sol->scheduling[i].size();

        //Para cada tarefa j da máquina i
        for (unsigned j = 0; j < num_job_maq; j++) {

            job = neighbor_sol->scheduling[i][j];

            old_op = neighbor_sol->job_mode_op[job];

            //Para cada tarefa k da máquina i
            for (unsigned k = 1; k <= Instance::num_mode_op; ++k) {

                if(old_op != k){

                    //Criar uma cópia da solução
                    //*neighbor_sol = *my_solution;

                    neighbor_sol->ChangeModeOpJobDelta(i, j, k);
                    /*neighbor_sol->ChangeModeOpJob(i, j, k);

                    neighbor_sol->CalculateShorterTimeHorizon();
                    neighbor_sol->CalculateObjective();*/

                    if(non_dominated_set_out.AddSolution(neighbor_sol)){
                    //if(neighbor_sol < best_solution){
                        //*best_solution = *neighbor_sol;
                        improve = true;
                    }

                    neighbor_sol->ChangeModeOpJobDelta(i, j, old_op);
                }

                neighbor_sol->CalculateShorterTimeHorizonMachine(i);
                neighbor_sol->CalculateObjectiveMachine(i);

            }
        }
    }

    delete neighbor_sol;
    //delete best_solution;

    return improve;
}

//bool ChangeHLS(LSSolution my_solution, vector<LSSolution> &non_dominated_set)
bool ChangeHLS(LSSolution , vector<LSSolution> &)
{
    return false;
}

/*
 * Método que gerar vizinhos através da realização de inserções de tarefas entre máquinas.
 * Ele tenta gerar novos vizinhos até encontrar um que possa ser incluído
 * no conjunto não-dominado (first improvement), ou até percorrer toda a vizinhança
 * Ele retorna verdadeiro, caso consiga adicionar um vizinho ao conjunto não-dominado
 */
bool InsertOutsideDuoLS_FI(LSSolution *my_solution, NDSetSolution<LSSolution *> &non_dominated_set)
{
    unsigned long num_job_maq1, num_job_maq2;

    LSSolution *neighbor_sol = new LSSolution();


    *neighbor_sol = *my_solution;

    //Para cada máquina i1 de 1 à n
    //for (unsigned i1 = 1; i1 <= Instance::num_machine; i1++) {
    unsigned i1 = neighbor_sol->GetMakespanMachine();
        num_job_maq1 = neighbor_sol->scheduling[i1].size();

        //Para cada máquina i2 de i1+1 à n
        for (unsigned i2 = 1; i2 <= Instance::num_machine; i2++) {
            num_job_maq2 = neighbor_sol->scheduling[i2].size();

            if(i1 != i2){
                //Para cada tarefa j da máquina i1
                for (unsigned j1 = 0; j1 < num_job_maq1-1; j1++) {
                for (unsigned j2 = j1+1; j2 < num_job_maq1-1; j2++) {

                    //Para cada tarefa k da máquina i2
                    for (unsigned k1 = 0; k1 <= num_job_maq2-1; ++k1) {
                    for (unsigned k2 = k1+1; k2 <= num_job_maq2; ++k2) {

                        //Criar uma cópia da solução
                        //*neighbor_sol = *my_solution;

                        //Realizar a troca
                        neighbor_sol->InsertOutsideDuoDelta(i1, j1, j2, i2, k1, k2);
                        /*neighbor_sol->InsertOutside(i1, j, i2, k);

                        neighbor_sol->CalculateShorterTimeHorizon();
                        neighbor_sol->CalculateObjective();*/

                        //Tentar adicionar o vizinho gerado no conjunto não-dominado
                        //Se conseguiu, então retorna true
                        //if(AddSolution(neighbor_sol, non_dominated_set)){


                        if(non_dominated_set.AddSolution(neighbor_sol)){

                            return true;
                        }


                        /*if(neighbor_sol->machine_completion_time[i1] > Instance::v_peak_start[0] || neighbor_sol->machine_completion_time[i2] > Instance::v_peak_start[0]){

                            neighbor_sol->CalculateHorizonAvoidingPeakMachine(i1);
                            neighbor_sol->CalculateObjectiveMachine(i1);

                            neighbor_sol->CalculateHorizonAvoidingPeakMachine(i2);
                            neighbor_sol->CalculateObjectiveMachine(i2);

                            //Tentar adicionar o vizinho gerado no conjunto não-dominado
                            //Se conseguiu, então retorna true, senão continua a busca
                            //if(AddSolution(neighbor_sol, non_dominated_set)){
                            if(non_dominated_set.AddSolution(neighbor_sol)){
                                return true;
                            }
                        }*/

                        neighbor_sol->InsertOutsideDuoDelta(i2, k1, k2, i1, j1, j2);
                        //neighbor_sol->InsertOutside(i2, k, i1, j);
                    }
                    }

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i1);
                    neighbor_sol->CalculateObjectiveMachine(i1);

                    neighbor_sol->CalculateShorterTimeHorizonMachine(i2);
                    neighbor_sol->CalculateObjectiveMachine(i2);
                }
                }
            }
        //}
    }

    delete neighbor_sol;

    //Retorna falso, caso não consiga encontrar um vizinho melhor
    return false;
}
