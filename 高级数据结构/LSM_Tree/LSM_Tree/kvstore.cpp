#include "kvstore.h"
#include "utils.h"

KVStore::KVStore(const std::string &dir): KVStoreAPI(dir), dir(dir) {
    init();
};

KVStore::~KVStore()
{
    finish();
    return;
}

void KVStore::init() {
    /* 创建文件管理器 */
    fileManager = new FileManager(dir);

    if (!utils::dirExists(dir)) utils::mkdir(dir.c_str());
    /*从硬盘中获取缓存并恢复时间戳*/
    timestamp = 0;
    this->get_from_disk();
    /* 创建当前内存跳表 */
    cur_MemTable = new MemTable();
}
void KVStore::finish() {
    /*将SSTable写回硬盘，SSTable释放内存*/
    memTable_to_SSTable(); //转换成SSTable形式

    /* cache_MemTable释放内存 */
    uint64_t m = cache_MemTable.size();
    for (uint64_t i = 0; i < m; i++) {
        uint64_t n = cache_MemTable[i].size();
        for (uint64_t j = 0; j < n; j++)
            if (cache_MemTable[i][j]) {
                delete cache_MemTable[i][j];
                cache_MemTable[i][j] = nullptr;
            }
        cache_MemTable[i].clear();
    };
    cache_MemTable.clear();

    /*释放FileManager*/
    if (fileManager) {
        delete fileManager;
        fileManager = nullptr;
    }

    /* 释放当前内存cur_MemTable*/
    if (cur_MemTable) {
        delete cur_MemTable;
        cur_MemTable = nullptr;
    }
}
void KVStore::memTable_to_SSTable() {
    SSTable* new_SSTable = _MemTable_to_SSTable(); //转换成SSTable形式，需要存在内存中的
    if (new_SSTable != nullptr) {
        std::set<std::string> add_cache, del_cache;
        new_SSTable->filepath = this->write_to_disk(new_SSTable, add_cache, del_cache); //写入硬盘
        new_SSTable->release_dataArea(); //释放数据区空间

        if (this->cache_MemTable.empty()) this->cache_MemTable.push_back(std::vector<SSTable*>());
        this->cache_MemTable[0].push_back(new_SSTable); //缓存至内存中

        if (cur_MemTable) {
            delete cur_MemTable;
            cur_MemTable = nullptr;
        }
        cur_MemTable = new MemTable(); //清空并重置现存内存表
        /* 重新调整缓存 */
        this->evaluate_cache_MemTable(add_cache, del_cache);
    }
    return;
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    std::string* origin_val = this->cur_MemTable->get(key);
    int64_t origin_size = origin_val == nullptr ? 0 : origin_val->size();

    //如果当前跳表大小再加上一个元素的key、offset和value大小超过最大容量，则进行转换
    if (this->cur_MemTable->getSize() + (8 + 4) + s.size() - origin_size > this->max_size) {
        memTable_to_SSTable();
    }; /*先将_MemTable中数据转换成SSTable保存在Level0中...再进行键值插入*/
    /* 将值插入 */
    this->cur_MemTable->put(key, s);

    if (origin_val) delete origin_val;
    return;
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    std::string* res = cur_MemTable->get(key); //先到现存内存中查找

    if (res == nullptr) {
        uint64_t timestamp = 0;
        std::string* find_str = nullptr;

        //如果内存找不到，则到缓存中查找
        for (auto sstables : cache_MemTable) {
            for (auto sstable : sstables) {
                /**
                 * 正确性基于：顶层的timestamp总是大于底层
                 * 在某一层缓存找出所有相关的，并去硬盘中查找并返回
                 */
                find_str = sstable->get(key);
                if (find_str != nullptr) {
                    if (sstable->_header->timestamp > timestamp) {
                        timestamp = sstable->_header->timestamp;

                        std::string* del = res;
                        res = find_str;
                        if (del) delete del;
                        del = nullptr;
                    } else {
                        if (find_str) delete find_str;
                        find_str = nullptr;
                    };
                }
            };
            if (res != nullptr) break; // 提前退出，可以优化时间性能
        }
    }
    if (res == nullptr) return "";
    else if ((*res) == "~DELETED~") {
        delete res; res = nullptr;
        return "";
    }
    else {
        std::string ans = *res;
        delete res; res = nullptr;
        return ans;
    }
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    if (this->get(key) == "")
        return false;
    else {
        this->put(key, "~DELETED~");
        return true;
    }
};

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    finish();
    /*重置KVStore*/
    std::vector<std::string> subdirs;
    utils::scanDir(dir, subdirs);

    for (auto subdir : subdirs) {
        std::vector<std::string> filenames;
        utils::scanDir(dir + "/" + subdir, filenames);
        for (auto filename: filenames)
            utils::rmfile((dir + "/" + subdir + "/" + filename).c_str());
        utils::rmdir((dir + "/" + subdir).c_str());
    };
    utils::rmdir(dir.c_str());
    utils::mkdir(dir.c_str());

    init();
    return;
}

