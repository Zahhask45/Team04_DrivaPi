TSF Quick Reference

A short cheat-sheet for the team. Use this for day-to-day work with requirements and traceability.

Terminology

- URD: User Requirements Document (high-level user needs)
- SRD: System Requirements Document (system-level requirements)
- SWD: Software Requirements Document (software-level requirements)
- LLTC: Low-Level Technical Components / testable lower-level requirements
- ASIL: Automotive Safety Integrity Level (A, B, C, D) — D is highest

Minimal requirement YAML fields (example)

```yaml
SWD-001:
  header: "Short title"
  text: |
    Full description...
  level: 1.0
  active: true
  derived: false
  normative: true
  links: []
  ref: "SWD-001"
  reviewers:
    - name: "Reviewer Name"
      email: "reviewer@example.com"
  reviewed: ""  # filled by trudag upon approval
```

See [TSF requirement template](TSF-requirement-template.md) for a field-by-field explanation, examples and validation tips.

Approval flow (manual)

- Edit the YAML and add a `reviewers` entry.
- Run `trudag --door manage set-item reqs/swd/SWD-001.yml` to set the `reviewed` provenance field (see trudag docs).
- Commit the updated YAML and tag the baseline release.

Traceability tips

- Always set `ref` to match the top-level key and file name.
- Use `links` to reference upstream/downstream requirement refs (e.g., `SRD-001`).

Useful commands

- Validate YAMLs: `python3 scripts/reqs_lint.py reqs --verbose` (project linter)
- Migrate requirements (trudag): `trudag --door manage migrate` (see trudag docs)
- Approve / set review (trudag): `trudag --door manage set-item <path>` (see trudag docs)

How to cite sources

This cheat sheet is based on the Trustable methodology and common automotive guidance. See `docs/tsf/SOURCES.md` for the primary references (Trustable docs, ISO 26262, ASPICE).
