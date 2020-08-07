#include "instance.h"

unsigned Instance::num_jobs = 0;
unsigned Instance::num_machine = 0;
unsigned Instance::num_planning_horizon = 0;
unsigned Instance::num_days = 0;
unsigned Instance::num_mode_op = 0;
double Instance::rate_on_peak = 0;
double Instance::rate_off_peak = 0;
unsigned Instance::max_cost = 0;
unsigned Instance::seed = 0;
unsigned Instance::discretization_factor = 1;

vector<unsigned> Instance::v_peak_start;
vector<unsigned> Instance::v_peak_end;
vector<vector<unsigned>> Instance::m_processing_time;
vector<vector<vector<unsigned>>> Instance::m_setup_time;
vector<double> Instance::v_speed_factor;
vector<double> Instance::v_consumption_factor;
vector<unsigned> Instance::v_machine_potency;

string Instance::file = "";

Instance::Instance()
{

}

void Instance::Init()
{
    Instance::v_speed_factor = vector <double>(Instance::num_mode_op+1);
    Instance::v_consumption_factor = vector <double>(Instance::num_mode_op+1);
    Instance::v_machine_potency = vector <unsigned>(Instance::num_machine+1);
    Instance::m_processing_time = vector< vector <unsigned> >(Instance::num_machine+1, vector<unsigned>(Instance::num_jobs+1));
    Instance::m_setup_time = vector<vector<vector<unsigned>>> (Instance::num_machine+1, vector<vector<unsigned>>(Instance::num_jobs+1, vector<unsigned>(Instance::num_jobs+1)));
}

