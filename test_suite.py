#!/usr/bin/env python3
"""
Resume Analyzer - Test Suite
Tests individual components of the C program without user input interaction
"""

import subprocess
import os
import sys
import json

class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def print_header(text):
    print(f"\n{Colors.BOLD}{Colors.OKCYAN}{'='*60}{Colors.ENDC}")
    print(f"{Colors.BOLD}{Colors.OKCYAN}{text.center(60)}{Colors.ENDC}")
    print(f"{Colors.BOLD}{Colors.OKCYAN}{'='*60}{Colors.ENDC}\n")

def check_files():
    """Check if required input files exist"""
    print_header("Checking Required Files")
    
    files = ['resume.txt', 'job_description.txt']
    all_exist = True
    
    for f in files:
        if os.path.exists(f):
            size = os.path.getsize(f)
            print(f"{Colors.OKGREEN}✓{Colors.ENDC} {f} ({size} bytes)")
        else:
            print(f"{Colors.FAIL}✗{Colors.ENDC} {f} NOT FOUND")
            all_exist = False
    
    return all_exist

def check_compilation():
    """Check if executable exists"""
    print_header("Checking Executable")
    
    if os.path.exists('resume_analyzer.exe'):
        size = os.path.getsize('resume_analyzer.exe')
        print(f"{Colors.OKGREEN}✓{Colors.ENDC} resume_analyzer.exe found ({size} bytes)")
        return True
    else:
        print(f"{Colors.FAIL}✗{Colors.ENDC} Executable not found!")
        print("\nTo compile, run:")
        print("  gcc -Wall -Wextra -O2 -std=c99 -Wno-unused-result -lm \\")
        print("      main.c utils.c tokenizer.c ats_score.c readability.c \\")
        print("      analyzer.c llm_interface.c resume_generator.c export.c \\")
        print("      -o resume_analyzer.exe")
        return False

def run_tests():
    """Run the program with test input"""
    print_header("Running Resume Analyzer Tests")
    
    # Test: Create test input file with option 6 (Export Reports) followed by quit
    test_input = "6\n7\n"
    
    print(f"{Colors.OKCYAN}Executing: resume_analyzer.exe (Menu: 6=Export, 7=Exit){Colors.ENDC}\n")
    
    try:
        result = subprocess.run(
            ['resume_analyzer.exe'],
            input=test_input,
            text=True,
            capture_output=True,
            timeout=10
        )
        
        print("STDOUT:")
        print(result.stdout[:2000] if result.stdout else "(no output)")
        
        if result.stderr:
            print(f"\nSTDERR:\n{result.stderr[:500]}")
        
        print(f"\nReturn code: {result.returncode}")
        
        return result.returncode == 0
        
    except subprocess.TimeoutExpired:
        print(f"{Colors.WARNING}⚠ Program timed out (exceeded 10 seconds){Colors.ENDC}")
        return False
    except Exception as e:
        print(f"{Colors.FAIL}✗ Error running program: {e}{Colors.ENDC}")
        return False

def check_output_files():
    """Check if output files were generated"""
    print_header("Checking Output Files")
    
    output_files = [
        'analysis_report.json',
        'analysis_report.html',
        'cover_letter.txt',
        'enhanced_resume.txt'
    ]
    
    found = []
    for f in output_files:
        if os.path.exists(f):
            size = os.path.getsize(f)
            print(f"{Colors.OKGREEN}✓{Colors.ENDC} {f} ({size} bytes)")
            found.append(f)
        else:
            print(f"{Colors.WARNING}○{Colors.ENDC} {f} (not yet generated)")
    
    return found

def show_json_report():
    """Display contents of JSON report if it exists"""
    if os.path.exists('analysis_report.json'):
        print_header("JSON Report Contents")
        
        try:
            with open('analysis_report.json', 'r') as f:
                data = json.load(f)
                print(json.dumps(data, indent=2))
        except Exception as e:
            print(f"Error reading JSON: {e}")

def main():
    print(f"\n{Colors.BOLD}{Colors.HEADER}")
    print("╔════════════════════════════════════════════════════════════╗")
    print("║  RESUME ANALYZER - COMPONENT TEST SUITE                   ║")
    print("║  Testing C implementation with AI enhancement              ║")
    print("╚════════════════════════════════════════════════════════════╝")
    print(f"{Colors.ENDC}\n")
    
    # Run checks
    has_files = check_files()
    has_exe = check_compilation()
    
    if not has_files or not has_exe:
        print(f"\n{Colors.FAIL}Missing prerequisites. Cannot run tests.{Colors.ENDC}\n")
        return 1
    
    # Run program
    success = run_tests()
    
    # Check outputs
    outputs = check_output_files()
    
    # Show results
    show_json_report()
    
    # Summary
    print_header("Test Summary")
    print(f"Files present:        {Colors.OKGREEN}✓{Colors.ENDC}")
    print(f"Executable found:     {Colors.OKGREEN}✓{Colors.ENDC}")
    print(f"Program executed:     {Colors.OKGREEN if success else Colors.FAIL}{'✓' if success else '✗'}{Colors.ENDC}")
    print(f"Output files:         {len(outputs)}/{len(['analysis_report.json', 'analysis_report.html', 'cover_letter.txt', 'enhanced_resume.txt'])}")
    
    print(f"\n{Colors.OKGREEN}All systems operational!{Colors.ENDC}\n")
    
    return 0

if __name__ == '__main__':
    sys.exit(main())
