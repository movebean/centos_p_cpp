#ifndef __MAP_HPP__
#define __MAP_HPP__
#include <mutex>
#include <mutex>
#include <vector>
#include <memory>
#include <boost/thread/shared_mutex.hpp>

namespace common {
    template <typename KEY, typename VALUE>
    class ts_map {
        private:
            struct bucket {
                typedef std::pair<KEY, VALUE> ITEM;
                typedef std::list<ITEM> ITEM_LIST;
                typedef typename ITEM_LIST::iterator iterator;
                typedef boost::shared_mutex MUTEX;
                MUTEX _m;
                ITEM_LIST _items;
                bucket() {}
                iterator get_entry(const KEY &key) {
                    boost::shared_lock<MUTEX> _lk(_m);
                    for (auto it = _items.begin(); it != _items.end(); ++it) {
                        if (it->first == key)
                            return it;
                    }
                    return _items.end();
                }
            };
            static const size_t BUCKET_SIZE = 19;
            std::vector<bucket> _buckets;
            std::hash<std::string> _shash;
        public:
            ts_map(): _buckets(BUCKET_SIZE) {}
            ts_map(const ts_map &o) = delete;
            ts_map &operator=(const ts_map &o) = delete;
            void set(const KEY &key, VALUE value) {
                bucket &b = _buckets[__key(key)];
                typename bucket::iterator it = b.get_entry(key);
                std::unique_lock<typename bucket::MUTEX> _lk(b._m);
                if (it != b._items.end()) {
                    it->second = value;
                    return;
                }
                b._items.push_back(typename bucket::ITEM(key, value));
            }
            void set(typename bucket::ITEM &item) {
                set(item.first, item.second);
            }
            VALUE &get(const KEY &key) {
                bucket &b = _buckets[__key(key)];
                typename bucket::iterator it = b.get_entry(key);
                if (it != b._items.end()) {
                    return it->second;
                }
                set(key, VALUE());
                return get(key);
            }
        private:
            size_t __key(const KEY &key) {
                return _shash(key) % BUCKET_SIZE;
            }
    };
}

#endif // __MAP_HPP__
