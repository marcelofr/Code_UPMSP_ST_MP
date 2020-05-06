#include "run_model.h"


void RunMathModel(unsigned max_time, double alpha, Solution * my_solution)
{

    //Instance::ReadJulioInstance(instance_file);
    Instance::PrintInstance1();

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

        string var = "CMax";
        cout << endl;
        cout << var << ": " <<  model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;
        var = "PecOn";
        cout << var << ": " << model.getVarByName(var).get(GRB_DoubleAttr_X) << endl;
        var = "PecOff";
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
