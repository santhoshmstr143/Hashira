/*
 * Shift-Or (Bitap) Algorithm Implementation
 * Time Complexity: O(n)
 * Space Complexity: O(σ) where σ is alphabet size
 * Works for patterns up to word size (typically 64 bits)
 */

#include "pattern_matching.h"

#define ALPHABET_SIZE 256

/**
 * Performs Exact Shift-Or (Bitap) pattern matching.
 * Optimized for patterns <= 64 characters using 64-bit integers.
 */
MatchResult shift_or_search(const char *text, const char *pattern) {
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
    
    if (m == 0 || m > 64) { // Limitation: pattern length <= 64
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        if (m > 64) {
            fprintf(stderr, "Shift-Or: Pattern too long (max 64 characters)\n");
        }
        return result;
    }
    
    // Preprocessing: create pattern bitmask
    // For each character in alphabet, create a mask where the i-th bit is 0 
    // if the character appears at position i in the pattern, and 1 otherwise.
    unsigned long long pattern_mask[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pattern_mask[i] = ~0ULL;
    }
    
    for (int i = 0; i < m; i++) {
        pattern_mask[(unsigned char)pattern[i]] &= ~(1ULL << i);
    }
    
    result.memory_used = ALPHABET_SIZE * sizeof(unsigned long long);
    
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used += capacity * sizeof(int);
    
    int count = 0;
    // State bitmask: 0 bit at position i means prefix of length i+1 matches
    unsigned long long state = ~0ULL;
    unsigned long long match_mask = 1ULL << (m - 1);
    
    for (int i = 0; i < n; i++) {
        // Update state: shift left (introduce 0 at pos 0) and OR with char mask
        // A bit remains 0 only if it was 0 in previous state (shifted) AND 
        // the current character matches the pattern at that position.
        state = (state << 1) | pattern_mask[(unsigned char)text[i]];
        
        // If the (m-1)-th bit is 0, it means we have matched the entire pattern
        if ((state & match_mask) == 0) {
            if (count >= capacity) {
                capacity *= 2;
                int *temp = (int *)realloc(matches, capacity * sizeof(int));
                if (!temp) {
                    free(matches);
                    fprintf(stderr, "Memory reallocation failed\n");
                    return result;
                }
                matches = temp;
                result.memory_used += capacity * sizeof(int) / 2;
            }
            matches[count++] = i - m + 1;
        }
    }
    
    clock_t end = clock();
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}
