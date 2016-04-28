#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <list>
#include <condition_variable>
#include <future>
#include <stack.hpp>
#include <queue.hpp>
#include <map.hpp>

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
  auto work3 = [] (std::mutex &m1, std::mutex &m2) {
    std::unique_lock<std::mutex> _lk1(m1, std::defer_lock);
    std::unique_lock<std::mutex> _lk2(m2, std::defer_lock);
    std::lock(_lk1, _lk2);
    std::cout << "Try with unique_lock" << std::endl;
  };
  std::mutex m1, m2;
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 5; ++i) {
    threads.push_back(std::thread(work1, std::ref(m1), std::ref(m2)));
    threads.push_back(std::thread(work2, std::ref(m1), std::ref(m2)));
    threads.push_back(std::thread(work3, std::ref(m1), std::ref(m2)));
  }
  for_each (threads.begin(), threads.end(), [] (std::thread &t) {
      t.join();
      });
}

class hierarchical_mutex {
  private:
    std::mutex _internal_mutex;
    unsigned long const _value;
    unsigned long _previous_value;
    static thread_local unsigned long this_thread_value;

    void check_for_hierarchy_violation() {
      if (this_thread_value <= _value) {
        throw std::logic_error("mutex value violated");
      }
    }

    void update_hierarchy_value() {
      _previous_value = this_thread_value;
      this_thread_value = _value;
    }

  public:
    explicit hierarchical_mutex(unsigned long value):
      _value(value), _previous_value(0) {}

    void lock() {
      check_for_hierarchy_violation();
      _internal_mutex.lock();
      update_hierarchy_value();
    }

    void unlock() {
      this_thread_value = _previous_value;
      _internal_mutex.unlock();
    }

    bool try_lock() {
      check_for_hierarchy_violation();
      if (!_internal_mutex.try_lock())
        return false;
      update_hierarchy_value();
      return true;
    }
};

void test8() {
  auto work = [] (std::mutex &m) {
    std::unique_lock<std::mutex> _lk_out;
    std::cout << "no sequence" << std::endl;
    for (size_t i = 0; i < 5000000; ++i) ;
    {
      std::unique_lock<std::mutex> _lk_inner(m);
      std::cout << "step one" << std::endl;
      _lk_out = std::move(_lk_inner);
    }
    std::cout << "step two" << std::endl;
  };

  std::mutex m;
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 5; ++i) {
    threads.push_back(std::thread(work, std::ref(m)));
  }
  for_each (threads.begin(), threads.end(), [] (std::thread &t) {
      t.join();
      });

  class gateway {
    private:
      std::mutex _mtx;
      uint32_t _resource {0};
    public:
      std::unique_lock<std::mutex> get_lock() {
        std::unique_lock<std::mutex> _lk(_mtx);
        return _lk;
      }
      void inc() {
        ++_resource;
      }
      uint32_t get_value() {
        return _resource;
      }
  };

  gateway resource;
  threads.clear();
  for (size_t i = 0; i < 5; ++i) {
    threads.push_back(std::thread([&resource] () {
          std::unique_lock<std::mutex> _lk(resource.get_lock());
          for (size_t i = 0; i < 100000; ++i) {
          resource.inc();
          }
          }));
  }
  for_each (threads.begin(), threads.end(), [] (std::thread &t) {
      t.join();
      });
  std::cout << resource.get_value() << std::endl;
}
class InstDemo {
  public:
    typedef std::shared_ptr<InstDemo> Ptr;
  private:
    static Ptr _inst;
    static std::once_flag _inst_flag;
  public:
    static Ptr GetInstance() {
      std::call_once(_inst_flag, init);
      return _inst;
    }
  private:
    InstDemo() {};
    static void init() {
      _inst = Ptr(new InstDemo);
    }
};

InstDemo::Ptr InstDemo::_inst;
std::once_flag InstDemo::_inst_flag;

