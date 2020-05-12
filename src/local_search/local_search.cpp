#include "local_search.h"

/*
 * Método para gerar um conjunto de soluções iniciais
 */
void GenInitialSet(vector<LSSolution> &non_dominated_set)
{
    LSSolution * my_solution;

    /*Gerar uma conjunto inicial*/
    /*Gerar uma solução gulosa considerando o objetivo do makespan*/
    my_solution = new LSSolution();
    my_solution->GreedyInitialSolutionMakespan();
    AddSolution(*my_solution, non_dominated_set);


    /*Gerar uma solução gulosa considerando o objetivo do TEC*/
    my_solution = new LSSolution();
    my_solution->GreedyInitialSolutionTEC3();
    AddSolution(*my_solution, non_dominated_set);
}

void HillClimbing(vector<Solution> &non_dominated_set)
{

    vector<LSSolution> cur_non_dominated_set;
    LSSolution cur_solution;

    GenInitialSet(cur_non_dominated_set);

   //Fazer busca local em cada solução do conjunto não-dominado
    for(auto it_sol = cur_non_dominated_set.begin(); it_sol != cur_non_dominated_set.end();){

        //Criar uma cópia de it_sol em cur_solution
        cur_solution = *it_sol;

        //A busca deve ser feita apenas se a solução ainda não foi visitada
        if(!cur_solution.was_visited){

            //Realiza busca local na solução corrente (troca de tarefas na mesma máquina)
            //Se houve melhora
            if(SwapInsideLS_FI(cur_solution, cur_non_dominated_set)){
                //Continua a busca a partir da primeira solução do conjunto
                it_sol = cur_non_dominated_set.begin();
                continue;
            }

            //Realiza busca local na solução corrente (troca de tarefas entre máquinas)
            //Se houve melhora
            if(SwapOutsideLS_FI(cur_solution, cur_non_dominated_set)){
                //Continua a busca a partir da primeira solução do conjunto
                it_sol = cur_non_dominated_set.begin();
                continue;
            }

            //Realiza busca local na solução corrente (inserção de tarefas na mesma máquina)
            //Se houve melhora
            if(InsertInsideLS_FI(cur_solution, cur_non_dominated_set)){
                //Continua a busca a partir da primeira solução do conjunto
                it_sol = cur_non_dominated_set.begin();
                continue;
            }

            //Realiza busca local na solução corrente (inserção de tarefas entre máquinas)
            //Se houve melhora
            if(InsertOutsideLS_FI(cur_solution, cur_non_dominated_set)){
                //Continua a busca a partir da primeira solução do conjunto
                it_sol = cur_non_dominated_set.begin();
                continue;
            }

            //Realiza busca local na solução corrente (troca do modo de operação da tarefa)
            //Se houve melhora
            if(ChangeOpModeLS(cur_solution, cur_non_dominated_set)){
                //Continua a busca a partir da primeira solução do conjunto
                it_sol = cur_non_dominated_set.begin();
                continue;
            }

            //Se chegar até aqui, então marcar essa solução como visitada
            cur_solution.was_visited = true;
        }

        ++it_sol;

        /*else{
            //Se a solução corrente já foi visitada, então vai pra próxima
            ++it_sol;
        }*/

    }

    //Salvar o conjunto em non_dominated_set
    non_dominated_set.clear();
    move(cur_non_dominated_set.begin(), cur_non_dominated_set.end(), back_inserter(non_dominated_set));

}
