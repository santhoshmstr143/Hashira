/*
 * Knuth-Morris-Pratt (KMP) Algorithm Implementation
 * Time Complexity: O(n + m)
 * Space Complexity: O(m)
 */

#include "pattern_matching.h"

/**
 * Computes the Longest Prefix Suffix (LPS) array.
 * lps[i] stores the length of the longest proper prefix of pattern[0..i]
 * that is also a suffix of pattern[0..i].
 */
void compute_lps_array(const char *pattern, int m, int *lps) {
    int len = 0;
    int i = 1;
    lps[0] = 0;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                // Fall back to the previous longest prefix length
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

/**
 * Performs KMP pattern matching on the given text.
 * Uses the LPS array to skip unnecessary comparisons.
 */
MatchResult kmp_search(const char *text, const char *pattern) {
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
    
    int *lps = (int *)malloc(m * sizeof(int));
    if (!lps) {
        fprintf(stderr, "Memory allocation failed for LPS array\n");
        return result;
    }
    
    result.memory_used += m * sizeof(int);
    
    compute_lps_array(pattern, m, lps);
    
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        free(lps);
        fprintf(stderr, "Memory allocation failed for matches\n");
        return result;
    }
    
    result.memory_used += capacity * sizeof(int);
    
    int count = 0;
    int i = 0; 
    int j = 0; 
    
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        
        if (j == m) {
            if (count >= capacity) {
                capacity *= 2;
                int *temp = (int *)realloc(matches, capacity * sizeof(int));
                if (!temp) {
                    free(lps);
                    free(matches);
                    fprintf(stderr, "Memory reallocation failed\n");
                    return result;
                }
                matches = temp;
                result.memory_used += capacity * sizeof(int) / 2;
            }
            matches[count++] = i - j;
            // Use LPS to shift pattern without re-scanning matched characters
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                // Mismatch after some matches: use LPS to skip
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    clock_t end = clock();
    
    free(lps);
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}

int verify_kmp_matches(const char *text, const char *pattern, const MatchResult *result) {
    int pattern_len = strlen(pattern);
    for (int i = 0; i < result->count; i++) {
        int pos = result->positions[i];
        if (strncmp(&text[pos], pattern, pattern_len) != 0) {
            return 0; // Invalid match found
        }
    }
    return 1; // All matches verified
}