Traceability Guide

Purpose

This guide explains how to create and maintain a traceability matrix linking requirements to design, implementation, and tests.

Suggested matrix columns
- Req ref
- Req header
- Linked design artifact (diagram or file)
- Linked code/module/function
- Test case ID
- Verification status

Maintaining traceability
- Add links in YAML `links:` when creating requirements.
- Update the matrix when implementation or tests are added/changed.
- Perform periodic reviews to find coverage gaps.

Tools
- Start with a simple CSV or spreadsheet in the repo (`docs/traceability-matrix.csv`).
- For automation, consider tools that support bidirectional traceability (Trello/Jira integrations, DOORs, or custom scripts linking Git/GitHub PRs to requirement refs).

Review cadence
- Review matrix at each sprint boundary or major milestone.
- Ensure every requirement has at least one test case mapped.

Example

req_ref,header,design,module,test_case,status
URD-001,User login,auth-design.md,auth.login,TC-001,not-tested

How to cite sources

This guide draws on best practices for traceability and the Trustable methodology. See `docs/tsf/SOURCES.md` for references.
