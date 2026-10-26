@echo off
REM Resume Analyzer - Windows Test Script

echo.
echo ========================================
echo Resume Analyzer - Compilation & Test
echo ========================================
echo.

cd /d "%~dp0"

echo Checking for required files...
if not exist resume.txt (
    echo ERROR: resume.txt not found!
    exit /b 1
)
if not exist job_description.txt (
    echo ERROR: job_description.txt not found!
    exit /b 1
)

echo [OK] All required files present
echo.

echo Compiling C code...
gcc -Wall -Wextra -O2 -std=c99 -Wno-unused-result -lm ^
    main.c utils.c tokenizer.c ats_score.c readability.c ^
    analyzer.c llm_interface.c resume_generator.c export.c ^
    -o resume_analyzer.exe

if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
)

echo [OK] Compilation successful
echo [INFO] Executable: resume_analyzer.exe
echo.

echo Program is ready. Usage:
echo   resume_analyzer.exe
echo.
echo Features:
echo   1. Analyze Resume
echo   2. Generate Enhanced Resume (3 Versions)
echo   3. Generate Cover Letter
echo   4. View ATS Optimization Suggestions
echo   5. View Readability Analysis
echo   6. Export Reports (JSON + HTML)
echo   7. Exit
echo.

pause
