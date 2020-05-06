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
    static unsigned num_jobs, num_machine, num_planning_horizon;
    static unsigned num_mode_op, peak_start, peak_end, max_cost;
    static double rate_on_peak, rate_off_peak;

    //Um fator de velocidade para cada modo de operação (v_l)
    static vector<double> v_speed_factor;

    //Um fator de consumo para cada modo de operação (lambda_l)
    static vector<double> v_consumption_factor;

    //Uma potência para cada maquina (pi_i)
    static vector<unsigned> v_machine_potency;

    //Um tempo de processamento para cada tarefa em cada máquina
    //ProcessingTime[k][j] tempo de processamento da tarefa j na máquina k
    static vector<vector<unsigned>> m_processing_time;
    //Tempo de preparação para cada máquina e cada par de tarefas
    //SetupTime[k][i][j] tempo de preparação para processar a tarefa j
    //após a tarefa i na máquina k
    static vector<vector<vector<unsigned>>> m_setup_time;

    Instance();

    static void Init();

    static void ReadJulioInstance(string instanceFile);
    static void ReadMarceloInstance(string instanceFile);
    static void PrintInstance1();

};

#endif // INSTANCE_H
