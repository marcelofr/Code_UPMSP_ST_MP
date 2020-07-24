#include "experiments.h"

void RunAlgorithm(Parameters Param){
    unsigned seed, max_time_factor;
    double max_time, alpha;
    Solution * my_solution;
    Timer *t1 = new Timer();

    //Make trace log in file (.trace.log)
    MakeTrace();

    //Set seed
    seed = unsigned(atoi(Param.seed.c_str()));
    //srand (time(NULL));

    //Ler a instância
    //Instance::ReadJulioInstance(Param.instance_file);
    Instance::ReadMarceloInstance(Param.instance_file);

    Instance::seed = seed;

    //Critério de parada baseado no artigo do Luciano, Swarm 2019
    max_time_factor = unsigned(atoi(Param.max_time_factor.c_str()));


    vector<Solution*> non_dominated_set;
    solution_data sd;

    sd.is_optimal = false;

    t1->start();


    if(Param.algorithm == "GA"){

        ParametersGA ParamGA;

        ParamGA.time_limit = max_time_factor * Instance::num_jobs * log(Instance::num_machine);
        ParamGA.tam_population = unsigned(atoi(Param.tam_population.c_str()));
        ParamGA.prob_mutation = unsigned(atoi(Param.prob_mutation.c_str()));

        //Instance::PrintInstance1();
        nsga_ii(ParamGA, non_dominated_set);

    }
    else if(Param.algorithm == "EXACT"){

        /*Fazer a discretização do tempo*/
        Instance::num_planning_horizon = ((Instance::num_planning_horizon+1)/10)-1;
        for(unsigned i=0; i<Instance::num_days; i++){
            Instance::v_peak_start[i] = Instance::v_peak_start[i]/10;
            Instance::v_peak_end[i] = ((Instance::v_peak_end[i]+1)/10)-1;
        }

        max_time = (max_time_factor*6/10)* Instance::num_jobs * log(Instance::num_machine);

        //Set seed
        alpha = atof(Param.alpha.c_str());

        //Gerar uma solução inicial gulosa considerando o objetivo do makespan
        my_solution = new Solution();
        my_solution->GreedyInitialSolutionMakespan();

        //RunWeightedMathModel(max_time, alpha, my_solution);
        //RunEpsilonMathModel(max_time, 0, my_solution->TEC, my_solution);
        RunEpsilonMathModel(max_time, my_solution->makeSpan, 0, my_solution);

        non_dominated_set.push_back(my_solution);

        if(my_solution->is_optimal){
            sd.is_optimal = true;
        }

    }
    else if(Param.algorithm == "LS"){
        HillClimbing(non_dominated_set);
    }

    t1->stop();

    SelectOnlyValidSolutions(non_dominated_set);

    SortByMakespan(non_dominated_set);

    //Salvar o conjunto não-dominado em arquivo
    sd.instance_name  = Param.instance_name;
    sd.algorithm_name = Param.algorithm;
    sd.time_limit = max_time_factor * Instance::num_jobs * log(Instance::num_machine);
    sd.seed = seed;
    sd.elapsed_time_sec = t1->getElapsedTimeInMilliSec();
    sd.alpha = atof(Param.alpha.c_str());
    sd.population_size = unsigned(atoi(Param.tam_population.c_str()));
    sd.prob_mutation = unsigned(atoi(Param.prob_mutation.c_str()));
    sd.non_dominated_set.clear();
    //copy(non_dominated_set.begin(), non_dominated_set.end(), sd.non_dominated_set.begin());
    pair<unsigned, double> aux;
    for(auto it:non_dominated_set){
        aux.first = it->makeSpan;
        aux.second = it->TEC;
        sd.non_dominated_set.push_back(aux);
    }
    sd.file_solution = Param.file_solution;

#ifndef IRACE
    SalveFileSolution(sd);
#endif

#ifdef IRACE
    pair<unsigned, double> reference_point;
    double hv;
    reference_point.first = sd.non_dominated_set.back().first;
    reference_point.second = sd.non_dominated_set.front().second;
    hv = CalculateHypervolumeMin(sd.non_dominated_set, reference_point);
    //cout << hv << " " << ir.elapsed_time_sec << endl;
    cout << hv << endl;
#endif

}

/*
 * Método para salvar em arquivo a solução encontrada por um algoritmo
 */
void SalveFileSolution(solution_data sd){

    ofstream MyFile;

    //Tentar abrir um arquivo existente
    MyFile.open(sd.file_solution, ios_base::out | ios_base::in | ios_base::ate);  // will not create file

    //Se o arquivo não existe então criar um novo
    if (!MyFile.is_open())
    {
        MyFile.clear();
        MyFile.open(sd.file_solution, ios_base::out);  // will create if necessary

    }
    else{
        MyFile << endl << endl;
    }

    MyFile << "Instance: " << sd.instance_name << endl;
    MyFile << "Algorithm: " << sd.algorithm_name << endl;
    MyFile << "Time_limit: "<< sd.time_limit << endl;
    MyFile << "Seed: "<< sd.seed << endl;
    MyFile << "Elapsed_time: " << sd.elapsed_time_sec << endl;
    MyFile << "Alpha: " << sd.alpha << endl;
    MyFile << "Population_size: " << sd.population_size << endl;
    MyFile << "Probability_of_mutation: " << sd.prob_mutation << endl;

    MyFile << endl;

    MyFile << "Makespan" << "\t" << "TEC";

    for (auto it=sd.non_dominated_set.begin(); it != sd.non_dominated_set.end();++it) {
        MyFile << endl << it->first << "\t" << it->second;
    }

    //Imprimir demarcação de final de arquivo
    if(sd.algorithm_name == "EXACT"){

        if(sd.is_optimal){
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
    for(auto instance : hypervolume){
        for(auto seed : instance.second){
            hypervolume[instance.first].insert({seed.first+"_diif", hypervolume[instance.first]["ref"]-seed.second});
        }
    }

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


