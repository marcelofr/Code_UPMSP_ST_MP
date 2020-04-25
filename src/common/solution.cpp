#include "solution.h"

Solution::Solution()
{
    //Criar uma lista vazia para cada máquina
    scheduling = vector<vector<unsigned>>(Instance::numMachine+1, vector<unsigned>(0));

    //Criar um vetor para armazenar o instante de início de cada tarefa
    job_start_time = vector<unsigned>(Instance::numJobs+1);

    //Criar um vetor para armazenar o instante de termino de cada tarefa
    job_end_time = vector<unsigned>(Instance::numJobs+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    machine_completion_time = vector<unsigned>(Instance::numMachine+1);

    //Criar um vetor para armazenar o modo de operação de cada tarefa
    job_mode_op = vector<unsigned>(Instance::numJobs+1);

    //Criar um vetor para armazenar o tempo de processamento em cada máquina
    machine_processing_time = vector<unsigned>(Instance::numMachine+1);

    //Criar um vetor para armazenar o tempo de preparação em cada máquina
    machine_setup_time = vector<unsigned>(Instance::numMachine+1);

    makeSpan = 0;
}

Solution::~Solution()
{
    this->scheduling.clear();
}

void Solution::Print()
{
    for (size_t i = 1; i <= Instance::numMachine; ++i) {
        cout << "Máquina " << i << ": ";
        for (auto it = scheduling[i].begin(); it != scheduling[i].end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    }
    cout << "Makespan: " << makeSpan << endl;
    cout << "TEC: " << TEC << endl;
    cout << endl;
}

void Solution::DummyInitialSolution()
{
    size_t i = 1;

    //Inserir cada tarefa j na solução inicial
    for (size_t j = 1; j <= Instance::numJobs; ++j) {
        //Inserir a tarefa j na máquina i
        scheduling[i].push_back(j);

        //Definir o modo de operação de cada tarefa j, aleatoriamente
        job_mode_op[j] = 1 + rand()%(Instance::numModeOp);

        //Distribuir as tarefas entre as máquinas, incrementar i
        if(j/i >= Instance::numJobs/Instance::numMachine){

            //Mudar de máquina
            i++;
        }
    }

    CalculateInitialTimeMin();
    CalculateObjective();
}

void Solution::RandomInitialSolution()
{

    vector<unsigned> jobs(Instance::numJobs+1);

    for(unsigned i = 0; i <= Instance::numJobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    random_shuffle(jobs.begin()+1, jobs.end());

    //Inserir cada tarefa através de uma estratégia aleatória
    for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

        //Definir o modo de operação de cada tarefa it, aleatoriamente
        job_mode_op[*it] = 1 + rand()%(Instance::numModeOp);

        //Inserir aleatoriamente a tarefa it
        InsertRandomPosition(*it);
    }

    CalculateInitialTimeMin();
    CalculateObjective();

#ifdef DEBUG
    Check();
#endif

}

/*
 * Método para gerar uma solução inicial gulosa,
 * considerando o objetivo do makespan
 */
void Solution::GreedyInitialSolutionMakespan()
{
    vector<unsigned> jobs(Instance::numJobs+1);

    for(unsigned i = 0; i <= Instance::numJobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    random_shuffle(jobs.begin()+1, jobs.end());

    //Selecionar o modo de operação, mais rápido
    //Como ele é um fator, deve ser escolhido aquele de maior valor
    unsigned best_op_mode = 1;
    for(unsigned i = 2; i > Instance::numModeOp; i++){
        if(Instance::SpeedFactor[i] < Instance::SpeedFactor[i-1]){
            best_op_mode = i;
        }
    }

    //Inserir cada tarefa através de uma estratégia gulosa
    for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

        //Inserir a tarefa it na melhor posição possível
        AddJobGreedyMakespan(*it, best_op_mode);

    }

    CalculateInitialTimeMin();
    CalculateObjective();

#ifdef DEBUG
    Check();
#endif
}

/*
 * Método para gerar uma solução inicial gulosa,
 * considerando o objetivo do TEC
 */
void Solution::GreedyInitialSolutionTEC()
{
    vector<unsigned> jobs(Instance::numJobs+1);

    for(unsigned i = 0; i <= Instance::numJobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    random_shuffle(jobs.begin()+1, jobs.end());

    //Selecionar o modo de operação, menor consumo
    unsigned best_op_mode = 1;
    for(unsigned i = 2; i <= Instance::numModeOp; i++){
        if(Instance::ConsumptionFactor[i] < Instance::ConsumptionFactor[i-1]){
            best_op_mode = i;
        }
    }

    //Inserir cada tarefa através de uma estratégia gulosa
    for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

        //Definir o modo de operação de cada tarefa it, aleatoriamente
        job_mode_op[*it] = 1 + rand()%(Instance::numModeOp);

        //Escolher a melhor posição para a tarefa it
        AddJobGreedyTEC(*it, best_op_mode);

    }

    //CalculateInitialTimeMin();
    CalculateInitialTimeAvoidPeak();
    CalculateObjective();

#ifdef DEBUG
    Check();
#endif
}

/*
 * Método para calcular o instante inicial para cada tarefa
 * considerando um tempo mínimo entre as tarefas
 */
void Solution::CalculateInitialTimeMin()
{
    unsigned start;
    unsigned last_job, l;
    unsigned processing_time, setup_time, previous_job;

    for(size_t i = 1; i <= Instance::numMachine; ++i){

        previous_job = 0;
        start = 0;

        //Percorrer as tarefas de cada máquina
        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            //Armazenar o tempo de início da tarefa j
            job_start_time[*j] = start;

            //Tempo de preparação da tarefa j
            setup_time = Instance::SetupTime[i][previous_job][(*j)];
            machine_setup_time[i] += setup_time;

            //Tempo de processamento da tarefa j
            l = job_mode_op[*j];
            processing_time = ceil(Instance::ProcessingTime[i][*j]/Instance::SpeedFactor[l]);
            machine_processing_time[i] += processing_time;

            //Identificar a tarefa anterior
            previous_job = *j;

            //Armazenar o instante de término da tarefa j
            job_end_time[*j] = start + setup_time + processing_time;

            //A próxima tarefa começa após a tarefa atual
            start = job_end_time[*j];

        }

        //Calcular o instante de conclusão de cada máquina
        if(scheduling[i].size() > 0){

            last_job = scheduling[i].back();

            machine_completion_time[i] = job_end_time[last_job];
        }
        else{
            machine_completion_time[i] = 0;
        }

    }
}

/*
 * Método para calcular o instante inicial para cada tarefa
 * considerando a alocação atual e evitando o horário de pico
 */
void Solution::CalculateInitialTimeAvoidPeak()
{
    unsigned completion_time_previos;
    unsigned last_job, l;
    unsigned processing_time, setup_time, previous_job;

    for(size_t i = 1; i <= Instance::numMachine; ++i){

        previous_job = 0;
        completion_time_previos = 0;

        //Percorrer as tarefas de cada máquina
        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            //Tempo de preparação da tarefa j
            setup_time = Instance::SetupTime[i][previous_job][(*j)];
            machine_setup_time[i] += setup_time;

            //Tempo de processamento da tarefa j
            l = job_mode_op[*j];
            processing_time = ceil(Instance::ProcessingTime[i][*j]/Instance::SpeedFactor[l]);
            machine_processing_time[i] += processing_time;

            //Se a tarefa anterior termina antes do horário de pico e
            //a tarefa atual termina após começar o horário de pico
            //Adiar o horário de início para o fim do horário de pico
            if(completion_time_previos < Instance::peakStart &&
                    completion_time_previos + setup_time + processing_time > Instance::peakStart){
                //Começar a tarefa após o horário de pico
                job_start_time[*j] = Instance::peakEnd;
            }
            else{
                //Começar a tarefa imediatamente após a anterior
                job_start_time[*j] = completion_time_previos;
            }

            //Armazenar o instante de término da tarefa j
            job_end_time[*j] = job_start_time[*j] + setup_time + processing_time;


            //A tarefa j passa a ser a tarefa anterior
            previous_job = *j;

            //O tempo de término da tarefa anterior é o tempo de término de j
            completion_time_previos = job_end_time[*j];

        }

        //Calcular o instante de conclusão de cada máquina
        if(scheduling[i].size() > 0){

            last_job = scheduling[i].back();

            machine_completion_time[i] = job_end_time[last_job];
        }
        else{
            machine_completion_time[i] = 0;
        }

    }
}

