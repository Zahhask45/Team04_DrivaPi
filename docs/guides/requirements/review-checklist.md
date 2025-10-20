# Reviewer Checklist - DrivaPi Requirements

**Official TSF tools validation checklist**

**Version:** 2.0
**Date:** October 20, 2025

---

## 🎯 Purpose

This checklist describes the review steps for requirement items managed with Doorstop/trudag official tools.

---

## 1️⃣ Validate Format and Schema

```bash
# Official TSF validation
trudag manage lint

# Doorstop validation
doorstop
```

**Expected output:** No errors or warnings

---

## 2️⃣ Human Review Points

### Content Quality
- [ ] `text` field is clear and testable
- [ ] Uses "shall" for mandatory requirements
- [ ] No ambiguous words ("approximately", "quickly", "often")
- [ ] Units specified for quantities (ms, km/h, %, etc.)
- [ ] Ranges specified for values (±1%, 0-100°C)
- [ ] Timing constraints clear (within 100ms, at 10Hz)

### Technical Accuracy
- [ ] Requirement is technically feasible
- [ ] Performance targets are realistic
- [ ] Interface specifications are correct
- [ ] Safety considerations addressed (if ASIL > QM)

### TSF Compliance
- [ ] `ref:` matches filename and top-level key
- [ ] `header:` is concise (<80 chars)
- [ ] `ASIL` level is correct (A/B/C/D/QM)
- [ ] `Verification Method` is appropriate:
  - Unit Test
  - Integration Test
  - System Test
  - Review
  - Formal Analysis
- [ ] `reviewers:` has at least one entry with name and email
- [ ] `reviewed:` is empty (will be filled on approval)

### Traceability
- [ ] `links:` field correctly references parent(s)
- [ ] Parent requirements exist and are valid
- [ ] No circular dependencies
- [ ] Hierarchical relationship correct (URD→SRD→SWD→LLTC)

### Acceptance Criteria (if present)
- [ ] GIVEN/WHEN/THEN format used
- [ ] Criteria are testable
- [ ] Coverage is complete

---

## 3️⃣ Approve Requirement

**When ready to approve:**

```bash
# Get current commit SHA
git rev-parse --verify HEAD

# Approve with trudag (updates 'reviewed' field)
trudag manage set-item reqs/<category>/<ID>.yml

# Verify the 'reviewed' field was updated
cat reqs/<category>/<ID>.yml | grep reviewed

# Commit approval
git add reqs/<category>/<ID>.yml
git commit -m "review: Approve <ID> - <short-description>"
git push
```

---

## 4️⃣ Create Baseline (When Set Complete)

**Create baseline tag when a set of requirements is approved:**

```bash
# Create annotated tag
git tag -a BASELINE-SWD-V1.0 -m "SWD baseline v1.0 - approved"

# Push tag
git push origin BASELINE-SWD-V1.0

# Export Trustable report
trudag report export --output artifacts/baselines/trustable-SWD-v1.0.zip
```

---

## 📋 Review Checklist Template

Use this template when reviewing:

```markdown
## Review: <REQ-ID>

**Reviewer:** <Your Name>
**Date:** YYYY-MM-DD
**Status:** [ ] APPROVED / [ ] CHANGES REQUESTED

### Format Validation
- [ ] `trudag manage lint` passes
- [ ] All mandatory fields present
- [ ] YAML syntax valid

### Content Quality
- [ ] Clear and unambiguous
- [ ] Testable
- [ ] No vague language
- [ ] Units/values/timing specified

### Technical
- [ ] Technically feasible
- [ ] ASIL appropriate
- [ ] Verification method suitable

### Traceability
- [ ] Links correct
- [ ] Parent exists
- [ ] No circular dependencies

### Comments
<Add specific feedback here>

### Recommendation
[ ] APPROVE - Ready to merge
[ ] REQUEST CHANGES - See comments above
```

---

## 🔧 ASIL-Specific Requirements

### ASIL A
- [ ] Peer review completed
- [ ] Unit tests exist (if applicable)

### ASIL B
- [ ] Integration tests included
- [ ] Design reviewer sign-off
- [ ] Static checks executed

### ASIL C
- [ ] Independent reviewer (non-author)
- [ ] System/integration test procedures documented
- [ ] Coverage/metric reports attached

### ASIL D
- [ ] Formal sign-off attached
- [ ] System/HIL test logs attached
- [ ] Baseline tag referenced in `reviewed:` field

---

## 📁 Verification Artifacts

Store evidence in:
- **Machine-readable:** `artifacts/verification/tests/`
- **Human-readable:** `artifacts/verification/reports/`

Reference from YAML:
```yaml
artifact:
  - type: test
    path: artifacts/verification/tests/SWD-001-junit.xml
    description: Unit test results
```

---

## ⚠️ Common Issues

### Issue: Missing Fields
**Problem:** Linter reports missing mandatory fields
**Solution:** Add all required fields per template

### Issue: Broken Links
**Problem:** Link to non-existent requirement
**Solution:** Verify parent exists or create it first

### Issue: Ambiguous Text
**Problem:** Requirement uses vague language
**Solution:** Rewrite with specific, measurable criteria

### Issue: Wrong ASIL
**Problem:** ASIL doesn't match system hazard analysis
**Solution:** Review hazard analysis and correct ASIL level

---

## 📖 References

- **Workflow:** `docs/guides/requirements/workflow.md`
- **Templates:** `reqs/templates/`
- **Training:** `docs/training/TSF-training.md`

---

**Notes:**
- If `trudag manage lint` raises errors due to tool versions, check compatibility
- Always use official TSF tools (trudag + doorstop)
- Never use custom Python scripts for validation

---

**Last Updated:** October 20, 2025
