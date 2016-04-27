#include <iostream>
#include <boost/shared_ptr.hpp>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <list>
#include <deque>
#include <iterator>
#include <unordered_set>
#include <iomanip>

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

void test1() {
  std::vector<int> coll({2, 5, 4, 3, 6, 1});
  auto min = std::min_element(coll.begin(), coll.end());
  std::cout << *min << std::endl;

  std::sort(coll.begin(), coll.end());
  for (auto i: coll) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;

  auto pos = std::find(coll.begin(), coll.end(), 3);
  std::reverse(pos, coll.end());
  for (auto i: coll) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
}

void test2() {
  std::list<int> li({1, 2, 3, 4, 5, 6, 7});
  //std::vector<int> vi;
  std::vector<int> vi(li.size());

  std::copy(li.begin(), li.end(), vi.begin()); //Segmentation fault (core dumped)
  for (auto i: vi) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
}

template <typename Iter>
void print(const Iter first, const Iter end) {
  for(auto i = first; i != end; ++i) {
    std::cout << *i << ' ';
  }
  std::cout << std::endl;
}

void test3() {
  std::list<int> coll = {1, 2, 3, 4, 5, 6, 7};

  std::vector<int> coll2;
  std::copy(coll.cbegin(), coll.cend(), std::back_inserter(coll2));
  print(coll2.begin(), coll2.end());

  std::deque<int> coll3;
  std::copy(coll.cbegin(), coll.cend(), std::front_inserter(coll3));
  print(coll3.begin(), coll3.end());

  std::set<int> coll4;
  std::copy(coll.cbegin(), coll.cend(), std::inserter(coll4, coll4.begin()));
  print(coll4.begin(), coll4.end());
}

void test4() {
  std::set<std::string> s;
  std::copy(std::istream_iterator<std::string>(std::cin),
            std::istream_iterator<std::string>(),
            std::inserter(s, s.begin()));

  std::copy(s.cbegin(), s.cend(),
            std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << std::endl;
  std::copy(s.crbegin(), s.crend(),
            std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << std::endl;
}

void test5() {
  std::list<int> vi;

  for (int i = 0; i < 6; ++i) {
    vi.push_back(i);
    vi.push_front(i);
  }

  auto end = std::remove(vi.begin(), vi.end(), 3);
  vi.erase(end, vi.end());
  print(vi.begin(), vi.end());

  std::vector<int> li;
  std::transform(vi.begin(), vi.end(), std::back_inserter(li),
      [] (int &i) -> int {
      return i*i;
      });
  print(li.begin(), li.end());

  std::transform(li.begin(), li.end(), li.begin(),
      [] (int &i) {
      return i*i*i;
      });
  print(li.begin(), li.end());

  std::replace_if(li.begin(), li.end(),
      [] (int &i) {
      return i%2 == 1;
      }, -1);
  print(li.begin(), li.end());
}

void test6() {
  class Person {
    public:
      std::string _f_name;
      std::string _l_name;
    public:
      Person(const std::string &f_name, const std::string &l_name):
        _f_name(f_name), _l_name(l_name) {}
      bool operator< (const Person &r) const {
        return (_f_name < r._f_name)?
          true: (_l_name <= r._l_name)?
          true: false;
      }
  };

  std::vector<Person> persons;
  persons.push_back(Person("yoyo", "koko"));
  persons.push_back(Person("yoyo", "haha"));
  persons.push_back(Person("haha", "koko"));
  std::sort(persons.begin(), persons.end());
  for_each(persons.begin(), persons.end(), [] (const Person &p) {
      std::cout << p._f_name << ' ' << p._l_name << std::endl;
      });

  std::sort(persons.begin(), persons.end(),
      [](const Person &p1, const Person &p2) {
      return (p1._f_name >= p2._f_name)?
      true: (p1._l_name > p2._l_name)?
      true: false;
      });
  for_each(persons.begin(), persons.end(), [] (const Person &p) {
      std::cout << p._f_name << ' ' << p._l_name << std::endl;
      });

}

std::ostream &operator<< (std::ostream &os, std::pair<const std::string, std::string> p) {
  os << '(' << p.first << ' ' << p.second << ')';
  return os;
}

template <typename Cont>
void print_hash_state(const Cont &cont) {
  std::cout << "size: " << cont.size() << std::endl;
  std::cout << "bucket: " << cont.bucket_count() << std::endl;
  std::cout << "load factor: " << cont.load_factor() << std::endl;
  std::cout << "max load factor: " << cont.max_load_factor() << std::endl;

  if (typeid(typename std::iterator_traits
        <typename Cont::iterator>::iterator_category) ==
      typeid(std::bidirectional_iterator_tag)) {
    std::cout << "chaining style: doubly-linked" << std::endl;
  }
  else {
    std::cout << "chaining style: singly-linked" << std::endl;
  }

  std::cout << "data: " << std::endl;
  for (std::size_t idx = 0; idx != cont.bucket_count(); ++idx) {
    std::cout << " b[" << std::setw(2) << idx << "]: ";
    for (auto pos = cont.begin(idx); pos != cont.end(idx); ++pos) {
      std::cout << *pos << " ";
    }
    std::cout << std::endl;
  }
}

void test7() {
  struct Customer {
    public:
      uint32_t _id;
      std::string _name;
    public:
      Customer(uint32_t id, const std::string &name):
        _id(id), _name(name) {}
      bool operator< (const Customer &o) const {
        return _id < o._id;
      }
      bool operator== (const Customer &o) const {
        return _id == o._id;
      }
  };

  std::function<std::size_t (const Customer &)> hash =
    [] (const Customer &c) -> std::size_t {
      return c._id;
    };

  std::unordered_set<Customer, decltype(hash)> cus(20, hash);
  cus.insert(Customer(1, "yoyo"));
  cus.insert(Customer(2, "gogo"));
  cus.insert(Customer(3, "haha"));
  std::for_each(cus.begin(), cus.end(), [](const Customer &c) {
      std::cout << c._id << ' ' << c._name << std::endl;
      });

  std::function<std::size_t (const Customer &)> hash2 =
    [] (const Customer &c) -> std::size_t {
      return std::hash<uint32_t>()(c._id) +
        std::hash<std::string>()(c._name);
    };
  std::unordered_set<Customer, decltype(hash2)> cus2(1, hash2);
  cus2.insert(Customer(1, "yoyo"));
  cus2.insert(Customer(2, "gogo"));
  cus2.insert(Customer(3, "haha"));
  std::for_each(cus2.begin(), cus2.end(), [](const Customer &c) {
      std::cout << c._id << ' ' << c._name << std::endl;
      });

  std::unordered_set<int> seti(10);
  seti.insert({11, 22, 33, 14, 25, 16, 23, 18, 19, 21});
  print_hash_state(seti);
  seti.insert({1, 2, 3});
  print_hash_state(seti);

  std::unordered_map<std::string, std::string> maps = {
    {"day", "Tag"},
    {"strange", "fremd"},
    {"car", "Auto"},
    {"smart", "elegant"},
    {"trait", "Merkmal"},
    {"strange", "seltsam"}
  };
  print_hash_state(maps);
  maps.insert({
      {"smart", "raffiniert"},
      {"smart", "klug"},
      {"clever", "faffiniert"}
      });
  print_hash_state(maps);
  maps.max_load_factor(0.7);
  print_hash_state(maps);
}

int main() {
  test();
  test1();
  test2();
  test3();
  //test4();
  test5();
  test6();
  test7();
  return 0;
}
