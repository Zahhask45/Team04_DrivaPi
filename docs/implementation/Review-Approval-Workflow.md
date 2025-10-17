# Requirement Review & Approval Workflow

**Purpose:** Step-by-step process for reviewing and approving requirements  
**Owner:** All team members with review responsibilities  
**Version:** 1.0

---

## Overview

This document provides a standardized workflow for reviewing and approving requirements in the DrivaPi project following TSF principles and ISO 26262 guidelines.

---

## Roles & Responsibilities

### Requirement Author
- Creates requirement following template
- Assigns reviewers
- Addresses review comments
- Commits approved requirement

### Reviewers (2 required per requirement)
- Technical expert (understands implementation)
- Domain expert (understands business/safety context)
- Complete review checklist
- Approve or request changes

### Approver (typically team lead or product owner)
- Final approval authority
- Sets `reviewed:` field with git SHA
- Commits approval

---

## Review Workflow

### Step 1: Author Creates Requirement

**Actions:**
1. Copy appropriate template from `reqs/templates/`
2. Fill in all mandatory fields
3. Assign 2 reviewers in `reviewers:` field
4. Ensure `reviewed: ''` (empty)
5. Run linter: `python scripts/reqs_lint.py`
6. Commit with message: `feat: Add [REQ-ID] - [description]`
7. Create pull request or notify reviewers

**Checklist for Author:**
- [ ] Correct file naming (`REQ-XXX.yml` matches `ref:`)
- [ ] All mandatory fields filled
- [ ] Text uses "shall" for requirements
- [ ] Requirement is testable
- [ ] Links to parent requirement(s) if applicable
- [ ] No linter errors
- [ ] Assigned 2 reviewers

---

### Step 2: Reviewers Evaluate Requirement

**Review Checklist:**

#### Format Validation
- [ ] `ref:` field matches filename
- [ ] `header:` is clear and concise (< 80 chars)
- [ ] `text:` field is complete and detailed
- [ ] All mandatory fields present
- [ ] YAML syntax valid (no linter errors)

#### Content Quality
- [ ] **Clear:** Unambiguous, single interpretation
- [ ] **Testable:** Can be verified with specific test
- [ ] **Complete:** All necessary information included
- [ ] **Consistent:** Aligns with other requirements
- [ ] **Traceable:** Links to parent/related requirements
- [ ] **Feasible:** Technically possible to implement

#### Text Quality
- [ ] Uses "shall" for mandatory requirements
- [ ] Uses "should" for recommendations (rare)
- [ ] Active voice, present tense
- [ ] No ambiguous words ("approximately", "quickly", "often")
- [ ] Units specified for quantities
- [ ] Ranges specified for values

#### Traceability
- [ ] `links:` field correctly references parent(s)
- [ ] Parent requirements exist and are valid
- [ ] No circular dependencies
- [ ] Hierarchical relationship correct (URD→SRD→SWD→LLTC)

#### Technical Accuracy
- [ ] Requirement is technically feasible
- [ ] Performance targets are realistic
- [ ] Interface specifications are correct
- [ ] Safety considerations addressed (if ASIL > QM)

#### Artifacts (if applicable)
- [ ] All artifact paths exist
- [ ] Artifact types are appropriate
- [ ] Artifact descriptions are clear
- [ ] Artifacts actually relate to requirement

---

### Step 3: Provide Review Feedback

**If Issues Found:**

Create a review comment file: `reviews/[REQ-ID]-review-[date].md`

```markdown
# Review: [REQ-ID]
**Reviewer:** [Your Name]  
**Date:** 2025-10-22  
**Status:** CHANGES REQUESTED

## Issues Found

### Issue 1: Ambiguous Wording
**Location:** `text:` field, line 3  
**Problem:** "quickly" is not quantifiable  
**Suggestion:** Replace with "within 100ms"

### Issue 2: Missing Parent Link
**Location:** `links:` field  
**Problem:** No link to parent SRD requirement  
**Suggestion:** Add `links: [SRD-003]`

## Recommendation
**REJECT** - Changes required before approval
```

**Notify author:**
- Add review comment to PR
- Or email/message author with review file
- Request changes

---

**If No Issues:**

Create approval comment: `reviews/[REQ-ID]-review-[date].md`

