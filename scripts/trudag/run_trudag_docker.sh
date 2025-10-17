#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
TAG=trudag-runner:latest
docker build -f docker/trudag/Dockerfile -t "$TAG" .
# Run the container: migrate the Doorstop backend, lint, then export a report
docker run --rm -v "$PWD":/workspace "$TAG" manage migrate || true
docker run --rm -v "$PWD":/workspace "$TAG" manage lint
# Export trustable report artifact (project name and artifact path may be adjusted)
mkdir -p artifacts
docker run --rm -v "$PWD":/workspace "$TAG" export --artifact artifacts/DrivaPi-trustable-report.zip -P "DrivePi" || true
