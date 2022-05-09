#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <set>
#include <queue>

#include "MaximumDiversity.h"

namespace MDP {

/**
 * @brief Solves the MDP problem with a greedy approach
 * 
 */
class GreedyMaximumDiversity : public MaximumDiversity {
  public:
    GreedyMaximumDiversity() {}
    
    /**
     * @brief Solve the problem with the adjacency matrix and number of vehicles given
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @return std::vector<std::vector<unsigned> > The solution paths
     */
    Solution greedyAlgorithm(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize, Solution solution);
  protected:
    /**
     * @brief Solve the problem with the adjacency matrix and number of vehicles given
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @return std::vector<std::vector<unsigned> > The solution paths
     */
    Solution solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize);
};

}