void Solution::CalculateObjective()
{
    //Calcular o makespan
    makeSpan = 0;
    for (unsigned i = 1; i <= Instance::numMachine; ++i) {
        if(machine_completion_time[i] > makeSpan){
            makeSpan = machine_completion_time[i];
        }
    }

    //Calcular o custo total de energia
    double PEC_on, PEC_off;
    double PEC_on_aux, PEC_off_aux;
    int l;

    PEC_on = PEC_off = 0;

    for(unsigned i = 1; i <= Instance::numMachine; ++i){

        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            PEC_on_aux = PEC_off_aux = 0;

            //Tarefa começa e termina antes do início do horário de pico
            if(job_end_time[*j] <= Instance::peakStart){
                PEC_off_aux += job_end_time[*j] - job_start_time[*j];
            }
            //Tarefa começa antes do início do horário de pico
            else if(job_start_time[*j] <= Instance::peakStart){
                PEC_off_aux += Instance::peakStart - job_start_time[*j];

                //Tarefa termina antes do fim do horário de pico
                if(job_end_time[*j] <= Instance::peakEnd){
                    PEC_on_aux += job_end_time[*j] - Instance::peakStart;
                }
                //Tarefa termina após o fim do horário de pico
                else{
                    PEC_on_aux += Instance::peakEnd - Instance::peakStart;
                    PEC_off_aux += job_end_time[*j] - Instance::peakEnd;
                }
            }
            //Tarefa começa antes do fim do horário de pico
            else if(job_start_time[*j] <= Instance::peakEnd){
                //Tarefa teremina antes do fim do horário de pico
                if(job_end_time[*j] <= Instance::peakEnd){
                    PEC_on_aux += job_end_time[*j] - job_start_time[*j];
                }
                //Tarefa termina após o fim do horário de pico
                else{
                    PEC_on_aux += Instance::peakEnd - job_start_time[*j];
                    PEC_off_aux += job_end_time[*j] - Instance::peakEnd;
                }
            }
            //Tarefa começa após o fim do horário de pico
            else{
                PEC_off_aux += job_end_time[*j] - job_start_time[*j];
            }

            //Cada tarefa está associada a um fator de consumo, que depende do modo de operação
            l = job_mode_op[*j];
            PEC_on_aux = PEC_on_aux * Instance::ConsumptionFactor[l]
                    * Instance::MachinePotency[i] * Instance::rateOnPeakHours / 6;
            PEC_off_aux = PEC_off_aux * Instance::ConsumptionFactor[l]
                    * Instance::MachinePotency[i] * Instance::rateOnPeakHours / 6;

            //Salvar o custo parcial de energia calculado para cada tarefa
            PEC_on += PEC_on_aux;
            PEC_off += PEC_off_aux;
        }
    }
    TEC = PEC_on + PEC_off;
}

