#ifndef ATS_SCORE_H
#define ATS_SCORE_H

#include "utils.h"
#include "tokenizer.h"

typedef struct {
    int score;
    int keyword_matches;
    int total_job_keywords;
    int formatting_score;
    int readability_score;
} ATSResult;

typedef struct {
    char keyword[MAX_STRING];
    int found;
    int frequency;
} KeywordMatch;

typedef struct {
    KeywordMatch* matches;
    int count;
    int capacity;
} KeywordMatches;

/* ATS Scoring */
ATSResult* calculate_ats_score(const char* resume_text, const char* job_desc_text);
void free_ats_result(ATSResult* result);

/* Keyword matching */
KeywordMatches* extract_job_keywords(const char* job_desc);
int match_keywords(const char* resume_text, KeywordMatches* keywords);
void free_keyword_matches(KeywordMatches* matches);

/* Detailed analysis */
void print_ats_report(ATSResult* result, KeywordMatches* matches);

#endif
