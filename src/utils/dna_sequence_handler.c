/*
 * DNA Sequence Handler - Load and process FASTA files
 */

#define _GNU_SOURCE
#include "pattern_matching.h"

#define INITIAL_BUFFER_SIZE 1024

DNASequence* load_fasta_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
    
    DNASequence *seq = (DNASequence *)calloc(1, sizeof(DNASequence));
    if (!seq) {
        fclose(file);
        return NULL;
    }
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    size_t seq_capacity = INITIAL_BUFFER_SIZE;
    seq->sequence = (char *)malloc(seq_capacity);
    if (!seq->sequence) {
        free(seq);
        fclose(file);
        return NULL;
    }
    seq->length = 0;
    
    int first_line = 1;
    
    while ((read = getline(&line, &len, file)) != -1) {
        // Remove newline
        if (read > 0 && line[read-1] == '\n') {
            line[read-1] = '\0';
            read--;
        }
        
        if (read == 0) continue; // Skip empty lines

        if (line[0] == '>') {
            // Header line
            if (first_line) {
                seq->header = strdup(line + 1); // Skip '>'
                first_line = 0;
            }
        } else {
            // Sequence line
            // Ensure capacity
            while (seq->length + read + 1 >= seq_capacity) { // +1 for null terminator safety
                seq_capacity *= 2;
                char *temp = (char *)realloc(seq->sequence, seq_capacity);
                if (!temp) {
                    free(seq->sequence);
                    if (seq->header) free(seq->header);
                    free(seq);
                    free(line);
                    fclose(file);
                    return NULL;
                }
                seq->sequence = temp;
            }
            
            // Copy sequence data and convert to uppercase
            for (int i = 0; i < read; i++) {
                char c = line[i];
                if (c >= 'a' && c <= 'z') {
                    c = c - 'a' + 'A';
                }
                // Filter out non-DNA characters if any (optional, but good for safety)
                if (strchr("ACGTN", c)) {
                     seq->sequence[seq->length++] = c;
                }
            }
        }
    }
    
    seq->sequence[seq->length] = '\0';
    
    free(line);
    fclose(file);
    
    return seq;
}

void free_dna_sequence(DNASequence *seq) {
    if (!seq) return;
    
    if (seq->header) {
        free(seq->header);
        seq->header = NULL;
    }
    if (seq->sequence) {
        free(seq->sequence);
        seq->sequence = NULL;
    }
    free(seq);
}

char* generate_random_dna(int length) {
    char *sequence = (char *)malloc((length + 1) * sizeof(char));
    if (!sequence) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    const char bases[] = "ACGT";
    
    for (int i = 0; i < length; i++) {
        sequence[i] = bases[rand() % 4];
    }
    
    sequence[length] = '\0';
    return sequence;
}