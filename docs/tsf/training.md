# TSF Training Guide - DrivaPi (Complete)

**Prerequisites:** Complete `docs/tsf/start.md` first
**Foundation:** Eclipse Trustable Software Framework

---

## 📋 Training Objectives

By the end of this training, you will:

✅ Understand TSF theoretical model (Statements, Links, DAGs)
✅ Know TSF methodology (6 key areas of evidence)
✅ Apply ISO 26262 basics and ASIL levels
✅ Write requirements following V-Model
✅ Create and maintain traceability
✅ Use official tools (`trudag` + `doorstop`)
✅ Complete full requirement lifecycle

---

## 🎯 Module 1: TSF Theoretical Model (45 min)

### 1.1 Core Principle

> **"Trust in software must be based on evidence, not assumptions"**

TSF is designed for software where **safety, security, performance, availability, and reliability** are critical.

### 1.2 Statements - The Building Blocks

**Definition:** A Statement is a definitive expression with meaningful interpretations when considered True or False.

#### ✅ Good Statement Example

```
"The Trustable project provides tools that are implemented in Python."
```

**Why it's good:**
- If **True**: Trustable offers Python tooling
- If **False**: Trustable has no Python tools (maybe other languages)
- Clear, unambiguous, testable

#### ❌ Bad Statement Example

```
"Trustable should be written in Python."
```

**Why it's bad:**
- Who thinks it should be Python?
- What parts should be Python?
- Unclear when True or False

### 1.3 Statement Writing Guidelines

**Statements should:**
- ✅ Use indicative mood (not subjunctive)
- ✅ Use third person perspective
- ✅ Use present tense
- ✅ Be affirmative (not negative)
- ✅ Be single sentences
- ✅ Be specific and testable

**Example for DrivaPi:**
```yaml
SWD-042:
  text: "The software SHALL read battery voltage via ADC channel 0
         with 12-bit resolution at 1 Hz sampling rate"
```

### 1.4 Links - Logical Implications

**Definition:** A Link from Statement A to Statement B means **A logically implies B**.

**Remember:** "B is a necessary but not sufficient condition for A"

**Convention:**
- Statement A = **parent**
- Statement B = **child**

**Example:**
```
Parent:  "The HMI SHALL display speed"
  ↓ (implies)
Child:   "The system SHALL provide speed data to HMI"
```

**In YAML:**
```yaml
SWD-001:
  text: "Qt widget SHALL display speed..."
  links: [SRD-001]  # ← SRD-001 is the parent (implied by this)
```

### 1.5 Trustable Graphs (DAGs)

**Definition:** The set of all Statements and their Links forms a **Directed Acyclic Graph (DAG)**.

**Why Acyclic?** No circular arguments allowed!

```
URD-001 (User need: Display speed)
   ↓ (implies)
SRD-001 (System: HMI shows speed)
   ↓ (implies)
SWD-001 (Software: Qt widget reads CAN)
   ↓ (implies)
LLTC-001 (Test: Verify widget timing)
```

This is a **Trustable Graph** - a DAG of Statements about DrivaPi.

### 1.6 Classifying Statements

**Based on Links:**

| Type | Definition | Has Parents? | Has Children? |
|------|------------|--------------|---------------|
| **Request** | Statement with children | Maybe | Yes |
| **Claim** | Statement with parents | Yes | Maybe |
| **Expectation** | Request but not Claim | No | Yes |
| **Assertion** | Both Request and Claim | Yes | Yes |
| **Premise** | Claim but not Request | Yes | No |

**Visual:**
```
Expectation (URD-001)
   ↓ links to
Assertion (SRD-001, SWD-001)  ← Has both parents and children
   ↓ links to
Premise (LLTC-001)
```

**In DrivaPi:**
- **URD** = Expectations (top-level user needs)
- **SRD/SWD** = Assertions (middle reasoning)
- **LLTC** = Premises (test cases, bottom)

### 1.7 Exercise: Identify Statement Types

Classify these statements:

1. **URD-001:** "User SHALL be able to view vehicle speed"
   - Has children? **Yes** (SRD-001)
   - Has parents? **No**
   - Type: **Expectation** ✅

