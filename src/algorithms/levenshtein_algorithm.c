/*
 * Levenshtein Distance Search Implementation
 * Approximate/Fuzzy matching using dynamic programming
 * Time Complexity: O(nm * t) where t is text length
 * Space Complexity: O(nm)
 */

#include "pattern_matching.h"

#define MIN(a,b,c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein_distance(const char *s1, int len1, const char *s2, int len2) {
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;
    
    int *prev_row = (int *)malloc((len2 + 1) * sizeof(int));
    int *curr_row = (int *)malloc((len2 + 1) * sizeof(int));
    
    // Initialize first row
    for (int j = 0; j <= len2; j++) {
        prev_row[j] = j;
    }
    
    for (int i = 1; i <= len1; i++) {
        curr_row[0] = i;
        
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            
            curr_row[j] = MIN(
                prev_row[j] + 1,      // deletion
                curr_row[j-1] + 1,    // insertion
                prev_row[j-1] + cost  // substitution
            );
        }
        
        int *temp = prev_row;
        prev_row = curr_row;
        curr_row = temp;
    }
    
    int result = prev_row[len2];
    free(prev_row);
    free(curr_row);
    
    return result;
}

ApproximateMatchResult levenshtein_search(const char *text, const char *pattern, int max_distance) {
    ApproximateMatchResult result;
    result.matches = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;
    
    if (!text || !pattern) {
        return result;
    }
    
    clock_t start = clock();
    
    int n = strlen(text);
    int m = strlen(pattern);
    
    if (m == 0) {
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    // Allocate space for matches
    int capacity = 100;
    ApproximateMatch *matches = (ApproximateMatch *)malloc(capacity * sizeof(ApproximateMatch));
    if (!matches) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    result.memory_used = capacity * sizeof(ApproximateMatch);
    int count = 0;
    
    // For each position in text, check substrings of varying lengths
    // Length can vary from (m - max_distance) to (m + max_distance)
    // to account for insertions and deletions
    for (int i = 0; i < n; i++) {
        // Try different substring lengths around the pattern length
        int min_len = m - max_distance;
        if (min_len < 1) min_len = 1;
        int max_len = m + max_distance;
        if (i + max_len > n) max_len = n - i;
        
        int best_distance = max_distance + 1;
        
        for (int len = min_len; len <= max_len && i + len <= n; len++) {
            int distance = levenshtein_distance(pattern, m, text + i, len);
            
            if (distance < best_distance) {
                best_distance = distance;
            }
        }
        
        if (best_distance <= max_distance) {
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
    
    result.matches = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}