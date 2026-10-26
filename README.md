# Advanced Resume Analyzer with AI Enhancement

A comprehensive C-based resume analysis tool with AI-powered suggestions using Ollama LLM integration.

## Features

### Phase 1: Core Analyzer (Pure C)
- ✅ Resume and job description parsing
- ✅ Tokenization and keyword frequency analysis
- ✅ ATS (Applicant Tracking System) score calculation (0-100)
- ✅ Missing keywords detection
- ✅ Resume section detection (Skills, Experience, Education, Projects)
- ✅ Readability scoring (Flesch-Kincaid, Coleman-Liau, Flesch Reading Ease)
- ✅ Detailed visual reports with color-coded output

### Phase 2: Advanced Features (Pure C with LLM Integration)
- ✅ Enhanced resume generation (3 versions: Conservative, Balanced, Aggressive)
- ✅ ATS optimization suggestions with AI recommendations
- ✅ Action verb suggestions
- ✅ Skill gap analysis with priority levels
- ✅ Cover letter generation from resume + job description
- ✅ Export capabilities:
  - Plain text enhanced resume
  - Markdown formatted resume
  - JSON metrics export
  - HTML visual report

## Technical Architecture

### Core Modules

#### Utils Module (`utils.h`, `utils.c`)
- String manipulation utilities
- Dynamic array management
- File I/O operations
- Color-coded terminal output
- Stop word filtering

#### Tokenizer Module (`tokenizer.h`, `tokenizer.c`)
- Text tokenization with stop word removal
- Frequency analysis
- Keyword extraction
- Sorting by frequency/position

#### ATS Score Module (`ats_score.h`, `ats_score.c`)
- ATS score calculation (40% keywords + 30% formatting + 30% readability)
- Job keyword extraction
- Keyword matching against resume
- Component-based scoring system

#### Readability Module (`readability.h`, `readability.c`)
- Flesch-Kincaid Grade Level calculation
- Coleman-Liau Index
- Flesch Reading Ease Score
- Syllable counting
- Text complexity analysis

#### Analyzer Module (`analyzer.h`, `analyzer.c`)
- Resume section detection
- Section extraction (Skills, Experience, Education, Projects, Summary, Contact)
- Section quality assessment

#### LLM Interface Module (`llm_interface.h`, `llm_interface.c`)
- Ollama integration
- Prompt generation and API calls
- AI-powered suggestions:
  - Bullet point improvement
  - Skills recommendations
  - Action verb suggestions
  - Summary enhancement
- Graceful fallback when LLM unavailable

#### Resume Generator Module (`resume_generator.h`, `resume_generator.c`)
- Enhanced resume generation
- Multiple version generation (Conservative/Balanced/Aggressive)
- Keyword injection
- Weak verb replacement
- Formatting optimization

#### Export Module (`export.h`, `export.c`)
- JSON metrics export
- HTML report generation
- Cover letter generation
- File output handling

## Installation & Compilation

### Prerequisites

1. **GCC Compiler**
   ```bash
   sudo apt-get install build-essential  # Ubuntu/Debian
   brew install gcc                       # macOS
   ```

2. **Ollama (Optional, for AI features)**
   - Download from: https://ollama.ai
   - Run: `ollama serve` in a separate terminal
   - Pull model: `ollama pull llama2`

### Building the Project

```bash
# Basic compilation
make

# Build and run
make run

# Clean build
make rebuild

# Memory check (requires Valgrind)
make memcheck

# Display help
make help
```

### For Windows Users

If using Windows without make, compile manually:

```bash
gcc -Wall -Wextra -O2 -std=c99 -Wno-unused-result -lm \
    main.c utils.c tokenizer.c ats_score.c readability.c \
    analyzer.c llm_interface.c resume_generator.c export.c \
    -o resume_analyzer.exe
```

Then run:
```bash
resume_analyzer.exe
```

## Usage

### Running the Program

```bash
./resume_analyzer
```

The program opens an interactive menu with the following options:

### Menu Options

**1. Analyze Resume**
- Calculates comprehensive ATS score
- Provides keyword matching analysis
- Generates readability metrics
- Detects resume sections
- Shows top keywords

**2. Generate Enhanced Resume (3 Versions)**
- Conservative: Minimal changes, focus on correctness
- Balanced: Optimal blend of keywords and professionalism
- Aggressive: Maximum keyword integration and impact

**3. Generate Cover Letter**
- Automatically generates professional cover letter
- Tailored to job description
- Saved to `cover_letter.txt`

**4. View ATS Optimization Suggestions**
- Skills recommendations
- Action verb suggestions
- Sample bullet point improvements
- AI-powered (if Ollama available)

**5. View Readability Analysis**
- Flesch-Kincaid Grade Level
- Coleman-Liau Index
- Flesch Reading Ease Score
- Word count and complexity metrics

**6. Export Reports**
- JSON metrics export
- HTML visual report with charts
- All analysis saved to files

**7. Exit**
- Close the application

## Input Files

### resume.txt
Your resume file (included with sample)

```
JOHN ANDERSON
123 Main Street, Springfield, IL 62701
(217) 555-0123 | john.anderson@email.com

PROFESSIONAL SUMMARY
[Your professional summary]

PROFESSIONAL EXPERIENCE
[Your experience]

EDUCATION
[Your education]

TECHNICAL SKILLS
[Your skills]
```

### job_description.txt
The job description you're applying for (included with sample)

