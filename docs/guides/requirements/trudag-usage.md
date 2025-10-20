# Running trudag lint (venv and Docker)

This document explains how to run the `trudag` lint for the DrivaPi repository in two ways: using the workspace virtualenv, or using the included Docker container.

1) Using the workspace virtualenv

- Ensure you have a Python venv at `.venv` (the workspace setup tool may create it).
- Install dependencies into the venv (if not already):

```bash
# Activate your venv (example on macOS with zsh)
source .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install trustable doorstop pyyaml
```

 - If you're on Python 3.14, Doorstop may import `distutils`. Use an isolated Python 3.11 environment (virtualenv or Docker) to avoid compatibility issues.

 - Run the lint with the official tool:

 ```bash
 .venv/bin/trudag manage lint
 ```

2) Using Docker (recommended for reproducibility)

Build and run the container using the same image/configuration as CI (recommended for reproducibility).

Options:

- Use the CI image referenced in `.github/workflows/reqs-checks.yml` — this ensures the same Python version and pinned deps.
- Or build a small local image using this example Dockerfile (not checked into the repo):

```dockerfile
# FROM python:3.11-slim
# RUN pip install --no-cache-dir trustable doorstop
# WORKDIR /work
# ENTRYPOINT ["trudag"]
```

Then run the container with a bind mount to your local checkout and invoke `trudag manage lint` inside it. This avoids local environment issues and reproduces CI behavior.

 Notes
 - This repository follows an official-only tool policy: prefer `trudag` + `doorstop` for all requirement validation and reporting. Some convenience helpers existed previously but were removed from the main tree to avoid divergence; see `docs/ops/trudag-official.md` for archival/restore instructions if you need them.
 - The Dockerfile or CI may apply temporary compatibility shims; those are implementation details of the CI container and not part of the official workflow.
