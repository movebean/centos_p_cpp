#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>
//#include <algorithm>

int test1() {
  int boys = 4, girls = 3;
  auto totalChild = [=]()->int { return girls + boys; };
  return totalChild();
}

void test2() {
  int x = 2, y = 3;
  auto even = [&] () -> bool {
    if ((x + y) % 2 == 0)
      return true;
    return false;
  };
  std::cout << even() << std::endl;
  return;
}

void test3() {
  int x = 2;
  int y = 3;
  auto get_sum = [=] () {
    return x + y;
  };
  //int y = 3;             //error: cannot move y definition here
  std::cout << get_sum() << std::endl;
  return;
}

void test4() {
  int j = 12;
  auto by_val = [=] {return j+1;};
  auto by_ref = [&] {return j+1;};
  std::cout << by_val() << std::endl;
  std::cout << by_ref() << std::endl;
  j++;
  std::cout << by_val() << std::endl;
  std::cout << by_ref() << std::endl;
}

void test5() {
  int val = 3;
  //auto change_by_val = [=]() { return val = 5; };        //error
  auto change_by_mval = [=]() mutable { return val = 5; };
  auto change_by_ref = [&]() { return val = 6; };
  //std::cout << change_by_val() << std::endl;
  std::cout << change_by_mval() << std::endl;
  std::cout << change_by_ref() << std::endl;
}

void test6() {
  std::vector<int> nums {1, 2, 3, 4, 5, 6};

  for_each (nums.begin(), nums.end(), [=](int i) {
      if (i%2 == 0)
        std::cout << i << ' ';
      });
  std::cout << std::endl;

  std::cout << *find_if (nums.begin(), nums.end(), [=](int i) {
      return i == 3;
      }) << std::endl;

  std::cout << count_if (nums.begin(), nums.end(), [=](int i) {
      return i > 3 && i % 2 == 0;
      }) << std::endl;

  int score = 0;
  int multiply = 1;
  for_each (nums.begin(), nums.end(), [&](int i) {
      score += i;
      multiply *= i;
      });
  std::cout << score << ' ' << multiply << std::endl;
}

void test7() {
  int id = 0;
  auto f = [=] () mutable {
    std::cout << id << std::endl;
    id++;
  };
  id = 52;
  f();      // >0
  f();      // >1
  f();      // >2
  std::cout << id << std::endl;     // >52
  //class {
  //  private:
  //    int id;
  //  public:
  //    void operator() () {
  //      std::cout << id << std::endl;
  //      id++;
  //    }
  //};
}

std::function<bool (int)> get_lambda_bigger(int val) {
  return [=] (int item) -> bool {
    return item > val;
  };
}

void test8() {
  auto b7 = get_lambda_bigger(7);
  auto b9 = get_lambda_bigger(9);
  std::cout << b7(2) << std::endl;
  std::cout << b7(8) << std::endl;
  std::cout << b9(8) << std::endl;
  std::cout << b9(10) << std::endl;

  std::vector<int> vs {5, 6, 7, 8, 9, 10};
  for_each (vs.begin(), vs.end(), [&] (int i) {
      if (b7(i) && !b9(i))
        std::cout << i << ' ';
      });
  std::cout << std::endl;
}

int main() {
  std::cout << test1() << std::endl;
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  return 0;
}
