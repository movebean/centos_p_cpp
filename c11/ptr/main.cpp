#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <cstdio>
#include <algorithm>

void test() {
  using StringPtr = std::shared_ptr<std::string>;
  std::vector<StringPtr> vecs;
  vecs.push_back(StringPtr(new std::string("hello")));
  vecs.push_back(StringPtr(new std::string("world")));
  vecs.push_back(StringPtr(new std::string("go")));
  vecs.push_back(StringPtr(new std::string("come")));
  for_each (vecs.begin(), vecs.end(), [](StringPtr s) {
      std::cout << *s << std::endl;
      });
}

void test1() {
  using StringPtr = std::shared_ptr<std::string>;
  StringPtr s(new std::string("hello"),
              [](std::string *item) {
              std::cout << "delete: " << *item << std::endl;
              delete item;
              });

  std::shared_ptr<int> p(new int[10],
                         [](int *p) {
                         std::cout << "delete array: " << p << std::endl;
                         delete[] p;
                         });

  std::shared_ptr<float> f(new float[10],
                           std::default_delete<float[]>());
}

void test2() {
  std::unique_ptr<int[], void (*)(int*)> p(new int[10],
                                         [] (int *p) {
                                         std::cout << "delete[]" << std::endl;
                                         delete[] p;
                                         });
  p[0] = 1;
  p[1] = 2;
  for (size_t i = 0; i < 10; ++i) {
    std::cout << &p[i] << std::endl;
  }
}

void test3() {
  struct Deleter {
    private:
      std::string _filename;
    public:
      Deleter (const std::string &filename):
        _filename(filename) {}
      void operator() (std::ofstream *fp) {
        fp->close();
        std::remove(_filename.c_str());
        std::cout << "delete " << _filename << std::endl;
        delete fp;
      }
  };

  std::shared_ptr<std::ofstream> f(new std::ofstream("tmpfile"),
                                   Deleter("tmpfile"));

  std::string tmp2("tmpfile2");
  std::shared_ptr<std::ofstream> f2(new std::ofstream(tmp2),
                                    [&tmp2] (std::ofstream *fp) {
                                    fp->close();
                                    std::remove(tmp2.c_str());
                                    std::cout << "delete " << tmp2 << std::endl;
                                    delete fp;
                                    });
}

void test4() {
  class Person {
    public:
      typedef std::shared_ptr<Person> Ptr;
      typedef std::weak_ptr<Person> WeakPtr;

    private:
      std::string _name;
      WeakPtr _mother;
      WeakPtr _father;
      std::vector<Ptr> _kids;

    public:
      Person (const std::string &name,
              Ptr m = nullptr,
              Ptr f = nullptr): _name(name), _mother(m), _father(f) {
      }

      WeakPtr f() { return _father; }
      WeakPtr m() { return _mother; }

      void add_kid(Ptr kid) {
        _kids.push_back(kid);
      }

      ~Person() {
        std::cout << "delete " << _name << std::endl;
      }
  };

  auto me = [] () -> typename Person::Ptr {
    Person::Ptr m(new Person("mom"));
    Person::Ptr f(new Person("dad"));
    Person::Ptr me(new Person("yoyo", m, f));
    m->add_kid(me);
    f->add_kid(me);
    Person::Ptr son(new Person("son", nullptr, me));
    me->add_kid(son);
    return me;
  }();
  std::cout << "==============" << std::endl;
  if (!me->f().lock())
    std::cout << "no f" << std::endl;
  if (!me->m().lock())
    std::cout << "no m" << std::endl;

  std::cout << me.use_count() << std::endl;
  auto me2 = std::move(me);
  std::cout << me.use_count() << std::endl;
  std::cout << me2.use_count() << std::endl;
}

void test5() {
  class Person {
    public:
      typedef std::shared_ptr<Person> Ptr;
    public:
      ~Person() {
        std::cout << "~Person" << std::endl;
      }
  };
  class Home {
    public:
      typedef std::shared_ptr<Home> Ptr;
    public:
      Person _owner;
    public:
      Home () {}
      ~Home() {
        std::cout << "~Home" << std::endl;
      }
  };
  [] () -> Person::Ptr {
    Home::Ptr yoyh(new Home);
    Person::Ptr yoyo(yoyh, &yoyh->_owner);
    std::cout << yoyo.use_count() << std::endl;
    std::cout << yoyh.use_count() << std::endl;
    return yoyo;
  }();
  //std::cout << "========================" << std::endl;
  //std::cout << yoyo.use_count() << std::endl;
}

void test6() {
  class Person {
    private:
      std::string _name;
    public:
      std::string &name() { return _name; }
      Person(const std::string &name): _name(name) {}
  };
  class PersonDeleter {
    public:
      void operator() (Person *p) {
        std::cout << p->name() << std::endl;
        delete p;
      }
  };

  std::unique_ptr<Person, PersonDeleter> yoyo(new Person("yoyo"));

  std::unique_ptr<Person, void (*)(Person *)>
    jah(new Person("jah"), [] (Person *p) {
        std::cout << p->name() << std::endl;
        delete p;
        });
}

int main() {
  test();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  return 0;
}
