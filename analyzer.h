#ifndef ANALYZER_H
#define ANALYZER_H

#include "utils.h"

typedef struct {
    char content[MAX_LINE_LENGTH];
    int start_line;
    int end_line;
} Section;

typedef struct {
    Section skills;
    Section experience;
    Section education;
    Section projects;
    Section summary;
    Section contact;
} ResumeAnalysis;

/* Section detection */
ResumeAnalysis* analyze_resume_sections(const char* resume_text);
void free_resume_analysis(ResumeAnalysis* analysis);

/* Section extraction */
char* extract_section(const char* text, const char* section_name);

/* Display */
void print_resume_analysis(ResumeAnalysis* analysis);

#endif
