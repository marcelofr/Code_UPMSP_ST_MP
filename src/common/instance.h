#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>


using namespace std;

class Instance
{
public:
    static string file;
    //fatorVelocidade => v_l
    //fatorPotencia => lambda_l
    //horizontePlanej => H
    static size_t numJobs, numMachine, numPlanningHorizon, numModeOp, peakStart, peakEnd, maxCost;
    static double rateOnPeakHours, rateOffPeakHours;

    //Um fator de velocidade para cada modo de operação (v_l)
    static vector<double> SpeedFactor;

    //Um fator de consumo para cada modo de operação (lambda_l)
    static vector<double> ConsumptionFactor;

    //Uma potência para cada maquina (pi_i)
    static vector<int> MachinePotency;

    //Um tempo de processamento para cada tarefa em cada máquina
    //ProcessingTime[k][j] tempo de processamento da tarefa j na máquina k
    static vector<vector<int>> ProcessingTime;
    //Tempo de preparação para cada máquina e cada par de tarefas
    //SetupTime[k][i][j] tempo de preparação para processar a tarefa j
    //após a tarefa i na máquina k
    static vector<vector<vector<int>>> SetupTime;

    Instance();

    static void Init();

    static void ReadJulioInstance(string instanceFile);
    static void PrintInstance1();

};

#endif // INSTANCE_H
