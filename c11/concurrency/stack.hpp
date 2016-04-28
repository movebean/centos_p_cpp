#ifndef __STACK_HPP__
#define __STACK_HPP__
#include <exception>
#include <mutex>
#include <stack>
#include <memory>

namespace common {
    struct empty_stack: std::exception {
        const char *what() const throw() {
            return "empty stack";
        }
    };

    template <typename T>
    class ts_stack {
        private:
            std::stack<T> _data;
            mutable std::mutex _m;
        public:
            ts_stack() {}
            ts_stack(const ts_stack &other) {
                std::lock_guard<std::mutex> _lk(other._m);
                _data = other._data;
            }
            ts_stack &operator=(const ts_stack &) = delete;
            void push(T new_value) {
                std::lock_guard<std::mutex> _lk(_m);
                _data.push(std::move(new_value));
            }
            std::shared_ptr<T> pop() {
                std::lock_guard<std::mutex> _lk(_m);
                if (_data.empty()) throw empty_stack();
                std::shared_ptr<T> const res(
                        std::make_shared<T>(std::move(_data.top())));
                _data.pop();
                return res;
            }
            std::shared_ptr<T> top() {
                std::lock_guard<std::mutex> _lk(_m);
                if (_data.empty()) throw empty_stack();
                return std::make_shared<T>(_data.top());
            }
            void pop(T &value) {
                std::lock_guard<std::mutex> _lk(_m);
                if (_data.empty()) throw empty_stack();
                value = std::move(_data.top());
                _data.pop();
            }
            bool empty() const {
                std::lock_guard<std::mutex> _lk(_m);
                return _data.empty();
            }
            std::shared_ptr<T> nonempty_pop() {
                std::lock_guard<std::mutex> _lk(_m);
                if (_data.empty())
                    return std::make_shared<T>();
                std::shared_ptr<T> const res(
                        std::make_shared<T>(std::move(_data.top())));
                _data.pop();
                return res;
            }
    };
}

#endif // __STACK_HPP__
