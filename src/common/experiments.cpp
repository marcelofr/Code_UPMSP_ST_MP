#include "experiments.h"

void RunAlgorithm(Parameters Param){
    unsigned seed;

    //Make trace log in file (.trace.log)
    MakeTrace();

    //Set seed
    stringstream ss(Param.seed);
    ss >> seed;
    srand(seed);
    //srand (time(NULL));

    //Read instance
    Instance::ReadJulioInstance(Param.instanceFile);

    if(Param.algorithm == "GA"){
        //Instance::PrintInstance1();
        nsga_ii(Param.timeLimit);
    }
}
