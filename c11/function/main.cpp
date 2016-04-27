#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>

void sum(int x, int y) {
  std::cout << x + y << std::endl;
}

void test() {
  int a = 2, b = 3;
  std::vector<std::function<void (int, int)>> tasks;
  tasks.push_back([] (int x, int y) {
      std::cout << x * y << std::endl;
      });
  tasks.push_back(sum);
  for (auto t: tasks) {
    t(a, b);
  }
}

std::function<int (int)> factory(int base) {
  return [=] (int a) -> int {
    return a * base;
  };
}

void test1() {
  auto b2 = factory(2);
  auto b3 = factory(3);
  std::cout << b2(10) << std::endl;
  std::cout << b3(10) << std::endl;
}

void test2() {
  int x = 2, y = 3, z = 5;
  int *px = &x, *py = &y, *pz = &z;
  int *min, *max;
  std::tie(min, max) = std::minmax({px, py, pz},
      [] (int *a, int *b) -> bool {
      return *a < *b;
      });
  std::cout << "min: " << *min << ' ' << "max: " << *max << std::endl;

  std::cout << std::min({1,2,3,4,5,6,}) << std::endl;
  std::cout << std::max({1,2,3,4,5,6,}) << std::endl;

  std::cout << "x: " << x << " y: " << y << std::endl;
  std::swap(x, y);
  std::cout << "x: " << x << " y: " << y << std::endl;

  struct MyContainer {
    std::vector<int> elems;
    int numElems;

    void swap (MyContainer &x) {
      std::swap(elems, x.elems);
      std::swap(numElems, x.numElems);
    }

    inline bool operator== (const MyContainer &other) const {
      auto i = elems.begin();
      auto j = other.elems.begin();
      for (; i != elems.end() and j != other.elems.end(); ++i, ++j) {
        if (*i != *j)
          return false;
      }
      return numElems == other.numElems;
    }

    inline bool operator< (const MyContainer &other) const {
      auto i = elems.begin();
      auto j = other.elems.begin();
      for (; i != elems.end() and j != other.elems.end(); ++i, ++j) {
        if (*i < *j)
          return true;
        else if (*i > *j)
          return false;
      }
      return numElems < other.numElems;
    }
  };

  MyContainer a;
  a.elems.push_back(1);
  a.elems.push_back(3);
  a.elems.push_back(5);
  a.numElems = 3;

  MyContainer b;
  b.elems.push_back(2);
  b.elems.push_back(4);
  b.numElems = 2;

  std::swap(a, b);
  for_each (a.elems.begin(), a.elems.end(), [] (int i) {
      std::cout << i << ' ';
      });
  std::cout << '(' << a.numElems << ')' << std::endl;
  for_each (b.elems.begin(), b.elems.end(), [] (int i) {
      std::cout << i << ' ';
      });
  std::cout << '(' << b.numElems << ')' << std::endl;

  using namespace std::rel_ops;
  MyContainer c;
  c.elems.push_back(2);
  c.elems.push_back(4);
  c.numElems = 2;
  if (a == c)
    std::cout << "a==c" << std::endl;
  if (b <= c)
    std::cout << "b<=c" << std::endl;
}

int main() {
  test();
  test1();
  test2();
  return 0;
}
