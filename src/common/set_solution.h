#ifndef SET_SOLUTION_H
#define SET_SOLUTION_H

#include "genetic_algorithm/ga.h"

template<class T>
class SetSolution{
public:
    SetSolution(){
    }

    /*SetSolution(const SetSolution<T> &set_solution){
        //set_solution.set_solution
        //move(this->set_solution.begin(), this->set_solution.end(), set_solution.set_solution.begin());
        for(auto it:this->set_solution){
            set_solution.set_solution.push_back(it);
        }
    }*/

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
    NDSetSolution() = default;
    NDSetSolution(const NDSetSolution<T> &nd_set_solution):SetSolution<T>(nd_set_solution){};
    //NDSetSolution(const NDSetSolution<T> &nd_set_solution){};
    ~NDSetSolution(){};

    /*
     * Método para adicionar uma nova solução ao conjunto não-dominado, caso possíveis:
     * 1 - A nova solução é dominada então ela não será adicionada
     * 2 - A nova solução é não-dominada então ela será adicionada
     * 3 - A solução domina então ela será adicionada e as outras serão removidas
     * Se conseguiu adicionar my_solution, então retorna verdadeiro
     */
    bool AddSolution(T my_solution, unsigned add_weak=0)
    {


        //caso 1
        //bool is_dominated = false;
        for(auto it_sol : this->set_solution){
            //Se my_solution é dominada por alguma solução do conjunto
            if(*it_sol < *my_solution || (*it_sol == *my_solution && !add_weak)){
                return false;
            }
        }

        //Caso 3
        for(auto it_sol = this->set_solution.begin(); it_sol != this->set_solution.end();){
            //Se my_solution domina alguma solução do conjunto
            if(*my_solution < **it_sol){
                //Remover essa solução do conjunto
                delete *it_sol;
                it_sol = this->set_solution.erase(it_sol);
            }
            else{
                ++it_sol;
            }
        }
        //Caso 2 e 3
        T new_solution = my_solution->Create();
        *new_solution = *my_solution;
        this->set_solution.push_back(new_solution);
        return true;
    }

