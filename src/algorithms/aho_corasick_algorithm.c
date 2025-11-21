/*
 * Aho-Corasick Algorithm Implementation
 * Multiple pattern matching using trie and failure links
 * Time Complexity: O(n + m + z) where z is number of matches
 * Space Complexity: O(m * σ) where σ is alphabet size
 * Excellent for finding multiple DNA motifs simultaneously
 */

#include "pattern_matching.h"

#define ALPHABET_SIZE 256
#define MAX_PATTERNS 10

typedef struct ACNode {
    struct ACNode *children[ALPHABET_SIZE];
    struct ACNode *failure;
    int *output;
    int output_count;
} ACNode;

typedef struct {
    ACNode *root;
    char **patterns;
    int pattern_count;
} ACTrie;

static ACNode* create_ac_node() {
    ACNode *node = (ACNode *)calloc(1, sizeof(ACNode));
    if (!node) return NULL;
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    node->failure = NULL;
    node->output = NULL;
    node->output_count = 0;
    return node;
}

static void add_pattern(ACTrie *trie, const char *pattern, int pattern_id) {
    ACNode *current = trie->root;
    int len = strlen(pattern);
    
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)pattern[i];
        if (!current->children[c]) {
            current->children[c] = create_ac_node();
        }
        current = current->children[c];
    }
    
    // Mark end of pattern
    int *new_output = (int *)realloc(current->output, (current->output_count + 1) * sizeof(int));
    if (new_output) {
        current->output = new_output;
        current->output[current->output_count] = pattern_id;
        current->output_count++;
    }
}

static void build_failure_links(ACTrie *trie) {
    // BFS queue implementation using array
    ACNode **queue = (ACNode **)malloc(10000 * sizeof(ACNode *));
    int front = 0, rear = 0;
    
    // Initialize root's children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (trie->root->children[i]) {
            trie->root->children[i]->failure = trie->root;
            queue[rear++] = trie->root->children[i];
        }
    }
    
    // Build failure links using BFS
    while (front < rear) {
        ACNode *current = queue[front++];
        
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i]) {
                ACNode *child = current->children[i];
                queue[rear++] = child;
                
                ACNode *failure = current->failure;
                while (failure && !failure->children[i]) {
                    failure = failure->failure;
                }
                
                child->failure = failure ? failure->children[i] : trie->root;
                
                // Merge outputs
                // Note: We don't need to merge outputs here if we traverse failure links during search
                // But merging is an optimization to avoid traversing failure links for every character
                // However, the previous implementation had bugs with merging.
                // Let's stick to traversing failure links during search for correctness (as implemented in the search function now)
                // Or we can keep merging but ensure it's correct.
                // The search function I just updated traverses failure links to find ALL matches (including suffixes).
                // So we don't strictly need to merge outputs into the child node if we traverse up.
                // But if we want O(1) output reporting per node, we should merge.
                // Given the complexity, traversing up failure links in search is safer for correctness.
                // So I will remove the merging logic here to avoid duplication if I traverse up in search.
                // Wait, if I traverse up in search, I will find matches from failure nodes.
                // If I ALSO merge outputs here, I might report duplicates?
                // No, the search function traverses `temp = temp->failure`.
                // If `child->output` already contains `failure->output`, then `temp->output` will contain it.
                // And `temp->failure->output` will also contain it.
                // So we would report duplicates.
                // So I should REMOVE the output merging here if I am traversing failure links in search.
            }
        }
    }
    
    free(queue);
}

MultiPatternResult aho_corasick_search(const char *text, const char **patterns, int pattern_count) {
    MultiPatternResult result;
    result.matches = NULL;
    result.match_count = 0;
    result.time_taken = 0.0;
    result.memory_used = 0;
    
    if (!text || !patterns || pattern_count == 0) {
        return result;
    }
    
    clock_t start = clock();
    
    // Build AC trie
    ACTrie trie;
    trie.root = create_ac_node();
    trie.patterns = (char **)patterns;
    trie.pattern_count = pattern_count;
    
    // Add all patterns to trie
    for (int i = 0; i < pattern_count; i++) {
        add_pattern(&trie, patterns[i], i);
    }
    
    // Build failure links
    build_failure_links(&trie);
    
    // Search in text
    int capacity = 100;
    result.matches = (PatternMatch *)malloc(capacity * sizeof(PatternMatch));
    int count = 0;
    
    ACNode *current = trie.root;
    int text_len = strlen(text);
    
    for (int i = 0; i < text_len; i++) {
        unsigned char c = (unsigned char)text[i];
        
        // Follow failure links if needed
        while (current != trie.root && !current->children[c]) {
            current = current->failure;
        }
        
        if (current->children[c]) {
            current = current->children[c];
        } else {
            current = trie.root;
        }
        
        // Check for matches
        ACNode *temp = current;
        while (temp != trie.root) {
            if (temp->output) {
                for (int j = 0; j < temp->output_count; j++) {
                    int pattern_id = temp->output[j];
                    int pattern_len = strlen(patterns[pattern_id]);
                    
                    if (count >= capacity) {
                        capacity *= 2;
                        PatternMatch *new_matches = (PatternMatch *)realloc(result.matches, 
                                                                     capacity * sizeof(PatternMatch));
                        if (new_matches) {
                            result.matches = new_matches;
                        }
                    }
                    
                    result.matches[count].position = i - pattern_len + 1;
                    result.matches[count].pattern_id = pattern_id;
                    result.matches[count].pattern_length = pattern_len;
                    count++;
                }
            }
            temp = temp->failure;
        }
    }
    
    clock_t end = clock();
    
    result.match_count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    result.memory_used = capacity * sizeof(PatternMatch);
    
    return result;
}
