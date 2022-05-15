#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <set>

#include "menuUtility.h"
#include "Solution.h"

namespace MDP {

class MDPVector {
  public:
  MDPVector() {}
  MDPVector(unsigned numberOfVectors, unsigned numberOfComponents) : 
    numberOfVectors_(numberOfVectors), numberOfComponents_(numberOfComponents) {
    v_ = std::vector<float>(numberOfVectors * numberOfComponents);
  }
  float& at(const unsigned vector, const unsigned component) {
    return v_[vector * numberOfComponents_ + component];
  }

  float at(unsigned vector, unsigned component) const {
    return v_[vector * numberOfComponents_ + component];
  }

  std::vector<float> at(unsigned vector) const {
    return std::vector<float>(v_.begin() + (vector * numberOfComponents_), v_.begin() + (vector * numberOfComponents_ + numberOfComponents_));
  }

  unsigned numberOfVectors() const { return numberOfVectors_; }
  unsigned numberOfComponents() const { return numberOfComponents_; }
  std::vector<float> v_;
  private:
    unsigned numberOfVectors_;
    unsigned numberOfComponents_;
};

class MDPSet {
  public: 
    MDPSet(unsigned numberOfVectors, unsigned numberOfComponents);
    MDPSet(std::string path);

    void exportInstance(std::string path);

    float at(unsigned vector, unsigned component) const;

    unsigned numberOfVectors() const { return vectorSet_.numberOfVectors(); }
    unsigned numberOfComponents() const { return vectorSet_.numberOfComponents(); }
    std::set<unsigned> getSuperSet();

    //int distance(unsigned vector1, unsigned vector2, unsigned component) const;
    float distance(unsigned vector1, unsigned vector2) const;
    float distance(unsigned vector1, std::vector<float> vector2) const;

    std::vector<float> computeGravityCenter(const std::set<unsigned>& vectors);
    float computeDiversity(const std::set<unsigned>& vectors, int candidate = -1);
    std::vector<unsigned> farthestFromGravityCenter(std::set<unsigned>& excludedVectores, std::set<unsigned>& excludedVectors, unsigned numberOfVectors = 1);
  private:
    void importInstance(std::string path);

    MDPVector vectorSet_;
};

}