## 2Q_Cache

This project implements a 2Q (Two Queues) cache, an efficient caching algorithm that uses three queues (A1in, Am, and A1out) to manage data and optimize hit rates by filtering new items and promoting frequently accessed ones. 

# Run the program

1. **Clone the Repository**
```bash
   git clone https://github.com/Atlaxsyys/2Q_Cache.git
   cd 2Q_Cache
```

2. **Build the Project**

```bash
cmake -S . -B build
cmake --build build
```

3. **Run tests for 2q**
```bash
cd tests/2Q
./run_tests_2q.sh
```

4. **Run tests for ideal cache**

```bash
cd tests/ideal
./run_tests_ideal.sh
```

# Comparison 2Q algorithm with ideal


| Input data                    | 2Q Cache (hits) | Ideal Cache (hits) |
|-------------------------------|------------------|-------------------|
|10 11  1 2 3 4 1 2 5 6 3 4 3   | 1                | 5                 |
| 20 40 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10                    | 16                | 30                 |
| 8 25 1 2 3 4 5 5 4 1 1 2 3 4 1 7 8 1 2 6 4 9 1 2 3 4 5             | 2                | 16                 |
| 5 18 9 8 7 1 2 3 3 3 8 7 7 6 3 9 8 7 1 2 3               | 3                | 10                 |
|10 20 1 2 3 4 5 1 2 3 4 5 5 5 7 8 1 2 3 9 4 1 | 7              | 12                |