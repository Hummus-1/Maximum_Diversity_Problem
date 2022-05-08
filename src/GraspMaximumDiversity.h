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
  protected:
    /**
     * @brief Solve the problem with the adjacency matrix and number of vehicles given
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @return std::vector<std::vector<unsigned> > The solution paths
     */
    Solution solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize);
    /**
     * @brief Computes a single solution of the problem with some randomness
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @param excludedNodes Nodes to exclude from the solution
     * @return std::vector<std::vector<unsigned> > The solutions path
     */
    Solution greedyIteration(MDPSet& mdpSet, std::vector<bool> &excludedNodes, localSearch* lSearch);
    /**
     * @brief Generates a random seed for the solution computation
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @param excludedNodes Nodes to exclude from the solution
     * @return std::vector<std::vector<unsigned> > The initial seed
     */
    void generateSeed(MDPSet& mdpSet, std::vector<bool> &excludedNodes, std::vector<unsigned> &closerToDepot);
  private:
    unsigned nodesOrder_;
    unsigned pathsOrder_;
    Solution closerToDepot_;
};

}