#ifndef TIMER_H
#define TIMER_H


#include <stddef.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <iostream>

using namespace std;

class Timer{
    public:
        inline Timer();
        inline void start();
        inline void stop();
        inline double getElapsedTimeInMilliSec();
        inline void printElapsedTimeInMilliSec();
    private:
        double t1, t2;
};

Timer::Timer(){
    this->t1 = this->t2 = 0;
}

/*
 * Registra o tempo inicial de relogio
 */
void Timer::start(){
    this->t1 = clock();
}

/*
 * Registra o tempo final de relogio
 */
void Timer::stop(){
    this->t2 = clock();
}


/*
 * Retorna o tempo em milisecundos
 */
double Timer::getElapsedTimeInMilliSec(){
    double elapsedTime;
    elapsedTime = ((this->t2 - this->t1)/CLOCKS_PER_SEC)*1000;
    return elapsedTime;
}

/*
 * Imprime o tempo que passou em ms
 */
void Timer::printElapsedTimeInMilliSec(){
    cout << getElapsedTimeInMilliSec() << " ms" << endl;
}

#endif // TIMER_H
