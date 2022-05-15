#include "BranchAndBoundMDP.h"

namespace MDP {

Solution BranchAndBoundMDP::solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
  const unsigned NUMBER_OF_VECTORS = mdpSet.numberOfVectors();
  const unsigned NUMBER_OF_COMPONENTS = mdpSet.numberOfComponents();
  float bestDiversity {-1.0};
  MDPTree<DFS> mdpTree(NUMBER_OF_VECTORS, solutionSize, mdpSet);
  float a = mdpTree.bestOpenBranchDiversity();
  Solution lowerBound = getLowerBound(mdpSet, lSearch, solutionSize);

  while(mdpTree.bestOpenBranchDiversity() > bestDiversity) {
    // MDPTree::Branch bestBranch = mdpTree.bestOpenBranch();
    // Solution newSolution = bestBranch.solve(mdpSet, lSearch, solutionSize);
    // float newDiversity = mdpSet.computeDiversity(newSolution.set_);
    // if(newDiversity > bestDiversity) {
    //   bestDiversity = newDiversity;
    //   lowerBound = newSolution;
    // }
  }
  
  lastDiversity_ = bestDiversity;
  //return solution;
}

}