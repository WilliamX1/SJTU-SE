#pragma once

#include <iostream>
#include <vector>
#include <string>

/* 跳表 */
template<typename K, typename V>
struct Node{
    Node<K, V> *right,*down;   //向右向下足矣
    K key;
    V val;
    Node(Node<K, V> *right,Node<K, V> *down, K key, V val): right(right), down(down), key(key), val(val){}
    Node(): right(nullptr), down(nullptr) {}
    ~Node(){};
};

template<typename K, typename V>
class Skiplist {
private:
    int64_t AddNode = 0;
    int64_t DelNode = 0;
public:
    Node<K, V> *head;
    Skiplist() {
        head = new Node<K, V>(); AddNode++; //初始化头结点
    }
    ~Skiplist() { clear(); };
    /*返回实际键值对数量*/
    size_t size(){
        size_t _size = 0;
        Node<K, V> *p = head;
        while(p->down) p = p->down;
        while(p->right){
            p = p->right;
            _size++;
        }
        return _size;
    }
    /*根据key查找对应的value并返回*/
    V* get(const K& key) {
        Node<K, V> *p = head;
        V* val = nullptr;
        while (p) {
            while (p->right && p->right->key < key) {
                p = p->right;
            }
            /*如果找到了对应的键值*/
            if (p->right && p->right->key == key) {
                val = new V(p->right->val);
                break;
            }
            p = p->down;
        }
        return val != nullptr ? val : nullptr;
    }
    /*插入键值对*/
    bool put(const K& key, const V& val) {
        std::vector<Node<K, V>*> pathList;    //从上至下记录搜索路径
        Node<K, V> *p = head;
        while(p){
            while(p->right && p->right->key < key){
                p = p->right;
            };
            /*如果已经存在key则用value进行覆盖*/
            if (p->right && p->right->key == key) {
                p = p->right;
                while (p) {
                    p->val = val;
                    p = p->down;
                };
                return false;
            };

            pathList.push_back(p);
            p = p->down;
        }

        bool insertUp = true;
        Node<K, V>* downNode= nullptr;
        while(insertUp && pathList.size() > 0){   //从下至上搜索路径回溯，50%概率
            Node<K, V> *insert = pathList.back();
            pathList.pop_back();
            insert->right = new Node<K, V>(insert->right, downNode, key, val); //add新结点

            AddNode++;

            downNode = insert->right;    //把新结点赋值为downNode
            insertUp = (rand() & 1);   //50%概率
        } std::vector<Node<K, V>*>().swap(pathList);

        if(insertUp){  //插入新的头结点，加层
            Node<K, V> * oldHead = head;
            head = new Node<K, V>();
            head->right = new Node<K, V>(NULL, downNode, key, val); AddNode++;
            head->down = oldHead;
        }

        return true;
    }
    /*清空跳表*/
    void clear() {
        if (head == nullptr) return;
        /*保存下一层头结点*/
        Node<K, V> *_save = head->down;
        Node<K, V> *del = head;
        Node<K, V> *cursor;

        while (_save) {
            _save = _save->down;
            while (del) {
                cursor = del->right;
                delete del; DelNode++;
                del = cursor;
            };
            del = _save;
        };

//        std::cout << "The skiplist: Insert " << AddNode << " Clear " << DelNode << " " << std::endl;
        head = nullptr;
        return;
    }
    /*获得键值最小*/
    K* min() {
        Node<K, V> *p = head;
        while (p->down) p = p->down;
        p = p->right;

        return p ? new K(p->key) : nullptr;
    }
    /*获得键值最大*/
    K* max() {
        Node<K, V> *p = head;
        while (p->down) p = p->down;
        while (p->right) p = p->right;

        return p != head ? new K(p->key) : nullptr;
    }
    /*遍历跳表*/
    void traverse() {
        Node<K, V> *p = head;
        Node<K, V> *q = p;

        while(q) {
            while(p) {
                std::cout << "(" << p->key << "," << p->val << ")";
                p = p->right;
            }
            std::cout << std::endl;
            p = q = q->down;
        }
        return;
    }
    /*获得key链表*/
    std::vector<K> get_key_arr() {
        std::vector<K> key_arr;

        Node<K, V> *p = head;
        while (p->down) p = p->down;
        p = p->right;

        while (p != nullptr) {
            key_arr.push_back(p->key);
            p = p->right;
        };
        return key_arr;
    };
    /*获得val链表*/
    std::vector<V> get_val_arr() {
        std::vector<V> val_arr;

        Node<K, V> *p = head;
        while (p->down) p = p->down;
        p = p->right;

        while (p != nullptr) {
            val_arr.push_back(p->val);
            p = p->right;
        };
        return val_arr;
    };
    /*判断是否为空*/
    bool empty() {
        return size() == 0;
    }
};
