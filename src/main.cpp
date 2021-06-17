#include <iostream>
#include <common/experiments.h>
#include "common/generate_intances.h"

using namespace std;

int main(int argc, char** argv)
{

    algorithm_data alg_data;

    if(argc > 1){

        alg_data.param.instance_folder = argv[1];
        alg_data.param.instance_name = argv[2];
        alg_data.param.instance_extension = argv[3];
        alg_data.param.instance_file = alg_data.param.instance_folder + alg_data.param.instance_name + alg_data.param.instance_extension;

#ifdef IRACE

        alg_data.param.instance_file = argv[1];
        unsigned long pos = alg_data.param.instance_file.find("/Instances/");
        alg_data.param.instance_folder = alg_data.param.instance_file.substr (0, pos+11);
        alg_data.param.instance_name = alg_data.param.instance_file.substr (pos+11);
        alg_data.param.instance_name = alg_data.param.instance_name.substr(0, alg_data.param.instance_name.size()-4);
        alg_data.param.instance_extension = ".dat";

#endif

        alg_data.param.s_seed = argv[4];
        alg_data.param.algorithm_name = argv[5];
        alg_data.param.folder_solution = argv[6];
        alg_data.param.s_alpha = argv[7];
        //Param.file_solution = Param.folder_solution + Param.algorithm + "_" + Param.instance_name + ".sol";
        alg_data.param.file_solution = alg_data.param.folder_solution + alg_data.param.algorithm_name + "_" + alg_data.param.instance_name + "_" + alg_data.param.s_seed + ".sol";
        alg_data.param.s_population_size = argv[8];
        alg_data.param.s_prob_mutation = argv[9];
        alg_data.param.s_max_time_factor = argv[10];

    }
    else{

        //alg_data.param.instance_folder = "../../Instances/SMALL/"; alg_data.param.instance_name = "6_2_1439_3_S_1-9";
        alg_data.param.instance_folder = "../../Instances/LARGE/"; alg_data.param.instance_name = "50_10_1439_5_S_1-9";
        //alg_data.param.instance_folder = "../../Instances/LARGE/"; alg_data.param.instance_name = "250_10_1439_5_S_1-124";

        //Param.instance_name = "7_2_1439_3_S_1-9";
        alg_data.param.instance_extension = ".dat";
        alg_data.param.instance_file = alg_data.param.instance_folder + alg_data.param.instance_name + alg_data.param.instance_extension;

        alg_data.param.s_seed = "60543";
        //alg_data.param.algorithm_name = "GENERATE_INSTANCES_SMALL";
        //alg_data.param.algorithm_name = "RUN_METRIC";
        //alg_data.param.algorithm_name = "GA"; //nsga-ii
        //alg_data.param.algorithm_name = "EXACT";
        //alg_data.param.algorithm_name = "LS"; //hill climb
        //alg_data.param.algorithm_name = "MOVNS";
        //alg_data.param.algorithm_name = "MOVNS_Arroyo";
        //alg_data.param.algorithm_name = "MOVNS_Eduardo";
        alg_data.param.algorithm_name = "MONO";
        alg_data.param.s_max_time_factor = "1000";
        //Param.folder_solution = "../../Solutions/2020_07_24_17_16/";
        alg_data.param.folder_solution = "../../Solutions/Test/";
        alg_data.param.file_solution = alg_data.param.folder_solution + alg_data.param.algorithm_name + "_"
                + alg_data.param.instance_name + "_" + alg_data.param.s_seed +  ".sol";
        alg_data.param.s_alpha = "0.9999";
        alg_data.param.s_population_size = "110";
        alg_data.param.s_prob_mutation = "5";

    }

    //Fazer as conversões de dados
    alg_data.param.u_population_size = unsigned(atoi(alg_data.param.s_population_size.c_str()));
    alg_data.param.u_seed = unsigned(atoi(alg_data.param.s_seed.c_str()));
    alg_data.param.u_max_time_factor = unsigned(atoi(alg_data.param.s_max_time_factor.c_str()));
    alg_data.param.d_alpha = atof(alg_data.param.s_alpha.c_str());
    alg_data.param.u_prob_mutation = unsigned(atoi(alg_data.param.s_prob_mutation.c_str()));

    if(alg_data.param.algorithm_name == "RUN_METRIC"){
        CalculateMetric(alg_data.param.folder_solution);
    }
    else if(alg_data.param.algorithm_name == "GENERATE_INSTANCES_SMALL"){
        GenerateSmallInstances();

    }
    else if(alg_data.param.algorithm_name == "GENERATE_INSTANCES_LARGE"){
        GenerateLargeInstances();
    }
    else{
        RunAlgorithm(alg_data);
    }
}
