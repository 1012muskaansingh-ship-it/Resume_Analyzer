#include "utils.h"

/* Windows console initialization - call this at program start */
void init_console(void) {
    #ifdef _WIN32
        /* Enable ANSI escape codes on Windows 10+ */
        system("chcp 65001 > nul 2>&1");
        printf("\x1b[?25h");  /* Show cursor */
    #endif
}

/* Static array of common stop words */
static const char* stop_words[] = {
    "the", "a", "an", "and", "or", "but", "in", "on", "at", "to", "for",
    "of", "with", "by", "from", "is", "are", "was", "were", "be", "been",
    "have", "has", "had", "do", "does", "did", "will", "would", "could",
    "should", "may", "might", "can", "must", "as", "if", "than", "that",
    "this", "which", "who", "when", "where", "why", "how", "not", "no",
    "yes", "all", "each", "every", "both", "either", "neither", "some",
    "any", "many", "much", "more", "most", "other", "same", "such", "it",
    "its", "them", "their", "they", "he", "she", "his", "her", "i", "we",
    "you", "our", "me", "him", "us", "what", "about", "into", "through",
    "during", "before", "after", "above", "below", "up", "down", "out",
    "off", "over", "under", "again", "further", "only", "own", "so",
    "too", "very", "just", "now", "here", "there", "there", "being",
    "having", "doing", "is", "s", "t", "can", "just", "don", "now",
    NULL
};

char* trim_whitespace(char* str) {
    if (!str) return str;
    
    while (*str && isspace(*str)) str++;
    
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    
    *(end + 1) = '\0';
    return str;
}

void to_lowercase(char* str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void remove_special_chars(char* str) {
    if (!str) return;
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (isalnum(str[i]) || isspace(str[i]) || str[i] == '-') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

int is_stop_word(const char* word) {
    if (!word || strlen(word) == 0) return 1;
    
    char lower_word[MAX_STRING];
    strcpy(lower_word, word);
    to_lowercase(lower_word);
    
    for (int i = 0; stop_words[i] != NULL; i++) {
        if (strcmp(lower_word, stop_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = (char*)malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);
    
    return content;
}

int write_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return 0;
    }
    
    fputs(content, file);
    fclose(file);
    return 1;
}

void append_to_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Error opening file");
        return;
    }
    
    fputs(content, file);
    fclose(file);
}

DynamicArray* create_array(void) {
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!arr) return NULL;
    
    arr->capacity = 100;
    arr->count = 0;
    arr->items = (char**)malloc(arr->capacity * sizeof(char*));
    
    if (!arr->items) {
        free(arr);
        return NULL;
    }
    
    return arr;
}

void add_to_array(DynamicArray* arr, const char* item) {
    if (!arr || !item) return;
    
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->items = (char**)realloc(arr->items, arr->capacity * sizeof(char*));
    }
    
    arr->items[arr->count] = (char*)malloc(strlen(item) + 1);
    strcpy(arr->items[arr->count], item);
    arr->count++;
}

void free_array(DynamicArray* arr) {
    if (!arr) return;
    
    for (int i = 0; i < arr->count; i++) {
        free(arr->items[i]);
    }
    free(arr->items);
    free(arr);
}

void print_array(DynamicArray* arr) {
    if (!arr) return;
    
    for (int i = 0; i < arr->count; i++) {
        printf("%s\n", arr->items[i]);
    }
}

char** split_string(const char* str, const char* delimiter, int* count) {
    if (!str || !delimiter || !count) return NULL;
    
    char* copy = (char*)malloc(strlen(str) + 1);
    strcpy(copy, str);
    
    int capacity = 100;
    char** result = (char**)malloc(capacity * sizeof(char*));
    *count = 0;
    
    char* token = strtok(copy, delimiter);
    while (token) {
        if (*count >= capacity) {
            capacity *= 2;
            result = (char**)realloc(result, capacity * sizeof(char*));
        }
        
        result[*count] = (char*)malloc(strlen(token) + 1);
        strcpy(result[*count], token);
        (*count)++;
        token = strtok(NULL, delimiter);
    }
    
    free(copy);
    return result;
}

void free_split(char** arr, int count) {
    if (!arr) return;
    for (int i = 0; i < count; i++) {
        free(arr[i]);
    }
    free(arr);
}

int count_words(const char* text) {
    if (!text) return 0;
    
    int count = 0;
    int in_word = 0;
    
    for (int i = 0; text[i]; i++) {
        if (isspace(text[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
    }
    
    return count;
}

int count_lines(const char* text) {
    if (!text) return 0;
    
    int count = 1;
    for (int i = 0; text[i]; i++) {
        if (text[i] == '\n') count++;
    }
    
    return count;
}

int count_sentences(const char* text) {
    if (!text) return 0;
    
    int count = 0;
    for (int i = 0; text[i]; i++) {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            count++;
        }
    }
    
    return count > 0 ? count : 1;
}

void print_header(const char* text) {
    printf("\n%s%s===== %s =====%s\n", BOLD, CYAN, text, RESET);
}

void print_success(const char* text) {
    printf("%s✓ %s%s\n", GREEN, text, RESET);
}

void print_error(const char* text) {
    printf("%s✗ %s%s\n", RED, text, RESET);
}

void print_warning(const char* text) {
    printf("%s⚠ %s%s\n", YELLOW, text, RESET);
}

void print_info(const char* text) {
    printf("%sℹ %s%s\n", BLUE, text, RESET);
}

void print_separator(void) {
    printf("%s─────────────────────────────────────────────────────────────%s\n", CYAN, RESET);
}

void print_score_bar(const char* label, int score, int max_score) {
    printf("%s%-25s: %s", BOLD, label, RESET);
    
    int bar_width = 30;
    int filled = (score * bar_width) / max_score;
    
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            if (score >= 70) printf("%s█%s", GREEN, RESET);
            else if (score >= 50) printf("%s█%s", YELLOW, RESET);
            else printf("%s█%s", RED, RESET);
        } else {
            printf("░");
        }
    }
    
    printf(" %d/%d\n", score, max_score);
}
