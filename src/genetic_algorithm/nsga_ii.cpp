#include "nsga_ii.h"

void nsga_ii(unsigned time_limit){
    vector<GASolution> P;

    GenerateInitialPopulation(P);

    vector<GASolution> Q, R;
    vector<vector<GASolution>> F;

    Timer *t1 = new Timer();

    unsigned i;

    while (t1->getElapsedTimeInMilliSec() < time_limit) {

        //R_t <- P_t U Q_t
        R.resize(P.size() + Q.size());
        move(P.begin(), P.end(), R.begin());
        move(Q.begin(), Q.end(), R.begin()+P.size());

        //Criar as frentes F[1], F[2], ...
        FastNonDominatedSort(F, R);

        //P_{t+1} <- empty
        P.clear();

        i = 0;

        while (P.size() + F[i].size() < POPULATION_SIZE) {

            //Calcular crowding distance para a frente i
            ComputeCrowdingDistance(F[i]);

            //P_{t+1} <- P_{t+1} U F[i]
            //P.resize(P.size() + F[i].size());
            move(F[i].begin(), F[i].end(), back_inserter(P));

            i++;
        }

        //Calcular crowding distance para a frente i
        ComputeCrowdingDistance(F[i]);

        //Ordenar a frente i, de acordo com a crowding distance
        Sort(F[i]);

        //P_{t+1} <- P_{t+1} U F[j][1:(N - |P|)]
        //P.resize(POPULATION_SIZE);
        move(F[i].begin(), F[i].begin()+POPULATION_SIZE-P.size(), back_inserter(P));

        //Criar um população Q_{t+1} de tamanho N
        Crossover(P, Q);
        Mutation(P, Q);
        Selection(P, Q);

        //Registar a contagem do tempo
        t1->stop();

    }

    //PrintPopulation(population);
    PrintPopulation(Q);

    t1->printElapsedTimeInMilliSec();

}

/*
 * Método para formar cada frente F[i] a partir da população R
 */
void FastNonDominatedSort(vector<vector<GASolution> > &F, vector<GASolution> P)
{

    //Conjunto auxiliar
    vector<GASolution> Q;

    Q.clear();

    //Gerar F[1]
    for(auto p = P.begin(); p != P.end(); ++p){
        p->set_solution_dominated.clear();
        p->n = 0;
        for(auto q = P.begin(); q != P.end(); ++q){
            //Se p domina q
            if(*p < *q){
                p->set_solution_dominated.push_back(&*q);
            }
            else if (*q < *p){
                p->n++;
            }
        }
        if((*p).n == 0){
            Q.push_back(*p);
            //F[0].push_back(*p);
        }
    }

    F.push_back(Q);

    unsigned i = 0;
    while (Q.size() > 0) {
        Q.clear();
        for(auto p = F[i].begin(); p != F[i].end(); ++p){
            for(auto q = p->set_solution_dominated.begin(); q != p->set_solution_dominated.end(); ++q){
                (*q)->n--;
                if((*q)->n == 0){
                    Q.push_back(*(*q));
                }
            }
        }
        i++;
        if(Q.size() > 0){
            F.push_back(Q);
        }
    }
}

/*
 * Calcular crowding distance das soluções de F[i]
 */
void ComputeCrowdingDistance(vector<GASolution> &F_i)
{
    for(auto p = F_i.begin(); p != F_i.end(); ++p){
        p->crowding_distance = 0;
    }
    //Ordenar makespan
    SortByMakespan(F_i);

    F_i.front().crowding_distance = INT_MAX;
    F_i.back().crowding_distance = INT_MAX;

    unsigned f_max, f_min;

    f_max = F_i.front().makeSpan;
    f_min = F_i.back().makeSpan;

    for(unsigned i = 1; i < F_i.size()-1; i++){
        F_i[i].crowding_distance += (F_i[i].crowding_distance -
                                      F_i[i].crowding_distance)/(f_max-f_min);
    }

}

//TODO
bool CompareCrowdingDistance(GASolution & l, GASolution & r) //(2)
{
    return l.crowding_distance < r.crowding_distance;
}

/*
 * Ordenar as soluções da frente F[i] de acordo com a crowding distance
 */
void Sort(vector<GASolution> &F_i)
{
    sort(F_i.begin(), F_i.end(), CompareCrowdingDistance);
}
