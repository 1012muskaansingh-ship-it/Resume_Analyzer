#include "llm_interface.h"
#include <stdlib.h>

LLMConfig* create_llm_config(const char* model) {
    LLMConfig* config = (LLMConfig*)malloc(sizeof(LLMConfig));
    if (!config) return NULL;
    
    strncpy(config->model, model ? model : "llama2", MAX_STRING - 1);
    config->timeout = 30;
    strcpy(config->endpoint, "http://localhost:11434");
    config->available = 0;
    
    return config;
}

void free_llm_config(LLMConfig* config) {
    if (config) free(config);
}

int check_ollama_availability(void) {
    /* Try to ping the Ollama service */
    int result = system("curl -s http://localhost:11434/api/tags > /dev/null 2>&1");
    return result == 0 ? 1 : 0;
}

int check_llm_availability(LLMConfig* config) {
    if (!config) return 0;
    config->available = check_ollama_availability();
    return config->available;
}

char* call_ollama(const char* prompt, const char* model) {
    if (!prompt || !model) return NULL;
    
    /* Create a temporary file for the prompt */
    char cmd[MAX_LINE_LENGTH * 3];
    char temp_file[MAX_STRING];
    strcpy(temp_file, "temp_prompt.txt");
    
    /* Escape quotes in prompt for shell */
    char escaped_prompt[MAX_LINE_LENGTH];
    int j = 0;
    for (int i = 0; prompt[i] && j < MAX_LINE_LENGTH - 1; i++) {
        if (prompt[i] == '"') {
            escaped_prompt[j++] = '\\';
        }
        escaped_prompt[j++] = prompt[i];
    }
    escaped_prompt[j] = '\0';
    
    /* Build the ollama command */
    snprintf(cmd, sizeof(cmd), 
             "echo \"%s\" | ollama run %s --nowordwrap 2>/dev/null", 
             escaped_prompt, model);
    
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return NULL;
    
    char* result = (char*)malloc(MAX_LINE_LENGTH);
    if (!result) {
        pclose(pipe);
        return NULL;
    }
    
    strcpy(result, "");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), pipe) != NULL) {
        if (strlen(result) + strlen(line) < MAX_LINE_LENGTH - 1) {
            strcat(result, line);
        }
    }
    
    pclose(pipe);
    trim_whitespace(result);
    
    return result;
}

LLMResponse* improve_bullet_point(const char* original, const char* job_keywords) {
    if (!original) return NULL;
    
    LLMResponse* response = (LLMResponse*)malloc(sizeof(LLMResponse));
    if (!response) return NULL;
    
    response->capacity = 10;
    response->count = 0;
    response->suggestions = (char**)malloc(response->capacity * sizeof(char*));
    
    if (!check_ollama_availability()) {
        /* Fallback: provide rule-based improvement */
        response->suggestions[response->count] = (char*)malloc(MAX_LINE_LENGTH);
        snprintf(response->suggestions[response->count], MAX_LINE_LENGTH,
                 "[Offline] Suggested: Use action verb at start - 'Improved %s' or 'Enhanced %s'",
                 original, original);
        response->count++;
        return response;
    }
    
    char prompt[MAX_LINE_LENGTH];
    snprintf(prompt, sizeof(prompt),
             "Rewrite this resume bullet point to be more impactful and include relevant keywords. "
             "Original: %s Keywords: %s. Provide only the improved bullet point, no explanation.",
             original, job_keywords);
    
    char* result = call_ollama(prompt, "llama2");
    if (result && strlen(result) > 0) {
        response->suggestions[response->count] = result;
        response->count++;
    }
    
    return response;
}

