# Breakasm Expression Parser

**Last Updated:** 2026-06-25  
**Files:** `asmexpr.h`, `asmexpr.cpp`

---

## Overview

The expression parser is a recursive descent parser that processes complex arithmetic and logical expressions using a syntax tree approach. It resolves labels, defines, and computes numeric results.

---

## Architecture

### Data Structures

#### token_t
```cpp
struct token_t {
    int     type;       // TOKEN_NUMBER, TOKEN_IDENT, TOKEN_STRING, TOKEN_OP
    OPS     op;         // Operation type (see OPS enum)
    long    number;     // Numeric value for TOKEN_NUMBER
    char    string[0x100];  // String representation
};
```

#### node_t
```cpp
struct node_t {
    node_t* lvalue;     // Left child
    node_t* rvalue;     // Right child
    token_t* token;     // Associated token
    int     depth;      // Tree depth for operator precedence
};
```

#### tree_t
```cpp
struct tree_t {
    std::list<node_t*> nodes;   // All tree nodes
    node_t* curr;               // Current node for tree growth
    int     depth;              // Current depth
    int     prio;               // Priority stack index
    int     prio_stack[1000];   // Priority stack
    int     initialized;        // Initialization flag
};
```

#### OPS Enum
```cpp
enum class OPS {
    NOP = 0,
    LPAREN, RPAREN,       // ( )
    PLUS, MINUS,          // + -
    NOT, NEG,             // ! ~
    MUL, DIV, MOD,        // * / %
    SHL, SHR, ROTL, ROTR, // << >> <<< >>>
    GREATER, GREATER_EQ, LESS, LESS_EQ, // > >= < <=
    LOGICAL_EQ, LOGICAL_NOTEQ,  // == !=
    AND, OR, XOR,         // & | ^
    EQ                    // =
};
```

---

## Token Types

| Type | Description | Examples |
|------|-------------|----------|
| TOKEN_NUMBER | Numeric values | `#$12`, `$AABB`, `255` |
| TOKEN_IDENT | Identifiers (labels/defines) | `MYDATA`, `entry_size` |
| TOKEN_STRING | String literals | `"Hello"`, `'Hello'` |
| TOKEN_OP | Operators | `+`, `-`, `*`, `<<`, `==`, etc. |

---

## Operator Precedence

| Priority | Operators | Description |
|----------|-----------|-------------|
| 1 | NOP | No operation (base) |
| 4 | `&` | Bitwise AND |
| 5 | `^` | Bitwise XOR |
| 6 | `|` | Bitwise OR |
| 7 | `==`, `!=` | Logical equality |
| 7 | `>`, `>=`, `<`, `<=` | Comparison |
| 8 | `<<`, `>>`, `<<<`, `>>>` | Shifts |
| 9 | `+`, `-` | Addition/Subtraction |
| 9 | `=` | Assignment |
| 10 | `*`, `/`, `%` | Multiplication/Division |
| 11 | `!`, `~` | Unary NOT/NEG |
| 12 | `(`, `)` | Parentheses |

---

## Parsing Process

### Step 1: Lexical Analysis (tokenize)

Converts input string into a stream of tokens.

**Behavior:**
- Skips whitespace
- Handles `#` and `$` prefixes for hex/decimal
- Detects identifiers (alphanumeric + underscore)
- Handles quoted strings
- Parses multi-character operators first (`<<`, `>>`, etc.)

**Example:**
```
"SPR_TAB + 32 * entry_size"
→ [TOKEN_IDENT: SPR_TAB, TOKEN_OP: PLUS, TOKEN_NUMBER: 32, TOKEN_OP: MUL, TOKEN_IDENT: entry_size]
```

### Step 2: Syntax Tree Growth (grow)

Builds the syntax tree with proper operator precedence.

**Algorithm:**
1. Initialize tree with first token
2. For each subsequent token:
   - If `(`: increase depth/priority
   - If `)`: decrease depth/priority
   - If operator: compare priority with stack
     - Higher priority: increase depth
     - Lower priority: decrease depth
   - Attach node as right child of current

**Key properties:**
- Binary tree structure
- Left child: previous node
- Right child: next node
- Depth determines precedence in evaluation

