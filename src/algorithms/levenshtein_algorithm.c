/*
 * Levenshtein Distance Search Implementation
 * Approximate/Fuzzy matching using dynamic programming
 * Time Complexity: O(nm * t) where t is text length
 * Space Complexity: O(nm)
 */

#include "pattern_matching.h"

#define MIN(a,b,c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein_distance(const char *s1, int len1, const char *s2, int len2) {
    // Base cases: if one string is empty, distance = length of other string
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;
    
    // Allocate two rows for space-optimized DP
    int *prev_row = (int *)malloc((len2 + 1) * sizeof(int));
    int *curr_row = (int *)malloc((len2 + 1) * sizeof(int));
    
    // Initialize first row: distance from empty string to s2[0..j]
    for (int j = 0; j <= len2; j++) {
        prev_row[j] = j;
    }
    
    // Fill DP table row by row
    for (int i = 1; i <= len1; i++) {
        curr_row[0] = i;  // Distance from s1[0..i] to empty string
        
        for (int j = 1; j <= len2; j++) {
            // Cost: 0 if characters match, 1 if they don't
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            
            // Take minimum of three operations
            curr_row[j] = MIN(
                prev_row[j] + 1,      // deletion: remove from s1
                curr_row[j-1] + 1,    // insertion: add to s1
                prev_row[j-1] + cost  // substitution: replace in s1
            );
        }
        
        // Swap rows for next iteration
        int *temp = prev_row;
        prev_row = curr_row;
        curr_row = temp;
    }
    
    // Result is in bottom-right cell (now in prev_row)
    int result = prev_row[len2];
    free(prev_row);
    free(curr_row);
    
    return result;
}

ApproximateMatchResult levenshtein_search(const char *text, const char *pattern, int max_distance) {
    // Initialize result structure
    ApproximateMatchResult result;
    result.matches = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;
    
    // Handle null inputs
    if (!text || !pattern) {
        return result;
    }
    
    clock_t start = clock();
    
    int n = strlen(text);
    int m = strlen(pattern);
    
    // Empty pattern matches nothing
    if (m == 0) {
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    // Allocate initial space for storing matches
    int capacity = 100;
    ApproximateMatch *matches = (ApproximateMatch *)malloc(capacity * sizeof(ApproximateMatch));
    if (!matches) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used = capacity * sizeof(ApproximateMatch);
    int count = 0;
    
    // Scan through each position in text
    // Check substrings of varying lengths to account for insertions/deletions
    for (int i = 0; i < n; i++) {
        // Calculate substring length range
        // Min: pattern length minus allowed deletions
        // Max: pattern length plus allowed insertions
        int min_len = m - max_distance;
        if (min_len < 1) min_len = 1;
        int max_len = m + max_distance;
        if (i + max_len > n) max_len = n - i;
        
        // Find best match distance at this position
        int best_distance = max_distance + 1;
        
        for (int len = min_len; len <= max_len && i + len <= n; len++) {
            int distance = levenshtein_distance(pattern, m, text + i, len);
            
            if (distance < best_distance) {
                best_distance = distance;
            }
        }
        
        // If match is within tolerance, save it
        if (best_distance <= max_distance) {
            // Expand array if needed
            if (count >= capacity) {
                capacity *= 2;
                ApproximateMatch *temp = (ApproximateMatch *)realloc(matches, 
                                         capacity * sizeof(ApproximateMatch));
                if (!temp) {
                    free(matches);
                    fprintf(stderr, "Memory reallocation failed\n");
                    return result;
                }
                matches = temp;
                result.memory_used += capacity * sizeof(ApproximateMatch) / 2;
            }
            matches[count].position = i;
            matches[count].distance = best_distance;
            count++;
        }
    }
    
    clock_t end = clock();
    
    // Populate final result
    result.matches = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}