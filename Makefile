CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -Wno-unused-result
LDFLAGS = -lm

SOURCES = main.c utils.c tokenizer.c ats_score.c readability.c analyzer.c llm_interface.c resume_generator.c export.c
HEADERS = utils.h tokenizer.h ats_score.h readability.h analyzer.h llm_interface.h resume_generator.h export.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = resume_analyzer

# Default target
all: $(EXECUTABLE)

# Compile executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "✓ Build successful: $(EXECUTABLE)"

# Compile object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	@echo "✓ Clean complete"

# Rebuild
rebuild: clean all

# Run the program
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Valgrind memory check
memcheck: $(EXECUTABLE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXECUTABLE)

# Display help
help:
	@echo "Resume Analyzer - Build Instructions"
	@echo "===================================="
	@echo "make          - Build the project"
	@echo "make run      - Build and run"
	@echo "make clean    - Remove build artifacts"
	@echo "make rebuild  - Clean and build again"
	@echo "make memcheck - Run with Valgrind memory checker"
	@echo "make help     - Show this help message"

.PHONY: all clean rebuild run memcheck help