```
SENIOR BACKEND ENGINEER

Location: Remote / San Francisco, CA

RESPONSIBILITIES
[Job responsibilities]

REQUIRED QUALIFICATIONS
[Required skills and experience]
```

## Output Files

After running analysis, the following files are generated:

- `enhanced_resume.txt` - Optimized resume (balanced version)
- `cover_letter.txt` - Generated cover letter
- `analysis_report.json` - Metrics in JSON format
- `analysis_report.html` - Visual HTML report

## ATS Score Breakdown

### Score Calculation (0-100)

| Component | Weight | Factors |
|-----------|--------|---------|
| Keywords | 40% | Job keywords found in resume |
| Formatting | 30% | Sections present, structure |
| Readability | 30% | Word count, action verbs, structure |

### Score Interpretation

- **80-100**: Excellent - Well-optimized for ATS systems
- **60-79**: Good - Minor improvements needed
- **40-59**: Fair - Consider revisions recommended
- **Below 40**: Poor - Major improvements needed

## Readability Indices

### Flesch Reading Ease
- 90-100: Very easy (5th grade)
- 80-89: Easy (6th grade)
- 70-79: Fairly easy (7th grade)
- 60-69: Standard (8-9th grade)
- 50-59: Fairly difficult (10-12th grade)
- 30-49: Difficult (College level)
- 0-29: Very difficult (College graduate)

### Flesch-Kincaid Grade Level
- Represents U.S. grade level needed to understand the text
- Lower is better for readability
- Target: 8-12 for professional documents

## Ollama Integration

### Setup

1. Install Ollama from https://ollama.ai
2. Run Ollama service:
   ```bash
   ollama serve
   ```

3. In another terminal, pull the llama2 model:
   ```bash
   ollama pull llama2
   ```

4. The program will automatically detect Ollama and use AI features

### AI Features (if Ollama available)

- Improved bullet point suggestions
- Skills recommendations based on job description
- Summary enhancement
- Action verb recommendations

### Fallback Mode

If Ollama is not available:
- Program provides rule-based suggestions
- All features still work with local analysis
- No external dependencies required

## Example Workflow

```
1. Start the program
   $ ./resume_analyzer

2. Analyze your resume (Option 1)
   - Get ATS score and recommendations
   - See readability analysis
   - Identify missing keywords

3. Generate enhanced versions (Option 2)
   - Review 3 versions
   - Choose best fit
   - Export enhanced_resume.txt

4. Get optimization suggestions (Option 4)
   - AI-powered recommendations
   - Specific improvements

5. Generate cover letter (Option 3)
   - Automatic generation
   - Customized to job

6. Export all reports (Option 6)
   - HTML report for review
   - JSON metrics for further analysis
```

## Performance Characteristics

- **Time Complexity**: O(n) for most operations (n = text length)
- **Space Complexity**: O(n) for tokenization and storage
- **Memory Usage**: < 50MB typical
- **Max Resume Size**: ~100KB (configurable)

## Valgrind Memory Testing

```bash
make memcheck

# With custom parameters
valgrind --leak-check=full --show-leak-kinds=all ./resume_analyzer
```

## Troubleshooting

### Issue: "Could not read resume.txt"
- Ensure `resume.txt` exists in current directory
- Check file permissions
- Verify path is correct

### Issue: Ollama not connecting
- Make sure `ollama serve` is running
- Check if running on default port 11434
- Program will fallback to rule-based suggestions

### Issue: Compilation errors
- Ensure GCC is installed: `gcc --version`
- Check all header files are present
- Try `make clean` then `make`

### Issue: Memory leak detected
- Run with `make memcheck`
- Check Valgrind output for specific issues
- All allocations should have corresponding frees

## Code Statistics

- **Lines of Code**: ~2500
- **Number of Functions**: 70+
- **Memory Safe**: Yes (malloc/free properly managed)
- **Standard Compliance**: C99

## Advanced Features Explained

### Multiple Resume Versions

**Conservative**
- Minimal changes
- Perfect for risk-averse applications
- Focus on accuracy
- Use when format is critical

**Balanced**
- Keywords integrated smoothly
- Professional tone maintained
- Recommended for most applications
- Good mix of safety and optimization

**Aggressive**
- Maximum keyword inclusion
- Strong action verbs throughout
- Optimized for ATS
- Use for highly competitive positions

### Section Detection

Automatically identifies and extracts:
- Contact Information
- Professional Summary
- Professional Experience
- Education
- Technical Skills
- Projects

## Best Practices

1. **Before Using**: Customize `resume.txt` and `job_description.txt`
2. **ATS Optimization**: Aim for score > 70
3. **Readability**: Keep Flesch Reading Ease between 50-80
4. **Keywords**: Include 60-70% of job keywords
5. **Formatting**: Keep sections clearly labeled
6. **Review Output**: Always review generated content before submitting

## Limitations

- Detects common section names only
- Readability formulas designed for English
- LLM quality depends on Ollama model chosen
- Does not validate professional accuracy
- Manual review recommended before submission

## Future Enhancements

- Support for multiple languages
- LinkedIn profile integration
- Interview preparation suggestions
- Salary negotiation guidance
- Recruiter keyword analysis
- Video interview tips

## License

This project is provided as-is for educational and professional use.

## Support & Contributing

For issues or suggestions, review the code comments and Valgrind output for debugging.

## Disclaimer

This tool provides suggestions and analysis. Always review generated content for accuracy and appropriateness before submitting resumes to employers. The tool is not responsible for hiring decisions.

---

**Version**: 1.0  
**Last Updated**: May 2026  
**Status**: Production Ready
