#!/bin/bash
# Generate code metrics (line counts, complexity, etc.)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
DOCS_DIR="$PROJECT_ROOT/docs"
SRC_DIR="$PROJECT_ROOT/src"
INCLUDE_DIR="$PROJECT_ROOT/include"

mkdir -p "$DOCS_DIR"

echo "Generating code metrics..."

> "$DOCS_DIR/code-metrics.md"
echo "# Code Metrics" >> "$DOCS_DIR/code-metrics.md"
echo "" >> "$DOCS_DIR/code-metrics.md"
echo "Generated: $(date)" >> "$DOCS_DIR/code-metrics.md"
echo "" >> "$DOCS_DIR/code-metrics.md"

> "$DOCS_DIR/complexity-report.md"
echo "# Complexity Report" >> "$DOCS_DIR/complexity-report.md"
echo "" >> "$DOCS_DIR/complexity-report.md"
echo "Generated: $(date)" >> "$DOCS_DIR/complexity-report.md"
echo "" >> "$DOCS_DIR/complexity-report.md"

# Use cloc if available
if command -v cloc &> /dev/null; then
    echo "## Line Counts (cloc)" >> "$DOCS_DIR/code-metrics.md"
    echo "" >> "$DOCS_DIR/code-metrics.md"
    echo "\`\`\`" >> "$DOCS_DIR/code-metrics.md"
    cloc "$SRC_DIR" "$INCLUDE_DIR" >> "$DOCS_DIR/code-metrics.md" 2>&1 || true
    echo "\`\`\`" >> "$DOCS_DIR/code-metrics.md"
    echo "" >> "$DOCS_DIR/code-metrics.md"
else
    echo "## Line Counts (manual)" >> "$DOCS_DIR/code-metrics.md"
    echo "" >> "$DOCS_DIR/code-metrics.md"
    echo "Note: Install cloc for detailed statistics" >> "$DOCS_DIR/code-metrics.md"
    echo "" >> "$DOCS_DIR/code-metrics.md"
    
    cpp_count=$(find "$SRC_DIR" -name "*.cpp" | wc -l)
    h_count=$(find "$INCLUDE_DIR" -name "*.h" | wc -l)
    cpp_lines=$(find "$SRC_DIR" -name "*.cpp" -exec wc -l {} + 2>/dev/null | tail -1 | awk '{print $1}')
    h_lines=$(find "$INCLUDE_DIR" -name "*.h" -exec wc -l {} + 2>/dev/null | tail -1 | awk '{print $1}')
    
    echo "  - C++ source files: $cpp_count" >> "$DOCS_DIR/code-metrics.md"
    echo "  - Header files: $h_count" >> "$DOCS_DIR/code-metrics.md"
    echo "  - Total C++ lines: $cpp_lines" >> "$DOCS_DIR/code-metrics.md"
    echo "  - Total header lines: $h_lines" >> "$DOCS_DIR/code-metrics.md"
    echo "" >> "$DOCS_DIR/code-metrics.md"
fi

# Function length statistics
echo "## Function Length Statistics" >> "$DOCS_DIR/code-metrics.md"
echo "" >> "$DOCS_DIR/code-metrics.md"
echo "Analyzing function lengths..." >> "$DOCS_DIR/code-metrics.md"
echo "" >> "$DOCS_DIR/code-metrics.md"

# Use cppcheck for complexity if available
if command -v cppcheck &> /dev/null; then
    echo "## Complexity Analysis (cppcheck)" >> "$DOCS_DIR/complexity-report.md"
    echo "" >> "$DOCS_DIR/complexity-report.md"
    echo "\`\`\`" >> "$DOCS_DIR/complexity-report.md"
    cppcheck --enable=all --suppress=missingIncludeSystem "$SRC_DIR" 2>&1 | head -100 >> "$DOCS_DIR/complexity-report.md" || true
    echo "\`\`\`" >> "$DOCS_DIR/complexity-report.md"
    echo "" >> "$DOCS_DIR/complexity-report.md"
else
    echo "Note: Install cppcheck for complexity analysis" >> "$DOCS_DIR/complexity-report.md"
    echo "" >> "$DOCS_DIR/complexity-report.md"
fi

echo "Code metrics generation complete!"
echo "  - Metrics: $DOCS_DIR/code-metrics.md"
echo "  - Complexity: $DOCS_DIR/complexity-report.md"
