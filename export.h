#ifndef EXPORT_H
#define EXPORT_H

#include "utils.h"
#include "ats_score.h"
#include "readability.h"
#include "analyzer.h"

typedef struct {
    int ats_score;
    int readability_score;
    int keyword_matches;
    int total_keywords;
    int sections_found;
} JSONMetrics;

/* JSON Export */
int export_metrics_json(const char* filename, ATSResult* ats, 
                        ReadabilityScore* readability, int sections_found);

/* HTML Export */
int export_html_report(const char* filename, const char* resume_name,
                       ATSResult* ats, ReadabilityScore* readability);

/* Cover Letter Generation */
char* generate_cover_letter(const char* resume_text, const char* job_description,
                            const char* candidate_name, const char* company_name);

/* Export Functions */
int save_report(const char* filename, const char* report_content);
int save_cover_letter(const char* filename, const char* letter_content);

#endif
