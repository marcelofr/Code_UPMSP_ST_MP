#include "metric.h"

void CalculateMetric(string folder_solution)
{
    vector<string> files;
    ReadFilesInFolder(folder_solution, files);
    double hv;

    instance_result ir;
    pair<unsigned, double> r;
    r.first = r.second = 0;
    //Ler cada arquivo de solução
    for(auto it : files){
        ReadFile(it, ir);
        hv = CalculateHypervolume(ir.non_dominated_set, r);
        cout << hv << endl;
    }

}

void ReadFilesInFolder(string folder_solution, vector<string> &files)
{

    //Criar um vetor com o nome de todos os arquivos do diretório
    for (auto entry : filesystem::directory_iterator(folder_solution)){
        files.push_back(entry.path());
    }

}

void ReadFile(string file_name, instance_result &ir)
{
    fstream file;
    string str;
    unsigned num;
    pair<unsigned, double> p;

    file.open(file_name);

    //Nome do algoritmo
    file >> str;
    ir.algorithm_name = str;

    //Tempo
    file >> num;
    ir.time = num;

    //Instância
    file >> str;
    ir.instance_name = str;

    //Semente
    file >> num;
    ir.seed = num;

    //Pular o nome dos objetivos
    file >> str;
    file >> str;

    ir.non_dominated_set.clear();
    //Ler o conjunto não-dominado
    while (file >> p.first >> p.second)
    {
        ir.non_dominated_set.push_back(p);
    }

    file.close();
}

void SalveSolution(instance_result ir, string file_solution){

    ofstream MyFile(file_solution);

    MyFile << ir.algorithm_name << endl;
    MyFile << ir.time << endl;
    MyFile << ir.instance_name << endl;
    MyFile << ir.seed << endl;
    MyFile << endl;
    MyFile << "makespan" << "\t" << "tec" << endl;

    for (auto it=ir.non_dominated_set.begin(); it != ir.non_dominated_set.end();++it) {
        MyFile << it->first << "\t" << it->second << endl;
    }
}
