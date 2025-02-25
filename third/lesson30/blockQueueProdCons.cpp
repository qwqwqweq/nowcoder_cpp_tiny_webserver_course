#include <iostream>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <chrono>
#include <unistd.h>

constexpr int TASK = 8;
//阻塞队列
template<typename T>
class BlockQueue {
private:
    std::queue<T> q;
    size_t max_size;
    std::mutex mtx;
    std::atomic<bool> stop_flag;
    std::condition_variable cv_cons;
    std::condition_variable cv_prod;

public:
    BlockQueue() : max_size(TASK), stop_flag(false) {}
    ~BlockQueue() {
        stop();
    }
    void stop() {
        stop_flag.store(true);
        cv_cons.notify_all();
        cv_prod.notify_all();
    }
    bool is_stop() {
        return stop_flag.load();
    }
    bool empty() {
        return q.empty();
    }
    bool full() {
        return max_size == q.size();
    }
    void push(int &x) {
        std::unique_lock<std::mutex> lck(mtx);
        while (full() && !is_stop()) {
            std::cout << "queue is full!" << std::endl;
            cv_cons.notify_one();
            cv_prod.wait(lck);
        }
        if (is_stop())
            return;
        q.push(x);
        cv_cons.notify_one();
    }
    void pop(int &x) {
        std::unique_lock<std::mutex> lck(mtx);
        while (empty() && !is_stop()) {
            std::cout << "queue is empty!" << std::endl;
            cv_prod.notify_one();
            cv_cons.wait(lck);
        }
        if (is_stop())
            return;
        x = q.front();
        q.pop();
        cv_prod.notify_one();
    }
};

int main() {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> rnd(0, 100);

    BlockQueue<int> bq;
    auto producer = [&]() {
        while (!bq.is_stop()) {
            int x = rnd(rng);
            bq.push(x);
            std::cout << "producer this thread id: " << std::this_thread::get_id() << " data: " << x << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    auto consumer = [&]() {
        while (!bq.is_stop()) {
            int x = -1;
            bq.pop(x);
            std::cout << "consumer this thread id: " << std::this_thread::get_id() << "data: " << x << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    std::vector<std::thread> prod, cons;
    for (int i = 0; i < 5; i++)
        prod.emplace_back(producer);
    
    for (int i = 0; i < 3; i++)
        cons.emplace_back(consumer);
    
    std::this_thread::sleep_for(std::chrono::seconds(10));
    bq.stop();

    for (auto &x : prod)
        x.join();

    for (auto &x : cons)
        x.join();

    return 0;
}