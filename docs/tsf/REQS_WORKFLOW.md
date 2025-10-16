# Requirements management workflow (Doorstop + trudag)

This document defines a practical workflow for authoring, reviewing, approving and baselining requirements using Doorstop + trudag as the recommended toolchain. It is written to be executable and CI-friendly.

Goals
- Provide a repeatable authoring and review workflow.
- Ensure traceability from user → system → software → tests.
- Automate checks (linting, basic traceability) in CI.
- Record reviewer provenance (commit SHA) into `reviewed:` when requirements are approved.

Repository layout
- `reqs/`
  - `urd/` — user requirements
  - `srd/` — system requirements
  - `swd/` — software requirements
  - `lltc/` — low-level technical components / test cases
  - `templates/` — requirement templates used by `scripts/new_req.py`
- `scripts/` — helper scripts (`reqs_lint.py`, `new_req.py`)
- `docs/tsf/` — training, templates, workflow documents

Tooling choices
- Doorstop as the simple, file-backed requirements backend (supported by trudag).
- trudag for producing Trustable graphs and reports and managing `reviewed` provenance.
- Keep the linter (`scripts/reqs_lint.py`) as a pre-commit or CI gate.

Authoring workflow (recommended)
1. Create a branch for your work: `git checkout -b feat/REQ-<ID>-shortdesc`.
2. Bootstrap a new requirement:
   - `python3 scripts/new_req.py swd SWD-123` (or use your editor to copy template)
3. Edit the YAML: fill `header`, `text`, `links` and set `reviewers:` to the reviewer name(s).
4. Run the linter locally:

```bash
python3 scripts/reqs_lint.py reqs/swd --verbose
```

5. Push your branch and open a PR for review.

Review and approval
- Reviewer checks the PR: content, traceability, and tests mapping.
- When ready, reviewer updates the YAML `reviewed:` field with the approving commit SHA, or uses trudag to set the approval:

  - Manual (simple): set `reviewed: <commit-sha>` in the YAML and push the change.
  - With trudag (recommended if configured):

```bash
# inside a Python environment where trudag is available
trudag --door manage set-item reqs/swd/SWD-123.yml
```

- The `trudag` command may update metadata (including `reviewed`) depending on the Doorstop adapter and configuration. Verify the file change is in the PR.

Baselining and release
- After PR merge, create a baseline commit and tag the baseline in Git:

```bash
# merge PR into main, then create a baseline tag
git tag -a v1.0.0-baseline -m "Baseline for release 1.0.0"
git push origin main --tags
```

- Produce a Trustable Report with trudag and attach it to the release bundle (not checked into source control):

```bash
trudag --door report export --output artifacts/DrivaPi-trustable-report-1.0.0.zip
```

CI integration suggestions
- Add a pipeline stage to run `python3 scripts/reqs_lint.py reqs` and fail on non-zero exit.
- Optionally run a traceability checker that verifies every user requirement has a downstream mapping to system/software/test artifacts (simple script or ad-hoc check).
- As part of release job, run trudag to produce a Trustable Report and store it as a build artifact.

Recording evidence and provenance
- Encourage reviewers to include a short review note in PRs and link to evidence artifacts (test reports, analysis logs).
- For high-ASIL features, require additional evidence (static analysis reports, long-run tests) before marking `reviewed`.

Appendix: sample CI job (pseudo)

- job: LintReqs
  script:
    - python3 scripts/reqs_lint.py reqs

- job: ReleaseTrustable
  needs: [LintReqs]
  script:
    - trudag --door manage migrate
    - trudag --door report export --output artifacts/trustable-report-${CI_COMMIT_TAG}.zip
  artifacts:
    paths:
      - artifacts/trustable-report-${CI_COMMIT_TAG}.zip


***

This workflow is intended as a starting point. We can refine it to match your CI system, Doorstop configuration, and organizational approval policies.
