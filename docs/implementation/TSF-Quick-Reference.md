# TSF Quick Reference Card

**Version:** 1.0 | **Date:** October 17, 2025 | **Team:** DrivaPi (Team04)

---

## Requirements Hierarchy

```
URD (User Requirements) → defines WHAT users need
  ↓
SRD (System Requirements) → defines HOW system provides it
  ↓
SWD (Software Requirements) → defines HOW software implements it
  ↓
LLTC (Low-Level Test Cases) → defines HOW to verify it
```

---

## File Naming & Location

| Type | Location | Naming | Example |
|------|----------|--------|---------|
| User Req | `reqs/urd/` | `URD-XXX.yml` | `URD-001.yml` |
| System Req | `reqs/srd/` | `SRD-XXX.yml` | `SRD-001.yml` |
| Software Req | `reqs/swd/` | `SWD-XXX.yml` | `SWD-001.yml` |
| Test Case | `reqs/lltc/` | `LLTC-XXX.yml` | `LLTC-001.yml` |
| Template | `reqs/templates/` | `XXX-000.yml` | `URD-000.yml` |

---

## Required YAML Fields

```yaml
ref: URD-XXX                    # Unique ID (MUST match filename)
header: Short Title             # Brief, descriptive title
text: |                         # The actual requirement
  The system shall...           # Use "shall" for requirements
rationale: Why this matters     # Business/safety justification
links: [PARENT-001]            # Parent requirement(s) for traceability
reviewers: [alice, bob]        # Who will review this
reviewed: ''                    # Empty until approved, then: <git-sha>
```

---

## Common Commands

### Validate Requirements
```bash
# Check format & consistency
python scripts/reqs_lint.py

# Check Doorstop structure
doorstop

# Check traceability
python scripts/traceability_check.py
```

### Create New Requirement
```bash
# 1. Copy template
cp reqs/templates/URD-000.yml reqs/urd/URD-XXX.yml

# 2. Edit fields (ref, header, text, links, reviewers)
nano reqs/urd/URD-XXX.yml

# 3. Validate
python scripts/reqs_lint.py

# 4. Commit
git add reqs/urd/URD-XXX.yml
git commit -m "feat: Add URD-XXX - [Short Description]"
```

### Review & Approve Requirement
```bash
# 1. Review the requirement file
# 2. If approved, get current commit SHA
git log -1 --format=%H

# 3. Edit requirement: set reviewed: <sha>
# 4. Commit
git add reqs/urd/URD-XXX.yml
git commit -m "review: Approve URD-XXX - [Description]"
```

### Create Baseline
```bash
# Generate timestamped baseline archive
./scripts/baseline.sh SWD v1.1

# Output: artifacts/DrivaPi-requirements-BASELINE-SWD-v1.1-TIMESTAMP.tar.gz
```

---

## Traceability Matrix Files

| File | Purpose |
|------|---------|
| `artifacts/traceability-matrix.csv` | Consolidated view of all links |
| `artifacts/traceability-matrix-forward.csv` | Requirement → Targets |
| `artifacts/traceability-matrix-backward.csv` | Target → Requirements |
| `artifacts/traceability/gap-report.csv` | Missing links (coverage gaps) |
| `artifacts/traceability-report.md` | Human-readable summary |

---

## Writing Good Requirements

### ✅ DO:
- Use "shall" for mandatory requirements
- Make it testable (how will you verify?)
- Make it unambiguous (one interpretation only)
- Include acceptance criteria
- Link to parent requirements

### ❌ DON'T:
- Use vague words: "should", "might", "approximately"
- Combine multiple requirements in one
- Include implementation details (unless SWD)
- Leave `links: []` empty (unless URD)
- Forget to specify units/ranges

### Example: Good vs Bad

❌ **Bad:** "The system should display speed nicely"
- Vague: "nicely" is subjective
- Not testable: how do you verify "nicely"?
- Missing units

✅ **Good:** "The system shall display vehicle speed in km/h with ±1 km/h accuracy, updated every 100ms"
- Clear, testable, specific
- Includes units, accuracy, timing

