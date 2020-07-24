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

#ifdef IRACE

        Param.instance_file = argv[1];
        unsigned long pos = Param.instance_file.find("/Instances/");
        Param.instance_folder = Param.instance_file.substr (0, pos+11);
        Param.instance_name = Param.instance_file.substr (pos+11);
        Param.instance_name = Param.instance_name.substr(0, Param.instance_name.size()-4);
        Param.instance_extension = ".dat";

#endif

        Param.seed = argv[4];
        Param.algorithm = argv[5];
        Param.folder_solution = argv[6];
        Param.alpha = argv[7];
        Param.file_solution = Param.folder_solution + Param.algorithm + "_" + Param.instance_name + ".sol";
        Param.tam_population = argv[8];
        Param.prob_mutation = argv[9];
        Param.max_time_factor = argv[10];

    }
    else{

        Param.instance_folder = "../../Instances/SMALL/";
        //Param.instance_folder = "../../Instances/LARGE/";
        //Param.instance_name = "6_2_1439_3_S_1-9";
        Param.instance_name = "6_2_1439_3_S_1-9";
        Param.instance_extension = ".dat";
        Param.instance_file = Param.instance_folder + Param.instance_name + Param.instance_extension;

        Param.seed = "123456";
        //Param.algorithm = "GENERATE_INSTANCES_SMALL";
        //Param.algorithm = "RUN_METRIC";
        Param.algorithm = "GA";
        //Param.algorithm = "EXACT";
        Param.max_time_factor = "1000";
        Param.folder_solution = "../../Solutions/2020_07_24/";
        //Param.folder_solution = "../../Solutions/Test/";
        Param.file_solution = Param.folder_solution + Param.algorithm + "_"
                + Param.instance_name + "_" + Param.seed +  ".sol";
        Param.alpha = "0.1";
        Param.tam_population = "100";
        Param.prob_mutation = "5";

    }

    if(Param.algorithm == "RUN_METRIC"){
        CalculateMetric(Param.folder_solution);
    }
    else if(Param.algorithm == "GENERATE_INSTANCES_SMALL"){
        GenerateSmallInstances();

    }
    else if(Param.algorithm == "GENERATE_INSTANCES_LARGE"){
        GenerateLargeInstances();
    }
    else{
        RunAlgorithm(Param);
    }
}
