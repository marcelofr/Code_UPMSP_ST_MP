#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::Create(GRBModel *model, double MaxTime)
{
    this->model = model;

    model->set(GRB_IntParam_OutputFlag, 1);

    model->set(GRB_IntParam_Threads, 4);
    //model->set(GRB_IntParam_LazyConstraints, 1);
    //model->set(GRB_IntParam_Presolve, 0);
    model->set(GRB_DoubleParam_TimeLimit, MaxTime);
    //model->set(GRB_DoubleParam_TimeLimit, 300.0);
    model->set(GRB_DoubleParam_MIPGap, EPS);

}

void Model::AddVar()
{
    string str;

    //X_ijhl = 1 Variável binária que assume valor 1 se a tarefa j for alocada à máquina i
    //no instante de tempo h e no modo de operação l, e valor 0, caso contrário
    this->X = vector<vector<vector<vector<GRBVar>>>>
                (Instance::num_machine+1, vector<vector<vector<GRBVar>>>
                    (Instance::num_jobs+1, vector<vector<GRBVar>>
                        (Instance::num_planning_horizon+1, vector<GRBVar>
                            (Instance::num_mode_op+1))));

    for (size_t i = 1; i <= Instance::num_machine; i++) {
        for (size_t j = 1; j <= Instance::num_jobs; j++) {
            for (size_t h = 0; h <= Instance::num_planning_horizon; h++) {
                for (size_t l = 1; l <= Instance::num_mode_op; l++) {
                    str = "X[" + itos(i) + "][" + itos(j) + "][" + itos(h) + "][" + itos(l) + "]";
                    X[i][j][h][l] = model->addVar(0, 1, 0, GRB_BINARY, str);
                }
            }
        }
    }

    //Variável para o makespan
    CMax = model->addVar(0, GRB_INFINITY, 0, GRB_INTEGER, "CMax");

    //Variável para o PEC_on
    PecOn = model->addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "PecOn");

    //Variável para o PEC_off
    PecOff = model->addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "PecOff");

    // Integrar novas variáveis ao modelo
    model->update();
}

void Model::SetInitialSolutionToMathModel(Solution *MySolutionLS)
{
    string str;
    unsigned size;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        size = MySolutionLS->scheduling[i].size();
        //Demais tarefas
        for (auto j = MySolutionLS->scheduling[i].begin(); j != MySolutionLS->scheduling[i].end(); ++j) {
            //str = "x[" + itos(i) + "][" + itos(MySolutionLS.scheduling[i][j-1]) + "][" + itos(MySolutionLS.scheduling[i][j]) + "]";
            //model->getVarByName(str).set(GRB_DoubleAttr_Start, 1);
            this->X[i][*j][MySolutionLS->H1[*j]][MySolutionLS->job_mode_op[*j]].set(GRB_DoubleAttr_Start, 1);
        }
    }
}

void Model::SetObjective(double alpha)
{
    // Set objective
    //(2)
    //double alpha = 0.5;
    GRBLinExpr aux = (CMax/Instance::num_planning_horizon)*alpha +((PecOn+PecOff)/Instance::max_cost)*(1-alpha);
    //GRBLinExpr aux = CMax*alpha + (PecOn+PecOff)*(1-alpha);
    //GRBLinExpr aux = (CMax);
    //GRBLinExpr aux = (PecOn+PecOff);
    //GRBLinExpr aux = PecOn;
    //GRBLinExpr aux = PecOff;
    model->setObjective(aux, GRB_MINIMIZE);
}

