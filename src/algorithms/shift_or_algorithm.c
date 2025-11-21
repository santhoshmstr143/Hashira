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
 * 
 * @param text The DNA sequence to search in.
 * @param pattern The pattern to search for.
 * @return MatchResult structure containing positions, count, time, and memory usage.
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
    unsigned long long pattern_mask[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pattern_mask[i] = ~0ULL;
    }
    
    for (int i = 0; i < m; i++) {
        pattern_mask[(unsigned char)pattern[i]] &= ~(1ULL << i);
    }
    
    result.memory_used = ALPHABET_SIZE * sizeof(unsigned long long);
    
    // Allocate space for matches
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used += capacity * sizeof(int);
    
    int count = 0;
    unsigned long long state = ~0ULL;
    unsigned long long match_mask = 1ULL << (m - 1);
    
    for (int i = 0; i < n; i++) {
        state = (state << 1) | pattern_mask[(unsigned char)text[i]];
        
        if ((state & match_mask) == 0) {
            // Pattern found
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

/**
 * Performs Approximate Shift-Or (Wu-Manber) pattern matching.
 * Allows for up to k errors (insertions, deletions, substitutions).
 * 
 * @param text The DNA sequence to search in.
 * @param pattern The pattern to search for.
 * @param max_errors The maximum allowed edit distance (k).
 * @return ApproximateMatchResult structure containing matches with their error counts.
 */
ApproximateMatchResult shift_or_approximate_search(const char *text, const char *pattern, int max_errors) {
    ApproximateMatchResult result;
    result.matches = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;

    if (!text || !pattern) return result;

    clock_t start = clock();
    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0 || m > 63) { // Limit to 63 for safety with shifts
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        if (m > 63) fprintf(stderr, "Shift-Or Approx: Pattern too long (max 63 chars)\n");
        return result;
    }

    unsigned long long pattern_mask[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) pattern_mask[i] = ~0ULL;
    for (int i = 0; i < m; i++) pattern_mask[(unsigned char)pattern[i]] &= ~(1ULL << i);

    // R[k] stores the state for k errors
    // We need max_errors + 1 states
    unsigned long long *R = (unsigned long long *)malloc((max_errors + 1) * sizeof(unsigned long long));
    unsigned long long *old_R = (unsigned long long *)malloc((max_errors + 1) * sizeof(unsigned long long));
    
    if (!R || !old_R) {
        free(R); free(old_R);
        return result;
    }

    // Initialize states: R[k] has 1s everywhere except first k bits? 
    // Actually standard init: R[k] = ~0ULL << k? No.
    // R[k] represents matching prefix with k errors.
    // Initially, we haven't matched anything.
    // R[k] = ~0ULL (all 1s)
    for (int k = 0; k <= max_errors; k++) {
        R[k] = ~0ULL;
    }

    int capacity = 100;
    result.matches = (ApproximateMatch *)malloc(capacity * sizeof(ApproximateMatch));
    result.memory_used += (max_errors + 1) * 2 * sizeof(unsigned long long) + capacity * sizeof(ApproximateMatch);

    unsigned long long match_bit = 1ULL << (m - 1);

    for (int i = 0; i < n; i++) {
        unsigned long long char_mask = pattern_mask[(unsigned char)text[i]];
        
        // Save current R to old_R
        for (int k = 0; k <= max_errors; k++) old_R[k] = R[k];

        // Update R[0] (exact match)
        R[0] = (old_R[0] << 1) | char_mask;

        // Update R[1..max_errors]
        for (int k = 1; k <= max_errors; k++) {
            // Substitution: old_R[k-1] << 1
            // Insertion: old_R[k-1] << 1 (wait, insertion in text means we stay in pattern? No)
            // Deletion: old_R[k-1] (we advance pattern but not text? No, we advance text but not pattern?)
            
            // Wu-Manber Recurrence:
            // R[k] = (old_R[k] << 1) | char_mask   (Match or mismatch on this char with k errors so far)
            //      & (old_R[k-1] << 1)             (Substitution)
            //      & (old_R[k-1])                  (Insertion: consume text, stay in pattern)
            //      & (R[k-1] << 1)                 (Deletion: consume pattern, stay in text - effectively skip pattern char)
            
            // Note: R[k-1] is the *new* R[k-1] we just computed? Yes.
            
            R[k] = ((old_R[k] << 1) | char_mask) &
                   (old_R[k-1] << 1) &
                   (old_R[k-1]) &
                   (R[k-1] << 1);
        }

        // Check for match in R[max_errors] (or any R[k] if we want best match)
        // If bit m-1 is 0, we have a match ending at i
        if ((R[max_errors] & match_bit) == 0) {
            // Found a match with <= max_errors
            // To find exact number of errors, check R[0]...R[max_errors]
            int errors = max_errors;
            for (int k = 0; k < max_errors; k++) {
                if ((R[k] & match_bit) == 0) {
                    errors = k;
                    break;
                }
            }

            if (result.count >= capacity) {
                capacity *= 2;
                ApproximateMatch *temp = (ApproximateMatch *)realloc(result.matches, capacity * sizeof(ApproximateMatch));
                if (temp) result.matches = temp;
            }
            result.matches[result.count].position = i - m + 1;
            result.matches[result.count].distance = errors;
            result.count++;
        }
    }

    free(R);
    free(old_R);
    clock_t end = clock();
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    return result;
}