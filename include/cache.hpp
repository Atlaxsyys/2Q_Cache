#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <stdexcept>

namespace cache {

template<typename Key, typename Value>

class TwoQCache {
private:
    using ListIterator = typename std::list<Key>::iterator;

    std::size_t capacity_;
    std::size_t kin_;
    std::size_t kout_;

    std::list<Key> A1in_;
    std::list<Key> A1out_;
    std::list<Key> Am_;

    std::unordered_map<Key, Value> values_;           // key -> value (only in cache)
    std::unordered_map<Key, ListIterator> iterators_; // key -> position
    std::unordered_map<Key, char> types_;            // 'i' - A1in, 'm' - Am, 'o' - A1out

    void reclaimFor(const Key& key)
    {
        if (values_.size() < capacity_)
        {
            return;
        }

        if (A1in_.size() > kin_)
        {
            // Evict from A1in
            Key evict_key = A1in_.back();
            A1in_.pop_back();
            values_.erase(evict_key);
            iterators_.erase(evict_key);
            
            // Add to A1out if not full
            if (A1out_.size() >= kout_)
            {
                Key out_tail = A1out_.back();
                A1out_.pop_back();
                types_.erase(out_tail);
            }

            A1out_.push_front(evict_key);
            types_[evict_key] = 'o';
            iterators_[evict_key] = A1out_.begin();

        }

        else
        {
            // Evict from Am
            Key evict_key = Am_.back();
            Am_.pop_back();
            values_.erase(evict_key);
            iterators_.erase(evict_key);
            types_.erase(evict_key);
        }
    }

public:
    TwoQCache(std::size_t capacity, std::size_t kin = 0, std::size_t kout = 0)
        : capacity_(capacity), 
          kin_(kin == 0 ? capacity / 4 : kin),
          kout_(kout == 0 ? capacity / 2 : kout)
    {
        if (capacity_ == 0)   { throw std::invalid_argument("Cache capacity must be greater than 0");  }
        if (kin_ > capacity_) { throw std::invalid_argument("A1in size (kin) cannot exceed capacity"); }
    }

    bool get(const Key& key, Value& value)
    {
        auto type_it = types_.find(key);
        
        if (type_it == types_.end())
        {
            return false; // Key not found
        }

        char type = type_it->second;
        
        if (type == 'm')
        {
            // Move to front of Am
            Am_.erase(iterators_[key]);
            Am_.push_front(key);
            iterators_[key] = Am_.begin();
            value = values_[key];
            return true;
        }

        else if (type == 'i')
        {
            // Stay in A1in
            value = values_[key];
            return true;
        }
        
        else if (type == 'o')
        {
            // Promote from A1out to Am
            reclaimFor(key);
            A1out_.erase(iterators_[key]);
            types_.erase(key);
            Am_.push_front(key);
            iterators_[key] = Am_.begin();
            types_[key] = 'm';
            return false; // Value not in cache yet
        }
        return false;
    }

    void put(const Key& key, const Value& value)
    {
        auto type_it = types_.find(key);

        if (type_it != types_.end())
        {
            char type = type_it->second;

            if (type == 'm')
            {
                // Update and move to front of Am
                Am_.erase(iterators_[key]);
                Am_.push_front(key);
                iterators_[key] = Am_.begin();
            }

            else if (type == 'i')
            {
                // Stay in A1in
            }
            
            else if (type == 'o')
            {
                // Promote to Am
                reclaimFor(key);
                A1out_.erase(iterators_[key]);
                Am_.push_front(key);
                iterators_[key] = Am_.begin();
                types_[key] = 'm';
            }

            values_[key] = value;
            return;
        }

        // New key
        reclaimFor(key);
        A1in_.push_front(key);
        iterators_[key] = A1in_.begin();
        types_[key] = 'i';
        values_[key] = value;
    }

    std::size_t size() const
    {
        return values_.size();
    }

    bool empty() const
    {
        return values_.empty();
    }
};

}