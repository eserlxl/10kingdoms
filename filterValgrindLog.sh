#!/usr/bin/env bash
set -euo pipefail

SIG_DEPTH=1
marker='Successfully downloaded debug'
trim_before_debug=true
scrub_raw=true        # will flip to false in verbose mode

usage() {
  cat >&2 <<EOF
Usage: $0 [options] <valgrind_log>

Options
  -k, --keep-debug-info   Keep everything; do not trim above last debug marker.
  -v, --verbose           Show completely raw blocks (no address / "at:" scrub).
  -d N, --depth N         Signature depth (default: $SIG_DEPTH, 0 = unlimited).
  -m S, --marker S        Marker string (default: "$marker").
  -h, --help              Show this help.
EOF
}

# ── CLI parsing ─────────────────────────────────────────────────────────────
while [[ $# -gt 0 ]]; do
  case $1 in
    -k|--keep-debug-info) trim_before_debug=false ;;
    -v|--verbose)         scrub_raw=false        ;;
    -d|--depth)           SIG_DEPTH=$2; shift    ;;
    -m|--marker)          marker=$2; shift       ;;
    -h|--help)            usage; exit 0          ;;
    --) shift; break ;;
    -*) echo "Unknown option: $1" >&2; usage; exit 1 ;;
     *) break ;;
  esac
  shift
done

file=${1:-}
[[ -z $file || ! -f $file ]] && { usage; exit 1; }

# ── trim (if requested) ─────────────────────────────────────────────────────
if $trim_before_debug; then
  last=$(grep -nF "$marker" -- "$file" | tail -n1 | cut -d: -f1 || true)
  if [[ -n $last ]]; then
    tail -n +"$(( last + 1 ))" -- "$file"
  else
    cat -- "$file"
  fi
else
  cat -- "$file"
fi |

# ── AWK engine ──────────────────────────────────────────────────────────────
gawk -v depth="$SIG_DEPTH" -v scrub="$scrub_raw" '
##############################################################################
function flush_block(  key,i,n) {
  if (raw == "") return
  if (depth>0) { n=split(sig,tmp,"\n"); key=""; for(i=1;i<=n&&i<=depth;i++) key=key tmp[i] "\n" }
  else key=sig
  if(!(key in seen)){ print raw; seen[key]=1 }
  raw=sig=""
}
function canon(l){
  gsub(/0x[0-9a-fA-F]+/,"0xADDR",l)
  gsub(/:[0-9]+/,":LINE",l)
  gsub(/\[[0-9]+\]/,"[]",l)
  gsub(/<[^>]*>/,"<T>",l)
  gsub(/[[:space:]]+/," ",l); sub(/[[:space:]]+$/,"",l)
  return l
}
BEGIN{
  start_pat="(Invalid (read|write)|Syscall param|Use of uninitialised|" \
            "Conditional jump|bytes in [0-9]+ blocks|still reachable|"  \
            "possibly lost|definitely lost|Process terminating)"
}
# ── main loop ───────────────────────────────────────────────────────────────
/^==[0-9]+==/{
  line=gensub(/^==[0-9]+==[[:space:]]*/,"",1)

  if(line ~ start_pat){
      flush_block()
      if(line ~ /[0-9]+ bytes in [0-9]+ blocks/) next
  }

  # ---------- Raw-block scrubbing (disabled when scrub==0) ----------
  if(scrub){
      gsub(/0x[0-9a-fA-F]+/,"", line)
      gsub(/at : +/,"", line)
      gsub(/by : +/,"", line)
      gsub(/???+/,"", line)
  }
  # ------------------------------------------------------------------

  if(length(line)){
      raw = raw line "\n"
      sig = sig canon(line) "\n"
  }
}
END{ flush_block() }
'