    /*
     * Método para gerar um conjunto inicial de soluções gulosas
     */
    void ConstrutiveGreedy(){

        T my_solution;
        my_solution = nullptr;

        /*Gerar uma solução gulosa considerando o objetivo do makespan*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionMakespan();
        this->set_solution.push_back(my_solution);


        /*Gerar uma solução gulosa considerando o objetivo do TEC*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionTEC3();
        this->set_solution.push_back(my_solution);
    }

    /*
     * Método para gerar um conjunto inicial de soluções aleatórias
     */
    void ConstrutiveRandom(unsigned set_size)
    {

        T my_solution;
        my_solution = nullptr;

        /*Gerar os indivíduos aleatoriamente*/
        for (unsigned i = 0; i < set_size; ++i) {

            my_solution = my_solution->Create();
            my_solution->RandomInitialSolution();
            this->set_solution.push_back(my_solution);

        }
    }

    /*
     * Método para gerar um conjunto inicial de soluções parcialmente gulosa
     * op=0 para criar uma população, op=1 para criar um conjunto não-dominado
     */
    void ContrutiveGRASP(double alpha, unsigned set_size, unsigned op=0){

        T my_solution;
        my_solution = nullptr;

        for (unsigned i = 0; i < set_size/2; ++i) {

            /*Gerar uma solução parcialmente gulosa considerando o objetivo do makespan*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionMakespan(alpha);

            if(op==0){
                this->set_solution.push_back(my_solution);
            }
            else if(op==1){
                this->AddSolution(my_solution);
            }


            /*Gerar uma solução parcialmente gulosa considerando o objetivo TEC*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionTEC(alpha);
            if(op==0){
                this->set_solution.push_back(my_solution);
            }
            else if(op==1){
                this->AddSolution(my_solution);
            }

        }

    }

    /*
     * Método para gerar um conjunto inicial de soluções
     * 2 gulosas e o restante aleatória
     */
    void ConstrutiveGreedyAndRandom(unsigned set_size)
    {

        T my_solution;

        my_solution = nullptr;

        /*Gerar uma solução gulosa considerando o objetivo do makespan*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionMakespan();
        this->set_solution.push_back(my_solution);

        /*Gerar uma solução gulosa considerando o objetivo do TEC*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionTEC3();
        this->set_solution.push_back(my_solution);

        /*Gerar o restante dos indivíduos aleatoriamente*/
        for (unsigned i = 2; i < set_size; ++i) {

            my_solution = my_solution->Create();
            my_solution->RandomInitialSolution();
            this->set_solution.push_back(my_solution);

        }
    }

    /*
     * Método para gerar um conjunto inicial de soluções
     * 1/3 parcialmente gulosa makespan
     * 1/3 parcialmente gulosa tec
     * 1/3 aleatória
     */
    void ContrutiveGRASPRandon(double alpha, unsigned set_size){

        T my_solution;

        my_solution = nullptr;


        for (unsigned i = 0; i < set_size/3; ++i) {

            /*Gerar uma solução gulosa considerando o objetivo do makespan*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionMakespan(alpha);
            //AddSolution(my_solution, non_dominated_set.set_solution);
            this->set_solution.push_back(my_solution);


            /*Gerar uma solução gulosa considerando o objetivo do makespan*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionTEC(alpha);
            //AddSolution(my_solution, non_dominated_set.set_solution);
            this->set_solution.push_back(my_solution);

            /*Gerar uma solução aleatoriamente*/
            my_solution = my_solution->Create();
            my_solution->RandomInitialSolution();
            this->set_solution.push_back(my_solution);

        }

    }

    /*
     * Método para gerar um conjunto inicial de soluções
     * 1/4 parcialmente gulosa makespan
     * 1/4 parcialmente gulosa tec
     * 1 gulosa makespan
     * 1 gulosa tec
     */
    void ContrutiveGRASPGreedy(double alpha, unsigned set_size){

        T my_solution;

        my_solution = nullptr;

        /*Gerar uma solução gulosa*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionMakespan();
        this->set_solution.push_back(my_solution);


        /*Gerar uma solução gulosa*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionTEC3();
        this->set_solution.push_back(my_solution);

        for (unsigned i = 2; i < set_size; ++i) {

            /*Gerar uma solução gulosa considerando o objetivo do makespan*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionMakespan(alpha);
            //AddSolution(my_solution, non_dominated_set.set_solution);
            this->set_solution.push_back(my_solution);


            /*Gerar uma solução gulosa considerando o objetivo do makespan*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionTEC(alpha);
            this->set_solution.push_back(my_solution);
        }

    }


    /*
     * Método para gerar um conjunto inicial de soluções
     * 1/5 parcialmente gulosa makespan
     * 1/5 parcialmente gulosa tec
     * 1/5 gulosa makespan
     * 1/5 gulosa tec
     * 1/5 aleatoria
     */
    void ContrutiveGRASPGreedyRandon(double alpha, unsigned tam_population){

        T my_solution;

        my_solution = nullptr;

        /*Gerar uma solução gulosa*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionMakespan();
        this->set_solution.push_back(my_solution);


        /*Gerar uma solução gulosa*/
        my_solution = my_solution->Create();
        my_solution->GenerateGreedySolutionTEC3();
        this->set_solution.push_back(my_solution);

        for (unsigned i = 0; i < tam_population/4; ++i) {

            /*Gerar uma solução parcialmente gulosa considerando o objetivo do makespan*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionMakespan(alpha);
            //AddSolution(my_solution, non_dominated_set.set_solution);
            this->set_solution.push_back(my_solution);


            /*Gerar uma solução parcialmente gulosa considerando o objetivo do makespan*/
            my_solution = my_solution->Create();
            my_solution->GenerateGRASPSolutionTEC(alpha);
            //AddSolution(my_solution, non_dominated_set.set_solution);
            this->set_solution.push_back(my_solution);

            /*Gerar uma solução aleatoriamente*/
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
