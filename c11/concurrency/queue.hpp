#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__
#include <exception>
#include <mutex>
#include <queue>
#include <memory>

namespace common {
    template <typename T>
    class ts_queue {
        private:
            mutable std::mutex _m;
            std::deque<T> _data;
            std::condition_variable _cond;
        public:
            ts_queue() {}
            void push(T new_value) {
                std::lock_guard<std::mutex> _lk(_m);
                _data.push_back(std::move(new_value));
                _cond.notify_one();
            }
            bool try_pop(T &value) {
                std::lock_guard<std::mutex> _lk(_m);
                if (_data.empty()) {
                    return false;
                }
                value = std::move(_data.front());
                _data.pop_front();
                return true;
            }
            bool try_pop_tail(T &value) {
                std::lock_guard<std::mutex> _lk(_m);
                if (_data.empty()) {
                    return false;
                }
                value = std::move(_data.back());
                _data.pop_back();
                return true;
            }
            void wait_and_pop(T &value) {
                std::unique_lock<std::mutex> _lk(_m);
                _cond.wait(_lk, [this] { return !_data.empty(); });
                value = std::move(_data.front());
                _data.pop_front();
            }
            std::shared_ptr<T> wait_and_pop() {
                std::unique_lock<std::mutex> _lk(_m);
                _data.wait(_lk, [this] { return !_data.empty(); });
                std::shared_ptr<T> res(
                        std::make_shared<T>(std::move(_data.front())));
                _data.pop_front();
                return res;
            }
            bool empty() const {
                std::lock_guard<std::mutex> _lk(_m);
                return _data.empty();
            }
            size_t size() const {
                std::lock_guard<std::mutex> _lk(_m);
                return _data.size();
            }
    };
}

#endif // __QUEUE_HPP__
