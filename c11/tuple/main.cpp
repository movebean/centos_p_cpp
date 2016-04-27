#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>

using IntFloatPair = std::pair<int, float>;

template <int IDX, int MAX, typename... Args>
struct PRINT {
  static void print (std::ostream &os, const std::tuple<Args...> &t) {
    os << std::get<IDX>(t) << (IDX+1==MAX?"":", ");
    PRINT<IDX+1, MAX, Args...>::print(os, t);
  }
};

template <int MAX, typename... Args>
struct PRINT<MAX, MAX, Args...> {
  static void print (std::ostream &os, const std::tuple<Args...> &t) {}
};

template <typename... Args>
std::ostream &operator<< (std::ostream &os, const std::tuple<Args...> &t) {
  PRINT<0, sizeof...(Args), Args...>::print(os, t);
  return os;
}

void test() {
  IntFloatPair p {42, 3.14};
  std::cout << std::get<0>(p) << std::endl;
  std::cout << std::get<1>(p) << std::endl;
  std::cout << std::tuple_size<decltype(p)>::value << std::endl;
}

void test1() {
  int i = 0;
  auto p = std::make_pair(std::ref(i), std::ref(i));
  ++i;
  ++p.first;
  std::cout << p.second << std::endl;
  int j = 0;
  std::tie(std::ignore, j) = p;
  std::cout << j << std::endl;
}

void test2() {
  std::tuple<std::string, int> t = std::make_pair<std::string, int>("hello", 2);
  std::cout << std::get<0>(t) << std::endl;
  std::get<1>(t) = 3;
  std::cout << std::get<1>(t) << std::endl;
  if (t > std::make_tuple("hello", 2))
    std::cout << "amazing" << std::endl;
}

void test3() {
  int n = 0;
  auto tt = std::tuple_cat(std::make_tuple(32, 7.7, "hello"),
                           std::tie(n),
                           std::make_tuple(std::vector<int>({2, 3, 4})));
  //<int, double, const char *, int&, std::vector<int>>
  std::cout << '(' << std::make_tuple(32, 7.7, "hello", n) << ')' << std::endl;
}

int main() {
  test();
  test1();
  test2();
  test3();
  return 0;
}
