#include "MDPSet.h"

namespace MDP {

MDPSet::MDPSet(unsigned numberOfVectors, unsigned numberOfComponents) :
               vectorSet_(MDPVector(numberOfVectors, numberOfComponents)) {
  srand((unsigned)time(NULL));
  for(unsigned i = 0; i < numberOfVectors; ++i) {
    for(unsigned j = 0; j < numberOfComponents; ++j) {
      vectorSet_.at(i, j) = Coordinate(rand() % 100, rand() % 100);
    }
  }
}

MDPSet::MDPSet(std::string path) {
  importInstance(path);
}

std::set<unsigned> MDPSet::getSuperSet() {
  std::set<unsigned> superSet;
  for(unsigned i = 0; i < vectorSet_.numberOfVectors(); ++i) {
    superSet.insert(i);
  }
  return superSet;
}

Coordinate MDPSet::at(unsigned vector, unsigned component) const {
  return vectorSet_.at(vector, component);
}

int MDPSet::distance(unsigned vector1, unsigned vector2, unsigned component) const {
  return std::sqrt(std::pow(vectorSet_.at(vector1, component).x - vectorSet_.at(vector2, component).x, 2) +
                   std::pow(vectorSet_.at(vector1, component).y - vectorSet_.at(vector2, component).y, 2));
}

// int MDPSet::distance(unsigned vector1, Coordinate vector2, unsigned component) const {
//   return std::sqrt(std::pow(vectorSet_.at(vector1, component).x - vectorSet_.at(vector2, component).x, 2) +
//                    std::pow(vectorSet_.at(vector1, component).y - vectorSet_.at(vector2, component).y, 2));
// }

int MDPSet::distance(unsigned vector1, unsigned vector2) const {
  int dist = 0;
  for(unsigned i = 0; i < vectorSet_.numberOfComponents(); ++i) {
    dist += distance(vector1, vector2, i);
  }
  return dist;
}

int MDPSet::computeDiversity(const std::set<unsigned>& vectors) {
  if(vectors.size() < 2)
    return 0;
  int diversity = 0;
  for(const auto &v : vectors) {
    for(const auto &v2 : vectors) {
      if(v != v2)
        diversity += distance(v, v2);
    }
  }
  return diversity / (vectors.size() * (vectors.size() - 1));
}

Coordinate MDPSet::computeGravityCenter(const std::set<unsigned>& vectors) {
  if(vectors.size() == 1) {
    // auto onlyVector = vectors.begin();
    // return vectorSet_.at(*onlyVector);
  }
}

std::vector<unsigned> MDPSet::farthestFromGravityCenter(std::set<unsigned>& excludedVectors, unsigned numberOfVectors) {
  std::vector<unsigned> closerVectors(0);
  const unsigned NUMBER_OF_VECTORS = vectorSet_.numberOfVectors();
  const Coordinate actualGravityCenter = computeGravityCenter(excludedVectors);
  int closestDistancePos = -1;
  int closestDistance = -1;
  for (unsigned i = 0; i < NUMBER_OF_VECTORS; ++i) {
    if(excludedVectors.find(i) != excludedVectors.end())
      continue;
    // if(closerVectors.size() < numberOfVectors) {
    //   closerVectors.push_back(i);
    //   closestDistancePos = -1;
    // } else if (closestDistance < distance(i, actualGravityCenter)) {
    //   closerVectors[closestDistancePos] = i;
    //   closestDistancePos = -1;
    // }
    // if(closestDistancePos == -1) {
    //   for(unsigned j = 0; j < closerVectors.size(); ++j) {
    //     if(closestDistance < distance(closerVectors[j], actualGravityCenter)) {
    //       closestDistance = distance(closerVectors[j], actualGravityCenter);
    //       closestDistancePos = j;
    //     }
    //   }
    // }
  } 
  return closerVectors;
}

void MDPSet::importInstance(std::string path) {
  unsigned lineCount = 1;
  try {
    std::queue<std::string> fileData = utility::ReadFile(path, 'v');
    const unsigned NUMBER_OF_VECTORS = std::stoi(fileData.front());
    fileData.pop(); ++lineCount;
    const unsigned NUMBER_OF_COMPONENTS = std::stoi(fileData.front());
    fileData.pop(); ++lineCount;
    vectorSet_ = MDPVector(NUMBER_OF_VECTORS, NUMBER_OF_COMPONENTS);
    for(unsigned i = 0; i < NUMBER_OF_VECTORS; ++i) {
      for(unsigned j = 0; j < NUMBER_OF_COMPONENTS; ++j) {
        std::vector<std::string> component = utility::split(fileData.front(), ',', 2);
        vectorSet_.at(i, j) = Coordinate(std::stoi(component[0]), std::stoi(component[1]));
        fileData.pop();
      }
      ++lineCount;
    }
  }
  catch(std::ifstream::failure &error) {
    throw std::runtime_error(error.what());
  }
  catch(std::invalid_argument &error) {
    throw std::runtime_error("Error while reding file " + path + " on line " + std::to_string(lineCount) + ":\n" + error.what());
  } 
}

void MDPSet::exportInstance(std::string path) {
  throw std::runtime_error("Not implemented");
  // unsigned NUMBER_OF_VECTORS = vectorSet_.numberOfNodes();
  // std::fstream outputFile = OpenFile(path, 'w');
  // outputFile << NUMBER_OF_VECTORS << '\n';
  // for(unsigned i = 0; i < NUMBER_OF_VECTORS; ++i) {
  //   for(unsigned j = i + 1; j < NUMBER_OF_VECTORS; ++j) {
  //     outputFile << vectorSet_.at(i, j) << ' ';
  //   }
  //   outputFile << '\n';
  // }
}

}