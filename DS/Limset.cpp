#include <iostream>
#include <unordered_map>
#include <list>
#include <algorithm>

using namespace std;
template<typename K>
class LimSet {
private:
    int size, cap;

    unordered_map<K, typename std::list<K>::iterator> _map;
    list<K> _list;
public:
    explicit LimSet(int capacity) {
        size = 0;
        cap = capacity;
    }

    bool in(const K &key) {
        if (_map.find(key) != _map.end()){
            /*如果存在该元素，则将其从链表中移到最新位置*/
            _list.erase(_map.find(key)->second);
            _list.push_back(key);
            /*更新_map节点中地址*/
            _map.find(key)->second = --_list.end();
            return true;
        } else return false;
    }

    void add(const K &key) {
        if (in(key) || cap == 0) return;
        else if (size == cap) {
            _map.erase(_list.front());
            _list.erase(_list.begin()); /*删除头结点*/ 
            
            _list.push_back(key); /*添加尾结点*/
            _map.insert(make_pair(key, --_list.end()));
        } else {
            _list.push_back(key);
            _map.insert(make_pair(key, --_list.end()));
            size++;
        };
        return;
    }

    void remove(const K &key) {
        if (_map.find(key) != _map.end()){
            _list.erase(_map.find(key)->second);
            _map.erase(key);
            size--;
        }    
        return;
    }

    void traverse(){
        typename list<K>::iterator it; //声明一个迭代器
        for(it = _list.begin(); it != _list.end(); it++){
            cout << *it <<endl;
        }
        cout << endl;
        return;
    }
};

// using namespace std;
// template<typename K>
// class LimSet {
// private:
//     int size, cap;

//     unordered_map<K, K> _map;
//     vector<K> _list;
// public:
//     explicit LimSet(int capacity) {
//         size = 0;
//         cap = capacity;
//     }

//     bool in(const K &key) {
//         if (_map.find(key) != _map.end()){
//             /*如果存在该元素，则将其从链表中移到最新位置*/
//             _list.erase(find(_list.begin(), _list.end(),key));
//             _list.push_back(key);
//             return true;
//         } else return false;
//     }

//     void add(const K &key) {
//         if (in(key) || cap == 0) return;
//         else if (size == cap) {
//             _map.erase(_list.front()); 
//             _map.insert(make_pair(key, key));

//             _list.erase(_list.begin()); /*删除头结点*/
//             _list.push_back(key); /*添加尾结点*/
//         } else {
//             _map.insert(make_pair(key, key));
//             _list.push_back(key);
//             size++;
//         };
//         return;
//     }

//     void remove(const K &key) {
//         if (_map.find(key) != _map.end()){
//             _map.erase(key);
//             _list.erase(find(_list.begin(), _list.end(), key));
//             size--;
//         }    
//         return;
//     }

//     void traverse(){
//         vector<int>::iterator it; //声明一个迭代器
//         for(it = _list.begin(); it != _list.end(); it++){
//             cout << *it <<endl;
//         }
//         cout << endl;
//         return;
//     }
// };

int main()
{
    LimSet<int> L(4);
    
    int ch, key;

    while(true){
        cin >> ch;
        switch (ch)
        {
        case 0:
            cin >> key;
            L.add(key);
            break;
        case 1:
            cin >> key;
            L.remove(key);
            break;
        case 2:
            cin >> key;
            L.in(key);
            break;
        default:
            break;
        }

        L.traverse();
    }

    return 0;
}


// template<typename K>
// class LimSet {
// private:
//     int size, cap;

//     struct node
//     {
//         K key;
//         node* next;
//         node(K k, node* n): key(k), next(n){};
//     };
//     /*头尾哨兵*/
//     node* head;
//     node* tail;
// public:
//     explicit LimSet(int capacity) {
//         size = 0;
//         cap = capacity;

//         head = new node(0, nullptr);
//     }

//     bool in(const K& key) {
//         node* cursor = head;

//         while(cursor->next != nullptr){
//             if (cursor->next->key == key) {
//                 node* tmp = cursor->next;
//                 cursor->next = tmp->next;

//                 tail = tail->next = tmp;
//                 tail->next = nullptr;

//                 return true;
//             }
//             else cursor = cursor->next;
//         }
//         return false;
//     }

//     void add(const K& key) {
//         if (in(key)) return;
//         else if (head->next == nullptr) {
//             if (cap == 0) return;

//             head->next = tail = new node(key, nullptr);
//             size++;
//         } else if (size == cap) {
//             node* tmp = head->next;
//             head->next = tmp->next;
//             delete tmp;

//             tail = tail->next = new node(key, nullptr);
//         } else {
//             tail = tail->next = new node(key, nullptr);
//             size++;
//         };
//         return;
//     }

//     void remove(const K& key) {
//         node* cursor = head;
//         while(cursor->next != nullptr) {
//             if (cursor->next->key == key) {
//                 node* tmp = cursor->next;
//                 cursor->next = tmp->next;
//                 delete tmp;

//                 size--;
//                 return;
//             } else {
//                 cursor = cursor->next;
//             };
//         };
//         return;
//     }

//     void traverse(){
//         node* cursor = head->next;
//         while(cursor != nullptr) {
//             cout << cursor->key << " ";
//             cursor = cursor->next;
//         }
//         cout << endl;
//         return;
//     }
// };