void Instance::ReadJulioInstance(string instanceFile)
{
    Instance::file = instanceFile;

    ifstream arqEntrada;
    string next, aux;
    arqEntrada.open(instanceFile.c_str());
    //cout << instanceFile << endl;
    if(!arqEntrada.good()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura do número de máquinas
    getline(arqEntrada, next);
    aux = next.substr(11,next.size()-12);
    Instance::num_machine = size_t(stoi(aux));


    //Leitura do número de tarefas
    getline(arqEntrada, next);
    aux = next.substr(11,next.size()-12);
    Instance::num_jobs = size_t(stoi(aux));

    //Leitura da quantidade de horizonte de planejamentos
    getline(arqEntrada, next);
    aux = next.substr(12,next.size()-13);
    Instance::num_planning_horizon = size_t(stoi(aux));

    //Leitura do número de modos de operação
    getline(arqEntrada, next);
    aux = next.substr(10,next.size()-11);
    Instance::num_mode_op = size_t(stoi(aux));

    //Leitura do início do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(16,next.size()-17);
    Instance::v_peak_start.push_back(size_t(stoi(aux)));

    //Leitura do final do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(16,next.size()-17);
    Instance::v_peak_end.push_back(size_t(stoi(aux)));

    //Leitura da tarifa dentro do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(18,next.size()-19);
    Instance::rate_on_peak = stod(aux);

    //Leitura da tarifa fora do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(19,next.size()-20);
    Instance::rate_off_peak = stod(aux);

    //Leitura do custo máximo
    getline(arqEntrada, next);
    aux = next.substr(19,next.size()-20);
    Instance::max_cost = size_t(stoi(aux));

    Instance::Init();

    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura das velocidades
    for (size_t i = 1; i <= Instance::num_mode_op; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        getline(iss, aux, ' ');
        Instance::v_speed_factor[i] = stod(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura dos lambdas
    for (size_t i = 1; i <= Instance::num_mode_op; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        getline(iss, aux, ' ');
        Instance::v_consumption_factor[i] = stod(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura das potências
    for (size_t i = 1; i <= Instance::num_machine; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        getline(iss, aux, ' ');
        Instance::v_machine_potency[i] = stoi(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    size_t a, b, c;

    //Leitura do tempo de processamento
    for (size_t i = 1; i <= Instance::num_jobs*Instance::num_machine; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        a = size_t(stoi(aux));
        getline(iss, aux, ' ');
        b = size_t(stoi(aux));
        getline(iss, aux, ' ');
        Instance::m_processing_time[a][b] = stoi(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura do tempo de preparação
    for (size_t i = 1; i <= Instance::num_machine*Instance::num_jobs*Instance::num_jobs; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        a = size_t(stoi(aux));
        getline(iss, aux, ' ');
        b = size_t(stoi(aux));
        getline(iss, aux, ' ');
        c = size_t(stoi(aux));
        getline(iss, aux, ' ');
        Instance::m_setup_time[a][b][c] = stoi(aux);
    }

    arqEntrada.close();
}

void Instance::ReadMarceloInstance(string instance_file_name)
{
    Instance::file = instance_file_name;

    ifstream arqEntrada;
    string next;
    unsigned u_num;
    double d_num;
    arqEntrada.open(instance_file_name.c_str());
    //cout << instanceFile << endl;
    if(!arqEntrada.good()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    //Leitura do número de tarefas
    arqEntrada >> next;
    arqEntrada >> u_num;
    Instance::num_jobs = u_num;

    //Leitura do número de máquinas
    arqEntrada >> next;
    arqEntrada >> u_num;
    Instance::num_machine = u_num;

    //Leitura do número de dias
    arqEntrada >> next;
    arqEntrada >> u_num;
    Instance::num_days = u_num;

    //Leitura da quantidade de horizonte de planejamentos
    arqEntrada >> next;
    arqEntrada >> u_num;
    Instance::num_planning_horizon = u_num;

    //Leitura do número de modos de operação
    arqEntrada >> next;
    arqEntrada >> u_num;
    Instance::num_mode_op = u_num;

    //Leitura da tarifa dentro do horário de pico
    arqEntrada >> next;
    arqEntrada >> d_num;
    Instance::rate_on_peak = d_num;

    //Leitura da tarifa fora do horário de pico
    arqEntrada >> next;
    arqEntrada >> d_num;
    Instance::rate_off_peak = d_num;

    //Leitura do custo máximo
    arqEntrada >> next;
    arqEntrada >> u_num;
    Instance::max_cost = u_num;

    Instance::Init();

    arqEntrada >> next;

    //Leitura do início do horário de pico
    for (unsigned i = 0; i < Instance::num_days; i++) {
        //Leitura do início do horário de pico
        //arqEntrada >> next;
        arqEntrada >> u_num;
        Instance::v_peak_start.push_back(u_num);
    }

    arqEntrada >> next;

    //Leitura do fim do horário de pico
    for (unsigned i = 0; i < Instance::num_days; i++) {
        //Leitura do final do horário de pico
        //arqEntrada >> next;
        arqEntrada >> u_num;
        Instance::v_peak_end.push_back(u_num);
    }

    arqEntrada >> next;

    //Leitura das velocidades
    for (unsigned i = 1; i <= Instance::num_mode_op; i++) {
        arqEntrada >> d_num;
        Instance::v_speed_factor[i] = d_num;
    }

    arqEntrada >> next;

    //Leitura dos lambdas
    for (unsigned i = 1; i <= Instance::num_mode_op; i++) {
        arqEntrada >> d_num;
        Instance::v_consumption_factor[i] = d_num;
    }

    arqEntrada >> next;

    //Leitura das potências
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        arqEntrada >> u_num;
        Instance::v_machine_potency[i] = u_num;
    }

    arqEntrada >> next;

    //Leitura do tempo de processamento
    for (unsigned j = 1; j <= Instance::num_jobs; j++) {
        for (unsigned i = 1; i <= Instance::num_machine; i++) {
            arqEntrada >> u_num;
            Instance::m_processing_time[i][j] = u_num;
        }
    }

    arqEntrada >> next;

    //Leitura do tempo de preparação
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned k = 1; k <= Instance::num_jobs; k++) {
                arqEntrada >> u_num;
                Instance::m_setup_time[i][j][k] = u_num;
                if(j == k){
                    Instance::m_setup_time[i][0][k] = u_num;
                }
            }
        }
    }

    arqEntrada.close();
}

void Instance::PrintInstance1()
{
    cout << "Número de máquinas: " << Instance::num_machine << endl;
    cout << "Número de tarefas: " << Instance::num_jobs << endl;
    cout << "Número de horizontes de planejamentos: " << Instance::num_planning_horizon << endl;
    cout << "Número de modos de operação: " << Instance::num_mode_op << endl;
    cout << "Tarifa dentro do horário de pico: " << Instance::rate_on_peak << endl;
    cout << "Tarifa fora do horário de pico: " << Instance::rate_off_peak << endl;
    cout << "Custo máximo: " << Instance::max_cost << endl;

    cout << "Horário de pico" << endl;
    for (size_t i = 0; i <= Instance::num_days; i++) {
        cout << Instance::v_peak_start[i] << "\t" << Instance::v_peak_end[i] << endl;
    }

    cout << "Fator de velocidade " << endl;
    for (size_t i = 1; i <= Instance::num_mode_op; i++) {
        cout << Instance::v_speed_factor[i] << " ";
    }
    cout << endl;

    cout << "Fator de consumo " << endl;
    for (size_t i = 1; i <= Instance::num_mode_op; i++) {
        cout << Instance::v_consumption_factor[i] << " ";
    }
    cout << endl;

    cout << "Potência da máquina" << endl;
    for (size_t i = 1; i <= Instance::num_machine; i++) {
        cout << Instance::v_machine_potency[i] << " ";
    }
    cout << endl;

    cout << "Tempo de processamento" << endl;
    for (size_t i = 1; i <= Instance::num_machine; i++) {
        for (size_t j = 1; j <= Instance::num_jobs; j++) {
            cout << Instance::m_processing_time[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "Tempo de preparação" << endl;
    for (size_t i = 1; i <= Instance::num_machine; i++) {
        for (size_t j = 1; j <= Instance::num_jobs; j++) {
            for (size_t k = 1; k <= Instance::num_jobs; k++) {
                cout << Instance::m_setup_time[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}
