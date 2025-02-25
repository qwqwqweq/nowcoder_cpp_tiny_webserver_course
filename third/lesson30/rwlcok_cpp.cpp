#include <iostream>
#include <thread>
#include <format>
#include <vector>
#include <mutex>
#include <chrono>
#include <random>
#include <shared_mutex>

std::shared_mutex rw_lock;
int num = 1;

int main() {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> rnd(0, 1);
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++)
        threads.push_back(std::thread([&]() {
            while (true) {
                std::unique_lock<std::shared_mutex> lock(rw_lock);
                num++;
                std::cout << std::format("++write, tid: {}, num: {}\n", std::hash<std::thread::id>{}(std::this_thread::get_id()), num);
                std::this_thread::sleep_for(std::chrono::milliseconds(rnd(rng)));
            }
        }));
    
    for (int i = 0; i < 5; i++)
        threads.push_back(std::thread([&]() {
            while (true) {
                std::shared_lock<std::shared_mutex> lock(rw_lock);
                std::cout << std::format("===read, tid: {}, num: {}\n", std::hash<std::thread::id>{}(std::this_thread::get_id()), num);
                std::this_thread::sleep_for(std::chrono::milliseconds(rnd(rng)));
            }
        }));

    for (auto &t : threads)
        t.join();

    return 0;
}