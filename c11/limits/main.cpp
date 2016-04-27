#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>

void test() {
  std::cout << std::numeric_limits<int>::min() << std::endl;
  std::cout << std::numeric_limits<int>::max() << std::endl;
  std::cout << std::numeric_limits<int>::digits << std::endl;

  std::cout << std::numeric_limits<size_t>::min() << std::endl;
  std::cout << std::numeric_limits<size_t>::max() << std::endl;
  std::cout << std::numeric_limits<size_t>::digits << std::endl;

  char cs[std::numeric_limits<unsigned char>::max()];
  std::cout << sizeof(cs) << std::endl;
}

int main() {
  test();
  return 0;
}
