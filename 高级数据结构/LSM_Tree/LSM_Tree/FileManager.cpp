#include <FileManager.h>

FileManager::FileManager(std::string _dir) : dir(_dir) {
    uint64_t x = 2;
    for (int i = 0; i < 63; i++) {
        fileLimit.push_back(x);
        x <<= 1;
    }
};

/* ./data/level-0 */
std::string FileManager::getFullSubdirpath(int64_t level) {
    return dir + '/' + getFullSubdirname(level);
}
/* ./data/level-9 */
std::string FileManager::getFullSubdirpath(std::string subdir) {
    return dir + '/' + subdir;
};
/* level-0 */
std::string FileManager::getFullSubdirname(int64_t level) {
    return subdirPrefix + std::to_string(level);
};
std::string FileManager::getFullFilename(int64_t timestamp, std::string addPost) {
    if (addPost == "") return filenamePrefix + std::to_string(timestamp) + filenameSuffix;
    else return filenamePrefix + std::to_string(timestamp) + '-' + addPost + filenameSuffix;
}


/**
 * get subdirs from the main dir
 * @param dir to be check
 * @return a vector include all subdirs in the dir
 */
std::vector<std::string> FileManager::getSubdirs(std::string dir) {
    std::vector<std::string> ans;
    utils::scanDir(dir, ans);
    return ans;
};

/**
 * get files from a subdir
 * @param subdir to be check
 * @return a vector include all filenames in the subdir
 */
std::vector<std::string> FileManager::getFiles(std::string subdir) {
    std::vector<std::string> ans;
    utils::scanDir(getFullSubdirpath(subdir), ans);
    return ans;
};

/**
 * finish compaction
 * @return a set with the changed filename to be affected
 */
