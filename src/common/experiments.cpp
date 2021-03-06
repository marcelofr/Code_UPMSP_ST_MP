#include "experiments.h"

void RunAlgorithm(algorithm_data alg_data){

    Timer *t1 = new Timer();

    //Make trace log in file (.trace.log)
    MakeTrace();

    //Ler a instância
    //Instance::ReadJulioInstance(Param.instance_file);
    Instance::ReadMarceloInstance(alg_data.param.instance_file);

    Instance::seed = alg_data.param.u_seed;

    Discretize(1);

    vector<Solution*> nd_set_solution;

    alg_data.is_optimal = false;

    t1->start();

    //Definir o tempo limite de execução
    alg_data.time_limit = alg_data.param.u_max_time_factor * Instance::num_jobs * log(Instance::num_machine);

    #ifdef DEBUG
        cout << "Tempo limite: " << alg_data.time_limit << endl;
    #endif

    if(alg_data.param.algorithm_name == "GA"){
        RunAlgorithmNSGAII(alg_data, nd_set_solution, t1);
    }
    else if(alg_data.param.algorithm_name == "EXACT"){
        RunAlgorithmExact(alg_data, nd_set_solution);
    }
    else if(alg_data.param.algorithm_name == "LS"){
        RunAlgorithmHillClimbing(nd_set_solution, alg_data, t1);
    }
    else if(alg_data.param.algorithm_name == "MOVNS"){
        RunAlgorithmMOVNS(alg_data, nd_set_solution, t1);
    }
    else if(alg_data.param.algorithm_name == "MOVNS_Arroyo"){
        RunAlgorithmMOVNSArroyo(alg_data, nd_set_solution, t1);
    }
    else if(alg_data.param.algorithm_name == "MOVNS_Eduardo"){
        RunAlgorithmMOVNSEduardo(alg_data, nd_set_solution, t1);
    }
    else if(alg_data.param.algorithm_name == "MOVNS_D"){
        RunAlgorithmMono(alg_data, nd_set_solution, t1);
    }

    t1->stop();

    //Eliminar soluções que estão fora do horizonte de planejamento
    SelectOnlyValidSolutions(nd_set_solution);

    alg_data.elapsed_time_sec = t1->getElapsedTimeInMilliSec();
    alg_data.non_dominated_set.clear();
    pair<unsigned, double> aux;
    for(auto it:nd_set_solution){
        #ifdef DEBUG
            it->Check();
        #endif
        aux.first = it->makeSpan*Instance::discretization_factor;
        aux.second = it->TEC*Instance::discretization_factor;
        alg_data.non_dominated_set.push_back(aux);
    }

    //Ordenar os pontos
    SortByMakespan(alg_data.non_dominated_set);

    #ifndef IRACE
        //Salvar o conjunto não-dominado em arquivo
        SalveFileSolution(alg_data);
    #endif

    #ifdef IRACE
        pair<unsigned, double> reference_point;
        double hv;
        /*reference_point.first = alg_data.non_dominated_set.back().first;
        reference_point.second = alg_data.non_dominated_set.front().second;*/
        reference_point.first = Instance::max_makespan;
        reference_point.second = Instance::max_energy_cost;
        hv = CalculateHypervolumeMin(alg_data.non_dominated_set, reference_point);
        //cout << hv << " " << ir.elapsed_time_sec << endl;
        cout << hv << endl;
    #endif

    delete t1;

}

