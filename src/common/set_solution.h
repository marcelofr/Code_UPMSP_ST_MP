#ifndef SET_SOLUTION_H
#define SET_SOLUTION_H

#include "genetic_algorithm/ga.h"

template<class T>
class SetSolution{
public:
    SetSolution(){
    }

    SetSolution(const vector<T> &set_solution){
        //this->set_solution = set_solution;
        move(this->set_solution_dominated.begin(), this->set_solution_dominated.end(), set_solution.begin());
    }

    ~SetSolution() {
        this->set_solution.clear();
    }

    void PrintSetSolution(){
        for(auto it = this->set_solution.begin(); it != this->set_solution.end(); ++it){
            (*it)->Print();
        }
    }
//    protected:
    vector<T> set_solution;


    SetSolution& operator=(const SetSolution &s){

        if( &s != this ) {

            SetSolution* ptr_object = NULL;
            if(&s != ptr_object){

                this->set_solution.clear();
                copy(s.set_solution.begin(), s.set_solution.end(), this->set_solution.begin());

            }

        }
        return *this;
    }

};

template<class T>
class NDSetSolution : public SetSolution<T>{
public:
    NDSetSolution(){};

    /*
     * Método para adicionar uma nova solução ao conjunto não-dominado, caso possíveis:
     * 1 - A nova solução é dominada então ela não será adicionada
     * 2 - A nova solução é não-dominada então ela será adicionada
     * 3 - A solução domina então ela será adicionada e as outras serão removidas
     * Se conseguiu adicionar my_solution, então retorna verdadeiro
     */
    bool AddSolution(T my_solution)
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

    void GenerateGreedyInitialSolution(){

        T my_solution;

        /*Gerar uma conjunto inicial*/

        /*Gerar uma solução gulosa considerando o objetivo do makespan*/
        my_solution = my_solution->Create();
        my_solution->GreedyInitialSolutionMakespan();
        //AddSolution(my_solution, non_dominated_set.set_solution);
        this->set_solution.push_back(my_solution);


        /*Gerar uma solução gulosa considerando o objetivo do TEC*/
        my_solution = my_solution->Create();
        my_solution->GreedyInitialSolutionTEC3();
        //AddSolution(my_solution, non_dominated_set);
        //non_dominated_set.AddSolution(my_solution);
        this->set_solution.push_back(my_solution);
    }

    /*
     * Método para gerar a população inicial com tamanho POPULATION_SIZE
     */
    void GenerateGreedyRandomInitialSolution(unsigned tam_population)
    {

        T my_solution;

        /*Gerar uma solução gulosa considerando o objetivo do makespan*/
        my_solution = my_solution->Create();
        my_solution->GreedyInitialSolutionMakespan();
        this->set_solution.push_back(my_solution);

        /*Gerar uma solução gulosa considerando o objetivo do TEC*/
        my_solution = my_solution->Create();
        my_solution->GreedyInitialSolutionTEC3();
        this->set_solution.push_back(my_solution);

        /*Gerar o restante dos indivíduos aleatoriamente*/
        for (unsigned i = 0; i < tam_population-2; ++i) {

            my_solution = my_solution->Create();
            my_solution->RandomInitialSolution();
            this->set_solution.push_back(my_solution);

        }
    }

    NDSetSolution& operator=(NDSetSolution &s){

        return (NDSetSolution&)(SetSolution<T>::operator =(s));
    }
};

void SortByMakespan(vector<Solution*> &set_solution);
void SortByTEC(vector<Solution*> &set_solution);

void SortByMakespan(vector<GASolution*> &set_solution);
void SortByTEC(vector<GASolution*> &set_solution);

#endif // SET_SOLUTION_H
