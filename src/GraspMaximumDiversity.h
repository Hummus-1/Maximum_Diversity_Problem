#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "GreedyMaximumDiversity.h"

namespace MDP {

/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class GraspMaximumDiversity : public MaximumDiversity {
  public:
    GraspMaximumDiversity() {}
    unsigned lastNumberOfIterations() { return numberOfIterations_; }
  protected:
    /**
     * @brief Solve the problem with the adjacency matrix and number of vehicles given
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @return std::vector<std::vector<unsigned> > The solution paths
     */
    Solution solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize);
  private:
    Solution farthestFromGravityCenter_;
    unsigned numberOfIterations_ = 0;
};

}