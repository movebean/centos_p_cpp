#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>

template <typename V, typename R>
std::ostream &operator<< (std::ostream &os,
    const std::chrono::duration<V, R> &d) {
  os << d.count() << " [" << R::num;
  if (R::den != 1)
    os << '/' << R::den;
  os << " secs]";
  return os;
}

std::ostream &operator<< (std::ostream &os,
    const std::chrono::system_clock::time_point &tp) {
  std::time_t t = std::chrono::system_clock::to_time_t(tp);
  //std::string ts = std::ctime(&t);
  std::string ts = std::asctime(gmtime(&t));
  ts.resize(ts.size()-1);
  os << ts;
  return os;
}

void test() {
  std::chrono::duration<int> twenty_sec(20);
  std::chrono::duration<double, std::ratio<60>> half_min(0.5);
  std::chrono::duration<int, std::milli> one_millisec(1);
  std::chrono::duration<double, std::ratio<86400>> half_day(0.5);

  //typedef duration<signed int> seconds
  std::chrono::seconds twenty_sec2(20);
  //typedef duration<signed int, ratio<3600>> hours
  std::chrono::hours a_day(24);
  //typedef duration<signed int, milli> milliseconds
  std::chrono::milliseconds one_millisec2(1);

  std::chrono::milliseconds di = twenty_sec;
  std::cout << di << std::endl;
  di -= one_millisec;
  std::cout << di << std::endl;
  di *= 20;
  std::cout << di << std::endl;
  std::cout << std::chrono::microseconds(di) << std::endl;
  typedef std::chrono::duration<int, std::ratio<60>> minutes;
  std::cout << std::chrono::duration_cast<minutes>(di) << std::endl;

  {
    using namespace std::chrono;
    di *= 20;
    std::cout << duration_cast<hours>(di) << std::endl;
    std::cout << duration_cast<minutes>(di % hours(1)) << std::endl;
    std::cout << duration_cast<seconds>(di % minutes(1)) << std::endl;
    std::cout << duration_cast<milliseconds>(di % seconds(1)) << std::endl;
  }

  std::cout << "you have " << [] (std::chrono::seconds left) {
    return left.count();
  }(std::chrono::hours(2)) << " seconds left" << std::endl;;

}

void test1() {
  std::cout << std::chrono::system_clock::duration(1) << std::endl;
  std::cout << std::chrono::high_resolution_clock::duration(1) << std::endl;
  std::cout << std::chrono::steady_clock::duration(1) << std::endl;

  std::cout << "sleep" << std::endl;
  std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(10));
  std::cout << "go" << std::endl;

  {
    using namespace std::chrono;
    auto system_start = system_clock::now();
    auto last_time = system_start;
    while (system_start + seconds(2) >= system_clock::now()) {
      if (last_time + seconds(1) < system_clock::now()) {
        auto now = system_clock::now();
        std::cout << duration_cast<seconds>(now - system_start) << std::endl;
        last_time = now;
      }
    }
    std::cout << "2 seconds finish" << std::endl;
  }

  std::cout << "sleep" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(10));
  std::cout << "go" << std::endl;

  {
    using namespace std::chrono;
    system_clock::time_point tp;
    std::cout << "epoch: " << tp << std::endl;

    tp = system_clock::now();
    std::cout << "now: " << tp << std::endl;
    tp += hours(8);
    std::cout << "now+8hours: " << tp << std::endl;
    if (tp > system_clock::now())
      std::cout << "I'm in the east" << std::endl;

    tp = system_clock::time_point::min();
    std::cout << "min: " << tp << std::endl;

    tp = system_clock::time_point::max();
    std::cout << "max: " << tp << std::endl;
  }
}

int main() {
  test();
  test1();
  return 0;
}
