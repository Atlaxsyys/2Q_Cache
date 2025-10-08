#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

namespace cache {

template<typename Key, typename Value>
class TwoQCache {
private:
    using ListIterator = typename std::list<Key>::iterator;

    std::size_t capacity_;
    std::size_t kin_;
    std::size_t kout_;
    std::size_t km_;

    std::list<Key> A1in_;
    std::list<Key> A1out_;
    std::list<Key> Am_;

    std::unordered_map<Key, Value> values_;
    std::unordered_map<Key, ListIterator> iterators_;
    std::unordered_map<Key, char> types_;

    void reclaim_for_A1in()
    {
        if (A1in_.size() >= kin_)
        {
            Key evict_key = A1in_.back();
            A1in_.pop_back();
            values_.erase(evict_key);
            iterators_.erase(evict_key);

            A1out_.push_front(evict_key);
            types_[evict_key] = 'o';
            iterators_[evict_key] = A1out_.begin();

            if (A1out_.size() > kout_)
            {
                Key out_evict_key = A1out_.back();
                A1out_.pop_back();
                iterators_.erase(out_evict_key);
                types_.erase(out_evict_key);
            }
        }
    }

    void reclaim_for_Am()
    {
        if (Am_.size() >= km_)
        {
            Key evict_key = Am_.back();
            Am_.pop_back();
            values_.erase(evict_key);
            iterators_.erase(evict_key);
            types_.erase(evict_key);
        }
    }

public:
    TwoQCache(std::size_t capacity, double kin_ratio = 0.20, double kout_ratio = 0.60)
        : capacity_(capacity)
    {
        if (capacity < 2)
        {
            throw std::invalid_argument("Cache capacity must be >= 2");
        }
        
        kin_ = std::max<std::size_t>(1, static_cast<std::size_t>(capacity * kin_ratio));
        kout_ = std::max<std::size_t>(1, static_cast<std::size_t>(capacity * kout_ratio));
        km_ = std::max<std::size_t>(1, static_cast<std::size_t>(capacity * 0.20));
    }

    bool get(const Key& key, Value& value)
    {
        auto it_type = types_.find(key);

        if (it_type == types_.end())
        {
            return false;
        }

        char type = it_type->second;

        if (type == 'm')
        {
            auto it_list = iterators_[key];
            Am_.splice(Am_.begin(), Am_, it_list);
            iterators_[key] = Am_.begin();
            value = values_[key];
            return true;
        }

        if (type == 'i')
        {
            value = values_[key];
            return true;
        }

        return false;
    }

    void put(const Key& key, const Value& value)
    {
        auto it_type = types_.find(key);
        
        if (it_type != types_.end() && it_type->second == 'm')
        {
            values_[key] = value;
            auto it_list = iterators_[key];
            Am_.splice(Am_.begin(), Am_, it_list);
            iterators_[key] = Am_.begin();
            return;
        }

        if (it_type != types_.end() && it_type->second == 'i')
        {
            values_[key] = value;
            return;
        }

        if (it_type != types_.end() && it_type->second == 'o')
        {
            reclaim_for_Am();

            A1out_.erase(iterators_[key]);
            Am_.push_front(key);
            iterators_[key] = Am_.begin();
            types_[key] = 'm';
            values_[key] = value;
            return;
        }

        if (it_type == types_.end())
        {            
            reclaim_for_A1in();

            A1in_.push_front(key);
            iterators_[key] = A1in_.begin();
            types_[key] = 'i';
            values_[key] = value;
        }
    }
};

} // namespace cache