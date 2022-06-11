#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <iostream>
#include <stdio.h>
#include "SkipList.h"

class MemTable
{
private:
    Skiplist<uint64_t, std::string>* realize;
    uint64_t size;
public:
    MemTable() {
        this->realize = new Skiplist<uint64_t, std::string>;
        this->size = 32 + 10240;
    };
    ~MemTable() {
        this->clear();
    }
    uint64_t getSize() {
        return size;
    };
    uint64_t get_key_value_num() {
        return this->realize->size();
    };
    uint64_t get_key_min() {
        uint64_t* ans = this->realize->min();
        uint64_t res = *ans;
        if (ans != nullptr) {
            delete ans;
            ans = nullptr;
        };
        return res;
    };
    uint64_t get_key_max() {
        uint64_t* ans = this->realize->max();
        uint64_t res = *ans;
        if (ans != nullptr) {
            delete ans;
            ans = nullptr;
        };
        return res;
    }
    void put(uint64_t key, const std::string &s) {
        std::string* origin_val = this->get(key);
        int64_t origin_size = origin_val == nullptr ? 0 : origin_val->size();

        this->realize->put(key, s);
        this->size += 8 + 4 + s.size() - origin_size;

        if(origin_val) delete origin_val;
        return;
    };
    std::string* get(uint64_t key) {
        return this->realize->get(key);
    };
    void del(uint64_t key) {
        this->put(key, "~DELETED~");
        return;
    };
    void clear() {
        if (realize != nullptr) {
            delete realize;
            realize = nullptr;
        }
        size = 32 + 10240;
        return;
    };
    void get_key_value_array(std::vector<uint64_t>& key_arr, std::vector<std::string>& val_arr) {
        key_arr = this->realize->get_key_arr();
        val_arr = this->realize->get_val_arr();
    };
    bool empty() {
        return realize->empty();
    };
};

#endif // MEMTABLE_H
