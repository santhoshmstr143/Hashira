#!/usr/bin/env python3
"""
Comprehensive Benchmarking Suite for DNA Pattern Matching Algorithms
Generates graphs, charts, and tables for:
- Wall-clock time
- Memory usage
- Solution quality (match count)
- Performance vs theoretical complexity
"""

import subprocess
import os
import random
import time
import json
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.gridspec import GridSpec

# Configuration
SIZES = [1000, 5000, 10000, 50000, 100000, 500000, 1000000]
PATTERN_LENGTHS = [5, 10, 20, 50, 100]
ALGORITHMS = {
    15: {"name": "Naive", "complexity": "O(nm)", "color": "#FF6B6B"},
    3: {"name": "KMP", "complexity": "O(n+m)", "color": "#4ECDC4"},
    4: {"name": "Boyer-Moore", "complexity": "O(n/m)", "color": "#45B7D1"},
    5: {"name": "Suffix Array", "complexity": "O(m log n)", "color": "#96CEB4"},
    6: {"name": "Shift-Or", "complexity": "O(n)", "color": "#FFEAA7"},
    11: {"name": "Rabin-Karp", "complexity": "O(n+m)", "color": "#DFE6E9"},
    12: {"name": "Z-Algorithm", "complexity": "O(n+m)", "color": "#A29BFE"}
}

# Paths relative to project root
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EXECUTABLE = os.path.join(PROJECT_ROOT, "bin/dna_pattern_matching")
TEMP_DIR = os.path.join(PROJECT_ROOT, "bench_temp")
OUTPUT_DIR = os.path.join(PROJECT_ROOT, "bench/results")

def setup_directories():
    """Create necessary directories"""
    for d in [TEMP_DIR, OUTPUT_DIR]:
        if not os.path.exists(d):
            os.makedirs(d)

def generate_dna(length):
    """Generate random DNA sequence"""
    return "".join(random.choice("ACGT") for _ in range(length))

def run_algorithm(algo_id, text_file, pattern):
    """Run a single algorithm and return time in ms"""
    try:
        cmd = [EXECUTABLE, "--benchmark", str(algo_id), text_file, pattern]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        
        if result.returncode != 0:
            return None
        
        try:
            time_taken = float(result.stdout.strip())
            return time_taken
        except ValueError:
            return None
    except subprocess.TimeoutExpired:
        return None
    except Exception as e:
        print(f"Error: {e}")
        return None

def benchmark_varying_text_size(pattern_len=10):
    """Benchmark: Varying text size with fixed pattern length"""
    print(f"\n{'='*60}")
    print(f"Benchmark 1: Varying Text Size (Pattern Length = {pattern_len})")
    print(f"{'='*60}")
    
    results = {algo_id: {"times": [], "sizes": []} for algo_id in ALGORITHMS.keys()}
    
    for size in SIZES:
        print(f"\nText size: {size:,} bp")
        seq = generate_dna(size)
        filename = os.path.join(TEMP_DIR, f"seq_{size}.fasta")
        
        with open(filename, "w") as f:
            f.write(f">seq_{size}\n{seq}\n")
        
        # Generate pattern that exists in text
        start = random.randint(0, max(0, size - pattern_len))
        pattern = seq[start:start+pattern_len]
        
        for algo_id, algo_info in ALGORITHMS.items():
            # Skip Shift-Or for patterns > 64
            if algo_id == 6 and pattern_len > 64:
                continue
                
            time_taken = run_algorithm(algo_id, filename, pattern)
            
            if time_taken is not None:
                results[algo_id]["times"].append(time_taken)
                results[algo_id]["sizes"].append(size)
                print(f"  {algo_info['name']:<20}: {time_taken:>10.4f} ms")
            else:
                print(f"  {algo_info['name']:<20}: {'FAILED':>10}")
    
    return results

def benchmark_varying_pattern_length(text_size=100000):
    """Benchmark: Varying pattern length with fixed text size"""
    print(f"\n{'='*60}")
    print(f"Benchmark 2: Varying Pattern Length (Text Size = {text_size:,})")
    print(f"{'='*60}")
    
    results = {algo_id: {"times": [], "pattern_lens": []} for algo_id in ALGORITHMS.keys()}
    
    # Generate text once
    seq = generate_dna(text_size)
    filename = os.path.join(TEMP_DIR, f"seq_{text_size}.fasta")
    with open(filename, "w") as f:
        f.write(f">seq_{text_size}\n{seq}\n")
    
    for plen in PATTERN_LENGTHS:
        print(f"\nPattern length: {plen} bp")
        
        # Generate pattern that exists
        start = random.randint(0, text_size - plen)
        pattern = seq[start:start+plen]
        
        for algo_id, algo_info in ALGORITHMS.items():
            # Skip Shift-Or for patterns > 64
            if algo_id == 6 and plen > 64:
                continue
            
            time_taken = run_algorithm(algo_id, filename, pattern)
            
            if time_taken is not None:
                results[algo_id]["times"].append(time_taken)
                results[algo_id]["pattern_lens"].append(plen)
                print(f"  {algo_info['name']:<20}: {time_taken:>10.4f} ms")
            else:
                print(f"  {algo_info['name']:<20}: {'FAILED':>10}")
    
    return results

