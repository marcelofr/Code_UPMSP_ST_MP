#include "experiments.h"

void RunAlgorithm(Parameters Param){
    unsigned seed, max_time, max_time_factor;
    double alpha;
    Solution * my_solution;
    Timer *t1 = new Timer();

    //Make trace log in file (.trace.log)
    MakeTrace();

    //Set seed
    stringstream ss(Param.seed);
    ss >> seed;
    srand(seed);
    //srand (time(NULL));

    //Read instance
    //Instance::ReadJulioInstance(Param.instance_file);
    Instance::ReadMarceloInstance(Param.instance_file);

    //Critério de parada baseado no artigo do Luciano, Swarm 2019
    stringstream ss1(Param.max_time_factor);
    ss1 >> max_time_factor;
    max_time = max_time_factor * Instance::num_jobs;

    vector<Solution> non_dominated_set;
    instance_result ir;

    if(Param.algorithm == "GA"){
        //Instance::PrintInstance1();
        nsga_ii(max_time, non_dominated_set);
    }
    else if(Param.algorithm == "EXACT"){

        t1->start();

        //Set seed
        stringstream ss(Param.alpha);
        ss >> alpha;

        /*Gerar uma população inicial*/
        /*Gerar uma solução gulosa considerando o objetivo do makespan*/
        my_solution = new Solution();
        my_solution->GreedyInitialSolutionMakespan();

        RunMathModel(max_time, alpha, my_solution);

        non_dominated_set.push_back(*my_solution);

        t1->stop();

    }

    SalveSolution(non_dominated_set, Param, ir);

    if(Param.algorithm == "EXACT"){
        ofstream MyFile;
        double time_s;
        MyFile.open(Param.file_solution, ios_base::out | ios_base::in | ios_base::ate);  // will not create file
        MyFile << "\t" << alpha;
        if(my_solution->is_optimal){
            MyFile << "\t" << "*";
        }
        time_s = t1->getElapsedTimeInMilliSec()/1000;
        MyFile << "\t" << time_s;
        MyFile.close();
    }
    else{
        ofstream MyFile;
        MyFile.open(Param.file_solution, ios_base::out | ios_base::in | ios_base::ate);  // will not create file
        MyFile << "\t" << "END";

    }

}

/*
 * Método para salvar em arquivo a solução encontrada por um algoritmo
 */
void SalveSolution(vector<Solution> non_dominated_set, Parameters Param, instance_result ir){

    unsigned max_time_factor;

    ofstream MyFile;

    //Salvar o conjunto não-dominado, em um arquivo
    ir.algorithm_name = Param.algorithm;
    stringstream ss1(Param.max_time_factor);
    ss1 >> max_time_factor;
    ir.time = max_time_factor*Instance::num_jobs;
    ir.instance_name = Param.instance_name;
    stringstream ss(Param.seed);
    ss >> ir.seed;
    pair<unsigned, double> p;
    for (auto it = non_dominated_set.begin(); it != non_dominated_set.end(); ++it) {
        p.first = it->makeSpan;
        p.second = it->TEC;
        ir.non_dominated_set.push_back(p);
    }

    MyFile.open(Param.file_solution, ios_base::out | ios_base::in | ios_base::ate);  // will not create file
    if (!MyFile.is_open())
    {
        MyFile.clear();
        MyFile.open(Param.file_solution, ios_base::out);  // will create if necessary

        MyFile << ir.algorithm_name << endl;
        MyFile << ir.time << endl;
        MyFile << ir.instance_name << endl;
        MyFile << ir.seed << endl;
        MyFile << endl;
        MyFile << "makespan" << "\t" << "tec";
    }

    for (auto it=ir.non_dominated_set.begin(); it != ir.non_dominated_set.end();++it) {
        MyFile << endl << it->first << "\t" << it->second;
    }

    MyFile.close();
}
