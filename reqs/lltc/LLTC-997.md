---
id: LLTC-997
header: Static Analysis Verification (Cppcheck)
text:  |
  "The software SHALL pass static analysis (Cppcheck) with zero ''error'' severity
  findings."

# TSF Type: A Premise

verification_method: Static Analysis
parents:
- id: SWD-998
reviewers:
- name: QA Engineer
  email: qa@team.com
reviewed: ''  # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)
references:
- type: file
  path: artifacts/verification/static-analysis/cppcheck-SWD-998.xml
  description: Static analysis report (0 errors)

active: true
derived: false
normative: true
level: 4.0
---
Verifies SWD-998 implementation against static analysis rules.
