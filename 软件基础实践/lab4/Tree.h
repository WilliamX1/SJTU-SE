//
// Created by Smile on 2019/4/8.
//

#ifndef C_BINARYDIMEN_TREE_H
#define C_BINARYDIMEN_TREE_H

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

struct AxisNode
{
    int x;
    int y;
    AxisNode(){};
    static bool myCompare(AxisNode a, AxisNode b){
        return a.x < b.x;
    }
};

class TreeNode
{
  friend ostream &operator<<(ostream &out, const TreeNode &b);
  friend class BinaryTree;
  friend class BinaryDimonTree;

private:
  /* data */
  int data[2];

public:
  /* methods */
  TreeNode* left;
  TreeNode* right;

  TreeNode(int x = 0, int y = 0, TreeNode* l = NULL, TreeNode* r = NULL){
      data[0] = x;
      data[1] = y;
      left = l;
      right = r;
  };
  int getX();  /* DO NOT CHANGE */
  int getY();  /* DO NOT CHANGE */
  ~TreeNode(); /* DO NOT CHANGE */
};


class BinaryDimonTree
{
friend istream &operator>>(istream &in, BinaryDimonTree &tree); /* DO NOT CHANGE */

private:
  /* data */
  TreeNode *root;

public:
  /* methods */
  BinaryDimonTree(){};          /* DO NOT CHANGE */
  TreeNode *find_nearest_node(int x, int y);  /* DO NOT CHANGE */

  void recur_search(TreeNode *cur, int x, int y, int depth, long long int &min_distance, TreeNode **guess);

};

#endif //C_BINARYDIMEN_TREE_H
