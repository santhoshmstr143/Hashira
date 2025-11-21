# DNA Pattern Matching Project - Presentation Outline

## Slide 1: Title Slide
- **Title**: A Comparative Analysis of String and Pattern Matching Algorithms on DNA Sequences
- **Team Members**: [Your Names]
- **Course**: Algorithm Analysis & Design
- **Date**: November 20, 2025

## Slide 2: Introduction & Problem Statement
- **Problem**: Finding specific nucleotide sequences (motifs) in massive genomic datasets (e.g., E. coli genome).
- **Relevance**: Crucial for gene identification, disease diagnosis, and evolutionary biology.
- **Objective**: Implement and benchmark a suite of exact and approximate matching algorithms to determine the most efficient approach for DNA analysis.

## Slide 3: Algorithms Overview (Exact Matching)
- **Knuth-Morris-Pratt (KMP)**: Linear time O(n+m). Uses LPS array to skip redundant comparisons.
- **Boyer-Moore**: Sub-linear average time. Skips characters using Bad Character & Good Suffix heuristics. Best for long patterns.
- **Suffix Array**: O(m log n) search. Preprocesses text for fast repeated queries. Space efficient alternative to Suffix Trees.
- **Shift-Or (Bitap)**: Bit-parallelism. O(n). Extremely fast for short patterns (<= 64 chars).

## Slide 4: Algorithms Overview (Approximate & Multi-Pattern) - BONUS
- **Levenshtein Distance**: Dynamic programming for fuzzy matching (edit distance).
- **Shift-Or Approximate (Wu-Manber)**: Bit-parallel fuzzy matching. Very fast for small error counts (k).
- **Aho-Corasick**: Searches for multiple patterns simultaneously in a single pass. O(n).

## Slide 5: Implementation Details
- **Language**: C (for performance) + Python (for benchmarking).
- **"From Scratch"**: All core logic implemented without external libraries.
- **Architecture**: Modular design with separate files for each algorithm.
- **Interface**: Interactive CLI with educational explanations and ANSI color visualization.

## Slide 6: Experimental Setup
- **Environment**: Linux, GCC, Python 3.
- **Dataset**: Randomly generated DNA sequences (10k to 1M bp) and FASTA file support.
- **Metrics**: Execution Time (ms) and Memory Usage.
- **Benchmark**: Automated script comparing C implementations vs Python `re` module.

## Slide 7: Results - Exact Matching
- *[Insert Graph from bench/benchmark_results.png]*
- **Key Finding 1**: Boyer-Moore outperforms others as pattern length increases.
- **Key Finding 2**: Shift-Or is fastest for short patterns due to bitwise operations.
- **Key Finding 3**: KMP provides stable, predictable linear performance.

## Slide 8: Results - Approximate & Multi-Pattern
- **Shift-Or Approx vs Levenshtein**: Shift-Or is significantly faster for small k (errors), while Levenshtein is more general.
- **Aho-Corasick**: Massive speedup when searching for >5 patterns compared to running KMP multiple times.

## Slide 9: Conclusion
- **Summary**: Successfully implemented and analyzed 8 algorithms.
- **Recommendation**: 
    - Use **Boyer-Moore** for general DNA search.
    - Use **Shift-Or** for short motifs.
    - Use **Aho-Corasick** for dictionary matching.
- **Future Work**: Parallelize algorithms using OpenMP/CUDA for whole-genome scale.

## Slide 10: Q&A
- Thank you!
- Questions?
