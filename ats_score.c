#include "ats_score.h"

KeywordMatches* extract_job_keywords(const char* job_desc) {
    if (!job_desc) return NULL;
    
    KeywordMatches* matches = (KeywordMatches*)malloc(sizeof(KeywordMatches));
    if (!matches) return NULL;
    
    matches->capacity = 500;
    matches->count = 0;
    matches->matches = (KeywordMatch*)malloc(matches->capacity * sizeof(KeywordMatch));
    
    Tokens* tokens = tokenize(job_desc);
    if (!tokens) {
        free(matches->matches);
        free(matches);
        return NULL;
    }
    
    for (int i = 0; i < tokens->count; i++) {
        int found = 0;
        
        for (int j = 0; j < matches->count; j++) {
            if (strcmp(matches->matches[j].keyword, tokens->tokens[i]) == 0) {
                matches->matches[j].frequency++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (matches->count >= matches->capacity) {
                matches->capacity *= 2;
                matches->matches = (KeywordMatch*)realloc(matches->matches, 
                                                          matches->capacity * sizeof(KeywordMatch));
            }
            
            strcpy(matches->matches[matches->count].keyword, tokens->tokens[i]);
            matches->matches[matches->count].found = 0;
            matches->matches[matches->count].frequency = 1;
            matches->count++;
        }
    }
    
    free_tokens(tokens);
    return matches;
}

int match_keywords(const char* resume_text, KeywordMatches* keywords) {
    if (!resume_text || !keywords) return 0;
    
    /* Create a lowercase version of resume text for matching */
    size_t resume_len = strlen(resume_text);
    char* lower_resume = (char*)malloc(resume_len + 1);
    if (!lower_resume) return 0;
    
    strcpy(lower_resume, resume_text);
    to_lowercase(lower_resume);
    
    int matches_found = 0;
    for (int i = 0; i < keywords->count; i++) {
        if (strstr(lower_resume, keywords->matches[i].keyword) != NULL) {
            keywords->matches[i].found = 1;
            matches_found++;
        }
    }
    
    free(lower_resume);
    return matches_found;
}

ATSResult* calculate_ats_score(const char* resume_text, const char* job_desc_text) {
    if (!resume_text || !job_desc_text) return NULL;
    
    ATSResult* result = (ATSResult*)malloc(sizeof(ATSResult));
    if (!result) return NULL;
    
    /* Initialize to zero */
    memset(result, 0, sizeof(ATSResult));
    
    KeywordMatches* job_keywords = extract_job_keywords(job_desc_text);
    if (!job_keywords) {
        free(result);
        return NULL;
    }
    
    result->total_job_keywords = job_keywords->count;
    result->keyword_matches = match_keywords(resume_text, job_keywords);
    
    /* Calculate keyword score (40% of total) */
    int keyword_score = 0;
    if (result->total_job_keywords > 0) {
        keyword_score = (result->keyword_matches * 100) / result->total_job_keywords;
    }
    if (keyword_score > 100) keyword_score = 100;
    
    /* Formatting score: check for common sections (30% of total) */
    int formatting_score = 0;
    const char* sections[] = {"experience", "education", "skills", "projects", NULL};
    
    char lower_resume[5000];
    strncpy(lower_resume, resume_text, sizeof(lower_resume) - 1);
    lower_resume[sizeof(lower_resume) - 1] = '\0';
    to_lowercase(lower_resume);
    
    int sections_found = 0;
    for (int i = 0; sections[i] != NULL; i++) {
        if (strstr(lower_resume, sections[i]) != NULL) {
            sections_found++;
        }
    }
    formatting_score = (sections_found * 100) / 4;
    if (formatting_score > 100) formatting_score = 100;
    
    result->formatting_score = formatting_score;
    
    /* Readability score: based on word count and structure (30% of total) */
    int word_count = count_words(resume_text);
    int readability_score = 50; /* Default */
    
    /* Longer resumes with good density are better */
    if (word_count >= 300 && word_count <= 1200) readability_score = 80;
    else if (word_count >= 200) readability_score = 65;
    else if (word_count >= 100) readability_score = 50;
    else readability_score = 30;
    
    /* Check for action verbs */
    const char* action_verbs[] = {"managed", "developed", "led", "implemented", 
                                   "designed", "created", "achieved", "improved",
                                   "increased", "reduced", "built", "optimized", NULL};
    
    int verb_count = 0;
    for (int i = 0; action_verbs[i] != NULL; i++) {
        if (strstr(lower_resume, action_verbs[i]) != NULL) {
            verb_count++;
        }
    }
    
    if (verb_count > 5) readability_score += 20;
    else if (verb_count > 2) readability_score += 10;
    
    if (readability_score > 100) readability_score = 100;
    
    result->readability_score = readability_score;
    
    /* Calculate final ATS score */
    result->score = (keyword_score * 40 + formatting_score * 30 + readability_score * 30) / 100;
    if (result->score > 100) result->score = 100;
    
    /* Clean up */
    if (job_keywords->matches) free(job_keywords->matches);
    free(job_keywords);
    
    return result;
}

void free_keyword_matches(KeywordMatches* matches) {
    if (!matches) return;
    if (matches->matches) free(matches->matches);
    free(matches);
}