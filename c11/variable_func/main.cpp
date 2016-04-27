#include <iostream>
#include <stdarg.h>
#include <vector>
#include <list>

double SumOfFloat(int count, ...) {
  va_list ap;
  double sum = 0;
  va_start(ap, count);
  for (int i = 0; i < count; ++i)
    sum += va_arg(ap, double);
  va_end(ap);
  return sum;
}

void test() {
  std::cout << SumOfFloat(3, 1.1f, 2.2f, 3.3f) << std::endl;
}

template <int ...Numbers> struct multi;

template <int First, int ...Last>
struct multi<First, Last...> {
  static const int val = First * multi<Last...>::val;
};

template <>
struct multi<> {
  static const int val = 1;
};

void test1() {
  std::cout << multi<1, 2, 3, 4, 5, 6>::val << std::endl;
}

template <typename ...T> void CallEach(T... t) {}
template <typename T> T pr(T t) {
  std::cout << t << std::endl;
  return t;
}
template <typename ...A>
void VTPrint(A... a) {
  CallEach(pr(a)...);
};

template <typename T1, typename ...T>
void Print(T1 t, T... a);

template <typename T>
void Print(T t) {
  pr(t);
}
template <typename T1, typename ...T>
void Print(T1 t, T... a) {
  pr(t);
  Print(a...);
}

void test2() {
  VTPrint(1, 2, 3, 4, 5, 6);
  Print(1, 2, 3, 4, 5, 6);
}

template <typename ItemType>
class MVector {
  public:
    typedef std::vector<ItemType> Type;
};

template <typename ItemType>
class MList {
  public:
    typedef std::list<ItemType> Type;
};

template <typename T, template<typename> class Cont1,
         template<typename> class ... Conts>
class MultiContainer {
  public:
    typename Cont1<T>::Type head;
    MultiContainer<T, Conts...> tail;
};

template <typename T, template<typename> class Cont>
class MultiContainer<T, Cont> {
  public:
    typename Cont<T>::Type head;
};

void test3() {
  std::cout << "=================================" << std::endl;
  MultiContainer<int, MVector, MList> multi;
  multi.tail.head.push_back(1);
  multi.tail.head.push_back(2);
  for (auto item: multi.tail.head)
    std::cout << item << std::endl;
}

int main() {
  test();
  test1();
  test2();
  test3();
  return 0;
}
