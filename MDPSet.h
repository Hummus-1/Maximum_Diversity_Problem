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

struct Coordinate {
  Coordinate() : x(0), y(0) {}
  Coordinate(int x, int y) : x(x), y(y) {}
  int x;
  int y;
};

class MDPVector {
  public:
  MDPVector() {}
  MDPVector(unsigned numberOfVectors, unsigned numberOfComponents) : 
    numberOfVectors_(numberOfVectors), numberOfComponents_(numberOfComponents) {
    v_ = std::vector<Coordinate>(numberOfVectors * numberOfComponents);
  }
  // void setSize() {
  //   v_.resize(numberOfVectors_ * numberOfVectors_);
  // }
  Coordinate& at(const unsigned vector, const unsigned component) {
    return v_[vector * numberOfComponents_ + component];
  }
  Coordinate at(unsigned vector, unsigned component) const {
    return v_[vector * numberOfComponents_ + component];
  }
  unsigned numberOfVectors() const { return numberOfVectors_; }
  unsigned numberOfComponents() const { return numberOfComponents_; }
  std::vector<Coordinate> v_;
  private:
    unsigned numberOfVectors_;
    unsigned numberOfComponents_;
};

/**
 * @brief Represents an adjacency matrix
 * 
 */
class MDPSet {
  public: 
    MDPSet(unsigned numberOfVectors, unsigned numberOfComponents);
    MDPSet(std::string path);

    void exportInstance(std::string path);

    Coordinate at(unsigned vector, unsigned component) const;

    unsigned numberOfVectors() const { return vectorSet_.numberOfVectors(); }
    unsigned numberOfComponents() const { return vectorSet_.numberOfComponents(); }
    std::set<unsigned> getSuperSet();

    int distance(unsigned vector1, unsigned vector2, unsigned component) const;
    int distance(unsigned vector1, unsigned vector2) const;

    Coordinate computeGravityCenter(const std::set<unsigned>& vectors);
    int computeDiversity(const std::set<unsigned>& vectors);
    std::vector<unsigned> farthestFromGravityCenter(std::set<unsigned>& excludedVectores, unsigned numberOfVectors = 1);
  private:
    void importInstance(std::string path);

    MDPVector vectorSet_;
};

}