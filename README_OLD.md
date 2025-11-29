# 🧬 DNA Pattern Matching Algorithm Suite

## **What Does This Project Do?**

This is an **interactive DNA search engine** that finds genetic patterns in DNA sequences using 8 different algorithms. You can:
- 🔍 Search for specific DNA patterns (like "ACGT") in genetic sequences
- ⚡ Compare which algorithm is fastest
- 📊 Generate performance graphs showing speed comparisons
- 🧪 Test on real or randomly generated DNA data

**Think of it like:** A search engine (like Ctrl+F) but specialized for DNA, with 8 different search methods you can race against each other!

---

## 🎮 COMPLETE STEP-BY-STEP WALKTHROUGH (Like a Game Tutorial)

### **LEVEL 1: Installation & Setup** ⚙️

**Step 1.1** - Open your terminal and navigate to the project folder:
```bash
cd /path/to/Hashira
```

**Step 1.2** - Install prerequisites (one-time only):
```bash
# On Ubuntu/Debian:
sudo apt install build-essential python3 python3-pip

# Install Python graphing library:
pip install matplotlib
```

**Step 1.3** - Compile the project:
```bash
make
```
✅ **Success looks like:** You see "Build successful! Run with: ./bin/dna_pattern_matching"

**Step 1.4** - Create sample DNA data:
```bash
make sample
```
✅ **Success looks like:** "Sample FASTA file created: data/sample.fasta"

---

### **LEVEL 2: Running Your First Search** 🔍

**Step 2.1** - Start the program:
```bash
./bin/dna_pattern_matching
```

**Step 2.2** - You'll see this menu:
```
╔════════════════════════════════════════════════════════╗
║   DNA Pattern Matching Algorithm Suite (8 Algorithms)  ║
╚════════════════════════════════════════════════════════╝

=== Data Management ===
1. Load DNA sequence from FASTA file
2. Generate random DNA sequence

=== Exact Matching Algorithms ===
3. Run KMP Algorithm
4. Run Boyer-Moore Algorithm
...

Enter your choice:
```

**Step 2.3** - ⚠️ **CRITICAL:** Load data first! Type `1` and press Enter

**Step 2.4** - When asked "Enter FASTA filename:", type:
```
data/sample.fasta
```

✅ **Success looks like:** "Sequence loaded successfully! Length: 77 bp"

**Step 2.5** - Now search! Type `3` (for KMP algorithm) and press Enter

**Step 2.6** - When asked "Enter pattern to search:", type:
```
ACGT
```

**Step 2.7** - 🎉 **Results appear:**
```
✅ Found 8 matches
⏱️ Time taken: 0.002 ms
📍 Match positions: 0, 4, 8, 16, 20, 24, 33, 37

Context view:
Position 0: [ACGT]ACGTACGT...
```

✅ **Congrats!** You just searched DNA using the KMP algorithm!

---

### **LEVEL 3: Testing Different Algorithms** 🏁

Want to see which algorithm is fastest? Try this:

**Step 3.1** - Type `8` (Compare all algorithms)

**Step 3.2** - Type a pattern, like `ACGT`

**Step 3.3** - Watch the "race":
```
Running 6 algorithms...

Algorithm         | Time (ms) | Matches
------------------|-----------|--------
KMP               | 0.002     | 8
Boyer-Moore       | 0.001     | 8
Suffix Tree       | 0.000     | 8  ⭐ WINNER!
Shift-Or          | 0.001     | 8
Rabin-Karp        | 0.003     | 8
Z-Algorithm       | 0.002     | 8
```

✅ **Now you know:** Suffix Tree won this race!

---

### **LEVEL 4: Generating the Performance Graph** 📊

This creates a beautiful chart comparing all algorithms!

**Step 4.1** - Exit the program (type `0`)

**Step 4.2** - Run the Python benchmark script:
```bash
python3 bench/benchmark_runner.py
```

**Step 4.3** - Watch it work (takes 1-2 minutes):
```
Compiling...
Build successful!
Running benchmark for size 10000...
Running benchmark for size 50000...
Running benchmark for size 100000...
Running benchmark for size 500000...
Running benchmark for size 1000000...

Benchmark complete. Graph saved to bench/benchmark_results.png
```