2. **SWD-001:** "Qt widget SHALL read CAN speed at 10Hz"
   - Has children? **Yes** (LLTC-001)
   - Has parents? **Yes** (SRD-001)
   - Type: **Assertion** ✅

3. **LLTC-001:** "Test SHALL verify widget updates <100ms"
   - Has children? **No**
   - Has parents? **Yes** (SWD-001)
   - Type: **Premise** ✅

---

## 🔬 Module 2: TSF Methodology (60 min)

TSF methodology has **6 key areas** for collecting evidence:

### 2.1 Six Areas of Evidence Collection

#### 1. Provenance
**What:** Where software comes from, who produced it, what claims they make

**For DrivaPi:**
- Repository: `https://github.com/SEAME-pt/Team04_DrivaPi`
- Authors: Team04 (Hugo, João, Bernardo, Miguel, Melanie)
- License: Educational (SEAME Program)
- Git commit SHAs for traceability

**Evidence:**
```yaml
artifact:
  - type: provenance
    path: .git/logs/HEAD
    description: Git history showing all contributors
```

#### 2. Construction
**What:** How to build, install, and run it correctly

**For DrivaPi:**
- Build instructions: `docs/GETTING_STARTED.md`
- Dependencies: Python 3.11+, trudag, doorstop
- Build artifacts: Compiled code, reports
- Checksums: Verify correct build

**Evidence:**
```yaml
artifact:
  - type: construction
    path: docs/GETTING_STARTED.md
    description: Reproducible build instructions
  - type: construction
    path: artifacts/baselines/BASELINE-V1.0.tar.gz
    description: Complete build archive with checksums
```

#### 3. Change
**What:** How to update it, confidence it won't break/regress

**For DrivaPi:**
- Change management: Git + GitHub PRs
- Regression tests: CI runs on every PR
- Baseline comparisons: Before/after metrics
- Release notes: Document all changes

**Evidence:**
```yaml
artifact:
  - type: change
    path: .github/workflows/tsf-validation.yml
    description: CI pipeline prevents regressions
  - type: change
    path: artifacts/baselines/
    description: Baseline comparison data
```

#### 4. Expectations
**What:** What software is expected to do and must not do

**For DrivaPi:**
- Functional expectations: URD, SRD requirements
- Non-functional: Performance, safety limits
- Constraints: What it must NOT do (safety)

**Evidence:**
```yaml
artifact:
  - type: expectation
    path: reqs/urd/URD-001.yml
    description: User expectation for speed display
```

#### 5. Results
**What:** What software actually does vs expectations

**For DrivaPi:**
- Test results: JUnit XML, coverage reports
- Performance measurements: Timing, latency
- Actual behavior: Logs, telemetry

**Evidence:**
```yaml
artifact:
  - type: result
    path: artifacts/verification/tests/LLTC-001-junit.xml
    description: Test execution results (5/5 passed)
```

#### 6. Confidence
**What:** Overall confidence based on all evidence above

**For DrivaPi:**
- Requirements reviewed: 85% (target: 100%)
- Tests passing: 70/70 (target: maintain 100%)
- Coverage: 80% (target: 85%+)
- Traceability: 100% (maintained)

**Evidence:**
```yaml
# Generated by trudag
artifact:
  - type: confidence
    path: artifacts/traceability/matrix.csv
    description: Complete traceability matrix
  - type: confidence
    path: artifacts/verification/coverage/report.html
    description: Code coverage 82%
```

### 2.2 TSF Methodology: 5-Stage Iterative Process

You can tackle these in any order, iteratively:

#### Stage 1: Setting Expectations

**What:** Stakeholders agree on critical requirements

**Who:** Consumers, Contributors, Others (regulators)

**Output:** Expectations (Requests without parents)

**For DrivaPi:**
```yaml
URD-001:
  header: "Display vehicle speed"
  text: "The user SHALL be able to view current vehicle speed on dashboard"
  # This is an Expectation - it's a Request (has children) but not a Claim
```

#### Stage 2: Providing Evidence

**What:** Measure properties using Artifacts

**Two types:**

**Validation** (algorithmic):
- Properties of transient artifacts
- Example: Test results, performance metrics
- Automated: CI runs tests, records results

