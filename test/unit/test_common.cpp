#include "test_common.h"

/*!
 *
 * \class TestCommon
 *
 * \brief Class to test the common class
 *
 */

/*!
 *
 * Unit Test of Common class constructor
 *
 */

TestCommon::TestCommon()
{
    //srand(10683);
}

/*!
 *
 * Unit Test of Common class destructor
 *
 */

TestCommon::~TestCommon()
{

}

void TestCommon::SetUp()
{
    //srand(10683);
}


void TestCommon::TearDown()
{

}

/*!
 * Unit test to check the instances values
 */

TEST_F(TestCommon, ReadMarceloInstance) {


    Instance::ReadMarceloInstance("../../../Instances/SMALL/6_2_1439_3_S_1-9.dat");

    EXPECT_EQ(Instance::num_jobs, 6);
    EXPECT_EQ(Instance::num_machine, 2);
    EXPECT_EQ(Instance::num_days, 1);
    EXPECT_EQ(Instance::num_planning_horizon, 1439);
    EXPECT_EQ(Instance::num_mode_op, 3);
    EXPECT_EQ(Instance::rate_on_peak, 0.47753);
    EXPECT_EQ(Instance::rate_off_peak, 0.32282);
    EXPECT_EQ(Instance::max_cost, 148);

    EXPECT_EQ(Instance::v_peak_start[0], 1080);
    EXPECT_EQ(Instance::v_peak_end[0], 1259);

    EXPECT_EQ(Instance::v_speed_factor[1], 1.2);
    EXPECT_EQ(Instance::v_speed_factor[2], 1.0);
    EXPECT_EQ(Instance::v_speed_factor[3], 0.8);

    EXPECT_EQ(Instance::v_consumption_factor[1], 1.5);
    EXPECT_EQ(Instance::v_consumption_factor[2], 1.0);
    EXPECT_EQ(Instance::v_consumption_factor[3], 0.6);

    EXPECT_EQ(Instance::v_machine_potency[1], 170);
    EXPECT_EQ(Instance::v_machine_potency[2], 171);

    EXPECT_EQ(Instance::m_processing_time[1][1], 27);
    EXPECT_EQ(Instance::m_processing_time[2][1], 89);
    EXPECT_EQ(Instance::m_processing_time[1][2], 73);
    EXPECT_EQ(Instance::m_processing_time[2][2], 8);
    EXPECT_EQ(Instance::m_processing_time[1][3], 42);
    EXPECT_EQ(Instance::m_processing_time[2][3], 89);
    EXPECT_EQ(Instance::m_processing_time[1][4], 2);
    EXPECT_EQ(Instance::m_processing_time[2][4], 19);
    EXPECT_EQ(Instance::m_processing_time[1][5], 16);
    EXPECT_EQ(Instance::m_processing_time[2][5], 98);
    EXPECT_EQ(Instance::m_processing_time[1][6], 78);
    EXPECT_EQ(Instance::m_processing_time[2][6], 14);

    EXPECT_EQ(Instance::m_setup_time[1][1][1], 0);
    EXPECT_EQ(Instance::m_setup_time[1][1][2], 7);
    EXPECT_EQ(Instance::m_setup_time[1][1][3], 5);
    EXPECT_EQ(Instance::m_setup_time[1][1][4], 5);
    EXPECT_EQ(Instance::m_setup_time[1][1][5], 3);
    EXPECT_EQ(Instance::m_setup_time[1][1][6], 3);

    EXPECT_EQ(Instance::m_setup_time[1][2][1], 5);
    EXPECT_EQ(Instance::m_setup_time[1][2][2], 0);
    EXPECT_EQ(Instance::m_setup_time[1][2][3], 7);
    EXPECT_EQ(Instance::m_setup_time[1][2][4], 2);
    EXPECT_EQ(Instance::m_setup_time[1][2][5], 4);
    EXPECT_EQ(Instance::m_setup_time[1][2][6], 6);

    EXPECT_EQ(Instance::m_setup_time[1][3][1], 5);
    EXPECT_EQ(Instance::m_setup_time[1][3][2], 4);
    EXPECT_EQ(Instance::m_setup_time[1][3][3], 0);
    EXPECT_EQ(Instance::m_setup_time[1][3][4], 3);
    EXPECT_EQ(Instance::m_setup_time[1][3][5], 5);
    EXPECT_EQ(Instance::m_setup_time[1][3][6], 3);

    EXPECT_EQ(Instance::m_setup_time[1][4][1], 2);
    EXPECT_EQ(Instance::m_setup_time[1][4][2], 7);
    EXPECT_EQ(Instance::m_setup_time[1][4][3], 1);
    EXPECT_EQ(Instance::m_setup_time[1][4][4], 0);
    EXPECT_EQ(Instance::m_setup_time[1][4][5], 5);
    EXPECT_EQ(Instance::m_setup_time[1][4][6], 3);

    EXPECT_EQ(Instance::m_setup_time[1][5][1], 6);
    EXPECT_EQ(Instance::m_setup_time[1][5][2], 3);
    EXPECT_EQ(Instance::m_setup_time[1][5][3], 5);
    EXPECT_EQ(Instance::m_setup_time[1][5][4], 4);
    EXPECT_EQ(Instance::m_setup_time[1][5][5], 0);
    EXPECT_EQ(Instance::m_setup_time[1][5][6], 2);

    EXPECT_EQ(Instance::m_setup_time[1][6][1], 8);
    EXPECT_EQ(Instance::m_setup_time[1][6][2], 2);
    EXPECT_EQ(Instance::m_setup_time[1][6][3], 6);
    EXPECT_EQ(Instance::m_setup_time[1][6][4], 7);
    EXPECT_EQ(Instance::m_setup_time[1][6][5], 3);
    EXPECT_EQ(Instance::m_setup_time[1][6][6], 0);


    EXPECT_EQ(Instance::m_setup_time[2][1][1], 0);
    EXPECT_EQ(Instance::m_setup_time[2][1][2], 6);
    EXPECT_EQ(Instance::m_setup_time[2][1][3], 6);
    EXPECT_EQ(Instance::m_setup_time[2][1][4], 6);
    EXPECT_EQ(Instance::m_setup_time[2][1][5], 4);
    EXPECT_EQ(Instance::m_setup_time[2][1][6], 2);

    EXPECT_EQ(Instance::m_setup_time[2][2][1], 2);
    EXPECT_EQ(Instance::m_setup_time[2][2][2], 0);
    EXPECT_EQ(Instance::m_setup_time[2][2][3], 6);
    EXPECT_EQ(Instance::m_setup_time[2][2][4], 6);
    EXPECT_EQ(Instance::m_setup_time[2][2][5], 3);
    EXPECT_EQ(Instance::m_setup_time[2][2][6], 2);

    EXPECT_EQ(Instance::m_setup_time[2][3][1], 5);
    EXPECT_EQ(Instance::m_setup_time[2][3][2], 3);
    EXPECT_EQ(Instance::m_setup_time[2][3][3], 0);
    EXPECT_EQ(Instance::m_setup_time[2][3][4], 6);
    EXPECT_EQ(Instance::m_setup_time[2][3][5], 1);
    EXPECT_EQ(Instance::m_setup_time[2][3][6], 3);

    EXPECT_EQ(Instance::m_setup_time[2][4][1], 6);
    EXPECT_EQ(Instance::m_setup_time[2][4][2], 5);
    EXPECT_EQ(Instance::m_setup_time[2][4][3], 5);
    EXPECT_EQ(Instance::m_setup_time[2][4][4], 0);
    EXPECT_EQ(Instance::m_setup_time[2][4][5], 5);
    EXPECT_EQ(Instance::m_setup_time[2][4][6], 5);

    EXPECT_EQ(Instance::m_setup_time[2][5][1], 2);
    EXPECT_EQ(Instance::m_setup_time[2][5][2], 5);
    EXPECT_EQ(Instance::m_setup_time[2][5][3], 5);
    EXPECT_EQ(Instance::m_setup_time[2][5][4], 8);
    EXPECT_EQ(Instance::m_setup_time[2][5][5], 0);
    EXPECT_EQ(Instance::m_setup_time[2][5][6], 6);

    EXPECT_EQ(Instance::m_setup_time[2][6][1], 1);
    EXPECT_EQ(Instance::m_setup_time[2][6][2], 6);
    EXPECT_EQ(Instance::m_setup_time[2][6][3], 6);
    EXPECT_EQ(Instance::m_setup_time[2][6][4], 5);
    EXPECT_EQ(Instance::m_setup_time[2][6][5], 4);
    EXPECT_EQ(Instance::m_setup_time[2][6][6], 0);

}

