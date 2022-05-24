#pragma once

#include <iostream>
#include <vector>
#include <set>

namespace MDP {

class Solution {
  public:
    Solution() {}
    Solution(std::set<unsigned> set) : set_(set) {}
    std::string toString() const {
      std::string s = "{";
      for(auto i : set_) {
        s += std::to_string(i) + ",";
      }
      //s[s.size() - 2] = ' ';
      s[s.size() - 1] = '}';
      return s;
    }
    std::set<unsigned> set_;
};

}