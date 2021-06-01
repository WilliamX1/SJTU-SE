#include <iostream>
#include <vector>

using namespace std;

class HashTable { // your implementation
private:
    struct node
    {
        /* data */
        int key;
        int val;
        /* 0对应空，1对应删除，2对应有值 */
        int state;
        node(int _key, int _val, int _state = 0)
        : key(_key), val(_val), state(_state) {};
    };
    
public:
    const int length = 97; /* 数组长度 */
    vector<node> v; /* 存放元素的数组 */
    HashTable() {
        v = vector<node>(length, node(0, 0, 0));
        return;
    };

    void put(int key, int val) {
        int hash_key = hash_function(key); // 将key用哈希函数进行转换
        int iter = hash_key;
        while (iter < length) { // 从hash_key位置向后探查
            switch (v[iter].state)
            {
            /* 插入成功*/
            case 0: case 1:
                v[iter].key = key;
                v[iter].val = val;
                v[iter].state = 2;
                return;
                break;
            case 2: 
                if (v[iter].key == key) { // 如果重复插入相同key，则用后插入的val替代
                    v[iter].val = val;
                    return;
                } else iter++;
                break;
            };
        }
        /* 如果没有找到空位，循环到从链表头开始查找 */
        iter = 0;
        while (iter < hash_key) {
            switch (v[iter].state)
            {
            /* 插入成功*/
            case 0: case 1:
                v[iter].key = key;
                v[iter].val = val;
                v[iter].state = 2;
                return;
                break;
            case 2:
                if (v[iter].key == key) {
                    v[iter].val = val;
                    return;
                } else iter++;
                break;
            };
        }
    }; 

    int get(int key) {
        int hash_key = hash_function(key);
        int iter = hash_key;
        while (iter < length) {
            switch (v[iter].state)
            {
            case 0: return -1; break; // 不存在则返回-1
            case 1: ++iter; break;
            case 2: 
                if (v[iter].key == hash_key) return v[iter].val;
                else ++iter;
                break;
            };
        };
        /* 如果没有找到，则从链表头开始遍历 */
        iter = 0;
        while (iter < hash_key) {
            switch (v[iter].state)
            {
            case 0: return -1; break;
            case 1: ++iter; break;
            case 2: 
                if (v[iter].key == hash_key) return v[iter].val;
                else ++iter;
                break;
            };
        };
        return -1;
    }; // return -1 on searching failure

    void remove(int key) {
        int hash_key = hash_function(key);
        int iter = hash_key;
        while (iter < length && v[iter].key != key) iter++;

        if (v[iter].key == key) {
            v[iter].state = 1;
            return;
        } else {
            iter = 0;
            while (iter < hash_key && v[iter].key != key) iter++;
            if (v[iter].key == key) {
                v[iter].state = 1;
                return;
            }
        };
        return;
    };

    int hash_function(int key) {
        /* 将key映射到数组空间中 */
        return key % length;
    }

};

int main(){

    HashTable ht = HashTable();

    ht.put(1,1);

    ht.put(2,2);

    cout << ht.get(1) << ' ';

    cout << ht.get(3) << ' ';

    ht.put(2,1);

    cout << ht.get(2) << ' ';

    ht.remove(2);

    cout << ht.get(2) << ' ';

    ht.remove(3);

    cout << ht.get(3) << ' ';

    return 0;
} // 应当输出1 -1 1 -1 -1