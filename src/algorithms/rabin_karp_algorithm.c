/*
 * Rabin-Karp Algorithm Implementation
 * Uses rolling hash for efficient pattern matching
 * Time Complexity: O(n+m) average case, O(nm) worst case
 * Space Complexity: O(1)
 */

#include "pattern_matching.h"

#define PRIME 101
#define BASE 256

MatchResult rabin_karp_search(const char *text, const char *pattern) {
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
    
    // Allocate space for matches
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used = capacity * sizeof(int);
    
    // Calculate hash values
    unsigned long long pattern_hash = 0;
    unsigned long long text_hash = 0;
    unsigned long long h = 1;
    
    // Calculate h = BASE^(m-1) % PRIME
    for (int i = 0; i < m - 1; i++) {
        h = (h * BASE) % PRIME;
    }
    
    // Calculate initial hash values
    for (int i = 0; i < m; i++) {
        pattern_hash = (BASE * pattern_hash + (unsigned char)pattern[i]) % PRIME;
        text_hash = (BASE * text_hash + (unsigned char)text[i]) % PRIME;
    }
    
    int count = 0;
    
    // Slide pattern over text
    for (int i = 0; i <= n - m; i++) {
        // Check if hash values match
        if (pattern_hash == text_hash) {
            // Verify character by character
            int match = 1;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = 0;
                    break;
                }
            }
            
            if (match) {
                // Pattern found - resize if needed
                if (count >= capacity) {
                    capacity *= 2;
                    int *temp = (int *)realloc(matches, capacity * sizeof(int));
                    if (!temp) {
                        free(matches);
                        fprintf(stderr, "Memory reallocation failed\n");
                        return result;
                    }
                    matches = temp;
                    result.memory_used = capacity * sizeof(int);
                }
                matches[count++] = i;
            }
        }
        
        // Calculate rolling hash for next window
        if (i < n - m) {
            // Remove leading character and add trailing character
            unsigned long long old_char = ((unsigned char)text[i] * h) % PRIME;
            text_hash = (text_hash + PRIME - old_char) % PRIME;
            text_hash = (text_hash * BASE) % PRIME;
            text_hash = (text_hash + (unsigned char)text[i + m]) % PRIME;
        }
    }
    
    clock_t end = clock();
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}
