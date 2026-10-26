#ifndef READABILITY_H
#define READABILITY_H

#include "utils.h"

typedef struct {
    double flesch_kincaid;
    double coleman_liau;
    double flesch_reading_ease;
    int syllable_count;
    int word_count;
    int sentence_count;
} ReadabilityScore;

/* Readability analysis */
ReadabilityScore* calculate_readability(const char* text);
void free_readability_score(ReadabilityScore* score);

/* Syllable counting */
int count_syllables_in_word(const char* word);
int count_total_syllables(const char* text);

/* Display */
void print_readability_report(ReadabilityScore* score);

#endif