void FileManager::compaction(std::set<std::string>& add_cache, std::set<std::string>& del_cache) {
    std::vector<std::string> subdirs;
    utils::scanDir(dir, subdirs);
    /* 按照文件夹名进行排序 */
    sort(subdirs.begin(), subdirs.end(), [](const std::string& A, const std::string& B){
        return atoi(A.substr(A.find('-') + 1).c_str()) < atoi(B.substr(B.find('-') + 1).c_str());
    });

    /* 清零 */
    add_cache.clear(); del_cache.clear();

    for (uint64_t i = 0; i < subdirs.size(); i++) {
        std::vector<std::string> files;
        utils::scanDir(getFullSubdirpath(subdirs[i]), files);

        if (files.size() <= this->fileLimit[i]) break;
        else {
            std::vector<SSTable*> compaction_sstables;
            if (i == 0) {
                compaction_sstables = getOneLevelFilesFromDisk(i, false);
            } else {
                std::vector<SSTable*> sstables = getOneLevelFilesFromDisk(i, false);
                /* 对缓存进行排序，优先选择时间戳最小的若干个文件（时间戳相等选择键最小的文件）*/
                std::sort(sstables.begin(), sstables.end(), [](const SSTable* A, const SSTable* B) {
                    if (A->_header->timestamp == B->_header->timestamp) return A->_header->key_min < B->_header->key_min;
                    else return A->_header->timestamp < B->_header->timestamp;
                });

                /* 获取多余的需要进行合并的sstables */
                while (sstables.size() > this->fileLimit[i]) {
                    auto cur = sstables.begin();
                    compaction_sstables.push_back(*cur);
                    sstables.erase(cur);
                };

                /* 释放sstables内存空间 */
                while (!sstables.empty()) {
                    auto del = sstables.back();
                    delete del; del = nullptr;
                    sstables.pop_back();
                }; std::vector<SSTable*>().swap(sstables);
            };

            /* 获取键值对 */
            std::vector<SSTable::KeyValueArrayNode> key_val_array_nodes;
            for (auto compaction_sstable : compaction_sstables) {
                std::vector<SSTable::KeyValueArrayNode> q = compaction_sstable->get_key_value_array(i);
                key_val_array_nodes.insert(key_val_array_nodes.end(), q.begin(), q.end());
            };

            /* 获取合并键值区间 */
            uint64_t key_min = compaction_sstables.front()->_header->key_min;
            uint64_t key_max = compaction_sstables.front()->_header->key_max;
            for (auto compaction_sstable : compaction_sstables) {
                key_min = std::min(key_min, compaction_sstable->_header->key_min);
                key_max = std::max(key_max, compaction_sstable->_header->key_max);
            };

            /* 释放compaction_sstables内存空间并加入相应文件名称 */
            while (!compaction_sstables.empty()) {
                auto del = compaction_sstables.back();
                del_cache.insert(del->filepath);
                utils::rmfile((*del).filepath.c_str());
                delete del; del = nullptr;
                compaction_sstables.pop_back();
            }; std::vector<SSTable*>().swap(compaction_sstables);

            /* 获取下一层有键值交集的文件进缓存 */
            std::vector<SSTable*> sstables = getOneLevelFilesFromDisk(i + 1, false);
            auto iter = sstables.begin();
            while (iter != sstables.end()) {
                if ((*iter)->_header->key_min <= key_max && (*iter)->_header->key_max >= key_min) {
                    compaction_sstables.push_back(*iter);
                    sstables.erase(iter);
                } else iter++;
            };
            /* 释放sstables内存空间 */
            while (!sstables.empty()) {
                auto del = sstables.back();
                delete del; del = nullptr;
                sstables.pop_back();
            }; std::vector<SSTable*>().swap(sstables);

            /* 获取键值对 */
            for (auto compaction_sstable : compaction_sstables) {
                std::vector<SSTable::KeyValueArrayNode> q = compaction_sstable->get_key_value_array(i);
                key_val_array_nodes.insert(key_val_array_nodes.end(), q.begin(), q.end());
            };
            /* 释放compaction_sstables内存空间并加入相应文件名称 */
            while (!compaction_sstables.empty()) {
                auto del = compaction_sstables.back();
                del_cache.insert(del->filepath);
                utils::rmfile((*del).filepath.c_str());
                delete del; del = nullptr;
                compaction_sstables.pop_back();
            }; std::vector<SSTable*>().swap(compaction_sstables);


            /* 对键值对排序 */
            std::sort(key_val_array_nodes.begin(), key_val_array_nodes.end(), [](const SSTable::KeyValueArrayNode& A,
                      const SSTable::KeyValueArrayNode& B) {
                if (A.key == B.key) {
                    if (A.timestamp == B.timestamp) return A.level < B.level;
                    else return A.timestamp > B.timestamp;
                }
                else return A.key < B.key;
            });


            /*创建新的SSTable并设置Header区域*/
            std::vector<SSTable::KeyValueArrayNode> cur_key_val_array_nodes;

            auto iter_create = key_val_array_nodes.begin();

            uint64_t size = 32 + 10240, key_value_num = 0, max_timestamp = 0;
            key_min = 0x7fffffffffffffff, key_max = 0;
            while (iter_create != key_val_array_nodes.end()) {
                if (size + (8 + 4) + (*iter_create).value.size() > this->max_size) {
                    SSTable* new_SSTable = new SSTable(max_timestamp, key_value_num, key_min, key_max);
                    /* 设置BloomFilter、IndexArea、DataArea*/
                    for (uint64_t i = 0; i < key_value_num; i++) {
                        new_SSTable->_bloom_filter->put(cur_key_val_array_nodes[i].key);
                        new_SSTable->_index_area->_index[i].key = cur_key_val_array_nodes[i].key;
                        new_SSTable->_index_area->_index[i].offset = 0;
                        new_SSTable->_data_area->_value.push_back(cur_key_val_array_nodes[i].value);
                    };
                    new_SSTable->filepath = this->writeFileToDisk(new_SSTable, max_timestamp, add_cache, del_cache, i + 1, false);
                    add_cache.insert(new_SSTable->filepath);

                    delete new_SSTable; new_SSTable = nullptr;
                    size = 32 + 10240, key_value_num = 0, max_timestamp = 0;
                    key_min = 0x7fffffffffffffff, key_max = 0;
                    std::vector<SSTable::KeyValueArrayNode>().swap(cur_key_val_array_nodes);
                };
                cur_key_val_array_nodes.push_back(*iter_create);
                size += 8 + 4 + (*iter_create).value.size();
                key_value_num++;
                max_timestamp = std::max(max_timestamp, iter_create->timestamp);

                key_min = std::min(key_min, iter_create->key);
                key_max = std::max(key_max, iter_create->key);
                iter_create++;
                while (iter_create != key_val_array_nodes.end()) {
                    if (iter_create->key == (iter_create - 1)->key) iter_create++;
                    else break;
                };
            }

            /* 将最后不足2MB的文件写回 */
            if (key_value_num != 0) {
                SSTable* new_SSTable = new SSTable(max_timestamp, key_value_num, key_min, key_max);
                /* 设置BloomFilter、IndexArea、DataArea*/
                for (uint64_t i = 0; i < key_value_num; i++) {
                    new_SSTable->_bloom_filter->put(cur_key_val_array_nodes[i].key);
                    new_SSTable->_index_area->_index[i].key = cur_key_val_array_nodes[i].key;
                    new_SSTable->_index_area->_index[i].offset = 0;
                    new_SSTable->_data_area->_value.push_back(cur_key_val_array_nodes[i].value);
                };
                new_SSTable->filepath = this->writeFileToDisk(new_SSTable, max_timestamp, add_cache, del_cache, i + 1, false);
                add_cache.insert(new_SSTable->filepath);

                delete new_SSTable; new_SSTable = nullptr;
                size = 32 + 10240, key_value_num = 0, max_timestamp = 0;
                key_min = 0x7fffffffffffffff, key_max = 0;
                std::vector<SSTable::KeyValueArrayNode>().swap(cur_key_val_array_nodes);
            };

            /* 释放key_value_nodes内存空间 */
            std::vector<SSTable::KeyValueArrayNode>().swap(key_val_array_nodes);
        };
        std::vector<std::string>().swap(files);
    }

    return;
};

