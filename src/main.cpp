#include <iostream>
#include <common/experiments.h>

using namespace std;

int main(int argc, char** argv)
{

    Parameters Param;

    if(argc > 1){


        Param.instance_file = argv[1];
        Param.seed = argv[2];
        Param.algorithm = argv[3];
        Param.file_solution = argv[4];

    }
    else{
        //Param.instanceFile = "../../Instances/Rabadi-2006/Large/Setup/8Machines-SetupDomin/100on8Rp50Rs50_5.dat";
        //Param.instanceFile = "../../Instances/Rabadi-2006/Large/Processing/2Machines-ProcDomin(Done)/100on2Rp50Rs50_1.dat";
        Param.instance_folder = "../../Instances/";
        Param.instance_name = "I_6_2_S_1-9_1_rev1";
        Param.instance_file = Param.instance_folder + Param.instance_name + ".dat";
        Param.seed = "1296910683";
        //Param.algorithm = "GA";
        Param.algorithm = "METRIC";
        Param.max_time_factor = 10;
        Param.folder_solution = "../../Solutions/27_04_2020/";
        Param.file_solution = Param.folder_solution + Param.algorithm +
                "_" + Param.instance_name + ".sol";

    }

    if(Param.algorithm == "METRIC"){
        CalculateMetric(Param.folder_solution);
    }
    else{
        RunAlgorithm(Param);
    }
}
