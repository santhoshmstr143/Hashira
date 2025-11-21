/*
 * Utility Functions for Pattern Matching
 */

#include "pattern_matching.h"

void free_match_result(MatchResult *result) {
    if (result && result->positions) {
        free(result->positions);
        result->positions = NULL;
        result->count = 0;
    }
}

void free_approximate_match_result(ApproximateMatchResult *result) {
    if (result && result->matches) {
        free(result->matches);
        result->matches = NULL;
        result->count = 0;
    }
}

void print_match_result(const char *algo_name, const MatchResult *result) {
    printf("\n┌─────────────────────────────────────────────────────────┐\n");
    printf("│  🧬 %s%-*s│\n", algo_name, (int)(52 - strlen(algo_name)), "");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    if (result->count == 0) {
        printf("  ❌ No matches found\n");
    } else if (result->count == 1) {
        printf("  ✅ Found %d match\n", result->count);
    } else {
        printf("  ✅ Found %d matches\n", result->count);
    }
    
    printf("  ⏱️  Time taken: %.3f ms\n", result->time_taken);
    printf("  💾 Memory used: %zu bytes\n", result->memory_used);
    
    if (result->count > 0 && result->count <= 10) {
        printf("  📍 Match positions: ");
        for (int i = 0; i < result->count; i++) {
            printf("%d", result->positions[i]);
            if (i < result->count - 1) printf(", ");
        }
        printf("\n");
    } else if (result->count > 10) {
        printf("  📍 First 10 positions: ");
        for (int i = 0; i < 10; i++) {
            printf("%d", result->positions[i]);
            if (i < 9) printf(", ");
        }
        printf(" ... (+%d more)\n", result->count - 10);
    }
    printf("\n");
}

void print_approximate_match_result(const char *algo_name, const ApproximateMatchResult *result) {
    printf("\n┌─────────────────────────────────────────────────────────┐\n");
    printf("│  🔍 %s%-*s│\n", algo_name, (int)(52 - strlen(algo_name)), "");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    if (result->count == 0) {
        printf("  ❌ No approximate matches found\n");
    } else if (result->count == 1) {
        printf("  ✅ Found %d approximate match\n", result->count);
    } else {
        printf("  ✅ Found %d approximate matches\n", result->count);
    }
    
    printf("  ⏱️  Time taken: %.3f ms\n", result->time_taken);
    printf("  💾 Memory used: %zu bytes\n", result->memory_used);
    
    if (result->count > 0 && result->count <= 10) {
        printf("\n  📊 Match details:\n");
        for (int i = 0; i < result->count; i++) {
            printf("     Position %d → Edit distance: %d\n", 
                   result->matches[i].position, 
                   result->matches[i].distance);
        }
    } else if (result->count > 10) {
        printf("\n  📊 First 10 match details:\n");
        for (int i = 0; i < 10; i++) {
            printf("     Position %d → Edit distance: %d\n", 
                   result->matches[i].position, 
                   result->matches[i].distance);
        }
        printf("     ... and %d more matches\n", result->count - 10);
    }
    printf("\n");
}

void free_multi_pattern_result(MultiPatternResult *result) {
    if (result && result->matches) {
        free(result->matches);
        result->matches = NULL;
        result->match_count = 0;
    }
}

void print_multi_pattern_result(const char *algo_name, const MultiPatternResult *result, const char **patterns) {
    printf("\n┌─────────────────────────────────────────────────────────┐\n");
    printf("│  🎯 %s%-*s│\n", algo_name, (int)(52 - strlen(algo_name)), "");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    if (result->match_count == 0) {
        printf("  ❌ No pattern matches found\n");
    } else if (result->match_count == 1) {
        printf("  ✅ Found %d match\n", result->match_count);
    } else {
        printf("  ✅ Found %d matches\n", result->match_count);
    }
    
    printf("  ⏱️  Time taken: %.3f ms\n", result->time_taken);
    printf("  💾 Memory used: %zu bytes\n", result->memory_used);
    
    if (result->match_count > 0) {
        printf("\n  📊 Match details:\n");
        int display_count = result->match_count > 20 ? 20 : result->match_count;
        for (int i = 0; i < display_count; i++) {
            printf("     Pattern[%d] '%s' → Position %d\n", 
                   result->matches[i].pattern_id,
                   patterns[result->matches[i].pattern_id],
                   result->matches[i].position);
        }
        if (result->match_count > 20) {
            printf("     ... and %d more matches\n", result->match_count - 20);
        }
    }
    printf("\n");
}

/* Print sequence with matched substrings highlighted using ANSI colors.
 * positions: array of starting indices of matches
 * count: number of matches
 * pattern_len: length of the matched pattern
 * context: how many bases of context to show around each match (for long sequences)
 */
void print_sequence_with_highlights(const char *sequence, const int *positions, int count, int pattern_len, int context) {
    if (!sequence || !positions || count <= 0 || pattern_len <= 0) return;

    int seq_len = strlen(sequence);

    /* For readability, if sequence is short show whole sequence; otherwise show
     * each match with surrounding context.
     */
    if (seq_len <= context * 2 + pattern_len + 10) {
        /* Print full sequence with highlights for each position */
        /* Build a marker array for each char: 0 = normal, 1 = match */
        char *mark = (char *)calloc(seq_len, 1);
        if (!mark) return;
        for (int i = 0; i < count; i++) {
            int p = positions[i];
            if (p < 0 || p + pattern_len > seq_len) continue;
            for (int j = 0; j < pattern_len; j++) mark[p + j] = 1;
        }

        for (int i = 0; i < seq_len; i++) {
            if (mark[i]) printf("\x1b[43m%c\x1b[0m", sequence[i]);
            else printf("%c", sequence[i]);
        }
        printf("\n");
        free(mark);
        return;
    }

    /* For long sequences show each match with context */
    for (int i = 0; i < count; i++) {
        int p = positions[i];
        if (p < 0 || p + pattern_len > seq_len) continue;
        int start = p - context; if (start < 0) start = 0;
        int end = p + pattern_len + context; if (end > seq_len) end = seq_len;
        printf("...%d: ", p);
        for (int j = start; j < p; j++) putchar(sequence[j]);
        /* highlight match */
        for (int j = p; j < p + pattern_len; j++) printf("\x1b[42m%c\x1b[0m", sequence[j]);
        for (int j = p + pattern_len; j < end; j++) putchar(sequence[j]);
        printf("...\n");
    }
}
