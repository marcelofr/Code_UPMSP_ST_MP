#include "metric.h"

void CalculateMetric(string folder_solution)
{
    double hv;
    vector<string> files;
    vector<vector<pair<unsigned, double>>>non_dominated_sets;
    instance_result ir;
    pair<unsigned, double> reference_point;

    ReadFilesInFolder(folder_solution, files);
    sort(files.begin(), files.end());

    reference_point.first = reference_point.second = 0;
    //Ler cada arquivo de solução
    for(auto it : files){
        non_dominated_sets.clear();
        ReadFile(it, ir, non_dominated_sets);
        cout << "Instância: " << ir.instance_name << endl;
        for(unsigned i = 0; i<non_dominated_sets.size(); i++){
            hv = CalculateHypervolume(non_dominated_sets[i], reference_point);
            cout << "Hipervolume: " << hv << endl;
        }
    }
}

/*
 * Método para identificar os arquivos presentes em uma pasta e
 * salvar seus nomes em um vetor
 */
void ReadFilesInFolder(string folder_solution, vector<string> &files)
{

    //Criar um vetor com o nome de todos os arquivos do diretório
    for (auto entry : filesystem::directory_iterator(folder_solution)){
        files.push_back(entry.path());
    }
}

/*
 * Método para ler um arquivo com um conjunto de soluções não-dominadas e
 * salvar os dados na estrutura ir
 */
void ReadFile(string file_name, instance_result &ir, vector<vector<pair<unsigned, double>>>&non_dominated_sets)
{
    fstream file;
    string str;
    unsigned num;
    double d_num;
    pair<unsigned, double> p;

    file.open(file_name);

    //Instância
    file >> str;
    file >> str;
    ir.instance_name = str;

    //Nome do algoritmo
    file >> str;
    file >> str;
    ir.algorithm_name = str;

    //Tempo
    file >> str;
    file >> d_num;
    ir.time_limit = d_num;

    //Semente
    file >> str;
    file >> num;
    ir.seed = num;

    //Tempo passado
    file >> str;
    file >> d_num;
    ir.elapsed_time_sec = d_num;

    //Alpha
    file >> str;
    file >> d_num;

    //Tamanho da população
    file >> str;
    file >> num;

    //Probabilidade de mutação
    file >> str;
    file >> num;

    //Pular o nome dos objetivos
    file >> str;
    file >> str;

    ir.non_dominated_set.clear();
    //Ler o conjunto não-dominado
    while (file >> p.first >> p.second)
    {
        ir.non_dominated_set.push_back(p);
        //Se tem espaço
        if(file.peek() == '\t'){
            //Ler a string END
            file >> str;
            non_dominated_sets.push_back(ir.non_dominated_set);
            ir.non_dominated_set.clear();

            if(file.peek() != EOF){
                //Instância
                file >> str;
                file >> str;

                //Nome do algoritmo
                file >> str;
                file >> str;

                //Tempo
                file >> str;
                file >> num;

                //Semente
                file >> str;
                file >> num;
                ir.seed = num;

                //Semente
                file >> str;
                file >> num;

                //Alpha
                file >> str;
                file >> num;

                //Tamanho da população
                file >> str;
                file >> num;

                //Probabilidade de mutação
                file >> str;
                file >> num;

                //Pular o nome dos objetivos
                file >> str;
                file >> str;
            }

        }
        //Se final do arquivo
        if(file.peek() == EOF){
            return;
        }

    }

    file.close();
}
