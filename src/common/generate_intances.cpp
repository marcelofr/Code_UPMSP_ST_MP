#include "generate_intances.h"

void GenerateSmallInstances()
{

    InstanceParam ip;

    srand (time(NULL));

    //TODO mudar aqui
    vector<unsigned> num_jobs;
    for(unsigned i=1; i<=5; i++){
        num_jobs.push_back(5 + (1*i));
    }

    //TODO mudar aqui
    vector<unsigned> num_machines;
    for(unsigned i=1; i<=1; i++){
        num_machines.push_back(1 + i);
    }

    ip.num_op_mode = 3;

    vector<pair<unsigned, unsigned>> v_range_setup(1);

    v_range_setup[0].first = 1;
    v_range_setup[0].second = 9;

    pair<unsigned, unsigned>range_processing;
    range_processing.first = 1;
    range_processing.second = 99;
    ip.range_processing = range_processing;


    pair<unsigned, unsigned>range_potency;
    range_potency.first = 100;
    range_potency.second = 200;
    ip.range_potency = range_potency;

    ip.destination = "../../Instances/Small/";

    ip.speed_factor.push_back(1.2);
    ip.speed_factor.push_back(1);
    ip.speed_factor.push_back(0.8);

    ip.consumption_factor.push_back(1.5);
    ip.consumption_factor.push_back(1);
    ip.consumption_factor.push_back(0.6);



    for(auto it_jobs : num_jobs){
        for(auto it_machines : num_machines){
            for (auto it_setup: v_range_setup) {

                ip.num_jobs = it_jobs;
                ip.num_machines = it_machines;

                ip.range_setup = it_setup;

                CreateNewIntance(ip);

            }
        }
    }
}

void CreateNewIntance(InstanceParam ip)
{

    string file_name;

    file_name = itos(ip.num_jobs);
    file_name += "_";
    file_name += itos(ip.num_machines);
    file_name += "_";
    file_name += itos(PLAN_HORIZON_SIZE);
    file_name += "_";
    file_name += itos(ip.num_op_mode);
    file_name += "_S_";
    file_name += itos(ip.range_setup.first) + "-" + itos(ip.range_setup.second);
    file_name += ".dat";

    string full_name_file;

    full_name_file = ip.destination+file_name;

    ofstream MyFile;
    MyFile.open(full_name_file);

    MyFile << "n " << ip.num_jobs << endl;
    MyFile << "m " << ip.num_machines << endl;
    MyFile << "hl " << PLAN_HORIZON_SIZE << endl;
    MyFile << "o " << ip.num_op_mode << endl;
    MyFile << "peak_start " << PEAK_START << endl;
    MyFile << "peak_end " << PEAK_END << endl;
    MyFile << "rate_in_peak " << RATE_IN_PEAK << endl;
    MyFile << "rate_off_peak " << RATE_OFF_PEAK << endl;
    MyFile << "max_cost " << MAX_COST << endl;

    MyFile << endl;

    MyFile << "v" << endl;
    for(unsigned i=0; i<ip.speed_factor.size(); i++){
        //MyFile << i+1 << " " << ip.speed_factor[i] << endl;
        MyFile << ip.speed_factor[i] << endl;
    }

    MyFile << endl;

    MyFile << "lambda" << endl;
    for(unsigned i=0; i<ip.consumption_factor.size(); i++){
        //MyFile << i+1 << " " << ip.consumption_factor[i] << endl;
        MyFile << ip.consumption_factor[i] << endl;
    }

    MyFile << endl;

    MyFile << "pi" << endl;
    ip.machine_potency.resize(ip.num_machines);
    for(unsigned i=0; i<ip.machine_potency.size(); i++){
        //MyFile << i+1 << " " << ip.machine_potency[i] << endl;
        ip.machine_potency[i] = ip.range_potency.first + rand()%(ip.range_potency.second - ip.range_potency.first);
        MyFile << ip.machine_potency[i] << endl;
    }

    MyFile << endl;

    MyFile << "processing" << endl;
    unsigned processing_time;
    /*for(unsigned i=0; i<ip.num_machines; i++){
        MyFile << "\t" << i+1;
    }

    MyFile << endl;*/

    for(unsigned j=0; j<ip.num_jobs; j++){
        //MyFile << j+1 << "\t";
        for(unsigned i=0; i<ip.num_machines; i++){
            //processing_time = MIN_PROCESSING_TIME + rand()%MAX_PROCESSING_TIME;
            processing_time = ip.range_processing.first + rand()%(ip.range_processing.second-ip.range_processing.first);
            MyFile << processing_time << "\t";
        }
        MyFile << endl;
    }

    MyFile << endl;

    MyFile << "setup" << endl;
    unsigned min, max;
    vector<vector<vector<unsigned>>> setup (ip.num_machines+1,
                                          vector<vector<unsigned>>(ip.num_jobs+1,
                                              vector<unsigned>(ip.num_jobs+1)));

    for(unsigned i=1; i<=ip.num_machines; i++){

        for(unsigned j=1; j<=ip.num_jobs; j++){

            for(unsigned k=1; k<=ip.num_jobs; k++){

                if(j!=k){

                    //Definir o intervalo padrão para geração dos tempos de preparação
                    //min = MIN_SETUP_TIME;
                    min = ip.range_setup.first;
                    //max = MAX_SETUP_TIME;
                    max = ip.range_setup.second;

                    //Percorrer todas as outras tarefas
                    for(unsigned l=1; l<=ip.num_jobs; l++){

                        //Limitar apenas quando os tempos de preparação já foram definidos
                        if(l!=j && l!=k && setup[i][j][l]>0 && setup[i][l][k]>0){

                            //O valor gerado não pode ser inferior a diferença dos outros dois
                            min = abs(int(setup[i][j][l]-setup[i][l][k]))+1;
                            //if(min > MAX_SETUP_TIME){
                            if(min > ip.range_setup.second){
                                max = min + 1;
                            }
                            else{
                                //O valor gerado não pode ser superior a soma dos outros dois
                                max = setup[i][j][l]+setup[i][l][k] - 1;

                                //MAX_SETUP_TIME é um outro limite para o valor gerado
                                //if(max > MAX_SETUP_TIME){
                                if(max > ip.range_setup.second){
                                    //max = MAX_SETUP_TIME;
                                    max = ip.range_setup.second;
                                }
                            }
                        }
                    }

                }
                else{

                    //Definir o intervalo padrão para geração dos tempos de preparação
                    //min = MIN_SETUP_TIME;
                    //min = ip.range_setup.first;
                    min = 0;
                    //max = MAX_SETUP_TIME;
                    //max = ip.range_setup.second;
                    max = 0;

                    /*//Percorrer todas as outras tarefas
                    for(unsigned l=1; l<=ip.num_jobs; l++){

                        //Limitar apenas quando os tempos de preparação já foram definidos
                        if(l!=j && l!=k && setup[i][l][l]>0 && setup[i][l][k]>0){

                            //O valor gerado não pode ser inferior a diferença dos outros dois
                            min = abs(int(setup[i][l][l]-setup[i][l][k]))+1;
                            //if(min > MAX_SETUP_TIME){
                            if(min > ip.range_setup.second){
                                max = min + 1;
                            }
                            else{
                                //O valor gerado não pode ser superior a soma dos outros dois
                                max = setup[i][l][l]+setup[i][l][k] - 1;

                                //MAX_SETUP_TIME é um outro limite para o valor gerado
                                //if(max > MAX_SETUP_TIME){
                                if(max > ip.range_setup.second){
                                    //max = MAX_SETUP_TIME;
                                    max = ip.range_setup.second;
                                }
                            }
                        }
                    }*/

                }

                if(max != min){
                    //Gerar um número entre min e max
                    setup[i][j][k] = min + rand()%(max - min);
                }
                else{
                    setup[i][j][k] = min;
                }

                //Salvar o número no arquivo
                MyFile << setup[i][j][k] << "\t";

                MyFile.flush();

            }

            MyFile << endl;

        }

        MyFile << endl;

    }


    MyFile.close();
}

