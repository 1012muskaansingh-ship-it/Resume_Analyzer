#include "export.h"

int export_metrics_json(const char* filename, ATSResult* ats,
                        ReadabilityScore* readability, int sections_found) {
    if (!filename) return 0;
    
    char json_content[MAX_LINE_LENGTH * 3];
    strcpy(json_content, "");
    
    strcat(json_content, "{\n");
    strcat(json_content, "  \"resume_analysis\": {\n");
    
    if (ats) {
        snprintf(json_content + strlen(json_content), 
                sizeof(json_content) - strlen(json_content),
                "    \"ats_score\": %d,\n", ats->score);
        
        snprintf(json_content + strlen(json_content),
                sizeof(json_content) - strlen(json_content),
                "    \"keyword_matches\": %d,\n", ats->keyword_matches);
        
        snprintf(json_content + strlen(json_content),
                sizeof(json_content) - strlen(json_content),
                "    \"total_keywords\": %d,\n", ats->total_job_keywords);
        
        snprintf(json_content + strlen(json_content),
                sizeof(json_content) - strlen(json_content),
                "    \"formatting_score\": %d,\n", ats->formatting_score);
        
        snprintf(json_content + strlen(json_content),
                sizeof(json_content) - strlen(json_content),
                "    \"readability_component\": %d,\n", ats->readability_score);
    }
    
    if (readability) {
        snprintf(json_content + strlen(json_content),
                sizeof(json_content) - strlen(json_content),
                "    \"flesch_kincaid_grade\": %.2f,\n", readability->flesch_kincaid);
        
        snprintf(json_content + strlen(json_content),
                sizeof(json_content) - strlen(json_content),
                "    \"coleman_liau_index\": %.2f,\n", readability->coleman_liau);
        
        snprintf(json_content + strlen(json_content),
                sizeof(json_content) - strlen(json_content),
                "    \"flesch_reading_ease\": %.2f,\n", readability->flesch_reading_ease);
    }
    
    snprintf(json_content + strlen(json_content),
            sizeof(json_content) - strlen(json_content),
            "    \"sections_found\": %d\n", sections_found);
    
    strcat(json_content, "  }\n");
    strcat(json_content, "}\n");
    
    return write_file(filename, json_content);
}

int export_html_report(const char* filename, const char* resume_name,
                       ATSResult* ats, ReadabilityScore* readability) {
    if (!filename) return 0;
    
    char html[MAX_LINE_LENGTH * 5];
    strcpy(html, "");
    
    strcat(html, "<!DOCTYPE html>\n");
    strcat(html, "<html>\n");
    strcat(html, "<head>\n");
    strcat(html, "  <title>Resume Analysis Report</title>\n");
    strcat(html, "  <style>\n");
    strcat(html, "    body { font-family: Arial, sans-serif; margin: 20px; }\n");
    strcat(html, "    .score { font-size: 24px; font-weight: bold; color: #0066cc; }\n");
    strcat(html, "    .good { color: green; }\n");
    strcat(html, "    .warning { color: orange; }\n");
    strcat(html, "    .bad { color: red; }\n");
    strcat(html, "    .bar { width: 300px; height: 20px; background: #ddd; border-radius: 5px; overflow: hidden; }\n");
    strcat(html, "    .fill { height: 100%; background: linear-gradient(90deg, #0066cc, #00ccff); }\n");
    strcat(html, "    h1 { color: #333; border-bottom: 2px solid #0066cc; }\n");
    strcat(html, "    h2 { color: #666; margin-top: 30px; }\n");
    strcat(html, "    .metric { margin: 15px 0; padding: 10px; background: #f5f5f5; border-left: 4px solid #0066cc; }\n");
    strcat(html, "  </style>\n");
    strcat(html, "</head>\n");
    strcat(html, "<body>\n");
    
    strcat(html, "<h1>📊 Resume Analysis Report</h1>\n");
    
    if (resume_name) {
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "<p><strong>Resume:</strong> %s</p>\n", resume_name);
    }
    
    if (ats) {
        strcat(html, "<h2>ATS Score</h2>\n");
        strcat(html, "<div class='metric'>\n");
        
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "<div class='score ");
        
        if (ats->score >= 70) strcat(html, "good");
        else if (ats->score >= 50) strcat(html, "warning");
        else strcat(html, "bad");
        
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "'>%d/100</div>\n", ats->score);
        
        strcat(html, "<div class='bar'>\n");
        
        int fill_width = (ats->score * 300) / 100;
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "<div class='fill' style='width: %dpx;'></div>\n", fill_width);
        
        strcat(html, "</div>\n");
        strcat(html, "<p>Keyword Matches: " );
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "%d / %d</p>\n", ats->keyword_matches, ats->total_job_keywords);
        
        strcat(html, "</div>\n");
    }
    
    if (readability) {
        strcat(html, "<h2>Readability Metrics</h2>\n");
        strcat(html, "<div class='metric'>\n");
        
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "<p>Flesch-Kincaid Grade: %.1f</p>\n", readability->flesch_kincaid);
        
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "<p>Flesch Reading Ease: %.1f/100</p>\n", readability->flesch_reading_ease);
        
        snprintf(html + strlen(html), sizeof(html) - strlen(html),
                "<p>Word Count: %d | Sentences: %d</p>\n",
                readability->word_count, readability->sentence_count);
        
        strcat(html, "</div>\n");
    }
    
    strcat(html, "<hr>\n");
    strcat(html, "<p><small>Generated with Resume Analyzer v1.0</small></p>\n");
    strcat(html, "</body>\n");
    strcat(html, "</html>\n");
    
    return write_file(filename, html);
}

