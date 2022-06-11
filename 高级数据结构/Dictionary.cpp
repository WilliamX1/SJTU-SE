#include <iostream>
using std::ostream;

template <typename K, typename V> 
struct Dictionary {
public:
    int sizecnt;
    struct node{
        K key;
        V value;
        node* next;
        node(){};
        node(K key, V value, node* next = nullptr)
        : key(key), value(value), next(next){};
    };
    node* head;
public:
    Dictionary(): sizecnt(0), head(nullptr){};
    int size(){
        return sizecnt;
    }
    bool put( K key, V value){
        if (head == nullptr) {
            head = new node(key, value);
            sizecnt++;
            return true;
        } else {
            node* cur = head;
            while(cur->next != nullptr && cur->next->key <= key) cur = cur->next;
            if (cur->key == key) {
                cur->value = value;
                return true;
            } else {
                cur->next = new node(key, value, cur->next);
                sizecnt++;
                return true;
            }
        }
        return true;
    }
    V* get( K key ){
        node* cur = head;
        while(cur != nullptr && cur->key < key) cur = cur->next;

        if (cur == nullptr || cur->key != key) return nullptr;
        
        V* value = new V(cur->value);
        return value;
    }
    bool remove( K key ) {
        node* pre = head;
        if (pre == nullptr) return false;
        else if (pre->key == key) {
            head = head->next;
            delete pre;
            sizecnt--;
            return true;
        }

        node* cur = head->next;
        while(cur != nullptr && cur->key < key){
            pre = cur;
            cur = cur->next;
        }
        
        if (cur == nullptr || cur->key != key) return false;
        else {
            pre->next = cur->next;
            delete cur;
            sizecnt--;
            return true;
        }

        return true;
    }
    void traverse() const{
        node* cur = head;
        while(cur != nullptr){
            std::cout << cur->key << ' ' << cur->value << ' ';
            cur = cur->next;
        }
        return;
    }
    template <typename K1, typename V1> 
    friend ostream& operator<<(ostream& os, const Dictionary<K1,V1>& dic);
};

template <typename K1, typename V1> 
ostream& operator<<(ostream& os, const Dictionary<K1,V1>& dic)
{
    dic.traverse();
    return os;
}

int main()
{
    Dictionary<int, int> D;
    D.put(1, 1);
    D.remove(1);
    return 0;
}