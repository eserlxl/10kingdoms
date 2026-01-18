#!/bin/bash
# Analyze global variables, singletons, and initialization order

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
DOCS_DIR="$PROJECT_ROOT/docs"
SRC_DIR="$PROJECT_ROOT/src"

mkdir -p "$DOCS_DIR"

echo "Analyzing global state..."

> "$DOCS_DIR/globals-inventory.md"
echo "# Global Variables Inventory" >> "$DOCS_DIR/globals-inventory.md"
echo "" >> "$DOCS_DIR/globals-inventory.md"
echo "Generated: $(date)" >> "$DOCS_DIR/globals-inventory.md"
echo "" >> "$DOCS_DIR/globals-inventory.md"

> "$DOCS_DIR/initialization-order.md"
echo "# Initialization Order" >> "$DOCS_DIR/initialization-order.md"
echo "" >> "$DOCS_DIR/initialization-order.md"
echo "Generated: $(date)" >> "$DOCS_DIR/initialization-order.md"
echo "" >> "$DOCS_DIR/initialization-order.md"

# Find globals.cpp specifically
if [ -f "$SRC_DIR/globals.cpp" ]; then
    echo "## globals.cpp" >> "$DOCS_DIR/globals-inventory.md"
    echo "" >> "$DOCS_DIR/globals-inventory.md"
    
    # Extract global variable declarations
    grep -E "^(extern\s+)?[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*[=;]" "$SRC_DIR/globals.cpp" | while IFS= read -r line; do
        echo "  - \`$line\`" >> "$DOCS_DIR/globals-inventory.md"
    done
    echo "" >> "$DOCS_DIR/globals-inventory.md"
fi

# Find all global variables across source files
echo "## Global Variables in Source Files" >> "$DOCS_DIR/globals-inventory.md"
echo "" >> "$DOCS_DIR/globals-inventory.md"

find "$SRC_DIR" -name "*.cpp" -type f | while IFS= read -r file; do
    filename=$(basename "$file")
    
    # Look for global variable declarations (outside functions)
    # This is a simple heuristic - may need refinement
    globals=$(grep -E "^(extern\s+)?[a-zA-Z_][a-zA-Z0-9_<>:,\s*&]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*[=;]" "$file" 2>/dev/null | head -5)
    
    if [ -n "$globals" ]; then
        echo "### $filename" >> "$DOCS_DIR/globals-inventory.md"
        echo "$globals" | while IFS= read -r global; do
            echo "  - \`$global\`" >> "$DOCS_DIR/globals-inventory.md"
        done
        echo "" >> "$DOCS_DIR/globals-inventory.md"
    fi
done

# Find singleton patterns
echo "## Singleton Patterns" >> "$DOCS_DIR/globals-inventory.md"
echo "" >> "$DOCS_DIR/globals-inventory.md"

find "$SRC_DIR" "$PROJECT_ROOT/include" -name "*.cpp" -o -name "*.h" | xargs grep -l "getInstance\|instance()\|singleton" 2>/dev/null | while IFS= read -r file; do
    filename=$(basename "$file")
    echo "  - \`$filename\` - Possible singleton pattern" >> "$DOCS_DIR/globals-inventory.md"
done
echo "" >> "$DOCS_DIR/globals-inventory.md"

# Find static variables
echo "## Static Variables" >> "$DOCS_DIR/globals-inventory.md"
echo "" >> "$DOCS_DIR/globals-inventory.md"

find "$SRC_DIR" -name "*.cpp" -type f | while IFS= read -r file; do
    filename=$(basename "$file")
    statics=$(grep -E "^\s*static\s+" "$file" 2>/dev/null | head -10)
    
    if [ -n "$statics" ]; then
        echo "### $filename" >> "$DOCS_DIR/globals-inventory.md"
        echo "$statics" | while IFS= read -r static_var; do
            echo "  - \`$static_var\`" >> "$DOCS_DIR/globals-inventory.md"
        done
        echo "" >> "$DOCS_DIR/globals-inventory.md"
    fi
done

# Analyze initialization order from main()
if [ -f "$SRC_DIR/AM.cpp" ]; then
    echo "## Initialization Sequence (from main())" >> "$DOCS_DIR/initialization-order.md"
    echo "" >> "$DOCS_DIR/initialization-order.md"
    
    # Extract initialization calls from main
    grep -A 50 "int main" "$SRC_DIR/AM.cpp" | grep -E "\.init\(|::init\(|init\(\)" | head -20 | while IFS= read -r init_call; do
        echo "  1. $init_call" >> "$DOCS_DIR/initialization-order.md"
    done
    echo "" >> "$DOCS_DIR/initialization-order.md"
fi

echo "Global state analysis complete!"
echo "  - Inventory: $DOCS_DIR/globals-inventory.md"
echo "  - Initialization: $DOCS_DIR/initialization-order.md"