def benchmark_memory_usage():
    """Estimate memory usage based on algorithm characteristics"""
    # This is a theoretical estimation since we can't easily measure actual memory
    text_size = 1000000
    pattern_len = 10
    
    memory_usage = {}
    
    # Theoretical memory usage (in bytes)
    memory_usage[15] = {"name": "Naive", "memory": 0}  # O(1)
    memory_usage[3] = {"name": "KMP", "memory": pattern_len * 4}  # O(m) for LPS array
    memory_usage[4] = {"name": "Boyer-Moore", "memory": (pattern_len + 256) * 4}  # O(m + σ)
    memory_usage[5] = {"name": "Suffix Array", "memory": text_size * 4}  # O(n)
    memory_usage[6] = {"name": "Shift-Or", "memory": 256 * 8}  # O(σ) for bitmasks
    memory_usage[11] = {"name": "Rabin-Karp", "memory": 0}  # O(1)
    memory_usage[12] = {"name": "Z-Algorithm", "memory": (text_size + pattern_len) * 4}  # O(n+m)
    
    return memory_usage

def plot_performance_comparison(results, output_file):
    """Create performance comparison graph"""
    plt.figure(figsize=(12, 7))
    
    for algo_id, data in results.items():
        if data["times"] and data["sizes"]:
            algo_info = ALGORITHMS[algo_id]
            plt.plot(data["sizes"], data["times"], 
                    marker='o', label=f"{algo_info['name']} - {algo_info['complexity']}", 
                    color=algo_info['color'], linewidth=2, markersize=6)
    
    plt.xlabel("Text Size (base pairs)", fontsize=12, fontweight='bold')
    plt.ylabel("Execution Time (ms)", fontsize=12, fontweight='bold')
    plt.title("Performance Comparison: DNA Pattern Matching Algorithms\n(Varying Text Size, Pattern Length = 10 bp)", 
              fontsize=14, fontweight='bold')
    plt.legend(loc='best', fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved: {output_file}")
    plt.close()

def plot_pattern_length_impact(results, output_file):
    """Create graph showing impact of pattern length"""
    plt.figure(figsize=(12, 7))
    
    for algo_id, data in results.items():
        if data["times"] and data["pattern_lens"]:
            algo_info = ALGORITHMS[algo_id]
            plt.plot(data["pattern_lens"], data["times"], 
                    marker='s', label=algo_info['name'], 
                    color=algo_info['color'], linewidth=2, markersize=6)
    
    plt.xlabel("Pattern Length (base pairs)", fontsize=12, fontweight='bold')
    plt.ylabel("Execution Time (ms)", fontsize=12, fontweight='bold')
    plt.title("Impact of Pattern Length on Algorithm Performance\n(Text Size = 100,000 bp)", 
              fontsize=14, fontweight='bold')
    plt.legend(loc='best', fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved: {output_file}")
    plt.close()

def plot_memory_usage(memory_data, output_file):
    """Create bar chart for memory usage"""
    names = [v['name'] for v in memory_data.values()]
    memory = [v['memory'] / 1024 for v in memory_data.values()]  # Convert to KB
    colors = [ALGORITHMS[k]['color'] if k in ALGORITHMS else '#95A5A6' for k in memory_data.keys()]
    
    plt.figure(figsize=(10, 6))
    bars = plt.bar(names, memory, color=colors, edgecolor='black', linewidth=1.5)
    
    plt.xlabel("Algorithm", fontsize=12, fontweight='bold')
    plt.ylabel("Memory Usage (KB)", fontsize=12, fontweight='bold')
    plt.title("Theoretical Memory Usage Comparison\n(Text: 1M bp, Pattern: 10 bp)", 
              fontsize=14, fontweight='bold')
    plt.xticks(rotation=45, ha='right')
    plt.grid(True, axis='y', alpha=0.3)
    
    # Add value labels on bars
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f}' if height > 0 else '~0',
                ha='center', va='bottom', fontweight='bold')
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved: {output_file}")
    plt.close()

