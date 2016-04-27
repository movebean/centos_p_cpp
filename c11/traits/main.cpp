#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>
#include <type_traits>

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

template <typename T>
void foo_impl(const T &val, std::false_type) {
  std::cout << val << std::endl;
}

template <typename T>
void foo_impl(const T &val, std::true_type) {
  std::cout << *val << std::endl;
}

template <typename T>
void foo(const T &val) {
  foo_impl(val, std::is_pointer<T>());
}

template <typename>
struct __is_float_helper:public std::false_type {};
template <>
struct __is_float_helper<float>:public std::true_type {};
template <>
struct __is_float_helper<double>:public std::true_type {};
template <typename T>
  struct is_float
  :public std::integral_constant<bool, (__is_float_helper<typename
      std::remove_cv<T>::type>::value)> { };

template <typename T>
void float_impl(const T&val, std::false_type) {
  std::cout << "non-float type: " << val << std::endl;
}
template <typename T>
void float_impl(const T&val, std::true_type) {
  std::cout << "float type: " << val << std::endl;
}
template <typename T>
void float_print(const T &val) {
  float_impl(val, is_float<T>());
}


void test1() {
  int a = 1;
  int *b = &a;
  foo(a);
  foo(b);

  float c = 1.0;
  double d = 2.0;
  float_print(a);
  float_print(c);
  float_print(d);
}

void test2() {
  std::common_type<int, double>::type a{};
  float_print(a);
  std::common_type<int, long>::type b{};
  float_print(b);
  //std::common_type<int, std::string>::type c{}; //error
  std::common_type<int, long, double>::type d{};
  float_print(d);
}

void test3() {
  class A {
    public:
      int item;
  };
  std::cout << std::boolalpha;
  std::cout << __is_class(A) << std::endl;
  union B{};
  std::cout << __is_union(B) << std::endl;
  enum class C {};
  std::cout << __is_enum(C) << std::endl;
  std::cout << std::is_member_pointer<int A::*>::value << std::endl;
}

template <typename>
class func_return {};
template <typename Return, typename... Args>
class func_return<Return(*)(Args...)> {
  typedef Return type;
};
template <typename Return, typename... Args>
class func_return<Return(&)(Args...)> {
  typedef Return type;
};
template <typename Return, typename... Args>
class func_return<Return(Args...)> {
  typedef Return type;
};

void test4() {
  std::cout << "=================================" << std::endl;
  int (*foo) (int);
  std::cout << std::is_integral<func_return<decltype(foo)>::type>::value << std::endl;
  std::cout << std::is_void<func_return<decltype(foo)>::type>::value << std::endl;

  void (*bar) (int);
  std::cout << std::is_integral<func_return<decltype(bar)>::type>::value << std::endl;
  std::cout << std::is_void<func_return<decltype(bar)>::type>::value << std::endl;

  std::cout << std::is_integral<func_return<decltype(test4)>::type>::value << std::endl;
  std::cout << std::is_void<func_return<decltype(test4)>::type>::value << std::endl;

  std::cout << std::is_function<decltype(foo)>::value << std::endl;
  std::cout << std::is_function<decltype(test4)>::value << std::endl;
}

template <int idx, typename T, typename... Args>
struct ELEM:public ELEM<idx-1, Args...> {};

template <typename T, typename... Args>
struct ELEM<0, T, Args...> {
  typedef T type;
};

template <int i, typename T>
struct get_elem {};

template <int i, typename... Args>
struct get_elem<i, std::tuple<Args...>> {
  typedef typename ELEM<i, Args...>::type type;
};

void test5() {
  std::cout << "=================================" << std::endl;
  typedef std::tuple<int, float, void> TestTuple;
  std::cout << std::is_void<get_elem<2, TestTuple>::type>::value << std::endl;
  std::cout << std::is_floating_point<get_elem<1, TestTuple>::type>::value << std::endl;
  std::cout << std::is_integral<get_elem<0, TestTuple>::type>::value << std::endl;

  typedef std::tuple<void, float, int> TestTuple2;
  std::cout << std::is_void<get_elem<0, TestTuple2>::type>::value << std::endl;
  std::cout << std::is_floating_point<get_elem<1, TestTuple2>::type>::value << std::endl;
  std::cout << std::is_integral<get_elem<2, TestTuple2>::type>::value << std::endl;
}

int main() {
  test();
  test1();
  test2();
  test3();
  test4();
  test5();
  return 0;
}
