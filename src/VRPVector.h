#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <math.h>

#include "menuUtility.h"
#include "Solution.h"

namespace VRP {

template<class T>
class VRPVector {
  public:
  VRPVector() {}
  VRPVector(unsigned numberOfNodes) : numberOfNodes_(numberOfNodes) {
    v_.resize(numberOfNodes_ * numberOfNodes_);
  }
  T& at(unsigned originNode, unsigned destinyNode) {
    return v_[originNode * numberOfNodes_ + destinyNode];
  }
  T at(unsigned originNode, unsigned destinyNode) const {
    return v_[originNode * numberOfNodes_ + destinyNode];
  }
  unsigned numberOfNodes() { return numberOfNodes_; }
  std::vector<T> v_;
  private:
    unsigned numberOfNodes_;
};

}