void Solution::Check()
{
    unsigned job, previous, end_job, start_job;
    for (unsigned i = 1; i <= Instance::numMachine; i++) {
        previous = 0;
        start_job = 0;
        for (unsigned j = 0; j < scheduling[i].size(); j++) {

            //Selecionar a tarefa atual
            job = scheduling[i][j];

            //Calcular o tempo de término da tarefa atual
            end_job = start_job +
                    ceil(Instance::ProcessingTime[i][job]) / Instance::SpeedFactor[job_mode_op[job]] +
                    Instance::SetupTime[i][previous][job];

            //Se o tempo calculado for diferente do valor armazenado, existe um erro
            if(job_start_time[job] < start_job){
                cout << "=========Erro==============" << endl;
                cout << "Calculo da H está errado" << endl;

            }

            //Atualizar a informação da tarefa anterior e do instante de início
            previous = job;
            start_job = end_job;
        }
    }
}

/*
 * Inserir uma nova tarefa em uma máquina específica da solução atual,
 * escolher a posição que implique em menor tempo de término nessa máquina
 * Escolher a posição que gere menor diferença de tempo de preparação,
 * uma vez que o tempo de processamento é constante
 */
void Solution::AddJobGreedyMakespanMachine(unsigned machine, unsigned new_job, unsigned mode_op)
{

    unsigned previos_job, best_position, decreased_time, increased_time;
    int difference, smaller_difference;
    smaller_difference = INT_MAX;

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina
    previos_job = 0;

    //Definir o modo de operação para a nova tarefa
    job_mode_op[new_job] = mode_op;

    //Tentar inserir antes de cada tarefa presente na máquina
    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        //Tempo de preparação que será reduzido
        decreased_time = Instance::SetupTime[machine][previos_job][*j];

        //Tempo de preparação que será incrementado
        increased_time = Instance::SetupTime[machine][previos_job][new_job];
        increased_time += Instance::SetupTime[machine][new_job][*j];

        //Diferença de tempo para inserção na posição j
        difference = increased_time - decreased_time;

        //Se a diferença é menor que a menor encontrada
        if(difference < smaller_difference){
            smaller_difference = difference;
            best_position = j - scheduling[machine].begin();
        }

        //Atualizar a tarefa anterior
        previos_job = *j;

    }

    //Tempo de preparação que será incrementado
    increased_time = Instance::SetupTime[machine][previos_job][new_job];

    difference = increased_time;

    if(difference < smaller_difference){
        smaller_difference = difference;
        best_position = scheduling[machine].size();
    }

    //Inserir a nova tarefa na última posição
    scheduling[machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[machine].size()-1; j > best_position; j--) {
        scheduling[machine][j] = scheduling[machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição
    scheduling[machine][best_position] = new_job;

}

/*
 * Inserir uma nova tarefa em uma máquina da solução atual,
 * escolher a posição que implique em menor custo total de energia nessa máquina
 */
void Solution::AddJobGreedyTECMachine(unsigned machine, unsigned new_job, unsigned mode_op)
{
    AddJobGreedyMakespanMachine(machine, new_job, mode_op);
}

/*
 * Inserir uma nova tarefa em todas as posições entre todas as máquinas
 * Escolher a posição e a máquina que gere menor makespan
 * e como critério de desempate, o menor tempo de término na máquina
 */
void Solution::AddJobGreedyMakespan(unsigned new_job, unsigned mode_op)
{
    unsigned previos_job, best_position, decreased_time, increased_time;
    unsigned best_machine;
    int completion_time, smaller_completion_time;
    smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::numMachine; i++) {

        //A tarefa fictícia zero é anterior à primeira tarefa da máquina i
        previos_job = 0;

        //Tentar inserir antes de cada tarefa presente na máquina
        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            //Tempo de preparação que será reduzido
            decreased_time = Instance::SetupTime[i][previos_job][*j];

            //Tempo de preparação que será incrementado
            increased_time = Instance::SetupTime[i][previos_job][new_job];
            increased_time += Instance::SetupTime[i][new_job][*j];

            //Tempo de processamento que será incrementado
            increased_time += ceil(Instance::ProcessingTime[i][new_job] /
                                   Instance::SpeedFactor[job_mode_op[new_job]]);

            //Tempo de termino da máquina i será igual ao valor atual mais
            //o custo da inserção da tarefa na posição j
            completion_time = this->machine_completion_time[i] + increased_time - decreased_time;

            //Se o novo tempo de término da máquina i é menor
            //que o melhor tempo de termíno encontra até agora,
            //então encontrei uma posição melhor para fazer a inserção
            if(completion_time < smaller_completion_time){
                smaller_completion_time = completion_time;
                best_position = j - scheduling[i].begin();
                best_machine = i;
            }

            //A tarefe atual passa a ser a tarefa anterior
            previos_job = *j;

        }

        //---Nessa parte será calculado o custo de inserir a nova tarefa após à última---

        //Tempo de preparação que será incrementado
        increased_time = Instance::SetupTime[i][previos_job][new_job];

        //Tempo de processamento que será incrementado
        increased_time += ceil(Instance::ProcessingTime[i][new_job] /
                               Instance::SpeedFactor[job_mode_op[new_job]]);

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        completion_time = this->machine_completion_time[i] + increased_time;

        //Se o novo tempo de término da máquina i é menor
        //que o melhor tempo de termíno encontra até agora,
        //então encontrei uma posição melhor para fazer a inserção
        if(completion_time < smaller_completion_time){
            smaller_completion_time = completion_time;
            best_position = scheduling[i].size();
            best_machine = i;
        }
    }

    //Inserir a nova tarefa na última posição
    scheduling[best_machine].push_back(new_job);

    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[best_machine].size()-1; j > best_position; j--) {
        scheduling[best_machine][j] = scheduling[best_machine][j-1];
    }

    //Alocar a nova tarefa na melhor posição identificada
    scheduling[best_machine][best_position] = new_job;
}

