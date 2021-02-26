#ifndef SET_SOLUTION_H
#define SET_SOLUTION_H

#include "genetic_algorithm/ga.h"

class SetSolution{
public:
    SetSolution(vector<Solution*> &set_solution){
        this->set_solution = set_solution;
    }
    SetSolution(){
    }
    virtual ~SetSolution() {}
    void PrintSetSolution(){
        for(auto it = this->set_solution.begin(); it != this->set_solution.end(); ++it){
            (*it)->Print();
        }
    }
//    protected:
    vector<Solution*> set_solution;

};

class NDSetSolution : public SetSolution{
public:
    NDSetSolution(){};

    /*
     * Método para adicionar uma nova solução ao conjunto não-dominado, caso possíveis:
     * 1 - A nova solução é dominada então ela não será adicionada
     * 2 - A nova solução é não-dominada então ela será adicionada
     * 3 - A solução domina então ela será adicionada e as outras serão removidas
     * Se conseguiu adicionar my_solution, então retorna verdadeiro
     */
    bool AddSolution(Solution *my_solution)
    {

        //caso 1
        //bool is_dominated = false;
        for(auto it_sol = this->set_solution.begin(); it_sol != this->set_solution.end(); ++it_sol){
            //Se my_solution é dominada por alguma solução do conjunto
            if(**it_sol < *my_solution || **it_sol == *my_solution){
                return false;
            }
        }

        //Caso 3
        for(auto it_sol = this->set_solution.begin(); it_sol != this->set_solution.end();){
            //Se my_solution domina alguma solução do conjunto
            if(*my_solution < **it_sol){
                //Remover essa solução do conjunto
                it_sol = this->set_solution.erase(it_sol);
            }
            else{
                ++it_sol;
            }
        }
        //Caso 2 e 3
        this->set_solution.push_back(my_solution);
        return true;
    }

    void GenInitialSet(){
        Solution * my_solution;

        /*Gerar uma conjunto inicial*/
        /*Gerar uma solução gulosa considerando o objetivo do makespan*/
        my_solution = new Solution();
        my_solution->GreedyInitialSolutionMakespan();
        //AddSolution(my_solution, non_dominated_set.set_solution);
        this->set_solution.push_back(my_solution);


        /*Gerar uma solução gulosa considerando o objetivo do TEC*/
        my_solution = new Solution();
        my_solution->GreedyInitialSolutionTEC3();
        //AddSolution(my_solution, non_dominated_set);
        //non_dominated_set.AddSolution(my_solution);
        this->set_solution.push_back(my_solution);
    }
};

void SortByMakespan(vector<Solution*> &set_solution);
void SortByTEC(vector<Solution*> &set_solution);

void SortByMakespan(vector<GASolution*> &set_solution);
void SortByTEC(vector<GASolution*> &set_solution);

#endif // SET_SOLUTION_H
