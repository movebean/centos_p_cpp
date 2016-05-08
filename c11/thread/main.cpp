#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <algorithm>

std::atomic<long long> total {0};

void func(int) {
  for (long long i = 0; i < 1000000LL; ++i) {
    total += i;
  }
}

void test1() {
  std::thread t1(func, 1);
  std::thread t2(func, 2);

  t1.join();
  t2.join();
  std::cout << total << std::endl;
}

std::atomic<int> a;
std::atomic<int> b;
void Thread1() {
  int t = 1;
  a.store(t, std::memory_order_relaxed);
  b.store(2, std::memory_order_release); //all write operation should be done before this
}

void Thread2() {
  while (b.load(std::memory_order_acquire) != 2); //after this operation then other can go
  std::cout << a.load(std::memory_order_relaxed) << std::endl;
}

void test2() {
  std::thread t1(Thread1);
  std::thread t2(Thread2);
  t1.join();
  t2.join();
}

void test3() {
    auto test_func = [] () {
        thread_local uint32_t var = 1;
        std::cout << var++ << std::endl;
    };

    std::vector<std::thread> threads;
    for (size_t t = 0; t < 10; ++t) {
        threads.push_back(std::thread([&test_func] () {
                    for (size_t i = 0; i < 5; ++i)
                        test_func();
                    }));
    }
    for_each (threads.begin(), threads.end(), [] (std::thread &t) {
        t.join();
        });
}

int main() {
  test1();
  test2();
  test3();
  return 0;
}
