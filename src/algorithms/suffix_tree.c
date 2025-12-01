/*
 * Suffix Tree Implementation using Ukkonen's Algorithm
 * Time Complexity: O(n) for construction, O(m) for search
 * Space Complexity: O(n)
 */

#include "pattern_matching.h"

/* File-scoped helpers for suffix-array comparator */
static const char *sa_text_for_cmp = NULL;
static int sa_cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return strcmp(sa_text_for_cmp + ia, sa_text_for_cmp + ib);
}

static int int_cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

/*
 * Replace the previous partial suffix-tree implementation with a simpler
 * suffix-array-based exact search. The public API (create_suffix_tree,
 * suffix_tree_search, free_suffix_tree) is kept so main.c and other files
 * don't need to change. Internally we build a sorted array of suffix
 * starting indices and use binary search to find occurrences.
 * 
 * Note: While named "Suffix Tree" in the API for compatibility, this is 
 * technically a Suffix Array implementation which is more memory efficient.
 */

SuffixTree* create_suffix_tree(const char *text) {
    if (!text) return NULL;
    int n = strlen(text);

    SuffixTree *wrapper = (SuffixTree *)malloc(sizeof(SuffixTree));
    if (!wrapper) return NULL;

    char *copied = (char *)malloc(n + 1);
    if (!copied) {
        free(wrapper);
        return NULL;
    }
    strcpy(copied, text);

    int *sa = (int *)malloc(n * sizeof(int));
    if (!sa) {
        free(copied);
        free(wrapper);
        return NULL;
    }

    for (int i = 0; i < n; i++) sa[i] = i;

    /* comparator uses a file-scoped pointer to the copied text */
    sa_text_for_cmp = copied;
    qsort(sa, n, sizeof(int), sa_cmp);

    /* store copied text and suffix array pointer in wrapper fields */
    wrapper->text = copied;
    wrapper->size = n;
    wrapper->root = (SuffixTreeNode *)sa; /* use root as opaque storage */

    return wrapper;
}

MatchResult suffix_tree_search(SuffixTree *tree, const char *pattern) {
    MatchResult result;
    result.positions = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;

    if (!tree || !pattern) return result;

    clock_t start = clock();
    char *text = tree->text;
    int n = tree->size;
    int *sa = (int *)tree->root;
    int m = strlen(pattern);
    if (m == 0) {
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }

    /* Binary search to find the leftmost occurrence of the pattern in the sorted suffixes */
    int left = 0, right = n - 1, first = -1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strncmp(text + sa[mid], pattern, m);
        if (cmp >= 0) {
            if (cmp == 0) first = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    if (first == -1) {
        clock_t end = clock();
        result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }

    int capacity = 100;
    int *matches = (int *)malloc(capacity * sizeof(int));
    int count = 0;
    
    // Collect all contiguous matches in the suffix array starting from 'first'
    for (int i = first; i < n; i++) {
        if (strncmp(text + sa[i], pattern, m) == 0) {
            if (count >= capacity) {
                capacity *= 2;
                int *tmp = (int *)realloc(matches, capacity * sizeof(int));
                if (!tmp) break;
                matches = tmp;
            }
            matches[count++] = sa[i];
        } else break;
    }

    /* sort match positions ascending for predictable output */
    if (count > 1) qsort(matches, count, sizeof(int), int_cmp);

    clock_t end = clock();
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    result.memory_used = capacity * sizeof(int);
    return result;
}

void free_suffix_tree(SuffixTree *tree) {
    if (!tree) return;
    if (tree->text) free(tree->text);
    if (tree->root) free(tree->root); /* free suffix array */
    free(tree);
}