LLMResponse* suggest_skills_to_add(const char* current_skills, const char* job_keywords) {
    if (!job_keywords) return NULL;
    
    LLMResponse* response = (LLMResponse*)malloc(sizeof(LLMResponse));
    if (!response) return NULL;
    
    response->capacity = 10;
    response->count = 0;
    response->suggestions = (char**)malloc(response->capacity * sizeof(char*));
    
    if (!check_ollama_availability()) {
        /* Fallback: provide common skills */
        const char* common_skills[] = {
            "Project Management",
            "Agile/Scrum",
            "Communication",
            "Problem Solving",
            "Team Leadership",
            NULL
        };
        
        for (int i = 0; common_skills[i] != NULL && response->count < 5; i++) {
            response->suggestions[response->count] = (char*)malloc(MAX_STRING);
            strcpy(response->suggestions[response->count], common_skills[i]);
            response->count++;
        }
        return response;
    }
    
    char prompt[MAX_LINE_LENGTH];
    snprintf(prompt, sizeof(prompt),
             "Based on these job keywords, suggest 5 important skills to add to a resume. "
             "Current skills: %s Job keywords: %s. Provide only a comma-separated list.",
             current_skills ? current_skills : "None", job_keywords);
    
    char* result = call_ollama(prompt, "llama2");
    if (result && strlen(result) > 0) {
        char* copy = (char*)malloc(strlen(result) + 1);
        strcpy(copy, result);
        
        char* token = strtok(copy, ",");
        while (token && response->count < 10) {
            response->suggestions[response->count] = (char*)malloc(MAX_STRING);
            strcpy(response->suggestions[response->count], trim_whitespace(token));
            response->count++;
            token = strtok(NULL, ",");
        }
        free(copy);
        free(result);
    }
    
    return response;
}

LLMResponse* suggest_action_verbs(void) {
    LLMResponse* response = (LLMResponse*)malloc(sizeof(LLMResponse));
    if (!response) return NULL;
    
    response->capacity = 20;
    response->count = 0;
    response->suggestions = (char**)malloc(response->capacity * sizeof(char*));
    
    /* Built-in list of strong action verbs for resumes */
    const char* action_verbs[] = {
        "Accelerated", "Achieved", "Acquired", "Adapted", "Advanced",
        "Advised", "Advocated", "Aligned", "Analyzed", "Anchored",
        "Announced", "Approved", "Arbitrated", "Architected", "Argued",
        "Arranged", "Articulated", "Ascertained", "Assembled", "Assessed",
        "Assigned", "Assisted", "Assumed", "Assured", "Attained",
        "Attended", "Attracted", "Audited", "Authored", "Authorized",
        NULL
    };
    
    for (int i = 0; action_verbs[i] != NULL && response->count < 20; i++) {
        response->suggestions[response->count] = (char*)malloc(MAX_STRING);
        strcpy(response->suggestions[response->count], action_verbs[i]);
        response->count++;
    }
    
    return response;
}

LLMResponse* improve_summary(const char* current_summary, const char* job_description) {
    if (!current_summary) return NULL;
    
    LLMResponse* response = (LLMResponse*)malloc(sizeof(LLMResponse));
    if (!response) return NULL;
    
    response->capacity = 5;
    response->count = 0;
    response->suggestions = (char**)malloc(response->capacity * sizeof(char*));
    
    if (!check_ollama_availability()) {
        response->suggestions[response->count] = (char*)malloc(MAX_LINE_LENGTH);
        strcpy(response->suggestions[response->count],
               "[Offline] Ensure summary highlights years of experience, key achievements, and relevant skills");
        response->count++;
        return response;
    }
    
    char prompt[MAX_LINE_LENGTH];
    snprintf(prompt, sizeof(prompt),
             "Improve this professional summary to match the job description requirements. "
             "Current: %s Job Description excerpt: %s. Provide an improved version.",
             current_summary, job_description ? job_description : "");
    
    char* result = call_ollama(prompt, "llama2");
    if (result && strlen(result) > 0) {
        response->suggestions[response->count] = result;
        response->count++;
    }
    
    return response;
}

void free_llm_response(LLMResponse* response) {
    if (!response) return;
    
    for (int i = 0; i < response->count; i++) {
        free(response->suggestions[i]);
    }
    free(response->suggestions);
    free(response);
}

void print_llm_response(LLMResponse* response) {
    if (!response) return;
    
    for (int i = 0; i < response->count; i++) {
        printf("  %d. %s\n", i + 1, response->suggestions[i]);
    }
}
