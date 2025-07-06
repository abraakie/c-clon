#!/bin/bash

set -euo pipefail

for cmd in md5sum; do
  if ! command -v "$cmd" &>/dev/null; then
    echo "Error: '$cmd' not found."
    exit 1
  fi
done

INPUT_FILE="${1:-codespell-output.txt}"
OUTPUT_FILE="${2:-codespell-report.json}"

if [ ! -f "$INPUT_FILE" ]; then
  echo "Error: Input file '$INPUT_FILE' not found."
  exit 1
fi

echo "[" > "$OUTPUT_FILE"
first=true

while IFS= read -r line; do
  [[ -z "$line" || "${line:0:1}" == "#" ]] && continue

  IFS=':' read -r path line_no rest <<< "$line"
  rest="${rest# }"
  word="${rest%% ==>*}"
  suggestions="${rest#*==> }"

  desc=$(printf 'Typo: \"%s\", Proposal: %s' \
         "$word" "$suggestions" | sed 's/"/\\"/g')

  # Fingerprint aus path:line:word
  fp_raw="${path}:${line_no}:${word}"
  fp=$(echo -n "$fp_raw" | md5sum | cut -d' ' -f1)

  if $first; then
    first=false
  else
    echo "," >> "$OUTPUT_FILE"
  fi

  cat <<EOF >> "$OUTPUT_FILE"
  {
    "description": "${desc}",
    "fingerprint": "${fp}",
    "check_name": "codespell",
    "severity": "minor",
    "location": {
      "path": "${path}",
      "lines": { "begin": ${line_no} }
    }
  }
EOF

done < "$INPUT_FILE"

echo "]" >> "$OUTPUT_FILE"
echo "Converted codespell results to Code Quality report: $OUTPUT_FILE"
