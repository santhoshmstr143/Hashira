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
 * 
 * @param pattern The pattern string.
 * @param m Length of the pattern.
 * @param bad_char Array to store the last occurrence index of each character.
 */
void compute_bad_character(const char *pattern, int m, int bad_char[]) {
    // Initialize all occurrences as -1
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        bad_char[i] = -1;
    }
    
    // Fill the actual value of last occurrence of each character
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
    
    if (!border) {
        fprintf(stderr, "Memory allocation failed for border array\n");
        // Initialize with safe default values
        for (int i = 0; i < m; i++) {
            good_suffix[i] = m;
        }
        return;
    }
    
    // Initialize good_suffix array
    for (int i = 0; i < m; i++) {
        good_suffix[i] = m;
    }
    
    // Preprocessing for case 2
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
    
    // Preprocessing for case 1
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
 * 
 * @param str The string to validate.
 * @param len Length of the string.
 * @return 1 if valid DNA sequence, 0 otherwise.
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
    
    // Optional: Validate DNA sequences (uncomment if needed)
    /*
    if (!is_valid_dna(text, n)) {
        fprintf(stderr, "Error: Invalid DNA character in text\n");
        return result;
    }
    
    if (!is_valid_dna(pattern, m)) {
        fprintf(stderr, "Error: Invalid DNA character in pattern\n");
        return result;
    }
    */
    
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
    
    // Main search loop
    while (shift <= n - m) {
        int j = m - 1;
        
        // Match pattern from right to left
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        
        if (j < 0) {
            // Pattern found at position shift
            
            // Resize array if needed
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
                // Only add the NEW memory allocated
                result.memory_used += (capacity - old_capacity) * sizeof(int);
            }
            
            matches[count++] = shift;
            
            // Shift by 1 to find overlapping occurrences
            // This ensures we don't miss any matches
            shift += 1;
        } else {
            // Mismatch - use both heuristics and take maximum shift
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