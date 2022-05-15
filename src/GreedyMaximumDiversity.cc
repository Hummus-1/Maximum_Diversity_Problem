#include "GreedyMaximumDiversity.h"

namespace MDP {

Solution GreedyMaximumDiversity::greedyAlgorithm(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize, Solution solution, unsigned aleatorice) {
  while(solution.set_.size() < solutionSize) {
    unsigned nextCandidate = mdpSet.farthestFromGravityCenter(solution.set_, solution.set_, aleatorice)[rand() % aleatorice];
    solution.set_.insert(nextCandidate);
  }
  
  if(lSearch != nullptr)
    lSearch->enhanceSolution(mdpSet, solution);
  lastDiversity_ = mdpSet.computeDiversity(solution.set_);
  return Solution(solution);
}

Solution GreedyMaximumDiversity::solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
  const unsigned NUMBER_OF_VECTORS = mdpSet.numberOfVectors();
  const unsigned NUMBER_OF_COMPONENTS = mdpSet.numberOfComponents();
  std::set superSet = mdpSet.getSuperSet();
  Solution solution;
  solution.set_.insert(mdpSet.farthestFromGravityCenter(superSet, solution.set_)[0]);
  return greedyAlgorithm(mdpSet, lSearch, solutionSize, solution);
}

}