/*
 * Boyer-Moore Algorithm Implementation
 * Time Complexity: O(nm) worst case, O(n/m) best case
 * Space Complexity: O(m + σ)
 */

#include "pattern_matching.h"

#define ALPHABET_SIZE 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * Preprocesses the pattern to create the Bad Character Heuristic array.
 * 
 * @param pattern The pattern string.
 * @param m Length of the pattern.
 * @param bad_char Array to store the last occurrence index of each character.
 */
void compute_bad_character(const char *pattern, int m, int bad_char[]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        bad_char[i] = -1;
    }
    
    for (int i = 0; i < m; i++) {
        bad_char[(unsigned char)pattern[i]] = i;
    }
}

/**
 * Preprocesses the pattern to create the Good Suffix Heuristic array.
 * 
 * @param pattern The pattern string.
 * @param m Length of the pattern.
 * @param good_suffix Array to store the shift distance for each suffix.
 */
void compute_good_suffix(const char *pattern, int m, int *good_suffix) {
    int *border = (int *)malloc((m + 1) * sizeof(int));
    
    for (int i = 0; i < m; i++) {
        good_suffix[i] = m;
    }
    
    int i = m;
    int j = m + 1;
    border[i] = j;
    
    while (i > 0) {
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (good_suffix[j - 1] == m) {
                good_suffix[j - 1] = j - i;
            }
            j = border[j];
        }
        i--;
        j--;
        border[i] = j;
    }
    
    j = border[0];
    for (i = 0; i < m; i++) {
        if (good_suffix[i] == m) {
            good_suffix[i] = j;
        }
        if (i == j) {
            j = border[j];
        }
    }
    
    free(border);
}

/**
 * Performs Boyer-Moore pattern matching using both Bad Character and Good Suffix heuristics.
 * 
 * @param text The DNA sequence to search in.
 * @param pattern The pattern to search for.
 * @return MatchResult structure containing positions, count, time, and memory usage.
 */
MatchResult boyer_moore_search(const char *text, const char *pattern) {
    MatchResult result;
    result.positions = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;
    
    if (!text || !pattern) {
        return result;
    }
    
    clock_t start = clock();
    
    int n = strlen(text);
    int m = strlen(pattern);
    
    if (m == 0 || m > n) {
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    // Preprocessing
    int bad_char[ALPHABET_SIZE];
    int *good_suffix = (int *)malloc(m * sizeof(int));
    
    if (!good_suffix) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used += ALPHABET_SIZE * sizeof(int) + m * sizeof(int);
    
    compute_bad_character(pattern, m, bad_char);
    compute_good_suffix(pattern, m, good_suffix);
    
    // Allocate space for matches
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        free(good_suffix);
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used += capacity * sizeof(int);
    
    int count = 0;
    int shift = 0;
    
    while (shift <= n - m) {
        int j = m - 1;
        
        // Match pattern from right to left
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        
        if (j < 0) {
            // Pattern found
            if (count >= capacity) {
                capacity *= 2;
                int *temp = (int *)realloc(matches, capacity * sizeof(int));
                if (!temp) {
                    free(good_suffix);
                    free(matches);
                    fprintf(stderr, "Memory reallocation failed\n");
                    return result;
                }
                matches = temp;
                result.memory_used += capacity * sizeof(int) / 2;
            }
            matches[count++] = shift;
            shift += (shift + m < n) ? good_suffix[0] : 1;
        } else {
            // Mismatch - use heuristics
            int bad_char_shift = j - bad_char[(unsigned char)text[shift + j]];
            int good_suffix_shift = good_suffix[j];
            shift += MAX(bad_char_shift, good_suffix_shift);
        }
    }
    
    clock_t end = clock();
    
    free(good_suffix);
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}