#include <iostream>
#include <vector>
#include <string>
#include "ideal_cache.hpp"

int main()
{
    std::size_t capacity, count;
    std::cin >> capacity >> count;

    std::vector<int> sequence(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        std::cin >> sequence[i];
    }

    cache::ideal_cache<int, std::string> cache(capacity, sequence);
    std::string value;

    for (std::size_t i = 0; i < count; ++i)
    {
        int key = sequence[i];
        if (!cache.get(key, value, i)) 
        {
            value = std::to_string(key);
            std::cout << "\n\nWORK" << std::endl;
            cache.put(key, value, i);
        }
    }

    std::cout << "Hits: " << cache.get_hits() << std::endl;

    return 0;
}