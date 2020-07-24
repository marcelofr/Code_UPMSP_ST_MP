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
void ReadFile(solution_data &sd)
{
    fstream file;
    string str;
    unsigned num;
    double d_num;
    pair<unsigned, double> p;

    file.open(sd.file_solution);

    //Instância
    file >> str;
    file >> str;
    sd.instance_name = str;

    //Nome do algoritmo
    file >> str;
    file >> str;
    sd.algorithm_name = str;

    //Tempo
    file >> str;
    file >> d_num;
    sd.time_limit = d_num;

    //Semente
    file >> str;
    file >> num;
    sd.seed = num;

    //Tempo passado
    file >> str;
    file >> d_num;
    sd.elapsed_time_sec = d_num;

    //Alpha
    file >> str;
    file >> d_num;
    sd.alpha = d_num;

    //Tamanho da população
    file >> str;
    file >> num;
    sd.population_size = num;

    //Probabilidade de mutação
    file >> str;
    file >> num;
    sd.prob_mutation = num;

    //Pular o nome dos objetivos
    file >> str;
    file >> str;

    sd.non_dominated_set.clear();
    //Ler o conjunto não-dominado
    while (file >> p.first >> p.second)
    {
        sd.non_dominated_set.push_back(p);
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
