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

// void printColumn(std::string value, unsigned length, std::string final) {
//   std::cout << *(new std::string((length - value.size()) / 2, ' ')) <<
//   " " << value <<
//   *(new std::string((length - value.size()) / 2 + (length - value.size()) % 2, ' ')) << final; 
// }

// void printMDP(unsigned size, MDP::MDPSet &mdpSet, std::string localSearchType, unsigned iterations = 1) {
//   std::fstream outputFile = utility::OpenFile("VNSresult.txt", 'w');
//   MDP::localSearch* lSearch = nullptr;
//   if(localSearchType == "IntraR")
//     lSearch = new MDP::IntraReincertion;
//   else if(localSearchType == "InterR")
//     lSearch = new MDP::InterReincertion;
//   else if(localSearchType == "IntraI")
//     lSearch = new MDP::IntraInterchange;
//   else if(localSearchType == "InterI")
//     lSearch = new MDP::InterInterchange;
//   else if(localSearchType == "2Opt")
//     lSearch = new MDP::TwoOpt;

//   std::map<std::string, std::pair<float, unsigned> > timeMap;
//   timeMap["greedy"] = std::make_pair(0, 0);
//   timeMap["grasp"] = std::make_pair(0, 0);

//   printColumn(std::to_string(size), 6, " |-> ");
//   for(unsigned i = 0; i < iterations; ++i) {
//     MDP::GreedyMaximumDiversity greedyMDP(numberOfVehicles);
//     clock_t t_start_greedy = clock();
//     greedyMDP.Solve(mdpSet, lSearch);
//     timeMap["greedy"].second += greedyMDP.lastPathCost();
//     timeMap["greedy"].first += (double)(clock() - t_start_greedy)/CLOCKS_PER_SEC;

//     MDP::GraspMaximumDiversity graspMDP(numberOfVehicles);
//     clock_t t_start_grasp = clock();
//     graspMDP.Solve(mdpSet, lSearch);
//     timeMap["grasp"].second += graspMDP.lastPathCost();
//     timeMap["grasp"].first += (double)(clock() - t_start_grasp)/CLOCKS_PER_SEC;
//   }
//   printColumn(std::to_string(timeMap["greedy"].second / iterations), 12, " | ");
//   printColumn(std::to_string(timeMap["greedy"].first / iterations), 12, " | ");
//   printColumn(std::to_string(timeMap["grasp"].second / iterations), 12, " | ");
//   printColumn(std::to_string(timeMap["grasp"].first / iterations), 12, " | ");
// }

// int main(int argc, char **argv) {
//   try {
//     if(argc == 3) {
//       MDP::MDPSet mdpSet(std::stoi(argv[1]), std::stoi(argv[2]));
//       mdpSet.exportInstance(std::to_string(mdpSet.numberOfVectors()) + "_" + std::to_string(mdpSet.numberOfComponents()) + ".txt");
//       return 0;
//     }
//     if(argc < 4 || argc > 5)
//         throw std::length_error("At least four arguments required\nExecution: ./MDP path numberOfVehicles\n");
//     unsigned numberOfVehicles = std::stoi(argv[2]);
//     std::string localSearchType = argv[3];
//     unsigned numberOfIterations = (argc == 5) ? std::stoi(argv[4]) : 1;
//     try {
//       unsigned maxSize = std::stoi(argv[1]);
//       if(maxSize < 100)
//         throw std::length_error("The maximum size must be greater than 100\n");
//       std::cout << "  Size  |   Greedy Cost   |  Greedy Time  |   GRASP Cost  |   GRASP Time  |   GVNS Cost   |   GVNS Time   |\n";
//       for(unsigned i = 100; i <= maxSize; ++i) {
//         throw std::runtime_error("Function not implemented");
//         // MDP::MDPSet mdpSet(i);
//         // printMDP(i, mdpSet, numberOfVehicles, localSearchType, numberOfIterations);
//       }
//     }
//     catch(std::length_error &e) {
//       std::cout << e.what() << "\n";
//     }
//     catch(std::invalid_argument &e) {
//       std::string path = argv[1];
//       std::cout << "  Size  |   Greedy Cost   |  Greedy Time  |   GRASP Cost  |   GRASP Time  |   GVNS Cost   |   GVNS Time   |\n";
//       for (const auto & entry : std::filesystem::directory_iterator(path)) {
//         MDP::MDPSet mdpSet(entry.path());
//         mdpSet.nodesCoordinates();
//         printMDP(mdpSet.numberOfNodes(), mdpSet, numberOfVehicles, localSearchType, numberOfIterations);
//       }
//     }
//   }
//   catch(std::invalid_argument &e) {
//     std::cout << "The second argument should be a number\nExecution: ...\n";
//   }
// }

int main () {
  MDP::MDPSet mdpSet("data/max_div_15_2.txt");
  std::set<unsigned> a {2, 7, 1};
  std::cout << mdpSet.computeDiversity(a) << '\n';
  std::set<unsigned> b {0, 11};
  std::cout << mdpSet.computeDiversity(b) << '\n';
  mdpSet.computeGravityCenter(mdpSet.getSuperSet());
  MDP::DepthGraspBranchAndBoundMDP greedyMDP;
  MDP::Interchange* interchangeSearch = new MDP::Interchange;
  greedyMDP.Solve(mdpSet, interchangeSearch, 5);
  std::cout << greedyMDP.lastDiversity() << '\n';
}