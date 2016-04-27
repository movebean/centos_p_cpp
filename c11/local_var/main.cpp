#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include <initializer_list>
#include <stdlib.h>


void test() {
  class Group {
    public:
      Group() {}
      Group(int a): Group() { data = a; }
      Group(double m): Group() { mem = m; }
      Group(int a, double m): Group(a) { mem = m; }
      Group(int a, double m, std::string n): data(a), mem(m), name(n) {}
      void print() {
        std::cout << "Base Group" << std::endl;
      }

    protected:
      int data = 1;
      double mem = 0.0;
      std::string name{"Group"};
  };

  class AGroup: public Group {
    public:
      using Group::Group;     //derive constructor of class "Group"
      using Group::print;
      void print(int) {
        std::cout << "AGroup:" << name << " and age:" << age << std::endl;
      }

    private:
      int age {11};
  };

  AGroup ag(1, 2.0, "me");
  ag.print(1);
}

void test1() {
  int a[] = {1, 3, 5};
  int b[] {2, 4, 6};
  std::vector<int> c{1, 3, 5};
  std::map<int, float> d = {{1, 1.0f}, {2, 2.0f}, {5, 3.2f}};
  for (auto &item: d) {
    std::cout << item.first << ' ' << item.second << std::endl;
  }

  enum Gender {boy, girl};
  class People {
    public:
      //#include <initializer_list>
      People(std::initializer_list<std::pair<std::string, Gender>> l) {
        for (auto i = l.begin(); i != l.end(); ++i)
          data.push_back(*i);
      }
    private:
      std::vector<std::pair<std::string, Gender>> data;
  };
  People ship2012 = {{"Garfield", boy}, {"HelloKitty", girl}};

  void Fun(std::initializer_list<int> iv);

  std::cout << a << ' ' << b << std::endl;
}

constexpr int func(int x) {
  return x;
}

void test2() {
  std::cout << func(random()) << std::endl;
  switch (2) {
    //case func(random()):
    //  std::cout << "random" << std::endl;
    //  break;
    case func(2):
      std::cout << "should be OK" << std::endl;
      break;
    default:
      std::cout << "default" << std::endl;
      break;
  }
}

constexpr int Fibonacci(int n) {
  return (n == 1) ? 1: ((n == 2) ? 1: Fibonacci(n - 1) + Fibonacci(n - 2));
}

template <long num>
struct FibT {
  static const long val = FibT<num - 1>::val + FibT<num - 2>::val;
};
template<> struct FibT<2> { static const long val = 1; };
template<> struct FibT<1> { static const long val = 1; };
template<> struct FibT<0> { static const long val = 0; };

void test3() {
  int fib[] = {
    Fibonacci(11), Fibonacci(12),
    Fibonacci(13), Fibonacci(14),
    Fibonacci(15), Fibonacci(16),
  };
  for (auto i: fib)
    std::cout << i << std::endl;

  int fibT[] = {
    FibT<11>::val, FibT<12>::val,
    FibT<13>::val, FibT<14>::val,
    FibT<15>::val, FibT<16>::val,
  };
  for (auto i: fibT)
    std::cout << i << std::endl;
}

int main() {
  test();
  test1();
  test2();
  test3();
  return 0;
}
