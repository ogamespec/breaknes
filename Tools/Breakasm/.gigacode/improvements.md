# Breakasm - Improvements Options

**Date:** 2026-06-25  
**Project Version:** 1.3

---

## Overview

This document outlines various improvement options for the Breakasm 6502 assembler, categorized by priority and impact.

---

## High Priority - Core Functionality

### 1. Error Recovery and Better Diagnostics
**Current Issue:** Assembly stops on first error, limited error context.

**Proposed Improvements:**
- Continue assembly after errors to report multiple issues
- Add line numbers to all error messages (partially implemented)
- Provide column position for syntax errors
- Suggest corrections for common mistakes
- Add error codes for automated tooling

**Impact:** High  
**Effort:** Medium

---

### 2. List File Generation
**Current Issue:** No output showing where code was placed.

**Proposed Improvements:**
- Add `-l` flag to generate list file with:
  - Address, bytes, source line, labels
  - Cross-reference table
  - Memory map
- Optional detailed listing with expression evaluations

**Example Output:**
```
0000: 00       1:     ORG     $200
0200: EA       3:     NOP
0201: A9 05    5:     LDA     #5
```

**Impact:** High  
**Effort:** Medium

---

### 3. Symbol Export
**Current Issue:** Labels are internal only.

**Proposed Improvements:**
- Add `-s` flag to export symbol table to file
- Support multiple formats: C header, assembly include, JSON
- Include address, size, and source location for each symbol

**Use Case:** Linking multiple assembly files, debugging, IDE integration

**Impact:** High  
**Effort:** Low-Medium

---

## Medium Priority - Usability

### 4. Macro Support
**Current Issue:** No way to define reusable code blocks.

**Proposed Improvements:**
- Add `MACRO`/`ENDM` directives
- Support parameters with default values
- Local label generation (`&label` or similar)
- Nested macro expansion

**Example:**
```asm
MACRO PUSH_REG reg
    PHA
    TXA
    PHA
    TYA
    PHA
ENDM
```

**Impact:** Medium-High  
**Effort:** High

---

### 5. Conditional Assembly
**Current Issue:** No way to conditionally include code.

**Proposed Improvements:**
- `IF`, `ELSE`, `ENDIF` directives
- Support numeric and string comparisons
- `IFDEF`, `IFNDEF` for symbol existence
- `IFB`, `IFNB` for blank/non-blank parameters

**Example:**
```asm
IFDEF DEBUG
    JSR     DebugPrint
ENDIF
```

**Impact:** Medium  
**Effort:** Medium

---

### 6. Include Path Search
**Current Issue:** INCLUDE requires full path or same directory.

**Proposed Improvements:**
- Add `-I` flag for include search paths
- Search current directory first, then paths in order
- Support `<>` syntax for system includes

**Example:**
```asm
INCLUDE "common/macros.asm"
INCLUDE <nes/nes.inc>
```

**Impact:** Medium  
**Effort:** Low

---

### 7. Binary Output Options
**Current Issue:** Always outputs 64KB fixed file.

**Proposed Improvements:**
- Add `-o` flag to specify output format
- Support raw binary, Intel HEX, Motorola S-Record
- Add option to output only used memory range
- Support multiple output files

**Note:** Output format is fixed to 64KB PRG for 6502 systems.

**Impact:** Medium  
**Effort:** Medium

---

## Lower Priority - Advanced Features

### 8. Expression Enhancements
**Current Issue:** Limited expression capabilities.

**Proposed Improvements:**
- Support for `MOD` operator
- Bit manipulation macros
- String concatenation
- Address arithmetic with segment awareness

**Impact:** Low-Medium  
**Effort:** Medium

---

### 9. Linker Integration
**Current Issue:** No linking capability.

**Proposed Improvements:**
- Object file format (simple symbol table + code)
- Linker script support
- Memory region definition
- Overlay support

**Note:** Designed for 6502 memory model with 64KB address space.

**Impact:** Medium  
**Effort:** High

---

### 10. Debug Information
**Current Issue:** No debugging support.

**Proposed Improvements:**
- Add `-g` flag for debug info
- Source line mapping to addresses
- Symbol information in output
- Optional DWARF format support

**Impact:** Low-Medium  
**Effort:** High

---

## Lower Impact - Polish & Maintenance

### 11. Code Modernization
**Current Issue:** Legacy C++ patterns.

**Proposed Improvements:**
- Modern C++17/20 features
- RAII for memory management
- Range-based loops
- Smart pointers

**Impact:** Low (internal only)  
**Effort:** High

---

### 12. Unit Tests
**Current Issue:** No automated testing.

**Proposed Improvements:**
- Unit tests for expression parser
- Integration tests for assembly
- Fuzz testing for edge cases
- CI/CD integration

**Impact:** Medium-High  
**Effort:** High

---

### 13. Documentation
**Current Issue:** Minimal documentation.

**Proposed Improvements:**
- Detailed command reference
- Example projects
- Architecture documentation
- Migration guide from other assemblers

**Impact:** Medium  
**Effort:** Medium

---

### 14. Performance Optimization
**Current Issue:** No performance benchmarking.

**Proposed Improvements:**
- Profile large source files
- Optimize symbol lookup (hash tables)
- Parallel processing for multi-pass
- Memory usage optimization

**Impact:** Low-Medium  
**Effort:** Medium

---

## Quick Wins (Easy to Implement)

| # | Improvement | Effort | Impact |
|---|-------------|--------|--------|
| 1 | Better error messages with column info | Low | High |
| 2 | Add `-v` flag for version | Low | Low |
| 3 | Add `-h` flag for help | Low | Medium |
| 4 | Support both `;` and `REM` comments | Low | Low |
| 5 | Add `EQU` alias for `DEFINE` | Low | Low |
| 6 | Case-insensitive keywords | Low | Medium |
| 7 | Add `*` as current ORG address | Low | Medium |
| 8 | Support hex in strings | Low | Low |

---

## Recommended Implementation Order

### Phase 1 (Foundation)
1. Error recovery and better diagnostics
2. List file generation
3. Symbol export

### Phase 2 (Usability Boost)
4. Include path search
5. Binary output options
6. Conditional assembly

### Phase 3 (Advanced Features)
7. Macro support
8. Expression enhancements

### Phase 4 (Polish)
9. Documentation
10. Unit tests
11. Code modernization
