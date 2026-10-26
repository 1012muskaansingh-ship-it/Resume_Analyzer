#include "resume_generator.h"
#include "llm_interface.h"

char* optimize_formatting(const char* resume) {
    if (!resume) return NULL;
    
    char* optimized = (char*)malloc(MAX_LINE_LENGTH * 5);
    if (!optimized) return NULL;
    
    strcpy(optimized, "");
    
    /* Optimize bullet points */
    char* copy = (char*)malloc(strlen(resume) + 1);
    strcpy(copy, resume);
    
    int count = 0;
    char** lines = split_string(copy, "\n", &count);
    
    for (int i = 0; i < count; i++) {
        char* line = lines[i];
        
        /* Add bullet if not present */
        if (strlen(line) > 0 && line[0] != '-' && line[0] != '*' && strstr(line, "•") == NULL) {
            if (line[0] != ' ' && i > 0) {
                strcat(optimized, "  ");
            }
        }
        
        strcat(optimized, line);
        strcat(optimized, "\n");
    }
    
    free_split(lines, count);
    free(copy);
    
    return optimized;
}

char* add_missing_keywords(const char* resume, const char* keywords) {
    if (!resume) return NULL;
    
    char* enhanced = (char*)malloc(MAX_LINE_LENGTH * 5);
    if (!enhanced) return NULL;
    
    strcpy(enhanced, resume);
    
    /* Find skills section and add keywords */
    char lower_resume[strlen(resume) + 1];
    strcpy(lower_resume, resume);
    to_lowercase(lower_resume);
    
    char* skills_pos = strstr(lower_resume, "skills");
    if (skills_pos) {
        /* Find position in original string */
        int offset = skills_pos - lower_resume;
        char* insert_pos = enhanced + offset;
        
        /* Skip to end of skills line */
        while (*insert_pos && *insert_pos != '\n') insert_pos++;
        
        if (*insert_pos == '\n') insert_pos++;
        
        /* Add new keywords */
        char temp[MAX_LINE_LENGTH * 5];
        strcpy(temp, "");
        strcat(temp, "  • ");
        strcat(temp, keywords);
        strcat(temp, "\n");
        strcat(temp, insert_pos);
        
        /* Check if we have space */
        if (strlen(enhanced) + strlen(temp) < MAX_LINE_LENGTH * 5 - 1) {
            *insert_pos = '\0';
            strcat(enhanced, temp);
        }
    }
    
    return enhanced;
}

char* improve_bullet_points(const char* resume) {
    if (!resume) return NULL;
    
    char* improved = (char*)malloc(MAX_LINE_LENGTH * 5);
    if (!improved) return NULL;
    
    strcpy(improved, resume);
    
    /* Replace weak verbs with strong action verbs */
    const char* weak_verbs[][2] = {
        {"helped", "facilitated"},
        {"did", "executed"},
        {"made", "created"},
        {"worked", "collaborated"},
        {"used", "leveraged"},
        {"was", "maintained"},
        {"is", "demonstrates"},
        {NULL, NULL}
    };
    
    for (int i = 0; weak_verbs[i][0] != NULL; i++) {
        char search[MAX_STRING];
        strcpy(search, weak_verbs[i][0]);
        
        char* pos = improved;
        while ((pos = strstr(pos, search)) != NULL) {
            /* Simple replacement */
            size_t len = strlen(weak_verbs[i][0]);
            if (strlen(weak_verbs[i][1]) <= len) {
                strncpy(pos, weak_verbs[i][1], len);
                if (strlen(weak_verbs[i][1]) < len) {
                    /* Shift remaining text */
                    memmove(pos + strlen(weak_verbs[i][1]), pos + len, 
                           strlen(pos + len) + 1);
                }
            }
            pos += strlen(weak_verbs[i][1]);
        }
    }
    
    return improved;
}

GeneratedResume* generate_enhanced_resume(const char* original_resume,
                                         const char* job_description,
                                         const char* style) {
    if (!original_resume || !style) return NULL;
    
    GeneratedResume* resume = (GeneratedResume*)malloc(sizeof(GeneratedResume));
    if (!resume) return NULL;
    
    strcpy(resume->style, style);
    strcpy(resume->content, "");
    
    if (strcmp(style, "conservative") == 0) {
        resume->aggressiveness = 1;
        /* Minimal changes, focus on correctness */
        char* optimized = optimize_formatting(original_resume);
        strncpy(resume->content, optimized, MAX_LINE_LENGTH * 5 - 1);
        free(optimized);
        
    } else if (strcmp(style, "balanced") == 0) {
        resume->aggressiveness = 2;
        /* Moderate changes, add keywords */
        char* optimized = optimize_formatting(original_resume);
        
        /* Extract job keywords */
        char keywords[MAX_LINE_LENGTH];
        strcpy(keywords, "");
        if (job_description) {
            int count = 0;
            char** parts = split_string(job_description, " ", &count);
            for (int i = 0; i < count && i < 5; i++) {
                if (!is_stop_word(parts[i]) && strlen(keywords) + strlen(parts[i]) < MAX_LINE_LENGTH - 10) {
                    strcat(keywords, parts[i]);
                    strcat(keywords, ", ");
                }
            }
            free_split(parts, count);
        }
        
        char* enhanced = add_missing_keywords(optimized, keywords);
        strncpy(resume->content, enhanced, MAX_LINE_LENGTH * 5 - 1);
        
        free(optimized);
        free(enhanced);
        
    } else if (strcmp(style, "aggressive") == 0) {
        resume->aggressiveness = 3;
        /* Aggressive changes, improve language, add keywords */
        char* optimized = optimize_formatting(original_resume);
        char* improved = improve_bullet_points(optimized);
        
        strncpy(resume->content, improved, MAX_LINE_LENGTH * 5 - 1);
        
        free(optimized);
        free(improved);
    }
    
    resume->content[MAX_LINE_LENGTH * 5 - 1] = '\0';
    return resume;
}

ResomeVersions* generate_resume_versions(const char* original_resume,
                                        const char* job_description) {
    if (!original_resume) return NULL;
    
    ResomeVersions* versions = (ResomeVersions*)malloc(sizeof(ResomeVersions));
    if (!versions) return NULL;
    
    versions->conservative = *generate_enhanced_resume(original_resume, job_description, "conservative");
    versions->balanced = *generate_enhanced_resume(original_resume, job_description, "balanced");
    versions->aggressive = *generate_enhanced_resume(original_resume, job_description, "aggressive");
    
    return versions;
}

int export_resume_text(const char* filename, const char* content) {
    if (!filename || !content) return 0;
    return write_file(filename, content);
}

int export_resume_markdown(const char* filename, const char* content) {
    if (!filename || !content) return 0;
    
    char* markdown = (char*)malloc(MAX_LINE_LENGTH * 5);
    if (!markdown) return 0;
    
    strcpy(markdown, "# Resume\n\n");
    strcat(markdown, content);
    strcat(markdown, "\n\n---\n*Generated with Resume Analyzer*\n");
    
    int result = write_file(filename, markdown);
    free(markdown);
    
    return result;
}

void free_generated_resume(GeneratedResume* resume) {
    if (resume) free(resume);
}

void free_resume_versions(ResomeVersions* versions) {
    if (versions) free(versions);
}
