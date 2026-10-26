#ifndef RESUME_GENERATOR_H
#define RESUME_GENERATOR_H

#include "utils.h"

typedef struct {
    char content[MAX_LINE_LENGTH * 5];
    char style[50];
    int aggressiveness;
} GeneratedResume;

typedef struct {
    GeneratedResume conservative;
    GeneratedResume balanced;
    GeneratedResume aggressive;
} ResomeVersions;

/* Generate enhanced resumes */
GeneratedResume* generate_enhanced_resume(const char* original_resume, 
                                          const char* job_description,
                                          const char* style);

/* Generate multiple versions */
ResomeVersions* generate_resume_versions(const char* original_resume,
                                         const char* job_description);

/* Resume optimization */
char* optimize_formatting(const char* resume);
char* add_missing_keywords(const char* resume, const char* keywords);
char* improve_bullet_points(const char* resume);

/* Export */
int export_resume_text(const char* filename, const char* content);
int export_resume_markdown(const char* filename, const char* content);

/* Memory management */
void free_generated_resume(GeneratedResume* resume);
void free_resume_versions(ResomeVersions* versions);

#endif