**Reference** (SME review):
- Properties of persistent artifacts
- Example: Design docs, source code quality
- Manual: Expert reviews and approves

**Evidence Definition:** Premises supported by Artifacts

**For DrivaPi:**
```yaml
LLTC-001:
  text: "Test SHALL verify speed widget updates within 100ms"
  artifact:
    - type: test
      path: tests/unit/test_speed_widget.cpp
      description: Test implementation (Reference)
    - type: test
      path: artifacts/verification/tests/LLTC-001-junit.xml
      description: Test results (Validation)
```

#### Stage 3: Documenting Assumptions

**What:** Requirements that cannot be satisfied within project scope

**Example:** "System requires Raspberry Pi 5"
- We don't build Raspberry Pi
- We assume it exists and works

**Assumptions:** Premises with NO justification (dangling)

**For DrivaPi:**
```yaml
ASSUMPTION-001:
  text: "Raspberry Pi 5 hardware is available and functional"
  # No links, no artifacts - this is an Assumption
  # We don't prove this, we assume it
```

**Why Important:** Transparency! Users know what they must provide.

#### Stage 4: Recording Reasoning

**What:** Intermediate logical steps between Expectations and Evidence

**Why:** Avoid large undocumented leaps in logic

**Output:** Assertions (middle layer)

**Qualification:** Assertions can be qualified by complex Artifacts

**For DrivaPi:**
```yaml
SRD-001:
  text: "System SHALL display speed with ±1 km/h accuracy, updated every 100ms"
  links: [URD-001]  # Parent: User expectation
  # This SRD is an Assertion - it connects user need to software implementation

SWD-001:
  text: "Qt widget SHALL read CAN speed at 10 Hz and update display"
  links: [SRD-001]  # Parent: System requirement
  # This SWD is also an Assertion - middle reasoning
```

#### Stage 5: Assessing Confidence

**What:** Score Evidence, calculate scores recursively

**Process:**
1. Score Evidence (SME review + validation)
2. Calculate scores for all Statements
3. Identify where to focus effort next

**For DrivaPi:**
- Evidence scored: Pass/Fail, or 0-100%
- Propagate up the DAG
- Low scores → need more work

### 2.3 Modification: Suspect vs Reviewed

**Problem:** How do we know a Statement and its score are still valid after changes?

**Solution:** Suspect marking

**Suspect:** Statement or Link that changed and needs re-review

**Convention:**
- **Suspect:** Needs human review
- **Reviewed/Clear:** Human verified

**In YAML:**
```yaml
SWD-001:
  reviewed: ''              # Suspect (empty)
  reviewed: 'abc123...'     # Reviewed (git SHA)
```

**Workflow:**
1. Create Statement → Mark reviewed
2. Modify Statement → Mark Suspect (reviewed: '')
3. Human reviews → Mark reviewed (trudag manage set-item)

---

## 🚗 Module 3: Automotive Standards (ISO 26262) (30 min)

### 3.1 ISO 26262 Overview

**Purpose:** Functional safety for road vehicles

**Goal:** Reduce risk of system failures causing harm

**V-Model:** Requirements → Implementation → Verification

### 3.2 ASIL Levels

**ASIL = Automotive Safety Integrity Level**

**Determination:** Severity × Exposure × Controllability

| ASIL | Risk | Examples | Verification |
|------|------|----------|--------------|
| **QM** | None | Radio | Basic review |
| **A** | Low | Rear lights | + Unit tests |
| **B** | Low-Med | Brake lights | + Integration + 2 reviewers |
| **C** | Medium | ABS | + System tests + coverage + independent review |
| **D** | High | Airbags | + HIL + formal sign-off |

### 3.3 ASIL for DrivaPi

**Typical assignments:**
```
Display features → ASIL A
Sensor processing → ASIL B
Motor control → ASIL B
Emergency stop → ASIL C (if implemented)
Configuration → QM
```

---

## 📝 Module 4: Requirements Engineering with TSF (45 min)

### 4.1 Writing Good Requirements as Statements

**Remember Statement rules:**
- Indicative mood
- Third person
- Present tense
- Affirmative
- Single sentence
- Testable!

**Use "SHALL" for mandatory:**
```yaml
text: "The system SHALL display speed in km/h ±1 km/h, updated every 100ms"
```

