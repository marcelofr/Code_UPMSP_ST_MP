#include "file_solution.h"

/*
 * Método para identificar os arquivos presentes em uma pasta e
 * salvar seus nomes em um vetor, considera apenas os arquivos com a extensão .sol
 */
void FindFilesInFolder(string folder_solution, vector<string> &files)
{
    int a;
    //Criar um vetor com o nome de todos os arquivos do diretório
    for (auto entry : filesystem::directory_iterator(folder_solution)){
        a = entry.path().string().find(".sol");
        if(a >= 0){
            files.push_back(entry.path());
        }
    }
}

/*
 * Método para ler um arquivo com um conjunto de soluções não-dominadas e
 * salvar os dados na estrutura ir
 */
void ReadFile(algorithm_data &alg_data)
{
    fstream file;
    string str;
    unsigned num;
    double d_num;
    pair<unsigned, double> p;

    file.open(alg_data.param.file_solution);

    //Instância
    file >> str;
    file >> str;
    alg_data.param.instance_name = str;

    //Nome do algoritmo
    file >> str;
    file >> str;
    alg_data.param.algorithm_name = str;

    //Tempo
    file >> str;
    file >> d_num;
    alg_data.time_limit = d_num;

    //Semente
    file >> str;
    file >> num;
    alg_data.param.u_seed = num;

    //Tempo passado
    file >> str;
    file >> d_num;
    alg_data.elapsed_time_sec = d_num;

    //Alpha
    file >> str;
    file >> d_num;
    alg_data.param.d_alpha = d_num;

    //Tamanho da população
    file >> str;
    file >> num;
    alg_data.param.u_population_size = num;

    //Probabilidade de mutação
    file >> str;
    file >> num;
    alg_data.param.u_prob_mutation = num;

    //Pular o nome dos objetivos
    file >> str;
    file >> str;

    alg_data.non_dominated_set.clear();
    //Ler o conjunto não-dominado
    while (file >> p.first >> p.second)
    {
        alg_data.non_dominated_set.push_back(p);
        //Se tem espaço
        if(file.peek() == '\t'){
            //Ler a string END
            file >> str;

        }
        //Se final do arquivo
        if(file.peek() == EOF){
            break;
        }

    }

    file.close();
}
