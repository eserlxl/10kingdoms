#!/usr/bin/env bash
# parseValgrindLog.sh — v4   “template-aware” fuzzy dedupe + optional trim)
#
#  * Addresses  → 0xADDR
#  * Source line → :LINE    (in the key only)
#  * Template arguments    <...>  → <T>
#  * Array extents         [123] → []
#  * Signature depth       = 10 stack frames   (configurable)
#
# First raw occurrence of each signature is printed; later ones suppressed.
#
# Example use
#   ./parseValgrindLog.sh valgrind.log                    > uniq.txt
#   ./parseValgrindLog.sh -k valgrind.log                 > uniq.txt
#   ./parseValgrindLog.sh --keep-debug-info valgrind.log | less
#
# ---------------------------------------------------------------------------

SIG_DEPTH=1           # Signature depth (as in your v4)

# ── argument parsing ────────────────────────────────────────────────────────
trim_before_debug=true
case "$1" in
  -k|--keep-debug-info) trim_before_debug=false; shift ;;
esac

file=$1
if [[ -z "$file" || ! -f "$file" ]]; then
    echo "Usage: $0 [--keep-debug-info] <valgrind_report.txt>" >&2
    exit 1
fi

# ── decide which part of the file to feed into gawk ─────────────────────────
# If trimming is requested, find the last occurrence of the marker line and
# pipe only what follows it.  Otherwise cat the whole file.
if $trim_before_debug; then
    # Number (1-based) of the last matching line, or empty string
    last=$(grep -nF "Successfully downloaded debug" "$file" | tail -n1 | cut -d: -f1)
    if [[ -n "$last" ]]; then
        # tail: start from line last+1 (skip the marker itself as well)
        tail -n +"$((last+1))" -- "$file"
    else
        cat -- "$file"
    fi
else
    cat -- "$file"
fi |

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
    gsub(/???+/, "", line)

    if(length(line)>0) {
        raw = raw line "\n"
        sig = sig canon(line) "\n"
    }
}

END { flush_block() }

'
