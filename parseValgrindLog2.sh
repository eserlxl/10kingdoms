#!/usr/bin/env bash
# extract_unique_valgrind.sh  —  v3 fuzzy-dedupe
#
# Usage:
#   ./extract_unique_valgrind.sh valgrind.log   > unique_errors.txt
#   ./extract_unique_valgrind.sh valgrind.log  | less
#
# Dedup strategy:
#   - Collect Valgrind error/leak blocks.
#   - Compute a canonical *key* per block with addresses masked & line numbers
#     replaced by :LINE (tolerates minor source-line drift).
#   - Emit only the first raw instance of each key; suppress later duplicates.

file=$1
[[ -z $file || ! -f $file ]] && {
    echo "Usage: $0 <valgrind_report.txt>" >&2
    exit 1
}

gawk '
##############################################################################
# Emit current block if new (by canonical key)
##############################################################################
function flush_block(  key) {
    if (block_raw == "") return
    key = block_key
    if (!(key in seen)) {
        print block_raw "\n"
        seen[key] = 1
    }
    block_raw = ""
    block_key = ""
}

##############################################################################
# Canonicalise a single *already prefix-stripped* Valgrind line for the key.
##############################################################################
function canon(line,   t) {
    t = line
    gsub(/0x[0-9a-fA-F]+/, "0xADDR", t)   # mask addresses
    gsub(/:[0-9]+/, ":LINE", t)           # <<< tolerate differing line nos
    gsub(/[[:space:]]+/, " ", t)          # compress whitespace
    sub(/[[:space:]]+$/, "", t)           # trim trailing space
    return t
}

##############################################################################
BEGIN {
    # Regex that marks the *first* line of every interesting block.
    start_pat = "(Invalid (read|write)|Syscall param|Use of uninitialised|" \
                "Conditional jump|bytes in [0-9]+ blocks|still reachable|"  \
                "possibly lost|definitely lost|Process terminating)"
}

##############################################################################
# Process Valgrind lines
##############################################################################
/^==[0-9]+==/{
    # Strip the “==PID== ” prefix
    line = gensub(/^==[0-9]+==[[:space:]]*/, "", 1)

    # New block? Flush previous.
    if (line ~ start_pat) {
        flush_block()
        # Skip leak headline counting bytes/blocks (noise)
        if (line ~ /[0-9]+ bytes in [0-9]+ blocks/) next
    }

    # Mask memory addresses so ASLR doesn’t create spurious differences
    gsub(/0x[0-9a-fA-F]+/, "", line)
    gsub(/at : +/, "", line)
    gsub(/by : +/, "", line)

    # Append raw + canonicalised
    block_raw = block_raw line "\n"
    block_key = block_key canon(line) "\n"
    next
}

##############################################################################
# End of file – flush anything still waiting
##############################################################################
END { flush_block() }

' "$file"
