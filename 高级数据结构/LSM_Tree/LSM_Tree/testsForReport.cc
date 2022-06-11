//#include <iostream>
//#include <chrono>
//#include <string>
//#include <cstdint>

//#include "kvstore.h"

//// #define test1_1
//// #define test1_2
//// #define test2
//// #define test3

//int main()
//{
//    KVStore kvStore("./data");

//#ifdef test1_1
//    // 1) regular test1
//    kvStore.reset();
//    std::cout << "REGULAR TEST1:" << std::endl;
//    const uint64_t cycleNum = 1024 * 64;
//    const uint64_t valueSize1 = 1024;
//    std::string insertValue1(valueSize1, 's');
//    std::cout << "Value Size: " << std::to_string(valueSize1) << std::endl;

//    auto start1 = std::chrono::steady_clock::now();
//    for (uint64_t i = 0; i < cycleNum; ++i) {
//        kvStore.put(i, insertValue1);
//    }
//    auto end1 = std::chrono::steady_clock::now();
//    std::cout << "Total time for " + std::to_string(cycleNum) + " PUTs: "
//              << std::chrono::duration_cast <std::chrono::milliseconds> (end1 - start1) .count()
//              << "ms. "
//              << double(std::chrono::duration_cast <std::chrono::milliseconds> (end1 - start1) .count()) * 1000 / double(cycleNum)
//              << "us. "
//              << std::endl;

//    start1 = std::chrono::steady_clock::now();
//    for (uint64_t i = 0; i < cycleNum; ++i) {
//        kvStore.get(i);
//    }
//    end1 = std::chrono::steady_clock::now();
//    std::cout << "Total time for " + std::to_string(cycleNum) + " GETs: "
//              << std::chrono::duration_cast <std::chrono::milliseconds> (end1 - start1) .count()
//              << "ms. "
//              << double(std::chrono::duration_cast <std::chrono::milliseconds> (end1 - start1) .count()) * 1000 / double(cycleNum)
//              << "us. "
//              << std::endl;

//    start1 = std::chrono::steady_clock::now();
//    for (uint64_t i = 0; i < cycleNum; i += 1) {
//        kvStore.del(i);
//    }
//    end1 = std::chrono::steady_clock::now();
//    std::cout << "Total time for " + std::to_string(cycleNum) + " DELs: "
//              << std::chrono::duration_cast <std::chrono::milliseconds> (end1 - start1) .count()
//              << "ms. "
//              << double(std::chrono::duration_cast <std::chrono::milliseconds> (end1 - start1) .count()) * 1000 / double(cycleNum)
//              << "us. "
//              << std::endl;
//#endif

//#ifdef test1_2
//    // 1) regular test2
//    kvStore.reset();
//    std::cout << "REGULAR TEST2:" << std::endl;
//    const uint64_t cycleNum1_ = 1024 * 128;
//    const uint64_t valueSize1_ = 1024 * 4;
//    std::string insertValue1_(valueSize1_, 's');
//    std::cout << "Value Size: " << std::to_string(valueSize1_) << std::endl;

//    std::cout << "Test PUTs ..." << std::endl;
//    std::vector<uint64_t> res1_;
//    auto start1_ = std::chrono::steady_clock::now();
//    for (uint64_t i = 0; i < cycleNum1_; ++i) {
//        auto end1_ = std::chrono::steady_clock::now();
//        if (std::chrono::duration_cast <std::chrono::seconds> (end1_ - start1_) .count() >= 1) {
//            res1_.emplace_back(i);
//            start1_ = std::chrono::steady_clock::now();
//        }
//        kvStore.put(i, insertValue1_);
//    }
//    for (uint64_t i = 0; i < res1_.size(); ++i) {
//        std::cout << "Total PUT amount for stage " + std::to_string(i) + ": " << res1_[i] << std::endl;
//    }

//    std::cout << "Test GETs ..." << std::endl;
//    res1_.clear();
//    start1_ = std::chrono::steady_clock::now();
//    for (uint64_t i = 0; i < cycleNum1_; ++i) {
//        auto end1_ = std::chrono::steady_clock::now();
//        if (std::chrono::duration_cast <std::chrono::seconds> (end1_ - start1_) .count() >= 1) {
//            res1_.emplace_back(i);
//            start1_ = std::chrono::steady_clock::now();
//        }
//        kvStore.get(i);
//    }
//    for (uint64_t i = 0; i < res1_.size(); ++i) {
//        std::cout << "Total GET amount for stage " + std::to_string(i) + ": " << res1_[i] << std::endl;
//    }

//    std::cout << "Test DELs ..." << std::endl;
//    res1_.clear();
//    start1_ = std::chrono::steady_clock::now();
//    for (uint64_t i = 0; i < cycleNum1_; i += 4) {
//        auto end1_ = std::chrono::steady_clock::now();
//        if (std::chrono::duration_cast <std::chrono::seconds> (end1_ - start1_) .count() >= 1) {
//            res1_.emplace_back(i / 4);
//            start1_ = std::chrono::steady_clock::now();
//        }
//        kvStore.del(i);
//    }
//    for (uint64_t i = 0; i < res1_.size(); ++i) {
//        std::cout << "Total DEL amount for stage " + std::to_string(i) + ": " << res1_[i] << std::endl;
//    }
//#endif

//#ifdef test2
//    kvStore.reset();
//    std::cout << "GET TEST:" << std::endl;
//    const uint64_t cycleNum2 = 1024 * 64;
//    const uint64_t valueSize2 = 4096;
//    std::string insertValue2(valueSize2, 's');

//    std::cout << "PUTs something for test ..." << std::endl;
//    for (uint64_t i = 0; i < cycleNum2; ++i) {
//        kvStore.put(i, insertValue2);
//    }

//    std::cout << "PUTs finished. Start GETs ..." << std::endl;
//    auto start2 = std::chrono::steady_clock::now();
//    for (uint64_t i = 0; i < cycleNum2; ++i) {
//        kvStore.get(i);
//    }
//    auto end2 = std::chrono::steady_clock::now();
//    auto count2 = std::chrono::duration_cast <std::chrono::milliseconds> (end2 - start2) .count();
//    std::cout << "Total time for " + std::to_string(cycleNum2) + " GETs: "
//              << count2
//              << "ms."
//              << std::endl;
//    std::cout << "Average latency: " << (double) count2 / cycleNum2 << "ms." << std::endl;
//#endif

//#ifdef test3
//    // 3) test3
//    kvStore.reset();
//    std::cout << "COMPACTION TEST:" << std::endl;
//    const uint64_t cycleNum3 = 1024 * 128;
//    const uint64_t valueSize3 = 9192;
//    std::string insertValue3(valueSize3, 's');
//    std::cout << "Value Size: " << std::to_string(valueSize3) << std::endl;

//    std::cout << "Test PUTs ..." << std::endl;
//    std::vector<uint64_t> res3;
//    auto gap3 = std::chrono::duration<long long, std::micro>(1000000);
//    auto start3 = std::chrono::steady_clock::now();
//    uint64_t cnt = 0;
//    for (uint64_t i = 0; i < cycleNum3; ++i, ++cnt) {
//        kvStore.put(i, insertValue3);
//        auto end3 = std::chrono::steady_clock::now();
//        if (std::chrono::duration_cast <std::chrono::microseconds> (end3 - start3) >= gap3) {
//            res3.emplace_back(cnt);
//            start3 = std::chrono::steady_clock::now();
//            cnt = 0;
//        }
//    }
//    for (auto it: res3) {
//        std::cout << it << ", ";
//    }
//    std::cout << std::endl;

//#endif

//    return 0;
//}
