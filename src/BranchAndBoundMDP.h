#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "GraspMaximumDiversity.h"

namespace MDP {

class TreeNode {
  public:
    TreeNode(float diversity, Solution fixedVectors) : diversity_(diversity), fixedVectors_(fixedVectors) {}
    float diversity_;
    Solution fixedVectors_;
};

struct LowerBound {
  bool operator()(const TreeNode& lhs, const TreeNode& rhs) {
    return lhs.diversity_ < rhs.diversity_;
  }
};

struct DFS {
  bool operator()(const TreeNode& lhs, const TreeNode& rhs) {
    if(lhs.diversity_ == rhs.diversity_) {
      return lhs.fixedVectors_.set_.size() < rhs.fixedVectors_.set_.size();
    }
    return lhs.diversity_ < rhs.diversity_;
  }
};

// AUMENTAR EN 1 EL TAMAÑO DE LA SOLUCIÓN DEL BOUND ???
template<class Comparator>
class MDPTree {
  public:
  MDPTree(unsigned numberOfVectors, unsigned solutionSize, MDPSet &mdpSet) : 
    numberOfVectors_(numberOfVectors), solutionSize_(solutionSize), mdpSet_(mdpSet) {
    for(unsigned i = 0; i < numberOfVectors_; i++) {
      Solution initial(std::set<unsigned>{i});
      completer.greedyAlgorithm(mdpSet, nullptr, solutionSize_, initial);
      openBranches_.push(TreeNode(completer.lastDiversity(), initial));
    }
  }

  float bestOpenBranchDiversity() {
    return openBranches_.top().diversity_;
  }

  void openNextBranch() {
    TreeNode node = *openBranches_.top();
    openBranches_.pop();
    for(unsigned vector = 0; vector < numberOfVectors_; ++vector) {
      if(node.fixedVectors_.set_.find(vector) == node.fixedVectors_.set_.end()) {
        Solution newBranch(node.fixedVectors_);
        newBranch.set_.insert(vector);
        completer.greedyAlgorithm(mdpSet_, nullptr, solutionSize_, newBranch);
        openBranches_.push(TreeNode(completer.lastDiversity(), newBranch));
      }
    }
  }

  unsigned numberOfVectors() const { return numberOfVectors_; }
  std::priority_queue<TreeNode, std::vector<TreeNode>, Comparator> openBranches_;
  private:
    MDPSet &mdpSet_;
    GreedyMaximumDiversity completer;
    unsigned numberOfVectors_;
    unsigned solutionSize_;
};

/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class BranchAndBoundMDP : public MaximumDiversity {
  public:
    BranchAndBoundMDP() {}
  protected:
    /**
     * @brief Solve the problem with the adjacency matrix and number of vehicles given
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @return std::vector<std::vector<unsigned> > The solution paths
     */
    Solution solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize);
    virtual Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) = 0;
};


/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class LowGreedyBranchAndBoundMDP : public BranchAndBoundMDP {
  public:
    LowGreedyBranchAndBoundMDP() {}
  protected:
    Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
      return GreedyMaximumDiversity().Solve(mdpSet, lSearch, solutionSize);
    }
};

/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class DepthGraspBranchAndBoundMDP : public BranchAndBoundMDP {
  public:
    DepthGraspBranchAndBoundMDP() {}
  protected:
    Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
      return GraspMaximumDiversity().Solve(mdpSet, lSearch, solutionSize);
    }
};

}