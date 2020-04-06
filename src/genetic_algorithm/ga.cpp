#include "ga.h"

using namespace std;

void GenerateInitialPopulation(vector<GASolution> &population)
{
    GASolution * my_solution;

    /*Gerar uma população inicial*/
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        my_solution = new GASolution();
        //my_solution->DummyInitialSolution();
        my_solution->RandomInitialSolution();
        population.push_back(*my_solution);
    }
}

void PrintPopulation(vector<GASolution> &population)
{
    for(auto it = population.begin(); it != population.end(); ++it){
        (*it).Print();
    }
}

void Crossover(vector<GASolution> &population, vector<GASolution> &new_population)
{
    GASolution *parent1, *parent2, *offspring1, *offspring2;
    size_t ind1, ind2;

    new_population = population;

    ind1 = rand()%POPULATION_SIZE;
    ind2 = rand()%POPULATION_SIZE;

    parent1 = &population[ind1];
    parent2 = &population[ind2];

    //GenerateOffspring1(parent1, parent2, offspring1, offspring2);
    for (unsigned i = 0; i < NEW_POPULATION_SIZE; ++i) {

        offspring1 = new GASolution();
        offspring2 = new GASolution();

        //GenerateOffspring1(*parent1, *parent2, *offspring1, *offspring2);
        //GenerateOffspring2(*parent1, *parent2, *offspring1, *offspring2);
        GenerateOffspring3(*parent1, *parent2, *offspring1, *offspring2);

        new_population.push_back(*offspring1);

    }

}

void Mutation(vector<GASolution> &population, vector<GASolution> &new_population)
{
    GASolution *parent1, *offspring1;
    size_t ind1;

    new_population = population;

    ind1 = rand()%POPULATION_SIZE;

    parent1 = &population[ind1];

    offspring1 = parent1;

    new_population.push_back(*offspring1);

}

void Selection(vector<GASolution> &population, vector<GASolution> &new_population)
{

}

/*
 * Gerar filhos usando uma representação de vetor
 */
void GenerateOffspring1(GASolution parent1, GASolution parent2,
                       GASolution &offspring1, GASolution &offspring2)
{

    //Criar a representação em forma de vetor para cada pai
    //Pai 1
    vector<unsigned> p1(Instance::numJobs+1);
    SolutionListToVector(parent1, p1);

    //Pai 2
    vector<unsigned> p2(Instance::numJobs+1);
    SolutionListToVector(parent2, p2);

    //Genes iguais
    vector<int> o1(Instance::numJobs+1, -1), o2(Instance::numJobs+1, -1);
    vector<unsigned> o1_g(Instance::numJobs+1, 0), o2_g(Instance::numJobs+1, 0);
    for (unsigned j = 0; j <= Instance::numJobs; ++j) {
        if(p1[j] == p2[j]){
            o1[j] = p1[j];
            o1_g[p1[j]] = 1;
            o2[j] = p2[j];
            o2_g[p2[j]] = 1;
        }
    }

    unsigned aux1;
    //Genes diferentes
    for (unsigned j = 0; j <= Instance::numJobs; ++j) {
        //Se o filho 1 ainda não herdou o gene j dos pais
        if(o1[j] == -1){
            //Se ele pode herdar de qualquer um dos dois pais
            if(o1_g[p1[j]] == 0 && o1_g[p2[j]] == 0){
                //Seleciona aletoriamente
                aux1 = rand()%2;
                if(aux1){
                    o1[j] = p1[j];
                    o1_g[p1[j]] = 1;
                }
                else{
                    o1[j] = p2[j];
                    o1_g[p2[j]] = 1;
                }
            }
            //Se ele pode herdar apenas do primeiro pai
            else if(o1_g[p1[j]] == 0){
                o1[j] = p1[j];
                o1_g[p1[j]] = 1;
            }
            //Se ele pode herdar apenas do segundo pai
            else if(o1_g[p2[j]] == 0){
                o1[j] = p2[j];
                o1_g[p2[j]] = 1;
            }
        }
    }

    //Identificar as tarefas que não foram alocadas
    vector<unsigned> o1_r;
    unsigned k = 0;
    for (unsigned j = 0; j <= Instance::numJobs; ++j) {
        //Se o filho 1 ainda não herdou o gene j dos pais
        if(o1_g[j] == 0){
            while (k <= Instance::numJobs) {
                if(o1[k] == -1){
                    o1[k] = j;
                }
                k++;
            }
        }
    }

    //Completar o filho gerado
    unsigned tam = 3;
    aux1 = 0;
    for (unsigned i = 1; i <= Instance::numMachine; ++i) {
        unsigned j = 0;
        while(j < tam){
            offspring1.scheduling[i].push_back(o1[aux1]);
            aux1 = o1[aux1];
            j++;
        }
    }

    //Verificar ciclos
}

/*
 * Gerar filhos usando uma representação de vetor, versão alternativa
 */
