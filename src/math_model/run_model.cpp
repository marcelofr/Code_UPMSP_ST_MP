#include "run_model.h"


void RunWeightedMathModel(double max_time, double alpha, Solution * my_solution)
{

    //Instance::ReadJulioInstance(instance_file);
    //Instance::PrintInstance1();

    double d_num;

    try {
        // Criar ambiente
        GRBEnv env = GRBEnv();

        // Criar modelo
        GRBModel model = GRBModel(env);

        Model *Mymodel = new Model();
        Mymodel->Create(&model, max_time);
        Mymodel->AddVar();
        Mymodel->SetObjective(alpha);
        Mymodel->SetConstraint();
        Mymodel->SetInitialSolutionToMathModel(my_solution);
        Mymodel->Optimize();

        Mymodel->GetSolutionFromModel(my_solution);

        if(model.get(GRB_IntAttr_Status) == GRB_OPTIMAL){
            my_solution->is_optimal = true;
        }
        else{
            my_solution->is_optimal = false;
        }

        string var;

        var = "CMax";
        d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
        my_solution->makeSpan = d_num;
        #ifdef DEBUG
        cout << endl << var << ": " << d_num << endl;
        #endif

        var = "PecOn";
        d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
        my_solution->TEC = d_num;
        #ifdef DEBUG
        cout << var << ": " << model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;
        #endif

        var = "PecOff";
        d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
        my_solution->TEC += d_num;
        #ifdef DEBUG
        cout << var << ": " << model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;
        //Mymodel->get

        Mymodel->PrintVars();

        my_solution->Print();
        #endif


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}

void RunEpsilonMathModel(double max_time, unsigned makespan, double TEP, Solution *my_solution)
{
    //Instance::ReadJulioInstance(instance_file);
    //Instance::PrintInstance1();
    my_solution->Print();

    double d_num;

    try {
        // Criar ambiente
        GRBEnv env = GRBEnv();

        // Criar modelo
        GRBModel model = GRBModel(env);

        Model *Mymodel = new Model();
        Mymodel->Create(&model, max_time);
        Mymodel->AddVar();
        if(TEP == 0){
            Mymodel->SetObjectiveTEP();
            Mymodel->SetConstraintWithMakespan(makespan);
        }
        else if (makespan == 0){
            Mymodel->SetObjectiveMakespan();
            Mymodel->SetConstraintWithTEP(TEP);
        }
        Mymodel->SetInitialSolutionToMathModel(my_solution);
        Mymodel->Optimize();

        Mymodel->GetSolutionFromModel(my_solution);

        if(model.get(GRB_IntAttr_Status) == GRB_OPTIMAL){
            my_solution->is_optimal = true;
        }
        else{
            my_solution->is_optimal = false;
        }

        //Verifica o status do modelo
        int status = model.get(GRB_IntAttr_Status);

        //Imprime o status do modelo
        if (status == GRB_UNBOUNDED)
        {
          cout << "O modelo nao pode ser resolvido porque e ilimitado" << endl;
          return;
        }
        if (status == GRB_OPTIMAL)
        {
          cout << "Solucao otima encontrada!" << endl;
          //Acessa e imprime o valor da funcao objetivo
          cout << "O valor da solucao otima e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

          string var;

          var = "CMax";
          d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
          my_solution->makeSpan = d_num;
          cout << endl << var << ": " << d_num << endl;

          var = "PecOn";
          d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
          my_solution->TEC = d_num;
          cout << var << ": " << model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;

          var = "PecOff";
          d_num = model.getVarByName(var).get(GRB_DoubleAttr_X);
          my_solution->TEC += d_num;
          cout << var << ": " << model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;
          //Mymodel->get

          Mymodel->PrintVars();

        }
        if (status == GRB_INFEASIBLE)
        {
          cout << "O modelo nao pode ser resolvido porque e inviavel" << endl;
          return;
        }


    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}