TEST_F(TestCommon, GreedyInitialSolutionMakespan) {

    Solution * my_solution = new Solution();

    Instance::ReadMarceloInstance("../../../Instances/SMALL/6_2_1439_3_S_1-9.dat");
    Instance::seed = 10683;

    my_solution->GenerateGreedySolutionMakespan();

    //Verificar a sequência das tarefas
    EXPECT_EQ(my_solution->scheduling[1][0], 3);
    EXPECT_EQ(my_solution->scheduling[1][1], 1);
    EXPECT_EQ(my_solution->scheduling[1][2], 5);
    EXPECT_EQ(my_solution->scheduling[2][0], 2);
    EXPECT_EQ(my_solution->scheduling[2][1], 6);
    EXPECT_EQ(my_solution->scheduling[2][2], 4);

    //Verificar o modo de operação das tarefas
    unsigned mode_op = 1;
    for (auto it = my_solution->job_mode_op.begin()+1; it != my_solution->job_mode_op.end(); ++it) {
        EXPECT_EQ(*it, mode_op);
    }

    //Verificar o tempo de processamento de cada tarefa
    unsigned job, machine, previos;
    int pt, st, ct;

    machine = 1;
    previos = 0;
    ct = 0;

    job = my_solution->scheduling[machine][0];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 35), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 0);
    previos = job;
    ct += pt + st;


    job = my_solution->scheduling[machine][1];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 23), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 5);
    previos = job;
    ct += pt + st;

    job = my_solution->scheduling[machine][2];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 14), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 3);
    ct += pt + st;


    EXPECT_EQ(ct, 80);

    machine = 2;
    previos = 0;
    ct = 0;

    job = my_solution->scheduling[machine][0];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 7), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 0);
    previos = job;
    ct += pt + st;

    job = my_solution->scheduling[machine][1];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 12), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 2);
    previos = job;
    ct += pt + st;

    job = my_solution->scheduling[machine][2];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 16), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 5);
    previos = job;
    ct += pt + st;

    EXPECT_EQ(ct, 42);

    //Verificar os valores da função objetivo
    EXPECT_EQ(my_solution->makeSpan, 80);
    double TEC;
    TEC = Instance::v_consumption_factor[mode_op]*Instance::v_machine_potency[1]*Instance::rate_off_peak*72/60;
    TEC += Instance::v_consumption_factor[mode_op]*Instance::v_machine_potency[2]*Instance::rate_off_peak*35/60;
    EXPECT_LE(abs(my_solution->TEC - TEC), EPS);

    //Verificar o instante de início de cada tarefa
    EXPECT_EQ(my_solution->H1[3], 0);
    EXPECT_EQ(my_solution->H1[2], 0);

    delete my_solution;
}

