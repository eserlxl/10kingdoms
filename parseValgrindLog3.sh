#!/usr/bin/env bash
# extract_unique_valgrind.sh — v4   “template-aware” fuzzy dedupe
#
#  * Addresses  → 0xADDR
#  * Source line → :LINE    (in the key only)
#  * Template arguments    <...>  → <T>
#  * Array extents         [123] → []
#  * Signature depth       = 10 stack frames   (configurable)
#
# First raw occurrence of each signature is printed; later ones suppressed.

SIG_DEPTH=1        # compare only the first N frames (0 = unlimited)

file=$1
[[ -z $file || ! -f $file ]] && {
    echo "Usage: $0 <valgrind_report.txt>" >&2
    exit 1
}

gawk -v depth="$SIG_DEPTH" '
##############################################################################
# Emit current block if its signature is new
##############################################################################
function flush_block(  key, i, n) {
    if (raw == "") return

    # limit signature to first N frames if requested
    if (depth > 0) {
        n = split(sig, tmp, "\n")
        key = ""
        for (i = 1; i <= n && i <= depth; ++i) key = key tmp[i] "\n"
    } else key = sig

    if (!(key in seen)) {
        print raw
        seen[key] = 1
    }
    raw = sig = ""        # reset
}

##############################################################################
# Canonicalise one already-stripped stack-line for signature
##############################################################################
function canon(l) {
    gsub(/0x[0-9a-fA-F]+/, "0xADDR", l)    # mask addresses
    gsub(/:[0-9]+/, ":LINE",        l)     # mask source line
    gsub(/\[[0-9]+\]/, "[]",        l)     # array sizes → []
    gsub(/<[^>]*>/, "<T>",          l)     # template args → <T>
    gsub(/[[:space:]]+/, " ",       l)     # fold whitespace
    sub(/[[:space:]]+$/, "",        l)
    return l
}

BEGIN {
    start_pat = "(Invalid (read|write)|Syscall param|Use of uninitialised|" \
                "Conditional jump|bytes in [0-9]+ blocks|still reachable|"  \
                "possibly lost|definitely lost|Process terminating)"
}

##############################################################################
# Main loop
##############################################################################
/^==[0-9]+==/{
    line = gensub(/^==[0-9]+==[[:space:]]*/, "", 1)

    if (line ~ start_pat) {
        flush_block()
        if (line ~ /[0-9]+ bytes in [0-9]+ blocks/) next   # ignore headline
    }

    # Mask memory addresses so ASLR doesn’t create spurious differences
    gsub(/0x[0-9a-fA-F]+/, "", line)
    gsub(/at : +/, "", line)
    gsub(/by : +/, "", line)

    raw = raw line "\n"
    sig = sig canon(line) "\n"
}

END { flush_block() }

' "$file"
