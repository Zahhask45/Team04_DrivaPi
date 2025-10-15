#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
TAG=trudag-runner:latest
docker build -f docker/trudag/Dockerfile -t "$TAG" .
docker run --rm -v "$PWD":/workspace "$TAG" --door manage lint
