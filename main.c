#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "tokenizer.h"
#include "ats_score.h"
#include "readability.h"
#include "analyzer.h"
#include "llm_interface.h"
#include "resume_generator.h"
#include "export.h"

#define RESUME_FILE "resume.txt"
#define JOB_DESC_FILE "job_description.txt"
#define ENHANCED_RESUME_FILE "enhanced_resume.txt"
#define COVER_LETTER_FILE "cover_letter.txt"
#define REPORT_JSON_FILE "analysis_report.json"
#define REPORT_HTML_FILE "analysis_report.html"

void print_banner(void) {
    printf("\n");
    printf("============================================================\n");
    printf("     ADVANCED RESUME ANALYZER WITH AI ENHANCEMENT          \n");
    printf("          Powered by Ollama LLM Integration                \n");
    printf("============================================================\n\n");
}

void print_menu(void) {
    printf("\n%s%s=== MAIN MENU ===%s\n", BOLD, CYAN, RESET);
    printf("1. Analyze Resume\n");
    printf("2. Generate Enhanced Resume (3 Versions)\n");
    printf("3. Generate Cover Letter\n");
    printf("4. View ATS Optimization Suggestions\n");
    printf("5. View Readability Analysis\n");
    printf("6. Export Reports (JSON + HTML)\n");
    printf("7. Exit\n");
    printf("\nSelect option (1-7): ");
}

void analyze_resume(void) {
    print_header("RESUME ANALYSIS");
    
    /* Read files */
    char* resume_text = read_file(RESUME_FILE);
    char* job_desc_text = read_file(JOB_DESC_FILE);
    
    if (!resume_text) {
        print_error("Could not read resume.txt");
        return;
    }
    
    if (!job_desc_text) {
        print_error("Could not read job_description.txt");
        free(resume_text);
        return;
    }
    
    print_separator();
    
    /* ATS Analysis */
    print_info("Calculating ATS score...");
    ATSResult* ats_result = calculate_ats_score(resume_text, job_desc_text);
    KeywordMatches* keyword_matches = extract_job_keywords(job_desc_text);
    
    if (ats_result && keyword_matches) {
        match_keywords(resume_text, keyword_matches);
        printf("\n");
        print_header("ATS ANALYSIS REPORT");
        printf("Overall ATS Score: %d/100\n", ats_result->score);
        print_separator();
        printf("Component Scores:\n");
        printf("  Keyword Matches    : %d/%d (%.1f%%)\n", 
               ats_result->keyword_matches, ats_result->total_job_keywords,
               (ats_result->keyword_matches * 100.0) / (ats_result->total_job_keywords > 0 ? ats_result->total_job_keywords : 1));
        printf("  Formatting Score   : %d/100\n", ats_result->formatting_score);
        printf("  Readability Score  : %d/100\n", ats_result->readability_score);
        
        /* Show missing keywords */
        printf("\nMissing Keywords:\n");
        int missing_count = 0;
        for (int i = 0; i < keyword_matches->count && missing_count < 10; i++) {
            if (!keyword_matches->matches[i].found) {
                printf("  • %s (appears %d time(s))\n",
                       keyword_matches->matches[i].keyword,
                       keyword_matches->matches[i].frequency);
                missing_count++;
            }
        }
        if (missing_count == 0) {
            printf("  None! Great keyword coverage!\n");
        }
    } else {
        print_error("ATS analysis failed");
    }
    
    print_separator();
    
    /* Readability Analysis */
    print_info("Analyzing readability...");
    ReadabilityScore* readability = calculate_readability(resume_text);
    
    if (readability) {
        printf("\n");
        print_header("READABILITY ANALYSIS");
        printf("Flesch-Kincaid Grade: %.1f\n", readability->flesch_kincaid);
        printf("Flesch Reading Ease: %.1f/100\n", readability->flesch_reading_ease);
        printf("Word Count: %d | Sentences: %d\n", readability->word_count, readability->sentence_count);
        free_readability_score(readability);
    }
    
    print_separator();
    
    /* Section Detection */
    print_info("Detecting resume sections...");
    ResumeAnalysis* analysis = analyze_resume_sections(resume_text);
    
    if (analysis) {
        printf("\n");
        print_header("SECTIONS FOUND");
        if (strlen(analysis->skills.content) > 0) print_success("  ✓ Skills section found");
        if (strlen(analysis->experience.content) > 0) print_success("  ✓ Experience section found");
        if (strlen(analysis->education.content) > 0) print_success("  ✓ Education section found");
        if (strlen(analysis->projects.content) > 0) print_success("  ✓ Projects section found");
        if (strlen(analysis->summary.content) > 0) print_success("  ✓ Summary section found");
        free_resume_analysis(analysis);
    }
    
    /* Cleanup */
    free(resume_text);
    free(job_desc_text);
    if (ats_result) free(ats_result);
    if (keyword_matches) {
        if (keyword_matches->matches) free(keyword_matches->matches);
        free(keyword_matches);
    }
}

