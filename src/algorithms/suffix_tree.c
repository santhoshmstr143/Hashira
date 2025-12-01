/*
 * Suffix Tree Implementation - Explicit Construction
 * 
 * A compressed trie of all suffixes. '$' terminator ensures uniqueness.
 * Construction: O(n²) - inserts all n suffixes with '$' terminator
 * Search: O(m + k) - tree walk + leaf collection
 * Space: O(n²) worst case
 */

#include "pattern_matching.h"

#define ALPHABET_SIZE 256
#define TERMINATOR '$'

// Edges are labeled with substrings via [start, end] indices into text
typedef struct SuffixTreeNode {
    struct SuffixTreeNode *children[ALPHABET_SIZE];
    int start;           // Start index in text for edge label
    int end;             // End index in text for edge label (inclusive)
    int suffix_index;    // For leaf nodes: starting position of suffix (-1 for internal)
} SuffixTreeNode;

static SuffixTreeNode* create_node(int start, int end) {
    SuffixTreeNode *node = (SuffixTreeNode*)malloc(sizeof(SuffixTreeNode));
    if (!node) return NULL;
    
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    
    node->start = start;
    node->end = end;
    node->suffix_index = -1;
    
    return node;
}

// Inserts suffix by walking tree and splitting edges when needed
// Returns: 0 on success, -1 on allocation failure
static int insert_suffix(SuffixTree *tree, int suffix_start) {
    SuffixTreeNode *current = tree->root;
    int text_pos = suffix_start;
    int text_len = tree->size;
    
    while (text_pos < text_len) {
        unsigned char c = (unsigned char)tree->text[text_pos];
        
        // Case 1: No edge for this character - create new leaf
        if (!current->children[c]) {
            SuffixTreeNode *leaf = create_node(text_pos, text_len - 1);
            if (!leaf) return -1;
            leaf->suffix_index = suffix_start;
            current->children[c] = leaf;
            return 0;
        }
        
        SuffixTreeNode *child = current->children[c];
        int edge_start = child->start;
        int edge_end = child->end;
        int edge_len = edge_end - edge_start + 1;
        
        // Match characters along edge until mismatch
        int matched = 0;
        while (matched < edge_len && 
               text_pos + matched < text_len && 
               tree->text[edge_start + matched] == tree->text[text_pos + matched]) {
            matched++;
        }
        
        // If matched entire edge, continue walking from child
        if (matched == edge_len) {
            current = child;
            text_pos += matched;
            continue;
        }
        
        // Mismatch in middle - split edge and create internal node
        SuffixTreeNode *split = create_node(edge_start, edge_start + matched - 1);
        if (!split) return -1;
        split->suffix_index = -1;
        
        // Attach old child and new leaf to split node
        unsigned char old_char = (unsigned char)tree->text[edge_start + matched];
        child->start = edge_start + matched;
        split->children[old_char] = child;
        
        unsigned char new_char = (unsigned char)tree->text[text_pos + matched];
        SuffixTreeNode *new_leaf = create_node(text_pos + matched, text_len - 1);
        if (!new_leaf) {
            free(split);
            return -1;
        }
        new_leaf->suffix_index = suffix_start;
        split->children[new_char] = new_leaf;
        
        current->children[c] = split;
        return 0;
    }
    
    current->suffix_index = suffix_start;
    return 0;
}

// Appends '$' terminator to ensure all suffixes end at unique leaf nodes
SuffixTree* create_suffix_tree(const char *text) {
    if (!text) return NULL;
    
    int original_len = strlen(text);
    
    SuffixTree *tree = (SuffixTree*)malloc(sizeof(SuffixTree));
    if (!tree) return NULL;
    
    tree->text = (char*)malloc(original_len + 2);
    if (!tree->text) {
        free(tree);
        return NULL;
    }
    
    strcpy(tree->text, text);
    tree->text[original_len] = TERMINATOR;
    tree->text[original_len + 1] = '\0';
    tree->size = original_len + 1;
    
    // Root has no incoming edge: start=-1, end=-1
    tree->root = create_node(-1, -1);
    if (!tree->root) {
        free(tree->text);
        free(tree);
        return NULL;
    }
    
    // Build tree by inserting each suffix (O(n²) construction)
    for (int i = 0; i < tree->size; i++) {
        if (insert_suffix(tree, i) < 0) {
            free_suffix_tree(tree);
            return NULL;
        }
    }
    
    return tree;
}

