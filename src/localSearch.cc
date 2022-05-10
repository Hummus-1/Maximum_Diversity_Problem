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

float Interchange::difference(MDPSet& mdpSet, Solution& solution, unsigned inVector, unsigned outVector) {
  float difference = 0;
  for(const auto& node : solution.set_) {
    if(node == inVector)
      continue;
    difference -= mdpSet.distance(node, inVector);
    difference += mdpSet.distance(node, outVector);
  }
  return difference;
}

void Interchange::nextOptimum(MDPSet& mdpSet, Solution& solution) {
  const unsigned NUMBER_OF_VECTORS = mdpSet.numberOfVectors();
  const unsigned NUMBER_OF_COMPONENTS = mdpSet.numberOfComponents();
  Solution baseSolution = solution;
  std::set superSet = mdpSet.getSuperSet(); 
  float initialDiversity = mdpSet.computeDiversity(solution.set_);
  float bestDifference = 0;

  for(const auto& inVector : baseSolution.set_) {
    for(const auto outVector : superSet) {
      if(inVector == outVector || baseSolution.set_.find(outVector) != baseSolution.set_.end())
        continue;
      float newDifference = difference(mdpSet, baseSolution, inVector, outVector);
      if(newDifference > bestDifference) {
        solution = baseSolution;
        solution.set_.erase(inVector);
        solution.set_.insert(outVector);
        bestDifference = newDifference;
      }
    }
  }
  lastDifference_ = bestDifference;
}

}