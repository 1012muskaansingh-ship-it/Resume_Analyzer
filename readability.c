#include "readability.h"

int count_syllables_in_word(const char* word) {
    if (!word || strlen(word) == 0) return 1;
    
    int syllables = 0;
    int previous_was_vowel = 0;
    
    for (int i = 0; word[i]; i++) {
        char c = tolower(word[i]);
        int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y');
        
        if (is_vowel && !previous_was_vowel) {
            syllables++;
        }
        
        previous_was_vowel = is_vowel;
    }
    
    /* Adjust for silent 'e' */
    char last = tolower(word[strlen(word) - 1]);
    if (last == 'e') syllables--;
    
    /* Adjust for 'le' ending */
    if (strlen(word) >= 2) {
        if (word[strlen(word) - 1] == 'e' && word[strlen(word) - 2] == 'l') {
            syllables++;
        }
    }
    
    return syllables > 0 ? syllables : 1;
}

int count_total_syllables(const char* text) {
    if (!text) return 0;
    
    int total = 0;
    char word[MAX_STRING];
    int j = 0;
    
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            word[j++] = text[i];
        } else {
            if (j > 0) {
                word[j] = '\0';
                total += count_syllables_in_word(word);
                j = 0;
            }
        }
    }
    
    if (j > 0) {
        word[j] = '\0';
        total += count_syllables_in_word(word);
    }
    
    return total;
}

ReadabilityScore* calculate_readability(const char* text) {
    if (!text) return NULL;
    
    ReadabilityScore* score = (ReadabilityScore*)malloc(sizeof(ReadabilityScore));
    if (!score) return NULL;
    
    score->word_count = count_words(text);
    score->sentence_count = count_sentences(text);
    score->syllable_count = count_total_syllables(text);
    
    if (score->word_count == 0 || score->sentence_count == 0) {
        free(score);
        return NULL;
    }
    
    /* Flesch-Kincaid Grade Level */
    score->flesch_kincaid = 0.39 * score->word_count / score->sentence_count +
                            11.8 * score->syllable_count / score->word_count - 15.59;
    
    if (score->flesch_kincaid < 0) score->flesch_kincaid = 0;
    if (score->flesch_kincaid > 18) score->flesch_kincaid = 18;
    
    /* Coleman-Liau Index */
    int character_count = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) character_count++;
    }
    
    score->coleman_liau = 0.0588 * character_count - 0.296 * score->sentence_count - 15.8;
    if (score->coleman_liau < 0) score->coleman_liau = 0;
    
    /* Flesch Reading Ease */
    score->flesch_reading_ease = 206.835 - 1.015 * score->word_count / score->sentence_count -
                                 84.6 * score->syllable_count / score->word_count;
    
    if (score->flesch_reading_ease < 0) score->flesch_reading_ease = 0;
    if (score->flesch_reading_ease > 100) score->flesch_reading_ease = 100;
    
    return score;
}

void free_readability_score(ReadabilityScore* score) {
    if (score) free(score);
}

void print_readability_report(ReadabilityScore* score) {
    if (!score) return;
    
    print_header("READABILITY ANALYSIS");
    
    printf("%sText Statistics:%s\n", BOLD, RESET);
    printf("  Words        : %d\n", score->word_count);
    printf("  Sentences    : %d\n", score->sentence_count);
    printf("  Syllables    : %d\n", score->syllable_count);
    printf("  Avg words/sent: %.1f\n\n", (double)score->word_count / score->sentence_count);
    
    printf("%sReadability Indices:%s\n", BOLD, RESET);
    printf("  Flesch-Kincaid Grade  : %.1f (lower is easier)\n", score->flesch_kincaid);
    printf("  Coleman-Liau Index    : %.1f\n", score->coleman_liau);
    printf("  Flesch Reading Ease   : %.1f (higher is easier, 0-100)\n\n", score->flesch_reading_ease);
    
    printf("%sInterpretation:%s\n", BOLD, RESET);
    
    if (score->flesch_reading_ease >= 90) {
        print_success("Very easy to read - suitable for most audiences");
    } else if (score->flesch_reading_ease >= 80) {
        print_success("Easy to read - good for professional documents");
    } else if (score->flesch_reading_ease >= 60) {
        print_info("Moderate difficulty - standard business writing");
    } else if (score->flesch_reading_ease >= 30) {
        print_warning("Difficult to read - consider simplifying");
    } else {
        print_error("Very difficult to read - needs significant revision");
    }
}