typedef struct {
    SuffixTreeNode *node;
} StackNode;

// Iterative DFS avoids stack overflow; excludes terminator-only suffix
static void collect_leaf_indices(SuffixTreeNode *root, int *matches, int *count, 
                                  int capacity, int original_text_len) {
    if (!root) return;
    
    int stack_capacity = 10000;
    StackNode *stack = (StackNode*)malloc(stack_capacity * sizeof(StackNode));
    if (!stack) return;
    
    int stack_top = 0;
    stack[stack_top++].node = root;
    
    while (stack_top > 0) {
        SuffixTreeNode *current = stack[--stack_top].node;
        
        // Leaf nodes have suffix_index >= 0; collect if not terminator
        if (current->suffix_index >= 0) {
            if (current->suffix_index < original_text_len && *count < capacity) {
                matches[(*count)++] = current->suffix_index;
            }
            continue;
        }
        
        // Internal node - add all children to stack
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i]) {
                if (stack_top >= stack_capacity) {
                    stack_capacity *= 2;
                    StackNode *new_stack = (StackNode*)realloc(stack, stack_capacity * sizeof(StackNode));
                    if (!new_stack) {
                        free(stack);
                        return;
                    }
                    stack = new_stack;
                }
                stack[stack_top++].node = current->children[i];
            }
        }
    }
    
    free(stack);
}

// Safe integer comparison for qsort (avoids overflow)
static int compare_int(const void *a, const void *b) {
    int ia = *(int*)a;
    int ib = *(int*)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

// Search: Walk tree O(m), collect leaves O(k), sort O(k log k)
MatchResult suffix_tree_search(SuffixTree *tree, const char *pattern) {
    MatchResult result;
    result.positions = NULL;
    result.count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;
    
    if (!tree || !pattern) return result;
    
    clock_t start = clock();
    
    int pattern_len = strlen(pattern);
    int original_text_len = tree->size - 1;
    
    if (pattern_len == 0 || pattern_len > original_text_len) {
        result.time_taken = ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    // Phase 1: Walk tree matching pattern
    SuffixTreeNode *current = tree->root;
    int pattern_pos = 0;
    
    while (pattern_pos < pattern_len) {
        unsigned char c = (unsigned char)pattern[pattern_pos];
        
        // No edge with this character means pattern not in tree
        if (!current->children[c]) {
            result.time_taken = ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000.0;
            return result;
        }
        
        SuffixTreeNode *child = current->children[c];
        int edge_start = child->start;
        int edge_end = child->end;
        int edge_len = edge_end - edge_start + 1;
        
        // Compare pattern with edge label character by character
        int matched = 0;
        while (matched < edge_len && pattern_pos < pattern_len) {
            if (tree->text[edge_start + matched] != pattern[pattern_pos]) {
                result.time_taken = ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000.0;
                return result;
            }
            matched++;
            pattern_pos++;
        }
        
        // Pattern can end mid-edge or at node - both valid
        if (pattern_pos == pattern_len) {
            current = child;
            break;
        }
        
        current = child;
    }
    
    // Phase 2: Collect all leaves in subtree
    int capacity = original_text_len;
    int *matches = (int*)malloc(capacity * sizeof(int));
    if (!matches) {
        result.time_taken = ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000.0;
        return result;
    }
    
    int count = 0;
    collect_leaf_indices(current, matches, &count, capacity, original_text_len);
    
    // Phase 3: Sort results
    if (count > 1) {
        qsort(matches, count, sizeof(int), compare_int);
    }
    
    clock_t end = clock();
    
    result.positions = matches;
    result.count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    result.memory_used = sizeof(SuffixTree) + (tree->size + 1) + (count * sizeof(int));
    
    return result;
}

static void free_node(SuffixTreeNode *node) {
    if (!node) return;
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            free_node(node->children[i]);
        }
    }
    free(node);
}

void free_suffix_tree(SuffixTree *tree) {
    if (!tree) return;
    
    free_node(tree->root);
    free(tree->text);
    free(tree);
}