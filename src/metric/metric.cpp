#include "metric.h"

void CalculateHypervolume(map<string, map<string, vector<pair<unsigned, double>>>> sets,
                          map<string, map<string, double>> &hypervolume,
                          map<string, pair<unsigned, double>> reference_points)
{

    solution_data sd;
    double hv;

    //cout << setprecision(10);

    //pair<unsigned, double> reference;


    //string ref = "ref";

    //Para cada instância
    for(auto instance: sets){

        for(auto seed : instance.second){

            //Calcular o hipervolume para cada solução e armazenar
            //SortByMakespan(sd.non_dominated_set);
            hv = CalculateHypervolumeMin(seed.second, reference_points[instance.first]);
            hypervolume[instance.first].insert({seed.first, hv});

        }

    }

}

void ReadFiles(vector<string> files, map<string, map<string, vector<pair<unsigned, double>>>> &sets){

    solution_data sd;

    //Ordenar o vetor de arquivos
    sort(files.begin(), files.end());

    //Ler cada arquivo de solução
    for(auto it : files){
        sd.file_solution = it;
        sd.non_dominated_set.clear();
        ReadFile(sd);

        //Montar a estrutura com todas as instâncias e seus conjunto de soluções
        sets[sd.instance_name].insert({to_string(sd.seed), sd.non_dominated_set});

    }
}

void GenerateReferenceSet(string folder_solution,
                          map<string, map<string, vector<pair<unsigned, double>>>> &sets,
                          map<string, map<string, double>> &hypervolume,
                          map<string, pair<unsigned, double>> &reference_points){

    double hv;
    vector<pair<unsigned, double>> non_dominated_set;

    pair<unsigned, double> reference_point;


    for(auto instance: sets){

        //cout << instance.first << "\t";

        //Iniciar o ponto de referência com zero, mas ele precisa ser atualizado
        reference_point.first = reference_point.second = 0;
        non_dominated_set.clear();

        for(auto seed : instance.second){
            for(auto point : seed.second){
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

        SortByMakespan(non_dominated_set);
        SalveReferenceSolution(non_dominated_set, folder_solution, instance.first, "GA", reference_point);

        //Inserir o conjunto de referência em sets
        sets[instance.first].insert({"ref", non_dominated_set});

        //Inserir o ponto de referência em reference_points
        reference_points.insert({instance.first, reference_point});

        hv = CalculateHypervolumeMin(non_dominated_set, reference_point);
        //cout << "Hipervolume reference: " << hv << endl;

        hypervolume[instance.first].insert({"ref", hv});

    }

}

void SalveReferenceSolution(vector<pair<unsigned, double>> non_dominated_set,
                            string folder_solution, string instance_name, string algorithm_name,
                            pair<unsigned, double> reference_point){

    ofstream MyFile;

    string file_name;

    file_name = folder_solution + algorithm_name + "_" + instance_name + ".ref";

    //Abrir o arquivo
    MyFile.open(file_name);

    MyFile << "Instance: " << instance_name << endl;
    MyFile << "Algorithm: " << algorithm_name << endl;
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
