#include <iostream>
#include <common/experiments.h>
#include "common/generate_intances.h"

using namespace std;

int main(int argc, char** argv)
{

    Parameters Param;

    if(argc > 1){

        Param.instance_folder = argv[1];
        Param.instance_name = argv[2];
        Param.instance_extension = argv[3];
        Param.instance_file = Param.instance_folder + Param.instance_name + Param.instance_extension;
        Param.seed = argv[4];
        Param.algorithm = argv[5];
        Param.max_time_factor = argv[6];
        Param.folder_solution = argv[7];
        Param.file_solution = Param.folder_solution + Param.algorithm + "_" + Param.instance_name + ".sol";


    }
    else{

        //Param.instance_folder = "../../Instances/";
        //Param.instance_folder = "../../Instances/Debug/";
        Param.instance_folder = "../../Instances/New/";
        //Param.instance_name = "I_6_2_S_1-9_1_rev1";
        //Param.instance_name = "6_2_143_3_S_1-9";
        Param.instance_name = "50_10_143_5_S_1-9";
        Param.instance_extension = ".dat";
        Param.instance_file = Param.instance_folder + Param.instance_name + Param.instance_extension;
        Param.seed = "1296910683";
        //Param.algorithm = "GENERATE_INSTANCES";
        Param.algorithm = "GA";
        //Param.algorithm = "EXACT";
        Param.max_time_factor = "10";
        Param.folder_solution = "../../Solutions/27_04_2020/";
        Param.file_solution = Param.folder_solution + Param.algorithm + "_" + Param.instance_name + ".sol";

    }

    if(Param.algorithm == "RUN_METRIC"){
        CalculateMetric(Param.folder_solution);
    }
    else if(Param.algorithm == "GENERATE_INSTANCES"){
        GenerateSmallInstances();
        //GenerateLargeInstances();
    }
    else{
        RunAlgorithm(Param);
    }
}
