# Memory Bank Update Rules

## Overview

This document defines the rules and guidelines for updating the memory bank (project context) after each task completion in the Breakasm project.

## When to Update Memory Bank

The memory bank **MUST** be updated after **EVERY TASK COMPLETION**, regardless of task size or importance.

## Memory Bank Contents

After each task, the memory bank should include:

1. **Project Overview**
   - Project name and purpose
   - Current version/status
   - Key technologies used

2. **Architecture Summary**
   - Main components and their responsibilities
   - Data flow between components
   - Key data structures

3. **Recent Changes**
   - Files modified in the last task
   - New features added
   - Bugs fixed

4. **Known Issues**
   - Current limitations
   - Planned improvements

## Format

Memory bank updates should be stored in the project's `.gigacode/memory.md` file in Markdown format.

## Process

1. After completing any task, review the changes made
2. Update `.gigacode/memory.md` with:
   - Summary of changes
   - Updated architecture notes if needed
   - Any new patterns or conventions adopted
3. Ensure the memory bank reflects the current state of the project
4. Archive the update with a brief timestamped note

## Examples

### Task Completion Entry

```
## 2026-06-25 - Task: Add new assembler directive

- Added `PROCESSOR` directive support for 6502
- Modified: asm.cpp, asmops.cpp
- No breaking changes to existing functionality
```

## Responsibility

All team members are responsible for maintaining accurate memory bank entries after their tasks are complete.
