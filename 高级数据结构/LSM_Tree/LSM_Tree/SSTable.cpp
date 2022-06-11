#include "SSTable.h"
#include <iostream>
#include <sys/stat.h>
#include <fstream>

bool useIndex = true;
bool useBloomFilter = true;

SSTable::SSTable(uint64_t timestamp, uint64_t key_value_num,
                 uint64_t key_min, uint64_t key_max) {
    _header = new Header(timestamp, key_value_num, key_min, key_max);
    _bloom_filter = new BloomFilter();
    _index_area = new IndexArea(_header->key_value_num);
    _data_area = new DataArea();
};

SSTable::~SSTable() {
    if (_header != nullptr) {
        delete _header;
        _header = nullptr;
    }
    if (_bloom_filter != nullptr) {
        delete _bloom_filter;
        _bloom_filter = nullptr;
    }
    if (_index_area != nullptr) {
        this->release_dataArea();
        delete _index_area;
        _index_area = nullptr;
    }
    if (_data_area != nullptr) {
        delete _data_area;
        _data_area = nullptr;
    }
};

std::string* SSTable::get(uint64_t key){
    if (key < _header->key_min || key > _header->key_max) return nullptr; //如果不在数值范围内
    if (useBloomFilter == true && _bloom_filter->get(key) == false) return nullptr; //先在布隆过滤器中查找是否存在
    else {
        if (useIndex == true) {
            uint64_t left_idx = 0, right_idx = _header->key_value_num - 1, mid_idx;
            while (left_idx <= right_idx) {
                mid_idx = (left_idx + right_idx) >> 1;
                /****需要将offset与文件地址关联起来****/
                if (_index_area->_index[mid_idx].key == key) {
                    uint64_t start_offset = _index_area->_index[mid_idx].offset, end_offset = 0;

                    /***********需要改进文件名存储方式**********/
                    const std::string filepath = this->filepath;
                    std::ifstream File(filepath, std::ios_base::in | std::ios_base::binary);
                    if (!File.is_open()) {
                        printf("File Not Exists!Something Wrong!");
                    }

                    if (mid_idx == _header->key_value_num - 1) {
                        File.seekg(0, std::ios_base::end);
                        end_offset = File.tellg();
                    } else {
                        end_offset = _index_area->_index[mid_idx + 1].offset;
                    }

                    char *line = (char*) malloc( (end_offset - start_offset) * sizeof (char) );

                    File.seekg(start_offset, std::ios_base::beg);
                    File.read(line, static_cast<int32_t>((end_offset - start_offset) * sizeof (char)));

                    std::string* res = new std::string(line, end_offset - start_offset); // 指针返回

                    /* 释放内存 */
                    free(line);

                    return res;
                }
                else if (_index_area->_index[mid_idx].key < key) left_idx = mid_idx + 1;
                else right_idx = mid_idx - 1;
            };
            return nullptr;
        }
        else {
            uint64_t idx = 0, right_idx = _header->key_value_num - 1;
            while (idx <= right_idx) {
                /****需要将offset与文件地址关联起来****/
                if (_index_area->_index[idx].key == key) {
                    uint64_t start_offset = _index_area->_index[idx].offset, end_offset = 0;

                    /***********需要改进文件名存储方式**********/
                    const std::string filepath = this->filepath;
                    std::ifstream File(filepath, std::ios_base::in | std::ios_base::binary);
                    if (!File.is_open()) {
                        printf("File Not Exists!Something Wrong!");
                    }

                    if (idx == _header->key_value_num - 1) {
                        File.seekg(0, std::ios_base::end);
                        end_offset = File.tellg();
                    } else {
                        end_offset = _index_area->_index[idx + 1].offset;
                    }

                    char *line = (char*) malloc( (end_offset - start_offset) * sizeof (char) );

                    File.seekg(start_offset, std::ios_base::beg);
                    File.read(line, static_cast<int32_t>((end_offset - start_offset) * sizeof (char)));

                    std::string* res = new std::string(line, end_offset - start_offset); // 指针返回

                    /* 释放内存 */
                    free(line);

                    return res;
                }
                else idx++;
            };
            return nullptr;
        }
    };
}

