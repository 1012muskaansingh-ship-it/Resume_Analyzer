#include "analyzer.h"

char* extract_section(const char* text, const char* section_name) {
    if (!text || !section_name) return NULL;
    
    char* result = (char*)malloc(MAX_LINE_LENGTH);
    if (!result) return NULL;
    
    strcpy(result, "");
    
    char lower_text[strlen(text) + 1];
    strcpy(lower_text, text);
    to_lowercase(lower_text);
    
    char lower_section[MAX_STRING];
    strcpy(lower_section, section_name);
    to_lowercase(lower_section);
    
    char* start = strstr(lower_text, lower_section);
    if (!start) {
        return result;
    }
    
    /* Find the actual position in original text */
    int offset = start - lower_text;
    const char* content_start = text + offset;
    
    /* Skip to the end of the section header line */
    while (*content_start && *content_start != '\n') {
        content_start++;
    }
    if (*content_start == '\n') content_start++;
    
    /* Find the end (next section or end of text) */
    const char* content_end = content_start;
    const char* section_markers[] = {"experience", "education", "skills", "projects", 
                                     "summary", "contact", "objective", "certifications", NULL};
    
    while (*content_end) {
        char lower_check[MAX_STRING];
        strncpy(lower_check, content_end, MAX_STRING - 1);
        to_lowercase(lower_check);
        
        int found = 0;
        for (int i = 0; section_markers[i] != NULL; i++) {
            if (strstr(lower_check, section_markers[i]) == lower_check && 
                content_end > content_start) {
                found = 1;
                break;
            }
        }
        
        if (found) break;
        content_end++;
    }
    
    int length = content_end - content_start;
    if (length > MAX_LINE_LENGTH - 1) length = MAX_LINE_LENGTH - 1;
    
    strncpy(result, content_start, length);
    result[length] = '\0';
    trim_whitespace(result);
    
    return result;
}

ResumeAnalysis* analyze_resume_sections(const char* resume_text) {
    if (!resume_text) return NULL;
    
    ResumeAnalysis* analysis = (ResumeAnalysis*)malloc(sizeof(ResumeAnalysis));
    if (!analysis) return NULL;
    
    /* Initialize all sections */
    memset(analysis, 0, sizeof(ResumeAnalysis));
    
    /* Extract each section */
    char* skills = extract_section(resume_text, "skills");
    if (skills) {
        strncpy(analysis->skills.content, skills, MAX_LINE_LENGTH - 1);
        free(skills);
    }
    
    char* experience = extract_section(resume_text, "experience");
    if (experience) {
        strncpy(analysis->experience.content, experience, MAX_LINE_LENGTH - 1);
        free(experience);
    }
    
    char* education = extract_section(resume_text, "education");
    if (education) {
        strncpy(analysis->education.content, education, MAX_LINE_LENGTH - 1);
        free(education);
    }
    
    char* projects = extract_section(resume_text, "projects");
    if (projects) {
        strncpy(analysis->projects.content, projects, MAX_LINE_LENGTH - 1);
        free(projects);
    }
    
    char* summary = extract_section(resume_text, "summary");
    if (summary) {
        strncpy(analysis->summary.content, summary, MAX_LINE_LENGTH - 1);
        free(summary);
    }
    
    char* contact = extract_section(resume_text, "contact");
    if (contact) {
        strncpy(analysis->contact.content, contact, MAX_LINE_LENGTH - 1);
        free(contact);
    }
    
    return analysis;
}

void free_resume_analysis(ResumeAnalysis* analysis) {
    if (analysis) free(analysis);
}

void print_resume_analysis(ResumeAnalysis* analysis) {
    if (!analysis) return;
    
    print_header("RESUME SECTIONS DETECTED");
    
    if (strlen(analysis->contact.content) > 0) {
        print_success("Contact Information Found");
    } else {
        print_warning("No Contact Information Found");
    }
    
    if (strlen(analysis->summary.content) > 0) {
        print_success("Professional Summary Found");
    } else {
        print_warning("No Professional Summary Found");
    }
    
    if (strlen(analysis->experience.content) > 0) {
        print_success("Experience Section Found");
    } else {
        print_warning("No Experience Section Found");
    }
    
    if (strlen(analysis->education.content) > 0) {
        print_success("Education Section Found");
    } else {
        print_warning("No Education Section Found");
    }
    
    if (strlen(analysis->skills.content) > 0) {
        print_success("Skills Section Found");
    } else {
        print_warning("No Skills Section Found");
    }
    
    if (strlen(analysis->projects.content) > 0) {
        print_success("Projects Section Found");
    } else {
        print_warning("No Projects Section Found");
    }
}
