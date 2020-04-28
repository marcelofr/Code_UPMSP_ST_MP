#include "experiments.h"

void RunAlgorithm(Parameters Param){
    unsigned seed, max_time, max_time_factor;

    //Make trace log in file (.trace.log)
    MakeTrace();

    //Set seed
    stringstream ss(Param.seed);
    ss >> seed;
    srand(seed);
    //srand (time(NULL));

    //Read instance
    Instance::ReadJulioInstance(Param.instance_file);

    //Critério de parada baseado no artigo do Luciano, Swarm 2019
    stringstream ss1(Param.max_time_factor);
    ss1 >> max_time_factor;
    max_time = max_time_factor * Instance::numJobs;

    vector<Solution> non_dominated_set;
    instance_result ir;

    if(Param.algorithm == "GA"){
        //Instance::PrintInstance1();
        nsga_ii(max_time, non_dominated_set);
    }
    else if(Param.algorithm == "EXACT"){
        RunMathModel(Param.instance_file, max_time, 0);
    }

    SalveSolution(non_dominated_set, Param, ir);

}

/*
 * Método para salvar em arquivo a solução encontrada por um algoritmo
 */
void SalveSolution(vector<Solution> non_dominated_set, Parameters Param, instance_result ir){

    ofstream MyFile(Param.file_solution);
    unsigned max_time_factor;

    //Salvar o conjunto não-dominado, em um arquivo
    ir.algorithm_name = Param.algorithm;
    stringstream ss1(Param.max_time_factor);
    ss1 >> max_time_factor;
    ir.time = max_time_factor*Instance::numJobs;
    ir.instance_name = Param.instance_name;
    stringstream ss(Param.seed);
    ss >> ir.seed;
    pair<unsigned, double> p;
    for (auto it = non_dominated_set.begin(); it != non_dominated_set.end(); ++it) {
        p.first = it->makeSpan;
        p.second = it->TEC;
        ir.non_dominated_set.push_back(p);
    }

    MyFile << ir.algorithm_name << endl;
    MyFile << ir.time << endl;
    MyFile << ir.instance_name << endl;
    MyFile << ir.seed << endl;
    MyFile << endl;
    MyFile << "makespan" << "\t" << "tec" << endl;

    for (auto it=ir.non_dominated_set.begin(); it != ir.non_dominated_set.end();++it) {
        MyFile << it->first << "\t" << it->second << endl;
    }
}
