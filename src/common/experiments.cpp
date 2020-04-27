#include "experiments.h"

void RunAlgorithm(Parameters Param){
    unsigned seed, max_time;

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
    max_time = Param.max_time_factor * Instance::numJobs;

    vector<Solution> non_dominated_set;
    instance_result ir;

    if(Param.algorithm == "GA"){
        //Instance::PrintInstance1();
        nsga_ii(max_time, non_dominated_set);
    }

    //Salvar o conjunto não-dominado, em um arquivo
    ir.algorithm_name = Param.algorithm;
    ir.time = max_time;
    ir.instance_name = Param.instance_name;
    ir.seed = seed;
    pair<unsigned, double> p;
    for (auto it=non_dominated_set.begin(); it != non_dominated_set.end(); ++it) {
        p.first = it->makeSpan;
        p.second = it->TEC;
        ir.non_dominated_set.push_back(p);
    }

    SalveSolution(ir, Param.file_solution);

}