**Step 4.4** - Open the graph:
```bash
xdg-open bench/benchmark_results.png
# Or on Mac: open bench/benchmark_results.png
```

**Step 4.5** - 🎨 **You'll see:** A colorful line graph with 6 colored lines showing how each algorithm performs as DNA sequences get longer!

✅ **What to look for:**
- **Flat lines = Fast** (like Suffix Tree - the green line)
- **Steep lines = Slower** (like Rabin-Karp - the purple line)
- **This graph goes in your report/presentation!**

---

### **LEVEL 5: Advanced Features** 🚀

#### **Generate Random DNA (No file needed)**

**Step 5.1** - Run the program: `./bin/dna_pattern_matching`

**Step 5.2** - Type `2` (Generate random DNA sequence)

**Step 5.3** - When asked "Enter sequence length:", type:
```
100000
```
✅ Now you have 100,000 random DNA letters to search!

#### **Fuzzy Searching (Find Similar Patterns)**

**Step 5.4** - Load data (Option 1 or 2)

**Step 5.5** - Type `7` (Levenshtein Distance Search)

**Step 5.6** - Type pattern: `ACGT`

**Step 5.7** - Type max distance: `1`

✅ **Result:** Finds `ACGT`, `ACGA`, `ACG`, `ACGTA`, etc. (1 letter different)

#### **Multi-Pattern Search (Find Many at Once)**

**Step 5.8** - Type `13` (Aho-Corasick)

**Step 5.9** - Type number of patterns: `3`

**Step 5.10** - Type patterns one by one:
```
ACGT
CGTA
TGAC
```

✅ **Result:** Finds all 3 patterns in one fast scan!

---

## 🎯 QUICK REFERENCE CARD

| I want to... | Press | Then... |
|--------------|-------|---------|
| Load a DNA file | `1` | Type `data/sample.fasta` |
| Generate random DNA | `2` | Type a number like `10000` |
| Search with KMP | `3` | Type pattern like `ACGT` |
| Search with Boyer-Moore | `4` | Type pattern |
| Race all algorithms | `8` | Type pattern |
| Make a graph | Exit, then run `python3 bench/benchmark_runner.py` |
| Find similar patterns | `7` | Type pattern + distance |
| Search multiple patterns | `13` | Type number, then patterns |
| Exit program | `0` | - |

---

## 🛠 Prerequisites

Before running, ensure you have:
- **C Compiler**: `gcc` (GNU Compiler Collection)
- **Build Tool**: `make`
- **Python 3** (Optional, for benchmarking): `python3` with `matplotlib`

Install on Ubuntu/Debian:
```bash
sudo apt install build-essential python3 python3-pip
pip install matplotlib
```

---

## 🔬 The 8 Algorithms (Quick Summary)

**Fast Exact Searchers:** 🎯
1. **KMP** - Reliable, never backtracks (Option 3)
2. **Boyer-Moore** - Usually fastest for long patterns (Option 4)
3. **Suffix Tree** - Instant search after building tree (Option 5)
4. **Shift-Or** - Lightning fast for short patterns (Option 6)
5. **Rabin-Karp** - Uses math/hashing (Option 11)
6. **Z-Algorithm** - Good at finding repeating patterns (Option 12)

**Special Purpose:** 🌟
7. **Levenshtein** - Finds similar patterns/mutations (Option 7)
8. **Aho-Corasick** - Searches many patterns at once (Option 13)

**Pro Tip:** Run Option 8 to race them all and see the fastest!

---

---

## 🏆 WHAT TO SAY ABOUT YOUR PROJECT

**"What does your project do?"**
*"It's a DNA pattern search engine with 8 different algorithms in C. You can search for genetic patterns, compare speeds, and generate performance graphs."*

**"How does it work?"**
*"Compile with `make`, run it, load a DNA file, type a pattern like 'ACGT', and it finds all matches instantly. You can race all algorithms or generate benchmark graphs."*

**"What's special?"**
*"It's 10-100x faster than Python, has an interactive menu, and includes automated benchmarking with visual graphs."*

---

## 📁 Project Structure