void generate_enhanced_resumes(void) {
    print_header("ENHANCED RESUME GENERATION");
    
    char* resume_text = read_file(RESUME_FILE);
    char* job_desc_text = read_file(JOB_DESC_FILE);
    
    if (!resume_text || !job_desc_text) {
        print_error("Could not read resume or job description files");
        if (resume_text) free(resume_text);
        if (job_desc_text) free(job_desc_text);
        return;
    }
    
    print_info("Generating 3 resume versions...\n");
    
    ResomeVersions* versions = generate_resume_versions(resume_text, job_desc_text);
    if (!versions) {
        print_error("Failed to generate resume versions");
        free(resume_text);
        free(job_desc_text);
        return;
    }
    
    /* Conservative version */
    print_header("VERSION 1: CONSERVATIVE");
    printf("%sMinimal changes, focus on correctness and formatting%s\n", YELLOW, RESET);
    printf("First 300 chars:\n%s\n", versions->conservative.content);
    
    /* Balanced version */
    print_header("VERSION 2: BALANCED");
    printf("%sOptimal blend of keywords and professionalism%s\n", YELLOW, RESET);
    printf("First 300 chars:\n%s\n", versions->balanced.content);
    
    /* Aggressive version */
    print_header("VERSION 3: AGGRESSIVE");
    printf("%sMaximum keyword integration and impact%s\n", YELLOW, RESET);
    printf("First 300 chars:\n%s\n", versions->aggressive.content);
    
    print_separator();
    
    /* Export enhanced resume (default to balanced) */
    export_resume_text(ENHANCED_RESUME_FILE, versions->balanced.content);
    print_success("Enhanced resume (balanced version) saved to enhanced_resume.txt");
    
    free(versions);
    free(resume_text);
    free(job_desc_text);
}

void show_optimization_suggestions(void) {
    print_header("ATS OPTIMIZATION SUGGESTIONS");
    
    char* resume_text = read_file(RESUME_FILE);
    char* job_desc_text = read_file(JOB_DESC_FILE);
    
    if (!resume_text || !job_desc_text) {
        print_error("Could not read files");
        if (resume_text) free(resume_text);
        if (job_desc_text) free(job_desc_text);
        return;
    }
    
    print_info("Checking Ollama LLM availability...");
    
    LLMConfig* config = create_llm_config("llama2");
    int llm_available = check_llm_availability(config);
    
    if (llm_available) {
        print_success("Ollama LLM connected - generating AI suggestions\n");
    } else {
        print_warning("Ollama not available - using rule-based suggestions\n");
    }
    
    print_separator();
    
    /* Suggest skills to add */
    print_header("SUGGESTED SKILLS TO ADD");
    LLMResponse* skill_suggestions = suggest_skills_to_add(NULL, job_desc_text);
    if (skill_suggestions) {
        print_llm_response(skill_suggestions);
        free_llm_response(skill_suggestions);
    }
    
    print_separator();
    
    /* Suggest action verbs */
    print_header("RECOMMENDED ACTION VERBS FOR BULLET POINTS");
    LLMResponse* verb_suggestions = suggest_action_verbs();
    if (verb_suggestions) {
        print_llm_response(verb_suggestions);
        free_llm_response(verb_suggestions);
    }
    
    print_separator();
    
    /* Sample bullet point improvement */
    print_header("SAMPLE BULLET POINT IMPROVEMENT");
    printf("Original: Helped with project management\n\n");
    
    LLMResponse* improved = improve_bullet_point("Helped with project management", job_desc_text);
    if (improved && improved->count > 0) {
        printf("Improved: %s\n", improved->suggestions[0]);
        free_llm_response(improved);
    }
    
    free_llm_config(config);
    free(resume_text);
    free(job_desc_text);
}

