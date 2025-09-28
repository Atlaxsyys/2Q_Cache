#include <iostream>

#include "cache.hpp"

int main()
{
    std::size_t capacity = 0;
    std::size_t count = 0;
    std::cin >> capacity >> count;
    cache::TwoQCache<int, std::string> cache(capacity);

    std::string value;
    
    for (std::size_t i = 0; i < count; i++)
    {
        int key = 0;
        std::cin >> key;

        if (!cache.get(key, value))
        {
            value = std::to_string(key);
            std::cout << "\n\nWORK" << std::endl;
            cache.put(key, value);
        }
        
        cache.dump();
    }
    
    std::cout << "Hits: " << cache.get_hits() << std::endl;

    return 0;
}