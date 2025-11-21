import subprocess
import os
import random
import time
import matplotlib.pyplot as plt
import sys

# Configuration
SIZES = [10000, 50000, 100000, 500000, 1000000]
ALGORITHMS = {
    3: "KMP",
    4: "Boyer-Moore",
    5: "Suffix Array",
    6: "Shift-Or",
    11: "Rabin-Karp",
    12: "Z-Algorithm"
}
PATTERN_LEN = 10
EXECUTABLE = "./bin/dna_pattern_matching"
TEMP_DIR = "bench_temp"

def generate_dna(length):
    return "".join(random.choice("ACGT") for _ in range(length))

def run_benchmark():
    if not os.path.exists(TEMP_DIR):
        os.makedirs(TEMP_DIR)

    # Compile first
    print("Compiling...")
    subprocess.run(["make"], check=True)

    results = {name: [] for name in ALGORITHMS.values()}
    
    print(f"{'Size':<10} | {'Algorithm':<15} | {'Time (ms)':<10}")
    print("-" * 40)

    for size in SIZES:
        # Generate data
        seq = generate_dna(size)
        filename = os.path.join(TEMP_DIR, f"seq_{size}.fasta")
        with open(filename, "w") as f:
            f.write(f">seq_{size}\n{seq}\n")
        
        # Pick a pattern that exists
        start = random.randint(0, size - PATTERN_LEN)
        pattern = seq[start:start+PATTERN_LEN]

        for algo_id, algo_name in ALGORITHMS.items():
            try:
                # Run C program
                cmd = [EXECUTABLE, "--benchmark", str(algo_id), filename, pattern]
                result = subprocess.run(cmd, capture_output=True, text=True)
                
                if result.returncode != 0:
                    print(f"Error running {algo_name}: {result.stderr}")
                    time_taken = 0
                else:
                    try:
                        time_taken = float(result.stdout.strip())
                    except ValueError:
                        print(f"Invalid output from {algo_name}: {result.stdout}")
                        time_taken = 0
                
                results[algo_name].append(time_taken)
                print(f"{size:<10} | {algo_name:<15} | {time_taken:<10.4f}")
                
            except Exception as e:
                print(f"Exception running {algo_name}: {e}")
                results[algo_name].append(0)

    # Plotting
    plt.figure(figsize=(10, 6))
    for name, times in results.items():
        plt.plot(SIZES, times, marker='o', label=name)
    
    plt.xlabel("DNA Sequence Length (bp)")
    plt.ylabel("Execution Time (ms)")
    plt.title("Exact Pattern Matching Performance Comparison")
    plt.legend()
    plt.grid(True)
    plt.savefig("bench/benchmark_results.png")
    print("\nBenchmark complete. Graph saved to bench/benchmark_results.png")

    # Clean up
    for f in os.listdir(TEMP_DIR):
        os.remove(os.path.join(TEMP_DIR, f))
    os.rmdir(TEMP_DIR)

if __name__ == "__main__":
    run_benchmark()
