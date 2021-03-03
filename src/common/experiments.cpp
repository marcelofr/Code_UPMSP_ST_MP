#include "experiments.h"

void RunAlgorithm(algorithm_data alg_data){

    Solution * my_solution;
    Timer *t1 = new Timer();

    //Make trace log in file (.trace.log)
    MakeTrace();

    //Ler a instância
    //Instance::ReadJulioInstance(Param.instance_file);
    Instance::ReadMarceloInstance(alg_data.param.instance_file);

    /*Fazer a discretização do tempo*/
    Instance::discretization_factor = 1;
    Instance::num_planning_horizon = ((Instance::num_planning_horizon+1)/Instance::discretization_factor)-1;
    for(unsigned i=0; i<Instance::num_days; i++){
        Instance::v_peak_start[i] = Instance::v_peak_start[i]/Instance::discretization_factor;
        Instance::v_peak_end[i] = ((Instance::v_peak_end[i]+1)/Instance::discretization_factor)-1;
    }
    for(unsigned i=1; i<=Instance::num_machine; i++){
        for(unsigned j=1; j<=Instance::num_jobs; j++){
            Instance::m_processing_time[i][j] = ceil(double(Instance::m_processing_time[i][j])/double(Instance::discretization_factor));
            for(unsigned k=1; k<=Instance::num_jobs; k++){
                Instance::m_setup_time[i][j][k] = ceil(double(Instance::m_setup_time[i][j][k])/double(Instance::discretization_factor));
            }
        }
    }
    Instance::max_cost = Instance::max_cost/Instance::discretization_factor;

    vector<Solution*> non_dominated_set;

    alg_data.is_optimal = false;

    t1->start();

    //Definir o tempo limite de execução
    alg_data.time_limit = alg_data.param.u_max_time_factor * Instance::num_jobs * log(Instance::num_machine);

    #ifdef DEBUG
        cout << "Tempo limite: " << alg_data.time_limit << endl;
    #endif

    if(alg_data.param.algorithm_name == "GA"){

        //vector<GASolution*> non_dominated_set_ga;
        NDSetSolution<GASolution *> *non_dominated_set_ga = new NDSetSolution<GASolution *>();

        //GenerateInitialPopulation(non_dominated_set_ga, alg_data.param.u_population_size);
        //GenerateInitialPopulation(non_dominated_set_ga->set_solution, alg_data.param.u_population_size);
        non_dominated_set_ga->GenerateGreedyRandomInitialSolution(alg_data.param.u_population_size);
        //non_dominated_set_ga->GenerateGreedyInitialSolution();

        #ifdef DEBUG
            cout << "===========Inicio População Inicial===========" << endl;
            PrintPopulation(non_dominated_set_ga->set_solution);
            //non_dominated_set_ga->PrintSetSolution();
            cout << "===========Fim População Inicial===========" << endl << endl;
        #endif

        nsga_ii(alg_data, non_dominated_set_ga->set_solution, t1);

        #ifdef DEBUG
            cout << "===========Inicio NSGA===========" << endl;
            PrintPopulation(non_dominated_set_ga->set_solution);
            //non_dominated_set_ga->PrintSetSolution();
            t1->printElapsedTimeInMilliSec();
            cout << "===========Fim NSGA===========" << endl;
        #endif

    }
    else if(alg_data.param.algorithm_name == "EXACT"){

        //Gerar uma solução inicial gulosa considerando o objetivo do makespan
        my_solution = new Solution();
        my_solution->GreedyInitialSolutionMakespan();

        //Modelo ponderado
        RunWeightedMathModel(alg_data.time_limit, alg_data.param.d_alpha, my_solution);
        //Modelo espsilon-restrito
        //RunEpsilonMathModel(max_time, 0, my_solution->TEC, my_solution);
        //RunEpsilonMathModel(max_time, my_solution->makeSpan, 0, my_solution);

        non_dominated_set.push_back(my_solution);

        if(my_solution->is_optimal){
            alg_data.is_optimal = true;
        }

        alg_data.param.file_solution = alg_data.param.folder_solution + alg_data.param.algorithm_name + "_" + alg_data.param.instance_name + "_" + alg_data.param.s_alpha + ".sol";

    }
    else if(alg_data.param.algorithm_name == "LS"){

        NDSetSolution<LSSolution *> *non_dominated_set_ls = new NDSetSolution<LSSolution *>();

        non_dominated_set_ls->GenerateGreedyInitialSolution();

        #ifdef DEBUG
            cout << "===========Inicio Solução Inicial===========" << endl;
            non_dominated_set_ls->PrintSetSolution();
            cout << "===========Fim Solução Inicial===========" << endl << endl;
        #endif

        HillClimbing(non_dominated_set_ls, t1);

        #ifdef DEBUG
            cout << "===========Inicio HillClimbing===========" << endl;
            non_dominated_set_ls->PrintSetSolution();
            t1->printElapsedTimeInMilliSec();
            cout << "===========Fim HillClimbing===========" << endl << endl;
        #endif
    }

    t1->stop();

    //Eliminar soluções que estão fora do horizonte de planejamento
    SelectOnlyValidSolutions(non_dominated_set);

    //Ordenar os pontos
    SortByMakespan(non_dominated_set);

    alg_data.elapsed_time_sec = t1->getElapsedTimeInMilliSec();
    alg_data.non_dominated_set.clear();
    pair<unsigned, double> aux;
    for(auto it:non_dominated_set){
        aux.first = it->makeSpan*Instance::discretization_factor;
        aux.second = it->TEC*Instance::discretization_factor;
        alg_data.non_dominated_set.push_back(aux);
    }

    #ifndef IRACE
        //Salvar o conjunto não-dominado em arquivo
        SalveFileSolution(alg_data);
    #endif

    #ifdef IRACE
        pair<unsigned, double> reference_point;
        double hv;
        reference_point.first = alg_data.non_dominated_set.back().first;
        reference_point.second = alg_data.non_dominated_set.front().second;
        hv = CalculateHypervolumeMin(alg_data.non_dominated_set, reference_point);
        //cout << hv << " " << ir.elapsed_time_sec << endl;
        cout << hv << endl;
    #endif

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
    MyFile << "Alpha: " << alg_data.param.s_alpha << endl;
    MyFile << "Population_size: " << alg_data.param.s_population_size << endl;
    MyFile << "Probability_of_mutation: " << alg_data.param.s_prob_mutation << endl;

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

void CalculateMetric(string folder_solution)
{

    vector<string> files;
    map<string, map<string, vector<pair<unsigned, double>>>> sets;
    map<string, map<string, double>> hypervolume;

    //Encontrar todos os arquivos que estão na pasta de soluções
    FindFilesInFolder(folder_solution, files);

    //Ler o conteúdo de cada arquivo de solução e guardar em sets
    ReadFiles(files, sets);

    map<string, pair<unsigned, double>> reference_points;

    //Gerar o conjunto de referência
    //Calcular seu hipervolume
    //Salvá-lo em um arquivo
    //Identificar os pontos de referência
    GenerateReferenceSet(folder_solution, sets, hypervolume, reference_points);

    //Calcular o hipervolume para todas as instâncias em sets
    CalculateHypervolume(sets, hypervolume, reference_points);

    //Calcular a diferença de hipervolume para todas as instâncias em sets
    /*for(auto instance : hypervolume){
        for(auto seed : instance.second){
            hypervolume[instance.first].insert({seed.first+"_diff", hypervolume[instance.first]["ref"]-seed.second});
        }
    }*/

    cout << setprecision(10);

    //Imprimir os resultados
    unsigned header = false;
    for(auto instance : hypervolume){

        if(!header){
            cout << "Instance ";

            for(auto seed : instance.second){
                cout << seed.first << " ";
            }

            cout << "Max_makespan ";
            cout << "Max_PEP ";

            cout << endl;

            header = true;
        }

        cout << instance.first << " " ;

        for(auto seed : instance.second){
            cout << seed.second << " ";
        }
        //cout << seed.first << ": " << seed.second << "\t";

        cout << reference_points[instance.first].first << " ";
        cout << reference_points[instance.first].second << " ";


        cout << endl;
    }

}