void show_readability_analysis(void) {
    print_header("DETAILED READABILITY ANALYSIS");
    
    char* resume_text = read_file(RESUME_FILE);
    if (!resume_text) {
        print_error("Could not read resume.txt");
        return;
    }
    
    ReadabilityScore* readability = calculate_readability(resume_text);
    if (readability) {
        print_readability_report(readability);
        free(readability);
    }
    
    free(resume_text);
}

void export_all_reports(void) {
    print_header("EXPORTING ALL REPORTS");
    
    char* resume_text = read_file(RESUME_FILE);
    char* job_desc_text = read_file(JOB_DESC_FILE);
    
    if (!resume_text || !job_desc_text) {
        print_error("Could not read files");
        return;
    }
    
    print_info("Generating reports...\n");
    
    ATSResult* ats = calculate_ats_score(resume_text, job_desc_text);
    ReadabilityScore* readability = calculate_readability(resume_text);
    ResumeAnalysis* analysis = analyze_resume_sections(resume_text);
    
    int sections = 0;
    if (strlen(analysis->skills.content) > 0) sections++;
    if (strlen(analysis->experience.content) > 0) sections++;
    if (strlen(analysis->education.content) > 0) sections++;
    if (strlen(analysis->projects.content) > 0) sections++;
    
    export_metrics_json(REPORT_JSON_FILE, ats, readability, sections);
    print_success("✓ JSON report exported to analysis_report.json");
    
    export_html_report(REPORT_HTML_FILE, "resume.txt", ats, readability);
    print_success("✓ HTML report exported to analysis_report.html");
    
    char* cover_letter = generate_cover_letter(resume_text, job_desc_text, "John Doe", "Acme Corp");
    save_cover_letter(COVER_LETTER_FILE, cover_letter);
    print_success("✓ Cover letter exported to cover_letter.txt");
    
    print_separator();
    printf("%sAll reports generated successfully!%s\n", GREEN, RESET);
    
    free(resume_text);
    free(job_desc_text);
    free(ats);
    free(readability);
    free(analysis);
    free(cover_letter);
}

int main(void) {
    print_banner();
    
    int choice = 0;
    
    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar(); /* consume newline */
        
        switch (choice) {
            case 1:
                analyze_resume();
                break;
            case 2:
                generate_enhanced_resumes();
                break;
            case 3: {
                print_header("COVER LETTER GENERATION");
                char* resume_text = read_file(RESUME_FILE);
                char* job_desc_text = read_file(JOB_DESC_FILE);
                if (resume_text && job_desc_text) {
                    char* letter = generate_cover_letter(resume_text, job_desc_text, "Your Name", "Company Name");
                    save_cover_letter(COVER_LETTER_FILE, letter);
                    print_success("Cover letter saved to cover_letter.txt");
                    printf("\n%s%s\n", letter, RESET);
                    free(letter);
                }
                if (resume_text) free(resume_text);
                if (job_desc_text) free(job_desc_text);
                break;
            }
            case 4:
                show_optimization_suggestions();
                break;
            case 5:
                show_readability_analysis();
                break;
            case 6:
                export_all_reports();
                break;
            case 7:
                print_header("THANK YOU FOR USING RESUME ANALYZER");
                print_success("Goodbye!");
                printf("\n");
                return 0;
            default:
                print_error("Invalid option. Please try again.");
        }
    }
    
    return 0;
}
