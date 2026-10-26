#include "tokenizer.h"

Tokens* tokenize(const char* text) {
    if (!text) return NULL;
    
    Tokens* tokens = (Tokens*)malloc(sizeof(Tokens));
    if (!tokens) return NULL;
    
    char* copy = (char*)malloc(strlen(text) + 1);
    strcpy(copy, text);
    
    to_lowercase(copy);
    remove_special_chars(copy);
    
    int count = 0;
    tokens->tokens = split_string(copy, " \n\t", &count);
    tokens->count = 0;
    
    /* Filter out stop words and empty strings */
    for (int i = 0; i < count; i++) {
        if (strlen(tokens->tokens[i]) > 0 && !is_stop_word(tokens->tokens[i])) {
            tokens->tokens[tokens->count] = tokens->tokens[i];
            tokens->count++;
        } else {
            free(tokens->tokens[i]);
        }
    }
    
    free(copy);
    return tokens;
}

void free_tokens(Tokens* tokens) {
    if (!tokens) return;
    
    for (int i = 0; i < tokens->count; i++) {
        free(tokens->tokens[i]);
    }
    free(tokens->tokens);
    free(tokens);
}

TokenFrequency* analyze_frequency(Tokens* tokens) {
    if (!tokens) return NULL;
    
    TokenFrequency* freq = (TokenFrequency*)malloc(sizeof(TokenFrequency));
    if (!freq) return NULL;
    
    freq->capacity = 1000;
    freq->word_count = 0;
    freq->words = (WordFreq*)malloc(freq->capacity * sizeof(WordFreq));
    
    if (!freq->words) {
        free(freq);
        return NULL;
    }
    
    for (int i = 0; i < tokens->count; i++) {
        int found = 0;
        
        for (int j = 0; j < freq->word_count; j++) {
            if (strcmp(freq->words[j].word, tokens->tokens[i]) == 0) {
                freq->words[j].frequency++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (freq->word_count >= freq->capacity) {
                freq->capacity *= 2;
                freq->words = (WordFreq*)realloc(freq->words, freq->capacity * sizeof(WordFreq));
            }
            
            strcpy(freq->words[freq->word_count].word, tokens->tokens[i]);
            freq->words[freq->word_count].frequency = 1;
            freq->words[freq->word_count].position = i;
            freq->word_count++;
        }
    }
    
    return freq;
}

void free_frequency(TokenFrequency* freq) {
    if (!freq) return;
    free(freq->words);
    free(freq);
}

int compare_frequency(const void* a, const void* b) {
    return ((WordFreq*)b)->frequency - ((WordFreq*)a)->frequency;
}

int compare_position(const void* a, const void* b) {
    return ((WordFreq*)a)->position - ((WordFreq*)b)->position;
}

void sort_by_frequency(TokenFrequency* freq) {
    if (!freq) return;
    qsort(freq->words, freq->word_count, sizeof(WordFreq), compare_frequency);
}

void sort_by_position(TokenFrequency* freq) {
    if (!freq) return;
    qsort(freq->words, freq->word_count, sizeof(WordFreq), compare_position);
}

void print_top_keywords(TokenFrequency* freq, int top_n) {
    if (!freq) return;
    
    sort_by_frequency(freq);
    
    printf("%s\n%-40s %s\n%s", BOLD, "KEYWORD", "FREQUENCY", RESET);
    print_separator();
    
    int limit = top_n < freq->word_count ? top_n : freq->word_count;
    for (int i = 0; i < limit; i++) {
        printf("%-40s %d\n", freq->words[i].word, freq->words[i].frequency);
    }
}
