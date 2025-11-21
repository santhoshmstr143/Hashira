# 🧬 DNA Pattern Matching Algorithm Suite

A comprehensive, high-performance implementation of **8 different pattern matching algorithms** in C, designed specifically for DNA sequence analysis. This project includes interactive tools, benchmarking scripts, and detailed performance analysis.


## 🚀 Quick Start

If you just want to see it working immediately:

1.  **Open a terminal** in the project folder.
2.  **Compile the project**:
    ```bash
    make
    ```
3.  **Run the interactive tool**:
    ```bash
    ./bin/dna_pattern_matching
    ```
4.  **Follow the on-screen menu** to load data and run algorithms!

---

## 🛠 Prerequisites

Before running, ensure you have the following installed:

*   **C Compiler**: `gcc` (GNU Compiler Collection)
*   **Build Tool**: `make`
*   **Python 3** (Optional, for benchmarking graphs): `python3` with `matplotlib`
    *   Install matplotlib: `pip install matplotlib`

---

## 🔨 Compilation & Build

We use a `Makefile` to automate the build process. Here is exactly what happens when you run each command:

### 1. Build the Project
*   **Command:** `make`
*   **What it does:**
    1.  Creates a `bin/` folder (for the executable) and `obj/` folder (for temporary files).
    2.  Compiles all your C files (`main.c`, algorithms, utils).
    3.  Links them into a single program.
*   **Output:** You will see `gcc ...` commands running. If successful, it says nothing or "Build successful!".
*   **Result:** A new file is created: `./bin/dna_pattern_matching`.

### 2. Clean the Project
*   **Command:** `make clean`
*   **What it does:** Deletes the `bin/` and `obj/` folders.
*   **Output:** `rm -rf bin obj`
*   **Result:** Your project is "clean" (no compiled files). Use this if you want to rebuild from scratch.

### 3. Generate Sample Data
*   **Command:** `make sample`
*   **What it does:** Runs a script to generate a random DNA file.
*   **Result:** A file `data/sample.fasta` is created.

---

## 🏃 How to Run

There are two main ways to use this project: **Interactive Mode** (for exploring) and **Benchmark Mode** (for performance testing).

### 1. Interactive Mode (Default)

This is how you use the algorithms manually.

*   **Command:** `./bin/dna_pattern_matching`
*   **What it does:** Starts the interactive menu.
*   **What you see:**
    ```
    ╔════════════════════════════════════════════════════════╗
    ║   DNA Pattern Matching Algorithm Suite (8 Algorithms)  ║
    ╚════════════════════════════════════════════════════════╝
    ... (List of options 1-13) ...
    ```
*   **How to use it:**
    1.  **Type `1`** and press Enter to load the `data/sample.fasta` file.
    2.  **Type `3`** (for KMP) or `4` (for Boyer-Moore).
    3.  **Type a pattern** (e.g., `ACGT`).
*   **Result:** It prints:
    *   ✅ Found X matches
    *   ⏱️ Time taken: Y ms
    *   📍 Match positions: ...

### 2. Benchmark Mode (Python Automation)

This is how you generate the performance comparison graph.

*   **Command:** `python3 bench/benchmark_runner.py`
*   **What it does:**
    1.  It automatically runs your C program many times with different text lengths (10k, 50k, 100k...).
    2.  It measures how long each algorithm takes.
    3.  It draws a graph using Python's `matplotlib`.
*   **Output:** Text showing progress (`Running benchmark for size 10000...`).
*   **Result:** A new image file is saved at `bench/benchmark_results.png`. You can open this image to see the lines comparing KMP, Boyer-Moore, etc.

---

## 📁 Project Structure

This project is organized professionally to separate code, data, and documentation.

```
aad/
├── bin/                        # 🚀 Contains the compiled executable
│   └── dna_pattern_matching    # <--- RUN THIS FILE
│
├── src/                        # 📝 Source code
│   ├── main.c                  # Main menu and program logic
│   ├── algorithms/             # 🧠 Algorithm implementations
│   │   ├── kmp_algorithm.c
│   │   ├── boyer_moore_algorithm.c
│   │   ├── aho_corasick_algorithm.c
│   │   └── ... (other algorithms)
│   └── utils/                  # 🛠 Helper functions
│       ├── dna_sequence_handler.c
│       └── utils.c
│
├── include/                    # 📄 Header files
│   └── pattern_matching.h      # Function declarations
│
├── bench/                      # 📊 Benchmarking tools
│   ├── benchmark_runner.py     # Python script to generate graphs
│   └── benchmark_results.png   # Output graph (after running script)
│
├── data/                       # 💾 Data files
│   └── sample.fasta            # Sample DNA sequence
│
├── docs/                       # 📚 Documentation
│   └── report.tex              # Project report in LaTeX
│
├── obj/                        # ⚙️ Temporary object files (created by make)
├── Makefile                    # 🏗️ Build configuration
└── README.md                   # 📖 This guide
```

---

## 🔬 Algorithms Implemented

We have implemented 8 distinct algorithms, categorized by their strategy:

### **Exact Matching (Find exact occurrences)**

| Algorithm | Best For... | Complexity |
|-----------|-------------|------------|
| **KMP (Knuth-Morris-Pratt)** | General purpose searching. No backup needed. | O(n+m) |
| **Boyer-Moore** | Long patterns. Skips characters for speed. | O(n/m) avg |
| **Rabin-Karp** | Multiple pattern matching (using hashing). | O(n+m) avg |
| **Z-Algorithm** | Pattern preprocessing and prefix matching. | O(n+m) |
| **Shift-Or (Bitap)** | Short patterns (≤64 chars). Very fast bitwise ops. | O(n) |
| **Suffix Tree** | Heavy preprocessing, but instant searching. | O(m) search |

### **Approximate & Multi-Pattern Matching**

| Algorithm | Best For... | Complexity |
|-----------|-------------|------------|
| **Levenshtein Distance** | Fuzzy matching (finding "similar" patterns). | O(nm) |
| **Aho-Corasick** | Searching for MANY patterns at once. | O(n+m+z) |

---

## ❓ Troubleshooting

**Q: `make: command not found`**
*   **Fix**: You need to install build tools. On Ubuntu/Debian: `sudo apt install build-essential`.

**Q: `./bin/dna_pattern_matching: No such file or directory`**
*   **Fix**: You haven't compiled yet! Run `make` first.

**Q: `Error opening file` when loading FASTA**
*   **Fix**: Ensure `data/sample.fasta` exists. Run `make sample` to generate it if it's missing.

**Q: Python script fails with `ModuleNotFoundError: No module named 'matplotlib'`**
*   **Fix**: Install the plotting library: `pip install matplotlib`.

---

## 📜 License & Academic Use

This project is designed for educational purposes, algorithm analysis, and bioinformatics research.
Feel free to use this code for learning, benchmarking, or as a base for your own DNA analysis tools.

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
