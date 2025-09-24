#include <iostream>

#include "cache.hpp"

int main()
{
    cache::TwoQCache<int, std::string> cache(4); // create cache: Key=int, Value=string, capacity=4 (kin=1, kout=2)
    cache.put(1, "one");   // added {1: "one"} в A1in
    cache.put(2, "two");

    std::string value;

    if (cache.get(1, value)) { std::cout << "Key 1: " << value << std::endl; }

    cache.put(3, "three");
    cache.put(4, "four");
    cache.put(5, "five");

    if (!cache.get(2, value)) { std::cout << "Key 2 not found" << std::endl; }

    cache.put(2, "two again");

    std::cout << "Cache size: " << cache.size() << std::endl;

    return 0;
}