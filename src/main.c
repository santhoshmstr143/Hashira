/*
 * Main Program - DNA Pattern Matching Suite
 * Implements and benchmarks multiple pattern matching algorithms
 */

#define _GNU_SOURCE
#include "pattern_matching.h"
#include <regex.h>

void print_algorithm_info(int choice) {
    printf("\n📘 \033[1;34mAlgorithm Explanation:\033[0m\n");
    switch(choice) {
        case 3: // KMP
            printf("   \033[1mKnuth-Morris-Pratt (KMP)\033[0m is an efficient exact matching algorithm.\n");
            printf("   It preprocesses the pattern to create a 'Longest Prefix Suffix' (LPS) array.\n");
            printf("   This allows it to skip unnecessary comparisons when a mismatch occurs, ensuring\n");
            printf("   we never go backwards in the text. Time Complexity: O(n+m).\n");
            break;
        case 4: // Boyer-Moore
            printf("   \033[1mBoyer-Moore\033[0m is often the fastest algorithm for standard text.\n");
            printf("   It scans the pattern from right to left and uses two heuristics (Bad Character\n");
            printf("   and Good Suffix) to skip large chunks of text when a mismatch is found.\n");
            printf("   Time Complexity: Average O(n/m), Worst O(nm).\n");
            break;
        case 5: // Suffix Tree
            printf("   \033[1mSuffix Array/Tree\033[0m is a powerful data structure approach.\n");
            printf("   It preprocesses the *text* (not the pattern) to create a sorted index of all suffixes.\n");
            printf("   This allows us to find any pattern in time proportional only to the pattern length,\n");
            printf("   making it ideal for searching many patterns in the same genome.\n");
            break;
        case 6: // Shift-Or
            printf("   \033[1mShift-Or (Bitap)\033[0m uses bitwise operations to simulate a non-deterministic automaton.\n");
            printf("   It is extremely fast for short patterns (<= 64 chars) because it processes the text\n");
            printf("   using native CPU bit operations. Time Complexity: O(n).\n");
            break;
        case 11: // Rabin-Karp
            printf("   \033[1mRabin-Karp\033[0m uses hashing.\n");
            printf("   It calculates a 'fingerprint' (hash) for the pattern and for every window of text.\n");
            printf("   If hashes match, it checks the characters. It uses a 'rolling hash' to update\n");
            printf("   the hash value quickly as it moves through the text.\n");
            break;
        case 12: // Z-Algorithm
            printf("   \033[1mZ-Algorithm\033[0m computes a 'Z-array'.\n");
            printf("   For every position in the text, it calculates the length of the longest substring\n");
            printf("   starting there that matches the prefix of the string. It's a fundamental linear-time method.\n");
            break;
        case 7: // Levenshtein
            printf("   \033[1mLevenshtein Distance\033[0m finds 'fuzzy' matches.\n");
            printf("   It allows for errors (insertions, deletions, substitutions). We search for substrings\n");
            printf("   in the text that are within a specific 'edit distance' from your pattern.\n");
            break;
        case 14: // Shift-Or Approx
            printf("   \033[1mShift-Or Approximate (Wu-Manber)\033[0m is a bit-parallel algorithm for fuzzy matching.\n");
            printf("   It extends the exact Shift-Or algorithm to track multiple error states simultaneously\n");
            printf("   using bitwise logic. Very fast for short patterns with small error counts.\n");
            break;
        case 13: // Aho-Corasick
            printf("   \033[1mAho-Corasick\033[0m is designed for finding *multiple* patterns at once.\n");
            printf("   It builds a finite automaton (Trie) of all patterns with 'failure links'.\n");
            printf("   It scans the text once and tells us whenever *any* of the patterns are found.\n");
            break;
    }
    printf("   ---------------------------------------------------------\n");
}