void GenerateOffspring2(GASolution parent1, GASolution parent2,
                        GASolution &offspring1, GASolution &offspring2)
{
    //Criar a representação em forma de vetor para cada pai
    //Pai 1
    vector<unsigned> p1(Instance::numJobs+1);
    SolutionListToVector(parent1, p1);

    //Pai 2
    vector<unsigned> p2(Instance::numJobs+1);
    SolutionListToVector(parent2, p2);

    //Vetores para os filhos
    vector<int> o1(Instance::numJobs+1, -1), o2(Instance::numJobs+1, -1);

    //Realizar o cruzamento
    unsigned next_index = 0;
    for (unsigned j = 0; j <= Instance::numJobs; ++j) {
        if(o1[p1[next_index]] < 0 && o1[p2[next_index]] < 0){
            if(p1[next_index] == p2[next_index]){
                o1[next_index] = p1[next_index];
            }
            else{
                if(rand()%2 == 0){
                    //Herda do pai 1
                    o1[next_index] = p1[next_index];
                }
                else{
                    //Herda do pai 2
                    o1[next_index] = p2[next_index];
                }
            }
        }
        else if(o1[p1[next_index]] < 0){
            o1[next_index] = p1[next_index];
        }
        else if(o1[p2[next_index]] < 0){
            o1[next_index] = p2[next_index];
        }
        else{
            unsigned k;
            for (k = 0; k <= Instance::numJobs; ++k) {
                if(o1[k] < 0 && k != next_index){
                    o1[next_index] = k;
                    break;
                }
            }
            if(k == Instance::numJobs+1){
                o1[next_index] = 0;
            }
        }
        next_index = o1[next_index];

        //Definir o modo de operação
        if(rand()%2 == 0){
            offspring1.job_mode_op[next_index] = parent1.job_mode_op[next_index];
        }
        else{
            offspring1.job_mode_op[next_index] = parent2.job_mode_op[next_index];
        }
    }

    //Construir o filho 1
    unsigned size_machine, sum_jobs;
    next_index = 0;
    sum_jobs = 0;
    for (unsigned i = 1; i < Instance::numMachine; ++i) {
        if(parent1.scheduling[i].size() == parent2.scheduling[i].size()){
            size_machine = parent1.scheduling[i].size();
        }
        else{
            size_machine = min(parent1.scheduling[i].size(),parent2.scheduling[i].size()) +
                         rand()%(max(parent1.scheduling[i].size(),parent2.scheduling[i].size())
                            -min(parent1.scheduling[i].size(),parent2.scheduling[i].size()));
        }
        sum_jobs += size_machine;
        for (unsigned j = 0; j < size_machine; ++j) {            
            offspring1.scheduling[i].push_back(o1[next_index]);
            next_index = o1[next_index];
        }
    }
    size_machine = Instance::numJobs - sum_jobs;
    for (unsigned j = 0; j < size_machine; ++j) {
        offspring1.scheduling[Instance::numMachine].push_back(o1[next_index]);
        next_index = o1[next_index];
    }
    offspring1.CalculateInitialTimeMin();
    offspring1.CalculateObjective();

}

/*
 * Gerar filhos usando o método do Ruiz
 */
void GenerateOffspring3(GASolution parent1, GASolution parent2,
                        GASolution &offspring1, GASolution &offspring2)
{

    //Vetores para os filhos
    //vector<int> o1(Instance::numJobs+1, -1), o2(Instance::numJobs+1, -1);

    vector<bool> o1(Instance::numJobs+1, false), o2(Instance::numJobs+1, false);

    unsigned size;

    //Selecionar uma parte do pai 1 para o filho 1, e o complemento para o filho 2
    for (unsigned i = 1; i <= Instance::numMachine; ++i) {

        //Filho 1
        size = rand()%parent1.scheduling.size();
        for(auto it = parent1.scheduling[i].begin(); it != parent1.scheduling[i].begin()+size; ++it){
            offspring1.scheduling[i].push_back(*it);
            o1[*it] = true;

            //Herdar o modo de operação do pai 1
            offspring1.job_mode_op[*it] = parent1.job_mode_op[*it];
        }

        //Filho 2
        size = parent1.scheduling.size() - size;
        for(auto it = parent1.scheduling[i].begin(); it != parent1.scheduling[i].begin()+size; ++it){
            offspring2.scheduling[i].push_back(*it);
            o2[*it] = true;

            //Herdar o modo de operação do pai 1
            offspring2.job_mode_op[*it] = parent1.job_mode_op[*it];
        }
    }

    //Completar os filhos 1 e 2 com o pai 2
    for (unsigned i = 1; i <= Instance::numMachine; ++i) {
        for(auto it = parent2.scheduling[i].begin(); it != parent2.scheduling[i].end(); ++it){

            //Se a tarefa it não está no filho 1, então ela deve ser adicionada
            if(!o1[*it]){
                offspring1.scheduling[i].push_back(*it);

                //Herdar o modo de operação do pai 2
                offspring1.job_mode_op[*it] = parent2.job_mode_op[*it];
            }

            //Se a tarefa it não está no filho 2, então ela deve ser adicionada
            if(!o2[*it]){
                offspring2.scheduling[i].push_back(*it);

                //Herdar o modo de operação do pai 2
                offspring2.job_mode_op[*it] = parent2.job_mode_op[*it];
            }
        }
    }

    offspring1.CalculateInitialTimeMin();
    offspring2.CalculateInitialTimeMin();
    offspring1.CalculateObjective();
    offspring2.CalculateObjective();

}

void SolutionListToVector(GASolution s, vector<unsigned> &v_solution)
{

    //Criar a representação em forma de vetor
    unsigned index = 0;
    for (unsigned i = 1; i <= Instance::numMachine; ++i) {
        v_solution[index] = s.scheduling[i][0];
        for (unsigned j = 0; j < s.scheduling[i].size()-1; ++j) {
            index = v_solution[index];
            v_solution[index] = s.scheduling[i][j+1];
        }
        index = v_solution[index];
    }
}

bool CompareMakespan(GASolution & l, GASolution & r) //(2)
{
    return l.makeSpan < r.makeSpan;
}

void SortByMakespan(vector<GASolution> &population)
{
    sort(population.begin(), population.end(), CompareMakespan);
}
