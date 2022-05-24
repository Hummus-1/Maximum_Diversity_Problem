#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <thread>
#include <chrono> 
#include <map>
#include <filesystem>
#include <math.h>
#include <deque>
#include <memory>

#include "BranchAndBoundMDP.h"
#include "GraspMaximumDiversity.h"
#include "GreedyMaximumDiversity.h"
#include "MaximumDiversity.h"
#include "menuUtility.h"

void printColumn(std::string value, unsigned length, std::string final, unsigned precision = 0) {
  if(precision != 0)
    value = value.substr(0, value.find(".") + precision);
  std::cout << *(new std::string((length - value.size()) / 2, ' ')) <<
  " " << value <<
  *(new std::string((length - value.size()) / 2 + (length - value.size()) % 2, ' ')) << final; 
}

void printMDP(std::string problem, unsigned numberOfVectors, unsigned numberOfComponents, unsigned solutionSize, MDP::MDPSet &mdpSet, unsigned iterations, std::string algorithm) {
  MDP::localSearch *lSearch = new MDP::Interchange();
  std::map<std::string, std::pair<float, float> > timeMap;
  timeMap["greedy"] = std::make_pair(0.0, 0.0);
  timeMap["grasp"] = std::make_pair(0.0, 0.0);
  timeMap["branchBound"] = std::make_pair(0.0, 0.0);
  MDP::Solution greedySolution, graspSolution, branchBoundSolution;

  printColumn("|" + problem, problem.size() + 1, " | ");
  printColumn(std::to_string(numberOfVectors), 3, " | ");
  printColumn(std::to_string(numberOfComponents), 2, " | ");
  printColumn(std::to_string(solutionSize), 2, " | ");
  MDP::GreedyMaximumDiversity greedyMDP;
  MDP::GraspMaximumDiversity graspMDP;
  MDP::DepthGraspBranchAndBoundMDP branchBound;
  for(unsigned i = 0; i < iterations; ++i) {
    if(algorithm == "greedy") {
      clock_t t_start_greedy = clock();
      greedySolution = greedyMDP.Solve(mdpSet, lSearch, solutionSize);
      timeMap["greedy"].second += greedyMDP.lastDiversity();
      timeMap["greedy"].first += (double)(clock() - t_start_greedy)/CLOCKS_PER_SEC;
    }
    else if(algorithm == "grasp") {
      clock_t t_start_grasp = clock();
      graspSolution = graspMDP.Solve(mdpSet, lSearch, solutionSize);
      timeMap["grasp"].second += graspMDP.lastDiversity();
      timeMap["grasp"].first += (double)(clock() - t_start_grasp)/CLOCKS_PER_SEC;
    }
    else if(algorithm == "branchBound") {
      clock_t t_start_branchBound = clock();
      branchBoundSolution = branchBound.Solve(mdpSet, lSearch, solutionSize);
      timeMap["branchBound"].second += branchBound.lastDiversity();
      timeMap["branchBound"].first += (double)(clock() - t_start_branchBound)/CLOCKS_PER_SEC;
    }
  }
  if(algorithm == "greedy") {
    printColumn(std::to_string(timeMap["greedy"].second / iterations), 7, " | ", 4);
    printColumn(std::to_string(timeMap["greedy"].first / iterations), 8, " | ", 6);
    printColumn(greedySolution.toString(), 16, " |\n");
  }
  else if(algorithm == "grasp") {
    printColumn(std::to_string(timeMap["grasp"].second / iterations), 7, " | ", 4);
    printColumn(std::to_string(timeMap["grasp"].first / iterations), 8, " | ", 6);
    printColumn(graspSolution.toString(), 16, " | ");
    printColumn(std::to_string(graspMDP.lastNumberOfIterations()), 11, " |\n");
  }
  else if(algorithm == "branchBound") {
    printColumn(std::to_string(timeMap["branchBound"].second / iterations), 7, " | ", 4);
    printColumn(std::to_string(timeMap["branchBound"].first / iterations), 8, " | ", 6);
    printColumn(branchBoundSolution.toString(), 16, " | ");
    printColumn(std::to_string(branchBound.numberOfGeneratedNodes()), 10, " |\n");
  }
}

int main(int argc, char **argv) {
  try {
    std::string path = argv[1];
    std::string algorithm = argv[2];
    unsigned numberOfIterations = (argc > 3) ? std::stoi(argv[3]) : 1;
    if(algorithm == "greedy") {
      std::cout << " |    Problem      |  n   |  K  |  m  |   Cost   |    Time   |      Solution     |\n";
      std::cout << " |:---------------:|:----:|:---:|:---:|:--------:|:---------:|:-----------------:|\n";
    }
    else if(algorithm == "grasp") {
      std::cout << " |    Problem      |  n   |  K  |  m  |   Cost   |    Time   |      Solution     |  Iterations  |\n";
      std::cout << " |:---------------:|:----:|:---:|:---:|:--------:|:---------:|:-----------------:|:------------:|\n";
    }
    else if(algorithm == "branchBound") {
      std::cout << " |    Problem      |  n   |  K  |  m  |   Cost   |    Time   |      Solution     |    Nodes    |\n";
      std::cout << " |:---------------:|:----:|:---:|:---:|:--------:|:---------:|:-----------------:|:-----------:|\n";
    }
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
      MDP::MDPSet mdpSet(entry.path());
      for(unsigned i = 2; i <= 5; ++i) {
        printMDP(entry.path().filename().string(), mdpSet.numberOfVectors(), mdpSet.numberOfComponents(), i, mdpSet, numberOfIterations, algorithm);
      }
    }
  }
  catch(std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

// int main () {
//   MDP::MDPSet mdpSet("data/max_div_15_2.txt");
//   mdpSet.computeGravityCenter(mdpSet.getSuperSet());
//   MDP::GraspMaximumDiversity graspMDP;
//   MDP::Interchange* interchangeSearch = new MDP::Interchange;
//   graspMDP.Solve(mdpSet, interchangeSearch, 5);
//   std::cout << graspMDP.lastDiversity() << '\n';
// }