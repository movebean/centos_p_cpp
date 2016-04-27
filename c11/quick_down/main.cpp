#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>

void test1() {
  nullptr_t my_null;
  std::cout << &my_null << std::endl;
  const nullptr_t &&default_null = nullptr;
  std::cout << &default_null << std::endl;
}

class NoHeapAllocC11 {
  public:
    void * operator new(size_t) = delete;
};

class NoHeapAllocC98 {
  private:
    void *operator new(size_t);
};

void test2() {
  NoHeapAllocC11 *nha1 = new NoHeapAllocC11();
  NoHeapAllocC98 *nha2 = new NoHeapAllocC98();
}

int main() {
  test1();
  test2();
  return 0;
}
