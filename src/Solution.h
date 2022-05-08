#pragma once

#include <iostream>
#include <vector>

namespace MDP {

class Solution {
  public:
    Solution() {}
    Solution(std::set<unsigned> set) : set_(set) {}
    std::set<unsigned> set_;
};

}