#include "localSearch.h"

namespace MDP {

void localSearch::enhanceSolution(MDPSet& mdpSet, Solution& solution) {
  float accumulatedDifference = 0;
  do {
    nextOptimum(mdpSet, solution);
    accumulatedDifference += lastDifference_;
  } while(lastDifference_ > 0);
  lastDifference_ = accumulatedDifference;
}

void Interchange::nextOptimum(MDPSet& mdpSet, Solution& solution) {
  const unsigned NUMBER_OF_VECTORS = mdpSet.numberOfVectors();
  const unsigned NUMBER_OF_COMPONENTS = mdpSet.numberOfComponents();
  Solution baseSolution = solution;
  Solution newSolution = solution;
  std::set superSet = mdpSet.getSuperSet(); 
  float initialDiversity = mdpSet.computeDiversity(solution.set_);
  float bestDiversity = initialDiversity;

  for(const auto& inVector : baseSolution.set_) {
    for(const auto outVector : superSet) {
      if(inVector == outVector || baseSolution.set_.find(outVector) != baseSolution.set_.end())
        continue;
      newSolution.set_.erase(inVector);
      newSolution.set_.insert(outVector);
      float newDiversity = mdpSet.computeDiversity(newSolution.set_);
      if(newDiversity > bestDiversity) {
        bestDiversity = newDiversity;
        solution = newSolution;
      }
      newSolution = baseSolution;
    }
  }
  lastDifference_ = bestDiversity - initialDiversity;
}

}