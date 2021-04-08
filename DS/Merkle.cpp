#include <iostream>
#include <vector>
using namespace std;

unsigned long hash64shift(unsigned long key)
{
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key;
}

class Merkle_Tree {
private:
    struct node
    {
        unsigned long hash_value;
        node* left;
        node* right;
        node* father; //父节点
        node(){};
        node(unsigned long hv) 
        : hash_value(hv), left(nullptr), right(nullptr), father(nullptr) {};
        node(unsigned long hv, node* l, node* r) 
        : hash_value(hv), left(l), right(r), father(nullptr) {};
    };
    struct leaf_node //最底层串联结点
    {
        unsigned long hash_value;
        node* father;
        leaf_node* next;
        leaf_node(){};
        leaf_node(unsigned long hv)
        : hash_value(hv), father(nullptr), next(nullptr) {};
        leaf_node(unsigned long hv, node* f, leaf_node* n)
        : hash_value(hv), father(f), next(n) {};
    };
    const unsigned long FLAG = 0; //哨兵结点值
    leaf_node* head;
public:
    Merkle_Tree() { head = new leaf_node(FLAG); return; };
    Merkle_Tree(vector<unsigned long>);
    vector<unsigned long> getProof(unsigned long hash_value);
    void addTransaction(unsigned long);
    unsigned long getRootHash();

    void printTree(); //调试用
};
Merkle_Tree::Merkle_Tree(vector<unsigned long> data)
{
    vector<unsigned long>::const_iterator data_iter = data.begin();
    vector<node*> tmp;

    head = new leaf_node(FLAG);
    leaf_node* cursor = head;
    while (data_iter != data.end()) {
        cursor = cursor->next = new leaf_node(*data_iter);

        cursor->father = new node(*data_iter);
        tmp.push_back(cursor->father);

        data_iter++;
    };

    vector<node*> evaluate_tmp;
    vector<node*>::iterator tmp_iter;
    node *left_child, *right_child;

    while (tmp.size() > 1) {
        tmp_iter = tmp.begin();
        evaluate_tmp.clear();

        while (tmp_iter != tmp.end()) {
            left_child = *tmp_iter;
            tmp_iter++;

            if (tmp_iter != tmp.end()) {
                right_child = *tmp_iter;
                tmp_iter++;

                unsigned long father_val = hash64shift(left_child->hash_value + right_child->hash_value);
                node *father = new node(father_val, left_child, right_child);
                left_child->father = right_child->father = father;

                evaluate_tmp.push_back(father);
            } else {
                unsigned long father_val = left_child->hash_value;
                node* father = new node(father_val, left_child, nullptr);
                
                left_child->father = father;
                evaluate_tmp.push_back(father);
            }
        }; 
        tmp = evaluate_tmp;
    }
    
    return;
};
vector<unsigned long> Merkle_Tree::getProof(unsigned long hash_value) {
    leaf_node* cursor = head->next;
    node* tmp = nullptr;

    while (cursor != nullptr) {
        if (cursor->hash_value == hash_value) {
            tmp = cursor->father;
            break;
        } else cursor = cursor->next;
    };

    vector<unsigned long> res;
    /*对于不存在的节点需要返回0*/
    if (tmp == nullptr) {
        // res.push_back(FLAG);
    } else {
        node* father_tmp = tmp->father;
        while (father_tmp != nullptr) {
            if (father_tmp->left == tmp && father_tmp->right != nullptr) {
                res.push_back(father_tmp->right->hash_value);
            } else if (father_tmp->right == tmp && father_tmp->left != nullptr) {
                res.push_back(father_tmp->left->hash_value);
            } else {
                res.push_back(FLAG);
            };
            tmp = father_tmp;
            father_tmp = tmp->father;
        }
    }
   
    
    return res;
}
void Merkle_Tree::addTransaction(unsigned long new_hash_value) {
    /*如果本身链表为空*/
    if (head->next == nullptr) {
        head->next = new leaf_node(new_hash_value);
        head->next->father = new node(new_hash_value);
        head->next->father->father = new node(new_hash_value, head->next->father, nullptr);
        return;
    }
    
    leaf_node* cursor = head;
    vector<node*> tmp_node;
    while (cursor->next != nullptr) {
        cursor = cursor->next;
        tmp_node.push_back(cursor->father);
    }
    /*将新哈希值添加到leaf_node链表中*/
    cursor->next = new leaf_node(new_hash_value);
    cursor->next->father = new node(new_hash_value);
    node *tmp_left, *tmp_right = cursor->next->father, *tmp_father;
    /*向上生成树形结构*/
    
    while (true) {
        tmp_left = *(tmp_node.rbegin());
        
        tmp_father = tmp_left->father;
        if (tmp_father == nullptr) {
            tmp_right->father = tmp_left->father = new node(hash64shift(tmp_left->hash_value + tmp_right->hash_value), tmp_left, tmp_right);
            break;
        } else if (tmp_father->right == nullptr) {
            tmp_father->right = tmp_right;
            tmp_right->father = tmp_father;
            while (tmp_father != nullptr) {
                if (tmp_father->right != nullptr) tmp_father->hash_value = hash64shift(tmp_father->left->hash_value + tmp_father->right->hash_value);
                else tmp_father->hash_value = tmp_father->left->hash_value;
                tmp_father = tmp_father->father;
            };
            break;
        } else {
            tmp_left = tmp_father;
            tmp_right = tmp_right->father = new node(new_hash_value, tmp_right, nullptr);
        };
        /*更新tmp_node*/
        vector<node*> copy_tmp_node = tmp_node;
        tmp_node.clear();

        vector<node*>::const_iterator iter = copy_tmp_node.begin();
        while (iter != copy_tmp_node.end()) {
            tmp_node.push_back((*iter)->father);
            
            iter++;
            if (iter == copy_tmp_node.end()) break;
            iter++;
        };
    };
    return;
}
unsigned long Merkle_Tree::getRootHash() {

    unsigned long res = FLAG;
    if (head->next != nullptr) {
        node* cursor = head->next->father;
        while (cursor->father != nullptr) cursor = cursor->father;
        res = cursor->hash_value;
    };
    return res;
};
void Merkle_Tree::printTree(){
    leaf_node* cursor = head->next;
    vector<node*> tmp;
    while (cursor != nullptr) {
        tmp.push_back(cursor->father);
        cursor = cursor->next;
    };

    while (!tmp.empty()) {
        vector<node*>::iterator iter = tmp.begin();
        while (iter != tmp.end()) {
            cout << (*iter)->hash_value << " ";
            iter++;
        }; cout << endl;

        /*更新tmp*/
        vector<node*> copy_tmp = tmp;
        tmp.clear();
        iter = copy_tmp.begin();
        while (iter != copy_tmp.end()) {
            if ((*iter)->father != nullptr) tmp.push_back((*iter)->father);

            iter++;
            if (iter == copy_tmp.end()) break;
            iter++;
        };
    }
    return;
}

// int main() {
    
//     vector<unsigned long> v;
//     for (int i = 1; i < 4; i++) {
//         v.push_back(i);
//     };

//     Merkle_Tree tree;
//     tree.addTransaction(1);
//     tree.printTree();

//     cout << tree.getRootHash() << endl;
    
//     vector<unsigned long> vv = tree.getProof(1);
//     vector<unsigned long>::iterator iter = vv.begin();
//     while (iter != vv.end()) {
//         cout << *iter << " ";
//         iter++;
//     };

//     return 0;
// }