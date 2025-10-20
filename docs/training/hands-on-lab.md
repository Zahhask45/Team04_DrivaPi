## TSF Hands-on Lab

Purpose: a short, repeatable lab that teaches requirement authors and reviewers how to create a requirement, add acceptance criteria, link verification evidence, run the repository checks (linter and traceability), fix issues, and create a baseline.

Audience: new team members and reviewers. The lab maps directly to the assessment rubric and CI checks used by this repository.

Duration: 30-60 minutes

Prerequisites
- A working checkout of this repository on your machine.
- Python 3 installed (python3 on PATH).
- A terminal with the repo root as the current directory.

Lab steps

1) Create a new requirement from a template

 - Copy an existing template or use the repository templates as a starting point. For example:

   - Create `reqs/swd/SWD-NEW-EXERCISE.yml` and use `reqs/templates/SWD-template.yml` as a guide.

 - Minimal YAML structure the linter expects (example):

   SWD-XXX:
     header: "Sensor input validation"
     text: "The system shall reject invalid sensor data as defined by schema v1."
     ref: SWD-XXX
     reviewers:
       - name: Jane Doe <jane@example.com>
     reviewed: ""
     links:
       - TEST-001
     acceptance:
       - GIVEN: a malformed sensor payload
         WHEN: the payload is submitted
         THEN: the system rejects it and logs the error code 400

2) Run the official validation steps

 - Run the official tools in the repository root. If you prefer a reproducible environment, use the Docker runner described in `docs/guides/requirements/trudag-usage.md`.

 ```bash
 # Validate structure and content with official tools
 trudag manage lint
 doorstop
 ```

 - Expected outcome:
  ## TSF Hands-on Lab — Authoring, Linting & Traceability

  Purpose
  - Hands-on, repeatable lab for requirement authors and reviewers. Participants will: author a requirement from a template, add acceptance criteria, link verification evidence, run the repo checks (linter and traceability), fix issues, and baseline the requirement.

  Audience and duration
  - Audience: engineers and reviewers who will author or review requirements.
  - Duration: ~30–60 minutes.

  Objectives
  - Author a valid requirement using provided templates or Doorstop interactive mode.
  - Run and fix issues reported by the official tools (`trudag manage lint` and `doorstop`).
  - Create a traceability mapping and verify it using the official `trudag report export` (CI produces a traceability CSV/manifest under `artifacts/`).
   - Author a valid requirement using Doorstop interactive mode or `reqs/templates/`.
   - Run and fix issues reported by `trudag manage lint` and `doorstop`.
   - Produce traceability reports using `trudag report export` and Doorstop/trudag commands.
  - Mark the requirement `reviewed:` and create a baseline commit/PR.

  Prerequisites
  - Repository checkout.
  - Python 3 (python3 on PATH).
  - git configured locally.
  - (Optional) AWS CLI if you plan to test S3 artifact retention.

  Quick workflow (commands)

  1) Create a branch

  ```bash
  git checkout -b lab/req-authoring-<yourname>
  ```

   2) Bootstrap a requirement

   Option A — use Doorstop interactive mode (preferred):

   ```bash
   doorstop add SWD
   doorstop edit SWD-999
   ```

   Option B — copy template and edit (manual):

   ```
   cp reqs/templates/SWD-template.yml reqs/swd/SWD-999.yml
   ```

  Minimal YAML example (what the linter expects):

  ```yaml
  SWD-999:
    header: "Sensor input validation"
    text: "The system shall reject invalid sensor data as defined by schema v1."
    ref: SWD-999
    reviewers:
      - name: Jane Doe <jane@example.com>
    reviewed: ""
    links:
      - LLTC-999
    acceptance:
      - GIVEN: a malformed sensor payload
        WHEN: the payload is submitted
        THEN: the system rejects it and logs a 400 error
  ```

   3) Run the official checks (skip template noise by using options on trudag if available)

   ```bash
   trudag manage lint
   # doorstop to check Doorstop layout
   doorstop
   ```

   - Expected: no errors from `trudag manage lint` for a clean file set.
  - If issues appear, fix the YAML (top-level ID, `ref:`, `reviewers:` block, acceptance presence) and re-run.

  4) Create a minimal linked LLTC placeholder (traceability demo)

  ```bash
  cp reqs/templates/LLTC-template.yml reqs/lltc/LLTC-999.yml
  # edit header/text/ref to LLTC-999 and commit
  ```

   5) Produce traceability and Trustable reports using official tooling

   ```bash
   # Update Doorstop structure for trudag, then use trudag to export reports
   trudag manage migrate
   trudag report export --output artifacts/trustable-report.zip
   ```

   - Expected artifacts: `artifacts/` contains Trustable report exports and any CI-collected traceability outputs.
  - If the checker reports orphaned requirements or broken links, inspect the source YAMLs and `links:` entries.

  6) Mark reviewed and baseline

  - After verifying locally, set the `reviewed:` field to the commit SHA that you validated:

  ```bash
  git rev-parse --verify HEAD
  # set that SHA in the YAML under reviewed:
  ```

  - Commit and push your changes, then open a PR. CI will run the linter and traceability checks and publish artifacts.

  7) Cleanup (optional)

  - Remove lab-specific reqs or move them to `reqs/examples/` if you want to keep them.

  Grading and pass criteria (maps to the TSF Assessment Rubric)
  - Lab tasks (each pass=1):
    - Requirement authored with `ref`, `header`, `text`.
    - Linter runs clean for authored files.
    - Traceability mapping appears in the generated matrix/report.
    - `reviewed:` set to a valid git SHA.
    - PR created and CI artifacts generated.
  - Lab passing threshold: 4/5.

  Troubleshooting notes
  - Template warnings: templates intentionally contain placeholder IDs. Use `--ignore-templates` during lab runs.
  - Evidence detection: the checker uses `traceability_patterns.json` to match artifact names/types. If your artifacts use different naming, add patterns to that JSON and re-run the checker.
  - CI failures: inspect the CI job artifacts (traceability CSVs and report) in the workflow run to debug link or evidence issues.

  Instructor notes
  - The instructor should verify YAML correctness, the traceability report, and that the `reviewed:` field contains a valid commit SHA.
  - Record lab results manually in `docs/tsf/training-results.md` (or ask me to add a small helper to record results automatically).

  Document created/updated: 2025-10-17
- Update `reviewed:` in your SWD YAML with the git commit SHA (e.g., `git rev-parse HEAD`) after your PR or after commit.

