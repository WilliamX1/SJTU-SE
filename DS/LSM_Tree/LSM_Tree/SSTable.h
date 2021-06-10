#pragma once

#include <stdint.h>
#include <memory.h>
#include <string>
#include <vector>

#include "MurmurHash3.h"

class SSTable
{
public:
    std::string filepath; //文件名称

    struct KeyValueArrayNode {
        uint64_t key;
        std::string value;
        uint64_t timestamp;
        uint64_t level;
        KeyValueArrayNode() {};
        KeyValueArrayNode(uint64_t k, std::string s, uint64_t t, uint64_t l) : key(k), value(s), timestamp(t), level(l) {};
    };

    struct Header
    {
        /*共占用32B，用于存放元数据*/
        uint64_t timestamp; //SSTable的时间戳（无符号64位整型）
        uint64_t key_value_num; //SSTable中键值对的数量（无符号64位整型）
        uint64_t key_min; //键最小值（有符号64位整型）
        uint64_t key_max; //键最大值（有符号64位整型）
        Header() {};
        Header(uint64_t tsp, uint64_t knum, uint64_t kmin, uint64_t kmax)
            : timestamp(tsp), key_value_num(knum), key_min(kmin), key_max(kmax) {};
    } *_header;
    struct BloomFilter
    {
        friend class KVStore;
        /*大小为10KB（10240字节）*/
        /*每个hash值为128-bit，分为四个无符号32位整型使用*/
        /* 10240 ÷ 4 = 2560 */
    private:
        const static int _hash_size = sizeof (uint8_t) << 3; //8位
        const static int mod = 2560 * _hash_size;
        void _put(unsigned int hash, int level) {
            hash %= mod;
            _hash_table[level][(hash / _hash_size)] |= 1 << (hash % _hash_size);
        }
        bool _get(unsigned int hash, int level){
            hash %= mod;
            return (_hash_table[level][hash / _hash_size] >> (hash % _hash_size)) & 1;
        }
    public:
        std::vector<std::vector<uint8_t>> _hash_table;
        BloomFilter() {
            _hash_table.assign(4, std::vector<uint8_t>(2560, 0));
        };
        ~BloomFilter() {
            std::vector<std::vector<uint8_t>>().swap(_hash_table);
        };
        /*****插入键值*****/
        void put(uint64_t key) {
            unsigned int hash[4] = {0};
            MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
            /****分成四个维度使用****/
            for (int i = 0; i < 4; i++)
                _put(hash[i], i);
        };
        bool get(uint64_t key){
            unsigned int hash[4] = {0};
            MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
            for (int i = 0; i < 4; i++)
                if (!_get(hash[i], i)) return false;
            return true;
        };
    } *_bloom_filter;
    struct IndexArea
    {
        /*索引区，用来存储有序的索引数据
         *包含所有的键及对应的值在文件中的offset
         *占用 key_value_num * 12B
         */
        struct IndexData
        {
            uint64_t key;
            uint32_t offset;
            IndexData(): key(0), offset(0) {};
            IndexData(uint64_t k, uint32_t o) : key(k), offset(o) {};
        };
        IndexData* _index;
        IndexArea(uint64_t key_value_num) {
            _index = (struct IndexData *) malloc(key_value_num * sizeof (IndexData));
        };
        ~IndexArea() {
            if (_index != nullptr) {
                delete[] _index;
                _index = nullptr;
            }
        }
    } *_index_area;
    struct DataArea
    {
        /*数据区，用于存储数据（不包含对应的key）*/
        std::vector<std::string> _value;
        DataArea() {};
        ~DataArea() {
            std::vector<std::string>().swap(_value);
        };
    } *_data_area;
public:
    SSTable(uint64_t timestamp, uint64_t key_value_num,
            uint64_t key_min, uint64_t key_max); //SSTable中键值对的数量（无符号64位整型）);
    ~SSTable();
    std::string* get(uint64_t key);
    /* 释放数据区空间 */
    void release_dataArea () {
        if (_data_area) delete _data_area;
        _data_area = nullptr;
    };

    std::vector<KeyValueArrayNode> get_key_value_array (uint64_t level) {
        std::vector<KeyValueArrayNode> ans;
        for (uint64_t i = 0; i < this->_header->key_value_num; i++)
            ans.push_back(KeyValueArrayNode(this->_index_area->_index[i].key, this->_data_area->_value[i], this->_header->timestamp, level));
        return ans;
    }
};

