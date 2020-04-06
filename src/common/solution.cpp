#include "solution.h"

Solution::Solution()
{
    //Criar uma lista vazia para cada máquina
    scheduling = vector<vector<size_t>>(Instance::numMachine+1, vector<size_t>(0));

    //Criar um vetor para armazenar o instante de início de cada tarefa
    job_start_time = vector<size_t>(Instance::numJobs+1);

    //Criar um vetor para armazenar o instante de termino de cada tarefa
    job_end_time = vector<size_t>(Instance::numJobs+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    completion_time = vector<size_t>(Instance::numMachine+1);

    //Criar um vetor para armazenar o modo de operação de cada tarefa
    job_mode_op = vector<size_t>(Instance::numJobs+1);

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

    size_t i;

    vector<size_t> size_machine(Instance::numMachine+1);
    size_t aux = Instance::numJobs;

    //Gerar o tamanho de cada máquina
    for (i = 1; i < Instance::numMachine; ++i) {
        size_machine[i] = rand()%aux;
        aux -= size_machine[i];
    }
    size_machine[i] = aux;

    //Criar uma sequência
    vector<size_t> jobs(Instance::numJobs+1);
    for (size_t j = 1; j <= Instance::numJobs; ++j) {
        //Inserir a tarefa j na máquina i
        jobs[j] = j;
    }

    //Embaralhar as tarefas

    aux = rand()%(Instance::numJobs*Instance::numJobs);
    size_t i1, i2, temp;
    for (i = 0; i < aux; ++i) {
        i1 = 1+rand()%Instance::numJobs;
        i2 = 1+rand()%Instance::numJobs;
        temp = jobs[i1];
        jobs[i1] = jobs[i2];
        jobs[i2] = temp;
    }

    i = 1;
    //Inserir cada tarefa j na solução inicial
    for (size_t j = 1; j <= Instance::numJobs; ++j) {
        //Inserir a tarefa j na máquina i
        scheduling[i].push_back(jobs[j]);

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

#ifdef DEBUG
    Check();
#endif

}

void Solution::GreedyInitialSolution()
{

}

void Solution::CalculateInitialTimeMin()
{
    size_t start;
    size_t end, l;
    for(size_t i = 1; i <= Instance::numMachine; ++i){

        //O tempo de início da primeira tarefa de cada máquina é zero

        //A 1° tarefa começa no instante zero
        start = 0;

        //Percorrer até a penúltima tarefa
        for(auto j = scheduling[i].begin(); j != scheduling[i].end()-1; ++j){

            //Armazenar o tempo de início da tarefa j
            job_start_time[*j] = start;

            //Calcular o tempo de termino da tarefa j
            l = job_mode_op[*j];
            end = start + ceil(Instance::ProcessingTime[i][*j]/Instance::SpeedFactor[l]) +
                    Instance::SetupTime[i][*j][(*(j+1))];

            job_end_time[*j] = start = end;

        }

        //Calcular o tempo da última tarefa

        auto j = scheduling[i].end()-1;

        //Armazenar o tempo de início da tarefa j
        job_start_time[*j] = start;

        //Calcular o tempo de termino da tarefa j
        l = job_mode_op[*j];
        end = start + ceil(Instance::ProcessingTime[i][*j]/Instance::SpeedFactor[l]);

        job_end_time[*j] = end;

        completion_time[i] = end;
    }
}

void Solution::CalculateObjective()
{
    //Calcular o makespan
    makeSpan = 0;
    for (size_t i = 1; i <= Instance::numMachine; ++i) {
        if(completion_time[i] > makeSpan){
            makeSpan = completion_time[i];
        }
    }

    //Calcular o custo total de energia
    double PEC_on, PEC_off;
    double PEC_on_aux, PEC_off_aux;
    int l;

    PEC_on = PEC_off = 0;

    for(size_t i = 1; i <= Instance::numMachine; ++i){

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
    unsigned job, previous;
    double end_job;
    for (unsigned i = 1; i <= Instance::numMachine; i++) {
        for (unsigned j = 1; j < scheduling[i].size(); j++) {
            job = scheduling[i][j];
            previous = scheduling[i][j-1];
            //Uma tarefa deve começar depois de terminar da tarefa anterior
            /*end_job = job_start_time[previous] +
                    ceil(Instance::ProcessingTime[i][previous]/Instance::SpeedFactor[previous]) +
                    Instance::SetupTime[i][previous][job];*/
            end_job = job_start_time[previous] +
                    double(Instance::ProcessingTime[i][previous]) / Instance::SpeedFactor[job_mode_op[previous]] +
                    Instance::SetupTime[i][previous][job];
            if(job_start_time[job] < end_job){
                cout << "=========Erro==============" << endl;
                cout << "Uma tarefa não pode começar antes de terminar a anterior" << endl;

            }
        }
    }
}
