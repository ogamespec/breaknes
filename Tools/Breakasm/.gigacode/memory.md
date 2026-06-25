# Project Memory Bank - Breakasm

**Last Updated:** 2026-06-25  
**Project Version:** 1.3

---

## Project Overview

**Name:** Breakasm  
**Type:** 6502 Assembler Tool  
**Purpose:** A simple assembler for generating 6502 machine code. Outputs fixed 64KB PRG files suitable for NES development and 6502 systems.

**Key Technologies:**
- C++ (legacy-style codebase)
- CMake build system
- Windows development environment

---

## Architecture Summary

### Core Components

1. **Main Module (main.cpp)**
   - Entry point
   - File I/O for assembly source and output PRG
   - Memory management for 64KB output buffer

2. **Assembler Core (asm.cpp, asm.h)**
   - Line parsing and tokenization
   - Label management with forward reference tracking
   - Expression evaluation infrastructure
   - Two-pass assembly process
   - Support for nested INCLUDE files via stack-based source tracking

3. **Operation Handlers (asmops.cpp, asmops.h)**
   - 6502 instruction encoders (BRK, RTI, RTS, PHP, CLC, etc.)
   - Load/Store operations (LDA, LDX, LDY, STA, STX, STY)
   - Branch and jump operations (BPL, BMI, BCC, JMP, JSR)
   - ALU operations (ORA, AND, EOR, ADC, CMP, SBC)
   - Shift operations (ASL, ROL, LSR, ROR)
   - Directives (ORG, INCLUDE, DEFINE, BYTE, WORD, END, PROCESSOR)

4. **Expression Evaluator (asmexpr.cpp, asmexpr.h)**
   - Lexical analyzer for complex expressions
   - Syntax tree builder
   - Semantic evaluation with label/define resolution
   - Supports arithmetic, bitwise, and logical operations

### Data Structures

- **label_s:** Stores label definitions with name, address, source file, line number, and composite expression flag
- **patch_s:** Tracks unresolved jumps/branches for second-pass patching
- **define_s:** Stores text substitution definitions
- **eval_t:** Expression evaluation result structure
- **token_t:** Lexical token for expression parsing

### Assembly Process

```
Pass 1:
  1. Parse source lines
  2. Build label table (with UNDEF for forward references)
  3. Track patches (branches/jumps with undefined targets)
  4. Emit known bytes

Pass 2:
  1. Evaluate composite expression labels
  2. Patch all branch/jump offsets
  3. Report errors for undefined labels
```

---

## Syntax

```
[LABEL:] COMMAND [OPERAND1, OPERAND2, OPERAND3] ; Comments
```

### Supported Directives

| Directive | Description |
|-----------|-------------|
| ORG | Set current emit offset (0-0xFFFF) |
| INCLUDE | Process nested source file |
| DEFINE | Define text constant |
| BYTE | Output byte(s) or string(s) |
| WORD | Output 16-bit value (little-endian) |
| END | Stop assembly |
| PROCESSOR | Processor type (informational) |

---

## Current State

### Files Modified in Last Session
- `pch.h` - Modified (git status shows modification)

### Recent Changes
- Initial project analysis completed
- Memory bank rules file created
- Improvements document created with 16 improvement options

### Known Issues/Limitations
- Codebase uses legacy C++ patterns (pre-stl style)
- No unit tests detected
- Hardcoded 64KB PRG size limit
- No error recovery beyond counting errors

---

## Development Guidelines

### Adding New Instructions
1. Add handler declaration in `asmops.h`
2. Implement handler in `asmops.cpp` following existing patterns
3. Register in `optab[]` array in `asm.cpp`
4. Ensure proper addressing mode support

### Adding New Directives
1. Add handler in `asmops.cpp`
2. Register in `optab[]` array in `asm.cpp`
3. Update documentation in Readme.md and ReadmeRus.md

### Expression Evaluation
- Use `eval_expr()` for complex expressions with label references
- Set `composite=1` on labels containing expressions
- Two-pass assembly resolves forward references

---

## Testing

### Test Files
- `test.asm` - Basic functionality test
- `testall.asm` - Comprehensive opcode coverage test

### Build System
- CMakeLists.txt defines build configuration
- Output executable: `breakasm`
- Usage: `breakasm <source.asm> <output.prg>`

---

## Improvement Options

See `.gigacode/improvements.md` for detailed improvement options covering:
- Error recovery and diagnostics
- List file generation
- Symbol export
- Macro support
- Conditional assembly
- Include path search
- Binary output options
- Multiple processor support
- Expression enhancements
- Linker integration
- NES ROM support
- Debug information
- And more...
