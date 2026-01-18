#!/usr/bin/env bash
# extract_unique_valgrind.sh
#
# Usage:
#   ./extract_unique_valgrind.sh valgrind.log   > unique_errors.txt
#   ./extract_unique_valgrind.sh valgrind.log  | less
#
# What it does
# ------------
# • Finds the first line of each leak or error block
#   –   “N bytes in M blocks … lost/still reachable/…”
#   –   “Invalid read/write …”
#   –   “Syscall param …”, “Use of uninitialised value …”, etc.
# • Collects the following stack-trace lines (those that still start with
#   “==…==”) until the next blank line or the next block start.
# • Drops the size/count headline (the part that changes run-to-run).
# • Replaces every concrete address (0x1234abcd) with the token **0xADDR**
#   so blocks that are identical except for ASLR addresses collapse together.
# • Prints each cleaned block once; subsequent duplicates are suppressed.

file=$1
[[ -z $file || ! -f $file ]] && {
    echo "Usage: $0 <valgrind_report.txt>" >&2
    exit 1
}

gawk '

##############################################################################
# Helper – emit the current block if we have not seen it before
##############################################################################
function flush_block(  key) {
    if (block == "") return
    key = block                                 # full cleaned text == hash
    if (!(key in seen)) {
        print block "\n"
        seen[key] = 1
    }
    block = ""                                  # reset for next round
}

##############################################################################
# Initialisation
##############################################################################
BEGIN {
    # Regex that marks the *first* line of every interesting block.
    start_pat = "(Invalid (read|write)|Syscall param|Use of uninitialised|"  \
                "Conditional jump|bytes in [0-9]+ blocks|still reachable|"   \
                "possibly lost|definitely lost|Process terminating)"
}

##############################################################################
# Main loop – read each line from the report
##############################################################################
{
    # We only care about Valgrind lines (==PID== …)
    if ($0 !~ /^==[0-9]+==/) next

    # Strip the “==PID== ” prefix – it is noise that prevents deduplication
    line = gensub(/^==[0-9]+==[[:space:]]*/, "", 1)

    # If this line *opens* a new block, flush whatever we collected before
    if (line ~ start_pat) {
        flush_block()
        # Skip the size/count headline itself (we want only the stack)
        if (line ~ /[0-9]+ bytes in [0-9]+ blocks/) next
    }

    # Mask memory addresses so ASLR doesn’t create spurious differences
    gsub(/0x[0-9a-fA-F]+/, "", line)
    gsub(/at : +/, "", line)
    gsub(/by : +/, "", line)

    # Keep every line that still belongs to a Valgrind stack trace
    block = block line "\n"
}

##############################################################################
# End of file – flush anything still waiting
##############################################################################
END { flush_block() }

' "$file"
