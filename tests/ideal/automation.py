import subprocess

for i in range(1, 21):
    formatted_idx = f"{i:03d}"
    try:
        result = subprocess.run(['../../build/ideal_test'], stdin=open(f'{formatted_idx}.dat'), capture_output=True, text=True)
        with open(f'{formatted_idx}.sol') as f:
            expected = f"{f.read().strip()}"
        actual = result.stdout.strip()
        if actual != expected:
            print(f"Test {formatted_idx} failed: expected {expected}, got {actual}")
        else:
            print(f"Test {formatted_idx} passed")
        
    except Exception as e:
        print(f"Test {formatted_idx} failed with error: {e}")