void print_menu() {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║   DNA Pattern Matching Algorithm Suite (8 Algorithms)  ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n=== Exact Matching Algorithms ===\n");
    printf("3. Run KMP Algorithm\n");
    printf("4. Run Boyer-Moore Algorithm\n");
    printf("5. Run Suffix Tree Algorithm\n");
    printf("6. Run Shift-Or Algorithm\n");
    printf("11. Run Rabin-Karp Algorithm (Rolling Hash)\n");
    printf("12. Run Z-Algorithm\n");
    printf("\n=== Approximate/Multi-Pattern Matching ===\n");
    printf("7. Run Levenshtein Distance Search (Approximate)\n");
    printf("14. Run Shift-Or Approximate Search (Wu-Manber)\n");
    printf("13. Run Aho-Corasick (Multiple Pattern Matching)\n");
    printf("\n=== Benchmarking & Testing ===\n");
    printf("8. Compare all algorithms (Exact Match)\n");
    printf("9. Benchmark with Python regex comparison\n");
    printf("10. Run comprehensive test suite\n");
    printf("\n=== Data Management ===\n");
    printf("1. Load DNA sequence from FASTA file\n");
    printf("2. Generate random DNA sequence\n");
    printf("\n0. Exit\n");
    printf("\nEnter your choice: ");
}

MatchResult regex_search(const char *text, const char *pattern) {
    MatchResult result;
    result.positions = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;
    
    clock_t start = clock();
    
    regex_t regex;
    regmatch_t match;
    
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Failed to compile regex\n");
        return result;
    }
    
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    int count = 0;
    
    const char *p = text;
    int offset = 0;
    
    while (regexec(&regex, p, 1, &match, 0) == 0) {
        if (count >= capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(matches, capacity * sizeof(int));
            if (!temp) {
                free(matches);
                regfree(&regex);
                return result;
            }
            matches = temp;
        }
        
        matches[count++] = offset + match.rm_so;
        p += match.rm_so + 1;
        offset += match.rm_so + 1;
    }
    
    regfree(&regex);
    
    clock_t end = clock();
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}

