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
    // BFS queue implemented with a dynamically resizing array to avoid fixed-size overflow
    size_t qcap = 1024;
    ACNode **queue = (ACNode **)malloc(qcap * sizeof(ACNode *));
    size_t front = 0, rear = 0;

    // Initialize root's children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (trie->root->children[i]) {
            trie->root->children[i]->failure = trie->root;
            if (rear >= qcap) {
                qcap *= 2;
                ACNode **tmp = (ACNode **)realloc(queue, qcap * sizeof(ACNode *));
                if (tmp) queue = tmp;
            }
            queue[rear++] = trie->root->children[i];
        }
    }

    // Build failure links using BFS
    // Failure link points to the longest proper suffix of the current string 
    // that is also a prefix of some pattern in the trie.
    while (front < rear) {
        ACNode *current = queue[front++];

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i]) {
                ACNode *child = current->children[i];
                if (rear >= qcap) {
                    qcap *= 2;
                    ACNode **tmp = (ACNode **)realloc(queue, qcap * sizeof(ACNode *));
                    if (tmp) queue = tmp;
                }
                queue[rear++] = child;

                ACNode *failure = current->failure;
                // Traverse failure links until a node with transition 'i' is found
                while (failure && !failure->children[i]) {
                    failure = failure->failure;
                }

                child->failure = failure ? failure->children[i] : trie->root;
                
                // Output links are not explicitly merged here; 
                // instead, we traverse the failure chain during search to collect all matches.
            }
        }
    }
    
    free(queue);
}

static void free_ac_node(ACNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            free_ac_node(node->children[i]);
        }
    }
    if (node->output) {
        free(node->output);
    }
    free(node);
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
        
        // Follow failure links if current char doesn't match
        // This simulates the transition in the Aho-Corasick automaton
        while (current != trie.root && !current->children[c]) {
            current = current->failure;
        }
        
        if (current->children[c]) {
            current = current->children[c];
        } else {
            current = trie.root;
        }
        
        // Traverse failure chain to find all patterns ending at this position
        // This handles cases where one pattern is a suffix of another (e.g., "he" inside "she")
        for (ACNode *temp = current; temp != NULL; temp = temp->failure) {
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
            if (temp == trie.root) break; // Stop at root to avoid infinite loops if root failure is set incorrectly
        }
    }
    
    clock_t end = clock();
    
    result.match_count = count;
    result.time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    result.memory_used = capacity * sizeof(PatternMatch);
    
    // Cleanup Trie to prevent memory leaks
    free_ac_node(trie.root);
    
    return result;
}
