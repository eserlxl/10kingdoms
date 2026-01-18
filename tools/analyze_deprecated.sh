#!/bin/bash
# Analyze deprecated and unsafe code patterns

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
DOCS_DIR="$PROJECT_ROOT/docs"
SRC_DIR="$PROJECT_ROOT/src"
INCLUDE_DIR="$PROJECT_ROOT/include"

mkdir -p "$DOCS_DIR"

echo "Analyzing deprecated patterns..."

> "$DOCS_DIR/deprecated-patterns.md"
echo "# Deprecated and Unsafe Pattern Analysis" >> "$DOCS_DIR/deprecated-patterns.md"
echo "" >> "$DOCS_DIR/deprecated-patterns.md"
echo "Generated: $(date)" >> "$DOCS_DIR/deprecated-patterns.md"
echo "" >> "$DOCS_DIR/deprecated-patterns.md"

> "$DOCS_DIR/memory-management-audit.md"
echo "# Memory Management Audit" >> "$DOCS_DIR/memory-management-audit.md"
echo "" >> "$DOCS_DIR/memory-management-audit.md"
echo "Generated: $(date)" >> "$DOCS_DIR/memory-management-audit.md"
echo "" >> "$DOCS_DIR/memory-management-audit.md"

> "$DOCS_DIR/code-quality-issues.md"
echo "# Code Quality Issues" >> "$DOCS_DIR/code-quality-issues.md"
echo "" >> "$DOCS_DIR/code-quality-issues.md"
echo "Generated: $(date)" >> "$DOCS_DIR/code-quality-issues.md"
echo "" >> "$DOCS_DIR/code-quality-issues.md"

# Find #pragma pack(1)
echo "## #pragma pack(1) Usage" >> "$DOCS_DIR/deprecated-patterns.md"
echo "" >> "$DOCS_DIR/deprecated-patterns.md"
find "$SRC_DIR" "$INCLUDE_DIR" -name "*.cpp" -o -name "*.h" | xargs grep -n "#pragma pack" 2>/dev/null | while IFS= read -r line; do
    echo "  - \`$line\`" >> "$DOCS_DIR/deprecated-patterns.md"
done
echo "" >> "$DOCS_DIR/deprecated-patterns.md"

# Find register keyword
echo "## register Keyword Usage" >> "$DOCS_DIR/deprecated-patterns.md"
echo "" >> "$DOCS_DIR/deprecated-patterns.md"
find "$SRC_DIR" "$INCLUDE_DIR" -name "*.cpp" -o -name "*.h" | xargs grep -n "\bregister\b" 2>/dev/null | head -20 | while IFS= read -r line; do
    echo "  - \`$line\`" >> "$DOCS_DIR/deprecated-patterns.md"
done
echo "" >> "$DOCS_DIR/deprecated-patterns.md"

# Find C-style casts
echo "## C-Style Casts" >> "$DOCS_DIR/deprecated-patterns.md"
echo "" >> "$DOCS_DIR/deprecated-patterns.md"
find "$SRC_DIR" -name "*.cpp" | xargs grep -n "([a-zA-Z_][a-zA-Z0-9_<>:,\s*&]*)" 2>/dev/null | grep -v "//" | head -30 | while IFS= read -r line; do
    echo "  - \`$line\`" >> "$DOCS_DIR/deprecated-patterns.md"
done
echo "" >> "$DOCS_DIR/deprecated-patterns.md"

# Memory management
echo "## Manual Memory Management" >> "$DOCS_DIR/memory-management-audit.md"
echo "" >> "$DOCS_DIR/memory-management-audit.md"

# malloc/free
echo "### malloc/free usage" >> "$DOCS_DIR/memory-management-audit.md"
find "$SRC_DIR" -name "*.cpp" | xargs grep -n "\bmalloc\b\|\bfree\b" 2>/dev/null | head -20 | while IFS= read -r line; do
    echo "  - \`$line\`" >> "$DOCS_DIR/memory-management-audit.md"
done
echo "" >> "$DOCS_DIR/memory-management-audit.md"

# new/delete
echo "### new/delete usage" >> "$DOCS_DIR/memory-management-audit.md"
find "$SRC_DIR" -name "*.cpp" | xargs grep -n "\bnew\b\|\bdelete\b" 2>/dev/null | head -30 | while IFS= read -r line; do
    echo "  - \`$line\`" >> "$DOCS_DIR/memory-management-audit.md"
done
echo "" >> "$DOCS_DIR/memory-management-audit.md"

# Raw pointers
echo "## Raw Pointer Usage" >> "$DOCS_DIR/memory-management-audit.md"
echo "" >> "$DOCS_DIR/memory-management-audit.md"
find "$SRC_DIR" "$INCLUDE_DIR" -name "*.cpp" -o -name "*.h" | xargs grep -n "\*\s*[a-zA-Z_]" 2>/dev/null | grep -v "//" | head -30 | while IFS= read -r line; do
    echo "  - \`$line\`" >> "$DOCS_DIR/memory-management-audit.md"
done
echo "" >> "$DOCS_DIR/memory-management-audit.md"

echo "Deprecated pattern analysis complete!"
echo "  - Patterns: $DOCS_DIR/deprecated-patterns.md"
echo "  - Memory: $DOCS_DIR/memory-management-audit.md"
echo "  - Quality: $DOCS_DIR/code-quality-issues.md"
