#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include "SSTable.h"
#include "MemTable.h"
#include "utils.h"

class FileManager
{
public:
    std::string dir; // 数据存放主文件夹 ex: './data'

    const std::string subdirPrefix = "Level-"; // 文件夹前缀

    const std::string filenamePrefix = "SSTable-"; // 文件名前缀

    const std::string filenameSuffix = ".ssh"; // 文件名后缀

    std::vector<uint64_t> fileLimit; // 子文件夹最大文件限制

    uint64_t max_size = 2 * 1024 * 1024; // 最大不能超过2MB

    FileManager(std::string _dir);

    /* ./data/level-0 */
    std::string getFullSubdirpath(int64_t level);
    /* ./data/level-9 */
    std::string getFullSubdirpath(std::string subdir);
    /* level-0 */
    std::string getFullSubdirname(int64_t level);
    std::string getFullFilename(int64_t timestamp, std::string addPost = "");

    /**
     * get subdirs from the main dir
     * @param dir to be check
     * @return a vector include all subdirs in the dir
     */
    std::vector<std::string> getSubdirs(std::string dir);

    /**
     * get files from a subdir
     * @param subdir to be check
     * @return a vector include all filenames in the subdir
     */
    std::vector<std::string> getFiles(std::string subdir);

    /**
     * finish compaction
     * @return a vector with the subdirlevels to be affected
     */
    void compaction(std::set<std::string>& add_cache, std::set<std::string>& del_cache);

    /**
     * convert memtable to sstable
     * @param the memtable need to be convert
     * @param the timestamp
     * @return the converted sstable
     * 将内存跳表转换成硬盘SSTable形式*/
    SSTable* memTableToSSTable(MemTable* Memtable, uint64_t timestamp);

    /**
     * copy one file from original place to target place and delete original files
     * @param the original full filepath
     * @param the target subdir
     * @return the result full filepath
     */
    std::string cutFile(std::string originalFilepath, int64_t subdir);

    /**
     * copy one file from original place to target place
     * @param the original full filepath
     * @param the target subdir
     * @return the result full filepath
     */
    std::string copyFile(std::string originalFilepath, int64_t subdir);

    /**
     * according to the level number, make next subdir
     * @return the subdir made
     */
    std::string makeNextSubdir();

    /**
     * write file to level-0 subdir
     * @param a sstable ready to write
     * @param timestamp to write (already const)
     * @param a vector to get which levels need to evaluate the cache_MemTables
     * @param the level to write in
     * @param whether to compaction
     * @return the filepath
     */
    std::string writeFileToDisk(SSTable* sstable, uint64_t timestamp, std::set<std::string>& add_cache, std::set<std::string>& del_cache, uint64_t level, bool compaction_active);

    /**
     * get all files in one level stored in disk before
     * @param the level of subdir to work in, if -1: all levels, else corresponding level subdir
     * @return a vector of sstables including all stored data files in this level
     */
    std::vector<SSTable*> getOneLevelFilesFromDisk(int64_t level, bool emptyDataArea);

    SSTable* getOneFileFromDisk(std::string filepath, bool emptyDataArea);

    /**
     * get all files stored in disk before
     * @return a vector of sstables including all stored data files
     */
    std::vector<std::vector<SSTable*>> getAllLevelFilesFromDisk(bool emptyDataArea);
};

#endif // FILEMANAGER_H
