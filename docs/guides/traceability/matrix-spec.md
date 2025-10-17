# Traceability matrix specification

This document describes the canonical traceability matrix outputs produced by
the repository tooling. The goal is to make a simple, machine- and human-
readable mapping between requirements, design artifacts and verification
evidence.

Files produced
- `artifacts/traceability/traceability-matrix-forward.csv` — forward mapping.
  Columns: `req_id,target_id,link_type,source_file`
  - `req_id`: requirement ID (top-level mapping key from YAML, e.g. `SWD-001`).
  - `target_id`: ID or artifact reference that the requirement links to. This
    may be another requirement ID (SRD, LLTC), or an artifact path (e.g.
    `artifacts/verification/tests/SWD-001-junit.xml`).
  - `link_type`: `ref` (logical ref), `satisfied-by`, `test`, or `artifact`.
  - `source_file`: path to the YAML that declares the link (relative to repo
    root).

- `artifacts/traceability/traceability-matrix-backward.csv` — backward mapping.
  Columns: `target_id,req_id,link_type,source_file` (reverse of forward).

- `artifacts/traceability/traceability-manifest.json` — high-level manifest
  containing `generated_at`, `counts` (files_scanned, links_extracted,
  broken, orphaned) and `paths` to the CSV and report files.

- `artifacts/traceability/gap-report.csv` — simple coverage report listing
  requirements that do not link to expected downstream artifacts. Columns:
  `req_id,category,missing_links`.

Schema and conventions
- Requirement YAMLs must contain a top-level mapping whose key is the
  requirement ID, and a `ref:` value that matches the ID.
- Use `links:` lists in requirement YAMLs to reference other requirements or
  artifacts. Artifacts in `artifacts/verification/` should be referenced by
  their repository path (e.g. `artifacts/verification/tests/SWD-001.xml`).
- Evidence detection uses `traceability_patterns.json` (list of substrings or
  suffixes) to identify whether a linked target looks like a test/report/coverage
  artifact.

Acceptance criteria for the matrix
- All current requirements produce at least one forward mapping line (even if
  the target is a placeholder).
- Backward mapping correctly reverses forward rows.
- Gap report lists uncovered requirements for triage.

Usage
- The matrix is produced by `scripts/generate_bidirectional_matrix.py` and
  published as CI artifacts. Consumers can load the JSON `paths` manifest or
  the CSVs for quick inspection.

Example forward CSV row
```
SWD-001,LLTC-001,ref,reqs/swd/SWD-001.yml
SWD-001,artifacts/verification/tests/SWD-001-junit.xml,artifact,reqs/swd/SWD-001.yml
```

Document created: 2025-10-17
