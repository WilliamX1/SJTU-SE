#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

template <typename T>
class SkipList {
public:
    struct node {
        T value;
        struct node* up;
        struct node* down;
        struct node* left;
        struct node* right;
        
        node (T val = " ")
        : value(val), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {};
    };
    node* head;
    node* tail;
    int lvl_num; //当前链表层数
    T tag; //查询不到时返回值
    /*获取元素节点*/
    node* search(const T& val) {
        node* cursor = head;
        while(cursor->right != nullptr && cursor->down != nullptr) {
            if (cursor->left == nullptr) cursor = cursor->right;
            while(cursor->right != nullptr && cursor->value <= val)
                cursor = cursor->right;
            cursor = cursor->left;
            cursor = cursor->down;
        }
        /*寻找最后一层*/
        if (cursor->left == nullptr) cursor = cursor->right;
        while(cursor->right != nullptr && cursor->value <= val) cursor = cursor->right;
        /*返回小于等于带查找值的节点*/
        return cursor->left;
    };
public:
    SkipList(T t): lvl_num(1), tag(t) {
        /*头尾节点内容表示层数，最底层是1*/
        head = new node(tag);
        tail = new node(tag);
        head->right = tail;
        tail->left = head;
    }
    /*插入新元素，重复元素无法插入*/
    bool insert(T val) {
        /*寻找底层待插入节点*/
        node* cursor = search(val);
        /*如果元素重复则无法插入*/
        if (cursor->left != nullptr && cursor->value == val) return false;
        /*向底层插入节点*/
        node* new_node = new node(val);
        new_node->left = cursor, new_node->right = cursor->right; //设置新节点前驱后继
        new_node->left->right = new_node, new_node->right->left = new_node; //设置新节点的前驱节点的后继和后继节点的前驱
        /*随机是否向上生长一层*/
        while (rand() % 5 != 1) {
            /*向前移动找到第一个有上节点的节点*/
            while (cursor->left != nullptr && cursor->up == nullptr) cursor = cursor->left;
            /*当前层数不够，则需开辟新层*/
            if (cursor->up == nullptr) {
                lvl_num++;
                node* new_head = head->up = new node(tag);
                node* new_tail = tail->up = new node(tag);
                
                new_head->down = head, head->up = new_head;
                new_tail->down = tail, tail->up = new_tail;
                tail = new_head->right = new_tail;
                head = new_tail->left = new_head;
            }
            /*生长一层*/
            cursor = cursor->up;
            node* _new_node = new node(val);
            _new_node->left = cursor, _new_node->right = cursor->right; //设置新节点前驱后继
            _new_node->left->right = _new_node, _new_node->right->left = _new_node; //设置新节点的前驱节点的后继和后继节点的前驱
            /*连通上下节点*/
            _new_node->down = new_node, new_node->up = _new_node;
            /*更新最"底层"的插入节点*/
            new_node = _new_node, new_node = _new_node;
        }
        return true;
    };
    /*查询元素*/
    T get(const T& val) {
        node* cursor = search(val);
        if (cursor->left == nullptr || cursor->value != val) return tag;
        else return true;
    };
    /*删除元素*/
    bool remove(T val) {
        node* cursor = search(val);
        /*如果查无此值*/
        if (cursor->left == nullptr || cursor->value != val) return false;
        else {
            node* up_cursor = cursor;

            while (cursor != nullptr) {
                up_cursor = up_cursor->up;
                /*分别更新左右节点的后继和前驱*/
                cursor->left->right = cursor->right;
                cursor->right->left = cursor->left;
                /*删除该节点*/
                delete cursor;
                
                cursor = up_cursor;  
            }
            /*如果该层只有一个有效节点，则删除头尾哨兵*/
            node* tmp_head = head;
            node* tmp_tail = tail;
            cursor = head;
            while (cursor->right == tail) {
                /*如果是第一层，则跳出*/
                if (cursor->down == nullptr) break;
                /*删除该层头尾哨兵*/
                head = head->down, tail = tail->down;

                cursor = cursor->down;
                delete tmp_head, delete tmp_tail;
                /*迭代*/
                tmp_head = head, tmp_tail = tail;
            }
            return true;
        }
    };
    /*遍历打印树形结构*/
    void traverse(){
        node* cursor = head;
        while(cursor != nullptr) {
            node* cur = cursor;
            while(cur != nullptr) {
                cout << cur->value << " ";
                cur = cur->right;
            }
            cursor = cursor->down;
            printf("\n");
        }
        return;
    }
    /*返回树的高度*/
    int height() const{
        return lvl_num;
    }
};

int main() {
    /*随机化种子*/
    srand(time(0));
    SkipList<string> L("||");
    
    L.insert("c");
    L.insert("b");
    L.insert("aa");
    L.insert("d");
    L.insert("c");
    L.insert("f");
    L.traverse();
    std::cout << L.remove("c") << std::endl;
    L.traverse();
    std::cout << L.remove("f") << std::endl;
    L.traverse();
    std::cout << L.remove("b") << std::endl;
    L.traverse();
    std::cout << L.remove("d") << std::endl;
    L.traverse();
    std::cout << L.remove("aa") << std::endl;
    L.traverse();
    
    return 0;
}