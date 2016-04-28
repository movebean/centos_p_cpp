#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>

void test() {
  std::thread t([] () {
      std::cout << "Hello concurrent thread\n";
      });
  t.join();
}

class thread_guard {
  std::thread &_t;
  public:
  explicit thread_guard(std::thread &t):
    _t(t) {}
  ~thread_guard() {
    if (_t.joinable()) {
      _t.join();
    }
  }
  thread_guard(const thread_guard &) = delete;
  thread_guard &operator=(const thread_guard &) = delete;
};

void test1() {
  std::thread t([] () {
      std::cout << "Hello thread guard\n";
      });
  thread_guard tg(t);
  std::cout << "I can do something else\n";
}

void test2() {
  auto double_string =
    [] (std::string s) -> void {
    s = s + s;
  };
  std::string me = "YoYo";
  std::thread t(double_string, me);
  t.join();
  std::cout << me << std::endl;

  auto double_string_ref =
    [] (std::string &s) -> void {
    s = s + s;
  };
  std::string me2 = "YoYo";
  std::thread t2(double_string_ref, std::ref(me2));
  t2.join();
  std::cout << me2 << std::endl;
}

void test3() {
  std::thread t1([] () {
      std::cout << "hello there" << std::endl;
      });
  //std::thread t2 = t1;     // error
  std::thread t2 = std::move(t1);
  t2.join();

  t1 = std::thread([] () {
      std::cout << "hello again" << std::endl;
      });
  t1.join();

  std::thread t3([] () {
      std::cout << "I'm lucky" << std::endl;
      });
  //t3 = std::move(t1);   //error, t3 will call terminate due to previous thread destroy
  t3.join();
}

void test4() {
  class scoped_thread {
    private:
      std::thread _t;
    public:
      explicit scoped_thread(std::thread t):
        _t(std::move(t)) {
          if (!_t.joinable())
            throw std::logic_error("No thread");
        }
      ~scoped_thread() {
        _t.join();
      }
      scoped_thread(const scoped_thread &) = delete;
      scoped_thread &operator=(const scoped_thread &) = delete;
  };

  std::thread t([] () {
    std::cout << "scoped thread\n";
  });
  scoped_thread st(std::move(t));
}

void test5() {
  auto work = [] (int i) {
    std::cout << i << ' ';
  };
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 20; ++i) {
    threads.push_back(std::thread(work, i));
  }
  std::for_each(threads.begin(), threads.end(), [] (std::thread &t) {
      t.join();
      });
  std::cout << '\n' << std::thread::hardware_concurrency() << std::endl;
}

void test6() {
  std::mutex m;
  int result = 0;
  auto work = [&] () {
    std::lock_guard<std::mutex> lock(m);
    for (size_t i = 0; i < 1000000; ++i, ++result);
  };
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 20; ++i) {
    threads.push_back(std::thread(work));
  }
  std::for_each(threads.begin(), threads.end(), [] (std::thread &t) {
      t.join();
      });
  std::cout << "result:" << result << std::endl;
}

void test7() {
  auto work1 = [] (std::mutex &m1, std::mutex &m2) {
    std::lock(m1, m2);
    std::lock_guard<std::mutex> _lk1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> _lk2(m2, std::adopt_lock);
    std::cout << "Try lock m1 m2" << std::endl;
  };
  auto work2 = [] (std::mutex &m1, std::mutex &m2) {
    std::lock(m2, m1);
    std::lock_guard<std::mutex> _lk2(m2, std::adopt_lock);
    std::lock_guard<std::mutex> _lk1(m1, std::adopt_lock);
    std::cout << "Try lock m2 m1" << std::endl;
  };
  std::mutex m1, m2;
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 5; ++i) {
    threads.push_back(std::thread(work1, std::ref(m1), std::ref(m2)));
    threads.push_back(std::thread(work2, std::ref(m1), std::ref(m2)));
  }
  for_each (threads.begin(), threads.end(), [] (std::thread &t) {
      t.join();
      });
}

int main() {
  test();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  return 0;
}
