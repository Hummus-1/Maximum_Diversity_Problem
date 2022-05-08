#pragma once

#include <iostream>
#include <math.h>

#include "MDPSet.h"
#include "Solution.h"

namespace MDP {

class localSearch {
  public:
    localSearch() {}
    ~localSearch() {}

    int lastDifference() { return lastDifference_; }

    void enhanceSolution(MDPSet& mdpSet, Solution& solution);
    virtual void nextOptimum(MDPSet& mdpSet, Solution& solution) = 0;
  protected:
    float lastDifference_{0};
};

class Interchange : public localSearch {
  public:
    ~Interchange() {}

    void nextOptimum(MDPSet& mdpSet, Solution& solution);
};

}