/*
 * Boyer-Moore Algorithm Implementation (Debugged)
 * Time Complexity: O(nm) worst case, O(n/m) best case
 * Space Complexity: O(m + σ)
 */

#include "pattern_matching.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ALPHABET_SIZE 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * Preprocesses the pattern to create the Bad Character Heuristic array.
 * Stores the index of the last occurrence of each character in the pattern.
 * This allows shifting the pattern to align the mismatching character in text
 * with its last occurrence in the pattern.
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
 * good_suffix[i] stores the shift distance when a mismatch occurs at index i-1.
 * It tries to align the matched suffix with another occurrence of the same suffix
 * in the pattern, or with a prefix of the pattern.
 */
void compute_good_suffix(const char *pattern, int m, int *good_suffix) {
    int *border = (int *)malloc((m + 1) * sizeof(int));
    
    if (!border) {
        fprintf(stderr, "Memory allocation failed for border array\n");
        for (int i = 0; i < m; i++) {
            good_suffix[i] = m;
        }
        return;
    }
    
    for (int i = 0; i < m; i++) {
        good_suffix[i] = m;
    }
    
    // Case 2: Suffix occurs elsewhere in pattern
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
    
    // Case 1: A prefix of the pattern matches a suffix of the pattern
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
 * Validates if the string contains only DNA characters (A, C, G, T).
 */
int is_valid_dna(const char *str, int len) {
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (c != 'A' && c != 'C' && c != 'G' && c != 'T' &&
            c != 'a' && c != 'c' && c != 'g' && c != 't') {
            return 0;
        }
    }
    return 1;
}

/**
 * Performs Boyer-Moore pattern matching using both Bad Character and Good Suffix heuristics.
 */
MatchResult boyer_moore_search(const char *text, const char *pattern) {
    MatchResult result;
    result.positions = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;
    
    // Check for NULL inputs
    if (!text || !pattern) {
        fprintf(stderr, "Error: NULL input detected\n");
        return result;
    }
    
    clock_t start = clock();
    
    int n = strlen(text);
    int m = strlen(pattern);
    
    // Check for empty pattern or pattern longer than text
    if (m == 0) {
        fprintf(stderr, "Error: Empty pattern\n");
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    if (m > n) {
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    // Preprocessing - Bad Character Heuristic
    int bad_char[ALPHABET_SIZE];
    result.memory_used += ALPHABET_SIZE * sizeof(int);
    compute_bad_character(pattern, m, bad_char);
    
    // Preprocessing - Good Suffix Heuristic
    int *good_suffix = (int *)malloc(m * sizeof(int));
    if (!good_suffix) {
        fprintf(stderr, "Memory allocation failed for good_suffix\n");
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    result.memory_used += m * sizeof(int);
    
    // Temporarily track border array memory (allocated inside compute_good_suffix)
    size_t temp_memory = (m + 1) * sizeof(int);
    result.memory_used += temp_memory;
    
    compute_good_suffix(pattern, m, good_suffix);
    
    // Border array is freed inside compute_good_suffix
    result.memory_used -= temp_memory;
    
    // Allocate initial space for matches
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        free(good_suffix);
        fprintf(stderr, "Memory allocation failed for matches array\n");
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    result.memory_used += capacity * sizeof(int);
    
    int count = 0;
    int shift = 0;
    
    // Slide the pattern over text
    while (shift <= n - m) {
        int j = m - 1;
        
        // Scan from right to left
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        
        if (j < 0) {
            // Pattern found at current shift
            if (count >= capacity) {
                int old_capacity = capacity;
                capacity *= 2;
                int *temp = (int *)realloc(matches, capacity * sizeof(int));
                if (!temp) {
                    free(good_suffix);
                    free(matches);
                    fprintf(stderr, "Memory reallocation failed\n");
                    result.positions = NULL;
                    result.count = 0;
                    clock_t end = clock();
                    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                    return result;
                }
                matches = temp;
                result.memory_used += (capacity - old_capacity) * sizeof(int);
            }
            
            matches[count++] = shift;
            
            // Shift pattern so that the next character in text aligns with the last occurrence in pattern
            // If not present, shift by pattern length + 1 (handled by bad_char logic implicitly or just shift 1)
            // For finding all occurrences, we can safely shift by 1 or use good suffix rule for full match
            shift += (shift + m < n) ? m - bad_char[(unsigned char)text[shift + m]] : 1;
        } else {
            // Mismatch occurred at index j
            // We take the maximum of two shifts:
            // 1. Bad Character Rule: Align text[shift+j] with its last occurrence in pattern
            // 2. Good Suffix Rule: Align the matched suffix with its recurrence in pattern
            int bad_char_shift = j - bad_char[(unsigned char)text[shift + j]];
            int good_suffix_shift = good_suffix[j];
            shift += MAX(bad_char_shift, good_suffix_shift);
        }
    }
    
    clock_t end = clock();
    
    // If no matches found, free the allocated array and set to NULL
    if (count == 0) {
        free(matches);
        matches = NULL;
        result.memory_used -= capacity * sizeof(int);
    } else if (count < capacity) {
        // Trim array to actual size to save memory
        int *temp = (int *)realloc(matches, count * sizeof(int));
        if (temp) {
            matches = temp;
            result.memory_used -= (capacity - count) * sizeof(int);
        }
        // If realloc fails, keep the original larger array
    }
    
    free(good_suffix);
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}