void Model::SetConstraint()
{
    GRBLinExpr aux1, aux2, aux3, aux4, resultado;
    double aux0;
    string str;

    //(3)
    //Toda tarefa j deve ser processada apenas uma vez
    unsigned limit, pt_round;
    for (unsigned j = 1; j <= Instance::num_jobs; j++) {
        aux1 = 0;
        for (unsigned i = 1; i <= Instance::num_machine; i++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {
                pt_round = ceil(Instance::m_processing_time[i][j]/Instance::v_speed_factor[l]);
                limit = Instance::num_planning_horizon - pt_round;
                for (unsigned h = 0; h <= limit; h++) {
                    aux1 +=X[i][j][h][l];
                }
            }
        }
        str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::num_machine) +
                "), l(1:" + itos(Instance::num_mode_op) + "), h(0:" + itos(Instance::num_planning_horizon) + ")";
        model->addConstr(aux1, GRB_EQUAL, 1, str);
    }

    //(4)
    //
    unsigned limit2;
    int a, b;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned k = 1; k <= Instance::num_jobs; k++) {
                if(j != k ){
                    for (unsigned l = 1; l <= Instance::num_mode_op; l++) {
                        for (unsigned h = 0; h <= Instance::num_planning_horizon; h++) {
                            aux1 = 0;
                            pt_round = ceil(Instance::m_processing_time[i][j]/Instance::v_speed_factor[l]);
                            a = h + pt_round + Instance::m_setup_time[i][j][k] - 1;
                            b = Instance::num_planning_horizon;
                            limit2 = unsigned(min(a, b));
                            for (unsigned u = h; u <= limit2; u++) {
                                for (unsigned l1 = 1; l1 <= Instance::num_mode_op; l1++) {
                                    aux1 +=X[i][k][u][l1];
                                }
                            }
                            //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::numMachine) + "), l(1:" + itos(Instance::numModeOp) + "), h(0:" + itos(Instance::numPlanningHorizon) + ")";
                            model->addConstr(X[i][j][h][l] + aux1, GRB_LESS_EQUAL, 1, str);
                        }
                    }
                }
            }
        }
    }

    //(5)
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {
                for (unsigned h = 0; h <= Instance::num_planning_horizon; h++) {
                    //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 |
                    //i(1:" + itos(Instance::numMachine) + "), l(1:" + itos(Instance::numModeOp) + "),
                    //h(0:" + itos(Instance::numPlanningHorizon) + ")";
                    pt_round = ceil(Instance::m_processing_time[i][j]/Instance::v_speed_factor[l]);
                    model->addConstr(CMax, GRB_GREATER_EQUAL, X[i][j][h][l]*(h+pt_round), str);
                }
            }
        }
    }

    aux1 = 0;
    int c;


    //(6)
    resultado = 0;
    aux0 = 0;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {

                aux0 = Instance::v_consumption_factor[l]*
                        Instance::v_machine_potency[i]*
                        Instance::rate_on_peak/6;

                aux2 = 0;
                for (unsigned h = 0; h < Instance::peak_start; h++) {
                    pt_round = ceil(Instance::m_processing_time[i][j]/Instance::v_speed_factor[l]);
                    a = h + pt_round - 1;
                    b = Instance::peak_end;
                    c = min(a, b);
                    a = 0;
                    b = c - (Instance::peak_start - 1);
                    c = max(a, b);
                    aux2 += X[i][j][h][l]*c;

                    //cout << aux2 << endl;
                }

                aux3 = 0;
                for (unsigned h = Instance::peak_start; h <= Instance::peak_end; h++) {
                    pt_round = ceil(Instance::m_processing_time[i][j]/Instance::v_speed_factor[l]);
                    a = h + pt_round;
                    b = Instance::peak_end + 1;
                    c = min(a, b);
                    c -= h;
                    aux3 += X[i][j][h][l]*c;
                }

                resultado += (aux0)*(aux2+aux3);
            }
        }
    }

    //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::numMachine) + "),
    //l(1:" + itos(Instance::numModeOp) + "), h(0:" + itos(Instance::numPlanningHorizon) + ")";
    model->addConstr(PecOn, GRB_GREATER_EQUAL, resultado, str);


    //(7) PEC_off
    aux1 = 0;
    aux2 = 0;
    aux3 = 0;
    aux4 = 0;
    aux0 = 0;
    resultado = 0;
    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned l = 1; l <= Instance::num_mode_op; l++) {

                aux0 = Instance::v_consumption_factor[l]*
                        Instance::v_machine_potency[i]*
                        Instance::rate_off_peak/6;

                aux2 = 0;
                for (unsigned h = 0; h < Instance::peak_start; h++) {
                    a = h+ceil(Instance::m_processing_time[i][j]/Instance::v_speed_factor[l]);
                    b = Instance::peak_start;
                    c = min(a, b);
                    c -= h;
                    a = 0;
                    b = h+ceil(Instance::m_processing_time[i][j]/
                                   Instance::v_speed_factor[l])-Instance::peak_end-1;
                    c += max(a, b);
                    aux2 += X[i][j][h][l]*c;
                }

                aux3 = 0;
                for (unsigned h = Instance::peak_start; h <= Instance::peak_end; h++) {
                    a = 0;
                    b = h+ceil(Instance::m_processing_time[i][j]/
                                   Instance::v_speed_factor[l])-Instance::peak_end-1;
                    c = max(a, b);
                    aux3 += X[i][j][h][l]*c;
                }

                aux4 = 0;
                for (unsigned h = Instance::peak_end+1; h <= Instance::num_planning_horizon; h++) {
                    c = ceil(Instance::m_processing_time[i][j]/Instance::v_speed_factor[l]);
                    aux4 += X[i][j][h][l]*c;
                }

                resultado += aux0*(aux2+aux3+aux4);

            }
        }
    }
    //str = "Sum(X[i, " + itos(j) + ", l, h]) = 1 | i(1:" + itos(Instance::numMachine) + "),
    //l(1:" + itos(Instance::numModeOp) + "), h(0:" + itos(Instance::numPlanningHorizon) + ")";
    model->addConstr(PecOff, GRB_GREATER_EQUAL, resultado, str);

}