/*
 * Inserir uma nova tarefa em todas as posições entre todas as máquinas
 * Escolher a posição e a máquina que gere menor TEC
 */
void Solution::AddJobGreedyTEC(unsigned new_job, unsigned mode_op)
{

    unsigned pt, best_pt, best_machine;

    best_machine = 1;
    best_pt = ceil(Instance::ProcessingTime[best_machine][new_job] /
                                   Instance::SpeedFactor[job_mode_op[new_job]]);

    //Escolher a máquina que forneça o menor tempo de processamento
    for (unsigned i = 2; i <= Instance::numMachine; i++) {
        pt = ceil(Instance::ProcessingTime[1][new_job] /
                                       Instance::SpeedFactor[job_mode_op[new_job]]);
        if(pt < best_pt)
            best_machine = i;
    }

    //Adicionar a nova tarefa na melhor posição da máquina
    this->AddJobGreedyMakespanMachine(best_machine, new_job, mode_op);
}

void Solution::InsertRandomPosition(unsigned new_job)
{
    unsigned machine, job_position;

    //Escolher aleatoriamente uma máquina
    machine = 1 + rand()%Instance::numMachine;

    if(scheduling[machine].size() > 0){
        //Escolher aleatoriamente uma posição nesse máquina
        job_position = rand()%scheduling[machine].size();
    }
    else{
        job_position = 0;
    }


    //Inserir a nova tarefa na posição e na máquina escolhida
    scheduling[machine].insert(scheduling[machine].begin()+job_position, new_job);
}

void Solution::SwapInside(unsigned machine, unsigned pos_job1, unsigned pos_job2)
{
    unsigned aux;
    aux = this->scheduling[machine][pos_job1];
    this->scheduling[machine][pos_job1] = this->scheduling[machine][pos_job2];
    this->scheduling[machine][pos_job2] = aux;
}

