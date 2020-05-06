#include "solution.h"

Solution::Solution()
{
    //Criar uma lista vazia para cada máquina
    scheduling = vector<vector<unsigned>>(Instance::num_machine+1, vector<unsigned>(0));

    //Criar um vetor para armazenar o instante de início de processamento cada tarefa
    H1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante de início de cada tarefa
    job_start_time1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante do final de cada tarefa
    job_end_time1 = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    machine_completion_time = vector<unsigned>(Instance::num_machine+1);

    //Criar um vetor para armazenar o instante de término para cada máquina
    machine_TEC = vector<unsigned>(Instance::num_machine+1);

    //Criar um vetor para armazenar o modo de operação de cada tarefa
    job_mode_op = vector<unsigned>(Instance::num_jobs+1);

    //Criar um vetor para armazenar o tempo de processamento em cada máquina
    machine_processing_time = vector<unsigned>(Instance::num_machine+1);

    //Criar um vetor para armazenar o tempo de preparação em cada máquina
    machine_setup_time = vector<unsigned>(Instance::num_machine+1);

    //Instance de término de cada bloco em cada máquina
    //Serão 3 blocos em cada máquina (antes, durante e depois do horário de pico)
    block_end_time = vector<vector<unsigned>>(Instance::num_machine+1, vector<unsigned>(3));

    makeSpan = 0;
    for(unsigned i=1; i<=Instance::num_machine; i++){
        block_end_time[i][0] = 0;
        block_end_time[i][1] = Instance::peak_start;
        block_end_time[i][2] = Instance::peak_end;
    }
}

Solution::~Solution()
{
    this->scheduling.clear();
}

void Solution::Print()
{
    for (size_t i = 1; i <= Instance::num_machine; ++i) {
        cout << "Máquina " << i << ": ";
        for (auto it = scheduling[i].begin(); it != scheduling[i].end(); ++it) {
            cout << *it << " ";
        }
        cout << " (" << machine_completion_time[i] << ") " << endl;
    }
    cout << "Makespan: " << makeSpan << endl;
    cout << "TEC: " << TEC << endl;
    cout << endl;
}

void Solution::DummyInitialSolution()
{
    size_t i = 1;

    //Inserir cada tarefa j na solução inicial
    for (size_t j = 1; j <= Instance::num_jobs; ++j) {
        //Inserir a tarefa j na máquina i
        scheduling[i].push_back(j);

        //Definir o modo de operação de cada tarefa j, aleatoriamente
        job_mode_op[j] = 1 + rand()%(Instance::num_mode_op);

        //Distribuir as tarefas entre as máquinas, incrementar i
        if(j/i >= Instance::num_jobs/Instance::num_machine){

            //Mudar de máquina
            i++;
        }
    }

    CalculateInitialTimeMin();
    CalculateObjective();
}