### Step 3: Semantic Evaluation (evaluate)

Traverses the syntax tree and computes the result.

**Algorithm:**
1. Start at root expression node
2. For each node at current depth:
   - Process optional unary operator (`!`, `~`)
   - Resolve identifier to value (label or define)
   - Process optional binary operation with previous result
3. Return final result

**Value Resolution:**
- **TOKEN_NUMBER**: Use directly
- **TOKEN_IDENT**: 
  - First check if it's a DEFINE, recursively evaluate replacement
  - Then check if it's a LABEL, use its address
  - Error if undefined
- **TOKEN_STRING**: Not supported in expressions (syntax error)

---

## Usage

### Function Signature
```cpp
long eval_expr(char* text, bool debug, bool quiet);
```

### Parameters
| Parameter | Description |
|-----------|-------------|
| `text` | Expression string (e.g., `"SPR_TAB + 32 * entry_size + 12"`) |
| `debug` | Output parsing steps to stdout |
| `quiet` | Suppress error messages (for testing) |

### Return Value
- **Success**: Computed numeric result
- **Error**: Returns 0 (if `quiet=false`, errors are printed)

### Examples

```cpp
// Simple arithmetic
eval_expr("10 + 20 * 3", false, false);  // Returns 70

// With labels
eval_expr("MYDATA + 10", false, false);  // Returns label address + 10

// With defines
DEFINE entry_size #32
eval_expr("2 * entry_size", false, false);  // Returns 64

// Complex expression
eval_expr("(base + index) << 2", false, false);  // Returns (base+index)*4

// String in expression (error)
eval_expr('"Hello" + 1', false, false);  // Syntax error
```

---

## Key Features

### 1. Forward References
Labels can be used before they are defined. The parser stores them as `UNDEF` and resolves during the second pass.

### 2. Define Substitution
Defines are recursively expanded before evaluation.

### 3. Composite Expressions
Labels containing expressions are marked with `composite=1` and evaluated during the second pass.

### 4. Error Handling
- Unknown characters: Syntax error
- Undefined identifiers: Error unless `quiet=true`
- Unmatched parentheses: Warning (ignored silently)

### 5. Debug Output
When `debug=true`, outputs:
- Token stream
- Tree structure
- Evaluation steps

---

## Implementation Notes

### Design Decisions

1. **Two-Pass Assembly**: First pass builds labels, second pass evaluates expressions. This handles forward references.

2. **Syntax Tree Approach**: Allows complex expressions with proper operator precedence without regex or complex parsing libraries.

3. **Depth-Based Precedence**: Operator depth in the tree determines evaluation order, not explicit precedence tables.

4. **Memory Management**: All tokens and nodes are allocated on heap and cleaned up after evaluation.

### Known Limitations

1. **No String Operations**: Strings cannot be used in expressions (syntax error).
2. **No Floating Point**: All calculations are integer-based.
3. **No Bitwise Shifts in Output**: Shift operators work in expressions but result is used as-is.
4. **Assignment Operator**: `=` is parsed but has no effect in this context.

---

## Testing

### Test Expression
```asm
; Test all operators
org $200

; Arithmetic
res1: word 10 + 20
res2: word 100 - 50
res3: word 5 * 10
res4: word 100 / 4
res5: word 17 % 5

; Bitwise
res6: word $FF & $0F
res7: word $FF | $0F
res8: word $FF ^ $0F
res9: word ~$00
res10: word $01 << 4
res11: word $10 >> 2

; Comparison (returns 0 or 1)
res12: word 10 > 5
res13: word 10 < 5
res14: word 10 == 10
res15: word 10 != 5

; Parentheses
res16: word (10 + 20) * 2
res17: word 10 + (20 * 2)

; With labels
start:
    nop
end:
    word end - start
```

---

## Future Enhancements

1. **Bit Rotation**: Implement `ROL`/`ROR` for rotation operations
2. **String Length**: Add `len("string")` function
3. **Modulo Operator**: Fix `%` operator (currently in grammar but not in operations list)
4. **Type Checking**: Add compile-time type validation
5. **Macro Expansion**: Support macros in expression context