void RunAlgorithmNSGAII(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){

    //vector<GASolution*> non_dominated_set_ga;
    NDSetSolution<GASolution *> *non_dominated_set_ga = new NDSetSolution<GASolution *>();

    //GenerateInitialPopulation(non_dominated_set_ga, alg_data.param.u_population_size);
    //GenerateInitialPopulation(non_dominated_set_ga->set_solution, alg_data.param.u_population_size);
    //non_dominated_set_ga->GenerateGreedyInitialSolution();

    //non_dominated_set_ga->ConstrutiveGreedy();
    //non_dominated_set_ga->ConstrutiveRandom(alg_data.param.u_population_size);
    //non_dominated_set_ga->ContrutiveGRASP(1.0, alg_data.param.u_population_size);
    non_dominated_set_ga->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);
    //non_dominated_set_ga->ContrutiveGRASPRandon(0.5, alg_data.param.u_population_size);
    //non_dominated_set_ga->ContrutiveGRASPGreedy(0.5, alg_data.param.u_population_size);
    //non_dominated_set_ga->ContrutiveGRASPGreedyRandon(0.0, alg_data.param.u_population_size);
    //non_dominated_set_ga->ConstructiveCombinatorialSolution();


    #ifdef DEBUG
        cout << "===========Inicio População Inicial===========" << endl;
        PrintPopulation(non_dominated_set_ga->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        cout << "===========Fim População Inicial===========" << endl << endl;
    #endif

    //alg_data.time_limit=0;
    nsga_ii(alg_data, non_dominated_set_ga->set_solution, t1);

    #ifdef DEBUG
        cout << "===========Inicio NSGA===========" << endl;
        PrintPopulation(non_dominated_set_ga->set_solution);
        //non_dominated_set_ga->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim NSGA===========" << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:non_dominated_set_ga->set_solution){
        non_dominated_set.push_back(it);
    }
}

void RunAlgorithmExact(algorithm_data alg_data, vector<Solution*> &non_dominated_set){

    MonoSolution * my_solution;

    //Gerar uma solução inicial gulosa considerando o objetivo do makespan
    my_solution = new MonoSolution();

    my_solution->weights.first = alg_data.param.d_alpha;
    my_solution->weights.second = 1-alg_data.param.d_alpha;

    my_solution->GenerateGreedySolutionWeigth();

    //Modelo ponderado
    RunWeightedMathModel(alg_data.time_limit, alg_data.param.d_alpha, my_solution);

    //Modelo espsilon-restrito
    /*if(true){
        my_solution->GenerateGreedySolutionMakespan();
        RunEpsilonMathModel(alg_data.time_limit, my_solution->makeSpan, 0, my_solution);
    }
    else{
        my_solution->GenerateGreedySolutionTEC3();
        RunEpsilonMathModel(alg_data.time_limit, 0, my_solution->TEC, my_solution);
    }*/

    non_dominated_set.push_back(my_solution);

    if(my_solution->is_optimal){
        alg_data.is_optimal = true;
    }

    alg_data.param.file_solution = alg_data.param.folder_solution + alg_data.param.algorithm_name + "_" + alg_data.param.instance_name + "_" + alg_data.param.s_alpha + ".sol";
}


void RunAlgorithmHillClimbing(vector<Solution*> &non_dominated_set, algorithm_data alg_data, Timer *t1){

    NDSetSolution<LSSolution *> *non_dominated_set_ls = new NDSetSolution<LSSolution *>();

    non_dominated_set_ls->ConstrutiveGreedy();
    //non_dominated_set_ls->ConstructiveCombinatorialSolution();
    //non_dominated_set_ls->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);

    #ifdef DEBUG
        cout << "===========Inicio Solução Inicial===========" << endl;
        non_dominated_set_ls->PrintSetSolution();
        cout << "===========Fim Solução Inicial===========" << endl << endl;
    #endif

    HillClimbing(*non_dominated_set_ls, alg_data, t1);

    #ifdef DEBUG
        cout << "===========Inicio HillClimbing===========" << endl;
        non_dominated_set_ls->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim HillClimbing===========" << endl << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:non_dominated_set_ls->set_solution){
        non_dominated_set.push_back(it);
    }

    delete non_dominated_set_ls;

}

void RunAlgorithmMOVNS(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){

    NDSetSolution<LSSolution *> *non_dominated_set_ls = new NDSetSolution<LSSolution *>();

    //non_dominated_set_ls->ConstrutiveGreedy();
    //non_dominated_set_ls->ConstrutiveRandom(1);
    non_dominated_set_ls->ContrutiveGRASP(0.5, alg_data.param.u_population_size, 1);

    #ifdef DEBUG
        cout << "===========Inicio Solução Inicial===========" << endl;
        non_dominated_set_ls->PrintSetSolution();
        cout << "===========Fim Solução Inicial===========" << endl << endl;
    #endif


    MOVNS(*non_dominated_set_ls, alg_data, t1);

    #ifdef DEBUG
        cout << "===========Inicio MOVNS===========" << endl;
        non_dominated_set_ls->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim MOVNS===========" << endl << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:non_dominated_set_ls->set_solution){
        non_dominated_set.push_back(it);
    }

    delete non_dominated_set_ls;
}

