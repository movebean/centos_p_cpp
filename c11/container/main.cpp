#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <iterator>

void test() {
  std::set<std::string> projects = {"wechat", "beetalk",
    "heros of kingdoms"};
  projects.insert({"talktalk"});
  for (auto p: projects) {
    std::cout << p << std::endl;
  }

  std::map<int, std::string> sp = { {1, "heros of kingdoms"},
                                    {2, "beetalk"},
                                    {3, "talktalk"},
                                    {4, "wechat"}};
  for (auto item: sp) {
    std::cout << item.second << std::endl;
  }

  std::unordered_map<int, std::string> usp = { {1, "heros of kingdoms"},
                                               {2, "beetalk"},
                                               {3, "talktalk"},
                                               {4, "wechat"}};
  for (auto item: usp) {
    std::cout << item.first << ' ' << item.second << std::endl;
  }
}

template <typename Iter>
void print(const Iter &first, const Iter &second) {
  for(auto i = first; i != second; ++i) {
    std::cout << *i << ' ';
  }
  std::cout << std::endl;
}

void test1() {
  std::list<std::string> li {"yoyo", "gogo", "haha"};
  print(li.begin(), li.end());

  std::vector<std::string> vi(std::make_move_iterator(li.begin()),
      std::make_move_iterator(li.end()));
  print(vi.begin(), vi.end());
  print(li.begin(), li.end());
}

void test2() {
  std::vector<int> vi;
  for (int i = 0; i < 9; ++i) {
    vi.push_back(i);
    decltype(vi)(vi).swap(vi);
    std::cout << "size: " << vi.size() << " cap:" << vi.capacity() << std::endl;
  }
  std::copy(vi.cbegin(), vi.cend(),
      std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

void test3() {
  std::set<int, std::greater<int>> si;
  si.insert({4, 3, 5, 1, 6, 2});
  print(si.begin(), si.end());

  si.erase(5);
  print(si.begin(), si.end());

  std::set<int> si2(si.begin(), si.end());
  print(si2.begin(), si2.end());
  if (std::equal(si.begin(), si.end(), si2.rbegin())) {
    std::cout << "equal" << std::endl;
  }
}

int main() {
  test();
  test1();
  test2();
  test3();
  return 0;
}
