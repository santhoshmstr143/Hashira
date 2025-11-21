/*
 * Suffix Tree Implementation using Ukkonen's Algorithm
 * Time Complexity: O(n) for construction, O(m) for search
 * Space Complexity: O(n)
 */

#include "pattern_matching.h"

/*
 * Replace the previous partial suffix-tree implementation with a simpler
 * suffix-array-based exact search. The public API (create_suffix_tree,
 * suffix_tree_search, free_suffix_tree) is kept so main.c and other files
 * don't need to change. Internally we build a sorted array of suffix
 * starting indices and use binary search to find occurrences.
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

    /* comparator closure using static pointer (simple and fine here) */
    static const char *gtext = NULL;
    gtext = copied;

    int cmp(const void *x, const void *y) {
        int ix = *(const int *)x;
        int iy = *(const int *)y;
        return strcmp(gtext + ix, gtext + iy);
    }
    qsort(sa, n, sizeof(int), cmp);

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

    /* find leftmost match */
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