void test9() {
  std::list<int> resources;
  std::mutex mtx;
  std::condition_variable cd;
  bool no_more = false;

  auto producer = [&] {
    for (size_t i = 0; i < 10000; ++i) {
      std::unique_lock<std::mutex> lk(mtx);
      resources.push_back(i);
      if (resources.size() == 1) {
        cd.notify_one();
      }
    }
    no_more = true;
  };

  auto consumer = [&] (uint32_t idx) {
    while (true) {
      std::unique_lock<std::mutex> lk(mtx);
      if (no_more && resources.empty())
        break;
      if (resources.empty()) {
        cd.wait(lk, [no_more, &resources] { return no_more || !resources.empty(); });
      }
      size_t i = resources.front();
      resources.pop_front();
      lk.unlock();
      if (i % 1000 == 0) {
        std::cout << idx << ':' << i << std::endl;
      }
    }
  };

  std::vector<std::thread> threads;
  threads.push_back(std::thread(producer));
  threads.push_back(std::thread(consumer, 1));
  threads.push_back(std::thread(consumer, 2));
  threads.push_back(std::thread(consumer, 3));

  for_each (threads.begin(), threads.end(), [] (std::thread &t) {
      t.join();
      });
}

void test10() {
  auto build_value = [] () -> int {
    std::cout << "inner" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 100;
  };

  //std::future<int> get_value = std::async(std::launch::deferred, build_value);
  std::future<int> get_value = std::async(std::launch::async, build_value);
  while (get_value.wait_for(std::chrono::seconds(1)) != std::future_status::ready) {
                                                   //== std::future_status::timeout
    std::cout << "outer" << std::endl;
  }
  std::cout << get_value.get() << std::endl;
}

void test11() {
  std::mutex m;
  std::list<std::packaged_task<int()>> tasks;
  auto post = [&tasks, &m] (std::function<int()> f) -> std::future<int> {
    std::packaged_task<int()> task(f);
    std::future<int> res = task.get_future();
    std::unique_lock<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;
  };

  auto worker = [&tasks, &m] () {
    while (true) {
      std::packaged_task<int()> task;
      {
        std::unique_lock<std::mutex> lk(m);
        if (tasks.empty())
          continue;
        task = std::move(tasks.front());
        tasks.pop_front();
      }
      task();
    }
  };

  std::thread t(worker);

  for (size_t i = 0; i < 1001; i+=100) {
    std::future<int> res = post(std::function<int()>([i] () -> int {
        return i;
        }));
    std::cout << res.get() << std::endl;
  }
  t.join();
}

void test12() {
    using namespace common;
    ts_map<std::string, int> m;
    std::vector<std::thread> threads;
    for (size_t i = 0; i < 20; ++i) {
        threads.push_back(std::thread([&m] () {
                    for (size_t i = 0; i < 100; ++i) {
                        m.set("hello", i);
                        std::cout << m.get("hello") << std::endl;
                    }
                    }));
    }
    for_each (threads.begin(), threads.end(), [] (std::thread &t) {
            t.join();
            });
}

template<typename Cont=std::vector<std::thread>>
class threads_guard {
    private:
        Cont &_ts;
    public:
        threads_guard(Cont &ts): _ts(ts) {}
        threads_guard(Cont &&t) = delete;
        ~threads_guard() {
            for_each (_ts.begin(), _ts.end(), [] (std::thread &t) {
                    if (t.joinable())
                        t.join();
                    });
        }
};

void test13() {
    auto worker = [] () {
        std::cout << "use thread join guard" << std::endl;
    };

    std::vector<std::thread> threads;
    threads_guard<> _guard(threads);
    for (size_t i = 0; i < 20; ++i) {
        threads.push_back(std::thread(worker));
    }
}

template <typename Iter, typename Func>
void p_for_each(Iter first, Iter last, Func f, size_t depth = 0) {
    static size_t idx = 0;
    depth++;
    unsigned long const length = std::distance(first, last);
    if (!length)
        return;
    unsigned long const min_per_thread = 25;
    if (length <= (2 * min_per_thread) || depth > 3) {
        std::for_each(first, last, f);
    }
    else {
        Iter const mid = first + length / 2;
        std::future<void> first_half =
            std::async(std::launch::async, &p_for_each<Iter, Func>, first, mid, f, depth);
        p_for_each(mid, last, f, depth);
        first_half.get();
    }
}

void test14() {
    auto work = [] (int idx) {
        for (size_t i = 0; i < 1000000; ++i);
    };
    std::vector<int> idx;
    for (size_t i = 0; i < 200; ++i) {
        idx.push_back(i);
    }
    p_for_each(idx.begin(), idx.end(), work);
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
  test8();
  //test9();
  test10();
  //test11();
  test12();
  test13();
  test14();
  return 0;
}