void Model::Optimize()
{
    model->optimize();

}

//void Exact::GetFinalSolutionFromModel(Solution &MySolution)
//{
//    size_t size;

//    try{

//        /*//Salvar solução final
//        FinalSolution = vector<vector<vector<double>>> (NumMachines, vector<vector<double>>(NumJobs, vector<double>(NumJobs)));
//        for (size_t i = 1; i < NumMachines; i++) {
//            for (size_t j = 0; j < NumJobs; j++) {
//                for (size_t k = 0; k < NumJobs; k++) {
//                    FinalSolution[i][j][k] = this->X[i][j][k].get(GRB_DoubleAttr_X);
//                }
//            }
//        }*/

//        //Definir o makespan
//        //MySolutionModel.MakeSpan = model.getObjective().getValue();

//        /*
//         * Pegar a primeira tarefa de cada máquina
//         */
//        for (size_t i = 1; i < NumMachines; i++) {
//            //Zerar o sequenciamento da máquina i
//            MySolution.scheduling[i].clear();
//            //Zerar o tempo de término na máquina i
//            MySolution.completionTimeMachine[i] = 0;
//            for (size_t k = 1; k < NumJobs; k++) {
//                if(this->FinalSolution[i][0][k] > 0.9){
//                    //Adicionar a tarefa na primeira posição da máquina i
//                    MySolution.scheduling[i].push_back(k);
//                    //So tem uma tarefa como primeira
//                    break;
//                }
//            }
//            //Pegar o tempo de término de todas as máquinas, no modelo matemático
//            /*var = "C[" + itos(i) + "]";
//            aux = model->getVarByName(var);
//            valor = int(aux.get(GRB_DoubleAttr_X));
//            MySolution.completionTimeMachine[i] = valor;*/

//        }

//        /*
//         * Pegar as demais tarefas de cada máquina
//         */
//        size_t prevjob = 0;
//        for (size_t i = 1; i < NumMachines; i++) {
//            size = MySolution.scheduling[i].size();
//            if(size){
//                prevjob = MySolution.scheduling[i][0];
//            }
//            for (size_t j = 1; j < NumJobs; j++) {
//                for (size_t k = 1; k < NumJobs; k++) {
//                    if(prevjob != k){
//                        if(this->FinalSolution[i][prevjob][k] > 0.9){
//                            //Adicionar a tarefa na primeira posição da máquina i
//                            MySolution.scheduling[i].push_back(k);
//                            prevjob = k;
//                            //So tem uma tarefa como primeira
//                            break;
//                        }
//                    }
//                }
//            }
//        }

//        MySolution.CalculateFull();
//    }
//    catch(GRBException e){
//        cout << e.getErrorCode();
//    }
//}

void Model::PrintVars()
{

    double valor;
    string str;

    cout << "=======  X  ==========" << endl << endl;

    for (unsigned i = 1; i <= Instance::num_machine; i++) {
        for (unsigned j = 1; j <= Instance::num_jobs; j++) {
            for (unsigned h = 0; h <= Instance::num_planning_horizon; h++) {
                for (unsigned l = 1; l <= Instance::num_mode_op; l++) {

                    str = "X[" + itos(i) + "][" + itos(j) + "][" + itos(h) + "][" + itos(l) + "]";
                    valor = model->getVarByName(str).get(GRB_DoubleAttr_X);

                    if(valor > 0)
                        cout << str << endl;

                }
            }
        }
    }
}