/*将内存跳表形式转换成硬盘中SSTable形式*/
SSTable* KVStore::_MemTable_to_SSTable()
{
    return fileManager->memTableToSSTable(this->cur_MemTable, ++this->timestamp);
};
/*将转换后的SSTable写回硬盘*/

/* 返回文件名 */
std::string KVStore::write_to_disk(SSTable* _SSTable, std::set<std::string>& add_cache, std::set<std::string>& del_cache) {
    return this->fileManager->writeFileToDisk(_SSTable, this->timestamp, add_cache, del_cache, 0, true);
}

/*从硬盘中缓存数据至内存*/
void KVStore::get_from_disk() {
    this->cache_MemTable = this->fileManager->getAllLevelFilesFromDisk(true);
    return;
};

/* 因为触发合并而更新对应level中缓存*/
void KVStore::evaluate_cache_MemTable(std::set<std::string>& add_cache, std::set<std::string>& del_cache) {
    auto iter = del_cache.begin();
    while (iter != del_cache.end()) {
        if (add_cache.find(*iter) != add_cache.end()) {
            iter++;
            continue;
        }
        std::string filepath = (*iter);
        uint64_t start = filepath.find_first_of('-') + 1, end = filepath.find_last_of('/');
        uint64_t level = std::atoi(filepath.substr(start, end - start).c_str());
        while (this->cache_MemTable.size() <= level) {
            this->cache_MemTable.push_back(std::vector<SSTable*>());
        };

//        std::cout << "Del: filepath: " << filepath
//                  << " level: " << level << std::endl;
        /* 删除原有缓存 */
        auto iter_evaluate = this->cache_MemTable[level].begin();
        while (iter_evaluate != this->cache_MemTable[level].end()) {
            if ((*iter_evaluate)->filepath == filepath) {
                delete *iter_evaluate;
                this->cache_MemTable[level].erase(iter_evaluate);
                break;
            } else iter_evaluate++;
        };
        utils::rmfile((filepath).c_str());
        iter++;
    };
    std::set<std::string>().swap(del_cache);

    iter = add_cache.begin();
    while (iter != add_cache.end()) {
        std::string filepath = (*iter);
        uint64_t start = filepath.find_first_of('-') + 1, end = filepath.find_last_of('/');
        uint64_t level = std::atoi(filepath.substr(start, end - start).c_str());
        while (this->cache_MemTable.size() <= level) {
            this->cache_MemTable.push_back(std::vector<SSTable*>());
        }

//        std::cout << "Add: filepath: " << filepath
//                  << " level: " << level << std::endl;
        /* 删除原有缓存 */
        auto iter_evaluate = this->cache_MemTable[level].begin();
        while (iter_evaluate != this->cache_MemTable[level].end()) {
            if ((*iter_evaluate)->filepath == filepath) {
                delete *iter_evaluate;
                this->cache_MemTable[level].erase(iter_evaluate);
                break;
            } else iter_evaluate++;
        };

        SSTable* new_SSTable = this->fileManager->getOneFileFromDisk(filepath, true);

        if (new_SSTable) this->cache_MemTable[level].push_back(new_SSTable);
        iter++;
    }
    std::set<std::string>().swap(add_cache);
    return;
};
