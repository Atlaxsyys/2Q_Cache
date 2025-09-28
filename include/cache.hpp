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

    std::size_t hits_ = 0;

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
        if (capacity_ == 0)   { throw std::invalid_argument("Cache capacity must be greater than 0" ); }
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
        ++hits_;
        return true;
    }
    else if (type == 'i')
    {
        // Stay in A1in
        value = values_[key];
        ++hits_;
        return true;
    }
else if (type == 'o')
{
    A1out_.erase(iterators_[key]); // Удаляем из A1out первым
    types_.erase(key);             // Удаляем тип
    reclaimFor(key);               // Освобождаем место, если нужно
    Am_.push_front(key);           // Добавляем в Am
    iterators_[key] = Am_.begin(); // Обновляем итератор
    types_[key] = 'm';             // Меняем тип
    return false;                  // Значение не было в кэше
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

    std::size_t get_hits() const { return hits_; }

    std::size_t size() const { return values_.size();  }
    bool empty()       const { return values_.empty(); }

    void dump() const
    {
        std::cout << "------------------DUMP------------------" << std::endl;
        std::cout << "\nCapacity: " << capacity_ << "\nKin: " << kin_  << "\nKout: " << kout_ << std::endl;

        std::cout << "\nA1in (size " << A1in_.size() << "): ";
        {
            auto it = A1in_.begin();
            auto end = A1in_.end();
            int first = 1;
            for (; it != end; ++it)
            {
                if (!first) std::cout << " ";
                std::cout << *it;
                first = 0;
            }
        }

        std::cout << std::endl;

        std::cout << "\n Am (size: " << Am_.size() << "): ";
        {
            auto it = Am_.begin();
            auto end = Am_.end();
            int first = 1;
            for (; it != end; ++it)
            {
                if (!first) std::cout << " ";
                std::cout << *it;
                first = 0;
            }
        }

        std::cout << std::endl;

    std::cout << "A1out ( size " << A1out_.size() << "): ";
    {
        auto it = A1out_.begin();
        auto end = A1out_.end();
        int first = 1;
        for (; it != end; ++it)
        {
            if (!first) std::cout << " ";
            std::cout << *it;
            first = 0;
        }
    }

    std::cout << std::endl;

    std::cout << "\nValues (only in cache):" << std::endl;
    {
        auto it = values_.begin();
        auto end = values_.end();
        for (; it != end; ++it)
        {
            std::cout << "Key " << it->first << ": " << it->second << std::endl;
        }
    }

    std::cout << "\nTypes:" << std::endl;

    {
        auto it = types_.begin();
        auto end = types_.end();
        for (; it != end; ++it)
        {
            std::cout << "Key " << it->first << ": " << it->second << std::endl;
        }
    }

    std::cout << "---------------------------------------" << std::endl;

    }
    
};

}