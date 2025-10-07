import os
from collections import defaultdict
import random
import bisect

def simulate_ideal_cache(capacity, sequence):
    if not sequence:
        return 0
    
    future_positions = defaultdict(list)
    for i, key in enumerate(sequence):
        future_positions[key].append(i)
    
    INF = float('inf')
    
    def next_use(key, current_pos):
        if key not in future_positions:
            return INF
        pos_list = future_positions[key]
        idx = bisect.bisect_right(pos_list, current_pos)
        if idx < len(pos_list):
            return pos_list[idx]
        return INF
    
    cache = {}
    hits = 0
    
    for pos, key in enumerate(sequence):
        if key in cache:
            hits += 1
        else:
            value = str(key)
            if next_use(key, pos) == INF:
                continue
            
            if len(cache) < capacity:
                cache[key] = value
                continue
            
            max_distance = -1
            evict_key = None
            for k in cache:
                dist = next_use(k, pos)
                if dist > max_distance:
                    max_distance = dist
                    evict_key = k
            
            if evict_key is not None:
                del cache[evict_key]
                cache[key] = value
    
    return hits

def generate_tests(num_tests=20):
    tests = []
    
    fixed_tests = [
        (4, [1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4]),
        (3, [1, 2, 3, 5, 1, 2, 3]),
        (3, [1, 2, 3, 5, 1, 2, 3, 5]),
    ]
    
    for cap, seq in fixed_tests:
        hits = simulate_ideal_cache(cap, seq)
        tests.append((cap, seq, hits))
    
    random.seed(42)
    for i in range(num_tests - len(fixed_tests)):
        if i % 3 == 0:
            n = random.randint(5, 20)
            max_key = random.randint(3, 10)
            cap = random.randint(1, max_key // 2 + 1)
        elif i % 3 == 1:
            n = random.randint(50, 200)
            max_key = random.randint(10, 50)
            cap = random.randint(5, 20)
        else:
            n = random.randint(10000, 200000)
            max_key = random.randint(1000, 10000)
            cap = random.randint(10, 50)
        
        seq = [random.randint(1, max_key) for _ in range(n)]
        hits = simulate_ideal_cache(cap, seq)
        tests.append((cap, seq, hits))
    
    return tests

def save_tests(tests):
    for idx, (cap, seq, hits) in enumerate(tests, 1):
        formatted_idx = f"{idx:03d}"
        
        test_filename = f"{formatted_idx}.dat"
        with open(test_filename, 'w') as f:
            f.write(f"{cap} {len(seq)}\n")
            f.write(" ".join(map(str, seq)))
        
        answer_filename = f"{formatted_idx}.sol"
        with open(answer_filename, 'w') as f:
            f.write(f"{hits}")

if __name__ == "__main__":
    tests = generate_tests(20)
    save_tests(tests)