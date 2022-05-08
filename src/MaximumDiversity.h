#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <deque>
#include <queue>
#include <list>

#include "MDPSet.h"
#include "Solution.h"
#include "localSearch.h"
#include "menuUtility.h"

namespace MDP {

/**
 * @brief 
 * @abstract
 */
class MaximumDiversity {
  public:
    ~MaximumDiversity() {}

    /**
     * @brief Solve the Maximum Diversity Problem with the given number of nodes
     * 
     * @param numberOfNodes The number of nodes
     * @param originNode The origin node name
     * @return Solution The solution path
     */
    Solution Solve(unsigned numberOfVectors, unsigned numberOfComponents, localSearch* lSearch, unsigned solutionSize);

    /**
     * @brief Solve the Maximum Diversity Problem with the MDPSet of the file given
     * 
     * @param path The filepath
     * @param originNode The origin node name
     * @return Solution The solution path
     */
    Solution Solve(std::string path, localSearch* lSearch, unsigned solutionSize);

    /**
     * @brief Solve the Maximum Diversity Problem with the MDPSet given
     * 
     * @param mdpSet The MDPSet
     * @param originNode The origin node name
     * @return Solution The solution path
     */
    Solution Solve(MDPSet &mdpSet, localSearch* lSearch, unsigned solutionSize);
    int lastDiversity() { return lastDiversity_; }
  protected:

    /**
     * @brief Particular problem solving to be implemented in the specific class
     * 
     * @param mdpSet The MDPSet to compute
     * @param originNode The origin node to apply the algorithm
     * @return Solution The solution path
     */
    virtual Solution solveProblem(MDPSet &mdpSet, localSearch* lSearch, unsigned solutionSize) = 0;
  protected:
    float lastDiversity_;
};

}