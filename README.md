# 🧬 DNA Pattern Matching Algorithm Suite

## **What Does This Project Do?**

This is an **interactive DNA search engine** that finds genetic patterns in DNA sequences using 8 different algorithms. You can:
- 🔍 Search for specific DNA patterns (like "ACGT") in genetic sequences
- ⚡ Compare which algorithm is fastest
- 📊 Generate performance graphs showing speed comparisons
- 🧪 Test on real or randomly generated DNA data

**Think of it like:** A search engine (like Ctrl+F) but specialized for DNA, with 8 different search methods you can race against each other!

---

## 🎮 COMPLETE STEP-BY-STEP WALKTHROUGH

### **LEVEL 1: Installation & Setup** ⚙️

**Step 1** - Navigate to project folder and install prerequisites:
```bash
cd /path/to/Hashira
sudo apt install build-essential python3 python3-pip
pip install matplotlib
```

**Step 2** - Compile and create sample data:
```bash
make
make sample
```
✅ **Success:** You see "Build successful!" and "Sample FASTA file created"

---

### **LEVEL 2: Running Your First Search** 🔍

**Step 1** - Start the program:
```bash
./bin/dna_pattern_matching
```

**Step 2** - ⚠️ **CRITICAL:** Load data first! Type `1` and press Enter

**Step 3** - Enter filename: `data/sample.fasta`

**Step 4** - Search! Type `3` (KMP algorithm)

**Step 5** - Enter pattern: `ACGT`

**Step 6** - 🎉 **Results:**
```
✅ Found 8 matches
⏱️ Time taken: 0.002 ms
📍 Match positions: 0, 4, 8, 16, 20, 24, 33, 37
```

---

### **LEVEL 3: Testing Different Algorithms** 🏁

**Step 1** - Type `8` (Compare all algorithms)

**Step 2** - Enter pattern: `ACGT`

**Step 3** - Watch the race:
```
Algorithm         | Time (ms) | Matches
------------------|-----------|--------
KMP               | 0.002     | 8
Boyer-Moore       | 0.001     | 8
Suffix Tree       | 0.000     | 8  ⭐ WINNER!
```

---

### **LEVEL 4: Generating the Performance Graph** 📊

**Step 1** - Exit program (type `0`)

**Step 2** - Run benchmark:
```bash
python3 bench/benchmark_runner.py
```

**Step 3** - Open the graph:
```bash
xdg-open bench/benchmark_results.png
```

✅ **You'll see:** A colorful graph comparing all algorithms across different DNA sequence sizes!

**What to look for:**
- **Flat lines = Fast** (Suffix Tree - green line)
- **Steep lines = Slower** (Rabin-Karp - purple line)

---

### **LEVEL 5: Advanced Features** 🚀

**Generate Random DNA:**
- Option `2` → Enter length (e.g., `100000`)

**Fuzzy Search (find similar patterns):**
- Option `7` → Pattern: `ACGT` → Distance: `1`
- Finds `ACGT`, `ACGA`, `ACG`, `ACGTA`, etc.

**Multi-Pattern Search:**
- Option `13` → Number: `3` → Enter 3 patterns
- Finds all patterns in one scan!

---

## 🎯 QUICK REFERENCE CARD

| I want to... | Press | Then... |
|--------------|-------|---------|
| Load a DNA file | `1` | Type `data/sample.fasta` |
| Generate random DNA | `2` | Type length like `10000` |
| Search with KMP | `3` | Type pattern like `ACGT` |
| Race all algorithms | `8` | Type pattern |
| Make benchmark graph | Exit → `python3 bench/benchmark_runner.py` |
| Find similar patterns | `7` | Pattern + distance |
| Search multiple patterns | `13` | Number + patterns |
| Exit | `0` | - |

---

## 🛠 Prerequisites

- **C Compiler**: `gcc`
- **Build Tool**: `make`
- **Python 3** with `matplotlib` (for benchmarking)

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
│   └── benchmark_results.png   ← The graph (after benchmark)
├── data/sample.fasta           ← Test DNA file
└── Makefile                    ← Build instructions
```

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

## 🎓 FOR YOUR REPORT/PRESENTATION

**What to include:**
1. Screenshot of interactive menu
2. Benchmark graph (`bench/benchmark_results.png`)
3. Example search output
4. Speed comparison table

**Demo script:**
```bash
make                              # 1. Show compilation
./bin/dna_pattern_matching        # 2. Run program
# Type: 1 → data/sample.fasta     # 3. Load data
# Type: 3 → ACGT                  # 4. Search with KMP
# Type: 8 → ACGT                  # 5. Compare all
python3 bench/benchmark_runner.py # 6. Show graph
```

---

## 🚀 Complete Command Cheat Sheet

```bash
# ONE-TIME SETUP
sudo apt install build-essential python3 python3-pip
pip install matplotlib
cd /path/to/Hashira
make
make sample

# EVERY TIME
./bin/dna_pattern_matching
# Follow: 1 → data/sample.fasta → 3 → ACGT

# GENERATE GRAPH
python3 bench/benchmark_runner.py
xdg-open bench/benchmark_results.png

# REBUILD
make clean && make
```

---

## 💡 FINAL TIPS

**For Beginners:**
- Always load data first (Option 1 or 2)
- Try Option 3 (KMP) for first search
- Then Option 8 to see all algorithms race

**Common Mistakes:**
- ❌ Searching before loading data
- ❌ Forgetting to compile (`make`)
- ❌ Wrong file path (use `data/sample.fasta`)

**Pro Moves:**
- ✅ Use large sequences (Option 2 with 1,000,000)
- ✅ Show fuzzy search (Option 7)
- ✅ Compare graph with Option 8 results

---

## 📜 License

Educational and research use. Open source for bioinformatics learning and algorithm analysis.

---

**🎉 That's it! You now know everything about this project. Go impress someone!** 🎉