/**
 * convert memtable to sstable
 * @param the memtable need to be convert
 * @param the timestamp
 * @return the converted sstable
 * 将内存跳表转换成硬盘SSTable形式*/
SSTable* FileManager::memTableToSSTable(MemTable* Memtable, uint64_t timestamp) {
    if (Memtable->empty()) return nullptr;
    /*创建新的SSTable并设置Header区域*/
    const uint64_t key_value_num = Memtable->get_key_value_num();
    const uint64_t key_min = Memtable->get_key_min();
    const uint64_t key_max = Memtable->get_key_max();
    SSTable* new_SSTable = new SSTable(timestamp, key_value_num, key_min, key_max);

    std::vector<uint64_t> key_arr;
    std::vector<std::string> val_arr;
    Memtable->get_key_value_array(key_arr, val_arr);

    /* 设置BloomFilter、IndexArea*/
    for (uint64_t i = 0; i < key_value_num; i++) {
        new_SSTable->_bloom_filter->put(key_arr[i]);
        new_SSTable->_index_area->_index[i].key = key_arr[i];
        new_SSTable->_index_area->_index[i].offset = 0; // 在真正写入硬盘时需要回改
    };
    /* 设置DataArea*/
    for (uint64_t i = 0; i < key_value_num; i++) {
        new_SSTable->_data_area->_value.push_back(val_arr[i]);
    };

    std::vector<uint64_t>().swap(key_arr);
    std::vector<std::string>().swap(val_arr);
    return new_SSTable;
}

/**
 * copy one file from original place to target place and delete original files
 * @param the original full filepath
 * @param the target subdir
 * @return the result full filepath
 */
std::string FileManager::cutFile(std::string originalFilepath, int64_t subdir) {
    copyFile(originalFilepath, subdir);
    std::string newFilepath = getFullSubdirpath(subdir) + originalFilepath.substr(originalFilepath.find_last_of('/') + 1);

    /* 删除原文件 */
    utils::rmfile(originalFilepath.c_str());

    return newFilepath;
}

/**
 * copy one file from original place to target place
 * @param the original full filepath
 * @param the target subdir
 * @return the result full filepath
 */
