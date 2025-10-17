# TSF Team Assessment Quiz & Lab

**Purpose:** Validate team understanding of TSF principles, automotive standards, and project implementation  
**Duration:** 45-60 minutes (30 min quiz + 15-30 min practical lab)  
**Passing Score:** 80% (16/20 questions)  
**Format:** Individual quiz, pair programming lab

---

## Part 1: Knowledge Assessment (30 minutes)

### Section A: TSF Fundamentals (5 questions)

**Q1. What are the three core components of a Trustable Graph?**
- [ ] A) Requirements, Tests, Code
- [ ] B) Statements, Links, Evidence
- [ ] C) Users, Systems, Software
- [ ] D) ASIL, Hazards, Mitigations

**Q2. In TSF terminology, what is an "Expectation"?**
- [ ] A) A Statement that is a Request but not a Claim
- [ ] B) A Statement that is both a Request and a Claim
- [ ] C) A Statement that is a Claim but not a Request
- [ ] D) A Statement without any Links

**Q3. What does bidirectional traceability mean?**
- [ ] A) Requirements can link to both code and tests
- [ ] B) You can trace from requirements to implementation AND from implementation back to requirements
- [ ] C) Requirements exist in two directions: forward and backward
- [ ] D) Traceability works in Doorstop and trudag

**Q4. What is the purpose of a "baseline" in requirements management?**
- [ ] A) The first requirement created
- [ ] B) A snapshot of approved requirements at a specific point in time
- [ ] C) The minimum number of requirements needed
- [ ] D) The default template for all requirements

**Q5. Which field in a requirement YAML proves it has been reviewed?**
- [ ] A) `reviewers: [names]`
- [ ] B) `status: approved`
- [ ] C) `reviewed: <git-sha>`
- [ ] D) `approved: true`

---

### Section B: Automotive Standards (5 questions)

**Q6. What does ISO 26262 primarily address?**
- [ ] A) Software quality metrics
- [ ] B) Functional safety for road vehicles
- [ ] C) Cybersecurity requirements
- [ ] D) Performance testing standards

**Q7. What does ASIL stand for?**
- [ ] A) Automotive Safety Integrity Level
- [ ] B) Advanced Software Integration Layer
- [ ] C) Approved Safety Implementation List
- [ ] D) Automated System Integrity Logging

**Q8. Which ASIL level requires the most stringent safety measures?**
- [ ] A) ASIL A
- [ ] B) ASIL B
- [ ] C) ASIL C
- [ ] D) ASIL D

**Q9. In the V-model, what corresponds to System Requirements (SRD)?**
- [ ] A) Unit Tests
- [ ] B) Integration Tests
- [ ] C) System Tests
- [ ] D) Acceptance Tests

**Q10. What is the purpose of ASPICE?**
- [ ] A) To define functional safety requirements
- [ ] B) To assess software development process capability
- [ ] C) To specify automotive security standards
- [ ] D) To mandate specific programming languages

---

### Section C: Requirements Engineering (5 questions)

**Q11. What are the three levels in the DrivaPi requirements hierarchy?**
- [ ] A) High, Medium, Low
- [ ] B) User (URD), System (SRD), Software (SWD)
- [ ] C) Requirements, Design, Implementation
- [ ] D) ASIL A, B, C

**Q12. Which of these is a characteristic of a GOOD requirement?**
- [ ] A) Vague and open to interpretation
- [ ] B) Testable, unambiguous, and traceable
- [ ] C) Contains implementation details
- [ ] D) Written in technical jargon

**Q13. What command checks requirements for format errors?**
- [ ] A) `doorstop check`
- [ ] B) `python scripts/reqs_lint.py`
- [ ] C) `trudag validate`
- [ ] D) `git lint`

**Q14. How should a Software Requirement (SWD) relate to a System Requirement (SRD)?**
- [ ] A) SWD is independent of SRD
- [ ] B) SWD links to SRD as a parent
- [ ] C) SRD links to SWD as a parent
- [ ] D) They don't need to be related

**Q15. What does LLTC stand for in the DrivaPi project?**
- [ ] A) Low-Level Test Cases
- [ ] B) Linked List Test Coverage
- [ ] C) Long-term Lifecycle Testing
- [ ] D) Linux Layer Test Configuration

---

### Section D: Project Implementation (5 questions)

**Q16. Where are User Requirements stored in the DrivaPi repository?**
- [ ] A) `docs/requirements/`
- [ ] B) `reqs/urd/`
- [ ] C) `artifacts/urd/`
- [ ] D) `tests/requirements/`

**Q17. What tool generates the traceability matrix in CI?**
- [ ] A) `doorstop`
- [ ] B) `trudag`
- [ ] C) `scripts/traceability_check.py`
- [ ] D) `pytest`

**Q18. What does `gap-report.csv` identify?**
- [ ] A) Missing requirements
- [ ] B) Broken links between requirements
- [ ] C) Missing artifact links (design, code, tests)
- [ ] D) Requirements that need approval

