#include "metric.h"

void CalculateHypervolume(map<string,map<string, map<string, vector<pair<unsigned, double>>>>> sets,
                          map<string,map<string, map<string, double>>> &hypervolume,
                          map<string, pair<unsigned, double>> reference_points)
{

    algorithm_data alg_data;
    double hv;

    //Para cada instância
    for(auto &it_instance: sets){

        //Para cada semente
        for(auto &it_seed : it_instance.second){

            for(auto &it_algorithm : it_seed.second){
                //Calcular o hipervolume para cada solução e armazenar
                //SortByMakespan(sd.non_dominated_set);
                hv = CalculateHypervolumeMin(it_algorithm.second, reference_points[it_instance.first]);
                hypervolume[it_instance.first][it_seed.first].insert({it_algorithm.first, hv});
            }

        }

    }

}

void ReadFiles(vector<string> files,
               map<string,map<string, map<string, vector<pair<unsigned, double>>>>> &sets){

    algorithm_data alg_data;

    //Ordenar o vetor de arquivos
    sort(files.begin(), files.end());

    //Ler cada arquivo de solução
    for(auto &it : files){
        alg_data.param.file_solution = it;
        alg_data.non_dominated_set.clear();
        ReadFile(alg_data);

        //Montar a estrutura com todas as instâncias e seus conjunto de soluções
        //sets[alg_data.param.instance_name].insert({alg_data.param.s_seed, alg_data.non_dominated_set});
        sets[alg_data.param.instance_name][alg_data.param.algorithm_name][alg_data.param.s_seed] = alg_data.non_dominated_set;

        //sets[alg_data.param.instance_name][alg_data.param.s_seed].insert(sets[alg_data.param.instance_name][alg_data.param.s_seed].begin(), alg_data.non_dominated_set.begin(), alg_data.non_dominated_set.end());

    }
}

void GenerateReferenceSet(string folder_solution,
                          map<string,map<string, map<string, vector<pair<unsigned, double>>>>> &sets,
                          map<string,map<string, map<string, double>>> &hypervolume,
                          map<string, pair<unsigned, double>> &reference_points){

    double hv;
    vector<pair<unsigned, double>> non_dominated_set;

    pair<unsigned, double> reference_point;

    for(auto &it_instance: sets){

        //cout << instance.first << "\t";

        //Iniciar o ponto de referência com zero, mas ele precisa ser atualizado
        reference_point.first = reference_point.second = 0;
        non_dominated_set.clear();

        for(auto &it_algorithm : it_instance.second){
        for(auto &it_seed: it_algorithm.second){
            for(auto &point : it_seed.second){
                //cout << point.first << "\t" << point.second << endl;
                AddPoint(point, non_dominated_set);

                if(point.first > reference_point.first){
                    reference_point.first = point.first;
                }
                if(point.second > reference_point.second){
                    reference_point.second = point.second;
                }
            }
        }
        }

        reference_points[it_instance.first] = reference_point;

        SortByMakespan(non_dominated_set);
        SalveReferenceSolution(non_dominated_set, folder_solution, it_instance.first, reference_point);

        //Inserir o conjunto de referência em sets
        sets[it_instance.first]["ref_set"].insert({"all", non_dominated_set});

        //Inserir o ponto de referência em reference_points
        //reference_points.insert({instance.first, reference_point});

        hv = CalculateHypervolumeMin(non_dominated_set, reference_point);
        //cout << "Hipervolume reference: " << hv << endl;

        hypervolume[it_instance.first]["ref_set"].insert({"all", hv});

    }

}

void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set,
                            string folder_solution, string instance_name,
                            pair<unsigned, double> reference_point){

    ofstream MyFile;

    string file_name;

    file_name = folder_solution + "Ref_" + instance_name + ".ref";

    //Abrir o arquivo
    MyFile.open(file_name);

    MyFile << "Instance: " << instance_name << endl;
    //MyFile << "Algorithm: " << algorithm_name << endl;
    MyFile << "Max_makespan: " << reference_point.first << endl;
    MyFile << "Max_PEP: " << reference_point.second << endl;

    MyFile << endl;

    MyFile << "Makespan" << "\t" << "TEC";

    for (auto it=non_dominated_set.begin(); it != non_dominated_set.end();++it) {
        MyFile << endl << it->first << "\t" << it->second;
    }
    MyFile << "\t" << "END";

    MyFile.close();
}
