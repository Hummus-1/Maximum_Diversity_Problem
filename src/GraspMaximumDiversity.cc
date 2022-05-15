#include "GraspMaximumDiversity.h"

namespace MDP {

Solution GraspMaximumDiversity::solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
  const unsigned NUMBER_OF_VECTORS = mdpSet.numberOfVectors();
  const unsigned NUMBER_OF_COMPONENTS = mdpSet.numberOfComponents();
  std::set superSet = mdpSet.getSuperSet();
  Solution solution;
  float bestDiversity {-1.0};
  GreedyMaximumDiversity greedySolve;

  unsigned numberOfInitialVectors = NUMBER_OF_VECTORS * 0.1 * solutionSize;
  std::vector<unsigned> farthestFromGravityCenterVec = mdpSet.farthestFromGravityCenter(superSet, solution.set_, numberOfInitialVectors);
  for(const auto& farthestFromGravityCenter : farthestFromGravityCenterVec) {
    farthestFromGravityCenter_.set_.insert(farthestFromGravityCenter);
  }

  for(unsigned aleatorice = 1; aleatorice <= solutionSize; aleatorice++) {
    for(const auto& initialVector : farthestFromGravityCenter_.set_) {
      Solution newSolution(std::set<unsigned>{initialVector});
      newSolution = greedySolve.greedyAlgorithm(mdpSet, lSearch, solutionSize, newSolution);
      float newDiversity = greedySolve.lastDiversity();
      if(newDiversity > bestDiversity) {
        bestDiversity = newDiversity;
        solution = newSolution;
      }
    }
  }
  lastDiversity_ = bestDiversity;
  return solution;
}

}