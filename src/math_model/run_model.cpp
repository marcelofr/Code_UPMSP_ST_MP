#include "run_model.h"


void RunMathModel(string instance_file, unsigned max_time, double alpha)
{

    Instance::ReadJulioInstance(instance_file);
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
        Mymodel->Optimize();

        string var = "CMax";
        cout << "CMax: " << model.getVarByName(var).get(GRB_DoubleAttr_X);
        //Mymodel->get

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}