```
Hashira/
├── bin/dna_pattern_matching    ← The program you run
├── src/
│   ├── main.c                  ← Menu system
│   ├── algorithms/             ← 8 algorithm files
│   └── utils/                  ← File loading, utilities
├── include/pattern_matching.h  ← Function definitions
├── bench/
│   ├── benchmark_runner.py     ← Generates the graph
│   └── benchmark_results.png   ← The graph (created after benchmark)
├── data/sample.fasta           ← Test DNA file
└── Makefile                    ← Build instructions
```

---

---

## 📊 Understanding Your Results

**When you search:**
```
✅ Found 8 matches          ← How many times pattern appears
⏱️ Time taken: 0.002 ms    ← Speed (lower = better)
📍 Match positions: 0, 4, 8 ← Exact DNA locations
```

**When comparing algorithms (Option 8):**
- Smallest time = Fastest algorithm
- All should find the same number of matches

**In benchmark graph:**
- X-axis: DNA sequence length
- Y-axis: Time in ms (lower = faster)
- Typical winners: Suffix Tree or Boyer-Moore

---

## ❓ TROUBLESHOOTING

| Problem | Fix |
|---------|-----|
| `make: command not found` | `sudo apt install build-essential` |
| `./bin/...: No such file` | Run `make` first |
| `Error opening file` | Run `make sample` to create test file |
| `Please load sequence first!` | Press `1` or `2` before searching |
| `ModuleNotFoundError: matplotlib` | `pip install matplotlib` |

---

### **What to include:**
1. ✅ Screenshot of the interactive menu
2. ✅ The benchmark graph (`bench/benchmark_results.png`)
3. ✅ Example output showing a search result
4. ✅ Table comparing algorithm speeds

### **Key talking points:**
- "Implemented 8 algorithms in C for maximum performance"
- "Interactive CLI for easy testing"
- "Automated benchmarking with visual graphs"
- "10-100x faster than Python implementations"
- "Handles real FASTA files and generated test data"

### **Demo script for presentation:**
1. Show compilation: `make`
2. Run program, load data (Option 1)
3. Search with one algorithm (Option 3)
4. Compare all algorithms (Option 8)
5. Show the benchmark graph

---

## ❓ Troubleshooting

**Q: `make: command not found`**
*   **Fix**: Install build tools:
    ```bash
    sudo apt install build-essential
    ```

**Q: `./bin/dna_pattern_matching: No such file or directory`**
*   **Fix**: You haven't compiled yet! Run `make` first.

**Q: `Error opening file` when loading FASTA**
*   **Fix**: The file doesn't exist. Generate it:
    ```bash
    make sample
    ```

**Q: "Please load a sequence first!" when trying to search**
*   **Fix**: You must load data first! Press `1` or `2` before searching.

**Q: Python script fails with `ModuleNotFoundError: No module named 'matplotlib'`**
*   **Fix**: Install the plotting library:
    ```bash
    pip install matplotlib
    ```

**Q: How do I know if it's working correctly?**
*   **Fix**: Run Option 10 (Comprehensive test suite) - it validates all algorithms.

---

## 🚀 Complete Command Cheat Sheet

```bash
# ONE-TIME SETUP
sudo apt install build-essential python3 python3-pip
pip install matplotlib
cd /path/to/Hashira
make
make sample

# EVERY TIME YOU WANT TO USE IT
./bin/dna_pattern_matching
# Then follow the interactive menu!

# TO GENERATE BENCHMARK GRAPH
python3 bench/benchmark_runner.py
xdg-open bench/benchmark_results.png

# TO REBUILD FROM SCRATCH
make clean
make

# TO TEST IF EVERYTHING WORKS
./bin/dna_pattern_matching
# Type: 1 → data/sample.fasta → 3 → ACGT
```

---

## 📜 License & Academic Use

This project is designed for educational purposes, algorithm analysis, and bioinformatics research.

---

## 💡 FINAL TIPS

### **For Beginners:**
1. Start with the Quick Reference Card above
2. Always load data first (Option 1 or 2)
3. Try Option 3 (KMP) for your first search
4. Then try Option 8 to see all algorithms race

### **For Your Demo:**
1. Practice the flow: `make` → run → load → search → compare
2. Have the sample file ready (`make sample`)
3. Use simple patterns first (`ACGT`, `GC`)
4. Show the benchmark graph - it's impressive!

### **Common Mistakes:**
- ❌ Trying to search before loading data (must do Option 1 or 2 first!)
- ❌ Forgetting to compile (`make`)
- ❌ Not generating sample data (`make sample`)
- ❌ Typing wrong file path (use `data/sample.fasta`)

