/*
 * Pattern Matching Algorithms Header File
 * Contains all function declarations and structures
 */

#ifndef PATTERN_MATCHING_H
#define PATTERN_MATCHING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to store match results
typedef struct {
    int *positions;          // Array of match positions
    int count;               // Number of matches found
    double time_taken;       // Time in milliseconds
    size_t memory_used;      // Memory used in bytes
} MatchResult;

// Structure for approximate matches
typedef struct {
    int position;
    int distance;
} ApproximateMatch;

typedef struct {
    ApproximateMatch *matches;
    int count;
    double time_taken;
    size_t memory_used;
} ApproximateMatchResult;

// KMP Algorithm
void compute_lps_array(const char *pattern, int m, int *lps);
MatchResult kmp_search(const char *text, const char *pattern);
int verify_kmp_matches(const char *text, const char *pattern, const MatchResult *result);

// Boyer-Moore Algorithm
void compute_bad_character(const char *pattern, int m, int bad_char[]);
void compute_good_suffix(const char *pattern, int m, int *good_suffix);
MatchResult boyer_moore_search(const char *text, const char *pattern);

// Suffix Tree Algorithm
typedef struct SuffixTreeNode {
    struct SuffixTreeNode **children;
    int start;
    int *end;
    int suffix_index;
    struct SuffixTreeNode *suffix_link;
} SuffixTreeNode;

typedef struct {
    char *text;
    SuffixTreeNode *root;
    SuffixTreeNode *active_node;
    int active_edge;
    int active_length;
    int remaining;
    int leaf_end;
    int size;
} SuffixTree;

/* For simplified suffix-array based search we store a suffix array here.
 * The existing API used by main.c expects create_suffix_tree/suffix_tree_search
 * to exist; to keep that stable we implement a suffix-array based backend
 * but keep the SuffixTree typedef name so the rest of the code compiles.
 */
typedef struct {
    char *text;
    int size;
    int *suffix_array; /* array of starting indices of sorted suffixes */
} SuffixArrayWrapper;

SuffixTree* create_suffix_tree(const char *text);
MatchResult suffix_tree_search(SuffixTree *tree, const char *pattern);
void free_suffix_tree(SuffixTree *tree);

/* Visual helpers */
void print_sequence_with_highlights(const char *sequence, const int *positions, int count, int pattern_len, int context);

// Shift-Or (Bitap) Algorithm
MatchResult shift_or_search(const char *text, const char *pattern);

// Levenshtein Distance Search
int levenshtein_distance(const char *s1, int len1, const char *s2, int len2);
ApproximateMatchResult levenshtein_search(const char *text, const char *pattern, int max_distance);

// DNA Sequence Handler
typedef struct {
    char *sequence;
    char *header;
    size_t length;
} DNASequence;

DNASequence* load_fasta_file(const char *filename);
void free_dna_sequence(DNASequence *seq);
char* generate_random_dna(int length);

// Rabin-Karp Algorithm
MatchResult rabin_karp_search(const char *text, const char *pattern);

// Z-Algorithm
MatchResult z_algorithm_search(const char *text, const char *pattern);
void compute_lcp_using_z(const char *text, int n, int *lcp);

// Aho-Corasick Algorithm (Multiple Pattern Matching)
typedef struct {
    int position;
    int pattern_id;
    int pattern_length;
} PatternMatch;

typedef struct {
    PatternMatch *matches;
    int match_count;
    double time_taken;
    size_t memory_used;
} MultiPatternResult;

MultiPatternResult aho_corasick_search(const char *text, const char **patterns, int pattern_count);
void free_multi_pattern_result(MultiPatternResult *result);
void print_multi_pattern_result(const char *algo_name, const MultiPatternResult *result, const char **patterns);

// Utility functions
void free_match_result(MatchResult *result);
void free_approximate_match_result(ApproximateMatchResult *result);
void print_match_result(const char *algo_name, const MatchResult *result);
void print_approximate_match_result(const char *algo_name, const ApproximateMatchResult *result);

#endif