```markdown
# Review: [REQ-ID]
**Reviewer:** [Your Name]  
**Date:** 2025-10-22  
**Status:** APPROVED

## Review Summary
- Format: ✅ Valid
- Content: ✅ Clear and complete
- Traceability: ✅ Correct links
- Technical: ✅ Feasible and accurate
- Artifacts: ✅ Valid (if applicable)

## Comments
Requirement is well-written and testable. No issues found.

## Recommendation
**APPROVE**
```

---

### Step 4: Author Addresses Feedback

**If changes requested:**
1. Review all reviewer comments
2. Make necessary changes to requirement YAML
3. Re-run linter
4. Commit changes: `fix: Address review feedback for [REQ-ID]`
5. Notify reviewers of updates
6. Restart review process (Step 2)

**If both reviewers approve:**
Proceed to Step 5

---

### Step 5: Final Approval & Commit

**Performed by:** Team lead, product owner, or authorized approver

**Actions:**

1. **Verify both reviewers approved**
   ```bash
   # Check review files
   ls reviews/[REQ-ID]-review-*.md
   ```

2. **Get current git commit SHA**
   ```bash
   git log -1 --format=%H
   ```
   Example output: `a1b2c3d4e5f6789012345678901234567890abcd`

3. **Edit requirement YAML**
   ```bash
   nano reqs/[category]/[REQ-ID].yml
   ```
   
   Update `reviewed:` field:
   ```yaml
   reviewed: a1b2c3d4e5f6789012345678901234567890abcd
   ```

4. **Commit approval**
   ```bash
   git add reqs/[category]/[REQ-ID].yml
   git commit -m "review: Approve [REQ-ID] - [short description]"
   git push
   ```

5. **Update tracking**
   - Mark requirement as approved in tracking spreadsheet
   - Update project management tool (Jira, GitHub Projects)
   - Notify stakeholders

---

## Special Cases

### Urgent Requirements (Fast-Track)

For critical requirements needed immediately:
- Reduce reviewers to 1 (must be senior)
- Conduct synchronous review (meeting/call)
- Approve within same day
- Still follow all quality checks

**Note:** Fast-track should be rare exception, not the norm.

---

### Template Updates

When updating `reqs/templates/XXX-000.yml`:
- Requires review from team lead + 1 technical lead
- Document changes in commit message
- Announce to team (affects all future requirements)
- Update documentation if template structure changes

---

### Baseline Requirements

For requirements in an approved baseline:
- Changes require full re-review
- Must create new baseline after approval
- Document why change was necessary
- Assess impact on dependent requirements

---

### Cross-Category Requirements

If requirement affects multiple categories:
- Assign reviewers from each affected category
- Ensure consistency across related requirements
- Update all affected requirements simultaneously
- Check traceability matrix for impacts

---

## Batch Approval Process

For approving multiple requirements efficiently:

### Preparation
1. Collect all requirements pending approval
2. Group by category (URD, SRD, SWD, LLTC)
3. Assign review pairs

### Review Meeting
1. Schedule 2-hour review session
2. Review each requirement as a group
3. Use checklist for each requirement
4. Document decisions immediately

### Batch Commit
```bash
# Approve all reviewed requirements at once
for req in URD-001 URD-002 SRD-001 SRD-002; do
  SHA=$(git log -1 --format=%H)
  # Edit each requirement's reviewed field
  sed -i "s/reviewed: ''/reviewed: $SHA/" reqs/*/[$req].yml
done

# Commit all approvals
git add reqs/
git commit -m "review: Batch approve URD-001, URD-002, SRD-001, SRD-002"
git push
```

---

## Quality Gates

Requirements **cannot** be approved if:
- [ ] ❌ Linter reports errors
- [ ] ❌ Parent requirement doesn't exist
- [ ] ❌ Less than 2 reviewers approved
- [ ] ❌ Requirement is not testable
- [ ] ❌ Artifact paths don't exist (if artifacts specified)
- [ ] ❌ Violates project standards or policies
- [ ] ❌ Safety requirements without ASIL classification

---

## Automation Support

### Pre-commit Hooks

Add to `.git/hooks/pre-commit`:

```bash
#!/bin/bash
# Validate requirements before commit

echo "Running requirements validation..."

# 1. Check YAML syntax
python scripts/reqs_lint.py
if [ $? -ne 0 ]; then
  echo "❌ Linter errors found. Fix before committing."
  exit 1
fi

# 2. Check Doorstop structure
doorstop > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "❌ Doorstop validation failed."
  exit 1
fi

echo "✅ Validation passed"
exit 0
```

### CI/CD Checks