void RunAlgorithmMOVNSArroyo(algorithm_data alg_data, vector<Solution*> &nd_set_solution, Timer *t1){

    NDSetSolution<LSSolution*> *obj_nd_set_solution = new NDSetSolution<LSSolution*>();

    obj_nd_set_solution->ConstrutiveGreedy();
    //obj_nd_set_solution->ConstructiveCombinatorialSolution();
    //obj_nd_set_solution->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);
    //obj_nd_set_solution->ContrutiveGRASP(0.5, alg_data.param.u_population_size, 1);
    //obj_nd_set_solution->ConstrutiveRandom(100);

    #ifdef DEBUG
        cout << "===========Inicio Solução Inicial===========" << endl;
        obj_nd_set_solution->PrintSetSolution();
        cout << "===========Fim Solução Inicial===========" << endl << endl;
    #endif

    alg_data.qtd_neighbor = QTD_NEIGHBOR;

    MOVNS_Arroyo(*obj_nd_set_solution, alg_data, t1);

    #ifdef DEBUG
        cout << "===========Inicio MOVNS Arroyo===========" << endl;
        SortByMakespanLSSolution(obj_nd_set_solution->set_solution);
        obj_nd_set_solution->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim MOVNS Arroyo===========" << endl << endl;
    #endif

    nd_set_solution.clear();
    for(auto it:obj_nd_set_solution->set_solution){
        nd_set_solution.push_back(it);
    }

    delete obj_nd_set_solution;
}

void RunAlgorithmMOVNSEduardo(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){

    NDSetSolution<LSSolution *> *non_dominated_set_ls = new NDSetSolution<LSSolution *>();

    non_dominated_set_ls->ContrutiveGRASP(0.5, alg_data.param.u_population_size/2, 1);
    //non_dominated_set_ls->ConstrutiveGreedyAndRandom(alg_data.param.u_population_size);
    //non_dominated_set_ls->ConstrutiveGreedy();
    //non_dominated_set_ls->ConstructiveCombinatorialSolution();

    #ifdef DEBUG
        cout << "===========Inicio Solução Inicial===========" << endl;
        non_dominated_set_ls->PrintSetSolution();
        cout << "===========Fim Solução Inicial===========" << endl << endl;
    #endif

    alg_data.qtd_neighbor = 5;
    alg_data.max_shake_level = ceil(double(Instance::num_jobs)/double(10));

    MOVNS_Eduardo(*non_dominated_set_ls, alg_data, t1);

    #ifdef DEBUG
        cout << "===========Inicio MOVNS Eduardo===========" << endl;
        SortByMakespanLSSolution(non_dominated_set_ls->set_solution);
        non_dominated_set_ls->PrintSetSolution();
        t1->printElapsedTimeInMilliSec();
        cout << "===========Fim MOVNS Eduardo===========" << endl << endl;
    #endif

    non_dominated_set.clear();
    for(auto it:non_dominated_set_ls->set_solution){
        non_dominated_set.push_back(it);
    }

    delete non_dominated_set_ls;
}

