#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "utils.h"

typedef struct {
    WordFreq* words;
    int word_count;
    int capacity;
} TokenFrequency;

typedef struct {
    char** tokens;
    int count;
} Tokens;

/* Tokenization */
Tokens* tokenize(const char* text);
void free_tokens(Tokens* tokens);

/* Frequency analysis */
TokenFrequency* analyze_frequency(Tokens* tokens);
void free_frequency(TokenFrequency* freq);

/* Sorting */
void sort_by_frequency(TokenFrequency* freq);
void sort_by_position(TokenFrequency* freq);

/* Display */
void print_top_keywords(TokenFrequency* freq, int top_n);

#endif
