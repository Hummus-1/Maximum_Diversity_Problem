#include "MaximumDiversity.h"

namespace MDP {

Solution MaximumDiversity::Solve(unsigned numberOfVectors, unsigned numberOfComponents, localSearch* lSearch, unsigned solutionSize) {
  MDPSet mdpSet(numberOfVectors, numberOfComponents);
  return solveProblem(mdpSet, lSearch, solutionSize);
}

Solution MaximumDiversity::Solve(std::string path, localSearch* lSearch, unsigned solutionSize) {
  MDPSet mdpSet(path);
  return solveProblem(mdpSet, lSearch, solutionSize);
}

Solution MaximumDiversity::Solve(MDPSet &mdpSet, localSearch* lSearch, unsigned solutionSize) {
  return solveProblem(mdpSet, lSearch, solutionSize);
}

}