TEST_F(TestCommon, GreedyInitialSolutionTEC3) {

    Instance::ReadMarceloInstance("../../../Instances/SMALL/6_2_1439_3_S_1-9.dat");
    Instance::seed = 10683;

    Solution * my_solution = new Solution();
    my_solution->GenerateGreedySolutionTEC3();

    //Verificar a sequência das tarefas
    EXPECT_EQ(my_solution->scheduling[1][0], 5);
    EXPECT_EQ(my_solution->scheduling[1][1], 3);
    EXPECT_EQ(my_solution->scheduling[1][2], 4);
    EXPECT_EQ(my_solution->scheduling[1][3], 1);
    EXPECT_EQ(my_solution->scheduling[2][0], 2);
    EXPECT_EQ(my_solution->scheduling[2][1], 6);


    //Verificar o modo de operação das tarefas
    unsigned mode_op = 3;
    for (auto it = my_solution->job_mode_op.begin()+1; it != my_solution->job_mode_op.end(); ++it) {
        EXPECT_EQ(*it, mode_op);
    }

    //Verificar o tempo de processamento de cada tarefa
    unsigned job, machine, previos;
    int pt, st, ct;

    machine = 1;
    previos = 0;
    ct = 0;

    job = my_solution->scheduling[machine][0];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 20), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 0);
    previos = job;
    ct += pt + st;


    job = my_solution->scheduling[machine][1];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 53), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 5);
    previos = job;
    ct += pt + st;

    job = my_solution->scheduling[machine][2];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 3), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 3);
    previos = job;
    ct += pt + st;

    job = my_solution->scheduling[machine][3];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 34), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 2);
    ct += pt + st;

    EXPECT_EQ(ct, 120);

    machine = 2;
    previos = 0;
    ct = 0;

    job = my_solution->scheduling[machine][0];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 10), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 0);
    previos = job;
    ct += pt + st;

    job = my_solution->scheduling[machine][1];
    pt = ceil(double(Instance::m_processing_time[machine][job])/
              double(Instance::v_speed_factor[my_solution->job_mode_op[job]]));
    EXPECT_LE(abs(pt - 18), EPS);
    st = Instance::m_setup_time[machine][previos][job];
    EXPECT_EQ(st, 2);
    previos = job;
    ct += pt + st;

    EXPECT_EQ(ct, 30);

    //Verificar os valores da função objetivo
    EXPECT_EQ(my_solution->makeSpan, 120);
    double TEC;
    TEC = Instance::v_consumption_factor[mode_op]*Instance::v_machine_potency[1]*Instance::rate_off_peak*110/60;
    TEC += Instance::v_consumption_factor[mode_op]*Instance::v_machine_potency[2]*Instance::rate_off_peak*28/60;
    EXPECT_LE(abs(my_solution->TEC - TEC), EPS);

    //Verificar o instante de início de cada tarefa
    EXPECT_EQ(my_solution->H1[5], 0);
    EXPECT_EQ(my_solution->H1[2], 0);

    delete my_solution;
}

