#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "kvstore_api.h"
#include "SSTable.h"
#include "MemTable.h"
#include "FileManager.h"
#include <vector>

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    uint64_t timestamp; //时间戳
    /* 缓存的SSTable */
    std::vector<std::vector<SSTable*>> cache_MemTable;
    /* 当前的内存存储 */
    MemTable* cur_MemTable; //硬盘存储：Sorted String Table, 目前在内存中存放
    /* 文件管理及合并器 */
    FileManager* fileManager;
    /*将内存跳表转换成硬盘SSTable形式*/
    SSTable* _MemTable_to_SSTable() ;

    uint64_t max_size = 2 * 1024 * 1024; // 最大不能超过2MB
    const std::string dir; //写入文件夹
public:
    KVStore(const std::string &dir);

    ~KVStore() override;

    void init();

    void finish();

    void put(uint64_t key, const std::string &s) override;

    std::string get(uint64_t key) override;

    bool del(uint64_t key) override;

	void reset() override;

    std::string write_to_disk(SSTable* _SSTable, std::set<std::string>& add_cache, std::set<std::string>& del_cache);

    void get_from_disk();

    void evaluate_cache_MemTable(std::set<std::string>& add_cache, std::set<std::string>& del_cache);

    void memTable_to_SSTable();
};
