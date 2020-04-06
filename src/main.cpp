#include <iostream>
#include <common/experiments.h>

using namespace std;

int main(int argc, char** argv)
{
    Parameters Param;

    if(argc > 1){


        Param.instanceFile = argv[1];
        Param.seed = argv[2];
        Param.algorithm = argv[3];
        Param.fileSolution = argv[4];

    }
    else{
        //Param.instanceFile = "../../Instances/Rabadi-2006/Large/Setup/8Machines-SetupDomin/100on8Rp50Rs50_5.dat";
        //Param.instanceFile = "../../Instances/Rabadi-2006/Large/Processing/2Machines-ProcDomin(Done)/100on2Rp50Rs50_1.dat";
        Param.instanceFile = "../../Instances/I_6_2_S_1-9_1_rev1.dat";
        Param.seed = "1296910683";
        Param.algorithm = "GA";
        Param.fileSolution = "../Solution/Test/" + Param.algorithm + "/teste.txt";
        Param.timeLimit = 300;

    }

    RunAlgorithm(Param);
}