void compare_all_algorithms(const char *text, const char *pattern) {
    printf("\n╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║        🏆 Comparing All Exact Matching Algorithms        ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    printf("\n  📏 Text length: %zu\n", strlen(text));
    printf("  📐 Pattern length: %zu\n", strlen(pattern));
    printf("  🔍 Pattern: %s\n", pattern);
    
    // KMP
    MatchResult kmp_result = kmp_search(text, pattern);
    print_match_result("KMP Algorithm", &kmp_result);
    
    // Boyer-Moore
    MatchResult bm_result = boyer_moore_search(text, pattern);
    print_match_result("Boyer-Moore Algorithm", &bm_result);
    
    // Suffix Tree
    printf("\n🌳 Building Suffix Tree...\n");
    clock_t st_start = clock();
    SuffixTree *tree = create_suffix_tree(text);
    clock_t st_end = clock();
    double construction_time = ((double)(st_end - st_start)) / CLOCKS_PER_SEC * 1000.0;
    printf("   Construction time: %.3f ms\n", construction_time);
    
    MatchResult st_result = suffix_tree_search(tree, pattern);
    print_match_result("Suffix Tree Algorithm", &st_result);
    free_suffix_tree(tree);
    
    // Shift-Or
    MatchResult so_result;
    so_result.count = 0;
    so_result.time_taken = 0;
    so_result.memory_used = 0;
    so_result.positions = NULL;
    
    if (strlen(pattern) <= 64) {
        so_result = shift_or_search(text, pattern);
        print_match_result("Shift-Or Algorithm", &so_result);
    } else {
        printf("\n⚠️  Shift-Or: Pattern too long (max 64 characters)\n");
    }
    
    // Rabin-Karp
    MatchResult rk_result = rabin_karp_search(text, pattern);
    print_match_result("Rabin-Karp Algorithm", &rk_result);
    
    // Z-Algorithm
    MatchResult z_result = z_algorithm_search(text, pattern);
    print_match_result("Z-Algorithm", &z_result);
    
    // Verify correctness
    printf("\n┌──────────────────────────────────┐\n");
    printf("│  ✅ Correctness Verification  │\n");
    printf("└──────────────────────────────────┘\n");
    printf("  KMP matches verified: %s\n", 
           verify_kmp_matches(text, pattern, &kmp_result) ? "✅ YES" : "❌ NO");
    
    int all_match = (kmp_result.count == bm_result.count && 
                     bm_result.count == st_result.count &&
                     st_result.count == so_result.count &&
                     so_result.count == rk_result.count &&
                     rk_result.count == z_result.count);
    printf("  All algorithms agree: %s\n", all_match ? "✅ YES" : "⚠️  NO");
    
    // Summary comparison
    printf("\n┌──────────────────────────────────────────────────────────────────────┐\n");
    printf("│        📊 Performance Summary (All Algorithms)          │\n");
    printf("└──────────────────────────────────────────────────────────────────────┘\n\n");
    printf("  %-20s | %10s | %15s | %15s\n", "Algorithm", "Matches", "Time (ms)", "Memory (bytes)");
    printf("  ───────────────────────────────────────────────────────────────\n");
    printf("  %-20s | %10d | %15.3f | %15zu\n", "KMP", kmp_result.count, 
           kmp_result.time_taken, kmp_result.memory_used);
    printf("  %-20s | %10d | %15.3f | %15zu\n", "Boyer-Moore", bm_result.count,
           bm_result.time_taken, bm_result.memory_used);
    printf("  %-20s | %10d | %15.3f | %15zu\n", "Suffix Tree", st_result.count,
           st_result.time_taken, st_result.memory_used);
    printf("  %-20s | %10d | %15.3f | %15zu\n", "Shift-Or", so_result.count,
           so_result.time_taken, so_result.memory_used);
    printf("  %-20s | %10d | %15.3f | %15zu\n", "Rabin-Karp", rk_result.count,
           rk_result.time_taken, rk_result.memory_used);
    printf("  %-20s | %10d | %15.3f | %15zu\n", "Z-Algorithm", z_result.count,
           z_result.time_taken, z_result.memory_used);
    
    // Find fastest
    double min_time = kmp_result.time_taken;
    const char *fastest = "KMP";
    
    if (bm_result.time_taken < min_time) {
        min_time = bm_result.time_taken;
        fastest = "Boyer-Moore";
    }
    if (st_result.time_taken < min_time) {
        min_time = st_result.time_taken;
        fastest = "Suffix Tree";
    }
    if (so_result.time_taken < min_time && strlen(pattern) <= 64) {
        min_time = so_result.time_taken;
        fastest = "Shift-Or";
    }
    if (rk_result.time_taken < min_time) {
        min_time = rk_result.time_taken;
        fastest = "Rabin-Karp";
    }
    if (z_result.time_taken < min_time) {
        min_time = z_result.time_taken;
        fastest = "Z-Algorithm";
    }
    
    printf("\n  🏆 Fastest algorithm: %s (%.3f ms)\n", fastest, min_time);
    
    // Cleanup
    free_match_result(&kmp_result);
    free_match_result(&bm_result);
    free_match_result(&st_result);
    free_match_result(&so_result);
    free_match_result(&rk_result);
    free_match_result(&z_result);
}

void run_comprehensive_tests() {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║           Comprehensive Test Suite                     ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    // Test 1: Simple pattern
    printf("\n--- Test 1: Simple Pattern ---\n");
    const char *text1 = "ACGTACGTACGT";
    const char *pattern1 = "ACG";
    compare_all_algorithms(text1, pattern1);
    
    // Test 2: Non-overlapping patterns
    printf("\n\n--- Test 2: Non-overlapping Pattern ---\n");
    const char *text2 = "AAAACAAAACAAAAC";
    const char *pattern2 = "AAAC";
    compare_all_algorithms(text2, pattern2);
    
    // Test 3: Overlapping patterns
    printf("\n\n--- Test 3: Overlapping Pattern ---\n");
    const char *text3 = "AAAAAAAAAA";
    const char *pattern3 = "AAA";
    compare_all_algorithms(text3, pattern3);
    
    // Test 4: No match
    printf("\n\n--- Test 4: No Match ---\n");
    const char *text4 = "ACGTACGTACGT";
    const char *pattern4 = "TTT";
    compare_all_algorithms(text4, pattern4);
    
    // Test 5: Approximate matching
    printf("\n\n--- Test 5: Approximate Matching (Levenshtein) ---\n");
    const char *text5 = "ACGTACGTTACGT";
    const char *pattern5 = "ACGT";
    printf("Text: %s\n", text5);
    printf("Pattern: %s\n", pattern5);
    printf("Max distance: 1\n");
    
    ApproximateMatchResult lev_result = levenshtein_search(text5, pattern5, 1);
    print_approximate_match_result("Levenshtein Distance Search", &lev_result);
    free_approximate_match_result(&lev_result);
}

// Helper to run benchmark mode
void run_benchmark_mode(int algo_id, const char *filename, const char *pattern) {
    DNASequence *seq = load_fasta_file(filename);
    if (!seq) {
        fprintf(stderr, "Error loading file\n");
        return;
    }

    MatchResult result;
    // Initialize result to avoid compiler warnings
    result.time_taken = -1.0;
    result.positions = NULL;
    result.count = 0;

    switch(algo_id) {
        case 3: result = kmp_search(seq->sequence, pattern); break;
        case 4: result = boyer_moore_search(seq->sequence, pattern); break;
        case 5: {
             // Suffix tree usually requires building, we'll just time the search + build if possible
             // But our current API separates them. For fair comparison, we might include build time or not.
             // Let's include build time as it's part of the "one-off" search cost if not pre-indexed.
             // Actually, our suffix_tree_search does everything.
             // Wait, let's check suffix_tree.c API.
             // It seems we replaced it with Suffix Array which might be inside a function.
             // Let's assume suffix_tree_search exists and works.
             // If not, we'll skip.
             // Checking header... SuffixTree* create... MatchResult suffix_tree_search...
             // We'll need to wrap it.
             SuffixTree *st = create_suffix_tree(seq->sequence);
             result = suffix_tree_search(st, pattern);
             free_suffix_tree(st);
             break;
        }
        case 6: result = shift_or_search(seq->sequence, pattern); break;
        case 11: result = rabin_karp_search(seq->sequence, pattern); break;
        case 12: result = z_algorithm_search(seq->sequence, pattern); break;
        default: fprintf(stderr, "Unknown algorithm\n"); break;
    }

    if (result.time_taken >= 0) {
        printf("%f\n", result.time_taken);
        free_match_result(&result);
    }
    free_dna_sequence(seq);
}

int main(int argc, char *argv[]) {
    // Check for benchmark mode
    if (argc == 5 && strcmp(argv[1], "--benchmark") == 0) {
        int algo_id = atoi(argv[2]);
        run_benchmark_mode(algo_id, argv[3], argv[4]);
        return 0;
    }

    DNASequence *sequence = NULL;
    char pattern[256];
    int choice;
    
    srand(time(NULL));
    
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) choice = -1;
        getchar(); // Consume newline
        
        switch (choice) {
            case 1: {
                char filename[256];
                char filepath[512];
                printf("Enter FASTA filename: ");
                if (scanf("%s", filename) != 1) filename[0] = '\0';
                getchar();
                
                // Try data/ directory first, then current directory
                snprintf(filepath, sizeof(filepath), "data/%s", filename);
                
                if (sequence) {
                    free_dna_sequence(sequence);
                }
                
                sequence = load_fasta_file(filepath);
                if (!sequence) {
                    // Try without data/ prefix
                    sequence = load_fasta_file(filename);
                }
                
                if (!sequence) {
                    printf("Failed to load sequence!\n");
                    printf("Tip: File should be in 'data/' folder or current directory\n");
                }
                break;
            }
            
            case 2: {
                int length;
                printf("Enter sequence length: ");
                if (scanf("%d", &length) != 1) length = 0;
                getchar();
                
                if (sequence) {
                    free_dna_sequence(sequence);
                }
                
                sequence = (DNASequence *)malloc(sizeof(DNASequence));
                sequence->header = strdup("Random DNA Sequence");
                sequence->sequence = generate_random_dna(length);
                sequence->length = length;
                
                printf("Generated random DNA sequence of length %d\n", length);
                printf("First 50 bases: %.50s\n", sequence->sequence);
                break;
            }
            
            case 3: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(3);
                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                MatchResult result = kmp_search(sequence->sequence, pattern);
                print_match_result("KMP Algorithm", &result);
                print_sequence_with_highlights(sequence->sequence, result.positions, result.count, strlen(pattern), 30);
                
                if (result.count > 0) {
                    printf("\nVerification: %s\n", 
                           verify_kmp_matches(sequence->sequence, pattern, &result) ? 
                           "All matches correct" : "Error in matches");
                }
                
                free_match_result(&result);
                break;
            }
            
            case 4: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(4);
                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                MatchResult result = boyer_moore_search(sequence->sequence, pattern);
                print_match_result("Boyer-Moore Algorithm", &result);
                print_sequence_with_highlights(sequence->sequence, result.positions, result.count, strlen(pattern), 30);
                free_match_result(&result);
                break;
            }
            
            case 5: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(5);
                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                printf("Building Suffix Tree (this may take a moment)...\n");
                clock_t start = clock();
                SuffixTree *tree = create_suffix_tree(sequence->sequence);
                clock_t end = clock();
                printf("Construction time: %.3f ms\n", 
                       ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);
                
                MatchResult result = suffix_tree_search(tree, pattern);
                print_match_result("Suffix Tree Algorithm", &result);
                print_sequence_with_highlights(sequence->sequence, result.positions, result.count, strlen(pattern), 30);
                
                free_suffix_tree(tree);
                free_match_result(&result);
                break;
            }
            
            case 6: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(6);
                printf("Enter pattern to search (max 64 chars): ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                MatchResult result = shift_or_search(sequence->sequence, pattern);
                print_match_result("Shift-Or Algorithm", &result);
                print_sequence_with_highlights(sequence->sequence, result.positions, result.count, strlen(pattern), 30);
                free_match_result(&result);
                break;
            }
            
            case 7: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(7);
                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                int max_dist;
                printf("Enter maximum edit distance: ");
                if (scanf("%d", &max_dist) != 1) max_dist = 1;
                getchar();
                
                ApproximateMatchResult result = levenshtein_search(
                    sequence->sequence, pattern, max_dist);
                print_approximate_match_result("Levenshtein Distance Search", &result);
                free_approximate_match_result(&result);
                break;
            }

            case 14: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(14);
                printf("Enter pattern to search (max 63 chars): ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                int max_dist;
                printf("Enter maximum edit distance: ");
                if (scanf("%d", &max_dist) != 1) max_dist = 1;
                getchar();
                
                ApproximateMatchResult result = shift_or_approximate_search(
                    sequence->sequence, pattern, max_dist);
                print_approximate_match_result("Shift-Or Approximate (Wu-Manber)", &result);
                free_approximate_match_result(&result);
                break;
            }
            
            case 8: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                compare_all_algorithms(sequence->sequence, pattern);
                break;
            }
            
            case 9: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }

                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();

                printf("\n=== Benchmark: KMP vs Python regex ===\n");

                MatchResult kmp = kmp_search(sequence->sequence, pattern);
                printf("\nKMP Algorithm:\n");
                printf("  Matches: %d\n", kmp.count);
                printf("  Time: %.3f ms\n", kmp.time_taken);

                /* Write sequence and pattern to temp files for the Python script */
                const char *text_tmp = "/tmp/aad_seq.txt";
                const char *pat_tmp = "/tmp/aad_pat.txt";
                FILE *ft = fopen(text_tmp, "w");
                FILE *fp = fopen(pat_tmp, "w");
                if (!ft || !fp) {
                    printf("Failed to create temporary files for python benchmark\n");
                } else {
                    fprintf(ft, "%s", sequence->sequence);
                    fprintf(fp, "%s", pattern);
                    fclose(ft);
                    fclose(fp);

                    char cmd[512];
                    snprintf(cmd, sizeof(cmd), "python3 bench/python_regex_bench.py %s %s", text_tmp, pat_tmp);
                    FILE *pipe = popen(cmd, "r");
                    if (!pipe) {
                        printf("Failed to run python benchmark\n");
                    } else {
                        int py_count = 0;
                        double py_time = 0.0;
                        if (fscanf(pipe, "%d %lf", &py_count, &py_time) == 2) {
                            printf("\nPython regex:\n");
                            printf("  Matches: %d\n", py_count);
                            printf("  Time: %.3f ms\n", py_time);

                            if (kmp.time_taken > 0.0 && py_time > 0.0) {
                                if (kmp.time_taken < py_time) {
                                    printf("\n✓ KMP is %.2fx faster than Python regex\n", py_time / kmp.time_taken);
                                } else {
                                    printf("\n✓ Python regex is %.2fx faster than KMP\n", kmp.time_taken / py_time);
                                }
                            }
                        } else {
                            printf("Python benchmark produced no output\n");
                        }
                        pclose(pipe);
                    }
                }

                free_match_result(&kmp);
                break;
            }
            
            case 10: {
                run_comprehensive_tests();
                break;
            }
            
            case 11: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(11);
                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                MatchResult result = rabin_karp_search(sequence->sequence, pattern);
                print_match_result("Rabin-Karp Algorithm (Rolling Hash)", &result);
                print_sequence_with_highlights(sequence->sequence, result.positions, result.count, strlen(pattern), 30);
                free_match_result(&result);
                break;
            }
            
            case 12: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(12);
                printf("Enter pattern to search: ");
                if (scanf("%s", pattern) != 1) pattern[0] = '\0';
                getchar();
                
                MatchResult result = z_algorithm_search(sequence->sequence, pattern);
                print_match_result("Z-Algorithm", &result);
                print_sequence_with_highlights(sequence->sequence, result.positions, result.count, strlen(pattern), 30);
                free_match_result(&result);
                break;
            }
            
            case 13: {
                if (!sequence) {
                    printf("Please load a sequence first!\n");
                    break;
                }
                
                print_algorithm_info(13);
                printf("\n=== Aho-Corasick Multiple Pattern Matching ===\n");
                printf("Enter number of patterns (max 10): ");
                int num_patterns;
                if (scanf("%d", &num_patterns) != 1) num_patterns = 0;
                getchar();
                
                if (num_patterns <= 0 || num_patterns > 10) {
                    printf("Invalid number of patterns!\n");
                    break;
                }
                
                char **patterns = (char **)malloc(num_patterns * sizeof(char *));
                for (int i = 0; i < num_patterns; i++) {
                    patterns[i] = (char *)malloc(256 * sizeof(char));
                    printf("Enter pattern %d: ", i + 1);
                    if (scanf("%s", patterns[i]) != 1) patterns[i][0] = '\0';
                    getchar();
                }
                
                MultiPatternResult result = aho_corasick_search(sequence->sequence, 
                                                                (const char **)patterns, 
                                                                num_patterns);
                print_multi_pattern_result("Aho-Corasick Algorithm", &result, 
                                          (const char **)patterns);
                
                free_multi_pattern_result(&result);
                for (int i = 0; i < num_patterns; i++) {
                    free(patterns[i]);
                }
                free(patterns);
                break;
            }
            
            case 0: {
                if (sequence) {
                    free_dna_sequence(sequence);
                }
                printf("\nThank you for using DNA Pattern Matching Suite!\n");
                return 0;
            }
            
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}