void Solution::RandomInitialSolution()
{

    vector<unsigned> jobs(Instance::num_jobs+1);

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    random_shuffle(jobs.begin()+1, jobs.end());

    //Inserir cada tarefa através de uma estratégia aleatória
    for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

        //Definir o modo de operação de cada tarefa it, aleatoriamente
        job_mode_op[*it] = 1 + rand()%(Instance::num_mode_op);

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
    vector<unsigned> jobs(Instance::num_jobs+1);

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    random_shuffle(jobs.begin()+1, jobs.end());

    //Selecionar o modo de operação, mais rápido
    //Como ele é um fator, deve ser escolhido aquele de maior valor
    unsigned best_op_mode = 1;
    for(unsigned i = 2; i <= Instance::num_mode_op; i++){
        if(Instance::v_speed_factor[i] > Instance::v_speed_factor[best_op_mode]){
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
    vector<unsigned> jobs(Instance::num_jobs+1);

    for(unsigned i = 0; i <= Instance::num_jobs; i++){
        jobs[i] = i;
    }

    //Gerar um vetor com números aleatórios
    random_shuffle(jobs.begin()+1, jobs.end());

    //Selecionar o modo de operação, menor consumo
    unsigned best_op_mode = 1;
    for(unsigned i = 2; i <= Instance::num_mode_op; i++){
        if(Instance::v_consumption_factor[i]/Instance::v_speed_factor[i]
                < Instance::v_consumption_factor[best_op_mode]/Instance::v_speed_factor[best_op_mode]){
            best_op_mode = i;
        }
    }

    //Inserir cada tarefa através de uma estratégia gulosa
    for(auto it = jobs.begin()+1; it != jobs.end(); ++it){

        //Escolher a melhor posição para a tarefa it
        AddJobGreedyTEC(*it, best_op_mode);

    }

    //CalculateInitialTimeMin();
    //CalculateInitialTimeAvoidPeak();
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

    for(size_t i = 1; i <= Instance::num_machine; ++i){

        previous_job = 0;
        start = 0;

        //Percorrer as tarefas de cada máquina
        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){


            job_start_time1[*j] = start;

            //Tempo de preparação da tarefa j
            setup_time = Instance::m_setup_time[i][previous_job][(*j)];

            //Armazenar o tempo de início da tarefa j
            H1[*j] = job_start_time1[*j] + setup_time;

            machine_setup_time[i] += setup_time;

            //Tempo de processamento da tarefa j
            l = job_mode_op[*j];
            processing_time = ceil(Instance::m_processing_time[i][*j]/Instance::v_speed_factor[l]);
            machine_processing_time[i] += processing_time;

            //Identificar a tarefa anterior
            previous_job = *j;

            job_end_time1[*j] = H1[*j] + processing_time;

            //A próxima tarefa começa após a tarefa atual
            start = job_end_time1[*j];

        }

        //Calcular o instante de conclusão de cada máquina
        if(scheduling[i].size() > 0){

            last_job = scheduling[i].back();

            machine_completion_time[i] = job_end_time1[last_job];
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

    /*unsigned l;
    unsigned processing_time, setup_time, previous_job, total_time_job;
    unsigned end_block[3];

    for(size_t i = 1; i <= Instance::num_machine; ++i){

        end_block[0] = 0;
        end_block[1] = Instance::peak_start;
        end_block[2] = Instance::peak_end;

        previous_job = 0;

        //Percorrer as tarefas de cada máquina
        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            //Tempo de preparação da tarefa j
            setup_time = Instance::m_setup_time[i][previous_job][(*j)];
            machine_setup_time[i] += setup_time;

            //Tempo de processamento da tarefa j
            l = job_mode_op[*j];
            processing_time = ceil(Instance::m_processing_time[i][*j]/Instance::v_speed_factor[l]);
            machine_processing_time[i] += processing_time;


            total_time_job = setup_time + processing_time;

            //Verificar se a máquina suporta essa inserção antes do horário de pico
            if(end_block[0]+total_time_job <= block_end_time[i][0]){

                H[*j] = end_block[0];

                end_block[0] += total_time_job;

                job_end_time[*j] = end_block[0];

            }
            //Verificar se a máquina suporta essa inserção após do horário de pico
            else if(end_block[2]+total_time_job <= block_end_time[i][2]){

                job_start_time[*j] = end_block[2];

                end_block[2] += total_time_job;

                job_end_time[*j] = end_block[2];

            }
            //Verificar se a tarefa pode ser executada começando antes do horário de pico
            //e terminando dentro do horário de pico
            else if(end_block[1]==Instance::peak_start &&
                    end_block[0]+total_time_job < Instance::peak_end){


                job_start_time[*j] = end_block[0];

                end_block[1] = end_block[0]+total_time_job;
                end_block[0] = Instance::peak_start;

                job_end_time[*j] = end_block[1];


            }
            //Verificar se a tarefa pode ser executada começando antes do horário de pico
            //e terminando dentro do horizonte de planejamento
            else if(end_block[1]==Instance::peak_start &&
                    end_block[2]==Instance::peak_end &&
                    end_block[0]+total_time_job < Instance::num_planning_horizon){
                //Tarefa será executada parcialmente dentro e parcialmente fora do horário de pico

                job_start_time[*j] = end_block[0];

                end_block[2] = end_block[0]+total_time_job;
                end_block[1] = Instance::peak_end;
                end_block[0] = Instance::peak_start;

                job_end_time[*j] = end_block[2];

            }

            //A tarefa j passa a ser a tarefa anterior
            previous_job = *j;

        }
    }*/




    /*unsigned cost = 0;
    for(unsigned machine = 1; machine <= Instance::numMachine; machine++){
        cost += block_end_time[machine][0];
        cost += block_end_time[machine][1] - Instance::peakStart;
        cost += block_end_time[machine][2] - Instance::peakEnd;
    }*/


    /*unsigned completion_time_previos;
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

    }*/
}

void Solution::CalculateObjective()
{
    //Calcular o makespan
    makeSpan = 0;
    unsigned size;
    for (unsigned i = 1; i <= Instance::num_machine; ++i) {
        size = scheduling[i].size();
        if(size){
            unsigned last_job = scheduling[i][size-1];
            machine_completion_time[i] = job_end_time1[last_job];
            if(machine_completion_time[i] > makeSpan){
                makeSpan = machine_completion_time[i];
            }
        }
    }

    //Calcular o custo total de energia

    TEC = 0;

    for(unsigned i = 1; i <= Instance::num_machine; ++i){

        for(auto j = scheduling[i].begin(); j != scheduling[i].end(); ++j){

            TEC += CalcPECToJob(i, *j, H1[*j]);
        }
    }
}

/*
 * Calcular o custo de energia para a tarefa que está em position
 * na máquina machine, começando a ser preparado em start e termina
 * em end, após a execução
 */
double Solution::CalcPECToJob(unsigned machine, unsigned job, unsigned h)
{
    double PEC, PEC_on, PEC_off, pt, end;

    PEC_off = PEC_on = 0;

    pt = ceil(Instance::m_processing_time[machine][job]
            /Instance::v_speed_factor[job_mode_op[job]]);

    end = h + pt;

    //Se a tarefa começa antes do pico
    if(h < Instance::peak_start){

        //Se a tarefa termina antes do pico
        if(end < Instance::peak_start){

            PEC_off = pt;

            PEC_on = 0;
        }
        //Se a tarefa termina dentro do pico
        else if(end < Instance::peak_end){

            PEC_off = Instance::peak_start - h;

            PEC_on = end - Instance::peak_start;

        }
        else{

            PEC_off = Instance::peak_start - h;
            PEC_off += end - Instance::peak_end;

            PEC_on = Instance::peak_end - Instance::peak_start;

        }
    }
    //Se a tarefa começa dentro do horário de pico
    else if(h < Instance::peak_end){
        //Se a tarefa termina dentro do pico
        if(end < Instance::peak_end){
            PEC_off = 0;

            PEC_on = pt;
        }
        else{
            PEC_on = Instance::peak_end - h;

            PEC_off = end - Instance::peak_end;
        }
    }
    else{
        PEC_off = pt;

        PEC_on = 0;
    }

    PEC_off = (PEC_off*Instance::rate_off_peak*Instance::v_consumption_factor[job_mode_op[job]]*Instance::v_machine_potency[machine]/6);
    PEC_on = (PEC_on*Instance::rate_on_peak*Instance::v_consumption_factor[job_mode_op[job]]*Instance::v_machine_potency[machine]/6);

    PEC = PEC_off + PEC_on;

    return PEC;

}

void Solution::Check()
{
    unsigned job, previous, end_job, start_job;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        previous = 0;
        start_job = 0;
        for (unsigned j = 0; j < scheduling[i].size(); j++) {

            //Selecionar a tarefa atual
            job = scheduling[i][j];

            //Calcular o tempo de término da tarefa atual
            end_job = start_job +
                    ceil(Instance::m_processing_time[i][job]) / Instance::v_speed_factor[job_mode_op[job]] +
                    Instance::m_setup_time[i][previous][job];

            //Se o tempo calculado for diferente do valor armazenado, existe um erro
            if(job_start_time1[job] < start_job){
                cout << "=========Erro==============" << endl;
                cout << "Inicio da tarefa " << job << ", na máquina " << i << " calculado errado" << endl;

            }

            //Atualizar a informação da tarefa anterior e do instante de início
            previous = job;
            start_job = end_job;
        }
    }
}

unsigned Solution::FindJobBestPosMacMakespan(unsigned new_job, unsigned machine)
{

    unsigned previos_job, best_position, decreased_time, increased_time;
    unsigned completion_time, smaller_completion_time;

    smaller_completion_time = INT_MAX;

    //Retornar se não definiu o modo de operação de new_job
    if(job_mode_op[new_job] == 0){
        return scheduling[machine].size();
    }

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina i
    previos_job = 0;

    //Tentar inserir antes de cada tarefa presente na máquina
    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        //Tempo de processamento que será incrementado
        increased_time = ceil(Instance::m_processing_time[machine][new_job] /
                               Instance::v_speed_factor[job_mode_op[new_job]]);

        //Tempo de preparação que será incrementado
        increased_time += Instance::m_setup_time[machine][previos_job][new_job];
        increased_time += Instance::m_setup_time[machine][new_job][*j];


        //Tempo de preparação que será reduzido
        decreased_time = Instance::m_setup_time[machine][previos_job][*j];

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        completion_time = this->machine_completion_time[machine] + increased_time - decreased_time;

        //Se o novo tempo de término da máquina i é menor
        //que o melhor tempo de termíno encontra até agora,
        //então encontrei uma posição melhor para fazer a inserção
        if(completion_time < smaller_completion_time){
            smaller_completion_time = completion_time;
            best_position = j - scheduling[machine].begin();
        }

        //A tarefe atual passa a ser a tarefa anterior
        previos_job = *j;

    }

    //---Nessa parte será calculado o custo de inserir a nova tarefa após à última---

    //Tempo de processamento que será incrementado
    increased_time = ceil(Instance::m_processing_time[machine][new_job] /
                           Instance::v_speed_factor[job_mode_op[new_job]]);

    //Tempo de preparação que será incrementado
    increased_time += Instance::m_setup_time[machine][previos_job][new_job];

    //Tempo de termino da máquina i será igual ao valor atual mais
    //o custo da inserção da tarefa na posição j
    completion_time = this->machine_completion_time[machine] + increased_time;

    //Se o novo tempo de término da máquina i é menor
    //que o melhor tempo de termíno encontra até agora,
    //então encontrei uma posição melhor para fazer a inserção
    if(completion_time < smaller_completion_time){
        smaller_completion_time = completion_time;
        best_position = scheduling[machine].size();
    }

    return best_position;
}


/*
 * Método para encontrar a melhor posição para new_job em machine
 * considerando a minimização do custo de energia
 */
unsigned Solution::FindJobBestPosMacTEC(unsigned new_job, unsigned machine, unsigned &best_op)
{

    unsigned previos_job, proc_time_new_job, setup_time_diff, time_diff_new_job;
    unsigned best_position, op;
    double add_job_tec, smaller_add_job_tec;

    //Retornar se não definiu o modo de operação de new_job
    if(job_mode_op[new_job] == 0){
        return scheduling[machine].size();
    }

    smaller_add_job_tec = INT_MAX;
    best_position = INT_MAX;

    //A tarefa fictícia zero é anterior à primeira tarefa da máquina i
    previos_job = 0;

    //Tentar inserir antes de cada tarefa presente na máquina
    for(auto j = scheduling[machine].begin(); j != scheduling[machine].end(); ++j){

        //Tempo de processamento de new_job
        proc_time_new_job = ceil(Instance::m_processing_time[machine][new_job] /
                               Instance::v_speed_factor[job_mode_op[new_job]]);

        //Tempo de preparação que será incrementado
        setup_time_diff = Instance::m_setup_time[machine][previos_job][new_job];
        setup_time_diff += Instance::m_setup_time[machine][new_job][*j];

        //Tempo de preparação que será reduzido
        setup_time_diff -= Instance::m_setup_time[machine][previos_job][*j];

        //Custo de inserir a nova tarefa na posição j
        time_diff_new_job = proc_time_new_job + setup_time_diff;

        //Verificar se a máquina suporta essa inserção antes do horário de pico
        if(block_end_time[machine][0]+time_diff_new_job <= Instance::peak_start){
            //Tarefa completa executando fora do horário de pico
            op = 1;
            //Novo custo adicional
            add_job_tec = proc_time_new_job*Instance::rate_off_peak*Instance::v_machine_potency[machine];
        }
        //Verificar se a máquina suporta essa inserção após do horário de pico
        else if(block_end_time[machine][2]+time_diff_new_job <= Instance::num_planning_horizon){
            //A tarefa será executada fora do horário de pico
            op = 2;
            //Novo custo adicional
            add_job_tec = proc_time_new_job*Instance::rate_off_peak*Instance::v_machine_potency[machine];
        }
        //Se não tem nenhuma tarefa no horário de pico
        //Tentar começar a tarefa anter e terminar dentro do horário de pico
        else if(block_end_time[machine][1]==Instance::peak_start &&
                block_end_time[machine][0]+time_diff_new_job <= Instance::peak_end){

            op = 3;

            //Calcular o custo das tarefas anteriores a j
            unsigned setup, h;
            double machine_tec = 0;
            for(auto k = scheduling[machine].begin(); k != j; ++k){
                machine_tec += CalcPECToJob(machine, *k, H1[*k]);
            }

            setup = Instance::m_setup_time[machine][previos_job][new_job];
            h = job_start_time1[*j] + setup;
            machine_tec += CalcPECToJob(machine, new_job, h);

            //Calcular o custo das tarefas posteriores a j
            for(auto k = j; k != scheduling[machine].end(); ++k){
                setup = Instance::m_setup_time[machine][previos_job][*k];
                h = job_start_time1[*k] + setup + time_diff_new_job;
                machine_tec += CalcPECToJob(machine, *k, h);

                previos_job = *k;
            }

            add_job_tec = machine_tec - this->machine_TEC[machine];

        }
        //Se não tem nenhuma tarefa no horário de pico nem após o horário de pico
        //Tentar começar a tarefa antes do horário de pico
        //e terminar até o final do horizonte de planejamento
        else if(block_end_time[machine][1]==Instance::peak_start &&
                block_end_time[machine][2]==Instance::peak_end &&
                block_end_time[machine][0]+time_diff_new_job <= Instance::num_planning_horizon){
            //Tarefa será executada parcialmente dentro e parcialmente fora do horário de pico
            op = 4;

            //Calcular o custo das tarefas anteriores a j
            unsigned setup, h;
            double machine_tec = 0;
            for(auto k = scheduling[machine].begin(); k != j; ++k){
                machine_tec += CalcPECToJob(machine, *k, H1[*k]);
            }

            setup = Instance::m_setup_time[machine][previos_job][new_job];
            h = job_start_time1[*j] + setup;
            machine_tec += CalcPECToJob(machine, new_job, h);

            //Calcular o custo das tarefas posteriores a j
            for(auto k = j; k != scheduling[machine].end(); ++k){
                setup = Instance::m_setup_time[machine][previos_job][*k];
                h = job_start_time1[*k] + setup + time_diff_new_job;
                machine_tec += CalcPECToJob(machine, *k, h);

                previos_job = *k;
            }

            add_job_tec = machine_tec - this->machine_TEC[machine];

        }
        //Caso contrário a máquina não suporta a execução dessa tarefa
        else{
            op = 5;
            add_job_tec = INT_MAX;
        }

        //Se encontrou uma alocação com custo melhor
        if(add_job_tec < smaller_add_job_tec){
            smaller_add_job_tec = add_job_tec;
            best_position = j - scheduling[machine].begin();
            best_op = op;
        }

        //A tarefe atual passa a ser a tarefa anterior
        previos_job = *j;
    }

    //---Nessa parte será calculado o custo de inserir a nova tarefa após à última---

    //Tempo de processamento de new_job
    proc_time_new_job = ceil(Instance::m_processing_time[machine][new_job] /
                           Instance::v_speed_factor[job_mode_op[new_job]]);

    //Tempo de preparação que será incrementado
    setup_time_diff = Instance::m_setup_time[machine][previos_job][new_job];

    //Custo de inserir a nova tarefa na posição j
    time_diff_new_job = proc_time_new_job + setup_time_diff;

    //Verificar se a máquina suporta essa inserção antes do horário de pico
    if(block_end_time[machine][0]+time_diff_new_job <= Instance::peak_start){
        //Tarefa completa executando fora do horário de pico
        op = 1;
        //Novo custo adicional
        add_job_tec = proc_time_new_job*Instance::rate_off_peak*Instance::v_machine_potency[machine];
    }
    //Verificar se a máquina suporta essa inserção após do horário de pico
    else if(block_end_time[machine][2]+time_diff_new_job <= Instance::num_planning_horizon){
        //A tarefa será executada fora do horário de pico
        op = 2;
        //Novo custo adicional
        add_job_tec = proc_time_new_job*Instance::rate_off_peak*Instance::v_machine_potency[machine];
    }
    //Se não tem nenhuma tarefa no horário de pico
    //Tentar começar a tarefa anter e terminar dentro do horário de pico
    else if(block_end_time[machine][1]==Instance::peak_start &&
            block_end_time[machine][0]+time_diff_new_job <= Instance::peak_end){

        op = 3;

        //Calcular o custo das tarefas anteriores a j
        unsigned setup, h;
        double machine_tec = 0;

        for(auto k = scheduling[machine].begin(); k != scheduling[machine].end(); ++k){
            machine_tec += CalcPECToJob(machine, *k, H1[*k]);
        }

        setup = Instance::m_setup_time[machine][previos_job][new_job];
        h = job_start_time1[previos_job] + setup;
        machine_tec += CalcPECToJob(machine, new_job, h);

        add_job_tec = machine_tec - this->machine_TEC[machine];

    }
    //Se não tem nenhuma tarefa no horário de pico nem após o horário de pico
    //Tentar começar a tarefa antes do horário de pico
    //e terminar até o final do horizonte de planejamento
    else if(block_end_time[machine][1]==Instance::peak_start &&
            block_end_time[machine][2]==Instance::peak_end &&
            block_end_time[machine][0]+time_diff_new_job <= Instance::num_planning_horizon){
        //Tarefa será executada parcialmente dentro e parcialmente fora do horário de pico
        op = 4;

        //Calcular o custo das tarefas anteriores a j
        unsigned setup, h;
        double machine_tec = 0;

        for(auto k = scheduling[machine].begin(); k != scheduling[machine].end(); ++k){
            machine_tec += CalcPECToJob(machine, *k, H1[*k]);
        }

        setup = Instance::m_setup_time[machine][previos_job][new_job];
        h = job_start_time1[previos_job] + setup;
        machine_tec += CalcPECToJob(machine, new_job, h);

        add_job_tec = machine_tec - this->machine_TEC[machine];

    }
    //Caso contrário a máquina não suporta a execução dessa tarefa
    else{
        op = 5;
        add_job_tec = INT_MAX;
    }

    //Se encontrou uma alocação com custo melhor
    if(add_job_tec < smaller_add_job_tec){
        smaller_add_job_tec = add_job_tec;
        best_position = scheduling[machine].size();
        best_op = op;
    }

    return best_position;

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
        decreased_time = Instance::m_setup_time[machine][previos_job][*j];

        //Tempo de preparação que será incrementado
        increased_time = Instance::m_setup_time[machine][previos_job][new_job];
        increased_time += Instance::m_setup_time[machine][new_job][*j];

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
    increased_time = Instance::m_setup_time[machine][previos_job][new_job];

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
    //AddJobGreedyMakespanMachine(machine, new_job, mode_op);
    unsigned best_position, previos_job;
    unsigned h, best_op;
    double new_job_tec, best_time_diff_new_job;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    best_position = 0;

    //Percorrer cada máquina
    //for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position = FindJobBestPosMacTEC(new_job, machine, best_op);

        //Se não encontrou nenhuma posição viável
        if(best_position > scheduling[machine].size()){
            return;
        }

        //Identificar a tarefa anterior
        if(best_position == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[machine][best_position-1];
        }

        h = job_end_time1[previos_job] + Instance::m_setup_time[machine][previos_job][new_job];

        new_job_tec = CalcPECToJob(machine, new_job, h);

    //}

    /*--------Fazer a inserção---*/
    //machine_setup_time[best_machine] += smaller_setup_time;
    //machine_processing_time[best_machine] += smaller_processing_time;

    //Inserir a nova tarefa na última posição
    scheduling[machine].push_back(new_job);


    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[machine].size()-1; j > best_position; j--) {

        scheduling[machine][j] = scheduling[machine][j-1];

    }

    machine_TEC[machine] += new_job_tec;

    //Atualizar start, h e end
    scheduling[machine][best_position] = new_job;

    //Identificar a tarefa anterior
    if(best_position == 0){
        previos_job = 0;
    }
    else{
        previos_job = scheduling[machine][best_position-1];
    }

    job_start_time1[new_job] = job_end_time1[previos_job];
    H1[new_job] = job_start_time1[new_job] + Instance::m_setup_time[machine][previos_job][new_job];
    job_end_time1[new_job] = H1[new_job] + ceil(Instance::m_processing_time[machine][new_job] /
                                                Instance::v_speed_factor[job_mode_op[new_job]]);
    previos_job = new_job;

    //Percorrer as tarefas que estão após new_job
    unsigned job;
    for(unsigned j=best_position+1; j<scheduling[machine].size();j++){

        job = scheduling[machine][j];

        job_start_time1[job] = job_end_time1[previos_job];
        H1[job] = job_start_time1[job] + Instance::m_setup_time[machine][previos_job][job];
        job_end_time1[job] = H1[job] + ceil(Instance::m_processing_time[machine][job] /
                                        Instance::v_speed_factor[job_mode_op[new_job]]);
        previos_job = job;
    }

    //Atualizar blocos
    unsigned diff_time_new_job, next_job;
    //Tempo de processamento de new_job
    diff_time_new_job = job_end_time1[new_job] - H1[new_job];
    //Tempo de preparação de new_job
    if(best_position == 0){
        previos_job = 0;
    }
    else{
        previos_job = scheduling[machine][best_position-1];
    }
    diff_time_new_job += Instance::m_setup_time[machine][previos_job][new_job];
    //Identifica se tem alguma tarefa depois
    if(best_position+1 < scheduling[machine].size()){
        next_job = scheduling[machine][best_position+1];
        diff_time_new_job += Instance::m_setup_time[machine][new_job][next_job];
        diff_time_new_job -= Instance::m_setup_time[machine][previos_job][next_job];
    }

    unsigned last_job = scheduling[machine].back();
    machine_completion_time[machine] = job_end_time1[last_job];

    //Se new_job começa e termina antes do horário de pico
    if(best_op == 1){
        //op 1
        block_end_time[machine][0] += diff_time_new_job;

    }
    //Se new_job começa e termina após o horário de pico
    else if(best_op == 2){
        //op 2
        block_end_time[machine][2] += diff_time_new_job;
    }
    //Se new_job começa antes do horário de pico e termina dentro do horário de pico
    else if(best_op == 3){
        //op 3
        block_end_time[machine][1] = (block_end_time[machine][0]+diff_time_new_job);
        block_end_time[machine][0] = Instance::peak_start;
    }
    else if(best_op == 4){
        //op 4
        block_end_time[machine][2] = (block_end_time[machine][0]+diff_time_new_job);
        block_end_time[machine][0] = Instance::peak_start;
        block_end_time[machine][1] = Instance::peak_end;
    }
}

