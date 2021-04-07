#include <iostream>
#include <vector>
using namespace std;

/*
    坑点在于删除函数时从顶部开始向右向下寻找并删除后，
    往下一层寻找时若只用key来找会出错，因为可能删除掉另一个key值相等且在其右边的值
    因此需要记录它对应的下节点来进行下一层判断！！
*/
template<typename K, typename V>
struct Node{
    Node<K, V> *right,*down;   //向右向下足矣
    K key;
    V val;
    Node(Node<K, V> *right,Node<K, V> *down, K key, V val): right(right), down(down), key(key), val(val){}
    Node(): right(nullptr), down(nullptr) {}
};

template<typename K, typename V>
class Skiplist {
public:
    Node<K, V> *head;
    Skiplist() {
        head = new Node<K, V>();  //初始化头结点
    }

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
        return val;
    }
    
    void put(const K& key, const V& val) {
        vector<Node<K, V>*> pathList;    //从上至下记录搜索路径
        Node<K, V> *p = head;
        while(p){
            while(p->right && p->right->key < key){ 
                p = p->right;
            }
            pathList.push_back(p);
            p = p->down;
        }

        bool insertUp = true;
        Node<K, V>* downNode= nullptr;
        while(insertUp && pathList.size() > 0){   //从下至上搜索路径回溯，50%概率
            Node<K, V> *insert = pathList.back();
            pathList.pop_back();
            insert->right = new Node<K, V>(insert->right, downNode, key, val); //add新结点
            downNode = insert->right;    //把新结点赋值为downNode
            insertUp = (rand()&1);   //50%概率
        }
        if(insertUp){  //插入新的头结点，加层
            Node<K, V> * oldHead = head;
            head = new Node<K, V>();
            head->right = new Node<K, V>(NULL, downNode, key, val);
            head->down = oldHead;
        }
    }
    
    bool remove(const K& key) {
        Node<K, V> *p = head;
        Node<K, V> *tmp = nullptr;
        bool flag = false;
        while (p) {
            while (p->right && p->right->key < key) {
                p = p->right;
            }
            /*如果右节点值相等*/
            if (tmp != nullptr) 
            {
                while (p->right != tmp) 
                    p = p->right;
            }

            if (p->right && p->right->key == key) {
                flag = true;
                /*删除节点*/
                tmp = p->right;
                p->right = tmp->right;

                tmp = tmp->down;
                /*如果该层没有别的元素了，则删除该层*/
                if (!head->right && head->down) head = head->down;
            }
            p = p->down;
        }
        return flag;
    }
    
    void traverse() {
        Node<K, V> *p = head;
        Node<K, V> *q = p;

        while(q) {
            while(p) {
                cout << "(" << p->key << "," << p->val << ")";
                p = p->right;
            }
            cout << endl;
            p = q = q->down;
        }
        return;
    }
};

int main() {
    Skiplist<int, int> L;
    
    while(true) {
        int k, v;
        int ch; 
        
        cout << endl << "1.put(k, v).... \n 2.get(k)... \n 3.remove(k)... \n" << endl;
        cin >> ch;
        switch (ch)
        {
        case 1 : cin >> k >> v; L.put(k, v);
            break;
        case 2 : cin >> k;
            if (L.get(k)) cout << *L.get(k) << endl;
            else cout << "No such element!" << endl;
            break;
        case 3 : cin >> k;
            if (L.remove(k))
                cout << "remove successfully!";
            else cout << "No such element!!!";
        default:
            break;
        }
        cout << "size : " << L.size() << endl;
        L.traverse();
    }

    return 0;
}