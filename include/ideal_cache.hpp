#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace cache {

template<typename Key, typename Value>

class ideal_cache {
    private:
    std::size_t capacity_;
    std::size_t hits_ = 0;

    std::unordered_map<Key, Value> cache_;
    std::unordered_map<Key, std::vector<std::size_t>> future_positions_;
    
    std::size_t current_pos_ = 0;

    void build_future_positions(const std::vector<Key>& sequence)
    {
        std::unordered_map<Key, std::vector<std::size_t>> positions;

        for (std::size_t i = 0; i < sequence.size(); i++)
        {
            positions[sequence[i]].push_back(i);
        }

        future_positions_ = positions;
    }

    std::size_t next_use(const Key& key, std::size_t current_pos) const
    {
        static constexpr std::size_t INF = 1ULL << 60;

        auto it = future_positions_.find(key);
        if (it == future_positions_.end())
        {
            return INF;
        }

        const auto& pos_list = it->second;
        auto pos_it = std::lower_bound(pos_list.begin(), pos_list.end(), current_pos + 1);

        if (pos_it == pos_list.end())
        {
            return INF;
        }

        return *pos_it;
    }

    public:
    ideal_cache(std::size_t capacity, const std::vector<Key>& sequence)
    : capacity_(capacity), current_pos_(0) {
        build_future_positions(sequence);
    }

    bool get(const Key& key, Value& value, std::size_t pos)
    {
        auto it = cache_.find(key);

        if (it != cache_.end())
        {
            hits_++;
            value = it->second;
            current_pos_ = pos + 1;
            
            return true;
        }

        current_pos_ = pos + 1;
        return false;
    }

    void put(const Key& key, const Value& value, std::size_t pos)
    {
        auto it = cache_.find(key);

        if (it != cache_.end())
        {
            it->second = value;
            current_pos_ = pos + 1;

            return;
        }

        if (cache_.size() < capacity_)
        {
            cache_[key] = value;
            current_pos_ = pos + 1;

            return;
        }

        Key evict_key;
        std::size_t max_distance = 0;
        for (auto it = cache_.begin(); it != cache_.end(); it++)
        {
            std::size_t dist = next_use(it->first, pos);

            if (dist > max_distance)
            {
                max_distance = dist;
                evict_key = it->first;
            }
        }

        cache_.erase(evict_key);
        cache_[key] = value;
        current_pos_ = pos + 1;
    }

    std::size_t get_hits() const { return hits_; }
};

}