TEST_F(TestCommon, HorizonTwoDays) {


    Instance::ReadMarceloInstance("../../../Instances/LARGE/100_10_1439_5_S_1-124.dat");
    Instance::seed = 10683;

    Solution * my_solution = new Solution();
    //Essa solução gulosa precisa de um horizonte maior
    my_solution->GenerateGreedySolutionTEC3();

    //Verificar apenas as máquinas nas quais as tarefas com horário de término
    //maior que o início do horário de pico: 3 e 6

    //Verificar a sequência das tarefas
    EXPECT_EQ(my_solution->scheduling[1][0], 57);
    EXPECT_EQ(my_solution->scheduling[1][1], 52);
    EXPECT_EQ(my_solution->scheduling[1][2], 48);
    EXPECT_EQ(my_solution->scheduling[1][3], 64);
    EXPECT_EQ(my_solution->scheduling[1][4], 2);
    EXPECT_EQ(my_solution->scheduling[1][5], 88);
    EXPECT_EQ(my_solution->scheduling[1][6], 62);
    EXPECT_EQ(my_solution->scheduling[1][7], 67);
    EXPECT_EQ(my_solution->scheduling[1][8], 96);
    EXPECT_EQ(my_solution->scheduling[1][9], 58);
    EXPECT_EQ(my_solution->scheduling[1][10], 20);
    EXPECT_EQ(my_solution->scheduling[1][11], 18);
    EXPECT_EQ(my_solution->scheduling[1][12], 51);
    EXPECT_EQ(my_solution->scheduling[1][13], 50);
    EXPECT_EQ(my_solution->scheduling[1][14], 75);
    EXPECT_EQ(my_solution->scheduling[1][15], 10);

    EXPECT_EQ(my_solution->scheduling[3][0], 79);
    EXPECT_EQ(my_solution->scheduling[3][1], 87);
    EXPECT_EQ(my_solution->scheduling[3][2], 60);
    EXPECT_EQ(my_solution->scheduling[3][3], 3);
    EXPECT_EQ(my_solution->scheduling[3][4], 7);
    EXPECT_EQ(my_solution->scheduling[3][5], 29);
    EXPECT_EQ(my_solution->scheduling[3][6], 72);
    EXPECT_EQ(my_solution->scheduling[3][7], 22);
    EXPECT_EQ(my_solution->scheduling[3][8], 1);
    EXPECT_EQ(my_solution->scheduling[3][9], 95);
    EXPECT_EQ(my_solution->scheduling[3][10], 46);
    EXPECT_EQ(my_solution->scheduling[3][11], 12);
    EXPECT_EQ(my_solution->scheduling[3][12], 63);
    EXPECT_EQ(my_solution->scheduling[3][13], 100);
    EXPECT_EQ(my_solution->scheduling[3][14], 68);
    EXPECT_EQ(my_solution->scheduling[3][15], 5);
    EXPECT_EQ(my_solution->scheduling[3][16], 89);
    EXPECT_EQ(my_solution->scheduling[3][17], 33);

    //Verificar o modo de operação das tarefas
    unsigned mode_op = 5;
    for (auto it = my_solution->job_mode_op.begin()+1; it != my_solution->job_mode_op.end(); ++it) {
        EXPECT_EQ(*it, mode_op);
    }

    //h
    unsigned previos, h;
    for (unsigned i=1; i<=Instance::num_machine;i++) {
        previos = h = 0;
        for (auto job = my_solution->scheduling[i].begin(); job != my_solution->scheduling[i].end(); ++job) {
            EXPECT_EQ(my_solution->job_start_time1[*job], h);
            h += Instance::m_setup_time[i][previos][*job];
            EXPECT_EQ(my_solution->H1[*job], h);
            h += ceil(double(Instance::m_processing_time[i][*job])/
                    double(Instance::v_speed_factor[mode_op]));
            EXPECT_EQ(my_solution->job_end_time1[*job], h);
            previos = *job;
        }
    }

    delete my_solution;
}
