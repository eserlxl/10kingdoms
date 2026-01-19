#!/bin/bash
# Debug script for 10kingdoms
# This script sets up the environment and launches GDB with the binary
# Usage: ./debug.sh [--run] [--log-file FILE]
#   --run: Run the game directly (not in GDB) with stderr capture
#   --log-file FILE: Specify stderr log file (default: /tmp/10kingdoms_debug.log)

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

BINARY="$SCRIPT_DIR/src/10kingdoms"
DATA_DIR="$SCRIPT_DIR/data"
LOG_FILE="/tmp/10kingdoms_debug.log"

# Parse arguments
RUN_DIRECTLY=0
while [[ $# -gt 0 ]]; do
    case $1 in
        --run)
            RUN_DIRECTLY=1
            shift
            ;;
        --log-file)
            LOG_FILE="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--run] [--log-file FILE]"
            exit 1
            ;;
    esac
done

# Check if binary exists
if [ ! -f "$BINARY" ]; then
    echo "Error: Binary not found at $BINARY"
    exit 1
fi

# Check if data directory exists
if [ ! -d "$DATA_DIR" ]; then
    echo "Error: Data directory not found at $DATA_DIR"
    exit 1
fi

# Export environment variable for data path
export TKDATA="$DATA_DIR"

# Export debug channel for GameFile logging
export TKDEBUG=GameFile

# Change to data directory
cd "$DATA_DIR"

if [ $RUN_DIRECTLY -eq 1 ]; then
    # Run the game directly with stderr capture
    echo "Starting 10kingdoms with stderr capture"
    echo "Binary: $BINARY"
    echo "Data directory: $DATA_DIR"
    echo "Working directory: $(pwd)"
    echo "Stderr log file: $LOG_FILE"
    echo ""
    echo "Stderr output will be written to: $LOG_FILE"
    echo "You can monitor it in another terminal with: tail -f $LOG_FILE"
    echo ""
    
    # Run the game with stderr redirected to log file
    "$BINARY" 2> "$LOG_FILE"
    EXIT_CODE=$?
    
    echo ""
    echo "Game exited with code: $EXIT_CODE"
    echo "Stderr log saved to: $LOG_FILE"
    exit $EXIT_CODE
else
    # Launch GDB with logging
    echo "Starting GDB debugger for 10kingdoms"
    echo "Binary: $BINARY"
    echo "Data directory: $DATA_DIR"
    echo "Working directory: $(pwd)"
    echo "Stderr log file: $LOG_FILE"
    echo ""
    echo "Useful GDB commands:"
    echo "  run                    - Start the program"
    echo "  break <function>       - Set breakpoint at function"
    echo "  break <file>:<line>    - Set breakpoint at file:line"
    echo "  continue               - Continue execution"
    echo "  step                   - Step into function"
    echo "  next                   - Step over function"
    echo "  print <variable>       - Print variable value"
    echo "  bt                     - Show backtrace"
    echo "  whereami               - Show current location (custom alias)"
    echo "  bt_full                - Full backtrace (custom alias)"
    echo ""
    echo "Note: To capture stderr when running in GDB, use:"
    echo "  (gdb) set logging file $LOG_FILE"
    echo "  (gdb) set logging on"
    echo "  (gdb) run 2>&1 | tee $LOG_FILE"
    echo ""
    echo "Or run the game directly with: $0 --run"
    echo ""
    
    exec gdb "$BINARY"
fi