void GenerateLargeInstances()
{

    InstanceParam ip;

    vector<unsigned> num_jobs;
    for(unsigned i=1; i<=5; i++){
        num_jobs.push_back(i*50);
    }

    vector<unsigned> num_machines;
    for(unsigned i=1; i<=3; i++){
        num_machines.push_back(i*10);
    }

    ip.num_op_mode = 5;

    vector<pair<unsigned, unsigned>> v_range_setup(2);

    v_range_setup[0].first = 1;
    v_range_setup[0].second = 9;

    v_range_setup[1].first = 1;
    v_range_setup[1].second = 124;

    pair<unsigned, unsigned>range_processing;
    range_processing.first = 1;
    range_processing.second = 99;
    ip.range_processing = range_processing;


    pair<unsigned, unsigned>range_potency;
    range_potency.first = 40;
    range_potency.second = 200;
    ip.range_potency = range_potency;

    ip.destination = "../../Instances/New/";

    ip.speed_factor.push_back(1.2);
    ip.speed_factor.push_back(1.1);
    ip.speed_factor.push_back(1);
    ip.speed_factor.push_back(0.9);
    ip.speed_factor.push_back(0.8);

    ip.consumption_factor.push_back(1.5);
    ip.consumption_factor.push_back(1.25);
    ip.consumption_factor.push_back(1);
    ip.consumption_factor.push_back(0.8);
    ip.consumption_factor.push_back(0.6);

    for(auto it_jobs : num_jobs){
        for(auto it_machines : num_machines){
            for (auto it_setup: v_range_setup) {

                ip.num_jobs = it_jobs;
                ip.num_machines = it_machines;

                ip.range_setup = it_setup;

                CreateNewIntance(ip);

            }
        }
    }
}
