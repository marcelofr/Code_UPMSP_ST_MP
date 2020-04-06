#include "instance.h"

size_t Instance::numJobs = 0;
size_t Instance::numMachine = 0;
size_t Instance::numPlanningHorizon = 0;
size_t Instance::numModeOp = 0;
size_t Instance::peakStart = 0;
size_t Instance::peakEnd = 0;
double Instance::rateOnPeakHours = 0;
double Instance::rateOffPeakHours = 0;
size_t Instance::maxCost = 0;

vector<vector<int>> Instance::ProcessingTime;
vector<vector<vector<int>>> Instance::SetupTime;
vector<double> Instance::SpeedFactor;
vector<double> Instance::ConsumptionFactor;
vector<int> Instance::MachinePotency;

string Instance::file = "";

Instance::Instance()
{

}

void Instance::Init()
{
    Instance::SpeedFactor = vector <double>(Instance::numModeOp+1);
    Instance::ConsumptionFactor = vector <double>(Instance::numModeOp+1);
    Instance::MachinePotency = vector <int>(Instance::numMachine+1);
    Instance::ProcessingTime = vector< vector <int> >(Instance::numJobs+1, vector<int>(Instance::numJobs+1));
    Instance::SetupTime = vector<vector<vector<int>>> (Instance::numMachine+1, vector<vector<int>>(Instance::numJobs+1, vector<int>(Instance::numJobs+1)));
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
    Instance::numMachine = size_t(stoi(aux));


    //Leitura do número de tarefas
    getline(arqEntrada, next);
    aux = next.substr(11,next.size()-12);
    Instance::numJobs = size_t(stoi(aux));

    //Leitura da quantidade de horizonte de planejamentos
    getline(arqEntrada, next);
    aux = next.substr(12,next.size()-13);
    Instance::numPlanningHorizon = size_t(stoi(aux));

    //Leitura do número de modos de operação
    getline(arqEntrada, next);
    aux = next.substr(10,next.size()-11);
    Instance::numModeOp = size_t(stoi(aux));

    //Leitura do início do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(16,next.size()-17);
    Instance::peakStart = size_t(stoi(aux));

    //Leitura do final do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(16,next.size()-17);
    Instance::peakEnd = size_t(stoi(aux));

    //Leitura da tarifa dentro do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(18,next.size()-19);
    Instance::rateOnPeakHours = stod(aux);

    //Leitura da tarifa fora do horário de pico
    getline(arqEntrada, next);
    aux = next.substr(19,next.size()-20);
    Instance::rateOffPeakHours = stod(aux);

    //Leitura do custo máximo
    getline(arqEntrada, next);
    aux = next.substr(19,next.size()-20);
    Instance::maxCost = size_t(stoi(aux));

    Instance::Init();

    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura das velocidades
    for (size_t i = 1; i <= Instance::numModeOp; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        getline(iss, aux, ' ');
        Instance::SpeedFactor[i] = stod(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura dos lambdas
    for (size_t i = 1; i <= Instance::numModeOp; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        getline(iss, aux, ' ');
        Instance::ConsumptionFactor[i] = stod(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura das potências
    for (size_t i = 1; i <= Instance::numMachine; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        getline(iss, aux, ' ');
        Instance::MachinePotency[i] = stoi(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    size_t a, b, c;

    //Leitura do tempo de processamento
    for (size_t i = 1; i <= Instance::numJobs*Instance::numMachine; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        a = size_t(stoi(aux));
        getline(iss, aux, ' ');
        b = size_t(stoi(aux));
        getline(iss, aux, ' ');
        Instance::ProcessingTime[a][b] = stoi(aux);
    }

    getline(arqEntrada, next);
    getline(arqEntrada, next);
    getline(arqEntrada, next);

    //Leitura do tempo de preparação
    for (size_t i = 1; i <= Instance::numMachine*Instance::numJobs*Instance::numJobs; i++) {
        getline(arqEntrada, next);
        istringstream iss(next);
        getline(iss, aux, ' ');
        a = size_t(stoi(aux));
        getline(iss, aux, ' ');
        b = size_t(stoi(aux));
        getline(iss, aux, ' ');
        c = size_t(stoi(aux));
        getline(iss, aux, ' ');
        Instance::SetupTime[a][b][c] = stoi(aux);
    }

    arqEntrada.close();
}

void Instance::PrintInstance1()
{
    cout << "Número de máquinas: " << Instance::numMachine << endl;
    cout << "Número de tarefas: " << Instance::numJobs << endl;
    cout << "Número de horizontes de planejamentos: " << Instance::numPlanningHorizon << endl;
    cout << "Número de modos de operação: " << Instance::numModeOp << endl;
    cout << "Início do horário de pico: " << Instance::peakStart << endl;
    cout << "Final do horário de pico: " << Instance::peakEnd << endl;
    cout << "Tarifa dentro do horário de pico: " << Instance::rateOnPeakHours << endl;
    cout << "Tarifa fora do horário de pico: " << Instance::rateOffPeakHours << endl;
    cout << "Custo máximo: " << Instance::maxCost << endl;

    cout << "Fator de velocidade " << endl;
    for (size_t i = 1; i <= Instance::numModeOp; i++) {
        cout << Instance::SpeedFactor[i] << " ";
    }
    cout << endl;

    cout << "Fator de consumo " << endl;
    for (size_t i = 1; i <= Instance::numModeOp; i++) {
        cout << Instance::ConsumptionFactor[i] << " ";
    }
    cout << endl;

    cout << "Potência da máquina" << endl;
    for (size_t i = 1; i <= Instance::numMachine; i++) {
        cout << Instance::MachinePotency[i] << " ";
    }
    cout << endl;

    cout << "Tempo de processamento" << endl;
    for (size_t i = 1; i <= Instance::numMachine; i++) {
        for (size_t j = 1; j <= Instance::numJobs; j++) {
            cout << Instance::ProcessingTime[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "Tempo de preparação" << endl;
    for (size_t i = 1; i <= Instance::numMachine; i++) {
        for (size_t j = 1; j <= Instance::numJobs; j++) {
            for (size_t k = 1; k <= Instance::numJobs; k++) {
                cout << Instance::SetupTime[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}