---

## ASIL Levels (ISO 26262)

| ASIL | Risk Level | Examples |
|------|-----------|----------|
| QM | Quality Managed (no safety) | Infotainment, radio |
| A | Lowest safety-critical | Rear lights |
| B | Low safety-critical | Brake lights |
| C | Medium safety-critical | ABS, ESC |
| D | Highest safety-critical | Airbags, steering |

**V&V Requirements by ASIL:**
- **ASIL A:** Basic testing, code review
- **ASIL B:** + Integration tests, static analysis
- **ASIL C:** + Formal inspections, MC/DC coverage
- **ASIL D:** + Formal methods, exhaustive testing

---

## CI/CD Pipeline

**On every push/PR:**
1. `reqs_lint.py` → Validates YAML format
2. `doorstop` → Checks structure consistency
3. `traceability_check.py` → Generates matrices
4. Gap report → Identifies missing links

**Check CI results:**
- GitHub Actions tab
- `.github/workflows/reqs-checks.yml`

---

## Artifact Linking

Requirements must link to:
- **Design documents** (architecture, diagrams)
- **Source code** (modules, functions)
- **Test specifications** (detailed test cases)

```yaml
artifact:
  - type: design
    path: docs/architecture/battery-monitoring.md
  - type: code
    path: src/battery_voltage_reader.cpp
  - type: test
    path: tests/test_battery_voltage.py
```

---

## Useful Doorstop Commands

```bash
doorstop                    # Validate all requirements
doorstop publish all        # Generate HTML documentation
doorstop create URD ./reqs/urd --parent SYS  # Create new document
doorstop add URD            # Add new requirement interactively
doorstop edit URD-001       # Edit requirement in editor
```

---

## trudag (TSF Tool)

```bash
# Run via Docker
docker run --rm -v $(pwd):/workspace trudag:latest <command>

# Common commands
trudag validate             # Validate TSF graph
trudag report               # Generate trustability report
trudag confidence           # Calculate confidence scores
```

---

## Review Checklist

Before approving a requirement:
- [ ] `ref` matches filename?
- [ ] `header` is clear and concise?
- [ ] `text` uses "shall" and is testable?
- [ ] `rationale` explains the "why"?
- [ ] `links` point to valid parent(s)?
- [ ] `reviewers` includes you?
- [ ] No linter errors?
- [ ] Traceability intact?

---

## Git Commit Conventions

```bash
feat: Add URD-XXX - [description]        # New requirement
fix: Correct SWD-XXX typo                # Fix existing requirement
review: Approve URD-XXX                  # Requirement approval
baseline: Create SWD-v1.1                # Baseline creation
docs: Update requirements guide          # Documentation
refactor: Reorganize SRD structure       # Restructure requirements
```

---

## Emergency Contacts

| Issue | Contact | Command/Doc |
|-------|---------|-------------|
| Linter errors | Check `scripts/reqs_lint.py` | `python scripts/reqs_lint.py --help` |
| Broken links | Check `gap-report.csv` | `python scripts/traceability_check.py` |
| Doorstop issues | See Doorstop docs | `doorstop --help` |
| TSF questions | Review training | `docs/tsf/TSF-training.md` |
| Review process | See checklist | `docs/requirements/REVIEWER_CHECKLIST.md` |

---

## Key Files Reference

| Purpose | File Path |
|---------|-----------|
| Training Materials | `docs/tsf/TSF-training.md` |
| Requirement Template | `docs/tsf/TSF-requirement-template.md` |
| Review Checklist | `docs/requirements/REVIEWER_CHECKLIST.md` |
| V&V Plan | `docs/tsf/VV-plan.md` |
| CI Workflow | `.github/workflows/reqs-checks.yml` |
| Traceability Script | `scripts/traceability_check.py` |
| Linter Script | `scripts/reqs_lint.py` |

---

**Keep this card handy!** Print or save to your desktop for quick reference.

**Questions?** Check `docs/tsf/TSF-training.md` or ask the team lead.