### 4.2 V-Model Decomposition (TSF Applied)

**URD (Expectation):**
```yaml
URD-001:
  ref: URD-001
  header: "Display vehicle speed"
  text: "The user SHALL be able to view current vehicle speed on the dashboard"
  links: []  # Expectation - no parents
  # This is a Request (has children below)
```

**SRD (Assertion):**
```yaml
SRD-001:
  ref: SRD-001
  header: "HMI speed display"
  text: "The system SHALL display vehicle speed on HMI with accuracy ±1 km/h, updated every 100ms"
  links: [URD-001]  # Claim (has parent) AND Request (has children)
  # This is an Assertion
```

**SWD (Assertion):**
```yaml
SWD-001:
  ref: SWD-001
  header: "Qt speed widget"
  text: "The software SHALL implement Qt widget reading CAN speed at 10 Hz, updating display within 100ms"
  links: [SRD-001]  # Claim AND Request
  # Another Assertion
```

**LLTC (Premise + Evidence):**
```yaml
LLTC-001:
  ref: LLTC-001
  header: "Test speed display timing"
  text: "Test SHALL verify Qt widget updates within 100ms when CAN speed message received"
  links: [SWD-001]  # Claim (has parent), no children
  # This is a Premise
  artifact:
    - type: test
      path: tests/unit/test_speed_widget.cpp
    - type: test
      path: artifacts/verification/tests/LLTC-001-junit.xml
  # With artifacts, this Premise becomes Evidence
```

### 4.3 Acceptance Criteria (GIVEN/WHEN/THEN)

```yaml
acceptance:
  - GIVEN: vehicle speed is 50 km/h
    WHEN: speed changes to 60 km/h
    THEN: display SHALL update within 100ms showing 60 ±1 km/h

  - GIVEN: CAN bus connection lost
    WHEN: timeout exceeds 500ms
    THEN: display SHALL show "---" and log error
```

---

## 🔗 Module 5: Traceability in TSF Context (30 min)

### 5.1 Traceability = DAG Structure

**Traceability is built-in!** Every Link creates traceability.

**Bidirectional automatically:**
```
Parent (SRD-001) ←→ Child (SWD-001)
```

Doorstop maintains both directions.

### 5.2 Generating Traceability Matrix

```bash
trudag manage migrate
trudag report export --output artifacts/trace.zip
```

**Matrix shows:**
- All Statements
- Their classifications (Expectation, Assertion, Premise)
- Parent/child Links
- Evidence attached
- Review status

### 5.3 Coverage Analysis

**Questions to ask:**
- Any Expectations without children? (Unimplemented needs)
- Any Assertions without Evidence chain? (Missing tests)
- Any Premises without Artifacts? (Should be Assumptions or need Evidence)
- Any Assumptions that should be satisfied? (Work needed)

---

## ✅ Module 6: Verification & Validation (30 min)

### 6.1 V&V in TSF Terms

**Validation:** Evidence collection (algorithmic)
- Run tests
- Measure performance
- Record results as Artifacts

**Verification:** Reference + Review (SME)
- Review design
- Review code quality
- Sign off requirements

### 6.2 Evidence Collection by ASIL

**ASIL A:**
- Premise + Artifact (test) = Evidence
- 1 reviewer

**ASIL B:**
- Evidence + Integration tests
- 2 reviewers
- Static analysis

**ASIL C:**
- Evidence + System tests + Coverage
- Independent reviewer
- Formal test reports

**ASIL D:**
- All above + HIL tests
- Formal sign-off
- Complete baseline

---

## 🛠️ Module 7: Official Tools (30 min)

### 7.1 trudag (Trustable DAG)

**Purpose:** Manage Trustable Graphs

```bash
# Validate Statements and Links
trudag manage lint

# Update DAG structure
trudag manage migrate

# Export Trustable report
trudag report export --output report.zip

# Approve Statement (mark reviewed)
trudag manage set-item reqs/swd/SWD-001.yml
```

### 7.2 doorstop (Requirements Backend)

**Purpose:** Create and manage Statements

```bash
# Create Statement
doorstop add SWD

# Edit Statement
doorstop edit SWD-001

# Create Link
doorstop link SWD-001 SRD-001

# Validate structure
doorstop
```

