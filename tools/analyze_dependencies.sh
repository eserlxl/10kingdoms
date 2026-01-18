#!/bin/bash
# Analyze C++ source file dependencies and generate dependency graph

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
DOCS_DIR="$PROJECT_ROOT/docs"
SRC_DIR="$PROJECT_ROOT/src"
INCLUDE_DIR="$PROJECT_ROOT/include"

mkdir -p "$DOCS_DIR"

echo "Analyzing dependencies..."

# Find all C++ source files
find "$SRC_DIR" -name "*.cpp" -type f | sort > /tmp/src_files.txt

# Find all header files
find "$INCLUDE_DIR" -name "*.h" -type f | sort > /tmp/header_files.txt

# Generate dependency list
echo "digraph dependencies {" > "$DOCS_DIR/dependency-graph.dot"
echo "  rankdir=LR;" >> "$DOCS_DIR/dependency-graph.dot"
echo "  node [shape=box];" >> "$DOCS_DIR/dependency-graph.dot"
echo "" >> "$DOCS_DIR/dependency-graph.dot"

# Analyze includes in source files
> "$DOCS_DIR/dependencies.txt"
echo "# Source File Dependencies" >> "$DOCS_DIR/dependencies.txt"
echo "# Generated: $(date)" >> "$DOCS_DIR/dependencies.txt"
echo "" >> "$DOCS_DIR/dependencies.txt"

external_deps=()
circular_deps=()

while IFS= read -r src_file; do
    src_name=$(basename "$src_file" .cpp)
    src_rel=$(realpath --relative-to="$PROJECT_ROOT" "$src_file" | sed 's|/|_|g' | sed 's|\.|_|g')
    
    # Extract includes
    includes=$(grep -E '^#include\s*[<"]' "$src_file" | sed 's/#include\s*[<"]//' | sed 's/[>"]//' | tr -d ' ')
    
    if [ -n "$includes" ]; then
        echo "" >> "$DOCS_DIR/dependencies.txt"
        echo "## $src_name.cpp" >> "$DOCS_DIR/dependencies.txt"
        
        for inc in $includes; do
            # Check if it's a project header
            if [ -f "$INCLUDE_DIR/$inc" ]; then
                header_name=$(basename "$inc" .h)
                header_rel=$(echo "$inc" | sed 's|/|_|g' | sed 's|\.|_|g')
                echo "  \"$src_rel\" -> \"$header_rel\";" >> "$DOCS_DIR/dependency-graph.dot"
                echo "  - $inc (project header)" >> "$DOCS_DIR/dependencies.txt"
            else
                # External dependency
                lib_name=$(echo "$inc" | cut -d'/' -f1)
                if [[ ! " ${external_deps[@]} " =~ " ${lib_name} " ]]; then
                    external_deps+=("$lib_name")
                fi
                echo "  - $inc (external)" >> "$DOCS_DIR/dependencies.txt"
            fi
        done
    fi
done < /tmp/src_files.txt

echo "}" >> "$DOCS_DIR/dependency-graph.dot"

# List external dependencies
echo "" >> "$DOCS_DIR/dependencies.txt"
echo "# External Dependencies" >> "$DOCS_DIR/dependencies.txt"
for dep in "${external_deps[@]}"; do
    echo "  - $dep" >> "$DOCS_DIR/dependencies.txt"
done

# Try to generate PNG if graphviz is available
if command -v dot &> /dev/null; then
    echo "Generating dependency graph PNG..."
    dot -Tpng "$DOCS_DIR/dependency-graph.dot" -o "$DOCS_DIR/dependency-graph.png" 2>/dev/null || echo "Warning: Could not generate PNG (graph may be too large)"
else
    echo "Note: Install graphviz (dot) to generate PNG visualization"
fi

echo "Dependency analysis complete!"
echo "  - Graph: $DOCS_DIR/dependency-graph.dot"
echo "  - Dependencies: $DOCS_DIR/dependencies.txt"
if [ -f "$DOCS_DIR/dependency-graph.png" ]; then
    echo "  - Visualization: $DOCS_DIR/dependency-graph.png"
fi
