#include <iostream>
#include <vector>
#include <string>

#include "cache.hpp"

int main()
{
    std::size_t capacity, count;
    std::cin >> capacity >> count;

    std::vector<int> sequence(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        std::cin >> sequence[i];
    }

    cache::TwoQCache<int, std::string> cache(capacity);

    std::size_t hits = 0;
    std::string value;

    for (std::size_t i = 0; i < count; ++i)
    {
        int key = sequence[i];

        if (cache.get(key, value))
        {
            hits++;
        }

        else
        {
            value = std::to_string(key);
            cache.put(key, value);
        }
    }

    std::cout << hits << std::endl;
    return 0;
}