/*
 * Naive (Brute Force) Algorithm Implementation
 * Time Complexity: O((n-m+1) * m)
 * Space Complexity: O(1)
 */

#include "pattern_matching.h"

/**
 * Performs Naive pattern matching on the given text.
 */
MatchResult naive_search(const char *text, const char *pattern) {
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
    
    // Allocate initial space for matches
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used += capacity * sizeof(int);
    
    int count = 0;
    
    // Iterate through every possible starting position in the text
    for (int i = 0; i <= n - m; i++) {
        int j;
        // For each position, check if the pattern matches character by character
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                break; // Mismatch found, move to next starting position
            }
        }
        
        if (j == m) {
            // Pattern fully matched
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
            matches[count++] = i;
        }
    }
    
    clock_t end = clock();
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}
