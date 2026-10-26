#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING 1024
#define MAX_WORDS 5000
#define MAX_LINE_LENGTH 2048

typedef struct {
    char word[MAX_STRING];
    int frequency;
    int position;
} WordFreq;

typedef struct {
    char** items;
    int count;
    int capacity;
} DynamicArray;

/* String manipulation */
char* trim_whitespace(char* str);
void to_lowercase(char* str);
void remove_special_chars(char* str);
int is_stop_word(const char* word);
char* read_file(const char* filename);
int write_file(const char* filename, const char* content);
void append_to_file(const char* filename, const char* content);

/* Array operations */
DynamicArray* create_array(void);
void add_to_array(DynamicArray* arr, const char* item);
void free_array(DynamicArray* arr);
void print_array(DynamicArray* arr);

/* Text processing */
char** split_string(const char* str, const char* delimiter, int* count);
void free_split(char** arr, int count);
int count_words(const char* text);
int count_lines(const char* text);
int count_sentences(const char* text);

/* Color output for terminal - Windows compatible */
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define BOLD    "\x1b[1m"

/* Windows console initialization */
void init_console(void);
void print_header(const char* text);
void print_success(const char* text);
void print_error(const char* text);
void print_warning(const char* text);
void print_info(const char* text);
void print_separator(void);
void print_score_bar(const char* label, int score, int max_score);

#endif