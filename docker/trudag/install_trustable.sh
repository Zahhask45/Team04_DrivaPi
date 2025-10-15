#!/usr/bin/env bash
set -euo pipefail

# Clone trustable source and install as-is (no local modifications)
rm -rf /tmp/trustable
git clone https://gitlab.com/CodethinkLabs/trustable/trustable.git /tmp/trustable
pip install --no-cache-dir /tmp/trustable