void RunAlgorithmMono(algorithm_data alg_data, vector<Solution*> &non_dominated_set, Timer *t1){

    NDSetSolution<MonoSolution *> *non_dominated_set_ms = new NDSetSolution<MonoSolution *>();

    //non_dominated_set_ms->ContrutiveGRASP(0.5, 4, 0);
    //non_dominated_set_ms->ConstrutiveGreedyAndRandom(3);
    //non_dominated_set_ms->ConstrutiveGreedy();
    //non_dominated_set_ms->ConstructiveCombinatorialSolution();
    //non_dominated_set_ms->ConstrutiveRandom(10);

    unsigned sz = alg_data.param.u_decomposition_size;

    //Tamanho de cada grupo
    alg_data.num_group = alg_data.param.u_decomposition_neighboor_size;

    //Quantidade de vizinhanças 5+1
    alg_data.qtd_neighbor = QTD_NEIGHBOR;

    alg_data.num_weights = sz;
    non_dominated_set_ms->ConstrutiveGreedyWeight(sz);

    /*non_dominated_set_ms->ConstructiveCombinatorialSolution();
    alg_data.num_weights = non_dominated_set_ms->set_solution.size();*/


    #ifdef DEBUG
        cout << "===========Inicio Solução Inicial===========" << endl;
        non_dominated_set_ms->PrintSetSolution();
        cout << "===========Fim Solução Inicial===========" << endl << endl;

        cout << "Tempo limite: " << alg_data.time_limit << endl;
    #endif

    //SortByMakespanMonoSolution(non_dominated_set_ms->set_solution);

    //SetWeights(*non_dominated_set_ms);
    MOVNS_D(*non_dominated_set_ms, alg_data, t1);

    NDSetSolution<MonoSolution *> *non_dominated_set_local = new NDSetSolution<MonoSolution *>();

    for(auto it:non_dominated_set_ms->set_solution){
        non_dominated_set_local->AddSolution(it);
    }

#ifdef DEBUG
    cout << "===========Inicio MOVNS Eduardo===========" << endl;
    t1->stop();
    non_dominated_set_local->PrintSetSolution();
    t1->printElapsedTimeInMilliSec();
    cout << "===========Fim MOVNS Eduardo===========" << endl << endl;
#endif

    non_dominated_set.clear();
    for(auto it:non_dominated_set_local->set_solution){
        non_dominated_set.push_back(it);
    }

    delete non_dominated_set_ms;
}

void Discretize(unsigned factor){

    /*Fazer a discretização do tempo*/
    Instance::discretization_factor = factor;
    Instance::num_planning_horizon = ceil(double(Instance::num_planning_horizon+1)
                                      /double(Instance::discretization_factor))-1;
    for(unsigned i=0; i<Instance::num_days; i++){
        Instance::v_peak_start[i] = ceil(double(Instance::v_peak_start[i])
                                        /double(Instance::discretization_factor));
        Instance::v_peak_end[i] = ceil(double(Instance::v_peak_end[i]+1)
                                   /double(Instance::discretization_factor))-1;
    }
    for(unsigned i=1; i<=Instance::num_machine; i++){
        for(unsigned j=1; j<=Instance::num_jobs; j++){
            Instance::m_processing_time[i][j] = ceil(double(Instance::m_processing_time[i][j])
                                                     /double(Instance::discretization_factor));
            for(unsigned k=1; k<=Instance::num_jobs; k++){
                Instance::m_setup_time[i][j][k] = ceil(double(Instance::m_setup_time[i][j][k])
                                                       /double(Instance::discretization_factor));
            }
        }
    }
    Instance::max_energy_cost = ceil(double(Instance::max_energy_cost)
                                     /double(Instance::discretization_factor));
    Instance::max_makespan = ceil(double(Instance::max_makespan)
                                  /double(Instance::discretization_factor));

}

/*
 * Método para salvar em arquivo a solução encontrada por um algoritmo
 */
void SalveFileSolution(algorithm_data alg_data){

    ofstream MyFile;

    //Tentar abrir um arquivo existente
    MyFile.open(alg_data.param.file_solution, ios_base::out | ios_base::in | ios_base::ate);  // will not create file

    //Se o arquivo não existe então criar um novo
    if (!MyFile.is_open())
    {
        MyFile.clear();
        MyFile.open(alg_data.param.file_solution, ios_base::out);  // will create if necessary

    }
    else{
        MyFile << endl << endl;
    }

    MyFile << "Instance: " << alg_data.param.instance_name << endl;
    MyFile << "Algorithm: " << alg_data.param.algorithm_name << endl;
    MyFile << "Time_limit: "<< alg_data.time_limit << endl;
    MyFile << "Seed: "<< alg_data.param.s_seed << endl;
    MyFile << "Elapsed_time: " << alg_data.elapsed_time_sec << endl;
    if(alg_data.param.algorithm_name == "GA"){
        MyFile << "param1: " << alg_data.param.s_population_size << endl;
        MyFile << "param2: " << alg_data.param.s_prob_mutation << endl;
        MyFile << "param3: " << "nan" << endl;
    }
    else if(alg_data.param.algorithm_name == "MOVNS_Arroyo"){
        MyFile << "param1: " << alg_data.param.s_destruction_factor << endl;
        MyFile << "param2: " << "nan" << endl;
        MyFile << "param3: " << "nan" << endl;
    }
    else if(alg_data.param.algorithm_name == "MOVNS_D"){
        MyFile << "param1: " << alg_data.param.s_decomposition_size << endl;
        MyFile << "param2: " << alg_data.param.s_decomposition_neighboor_size << endl;
        MyFile << "param3: " << "nan" << endl;
    }
    else{
        MyFile << "param1: " << "nan" << endl;
        MyFile << "param2: " << "nan" << endl;
        MyFile << "param3: " << "nan" << endl;
    }


    MyFile << endl;

    MyFile << "Makespan" << "\t" << "TEC";

    for (auto it=alg_data.non_dominated_set.begin(); it != alg_data.non_dominated_set.end();++it) {
        MyFile << endl << it->first << "\t" << it->second;
    }

    //Imprimir demarcação de final de arquivo
    if(alg_data.param.algorithm_name == "EXACT"){

        if(alg_data.is_optimal){
            MyFile << "\t" << "*";
        }
        else{
            MyFile << "\t" << "-";
        }
    }

    MyFile << "\t" << "END";

    MyFile.close();
}

