#include "ga.h"

using namespace std;

/*
 * Método para gerar a população inicial com tamanho POPULATION_SIZE
 */
void GenerateInitialPopulation(vector<GASolution> &population)
{
    GASolution * my_solution;

    /*Gerar uma população inicial*/
    /*Gerar uma solução gulosa considerando o objetivo do makespan*/
    my_solution = new GASolution();
    my_solution->GreedyInitialSolutionMakespan();
    //population.push_back(*my_solution);
    PopulationAddIndividual(population, *my_solution);


    /*Gerar uma solução gulosa considerando o objetivo do TEC*/
    my_solution = new GASolution();
    my_solution->GreedyInitialSolutionTEC3();
    population.push_back(*my_solution);

    /*Gerar o restante dos indivíduos aleatoriamente*/
    for (int i = 0; i < POPULATION_SIZE-2; ++i) {
        my_solution = new GASolution();
        //my_solution->DummyInitialSolution();
        my_solution->RandomInitialSolution();
        population.push_back(*my_solution);
    }
}

/*
 * Método para imprimir um vetor de soluções
 */
void PrintPopulation(vector<GASolution> &population)
{
    for(auto it = population.begin(); it != population.end(); ++it){
        (*it).Print();
    }
}

/*
 * Cruzamento para gerar TAM_CROSSOVER indivíduos
 * Uso de torneio binário para selecionar os indivíduos para o cruzamento
 */
void Crossover(vector<GASolution> &population, vector<GASolution> &new_population, vector<vector<GASolution>> F)
{
    GASolution *parent1, *parent2, *offspring1, *offspring2;
    unsigned ind1, ind2;
    unsigned size = population.size();

    new_population.clear();

    //Gerar novos indivíduos com o cruzamento
    for (unsigned i = 0; i < TAM_CROSSOVER/2; ++i) {

        //Seleção de indivíduos por torneio binário

        //Escolher dois indivíduos aleatoriamente
        ind1 = rand()%size;
        ind2 = rand()%size;
        //Escolher o melhor indivíduo para ser o pai 1
        if(population[ind1] < population[ind2]){
            parent1 = &population[ind1];
        }
        else{
            parent1 = &population[ind2];
        }

        //Escolher dois indivíduos aleatoriamente
        ind1 = rand()%size;
        ind2 = rand()%size;
        //Escolher o melhor indivíduo para ser o pai 2
        if(population[ind1] < population[ind2]){
            parent2 = &population[ind1];
        }
        else{
            parent2 = &population[ind2];
        }

        //Realizar o cruzamento
        offspring1 = new GASolution();
        offspring2 = new GASolution();
        GenerateOffspring3(*parent1, *parent2, *offspring1, *offspring2);

        //Adicionar os filhos gerados à nova população
        new_population.push_back(*offspring1);
        new_population.push_back(*offspring2);

    }

}

/*
 * Método para adicionar nova solução gerada a partir de uma mutação
 */
