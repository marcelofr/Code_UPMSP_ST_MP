#include "set_solution.h"

bool CompareMakespan(Solution & l, Solution & r) //(2)
{
    if(l.makeSpan < r.makeSpan){
        return true;
    }
    else if (l.makeSpan > r.makeSpan){
        return false;
    }
    else{
        if(l.TEC < r.TEC){
            return true;
        }
        else{
            return false;
        }
    }

}

bool CompareTEC(Solution & l, Solution & r) //(2)
{
    if(l.TEC < r.TEC){
        return true;
    }
    else if (l.TEC > r.TEC){
        return false;
    }
    else{
        if(l.makeSpan < r.makeSpan){
            return true;
        }
        else{
            return false;
        }
    }

}

void SortByMakespan(vector<Solution> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareMakespan);
}

void SortByTEC(vector<Solution> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareTEC);
}