/*
 * Função para selecionar apenas solução válidas que estão no conjunto não-dominado
 */
void SelectOnlyValidSolutions(vector<Solution*> non_dominated_set){
    for(auto it_sol = non_dominated_set.begin(); it_sol != non_dominated_set.end();){
        //Se o makespan é maior que o horizonte de planejamento
        if((*it_sol)->makeSpan > (Instance::num_planning_horizon)*Instance::num_days){
            //Remover essa solução do conjunto
            it_sol = non_dominated_set.erase(it_sol);
        }
        else{
            ++it_sol;
        }
    }
}

void CalculateMetric(string folder_solution, string folder_instance)
{


    vector<string> files;
    map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets;
    //map<string, map<string, double>> hypervolume;
    map<string,map<string, map<string, double>>> hypervolume1;
    map<string, pair<unsigned, double>> reference_points;

    //Encontrar todos os arquivos que estão na pasta de soluções
    FindFilesInFolder(folder_solution, files);

    //Ler o conteúdo de cada arquivo de solução e guardar em sets
    ReadFiles(files, sets);


    //Identificar os pontos de referência
    /*string file_instance;
    pair<unsigned, double> reference_point;
    for(auto &it_instance:sets){

        file_instance = folder_instance + it_instance.first + ".dat";

        Instance::ReadMarceloInstance(file_instance);
        reference_point.first = Instance::max_makespan;
        reference_point.second = Instance::max_energy_cost;
        reference_points.insert({it_instance.first, reference_point});
    }*/

    //Gerar o conjunto de referência
    //Calcular seu hipervolume do conjunto de referência
    //Salvá-lo em um arquivo
    GenerateReferenceSet(folder_solution, sets, hypervolume1, reference_points);

    //Calcular o hipervolume para todas as instâncias em sets
    CalculateHypervolume(sets, hypervolume1, reference_points);

    //Calcular a diferença de hipervolume para todas as instâncias em sets
    /*for(auto instance : hypervolume){
        for(auto seed : instance.second){
            hypervolume[instance.first].insert({seed.first+"_diff", hypervolume[instance.first]["ref"]-seed.second});
        }
    }*/

    cout << setprecision(10);

    //Imprimir os resultados
    unsigned header = false;
    for(auto &instance : hypervolume1){

        if(!header){
            cout << "Instance ";

            //cout << "Algoritmo ";

            //cout << "Média HV ";

            /*for(auto &seed : instance.second){
                cout << seed.first << " ";
            }

            cout << "Max_makespan ";
            cout << "Max_PEP ";*/

            for(auto &it_algorithm : instance.second){
                cout << " " << it_algorithm.first << " ";
            }

            cout << endl;

            header = true;
        }

        cout << instance.first << " " ;


        for(auto &it_algorithm : instance.second){
        long HV = 0;
        for(auto &seed : it_algorithm.second){

            HV += seed.second;
            //cout << seed.second << " ";
        }
        cout << HV/it_algorithm.second.size() << " ";
        }

        //cout << seed.first << ": " << seed.second << "\t";

        /*cout << reference_points[instance.first].first << " ";
        cout << reference_points[instance.first].second << " ";*/


        cout << endl;
    }

}


