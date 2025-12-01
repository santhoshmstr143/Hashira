/*
 * Z-Algorithm Implementation
 * Linear time pattern matching using Z-array
 * Time Complexity: O(n+m) guaranteed linear time
 * Space Complexity: O(n+m)
 * Efficient for patterns with repetitive structure
 */

#include "pattern_matching.h"

static void compute_z_array(const char *str, int len, int *z) {
    int left = 0, right = 0;
    
    z[0] = len;
    
    // Iterate through the string to compute Z-values
    // [left, right] is the current "Z-box" (interval matching the prefix)
    for (int i = 1; i < len; i++) {
        if (i > right) {
            // Case 1: i is outside the current Z-box
            // Compute new Z-box manually by comparing characters
            left = right = i;
            while (right < len && str[right] == str[right - left]) {
                right++;
            }
            z[i] = right - left;
            right--;
        } else {
            // Case 2: i is inside the current Z-box
            // Use previously computed Z-values to optimize
            int k = i - left;
            if (z[k] < right - i + 1) {
                // Case 2a: The value fits entirely within the current Z-box
                z[i] = z[k];
            } else {
                // Case 2b: The value touches the boundary of the Z-box
                // Need to extend the search beyond the current Z-box
                left = i;
                while (right < len && str[right] == str[right - left]) {
                    right++;
                }
                z[i] = right - left;
                right--;
            }
        }
    }
}

MatchResult z_algorithm_search(const char *text, const char *pattern) {
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
    
    // Create concatenated string: pattern$text
    // This allows us to use the Z-array to find pattern occurrences in text
    int concat_len = m + n + 1;
    char *concat = (char *)malloc((concat_len + 1) * sizeof(char));
    if (!concat) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    strcpy(concat, pattern);
    concat[m] = '$';  // Special separator character that doesn't appear in DNA
    strcpy(concat + m + 1, text);
    
    int *z = (int *)calloc(concat_len, sizeof(int));
    if (!z) {
        free(concat);
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    compute_z_array(concat, concat_len, z);
    
    result.memory_used = concat_len * sizeof(char) + concat_len * sizeof(int);
    
    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    if (!matches) {
        free(concat);
        free(z);
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    int count = 0;
    
    // Check Z-array values corresponding to the text part
    // If Z-value equals pattern length, we found a match
    for (int i = m + 1; i < concat_len; i++) {
        if (z[i] == m) {
            if (count >= capacity) {
                capacity *= 2;
                int *temp = (int *)realloc(matches, capacity * sizeof(int));
                if (!temp) {
                    free(concat);
                    free(z);
                    free(matches);
                    fprintf(stderr, "Memory reallocation failed\n");
                    return result;
                }
                matches = temp;
            }
            matches[count++] = i - m - 1;
        }
    }
    
    clock_t end = clock();
    
    free(concat);
    free(z);
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    result.memory_used += capacity * sizeof(int);
    
    return result;
}

// Z-Algorithm for finding longest common prefix (useful for DNA sequence analysis)
void compute_lcp_using_z(const char *text, int n, int *lcp) {
    compute_z_array(text, n, lcp);
}
