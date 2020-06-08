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
    instance_result ir;

    ir.is_optimal = false;

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

        max_time = (max_time_factor*6/10)* Instance::num_jobs * log(Instance::num_machine);

        //Set seed
        alpha = atof(Param.alpha.c_str());

        //Gerar uma solução inicial gulosa considerando o objetivo do makespan
        my_solution = new Solution();
        my_solution->GreedyInitialSolutionMakespan();

        RunMathModel(max_time, alpha, my_solution);

        non_dominated_set.push_back(my_solution);

        if(my_solution->is_optimal){
            ir.is_optimal = true;
        }

    }
    else if(Param.algorithm == "LS"){
        HillClimbing(non_dominated_set);
    }

    t1->stop();
    ir.elapsed_time_sec = t1->getElapsedTimeInMilliSec()/1000;

    SelectOnlyValidSolutions(non_dominated_set);

    SortByMakespan(non_dominated_set);

#ifndef IRACE

    //Salvar o conjunto não-dominado em arquivo
    SalveSolution(non_dominated_set, Param, ir);

#endif



#ifdef IRACE
    pair<unsigned, double> reference_point, aux;
    double hv;
    vector<pair<unsigned, double>>point_non_dominated_set;
    for(auto it:non_dominated_set){
        aux.first = it->makeSpan;
        aux.second = it->TEC;
        point_non_dominated_set.push_back(aux);
    }
    reference_point.first = 0;
    reference_point.second = 0;
    hv = CalculateHypervolume(point_non_dominated_set, reference_point);
    //cout << hv << " " << ir.elapsed_time_sec << endl;
    cout << hv << endl;
#endif

}

/*
 * Método para salvar em arquivo a solução encontrada por um algoritmo
 */
void SalveSolution(vector<Solution*> non_dominated_set, Parameters Param, instance_result ir){

    unsigned max_time_factor;

    ofstream MyFile;

    //Salvar o conjunto não-dominado, em um arquivo
    ir.algorithm_name = Param.algorithm;
    max_time_factor = unsigned(stoi(Param.max_time_factor));
    ir.time_limit = max_time_factor*Instance::num_jobs*log(Instance::num_machine);
    ir.instance_name = Param.instance_name;
    ir.seed = unsigned(stoi(Param.seed));
    pair<unsigned, double> p;
    for (auto it = non_dominated_set.begin(); it != non_dominated_set.end(); ++it) {
        p.first = (*it)->makeSpan;
        p.second = (*it)->TEC;
        ir.non_dominated_set.push_back(p);
    }

    //Tentar abrir um arquivo existente
    MyFile.open(Param.file_solution, ios_base::out | ios_base::in | ios_base::ate);  // will not create file

    //Se o arquivo não existe então criar um novo
    if (!MyFile.is_open())
    {
        MyFile.clear();
        MyFile.open(Param.file_solution, ios_base::out);  // will create if necessary

    }
    else{
        MyFile << endl << endl;
    }

    MyFile << "Instance: " << ir.instance_name << endl;
    MyFile << "Algorithm: " << ir.algorithm_name << endl;
    MyFile << "Time limit: "<< ir.time_limit << endl;
    MyFile << "Seed: "<< ir.seed << endl;
    MyFile << "Elapsed time: " << ir.elapsed_time_sec << endl;
    MyFile << "Alpha: " << Param.alpha << endl;
    MyFile << "Population size: " << Param.tam_population << endl;
    MyFile << "Probability of mutation: " << Param.prob_mutation << endl;

    MyFile << endl;

    MyFile << "Makespan" << "\t" << "TEC";

    for (auto it=ir.non_dominated_set.begin(); it != ir.non_dominated_set.end();++it) {
        MyFile << endl << it->first << "\t" << it->second;
    }
    MyFile << endl << "END";

    //Imprimir demarcação de final de arquivo
    if(Param.algorithm == "EXACT"){
        double alpha;
        stringstream ss(Param.alpha);
        ss >> alpha;
        MyFile << "\t" << alpha;
        if(ir.is_optimal){
            MyFile << "\t" << "*";
        }
        else{
            MyFile << "\t" << "-";
        }
    }

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
