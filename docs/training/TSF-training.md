# Trustable Software Framework (TSF) — Training and Quick Reference

This document is a practical training pack to get the team up to speed with the Trustable Software Framework (TSF) and key automotive software standards (ISO 26262, ASPICE). It contains conceptual explanations, recommended reading, and hands-on exercises.

## Learning goals
- Understand TSF principles and how they apply to software requirements and releases.
- Know the basics of automotive software standards: ISO 26262 and ASPICE.
- Explain ASIL levels and basic hazard analysis concepts.
- Write structured, traceable requirements using the repo templates.
- Understand traceability, verification & validation strategies and documentation expectations.

---

## 1. TSF fundamentals (summary)
- TSF models requirements as a directed acyclic graph of Statements linked by logical implication.
- Statements are either Requests (parents) or Claims (children); Expectations, Assertions, and Premises are derived categories.
- Evidence should be captured as Artifacts (tests, reports, source code). Evidence is used to validate Statements.
- Changes to Statements or Links mark them as 'Suspect' until reviewed; reviewed items are considered cleared.

### Practical takeaway
- Name every requirement with a stable ID and a `ref:` that matches the filename.
- Record `reviewers:` and the `reviewed` marker once SME review is complete.
- Keep artifacts (test results, design notes, builds) linked in the requirement's `links:` field.

---

## 2. Automotive standards primer
### ISO 26262 (very short)
- Functional safety standard for road vehicles.
- Key concepts: safety lifecycle, hazards, safety goals, ASIL (A–D) determination, and requirements decomposition.
- ASIL D = highest integrity; ASIL A = lowest integrity in the automotive ASIL scale.

### ASPICE (brief)
- Process assessment model focusing on engineering processes; useful to align team workflows and evidence collection.

### Practical takeaway
- Use ASIL as a guide for verification effort and evidence rigor.
- Capture hazard analysis and safety goals in project artifacts; link them to requirements.

---

## TSF: key information areas (practical guidance)

When applying TSF to a project we collect and present evidence across the following dimensions. For each, record artifacts (documents, logs, test-results, build metadata) and link them from the relevant Statements in the TSF graph.

- Provenance
	- What produced the component (author, organization), licensing and supply-chain claims.
	- Practical: capture repository URL, commit SHA, author, licensing, and contributor provenance in artifacts.

- Construction
	- How to build, install and run the component; include build instructions, pinned dependencies and build artifacts.
	- Practical: store reproducible build commands, Dockerfiles, and a checksum of the produced deliverable as artifacts.

- Change
	- How updates are managed and what regression evidence exists for new versions.
	- Practical: link release notes, CI pipelines, regression test results and a changelog to each release Statement.

- Expectations
	- What the software is expected to do (functional and non-functional expectations).
	- Practical: express expectations as Statements and link tests that validate them.

- Results
	- Measured behaviour and test outcomes; attach test logs and metrics as Artifacts.
	- Practical: include test harness outputs, performance graphs and incident logs.

- Confidence
	- An aggregated judgement based on the above evidence; score or qualitative judgement.
	- Practical: capture SME review results and automated validation outputs; track a per-Assertion confidence score if desired.

---

## Using the `-trustable` suffix and Trustable report guidance

- A release may be marked with the `-trustable` suffix when it meets these conditions:
	1) The Trustable methodology has been applied to the claims made for the release.
	2) A Trustable Report (summary of evidence and confidence) is produced and included with the release (not checked into source control, but provided with the release bundle).

- Important: `-trustable` does not mean "perfect" or "fully safe" — it means sufficient evidence and tooling have been provided to allow consumers to make informed decisions.

## Reference implementation notes (tooling + process)

- We recommend using trudag + a Doorstop backend to author and manage TSF/requirements as a graph in the repository. The repo should contain:
	- `reqs/` structured into categories (urd/, srd/, swd/, etc.)
	- A reproducible baseline process (scripted via CI) that: runs linters, migrates/trudag manage migrate, and exports a Trustable report artifact.

- Capture review evidence by writing the reviewer name and the commit SHA into `reviewed:` for each requirement when the SME clears the Statement. Automate this as part of the approval workflow if you have a supported toolchain.

---

## Practical checklist for a Trustable release (short)

1. All critical Statements have at least one piece of evidence attached (artifact or validation).
2. All Statements that changed since last release are marked as Suspect until reviewed.
3. Traceability matrix links requirements → design → code → tests and is updated.
4. A Trustable Report is produced (automated + SME summary) and packaged with the release.
5. Tag the release using the agreed convention; append `-trustable` if criteria met.


## 3. Requirements engineering essentials
- Requirements should be single-sentence, testable, unambiguous and traceable.
- Hierarchy: User requirements → System requirements → Software requirements.
- Use templates and linters to enforce structure; maintain review logs.

---

## 4. Traceability concepts
- Bidirectional traceability ensures every requirement maps to design artifacts and tests, and vice-versa.
- Keep a traceability matrix (CSV/Markdown/YAML or tool-backed) that links IDs across artifacts.
- Regularly review the matrix to find coverage gaps.

---

## 5. Verification & validation strategies
- Unit tests, integration tests, system tests, acceptance tests—map test cases to requirements.
- For ASIL-graded features, increase evidence depth: code reviews, static analysis, fault-injection, long-duration soak tests, and formal checks when available.

---

## 6. Documentation & artifacts
- Keep architecture docs, test reports, toolchain instructions and binary release artifacts together with requirements (or link to them).
- Prefer reproducible builds and pinned dependencies.

---

## 7. Hands-on exercises (recommended)
1. Validate requirements using the official TSF tools (run `trudag manage lint` and `doorstop`), and fix issues in one sample file.
2. Create a sample traceability matrix linking one user requirement to a system and software requirement and a simple test case.
3. Run a small integration test and attach the test log as an artifact in a requirement `links:` field.

---

## Quick reference (one page)
- File naming: `reqs/<category>/<ID>.yml` where `<ID>` is like `SWD-001`.
- Required fields: top-level ID mapping, `header:`, `text:`, `ref: <ID>`, `reviewers: - name: '...'`, `reviewed: <git-sha | null>`.
- Use the official linter: `trudag manage lint` (and run `doorstop` for Doorstop checks).
- For baselining: use the approval process defined in the project (record the reviewer and commit SHA into `reviewed`).

---

## Further reading
- TSF reference docs in `docs/tsf/`.
- ISO 26262 overview and ASPICE introduction (links kept in `docs/tsf/SOURCES.md`).

---

## Assessment
- Deliver a short quiz and a small lab: author one requirement, link it to a test case and mark it reviewed. Instructor verifies completeness.

*** end of training starter pack ***