std::string FileManager::copyFile(std::string originalFilepath, int64_t subdir) {
    /* 原来文件读 */
    std::ifstream originalFile(originalFilepath, std::ios::out | std::ios::binary);

    /* 新文件写 */
    std::string newFilepath = getFullSubdirpath(subdir) + originalFilepath.substr(originalFilepath.find_last_of('/') + 1);
    std::ofstream newFile(newFilepath, std::ios::in | std::ios::binary);

    newFile << originalFile.rdbuf();
    originalFile.close();
    newFile.close();

    return newFilepath;
}


/**
 * according to the level number, make next subdir
 * @return the subdir made
 */
std::string FileManager::makeNextSubdir() {
    std::vector<std::string> existSubdirs = getSubdirs(dir);
    std::string existLastSubdir = existSubdirs.back();
    std::string nextSubdir = getFullSubdirpath(subdirPrefix + existLastSubdir.substr(existLastSubdir.find('-') + 1));
    utils::mkdir(nextSubdir.c_str());
    return nextSubdir;
};

/**
 * write file to level-0 subdir
 * @param a sstable ready to write
 * @param timestamp to write (already const)
 * @param a vector to get which levels need to evaluate the cache_MemTables
 * @param the level to write in
 * @param whether to compaction
 * @return the filepath
 */
std::string FileManager::writeFileToDisk(SSTable* sstable, uint64_t timestamp, std::set<std::string>& add_cache, std::set<std::string>& del_cache, uint64_t level, bool compaction_active) {
    if (sstable == nullptr)
        return "";
    /* 识别并创建新子文件夹Level-0 */
    if (!utils::dirExists(getFullSubdirpath(level))) utils::mkdir(getFullSubdirpath(level).c_str());

    /* ./data/Level-0/SSTable-timestamp */

    std::string filename = getFullFilename(timestamp);
    std::string filepath = getFullSubdirpath(level) + '/' + filename;

    /* check if the file exists, then add some post to the filename */
    std::vector<std::string> LevelFiles;
    utils::scanDir(getFullSubdirpath(level), LevelFiles);
    bool isRepeatedFilename;
    int64_t addPost = 1;
    /* 确保没有相同的文件名存在 */
    do {
        isRepeatedFilename = false;
        for (auto firstLevelFilename : LevelFiles) {
            if (firstLevelFilename == filename) {
                filename = getFullFilename(timestamp, std::to_string(addPost));
                filepath = getFullSubdirpath(level) + '/' + filename;
                addPost++;
                isRepeatedFilename = true;
                break;
            };
        };
    } while (isRepeatedFilename);

    std::ofstream File(filepath, std::ios::out | std::ios::binary);
    /* 以二进制方式写入文件 */
    if (!File.is_open()) {
        std::cout << "Error: fail to create file!" << std::endl;
        return "";
    }
    /* 写入header */
    File.write((char*)&sstable->_header->timestamp, sizeof(uint64_t));
    File.write((char*)&sstable->_header->key_value_num, sizeof(uint64_t));
    File.write((char*)&sstable->_header->key_min, sizeof(uint64_t));
    File.write((char*)&sstable->_header->key_max, sizeof(uint64_t));
    /* 写入bloom filter */
    for (auto i = 0; i < 4; i++)
        for (auto j = 0; j < 2560; j++)
            File.write((char*)&sstable->_bloom_filter->_hash_table[i][j], sizeof(uint8_t));
      /* 写入索引区 */
    uint32_t offset = 4 * 8 + 10240 + sstable->_header->key_value_num * 12;
    for (uint64_t i = 0; i < sstable->_header->key_value_num; i++)
    {
        File.write((char*)&sstable->_index_area->_index[i].key, sizeof(uint64_t));
        sstable->_index_area->_index[i].offset = offset;
        File.write((char*)&sstable->_index_area->_index[i].offset, sizeof(uint32_t));
        offset += sstable->_data_area->_value[i].size();
    }
      /* 写入数据区 */
    for (uint64_t i = 0; i < sstable->_header->key_value_num; i++)
    {
        std::string str = sstable->_data_area->_value[i];
        File << str;
    };
    /* 关闭文件 */
    File.close();

    /* 合并更新 */
    if (compaction_active) {
        /**************内存泄漏**************/
        compaction(add_cache, del_cache);
    }

    return filepath;
}

