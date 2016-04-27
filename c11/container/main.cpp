#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>
#include <set>
#include <map>

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

int main() {
  test();
  return 0;
}