GitHub Actions (`.github/workflows/reqs-checks.yml`) automatically:
- Runs linter on all requirements
- Validates traceability links
- Checks artifact paths exist
- Generates updated traceability matrix
- Fails PR if validation errors

---

## Metrics & Tracking

Track the following metrics:

### Review Metrics
- Average time to review (target: < 24 hours)
- Approval rate (target: > 80% first submission)
- Number of review iterations (target: < 2)
- Review backlog size (target: < 5 requirements)

### Quality Metrics
- Requirements with issues found (track trends)
- Most common issues (improve training)
- Requirements requiring re-review
- Post-approval changes (should be rare)

### Process Metrics
- Total requirements approved per week
- Time from creation to approval
- Reviewer workload distribution
- Baseline frequency

---

## Tools & Commands Reference

### Validate Single Requirement
```bash
python scripts/reqs_lint.py reqs/urd/URD-001.yml
```

### Validate All Requirements
```bash
python scripts/reqs_lint.py
```

### Check Traceability for Requirement
```bash
doorstop review URD-001
```

### List Requirements Needing Approval
```bash
grep -r "reviewed: ''" reqs/*/
```

### Generate Traceability Report
```bash
python scripts/traceability_check.py
cat artifacts/traceability-report.md
```

### Check for Broken Links
```bash
doorstop
# Look for "broken link" warnings
```

---

## Example: Complete Review Session

**Scenario:** Reviewing URD-002 (Display Battery Voltage)

### 1. Reviewer 1 (Technical Expert) Review

```bash
# Open requirement
cat reqs/urd/URD-002.yml
```

```yaml
ref: URD-002
header: Display Battery Voltage
text: |
  The system shall display the current battery voltage on the main
  dashboard with accuracy of ±0.1V, updated every 100ms
rationale: |
  Users need to monitor battery health to prevent unexpected failures
  and optimize vehicle performance
links: []
reviewers: [tech-lead, product-owner]
reviewed: ''
```

**Review Notes:**
- Format: ✅ Valid
- Content: ✅ Clear, testable (±0.1V, 100ms update rate)
- Traceability: ✅ URD has no parent (top-level), so empty links OK
- Technical: ✅ 100ms update is feasible
- Units: ✅ Specified (V = volts, ms = milliseconds)

**Recommendation:** APPROVE

---

### 2. Reviewer 2 (Product Owner) Review

**Review Notes:**
- Business value: ✅ Addresses user need (battery health monitoring)
- Rationale: ✅ Well explained
- Acceptance criteria: ✅ Clear (±0.1V accuracy)
- Priority: ✅ Important for user safety

**Recommendation:** APPROVE

---

### 3. Final Approval

```bash
# Get commit SHA
git log -1 --format=%H
# Output: abc123def456

# Edit URD-002
nano reqs/urd/URD-002.yml
# Change: reviewed: ''
# To: reviewed: abc123def456

# Commit
git add reqs/urd/URD-002.yml
git commit -m "review: Approve URD-002 - Display Battery Voltage"
git push
```

**Result:** URD-002 is now approved and can be used as parent for SRD requirements! ✅

---

## Troubleshooting

### Problem: Linter errors
**Solution:** Run `python scripts/reqs_lint.py` and fix reported issues

### Problem: Broken parent link
**Solution:** Verify parent requirement exists and ref is correct

### Problem: Can't find git SHA
**Solution:** `git log -1 --format=%H` or `git rev-parse HEAD`

### Problem: Two reviewers disagree
**Solution:** Escalate to team lead for tie-breaking decision

### Problem: Requirement approved but needs change
**Solution:** 
1. Reset `reviewed: ''`
2. Make changes
3. Full re-review process
4. Document why change was needed

---

## Checklist Summary

**Before Submitting for Review:**
- [ ] Linter passes
- [ ] All mandatory fields filled
- [ ] 2 reviewers assigned
- [ ] `reviewed: ''` (empty)
- [ ] Committed to git

**During Review:**
- [ ] Format validated
- [ ] Content is clear, testable, complete
- [ ] Traceability correct
- [ ] Technically accurate
- [ ] Artifacts valid (if present)

**Approval:**
- [ ] Both reviewers approved
- [ ] Git SHA obtained
- [ ] `reviewed:` field updated
- [ ] Committed with proper message
- [ ] Stakeholders notified

---

**Questions?** Consult `docs/tsf/TSF-training.md` or escalate to team lead.