### 7.3 Why Official Tools Only

- Maintained by Eclipse Foundation
- Auditable and reproducible
- No custom code to maintain
- Community support
- TSF-compliant by design

---

## 🎓 Module 8: Hands-On Lab (90 min)

### Lab Objective

Create complete V-Model chain with TSF concepts applied.

### Scenario: Battery Voltage Display

You'll create:
1. **URD-002** (Expectation)
2. **SRD-002** (Assertion)
3. **SWD-002** (Assertion)
4. **LLTC-002** (Premise + Evidence)

### Lab Steps

#### Step 1: Create Expectation (URD)

```bash
doorstop add URD
doorstop edit URD-002
```

```yaml
URD-002:
  ref: URD-002
  header: "Display battery voltage"
  text: "The user SHALL be able to view current battery voltage on main dashboard to monitor battery health"
  ASIL: A
  Verification Method: User Acceptance Test
  links: []  # Expectation - no parents
  reviewers:
    - name: "Your Name"
      email: "you@example.com"
  reviewed: ''
  acceptance:
    - GIVEN: battery connected and system powered
      WHEN: user views main dashboard
      THEN: battery voltage SHALL be visible with unit (V)
  active: true
  derived: false
  normative: true
  level: 1.0
```

#### Step 2: Create First Assertion (SRD)

```bash
doorstop add SRD
doorstop edit SRD-002
doorstop link SRD-002 URD-002  # Create Link!
```

```yaml
SRD-002:
  ref: SRD-002
  header: "HMI battery voltage display"
  text: "The system SHALL display battery voltage on HMI with accuracy ±0.1V, sampled at 1 Hz"
  ASIL: A
  Verification Method: System Test
  links: [URD-002]  # This makes it an Assertion (Claim + Request)
  reviewers:
    - name: "Your Name"
      email: "you@example.com"
  reviewed: ''
  acceptance:
    - GIVEN: battery voltage is 12.0V
      WHEN: system reads voltage
      THEN: display SHALL show 12.0V ±0.1V within 1 second
  active: true
  derived: false
  normative: true
  level: 1.0
```

#### Step 3: Create Second Assertion (SWD)

```bash
doorstop add SWD
doorstop edit SWD-002
doorstop link SWD-002 SRD-002
```

```yaml
SWD-002:
  ref: SWD-002
  header: "Battery voltage ADC reader"
  text: "The software SHALL read battery voltage via ADC channel 0 with 12-bit resolution at 1 Hz and update Qt BatteryWidget"
  ASIL: A
  Verification Method: Unit Test
  links: [SRD-002]  # Assertion
  reviewers:
    - name: "Your Name"
      email: "you@example.com"
  reviewed: ''
  acceptance:
    - GIVEN: ADC configured for channel 0
      WHEN: readVoltage() called
      THEN: SHALL return voltage as float within 100ms
  active: true
  derived: false
  normative: true
  level: 1.0
```

#### Step 4: Create Premise with Evidence (LLTC)

```bash
doorstop add LLTC
doorstop edit LLTC-002
doorstop link LLTC-002 SWD-002
```

```yaml
LLTC-002:
  ref: LLTC-002
  header: "Test battery voltage reader"
  text: "Test SHALL verify battery voltage reader accuracy and timing"
  ASIL: A
  Verification Method: Unit Test
  links: [SWD-002]  # Premise (Claim only, no children)
  reviewers:
    - name: "Your Name"
      email: "you@example.com"
  reviewed: ''

  # This makes it Evidence (Premise + Artifacts)
  artifact:
    - type: test
      path: tests/unit/test_battery_reader.cpp
      description: Unit test implementation
    - type: test
      path: artifacts/verification/tests/LLTC-002-junit.xml
      description: Test results (5/5 passed)

  test_procedure:
    setup:
      - Mock ADC with known voltage (12.0V)
      - Initialize BatteryVoltageReader
    test_steps:
      - step: 1
        action: Call readVoltage()
        expected: Returns 12.0 ±0.1V
      - step: 2
        action: Measure execution time
        expected: Completes in <100ms

  active: true
  derived: false
  normative: true
  level: 1.0
```