### **Pro Moves:**
- ✅ Use large random sequences (Option 2 with 1,000,000 length) to show performance
- ✅ Compare Option 8 results with the benchmark graph
- ✅ Show fuzzy search (Option 7) to demonstrate approximate matching
- ✅ Run the Python benchmark to prove C is faster

---

**🎉 That's it! You now know everything about this project. Go impress someone!** 🎉

# 📘 Compilation & Execution Guide

This guide explains exactly what happens when you compile and run the DNA Pattern Matching Suite.

---

## 🔨 1. Compilation Commands

These commands are run in your terminal to build the project.

| Command | What it does | When to use it |
|---------|--------------|----------------|
| `make` | **Builds the entire project.**<br>1. Creates `bin/` and `obj/` folders.<br>2. Compiles all C files.<br>3. Generates the executable `./bin/dna_pattern_matching`. | **First time** or after making changes to code. |
| `make clean` | **Deletes compiled files.**<br>Removes `bin/` and `obj/` folders. | If you want to restart the build from scratch. |
| `make sample` | **Generates test data.**<br>Creates `data/sample.fasta` with random DNA. | If you don't have a FASTA file to test with. |

---

## 🏃 2. Running the Program

To start the application, run:
```bash
./bin/dna_pattern_matching
```

### 🛑 IMPORTANT: The Golden Rule
**You MUST select Option 1 or Option 2 FIRST.**
The algorithms (Options 3-14) need a DNA sequence to search through. If you try to run them without loading data, the program will say:
> `Please load a sequence first!`

---

## 🎮 3. Step-by-Step Walkthrough

Here is exactly what happens when you run the program and interact with the menu.

### **Step 1: Start the Program**
You run `./bin/dna_pattern_matching`.
**Screen Output:**
```
╔════════════════════════════════════════════════════════╗
║   DNA Pattern Matching Algorithm Suite (8 Algorithms)  ║
╚════════════════════════════════════════════════════════╝

=== Exact Matching Algorithms ===
3. Run KMP Algorithm
...
=== Data Management ===
1. Load DNA sequence from FASTA file
2. Generate random DNA sequence
...
Enter your choice: 
```

### **Step 2: Load Data (MANDATORY)**
You **must** choose Option 1 or 2 before doing anything else.

#### **Option A: Load from File (Choice 1)**
1.  Type `1` and press Enter.
2.  Program asks: `Enter FASTA filename:`
3.  You type: `data/sample.fasta` (or your own file path).
4.  **Result:** The program reads the file into memory. It prints "Sequence loaded successfully".

#### **Option B: Generate Random Data (Choice 2)**
1.  Type `2` and press Enter.
2.  Program asks: `Enter sequence length:`
3.  You type: `1000` (or any number).
4.  **Result:** The program creates a random DNA string of that length in memory.

---

### **Step 3: Run an Algorithm**
Now that data is loaded, you can pick any algorithm (3-14).

#### **Example: Running KMP (Choice 3)**
1.  Type `3` and press Enter.
2.  Program prints a brief explanation of KMP.
3.  Program asks: `Enter pattern to search:`
4.  You type: `ACGT` (the DNA snippet you want to find).
5.  **Result:**
    *   The algorithm runs.
    *   It shows how many matches were found.
    *   It shows the time taken (e.g., `0.002 ms`).
    *   It shows the exact positions (indices) where `ACGT` appears.
    *   It prints a visual snippet showing the match in context.

---

### **Step 4: Compare All (Choice 8)**
This is the "Battle Mode".

1.  Type `8` and press Enter.
2.  Program asks: `Enter pattern to search:`
3.  You type: `ACGT`.
4.  **Result:**
    *   The program runs **ALL** exact matching algorithms one by one on the same data.
    *   It prints a table showing the time taken for each.
    *   It declares a "Winner" (the fastest algorithm).

---

## ❓ FAQ

**Q: Can I switch data in the middle?**
**A:** Yes! Just select Option 1 or 2 again. The old sequence will be cleared from memory, and the new one will be loaded.

**Q: What happens if I type a pattern that doesn't exist?**
**A:** The program will simply say `✅ Found 0 matches`.

