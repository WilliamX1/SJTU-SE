#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>

using namespace std;

class Triple
{
private:
    int n;
    int cnt = 1;
    bool flag = true;
    std::condition_variable cond;
    std::mutex mtx;
public:
    Triple(int _n): n(_n) {};
    void zero() {
        while (cnt <= n) {
            if (flag) {
            cout << 0;
            flag = false;
            };
            cond.notify_one();
        }
    };
    void one() { 
        while (cnt <= n) {
            std::unique_lock<std::mutex> lock(mtx);
            if (flag) {
                cond.notify_one();
                cond.wait(lock);
            } else if (cnt % 3 == 1) {
                cout << cnt++;
                flag = true;
            } else {
                cond.notify_one();
                cond.wait(lock);
            }
        };
    };
    void two() {
        while (cnt <= n) {
            std::unique_lock<std::mutex> lock(mtx);
            if (flag) {
                cond.notify_one();
                cond.wait(lock);
            } else if (cnt % 3 == 2) {
                cout << cnt++;
                flag = true;
            } else {
                cond.notify_one();
                cond.wait(lock);
            }
        };
    };
    void three() {
        while (cnt <= n) {
            std::unique_lock<std::mutex> lock(mtx);
            if (flag) {
                cond.notify_one();
                cond.wait(lock);
            } else if (cnt % 3 == 0) {
                cout << cnt++;
                flag = true;
            } else {
                cond.notify_one();
                cond.wait(lock);
            }
        };
     };

};

int main() {

    Triple t(10);

    vector<thread> threads;

    threads.emplace_back([&](){t.zero();});

    threads.emplace_back([&](){t.one();});

    threads.emplace_back([&](){t.two();});

    threads.emplace_back([&](){t.three();});

    for (auto& thread: threads)
        thread.join();

    return 0;
}