**Q19. How often does the CI workflow validate requirements?**
- [ ] A) Daily at midnight
- [ ] B) On every push/PR
- [ ] C) Weekly on Mondays
- [ ] D) Only when manually triggered

**Q20. What is the correct naming format for a Software Requirement?**
- [ ] A) `SWD_001.yaml`
- [ ] B) `swd-001.yml`
- [ ] C) `SWD-001.yml`
- [ ] D) `software-req-001.yaml`

---

## Part 2: Practical Lab (15-30 minutes)

### Exercise 1: Create a New Requirement (10 min)

**Scenario:** The DrivaPi system needs to display battery voltage on the HMI.

**Task:** Create a new User Requirement (URD-002) using the template.

**Steps:**
1. Copy `reqs/templates/URD-000.yml` to `reqs/urd/URD-002.yml`
2. Fill in the fields:
   - `ref: URD-002`
   - `header: Display Battery Voltage`
   - `text: The system shall display the current battery voltage on the main dashboard with accuracy of ±0.1V`
   - `rationale: Users need to monitor battery health for safety`
   - `links: []` (leave empty for now)
   - `reviewers: [your-name]`
   - `reviewed: ''` (empty until approved)

3. Validate with linter: `python scripts/reqs_lint.py`
4. Check Doorstop: `doorstop`

**Success Criteria:**
- ✅ File created with correct naming
- ✅ No linter errors
- ✅ Doorstop reports no issues

---

### Exercise 2: Create Traceability Link (10 min)

**Task:** Create a System Requirement (SRD-002) that implements URD-002, then link them.

**Steps:**
1. Copy `reqs/templates/SRD-000.yml` to `reqs/srd/SRD-002.yml`
2. Fill in the fields:
   - `ref: SRD-002`
   - `header: Battery Voltage ADC Reading`
   - `text: The system shall read battery voltage via ADC channel 0 with 12-bit resolution at 10Hz sampling rate`
   - `links: [URD-002]` ← **This creates traceability!**
   - `reviewers: [your-name]`

3. Run traceability check: `python scripts/traceability_check.py`
4. Check the generated `artifacts/traceability-matrix.csv`

**Success Criteria:**
- ✅ SRD-002 created correctly
- ✅ Link to URD-002 established
- ✅ Traceability matrix shows URD-002 → SRD-002
- ✅ No broken links in traceability report

---

### Exercise 3: Review and Approve (5 min)

**Task:** Approve your URD-002 requirement following the review process.

**Steps:**
1. Open `reqs/urd/URD-002.yml`
2. Review the requirement against the checklist:
   - [ ] Clear and unambiguous?
   - [ ] Testable?
   - [ ] Traceable?
   - [ ] Follows template format?

3. If approved, get the current git commit SHA:
   ```bash
   git log -1 --format=%H
   ```

4. Update the requirement:
   ```yaml
   reviewed: <git-sha-from-step-3>
   ```

5. Commit with message:
   ```bash
   git add reqs/urd/URD-002.yml
   git commit -m "review: Approve URD-002 - Display Battery Voltage"
   ```

**Success Criteria:**
- ✅ Requirement has valid `reviewed` field with git SHA
- ✅ Committed to version control
- ✅ Follows review process from `REVIEWER_CHECKLIST.md`

---

## Answer Key (For Assessor Only)

### Part 1 Answers:
1. B | 2. A | 3. B | 4. B | 5. C
6. B | 7. A | 8. D | 9. C | 10. B
11. B | 12. B | 13. B | 14. B | 15. A
16. B | 17. C | 18. C | 19. B | 20. C

### Part 2 Grading Rubric:

**Exercise 1 (30 points):**
- File created correctly (5 pts)
- All mandatory fields filled (10 pts)
- Content quality (10 pts)
- No linter errors (5 pts)

**Exercise 2 (40 points):**
- SRD-002 created correctly (10 pts)
- Link field correctly references URD-002 (15 pts)
- Traceability matrix updated (10 pts)
- No broken links (5 pts)

**Exercise 3 (30 points):**
- Review checklist completed (10 pts)
- Git SHA correctly inserted (10 pts)
- Proper commit message (10 pts)

**Total: 100 points** (Part 1: 20 pts, Part 2: 80 pts weighted to 20 pts)

---

## Post-Assessment Actions

### For Passing Team Members (≥80%)
- ✅ Considered TSF-competent
- ✅ Authorized to review requirements
- ✅ Can approve requirements after SME review

### For Team Members Needing Improvement (<80%)
- Review incorrect answers with trainer
- Re-study relevant sections of `TSF-training.md`
- Retake assessment after 2-3 days

### Documentation
1. Record scores in team assessment log
2. Update team competency matrix
3. File completed assessments in `docs/team/assessments/`
4. Update Definition of Done tracking: "Assessment confirms understanding" ✅

---

## Notes for Assessor

- Quiz is open-book (they can reference `TSF-training.md`)
- Lab should be done individually but questions allowed
- Practical lab is more important than quiz score
- Focus on understanding, not memorization
- Adapt difficulty based on team experience level