#include "MDPSet.h"

namespace MDP {

MDPSet::MDPSet(unsigned numberOfVectors, unsigned numberOfComponents) :
               vectorSet_(MDPVector(numberOfVectors, numberOfComponents)) {
  srand((unsigned)time(NULL));
  for(unsigned i = 0; i < numberOfVectors; ++i) {
    for(unsigned j = 0; j < numberOfComponents; ++j) {
      vectorSet_.at(i, j) = rand() % 10;
    }
  }
  computeDistances();
}

MDPSet::MDPSet(std::string path) {
  importInstance(path);
  computeDistances();
}

void MDPSet::computeDistances() {
  const unsigned NUMBER_OF_VECTORS = vectorSet_.numberOfVectors();
  distanceMatrix_ = VRP::VRPVector<float>(NUMBER_OF_VECTORS);
  for(unsigned i = 0; i < NUMBER_OF_VECTORS; ++i) {
    for(unsigned j = 0; j < NUMBER_OF_VECTORS; ++j) {
      float dist = 0;
      for(unsigned k = 0; k < vectorSet_.numberOfComponents(); ++k) {
        dist += std::pow(vectorSet_.at(i, k) - vectorSet_.at(j, k), 2);
      }
      distanceMatrix_.at(i, j) = std::sqrt(dist);
    }
  }
}

std::set<unsigned> MDPSet::getSuperSet() {
  std::set<unsigned> superSet;
  for(unsigned i = 0; i < vectorSet_.numberOfVectors(); ++i) {
    superSet.insert(i);
  }
  return superSet;
}

float MDPSet::at(unsigned vector, unsigned component) const {
  return vectorSet_.at(vector, component);
}

// int MDPSet::distance(unsigned vector1, unsigned vector2, unsigned component) const {
//   return std::sqrt(std::pow(vectorSet_.at(vector1, component).x - vectorSet_.at(vector2, component).x, 2) +
//                    std::pow(vectorSet_.at(vector1, component).y - vectorSet_.at(vector2, component).y, 2));
// }

// int MDPSet::distance(unsigned vector1, float vector2, unsigned component) const {
//   return std::sqrt(std::pow(vectorSet_.at(vector1, component).x - vectorSet_.at(vector2, component).x, 2) +
//                    std::pow(vectorSet_.at(vector1, component).y - vectorSet_.at(vector2, component).y, 2));
// }

float MDPSet::distance(unsigned vector1, unsigned vector2) const {
  return distanceMatrix_.at(vector1, vector2);
}

float MDPSet::distance(unsigned vector1, std::vector<float> vector2) const {
  if(vectorSet_.numberOfComponents() != vector2.size())
    throw std::invalid_argument("The number of components in the vector is not equal to the number of components in the set vectors.");
  float dist = 0;
  for(unsigned i = 0; i < vectorSet_.numberOfComponents(); ++i) {
    dist += std::pow(vectorSet_.at(vector1, i) - vector2[i], 2);
  }
  return std::sqrt(dist);
}

float MDPSet::distance(const std::set<unsigned>& vectorSet, unsigned vector) const {
  float dist = 0.0;
  for(const auto& vec : vectorSet) {
    dist += distance(vec, vector);
  }
  return dist;
}

float MDPSet::computeDiversity(const std::set<unsigned>& vectors, int candidate) {
  std::set candidateSet = vectors;
  if(candidate != -1)
    candidateSet.insert(candidate);
  if(candidateSet.size() < 2)
    return 0;
  float diversity = 0;
  for(const auto &v : candidateSet) {
    for(const auto &v2 : candidateSet) {
      if(v2 > v)
        diversity += distance(v, v2);
    }
  }
  return diversity;
}

std::vector<float> MDPSet::computeGravityCenter(const std::set<unsigned>& vectors) {
  if(vectors.size() == 1) {
    auto onlyVector = vectors.begin();
    return vectorSet_.at(*onlyVector);
  }
  std::vector<float> gravityCenter;
  for(unsigned component = 0; component < vectorSet_.numberOfComponents(); ++component) {
    float sum = 0;
    for(const auto &v : vectors) {
      sum += vectorSet_.at(v, component);
    }
    gravityCenter.push_back(sum / vectors.size());
  }
  return gravityCenter;
}

std::vector<unsigned> MDPSet::farthestFromGravityCenter(std::set<unsigned>& subset, std::set<unsigned>& excludedVectors, unsigned numberOfVectors) {
  std::vector<unsigned> closerVectors(0);
  const unsigned NUMBER_OF_VECTORS = vectorSet_.numberOfVectors();
  const std::vector<float> actualGravityCenter = computeGravityCenter(subset);
  int closestDistancePos = 0;
  float closestDistance = 0;
  for (unsigned i = 0; i < NUMBER_OF_VECTORS; ++i) {
    if(excludedVectors.find(i) != excludedVectors.end())
      continue;
    if(closerVectors.size() < numberOfVectors) {
      closerVectors.push_back(i);
      closestDistancePos = -1;
    } else if (closestDistance < distance(i, actualGravityCenter)) {
      closerVectors[closestDistancePos] = i;
      closestDistancePos = -1;
    }
    if(closestDistancePos == -1) {
      closestDistance = std::numeric_limits<float>::max();
      for(unsigned j = 0; j < closerVectors.size(); ++j) {
        if(closestDistance > distance(closerVectors[j], actualGravityCenter)) {
          closestDistance = distance(closerVectors[j], actualGravityCenter);
          closestDistancePos = j;
        }
      }
    }
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
        vectorSet_.at(i, j) = std::stof(component[0] + '.' +component[1]);
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