**Q: What is Option 9 (Python Benchmark)?**
**A:** This runs the current C algorithm against Python's built-in regex engine to see if your C code is faster (it usually is!).


# 📖 Menu Options Explained

Here is a detailed explanation of every option in the main menu.

---

## 📂 Data Management (Start Here!)
**You must select one of these first.**

### `1. Load DNA sequence from FASTA file`
*   **What it does:** Reads a DNA sequence from a text file on your computer.
*   **Use this when:** You have a real dataset (like `data/sample.fasta`) that you want to analyze.

### `2. Generate random DNA sequence`
*   **What it does:** Creates a random string of A, C, G, T characters in the computer's memory.
*   **Use this when:** You just want to test the algorithms quickly without needing a file. You can specify any length (e.g., 1,000,000 characters).

---

## 🎯 Exact Matching Algorithms
**These find the pattern EXACTLY as you type it.**

### `3. Run KMP Algorithm (Knuth-Morris-Pratt)`
*   **How it works:** It scans the text from left to right. If it finds a mismatch, it knows exactly how far to jump ahead so it doesn't re-check characters it has already seen.
*   **Best for:** General purpose searching. It's the "standard" efficient algorithm.

### `4. Run Boyer-Moore Algorithm`
*   **How it works:** It tries to match the pattern from **right to left**. If it sees a character in the text that isn't in the pattern, it can skip the entire length of the pattern instantly.
*   **Best for:** Long patterns. It is often the **fastest** algorithm for normal text searches.

### `5. Run Suffix Tree Algorithm`
*   **How it works:** It spends time upfront to build a giant tree structure of the text. Once the tree is built, searching is instant.
*   **Best for:** If you have one huge text (like a genome) and you want to search for **thousands** of different patterns in it.

### `6. Run Shift-Or Algorithm`
*   **How it works:** It uses binary numbers (bits) to represent the pattern. It performs bitwise operations (AND, OR) to find matches.
*   **Best for:** Short patterns (less than 64 characters). It is incredibly fast because it uses the CPU's native bit logic.

### `11. Run Rabin-Karp Algorithm`
*   **How it works:** It turns the pattern into a number (hash). It then slides a window over the text, turning each window into a hash. If the numbers match, the text matches.
*   **Best for:** Detecting plagiarism or finding multiple patterns of the same length.

### `12. Run Z-Algorithm`
*   **How it works:** It calculates a "Z-value" for every character, which represents the length of the substring starting there that matches the prefix of the string.
*   **Best for:** Analyzing string structures and repetitions.

---

## 〰️ Approximate & Multi-Pattern Matching
**These are advanced search modes.**

### `7. Run Levenshtein Distance Search`
*   **What it does:** Finds "Fuzzy" matches. If you search for `ACGT` with distance 1, it will find `ACGT` (exact), `ACGA` (substitution), `ACG` (deletion), or `ACGTA` (insertion).
*   **Use this when:** You are looking for mutations or sequencing errors.

### `14. Run Shift-Or Approximate Search`
*   **What it does:** Also finds fuzzy matches, but uses the fast bitwise method (Shift-Or).
*   **Use this when:** You need fuzzy matching but Levenshtein is too slow, and your pattern is short.

### `13. Run Aho-Corasick`
*   **What it does:** Allows you to search for **multiple different patterns** at the same time.
*   **Example:** You can search for "GeneA", "GeneB", and "GeneC" all in one single pass through the text.

---

## 📊 Benchmarking & Testing
**Tools to measure performance.**

### `8. Compare all algorithms`
*   **What it does:** Runs options 3, 4, 5, 6, 11, and 12 one after another on your current data.
*   **Result:** Displays a table showing exactly how many milliseconds each one took, so you can see which is the winner.

### `9. Benchmark with Python regex`
*   **What it does:** Runs your C code, then runs a Python script doing the same search.
*   **Result:** Shows you how much faster your C program is compared to standard Python.

### `10. Run comprehensive test suite`
*   **What it does:** Runs a series of internal checks (unit tests) to make sure the algorithms are giving correct answers (e.g., "Does KMP find the pattern at index 0? Yes.").
*   **Use this when:** You have modified the code and want to make sure you didn't break anything.

---

### `0. Exit`
*   **What it does:** Closes the program.
