#include "run_model.h"


void RunMathModel(unsigned max_time, double alpha, Solution * my_solution)
{

    //Instance::ReadJulioInstance(instance_file);
    Instance::PrintInstance1();

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
        //Mymodel->SetInitialSolutionToMathModel(my_solution);
        Mymodel->Optimize();

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

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}