char* generate_cover_letter(const char* resume_text, const char* job_description,
                            const char* candidate_name, const char* company_name) {
    if (!resume_text || !job_description) return NULL;
    
    char* letter = (char*)malloc(MAX_LINE_LENGTH * 3);
    if (!letter) return NULL;
    
    strcpy(letter, "");
    
    if (candidate_name) {
        strcat(letter, candidate_name);
    } else {
        strcat(letter, "[Your Name]");
    }
    strcat(letter, "\n");
    strcat(letter, "[Your Address]\n");
    strcat(letter, "[City, State ZIP]\n");
    strcat(letter, "[Your Email]\n");
    strcat(letter, "[Your Phone]\n\n");
    
    strcat(letter, "[Date]\n\n");
    
    strcat(letter, "Dear Hiring Manager,\n\n");
    
    strcat(letter, "I am writing to express my strong interest in the position at ");
    if (company_name) {
        strcat(letter, company_name);
    } else {
        strcat(letter, "your esteemed organization");
    }
    strcat(letter, ". With my ");
    
    /* Extract experience level from resume */
    if (strstr(resume_text, "10+") || strstr(resume_text, "15+") || strstr(resume_text, "20+")) {
        strcat(letter, "extensive professional experience");
    } else if (strstr(resume_text, "5+") || strstr(resume_text, "7+")) {
        strcat(letter, "solid professional background");
    } else {
        strcat(letter, "relevant experience");
    }
    
    strcat(letter, ", I am confident in my ability to make a significant contribution to your team.\n\n");
    
    strcat(letter, "Throughout my career, I have developed expertise in ");
    
    /* Extract key skills from resume */
    char* skills_copy = (char*)malloc(strlen(resume_text) + 1);
    strcpy(skills_copy, resume_text);
    to_lowercase(skills_copy);
    
    const char* skills[] = {"management", "development", "analysis", "leadership", "communication", NULL};
    for (int i = 0; skills[i] && i < 3; i++) {
        if (strstr(skills_copy, skills[i])) {
            strcat(letter, skills[i]);
            strcat(letter, ", ");
        }
    }
    
    strcat(letter, "and other critical areas relevant to this role. I am particularly interested in the opportunity to ");
    
    if (strstr(job_description, "lead") || strstr(job_description, "Lead")) {
        strcat(letter, "lead and mentor teams");
    } else if (strstr(job_description, "develop") || strstr(job_description, "Develop")) {
        strcat(letter, "develop innovative solutions");
    } else {
        strcat(letter, "contribute to organizational success");
    }
    
    strcat(letter, ".\n\n");
    strcat(letter, "I am excited about the prospect of bringing my skills, experience, and dedication to your organization. ");
    strcat(letter, "I welcome the opportunity to discuss how I can contribute to your team's success.\n\n");
    strcat(letter, "Thank you for considering my application. I look forward to hearing from you.\n\n");
    strcat(letter, "Sincerely,\n\n");
    
    if (candidate_name) {
        strcat(letter, candidate_name);
    } else {
        strcat(letter, "[Your Name]");
    }
    strcat(letter, "\n");
    
    free(skills_copy);
    return letter;
}

int save_report(const char* filename, const char* report_content) {
    if (!filename || !report_content) return 0;
    return write_file(filename, report_content);
}

int save_cover_letter(const char* filename, const char* letter_content) {
    if (!filename || !letter_content) return 0;
    return write_file(filename, letter_content);
}