#### Step 5: Validate DAG

```bash
trudag manage lint
doorstop
```

**Check:**
- All Statements valid?
- All Links valid?
- DAG structure correct?

#### Step 6: Generate Trustable Report

```bash
trudag manage migrate
trudag report export --output artifacts/lab-report.zip

cd artifacts/
unzip lab-report.zip -d lab-report/
cat lab-report/matrix.csv
```

**Verify:**
- URD-002: Expectation (Request, no parents)
- SRD-002: Assertion (Claim + Request)
- SWD-002: Assertion (Claim + Request)
- LLTC-002: Evidence (Premise + Artifacts)

#### Step 7: Mark Reviewed

```bash
trudag manage set-item reqs/urd/URD-002.yml
trudag manage set-item reqs/srd/SRD-002.yml
trudag manage set-item reqs/swd/SWD-002.yml
trudag manage set-item reqs/lltc/LLTC-002.yml
```

### Lab Success Criteria

- [ ] 4 Statements created (URD, SRD, SWD, LLTC)
- [ ] Links form valid DAG
- [ ] `trudag manage lint` passes
- [ ] `doorstop` passes
- [ ] Matrix shows: 1 Expectation, 2 Assertions, 1 Evidence
- [ ] All marked reviewed

---

## 📚 Module 9: Documentation & Baselines (30 min)

### 9.1 Required Documentation

**Per Requirement:**
- YAML with all mandatory fields
- TSF classification (Expectation/Assertion/Premise/Evidence)
- Links (parents)
- Artifacts (if Evidence)
- Review evidence (`reviewed:` field)

**Per Sprint:**
- Traceability matrix (DAG visualization)
- Coverage report
- Confidence scores

**Per Baseline:**
- Trustable report (ZIP)
- All verification artifacts
- Provenance records

### 9.2 Baseline Process

```bash
# Validate everything
trudag manage lint
doorstop

# Generate Trustable report
trudag report export --output artifacts/baselines/BASELINE-V1.0.zip

# Create git tag
git tag -a BASELINE-V1.0 -m "Baseline v1.0

Requirements: 110 total
- 30 Expectations (URD)
- 50 Assertions (SRD/SWD)
- 30 Evidence (LLTC with artifacts)

Traceability: 100%
Reviewed: 100%
Confidence: High

TSF Methodology Applied:
- Provenance: Git history
- Construction: Reproducible builds
- Change: CI/CD regression tests
- Expectations: Complete DAG
- Results: All tests passing
- Confidence: 85% coverage

Date: $(date +%Y-%m-%d)"

git push origin BASELINE-V1.0
```

---

## 🎯 Module 10: Summary & Next Steps (15 min)

### Key Takeaways

1. **TSF Model:**
   - Statements = building blocks
   - Links = logical implications
   - DAG = Trustable Graph

2. **TSF Classification:**
   - Expectation = Request without parents (URD)
   - Assertion = Request + Claim (SRD/SWD)
   - Premise = Claim without children (LLTC)
   - Evidence = Premise + Artifacts

3. **TSF Methodology (6 areas):**
   - Provenance
   - Construction
   - Change
   - Expectations
   - Results
   - Confidence

4. **Daily Practice:**
   - Write Statements (not just requirements)
   - Create Links (build DAG)
   - Collect Evidence (Artifacts)
   - Review and mark reviewed
   - Generate Trustable reports

### Next Steps

1. **Review:** docs/TSF_REFERENCE.md
2. **Practice:** Create requirements daily
3. **Validate:** Run trudag + doorstop always
4. **Evidence:** Link artifacts systematically
5. **Baseline:** Regular snapshots

### Resources

- **TSF Official:** https://codethinklabs.gitlab.io/trustable/trustable/
- **Doorstop:** https://doorstop.readthedocs.io/
- **ISO 26262:** https://www.iso.org/standard/68383.html

---

**Training Complete! 🎉**

You now understand:
- TSF theoretical foundation
- TSF methodology
- Automotive standards
- Practical implementation

**Continue with:** docs/WORKFLOWS.md for daily operations

---

**Last Updated:** October 2025
**Version:** 2.0 (Complete with TSF Theory)
**Maintained By:** DrivaPi Team