void Mutation(vector<GASolution> &population, vector<GASolution> &new_population)
{
    GASolution *individual;
    size_t ind1;
    unsigned op;

    unsigned prob;

    for (unsigned i = 0; i < population.size(); ++i) {

        //Gerar um número entre zero e cem
        prob = rand()%100;

        //Se o número gerado é menor que a probabilidade definida, fazer a mutação
        if(prob < PROBABILITY_MUTATION){

            //Escolher o indivídua para fazer a mutação
            ind1 = rand()%POPULATION_SIZE;
            individual = new GASolution();
            *individual = population[ind1];

            //Escolher um tipo de mutação
            op = rand()%1;

            switch (op) {
                case 0:
                    MutationOperatorSwapInside(*individual);
                    break;
                default:
                    break;
            }

            new_population.push_back(*individual);

        }
    }
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
    vector<unsigned> p1(Instance::num_jobs+1);
    SolutionListToVector(parent1, p1);

    //Pai 2
    vector<unsigned> p2(Instance::num_jobs+1);
    SolutionListToVector(parent2, p2);

    //Genes iguais
    vector<int> o1(Instance::num_jobs+1, -1), o2(Instance::num_jobs+1, -1);
    vector<unsigned> o1_g(Instance::num_jobs+1, 0), o2_g(Instance::num_jobs+1, 0);
    for (unsigned j = 0; j <= Instance::num_jobs; ++j) {
        if(p1[j] == p2[j]){
            o1[j] = p1[j];
            o1_g[p1[j]] = 1;
            o2[j] = p2[j];
            o2_g[p2[j]] = 1;
        }
    }

    unsigned aux1;
    //Genes diferentes
    for (unsigned j = 0; j <= Instance::num_jobs; ++j) {
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
    for (unsigned j = 0; j <= Instance::num_jobs; ++j) {
        //Se o filho 1 ainda não herdou o gene j dos pais
        if(o1_g[j] == 0){
            while (k <= Instance::num_jobs) {
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
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
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
    vector<unsigned> p1(Instance::num_jobs+1);
    SolutionListToVector(parent1, p1);

    //Pai 2
    vector<unsigned> p2(Instance::num_jobs+1);
    SolutionListToVector(parent2, p2);

    //Vetores para os filhos
    vector<int> o1(Instance::num_jobs+1, -1), o2(Instance::num_jobs+1, -1);

    //Realizar o cruzamento
    unsigned next_index = 0;
    for (unsigned j = 0; j <= Instance::num_jobs; ++j) {
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
            for (k = 0; k <= Instance::num_jobs; ++k) {
                if(o1[k] < 0 && k != next_index){
                    o1[next_index] = k;
                    break;
                }
            }
            if(k == Instance::num_jobs+1){
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
    for (unsigned i = 1; i < Instance::num_machine; ++i) {
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
    size_machine = Instance::num_jobs - sum_jobs;
    for (unsigned j = 0; j < size_machine; ++j) {
        offspring1.scheduling[Instance::num_machine].push_back(o1[next_index]);
        next_index = o1[next_index];
    }
    offspring1.CalculateShorterTimeHorizon();
    offspring1.CalculateObjective();

}

/*
 * Gerar filhos usando o método do Ruiz
 */
void GenerateOffspring3(GASolution parent1, GASolution parent2,
                        GASolution &offspring1, GASolution &offspring2)
{

    //Vetores para os filhos
    vector<bool> o1(Instance::num_jobs+1, false), o2(Instance::num_jobs+1, false);

    unsigned size;

    unsigned op = rand()%2;

    //Para cada máquina, a primeira parte das tarefas do pai 1 é herdada pelo filho 1
    //A segunda parte é herdada pelo filho 2
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {

        if(parent1.scheduling[i].size() > 0){
            //O filho 1 herda a primeira parte das tarefas do pai 1, na máquina i
            size = rand()%parent1.scheduling[i].size();
            for(auto it = parent1.scheduling[i].begin(); it != parent1.scheduling[i].begin()+size; ++it){
                offspring1.scheduling[i].push_back(*it);
                o1[*it] = true;

                //O filho 1 herda o modo de operação do pai 1
                offspring1.job_mode_op[*it] = parent1.job_mode_op[*it];
            }

            //O filho 2 herda a segunda parte das tarefas do pai 1, na máquina i
            size = parent1.scheduling[i].size() - size;
            for(auto it = parent1.scheduling[i].begin(); it != parent1.scheduling[i].begin()+size; ++it){
                offspring2.scheduling[i].push_back(*it);
                o2[*it] = true;

                //O filho 2 herda o modo de operação do pai 1
                offspring2.job_mode_op[*it] = parent1.job_mode_op[*it];
            }
        }

    }

    //As tarefas restantes dos filhos 1 e 2 são herdadas de acordo com característica do pai 2
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
        for(auto it = parent2.scheduling[i].begin(); it != parent2.scheduling[i].end(); ++it){

            //Se a tarefa it ainda não está no filho 1, então ela deve ser adicionada
            if(!o1[*it]){

                //Herdar o modo de operação do pai 2
                //offspring1.job_mode_op[*it] = parent2.job_mode_op[*it];

                //Inserir a nova tarefa, na melhor posição considerando um dos objetivos
                if(op == 0){
                //if(true){
                    //offspring1.scheduling[i].push_back(*it);
                    offspring1.AddJobGreedyMakespanMachine(i, *it, parent2.job_mode_op[*it]);
                }
                else{
                    //offspring1.AddJobGreedyTECMachine(i, *it, parent2.job_mode_op[*it]);
                    offspring1.AddJobGreedyTECMachine3(i, *it, parent2.job_mode_op[*it]);
                }

            }

            //Se a tarefa it não está no filho 2, então ela deve ser adicionada
            if(!o2[*it]){

                //Herdar o modo de operação do pai 2
                //offspring2.job_mode_op[*it] = parent2.job_mode_op[*it];

                //Inserir a nova tarefa, na melhor posição considerando um dos objetivos
                if(op == 0){
                //if(true){
                    //offspring2.scheduling[i].push_back(*it);
                    offspring2.AddJobGreedyMakespanMachine(i, *it, parent2.job_mode_op[*it]);
                }
                else{
                    //offspring2.AddJobGreedyTECMachine(i, *it, parent2.job_mode_op[*it]);
                    offspring2.AddJobGreedyTECMachine3(i, *it, parent2.job_mode_op[*it]);
                }

            }
        }
    }

    if(op == 0){
        //Definir o instance inicial de cada tarefa presente na sequência
        //considerando o menor valor para o tempo de término em cada máquina
        offspring1.CalculateShorterTimeHorizon();
        offspring2.CalculateShorterTimeHorizon();
    }
    else{
        //Definir o instance inicial de cada tarefa presente na sequência
        //considerando o menor valor para o custo de energia
        offspring1.CalculateHorizonAvoidingPeak();
        offspring2.CalculateHorizonAvoidingPeak();
    }


    //Calcular a função objetivo
    offspring1.CalculateObjective();
    offspring2.CalculateObjective();

}

void SolutionListToVector(GASolution s, vector<unsigned> &v_solution)
{

    //Criar a representação em forma de vetor
    unsigned index = 0;
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
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
    if(l.makeSpan < r.makeSpan){
        return true;
    }
    else if (l.makeSpan > r.makeSpan){
        return false;
    }
    else{
        if(l.TEC < r.TEC){
            return true;
        }
        else{
            return false;
        }
    }

}

bool CompareTEC(GASolution & l, GASolution & r) //(2)
{
    if(l.TEC < r.TEC){
        return true;
    }
    else if (l.TEC > r.TEC){
        return false;
    }
    else{
        if(l.makeSpan < r.makeSpan){
            return true;
        }
        else{
            return false;
        }
    }

}

void SortByMakespan(vector<GASolution> &population)
{
    sort(population.begin(), population.end(), CompareMakespan);
}

void SortByTEC(vector<GASolution> &population)
{
    sort(population.begin(), population.end(), CompareTEC);
}

/*
 * Método para realizar mutação em um indivíduo
 * A mutação consiste em trocar duas tarefas de posição na mesma máquina
 */
void MutationOperatorSwapInside(GASolution &individual)
{
    unsigned machine, pos_job1, pos_job2;
    //Escolher aleatoriamente a máquina na qual a troca será realizada
    machine = 1 + rand()%Instance::num_machine;

    //Caso não tenha pelo menos duas tarefas, não deve realizar a mutação
    if(individual.scheduling[machine].size() < 2){
        return;
    }

    //Escolher a tarefa
    pos_job1 = rand()%individual.scheduling[machine].size();
    do{
        pos_job2 = rand()%individual.scheduling[machine].size();

    }while(pos_job1 != pos_job2);

    individual.SwapInside(machine, pos_job1, pos_job2);
}

void PopulationAddIndividual(vector<GASolution> &population, GASolution &individual)
{
    bool add = true;
    for(auto it = population.begin(); it != population.end(); ++it){
        //Se já tem um indivíduo igual na população
        if(individual.makeSpan == it->makeSpan && abs(individual.TEC - it->TEC) < EPS){
            //Não adicionar
            add = false;
            break;
        }
    }

    if(add){
        population.push_back(individual);
    }
}

/*
 * População @R recebe a união entre @P e @Q
 */
void UnionPopulation(vector<GASolution> &R, vector<GASolution> P, vector<GASolution> Q)
{

    R.clear();

    for(auto it = P.begin(); it != P.end(); ++it){
        PopulationAddIndividual(R, *it);
    }

    for(auto it = Q.begin(); it != Q.end(); ++it){
        PopulationAddIndividual(R, *it);
    }
}