/**
 * get all files in one level stored in disk before
 * @param the level of subdir to work in, if -1: all levels, else corresponding level subdir
 * @return a vector of sstables including all stored data files in this level
 */
std::vector<SSTable*> FileManager::getOneLevelFilesFromDisk(int64_t level, bool emptyDataArea) {
    std::vector<SSTable*> ans;

    if (!utils::dirExists(getFullSubdirpath(level))) utils::mkdir(getFullSubdirpath(level).c_str());
    /* 只读取某一level */
    std::vector<std::string> levelFilenames;
    utils::scanDir(getFullSubdirpath(level), levelFilenames);
    for (auto levelFilename : levelFilenames) {
        /* ./data/level-0/SSTable-0 */
        std::string filepath = getFullSubdirpath(level) + '/' + levelFilename;
        auto new_SSTable = this->getOneFileFromDisk(filepath, emptyDataArea);
        /*生成的SSTable进缓存*/
        ans.push_back(new_SSTable);
    };

    return ans;
};


SSTable* FileManager::getOneFileFromDisk(std::string filepath, bool emptyDataArea) {
    std::ifstream File(filepath, std::ios::in | std::ios::binary);
    if (!File.is_open()) return nullptr;
    /*读取header信息*/
    uint64_t cur_timestamp, cur_key_value_num, cur_key_min, cur_key_max;
    File.read((char*)&cur_timestamp, sizeof (uint64_t));
    File.read((char*)&cur_key_value_num, sizeof (uint64_t));
    File.read((char*)&cur_key_min, sizeof (uint64_t));
    File.read((char*)&cur_key_max, sizeof (uint64_t));
    /*创建SSTable*/
    SSTable* new_SSTable = new SSTable(cur_timestamp, cur_key_value_num, cur_key_min, cur_key_max);
    new_SSTable->filepath = filepath;
    /*读取bloom filter信息*/
    for (auto i = 0; i < 4; i++)
        for (auto j = 0; j < 2560; j++)
            File.read((char*)&new_SSTable->_bloom_filter->_hash_table[i][j], sizeof (uint8_t));
    /*读取索引区*/
    for (uint64_t i = 0; i < new_SSTable->_header->key_value_num; i++)
    {
        File.read((char*)&new_SSTable->_index_area->_index[i].key, sizeof (uint64_t));
        File.read((char*)&new_SSTable->_index_area->_index[i].offset, sizeof (uint32_t));
    };
    /*读取数据区*/
    if (emptyDataArea == true) new_SSTable->release_dataArea();
    else {
        new_SSTable->_data_area->_value.clear();
        for (uint64_t i = 0; i < new_SSTable->_header->key_value_num; i++) {
            uint64_t start_offset = new_SSTable->_index_area->_index[i].offset;
            uint64_t end_offset = 0;
            if (i != new_SSTable->_header->key_value_num - 1) {
                end_offset = new_SSTable->_index_area->_index[i + 1].offset;
            } else { /* 单独处理最后一个文件末尾字符串 */
                File.seekg(0, std::ios_base::end);
                end_offset = File.tellg();
            };
            char *line = new char [end_offset - start_offset];
            File.seekg(start_offset, std::ios_base::beg);
            File.read(line, static_cast<int32_t>((end_offset - start_offset) * sizeof (char)));
            std::string* res = new std::string(line, end_offset - start_offset);

            if (line != nullptr) {
                delete[] line;
                line = nullptr;
            }

            new_SSTable->_data_area->_value.push_back(*res);

            if (res) {
                delete res;
                res = nullptr;
            }
        };
    }
    /*关闭文件*/
    File.close();
    return new_SSTable;
}
/**
 * get all files stored in disk before
 * @return a vector of sstables including all stored data files
 */
std::vector<std::vector<SSTable*>> FileManager::getAllLevelFilesFromDisk(bool emptyDataArea) {
    std::vector<std::string> sstableNames;
    utils::scanDir(dir, sstableNames);
    int64_t size = sstableNames.size();

    std::vector<std::vector<SSTable*>> ans;
    for (int64_t i = 0; i < size; i++) {
        ans.push_back(getOneLevelFilesFromDisk(i, emptyDataArea));
    };
    return ans;
};
