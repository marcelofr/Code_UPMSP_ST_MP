#ifndef LSSOLUTION_H
#define LSSOLUTION_H

#include "common/solution.h"
#include "common/set_solution.h"


class LSSolution : public Solution{
public:
    bool was_visited;
    vector<unsigned> removed_jobs;

    LSSolution():Solution(){
        was_visited = false;
    }

    LSSolution* Create(){
        return new LSSolution();
    }

    ~LSSolution(){
        removed_jobs.clear();
    }

    LSSolution& operator=(const LSSolution &s){
        Solution::operator=(s);

        this->was_visited = s.was_visited;
        return *this;
    }
    //bool operator <(const Solution& s) ;
    //bool operator ==(const Solution& s) ;

    void RemovingJob(unsigned machine, unsigned job_position){
        auto it = this->scheduling[machine].begin()+job_position;

        this->removed_jobs.push_back(*it);

        this->H1[*it] = 0;
        this->job_start_time1[*it] = 0;
        this->job_end_time1[*it] = 0;
        //this->job_mode_op[*it] = 0;

        this->scheduling[machine].erase(it);

    }
};

#endif // LSSOLUTION_H
