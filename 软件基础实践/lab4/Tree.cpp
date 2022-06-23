#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <queue>
#include <algorithm>

#include "Tree.h"

using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/
int TreeNode::getX()
{
    return data[0];
}
int TreeNode::getY()
{
    return data[1];
}

istream& operator>>(istream &in, BinaryDimonTree &tree)
{
    int M; in >> M;

    AxisNode A[5096];
    for (int i = 0; i < M; i++)
        in >> A[i].x >> A[i].y;
    random_shuffle(A, A + M);

    tree.root = new TreeNode(A[0].x, A[0].y);

    TreeNode* findpos;
    int depth, x, y;
    for (int i = 1; i < M; i++)
    {
        x = A[i].x, y = A[i].y;

        findpos = tree.root;
        depth = 0;
        while(++depth)
        {
            if (depth & 1)
                if (x < findpos->getX())
                    if (findpos->left)
                        findpos = findpos->left;
                    else{
                        findpos->left = new TreeNode(x, y);
                        break;
                    }
                else
                    if (findpos->right)
                        findpos = findpos->right;
                    else{
                        findpos->right = new TreeNode(x, y);
                        break;
                    }
            else
                if (y < findpos->getY())
                    if (findpos->left)
                        findpos = findpos->left;
                    else{
                        findpos->left = new TreeNode(x, y);
                        break;
                    }
                else
                    if (findpos->right)
                        findpos = findpos->right;
                    else{
                        findpos->right = new TreeNode(x, y);
                        break;
                    }
        }
    }
    return in;
}


TreeNode* BinaryDimonTree::find_nearest_node(int x, int y)
{
    TreeNode* guess = NULL;
    long long int bestDist = 0x7fffffff;
    recur_search(root, x, y, 1, bestDist, &guess);
    return guess;
}

void BinaryDimonTree::recur_search(TreeNode *cur, int x, int y, int depth, long long int &min_distance, TreeNode **guess)
{
    if (cur == NULL) return;

    int dis2 = (x - cur->getX()) * (x - cur->getX()) + (y - cur->getY()) * (y - cur->getY());

    if (dis2 < min_distance)
    {
        min_distance = dis2;
        *guess = cur;
    }
    else if (dis2 == min_distance)
    {
        if (cur->getX() < (*guess)->getX())
            *guess = cur;
        else if (cur->getX() == (*guess)->getX() && cur->getY() < (*guess)->getY())
            *guess = cur;
    }

    if (depth & 1)
    {
        if (x < cur->getX())
        {
            recur_search(cur->left, x, y, depth + 1, min_distance, guess);
            if ((x - cur->getX()) * (x - cur->getX()) < min_distance)
                recur_search(cur->right, x, y, depth + 1, min_distance, guess);
        }
        else
        {
            recur_search(cur->right, x, y, depth + 1, min_distance, guess);
            if ((x - cur->getX()) * (x - cur->getX()) < min_distance)
                recur_search(cur->left, x, y, depth + 1, min_distance, guess);
        }
    }
    else
    {
        if (y < cur->getY())
        {
            recur_search(cur->left, x, y, depth + 1, min_distance, guess);
            if ((y - cur->getY()) * (y - cur->getY()) < min_distance)
                recur_search(cur->right, x, y, depth + 1, min_distance, guess);
        }
        else
        {
            recur_search(cur->right, x, y, depth + 1, min_distance, guess);
            if ((y - cur->getY()) * (y - cur->getY()) < min_distance)
                recur_search(cur->left, x, y, depth + 1, min_distance, guess);
        }
    }
    return;
}

