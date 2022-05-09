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

  // Without random insertions
  for(const auto& initialVector : farthestFromGravityCenter_.set_) {
    Solution newSolution(std::set<unsigned>{initialVector});
    newSolution = greedySolve.greedyAlgorithm(mdpSet, lSearch, solutionSize, newSolution);
    if(greedySolve.lastDiversity() > bestDiversity) {
      bestDiversity = greedySolve.lastDiversity();
      solution = newSolution;
    }
  }

  return solution;


  // GreedyMaximumDiversity greedyApproach(numberOfVehicles_);
  // Solution bestPath = greedyApproach.Solve(mdpSet, lSearch);
  // int bestCost = greedyApproach.lastPathCost();

  // unsigned count{}, maxIterations{2000};
  // unsigned stopCondition = (std::sqrt(maxIterations) * 10);

  // std::vector<bool> excludedNodes(mdpSet.numberOfNodes(), false);
  // excludedNodes[0] = true;
  // unsigned initialNodes = (numberOfVehicles_ > mdpSet.numberOfNodes() / 6) ? (mdpSet.numberOfNodes() / 6) : numberOfVehicles_;
  // std::vector<unsigned> closerToDepot = mdpSet.closerConnection('R', excludedNodes, 0, (initialNodes * 2) * (mdpSet.numberOfNodes() / (initialNodes * 3) * 0.1 + 1));
  // std::vector<unsigned> closerToDepotL = mdpSet.closerConnection('L', excludedNodes, 0, (initialNodes * 2) * (mdpSet.numberOfNodes() / (initialNodes * 3) * 0.1 + 1));
  // closerToDepot.insert(closerToDepot.end(), closerToDepotL.begin(), closerToDepotL.end());

  // srand((unsigned) time(NULL) + rand());
  // for(unsigned i = 0; i < maxIterations; ++i, ++count) {
  //   excludedNodes = std::vector<bool>(mdpSet.numberOfNodes(), false);
  //   excludedNodes[0] = true;
  //   generateSeed(mdpSet, excludedNodes, closerToDepot);
  //   Solution paths = greedyIteration(mdpSet, excludedNodes, lSearch);
  //   int pathsCost = mdpSet.pathCost(paths);
  //   if(count > stopCondition) 
  //     break;
  //   if(bestCost < 0 || pathsCost < bestCost) {
  //     count = 0;
  //     bestCost = pathsCost;
  //     bestPath = paths;
  //   }
  // }
  // lastPathCost_ = bestCost;
  // return bestPath;
}

void GraspMaximumDiversity::generateSeed(MDPSet& mdpSet, std::vector<bool> &excludedNodes, std::vector<unsigned> &closerToDepot) {
  
}

Solution GraspMaximumDiversity::greedyIteration(MDPSet& mdpSet, std::vector<bool> &excludedNodes, localSearch* lSearch) {
  
  // if(lSearch != nullptr)
  //   lSearch->enhanceSolution(mdpSet, paths);
  // return paths;
}

}