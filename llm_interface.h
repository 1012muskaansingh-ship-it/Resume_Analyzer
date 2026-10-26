#ifndef LLM_INTERFACE_H
#define LLM_INTERFACE_H

#include "utils.h"

typedef struct {
    char model[MAX_STRING];
    int timeout;
    char endpoint[MAX_STRING];
    int available;
} LLMConfig;

typedef struct {
    char** suggestions;
    int count;
    int capacity;
} LLMResponse;

/* LLM Configuration */
LLMConfig* create_llm_config(const char* model);
void free_llm_config(LLMConfig* config);

/* Check LLM availability */
int check_ollama_availability(void);
int check_llm_availability(LLMConfig* config);

/* LLM Calls */
char* call_ollama(const char* prompt, const char* model);
LLMResponse* improve_bullet_point(const char* original, const char* job_keywords);
LLMResponse* suggest_skills_to_add(const char* current_skills, const char* job_keywords);
LLMResponse* suggest_action_verbs(void);
LLMResponse* improve_summary(const char* current_summary, const char* job_description);

/* Response handling */
void free_llm_response(LLMResponse* response);
void print_llm_response(LLMResponse* response);

#endif
