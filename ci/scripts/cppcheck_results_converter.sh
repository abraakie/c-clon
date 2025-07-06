#!/bin/bash

set -euo pipefail

for cmd in xmlstarlet; do
  if ! command -v "$cmd" &>/dev/null; then
    echo "Error: '$cmd' not found."
    exit 1
  fi
done

input_file="${1:-cppcheck-result.xml}"
output_file="${2:-gl-code-quality-report.json}"

if [ ! -f "$input_file" ]; then
  echo "Error: Input file '$input_file' not found."
  exit 1
fi

echo "[" > "$output_file"
first=true

xmlstarlet sel -T -t \
  -m "//error" \
  -v "@msg" -o $'\t' \
  -v "@id"  -o $'\t' \
  -v "@severity" -o $'\t' \
  -v "location[1]/@file" -o $'\t' \
  -v "location[1]/@line" -n \
  "$input_file" \
| while IFS=$'\t' read -r msg id sev path line; do
    msg_escaped=$(printf '%s' "$msg" | sed 's/"/\\"/g')
    case "$sev" in
      error)     sev_out="critical" ;;
      warning)   sev_out="major"    ;;
      performance|portability|style) sev_out="minor" ;;
      information) sev_out="info"   ;;
      *)         sev_out="major"    ;;
    esac
    raw_fp="${id}|${path}|${line}"
    fp=$(echo -n "$raw_fp" | md5sum | cut -d' ' -f1)

    if $first; then
      first=false
    else
      echo "," >> "$output_file"
    fi

    cat <<EOF >> "$output_file"
  {
    "description": "${msg_escaped}",
    "fingerprint": "${fp}",
    "check_name": "${id}",
    "severity": "${sev_out}",
    "location": {
      "path": "${path}",
      "lines": { "begin": ${line} }
    }
  }
EOF
  done

echo "]" >> "$output_file"
echo "Translated cppcheck results to GitLab readable code quality format: $output_file"
