#pragma once

#include <iostream>
#include <list>
#include <unordered_map>

template<typename Value, typename Key>

class 2Q_cache {
private:
    using ListIterator = typename std::list<Key>::iterator;

    std::size_t capacity_;
    std::size_t kin_;
    std::size_t kout_;

    std::list<Key> A1in_;
    std::list<Key> A1out_;
    std::list<Key> Am_;

    std::unordered_map<Key, Value> values;  // key -> value (only in cache)
    std::unordered_map<Value, ListIterator> iterators;  // key -> position
    std::unordered_map<Key, char> types;  // 'i' - A1in, 'm' - Am, 'o' - A1out

    
        void reclaimFor(const Key& key)
        {

        if (values_.size() < capacity_)
        {
            return; // Free slot available
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
}