/*
 * Inserir uma nova tarefa em todas as posições entre todas as máquinas
 * Escolher a posição e a máquina que gere menor makespan
 * e como critério de desempate, o menor tempo de término na máquina
 */
void Solution::AddJobGreedyMakespan(unsigned new_job, unsigned mode_op)
{
    unsigned best_position_machine, best_position, completion_time, difference;
    unsigned previos_job, next_job, best_machine;
    unsigned smaller_completion_time = INT_MAX;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacMakespan(new_job, i);

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        //Identificar a tarefa seguinte
        if(best_position_machine < scheduling[i].size()){
            next_job = scheduling[i][best_position_machine];
        }
        else{
            next_job = 0;
        }

        //Tempo de processamento de new_job que será incrementado
        difference = ceil(Instance::m_processing_time[i][new_job] /
                               Instance::v_speed_factor[job_mode_op[new_job]]);

        //Tempo de preparação antes de new_job que será incrementado
        difference += Instance::m_setup_time[i][previos_job][new_job];

        if(next_job > 0){
            //Tempo de preparação após new_job que será incrementado
            difference += Instance::m_setup_time[i][new_job][next_job];

            //Tempo de preparação entre previos_job e next_job que será reduzido
            difference -= Instance::m_setup_time[i][previos_job][next_job];
        }

        //Tempo de termino da máquina i será igual ao valor atual mais
        //o custo da inserção da tarefa na posição j
        completion_time = this->machine_completion_time[i] + difference;

        if(completion_time < smaller_completion_time){
            smaller_completion_time = completion_time;
            best_machine = i;
            best_position = best_position_machine;
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

    this->machine_completion_time[best_machine] = smaller_completion_time;
}

/*
 * Inserir uma nova tarefa em todas as posições entre todas as máquinas
 * Escolher a posição e a máquina que gere menor TEC
 */
void Solution::AddJobGreedyTEC(unsigned new_job, unsigned mode_op)
{

    unsigned best_position_machine, best_position, best_machine, previos_job;
    unsigned h, best_op;
    double new_job_tec, smaller_new_job_tec, proc_time_new_job;

    //Definir o modo de operação da nova tarefa
    job_mode_op[new_job] = mode_op;

    smaller_new_job_tec = INT_MAX;
    best_machine = best_position_machine = 0;

    best_op = 5;

    //Percorrer cada máquina
    for (unsigned i = 1; i <= Instance::num_machine; i++) {

        //Encontrar a melhor posição na máquina i
        best_position_machine = FindJobBestPosMacTEC(new_job, i, best_op);

        //Se não encontrou nenhuma posição viável
        if(best_position_machine > scheduling[i].size()){
            continue;
        }

        //Identificar a tarefa anterior
        if(best_position_machine == 0){
            previos_job = 0;
        }
        else{
            previos_job = scheduling[i][best_position_machine-1];
        }

        h = job_end_time1[previos_job] + Instance::m_setup_time[i][previos_job][new_job];

        proc_time_new_job = ceil(Instance::m_processing_time[i][new_job] /
                               Instance::v_speed_factor[job_mode_op[new_job]]);

        if(h + proc_time_new_job <= Instance::num_planning_horizon){

            new_job_tec = CalcPECToJob(i, new_job, h);

            if(new_job_tec < smaller_new_job_tec){
                smaller_new_job_tec = new_job_tec;
                best_machine = i;
                best_position = best_position_machine;
            }
        }

    }

    /*if(best_op == 5){
        cout << "best_position: " << best_position << endl;
    }*/

    /*--------Fazer a inserção---*/
    //machine_setup_time[best_machine] += smaller_setup_time;
    //machine_processing_time[best_machine] += smaller_processing_time;

    //Se não encontrou nenhuma posição para realizar a inserção
    //if(best_machine == 0){
    if(best_position > scheduling[best_machine].size()){

        unsigned next_job, difference, completion_time, smaller_completion_time;

        smaller_completion_time = INT_MAX;

        //Percorrer cada máquina
        for (unsigned i = 1; i <= Instance::num_machine; i++) {

            //Encontrar a melhor posição na máquina i
            best_position_machine = FindJobBestPosMacMakespan(new_job, i);

            //Identificar a tarefa anterior
            if(best_position_machine == 0){
                previos_job = 0;
            }
            else{
                previos_job = scheduling[i][best_position_machine-1];
            }

            //Identificar a tarefa seguinte
            if(best_position_machine < scheduling[i].size()){
                next_job = scheduling[i][best_position_machine];
            }
            else{
                next_job = 0;
            }

            //Tempo de processamento de new_job que será incrementado
            difference = ceil(Instance::m_processing_time[i][new_job] /
                                   Instance::v_speed_factor[job_mode_op[new_job]]);

            //Tempo de preparação antes de new_job que será incrementado
            difference += Instance::m_setup_time[i][previos_job][new_job];

            if(next_job > 0){
                //Tempo de preparação após new_job que será incrementado
                difference += Instance::m_setup_time[i][new_job][next_job];

                //Tempo de preparação entre previos_job e next_job que será reduzido
                difference -= Instance::m_setup_time[i][previos_job][next_job];
            }

            //Tempo de termino da máquina i será igual ao valor atual mais
            //o custo da inserção da tarefa na posição j
            completion_time = this->machine_completion_time[i] + difference;

            //Se encontrou uma posição melhor
            //if(completion_time < smaller_completion_time){

                //Se a posição encontrada excede o horizonte de planejamento
                if(completion_time > Instance::num_planning_horizon){

                    //Guardar o custo da melhor posição encontrada
                    smaller_completion_time = completion_time;

                    difference =
                            (ceil(Instance::m_processing_time[i][new_job] /
                                  Instance::v_speed_factor[job_mode_op[new_job]]))
                            -
                            (ceil(Instance::m_processing_time[i][new_job] /
                                  Instance::v_speed_factor[job_mode_op[new_job]-1]));
                    job_mode_op[new_job]--;

                    smaller_completion_time -= difference;

                    if(smaller_completion_time > Instance::num_planning_horizon){
                        difference =
                                (ceil(Instance::m_processing_time[i][new_job] /
                                      Instance::v_speed_factor[job_mode_op[new_job]]))
                                -
                                (ceil(Instance::m_processing_time[i][new_job] /
                                      Instance::v_speed_factor[job_mode_op[new_job]-1]));
                        job_mode_op[new_job]--;

                    }
                }

                smaller_completion_time = completion_time;
                best_machine = i;
                best_position = best_position_machine;
            //}

        }
    }

    //Inserir a nova tarefa na última posição
    scheduling[best_machine].push_back(new_job);


    //Deslocar as tarefas que estão após da melhor posição
    for (unsigned j = scheduling[best_machine].size()-1; j > best_position; j--) {

        scheduling[best_machine][j] = scheduling[best_machine][j-1];

    }

    machine_TEC[best_machine] += smaller_new_job_tec;

    //Atualizar start, h e end
    scheduling[best_machine][best_position] = new_job;

    //Identificar a tarefa anterior
    if(best_position == 0){
        previos_job = 0;
    }
    else{
        previos_job = scheduling[best_machine][best_position-1];
    }

    job_start_time1[new_job] = job_end_time1[previos_job];
    H1[new_job] = job_start_time1[new_job] + Instance::m_setup_time[best_machine][previos_job][new_job];
    job_end_time1[new_job] = H1[new_job] + ceil(Instance::m_processing_time[best_machine][new_job] /
                                                Instance::v_speed_factor[job_mode_op[new_job]]);
    previos_job = new_job;

    //Percorrer as tarefas que estão após new_job
    unsigned job;
    for(unsigned j=best_position+1; j<scheduling[best_machine].size();j++){

        job = scheduling[best_machine][j];

        job_start_time1[job] = job_end_time1[previos_job];
        H1[job] = job_start_time1[job] + Instance::m_setup_time[best_machine][previos_job][job];
        job_end_time1[job] = H1[job] + ceil(Instance::m_processing_time[best_machine][job] /
                                        Instance::v_speed_factor[job_mode_op[new_job]]);
        previos_job = job;
    }

    //Atualizar blocos
    unsigned diff_time_new_job, next_job;
    //Tempo de processamento de new_job
    diff_time_new_job = job_end_time1[new_job] - H1[new_job];
    //Tempo de preparação de new_job
    if(best_position == 0){
        previos_job = 0;
    }
    else{
        previos_job = scheduling[best_machine][best_position-1];
    }
    diff_time_new_job += Instance::m_setup_time[best_machine][previos_job][new_job];
    //Identifica se tem alguma tarefa depois
    if(best_position+1 < scheduling[best_machine].size()){
        next_job = scheduling[best_machine][best_position+1];
        diff_time_new_job += Instance::m_setup_time[best_machine][new_job][next_job];
        diff_time_new_job -= Instance::m_setup_time[best_machine][previos_job][next_job];
    }

    unsigned last_job = scheduling[best_machine].back();
    machine_completion_time[best_machine] = job_end_time1[last_job];

    //Se new_job começa e termina antes do horário de pico
    if(best_op == 1){
        //op 1
        block_end_time[best_machine][0] += diff_time_new_job;

    }
    //Se new_job começa e termina após o horário de pico
    else if(best_op == 2){
        //op 2
        block_end_time[best_machine][2] += diff_time_new_job;
    }
    //Se new_job começa antes do horário de pico e termina dentro do horário de pico
    else if(best_op == 3){
        //op 3
        block_end_time[best_machine][1] = (block_end_time[best_machine][0]+diff_time_new_job);
        block_end_time[best_machine][0] = Instance::peak_start;
    }
    else if(best_op == 4){
        //op 4
        block_end_time[best_machine][2] = (block_end_time[best_machine][0]+diff_time_new_job);
        block_end_time[best_machine][0] = Instance::peak_start;
        block_end_time[best_machine][1] = Instance::peak_end;
    }

}

void Solution::InsertRandomPosition(unsigned new_job)
{
    unsigned machine, job_position;

    //Escolher aleatoriamente uma máquina
    machine = 1 + rand()%Instance::num_machine;

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