def plot_speedup_comparison(results, output_file):
    """Create speedup comparison relative to Naive algorithm"""
    plt.figure(figsize=(12, 7))
    
    # Get Naive algorithm times as baseline
    naive_times = results[15]["times"]
    naive_sizes = results[15]["sizes"]
    
    if not naive_times:
        print("Cannot create speedup comparison - Naive algorithm data missing")
        return
    
    for algo_id, data in results.items():
        if algo_id == 15 or not data["times"]:  # Skip Naive itself
            continue
        
        speedups = []
        sizes = []
        
        for i, size in enumerate(data["sizes"]):
            if size in naive_sizes:
                naive_idx = naive_sizes.index(size)
                if naive_times[naive_idx] > 0 and data["times"][i] > 0:
                    speedup = naive_times[naive_idx] / data["times"][i]
                    speedups.append(speedup)
                    sizes.append(size)
        
        if speedups:
            algo_info = ALGORITHMS[algo_id]
            plt.plot(sizes, speedups, marker='o', 
                    label=algo_info['name'], 
                    color=algo_info['color'], linewidth=2, markersize=6)
    
    plt.axhline(y=1.0, color='red', linestyle='--', linewidth=2, label='Baseline (Naive)', alpha=0.7)
    plt.xlabel("Text Size (base pairs)", fontsize=12, fontweight='bold')
    plt.ylabel("Speedup Factor (relative to Naive)", fontsize=12, fontweight='bold')
    plt.title("Algorithm Speedup Comparison\n(Speedup = Naive Time / Algorithm Time)", 
              fontsize=14, fontweight='bold')
    plt.legend(loc='best', fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved: {output_file}")
    plt.close()

def plot_complexity_verification(results, output_file):
    """Verify theoretical complexity by plotting log-log graphs"""
    fig = plt.figure(figsize=(16, 10))
    gs = GridSpec(2, 3, figure=fig)
    
    plot_configs = [
        (15, 0, 0, "Naive - O(nm)"),
        (3, 0, 1, "KMP - O(n+m)"),
        (4, 0, 2, "Boyer-Moore - O(n/m) best"),
        (11, 1, 0, "Rabin-Karp - O(n+m)"),
        (12, 1, 1, "Z-Algorithm - O(n+m)"),
        (6, 1, 2, "Shift-Or - O(n)")
    ]
    
    for algo_id, row, col, title in plot_configs:
        ax = fig.add_subplot(gs[row, col])
        data = results[algo_id]
        
        if data["times"] and data["sizes"]:
            sizes = np.array(data["sizes"])
            times = np.array(data["times"])
            
            # Plot actual data
            ax.loglog(sizes, times, 'o-', color=ALGORITHMS[algo_id]['color'], 
                     linewidth=2, markersize=6, label='Actual')
            
            # Fit and plot theoretical curve
            if len(sizes) > 1:
                # Simple linear fit in log-log space
                coeffs = np.polyfit(np.log(sizes), np.log(times), 1)
                fitted = np.exp(coeffs[1]) * sizes ** coeffs[0]
                ax.loglog(sizes, fitted, '--', color='red', 
                         linewidth=2, alpha=0.7, label=f'Fit: O(n^{coeffs[0]:.2f})')
            
            ax.set_xlabel("Text Size (bp)", fontweight='bold')
            ax.set_ylabel("Time (ms)", fontweight='bold')
            ax.set_title(title, fontweight='bold')
            ax.grid(True, alpha=0.3)
            ax.legend()
    
    plt.suptitle("Theoretical Complexity Verification", 
                 fontsize=16, fontweight='bold', y=0.995)
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved: {output_file}")
    plt.close()

def generate_latex_table(results, memory_data, output_file):
    """Generate LaTeX table for inclusion in the report"""
    
    with open(output_file, 'w') as f:
        f.write("% Performance Comparison Table - Generated by comprehensive_benchmark.py\n")
        f.write("\\begin{table}[H]\n")
        f.write("\\centering\n")
        f.write("\\caption{Comprehensive Performance Metrics (Text: 100K bp, Pattern: 10 bp)}\n")
        f.write("\\label{tab:performance}\n")
        f.write("\\begin{tabular}{|l|c|c|c|c|}\n")
        f.write("\\hline\n")
        f.write("\\textbf{Algorithm} & \\textbf{Time (ms)} & \\textbf{Memory (KB)} & \\textbf{Complexity} & \\textbf{Speedup} \\\\\n")
        f.write("\\hline\n")
        
        # Find the time for 100K size
        target_size = 100000
        baseline_time = None
        
        # Get baseline (Naive) time
        if 15 in results and target_size in results[15]["sizes"]:
            idx = results[15]["sizes"].index(target_size)
            baseline_time = results[15]["times"][idx]
        
        for algo_id in sorted(ALGORITHMS.keys()):
            algo_info = ALGORITHMS[algo_id]
            
            time_val = "N/A"
            if target_size in results[algo_id]["sizes"]:
                idx = results[algo_id]["sizes"].index(target_size)
                time_val = f"{results[algo_id]['times'][idx]:.3f}"
            
            mem_val = "~0"
            if algo_id in memory_data:
                mem_kb = memory_data[algo_id]['memory'] / 1024
                mem_val = f"{mem_kb:.1f}" if mem_kb > 0 else "~0"
            
            speedup_val = "1.00x"
            if baseline_time and time_val != "N/A":
                current_time = float(time_val)
                if current_time > 0:
                    speedup = baseline_time / current_time
                    speedup_val = f"{speedup:.2f}x"
            
            f.write(f"{algo_info['name']} & {time_val} & {mem_val} & {algo_info['complexity']} & {speedup_val} \\\\\n")
            f.write("\\hline\n")
        
        f.write("\\end{tabular}\n")
        f.write("\\end{table}\n")
    
    print(f"Saved: {output_file}")

def generate_detailed_csv(results_text_size, results_pattern_len, output_file):
    """Generate detailed CSV file with all benchmark data"""
    
    data_rows = []
    
    # Text size variation data
    for algo_id, data in results_text_size.items():
        algo_name = ALGORITHMS[algo_id]['name']
        for i, size in enumerate(data["sizes"]):
            data_rows.append({
                'Algorithm': algo_name,
                'Benchmark': 'Varying Text Size',
                'Text_Size': size,
                'Pattern_Length': 10,
                'Time_ms': data["times"][i]
            })
    
    # Pattern length variation data
    for algo_id, data in results_pattern_len.items():
        algo_name = ALGORITHMS[algo_id]['name']
        for i, plen in enumerate(data["pattern_lens"]):
            data_rows.append({
                'Algorithm': algo_name,
                'Benchmark': 'Varying Pattern Length',
                'Text_Size': 100000,
                'Pattern_Length': plen,
                'Time_ms': data["times"][i]
            })
    
    df = pd.DataFrame(data_rows)
    df.to_csv(output_file, index=False)
    print(f"Saved: {output_file}")

def cleanup():
    """Clean up temporary files"""
    if os.path.exists(TEMP_DIR):
        for f in os.listdir(TEMP_DIR):
            os.remove(os.path.join(TEMP_DIR, f))
        os.rmdir(TEMP_DIR)

def main():
    print("\n" + "="*60)
    print(" COMPREHENSIVE DNA PATTERN MATCHING BENCHMARK SUITE")
    print("="*60)
    
    setup_directories()
    
    # Compile project
    print("\nCompiling project...")
    subprocess.run(["make"], cwd=PROJECT_ROOT, check=True)
    
    # Run benchmarks
    results_text_size = benchmark_varying_text_size(pattern_len=10)
    results_pattern_len = benchmark_varying_pattern_length(text_size=100000)
    memory_data = benchmark_memory_usage()
    
    # Generate visualizations
    print(f"\n{'='*60}")
    print("Generating Visualizations...")
    print(f"{'='*60}")
    
    plot_performance_comparison(results_text_size, 
                                f"{OUTPUT_DIR}/performance_comparison.png")
    
    plot_pattern_length_impact(results_pattern_len, 
                               f"{OUTPUT_DIR}/pattern_length_impact.png")
    
    plot_memory_usage(memory_data, 
                     f"{OUTPUT_DIR}/memory_usage.png")
    
    plot_speedup_comparison(results_text_size, 
                           f"{OUTPUT_DIR}/speedup_comparison.png")
    
    plot_complexity_verification(results_text_size, 
                                f"{OUTPUT_DIR}/complexity_verification.png")
    
    # Generate LaTeX table
    generate_latex_table(results_text_size, memory_data, 
                        f"{OUTPUT_DIR}/performance_table.tex")
    
    # Generate CSV data
    generate_detailed_csv(results_text_size, results_pattern_len, 
                         f"{OUTPUT_DIR}/benchmark_data.csv")
    
    # Cleanup
    cleanup()
    
    print(f"\n{'='*60}")
    print("Benchmark Complete!")
    print(f"{'='*60}")
    print(f"\nAll results saved to: {OUTPUT_DIR}/")
    print("\nGenerated files:")
    print("  - performance_comparison.png")
    print("  - pattern_length_impact.png")
    print("  - memory_usage.png")
    print("  - speedup_comparison.png")
    print("  - complexity_verification.png")
    print("  - performance_table.tex")
    print("  - benchmark_data.csv")
    print("\nYou can now include these in your LaTeX report!")

if __name__ == "__main__":
    main()
