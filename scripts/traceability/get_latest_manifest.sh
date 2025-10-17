#!/usr/bin/env bash
set -euo pipefail

# Helper: fetch the latest traceability manifest from S3
# Usage: scripts/get_latest_traceability_manifest.sh <bucket> [prefix]

BUCKET="$1"
PREFIX="${2:-traceability}"

if [[ -z "$BUCKET" ]]; then
  echo "Usage: $0 <bucket> [prefix]"
  exit 2
fi

# list objects under prefix and pick the latest prefix (by lexicographic timestamp)
LATEST_PREFIX=$(aws s3 ls s3://${BUCKET}/${PREFIX}/ | awk '{print $2}' | sort -r | head -n1 | tr -d '/')
if [[ -z "$LATEST_PREFIX" ]]; then
  echo "No manifests found under s3://${BUCKET}/${PREFIX}/"
  exit 1
fi

MANIFEST_KEY="${PREFIX}/${LATEST_PREFIX}/traceability-manifest.json"
echo "Fetching s3://${BUCKET}/${MANIFEST_KEY}"
aws s3 cp s3://${BUCKET}/${MANIFEST_KEY} - || exit 1
