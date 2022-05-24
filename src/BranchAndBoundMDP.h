#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "GraspMaximumDiversity.h"

namespace MDP {

const unsigned EPSILON = 0.00001;

class TreeNode {
  public:
    TreeNode(float diversity, Solution fixedVectors) : value_(diversity), fixedVectors_(fixedVectors) {}
    float value_;
    unsigned last_;
    Solution fixedVectors_;
};

struct LowerBound {
  bool operator()(const TreeNode& lhs, const TreeNode& rhs) {
    return lhs.value_ > rhs.value_;
  }
};

struct DFS {
  bool operator()(const TreeNode& lhs, const TreeNode& rhs) {
    if(lhs.fixedVectors_.set_.size() == rhs.fixedVectors_.set_.size()) {
      return lhs.value_ > rhs.value_;
    }
    return lhs.fixedVectors_.set_.size() < rhs.fixedVectors_.set_.size();
  }
};

template<class Comparator>
class MDPTree {
  public:
  MDPTree(unsigned numberOfVectors, unsigned solutionSize, MDPSet &mdpSet) : 
    numberOfVectors_(numberOfVectors), solutionSize_(solutionSize), mdpSet_(mdpSet) {
    for(unsigned i = 0; i < numberOfVectors_; i++) {
      Solution initial(std::set<unsigned>{i});
      TreeNode newBranch = createBranch(initial);
      newBranch.last_ = i;
      openBranches_.push(newBranch);
    }
  }

  float firstOpenBranchValue() {
    return openBranches_.top().value_;
  }

  TreeNode firstOpenBranch() {
    return openBranches_.top();
  }

  void openNextBranch() {
    TreeNode node = openBranches_.top();
    openBranches_.pop();
    for(unsigned vector = node.last_; vector < numberOfVectors_; ++vector) {
      if(node.fixedVectors_.set_.find(vector) == node.fixedVectors_.set_.end()) {
        Solution initial(node.fixedVectors_);
        initial.set_.insert(vector);
        TreeNode newNode = createBranch(initial);
        newNode.last_ = vector;
        openBranches_.push(newNode);
      }
    }
  }

  TreeNode createBranch(const Solution& fixedVectors) {
    ++numberOfGeneratedNodes_;
    Solution fixedVectorsCopy = fixedVectors;
    float diversity = mdpSet_.computeDiversity(fixedVectorsCopy.set_);
    unsigned requiredVectors = (solutionSize_ - fixedVectorsCopy.set_.size());
    unsigned insertedVectors = 0;
    while(requiredVectors > insertedVectors) {
      int bestVector = -1;
      float bestDistance = 0;
      for(unsigned vector = 0; vector < numberOfVectors_; ++vector) {
        if(fixedVectorsCopy.set_.find(vector) == fixedVectorsCopy.set_.end()) {
          float distance = mdpSet_.distance(fixedVectorsCopy.set_, vector);
          if(bestVector == -1 || distance > bestDistance) {
            bestVector = vector;
            bestDistance = distance;
          }
        }
      }
      if(requiredVectors == 1) {
        diversity += bestDistance;
        insertedVectors += 1;
      }
      else {
        diversity += bestDistance * ((fixedVectors.set_.size() + 2) / fixedVectors.set_.size());
        insertedVectors += 1;
      }
      fixedVectorsCopy.set_.insert(bestVector);
    }
    return TreeNode(diversity, fixedVectors);
  }

  bool isEmpty() {
    return openBranches_.empty();
  }

  void bound() {
    openBranches_.pop();
  }

  unsigned numberOfGeneratedNodes() {
    return numberOfGeneratedNodes_;
  }

  unsigned numberOfVectors() const { return numberOfVectors_; }
  std::priority_queue<TreeNode, std::vector<TreeNode>, Comparator> openBranches_;
  private:
    unsigned numberOfGeneratedNodes_ = 0;
    MDPSet &mdpSet_;
    GreedyMaximumDiversity completer;
    unsigned numberOfVectors_;
    unsigned solutionSize_;
};

/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
template<class Comparator>
class BranchAndBoundMDP : public MaximumDiversity {
  public:
    unsigned numberOfGeneratedNodes() {
      return numberOfGeneratedNodes_;
    }
  protected:
    /**
     * @brief Solve the problem with the adjacency matrix and number of vehicles given
     * 
     * @param mdpSet The adjacency matrix to compute
     * @param numberOfVehicles The number of vehicles
     * @return std::vector<std::vector<unsigned> > The solution paths
     */
    Solution solveProblem(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
      const unsigned NUMBER_OF_VECTORS = mdpSet.numberOfVectors();
      MDPTree<Comparator> mdpTree(NUMBER_OF_VECTORS, solutionSize, mdpSet);
      Solution lowerBound = getLowerBound(mdpSet, lSearch, solutionSize);
      GreedyMaximumDiversity greedySolve;
      float lowerBoundValue = mdpSet.computeDiversity(lowerBound.set_);

      while(!mdpTree.isEmpty() && (untilEmpty_ || mdpTree.firstOpenBranchValue() > lowerBoundValue)) {
        TreeNode newBranch = mdpTree.firstOpenBranch();
        if(newBranch.value_ > lowerBoundValue) {
          if(newBranch.fixedVectors_.set_.size() == solutionSize - 1) {
            Solution lowerBoundCandidate = greedySolve.greedyAlgorithm(mdpSet, nullptr, solutionSize, newBranch.fixedVectors_);
            if((greedySolve.lastDiversity() - lowerBoundValue) > EPSILON) {
              lowerBound = lowerBoundCandidate;
              lowerBoundValue = greedySolve.lastDiversity();
            }
          }
          if(newBranch.fixedVectors_.set_.size() < solutionSize - 1) {
            mdpTree.openNextBranch();
          }
          else
            mdpTree.bound();
        }
        else
         mdpTree.bound();
      }
      
      numberOfGeneratedNodes_ = mdpTree.numberOfGeneratedNodes();
      lastDiversity_ = lowerBoundValue;
      return lowerBound;
    }

    virtual Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) = 0;
    unsigned numberOfGeneratedNodes_;
    bool untilEmpty_;
};


/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class LowGreedyBranchAndBoundMDP : public BranchAndBoundMDP<LowerBound> {
  public:
    LowGreedyBranchAndBoundMDP() {untilEmpty_ = true;}
  protected:
    Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
      return GreedyMaximumDiversity().Solve(mdpSet, lSearch, solutionSize);
    }
};

/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class DepthGreedyBranchAndBoundMDP : public BranchAndBoundMDP<DFS> {
  public:
    DepthGreedyBranchAndBoundMDP() {untilEmpty_ = true;}
  protected:
    Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
      return GreedyMaximumDiversity().Solve(mdpSet, lSearch, solutionSize);
    }
};

/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class LowGraspBranchAndBoundMDP : public BranchAndBoundMDP<LowerBound> {
  public:
    LowGraspBranchAndBoundMDP() {untilEmpty_ = true;}
  protected:
    Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
      return GraspMaximumDiversity().Solve(mdpSet, lSearch, solutionSize);
    }
};

/**
 * @brief Solves the MDP problem with a GRASP approach
 * 
 */
class DepthGraspBranchAndBoundMDP : public BranchAndBoundMDP<DFS> {
  public:
    DepthGraspBranchAndBoundMDP() {untilEmpty_ = true;}
  protected:
    Solution getLowerBound(MDPSet& mdpSet, localSearch* lSearch, unsigned solutionSize) {
      return GraspMaximumDiversity().Solve(mdpSet, lSearch, solutionSize);
    }
};

}