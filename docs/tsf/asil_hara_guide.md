# ASIL Determination & HARA Documentation Guide

**Date:** October 30, 2025

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [Understanding ASIL](#1-understanding-asil)
3. [ASIL Calculation Methodology](#2-asil-calculation-methodology)
4. [HARA Process](#3-hara-process)
5. [Templates](#4-templates)
6. [Standard Locations](#5-standard-locations)
7. [Linking to TSF Requirements](#6-linking-to-tsf-requirements)
8. [Practical Example](#7-practical-example)
9. [Best Practices](#8-best-practices)
10. [Summary](#9-summary)
11. [Citations](#10-citations)
12. [Next Steps](#11-next-steps)

---

## Executive Summary

This guide provides the team with:
1. Understanding of ASIL calculation methodology (Severity × Exposure × Controllability)
2. Templates for HARA and ASIL justification documentation
3. Standard location and linking process for compliance evidence in TSF

**Research Scope:** Review of ISO 26262:2018 Parts 2, 3, 6, and 9, with practical examples from automotive industry sources (Jama Software, Spyro-Soft, Infineon, etc.). All information is sourced from ISO 26262 standard documentation and verified automotive industry sources.

**Verification Date:** October 30, 2025. Cross-check sources for updates regularly.

---

## 1. Understanding ASIL

### What is ASIL?

**ASIL (Automotive Safety Integrity Level)** is a risk classification system defined by ISO 26262[59]. It determines the safety requirements necessary for electrical/electronic systems in vehicles[53].

**ASIL Levels** (from ISO 26262)[53][59]:
- **QM (Quality Management):** No automotive hazard, no special safety requirements
- **ASIL A:** Lowest risk level
- **ASIL B:** Medium-low risk
- **ASIL C:** Medium-high risk
- **ASIL D:** Highest risk level (most stringent requirements)

**DrivaPi Context:**
DrivaPi is a prototype vehicle control system for testing and education. ASIL assignments depend on deployment context:
- **QM:** Display widgets (speed, temperature displays)
- **ASIL A:** Non-critical monitoring functions (e.g., rear lighting indicators)
- **ASIL B:** Typical for prototype sensors and control loops (speed sensor, temperature sensor, motor control)
- **ASIL C:** Safety-critical functions (e.g., emergency stop)
- **ASIL D:** Not intended for DrivaPi (reserved for production systems with critical safety implications)

**Note:** Examples in this guide use ASIL B/C for illustration. Actual DrivaPi assessments must account for deployment context and intended use.

---

## 2. ASIL Calculation Methodology

ASIL is determined through **HARA (Hazard Analysis and Risk Assessment)** using three factors as defined in ISO 26262 Part 3[51][63][76].

### Formula:
```
ASIL = f(Severity, Exposure, Controllability)
```

### 2.1 Severity (S)

**Definition:** Potential harm to people if the hazard occurs[56][76]

**Classifications (from ISO 26262-3:2018, Annex B.2)**[56][76]:

| Class | Description | Injury Level |
|-------|-------------|--------------|
| **S0** | No injuries | Minor property damage only |
| **S1** | Light to moderate injuries | Bruises, sprains |
| **S2** | Severe to life-threatening injuries (survival probable) | Broken bones, concussions |
| **S3** | Life-threatening (survival uncertain) to fatal injuries | Critical trauma, death |

---

### 2.2 Exposure (E)

**Definition:** Frequency of conditions in which the hazard could occur[56][76]

**Classifications (from ISO 26262-3:2018, Annex B.3)**[56][76]:

| Class | Description | Operating Time (Qualitative) | Example |
|-------|-------------|------------------------------|---------|
| **E0** | Incredibly unlikely | < 0.001% | Once per 1000+ years of operation |
| **E1** | Very low probability | < 1% | Less than once per year for most drivers |
| **E2** | Low probability | 1-10% | A few times in vehicle lifetime |
| **E3** | Medium probability | ~10% | Approximately once per month |
| **E4** | High probability | > 10% | During almost every drive |

**Note:** Percentages are approximate and qualitative. Assessments should be based on operational data and realistic use cases, per ISO 26262-3:2018 Annex B.3.

---

### 2.3 Controllability (C)

**Definition:** Ability of driver/operator to control the situation and prevent harm[56][76]

**Classifications (from ISO 26262-3:2018, Annex B.4)**[56][76]:

| Class | Description |
|-------|-------------|
| **C0** | Controllable in general - Controllable by nearly all road users, including vulnerable ones (no specific skills required). *Note: For C0, ASIL is always QM per ISO 26262-3:2018, Annex B.4* |
| **C1** | Simply controllable - 99% or more of average drivers/operators can avoid harm with sufficient warning time; easy recovery |
| **C2** | Normally controllable - 90% or more of average drivers/operators can avoid harm with reasonable warning time and actions |
| **C3** | Difficult to control or uncontrollable - Less than 90% of average drivers/operators can avoid harm due to limited reaction time or complex control requirements |

---

### 2.4 ASIL Determination Table

**Source:** ISO 26262-3:2018, Clause 6.4.3.10, Table 4[71]

**Note:** For C0 (controllable in general), ASIL is always QM and is not included in this classification table. See ISO 26262-3:2018, Annex B.4.

**Additional Note:** For S3 E1 C3 = A, see ISO 26262-3:2018 Clause 6.4.3.11: If several unlikely situations are combined that result in a lower probability of exposure than E1, QM may be argued for S3, C3 based on this combination.

| Severity | Exposure | **Controllability C1** | **Controllability C2** | **Controllability C3** |
|----------|----------|----------------------|----------------------|----------------------|
| **S1** | E1 | QM | QM | QM |
| | E2 | QM | QM | QM |
| | E3 | QM | QM | A |
| | E4 | QM | A | B |
| **S2** | E1 | QM | QM | QM |
| | E2 | QM | QM | A |
| | E3 | QM | A | B |
| | E4 | A | B | C |
| **S3** | E1 | QM | QM | A |
| | E2 | QM | A | B |
| | E3 | A | B | C |
| | E4 | B | C | D |

**Reading the Table:** Intersection of Severity (row) and (Exposure, Controllability) column = ASIL.

**Example:** S3 / E3 / C3 = **ASIL C** (per table above)

---

## 3. HARA Process

According to ISO 26262 Part 3[51][63], HARA consists of the following steps:

### Step 1: Item Definition

Define the system/component to be analyzed:
- Description and boundaries
- Interfaces with other systems
- Assumptions about operation

**Source:** ISO 26262-3:2018, Clause 5[63][74]

### Step 2: Hazard Identification

Identify potential hazards from malfunctions or misuse using:
- **HAZOP (Hazard and Operability Analysis)**: Identifies risks from functional deviations[63]
- **STPA (System-Theoretic Process Analysis)**: System-level safety analysis[63]

**Source:** ISO 26262-3:2018, Chapter 6.4.2[63]

### Step 3: Identify Operational Situations

Define relevant scenarios where the item operates (e.g., city driving, highway driving, parking)[63][74]

### Step 4: Define Hazardous Events

Combine hazard + operational situation = hazardous event[63]

**Example from industry practice**[74]:
```
Hazard: Loss of speed control
+ Operational Situation: Highway driving
= Hazardous Event: Uncontrolled speed increase at high speed
```

### Step 5: Assess Severity (S)

Determine potential harm level (S0-S3) using ISO 26262-3 classifications[56][76]

### Step 6: Assess Exposure (E)

Determine likelihood of the operational situation (E0-E4)[56][76]

### Step 7: Assess Controllability (C)

Determine if driver can prevent harm (C0-C3)[56][76]

### Step 8: Determine ASIL

Use ISO 26262-3:2018 Table 4 to assign ASIL (QM/A/B/C/D)[71]

### Step 9: Derive Safety Goals

Set safety goals based on identified hazards[63]

**Source:** ISO 26262-3:2018, Clause 6.4.4[63]

---

## 4. Templates

### 4.1 HARA Template

**Based on:** ISO 26262-3:2018 structure and industry best practices[63][72]
**File Location:** `docs/standards/iso26262/hara-<feature>.md`
**Version Control:** Use semantic versioning (v1.0, v1.1, etc.) for traceability

```markdown
# HARA: <Feature Name>

**Version:** 1.0 | **Date:** YYYY-MM-DD | **Authors/Reviewers:** <Names> | **ASIL Target:** <QM/A/B/C/D>

## 1. Item Definition & Assumptions
- **Description/Boundaries/Interfaces:** <Brief overview, e.g., "Speed control system for DrivaPi test track; excludes public road use.">
- **Assumptions:** <Key points, e.g., "Controlled environment; trained operators.">

## 2. Hazards & Operational Situations
| ID | Hazard Description | Operational Situation | Exposure (E) / Justification |
|----|--------------------|-----------------------|------------------------------|
| H-01 | Loss of speed control | Highway simulation (E3-E4) | ~30% of tests; based on logs. |

## 3. Risk Assessment & ASIL
For each hazard:
- **Severity (S):** <S1-S3> - <Justification, e.g., "S3: Potential collision.">
- **Controllability (C):** <C1-C3> - <Justification, e.g., "C3: Limited reaction time.">
- **ASIL Calculation:** <From table, e.g., S3 + E4 + C3 = D (conservative for prototype).>

## 4. Safety Goals
| Goal ID | ASIL | Description | Safe State |
|---------|------|-------------|------------|
| SG-01 | D | Prevent unintended acceleration | Vehicle stop. |

## 5. Dependencies & Review Notes
- **Dependencies:** <e.g., FMEA link, emergency stop.>
- **Review:** <Brief notes or Git commit reference.>

**References:** ISO 26262-3:2018.
```

---

### 4.2 ASIL Justification Template

**Based on:** ISO 26262 Part 9 and industry practices[53][61]
**File Location:** `docs/standards/iso26262/asil-justification-<REQ-ID>.md`
**Version Control:** Use semantic versioning for traceability

```markdown
# ASIL Justification: <REQ-ID>

**Version:** 1.0 | **Date:** YYYY-MM-DD | **Author/Reviewer:** <Names> | **ASIL:** <A/B/C/D>

## 1. Requirement Summary
- **ID/Description:** <REQ-ID> - <Brief text, e.g., "Speed sensor driver shall provide accurate data.">
- **Related HARA/Safety Goal:** <Link to HARA, e.g., hara-speed-control.md; SG-ID from there.>

## 2. ASIL Assignment & Decomposition
- **Assigned ASIL:** <A/B/C/D> - Derived from HARA (e.g., S3 + E3 + C3 = C).
- **Justification:** <Brief rationale, e.g., "Based on controlled test environment; conservative for prototype.">
- **Decomposition (if applicable):** Original ASIL <X> decomposed to: <e.g., ASIL B(<X>) + ASIL A(<X>)>.
  - Scheme: <From ISO table, e.g., C → B(C) + A(C)>.
  - Independence: <Key evidence, e.g., "Separate sensors/code; no shared resources (reference FMEA).">

## 3. Risk Assessment Summary
| Factor | Classification | Justification/Evidence |
|--------|----------------|------------------------|
| Severity (S) | <S1-S3> | <e.g., "S3: Potential collision; from HARA."> |
| Exposure (E) | <E1-E4> | <e.g., "E3: ~10% of tests; based on logs/simulations."> |
| Controllability (C) | <C1-C3> | <e.g., "C3: <90% success; emergency stop available."> |

## 4. Verification Requirements (Tailored to ASIL)
- Unit/Integration Testing: <e.g., >80% coverage for ASIL B; link to test artifacts.>
- Static Analysis: <e.g., Zero critical errors; validator in evidence: block.>
- Reviews: <e.g., Independent for ASIL C+ (I2 level); Git PR link.>
- Other: <e.g., Fault injection for ASIL C; coverage report.>

## 5. Review Notes & Dependencies
- **Dependencies:** <e.g., HARA doc, FMEA, emergency system.>
- **Review:** <Brief notes, e.g., "Approved via Git PR #142; independence per ISO 26262-2 Table 1.">

**References:** ISO 26262-9:2018; Link to TSF requirement via `references: - type: "compliance" path: this-file.md`.
```

---

## 5. Standard Locations

### Directory Structure

```
docs/
└── standards/
    └── iso26262/
        ├── README.md                          # Index of all HARA/ASIL docs
        ├── hara-speed-control.md              # HARA for speed control system
        ├── hara-temperature-monitoring.md     # HARA for temperature system
        ├── hara-emergency-stop.md             # HARA for emergency stop
        ├── asil-justification-SWD-042.md      # ASIL justification for SWD-042
        ├── asil-justification-SRD-015.md      # ASIL justification for SRD-015
        └── asil-decomposition-analysis.md     # Decomposition documentation (if applicable)
```

---

## 6. Linking to TSF Requirements

### 6.1 Using `references:` and `compliance` Artifact Type

Add compliance evidence to requirement frontmatter using `type: "compliance"` for standards/HARA documents:

```yaml
---
id: SWD-042
header: "Speed sensor driver implementation"
text: |
  Driver shall read speed sensor and provide accurate speed data

ASIL: B
verification_method: Analysis

references:
  - type: "file"
    path: src/sensors/speed.cpp
    description: "Speed sensor driver implementation"

  - type: "compliance"
    path: docs/standards/iso26262/hara-speed-control.md
    description: "HARA for speed control system"

  - type: "compliance"
    path: docs/standards/iso26262/asil-justification-SWD-042.md
    description: "ASIL B justification with independence analysis"

  - type: "file"
    path: artifacts/verification/tests/LLTC-142-junit.xml
    description: "Unit test results (JUnit XML)"

score:
  SafetyEngineer: 0.95
---
```

**Note:** Using `type: "compliance"` distinguishes standards/HARA evidence from code/test files (type: "file"). The `description` field provides human-readable traceability.

---

## 7. Practical Example: Speed Control System

### 7.1 HARA Summary

**Feature:** Speed Control System
**File:** `docs/standards/iso26262/hara-speed-control.md`
**Deployment Context:** DrivaPi prototype on controlled test track

**Hazardous Event HE-01:** Loss of speed control during high-speed testing

**Risk Assessment** (following ISO 26262-3:2018, Table 4)[71]:
- **Severity:** S3 (life-threatening/fatal) - Collision at high speed
- **Exposure:** E4 (>10% of operation) - High-speed testing ~40% of scenarios
- **Controllability:** C3 (difficult, <90% success) - Limited reaction time at high speed
- **Result:** ASIL D (from Table 4)

**Used here for illustrative purposes in a conservative analysis; actual DrivaPi prototype may justify lower (e.g., ASIL C via E3) based on controlled testing environment.**

**Safety Goal SG-01 (ASIL D):**
"The speed control system shall prevent unintended acceleration to unsafe speeds and provide independent emergency deceleration"

**Source:** ISO 26262-3:2018, Table 4[71]

### 7.2 ASIL Decomposition Example

**Original:** SG-01 ASIL D

**Decomposed** (per ISO 26262-9:2018, Clause 5)[61]:
- SWD-042 (Speed sensor driver): **ASIL B(D)** - Primary speed measurement
- SWD-043 (Speed monitoring watchdog): **ASIL B(D)** - Independent speed verification

**Justification:** Two independent speed measurement channels with sufficient independence (separate sensors, separate microcontroller pins, separate code modules). Failure of one channel detected by the other. Both channels must fail to violate safety goal.

**Independence Analysis:**
- Hardware: Separate sensors (potentiometer + encoder) on different I2C addresses
- Software: Separate driver modules with no shared memory
- Common Cause: Single point failure (communication loss) triggers safe state (motor stop)

**Decomposition requires demonstrating independence and no common-mode failures (ISO 26262-9:2018, Clause 5.4). Reference FMEA for evidence.**

**Reference:** ISO 26262-9:2018, Clause 5.4, Tables 1-2[61]

---

## 8. Best Practices

### 8.1 ASIL Assignment
1. **Start with HARA** - Always derive ASIL from systematic hazard analysis[51][63]
2. **Document assumptions** - Clearly state operational scenarios and deployment context[74]
3. **Be conservative** - When in doubt, choose higher severity/exposure for safety-critical functions[59]
4. **Use data-driven justification** - Base Exposure and Controllability assessments on operational data, simulation logs, or human factors studies to avoid subjectivity[76]
5. **Document rationale** - Record and justify conservative vs. data-driven choices (e.g., E4 vs. E3) in HARA to support audits and maintain compliance
6. **Reassess if context changes** - If operational context changes (e.g., from test track to public roads), re-perform HARA and update ASIL accordingly
7. **Independent review** - ISO 26262 requires independent review of HARA for ASIL C/D[53][59]

### 8.2 Documentation
1. **Link early** - Create HARA documents during concept phase (ISO 26262 Part 3)[63]
2. **Version control** - Keep HARA documents in Git with semantic versioning for traceability
3. **Cross-reference** - Link requirements ↔ HARA ↔ ASIL justifications via `type: "compliance"` blocks
4. **Timestamp reviews** - Record review dates and signatures for audit trail
5. **Change history** - Maintain version history in templates for compliance audits

### 8.3 Verification
1. **ASIL-specific evidence** - Higher ASIL = more verification artifacts required[50][59]
2. **Coverage targets** (per ISO 26262-6:2018, Tables 7-12)[59]:
   - ASIL A: ≥ 60%
   - ASIL B: ≥ 80%
   - ASIL C: ≥ 90%
   - ASIL D: ≥ 95%
   - **Note:** Targets are recommendations per ISO 26262-6:2018; tailor per project needs and tool capabilities
3. **Fault injection testing** - Recommended for ASIL C, required for ASIL D (ISO 26262-6:2018, Table 11)

---

## 9. Summary

**ASIL Calculation:**
`ASIL = f(Severity, Exposure, Controllability)` using ISO 26262-3:2018 Clause 6.4.3.10, Table 4[71]

**HARA Process:**
9 steps from item definition → operational situations → hazards → risk assessment → ASIL → safety goals[63]

**Templates Created:**
- **HARA template** (based on ISO 26262-3:2018[63][72]) with version control, change history, and E-class columns
- **ASIL Justification template** (based on ISO 26262-9:2018[53][61]) with data-driven guidance, rationale documentation, and fault injection requirements

**Linking Method:**
Use `references:` block with `type: "compliance"` in TSF requirements to link HARA and ASIL justification documents

**Standard Location:**
`docs/standards/iso26262/`

**Key Features in Version 1.3:**
- ✅ ASIL Determination Table corrected per ISO 26262-3:2018 Clause 6.4.3.10, Table 4
- ✅ Exact clause references for all table citations
- ✅ Controllability classifications refined to match Annex B.4
- ✅ DrivaPi context clarified as prototype with conditional ASIL assignment
- ✅ Data-driven guidance with documentation of rationale (conservative vs. data-driven)
- ✅ Fault injection testing requirements specified for ASIL C/D
- ✅ Version control and change history fields in templates
- ✅ Independence requirements and links to review evidence documented
- ✅ Operational Situations table with direct E-class mapping
- ✅ FMEA reference included as dependency

---

## 10. Citations

[50] LDRA. (2025). "ISO 26262, functional safety, and ASILs."
Retrieved from https://ldra.com/iso-26262/

[51] Innodare Platform. "HARA (Hazard Analysis and Risk Assessment)."
Retrieved from https://www.innodareplatform.com/en/insight/hara-hazard-analysis-and-risk-assessment

[53] Spyro-Soft. (2024). "ISO 26262: The Complete Guide."
Retrieved from https://spyro-soft.com/blog/automotive/iso-26262

[56] Wikipedia. "ISO 26262."
Retrieved from https://en.wikipedia.org/wiki/ISO_26262

[59] LHPES. (2020). "Understanding an ASIL in the Functional Safety Standard ISO 26262."
Retrieved from https://www.lhpes.com/blog/understanding-an-asil-in-the-functional-safety-standard-iso-26262

[61] Infineon Community. (2025). "ASIL decomposition: ISO 26262."
Retrieved from https://community.infineon.com/t5/Knowledge-Base-Articles/ASIL-decomposition-ISO-26262/ta-p/852405

[63] HermesSol. (2024). "Automotive safety standard ISO 26262: Hazard analysis and risk assessment (HARA)."
Retrieved from https://www.hermessol.com/2024/06/17/3_6/

[71] Infocentre. "INTERNATIONAL STANDARD ISO 26262-3:2018."
Retrieved from https://files.infocentre.io/files/docs_clients/126_2008096318_4226751_docu_nt_doga_ISO_26262-3.pdf
**Note:** Sample PDF for research and reference. For official compliance audits, obtain the full ISO 26262-3:2018 standard from ISO or authorized distributors (e.g., ANSI, BSI, or ISO directly).

[72] Visure Solutions. (2023). "Best ISO-26262 Compliance Tools, Checklists & Templates."
Retrieved from https://visuresolutions.com/iso-26262-guide/tools-checklists-templates/

[74] LinkedIn. (2024). "Article 2: Applying ISO 26262 – A Practical Example of Functional Safety."
Retrieved from https://www.linkedin.com/pulse/article-2-applying-iso-26262-practical-example-safety-abdelrhman-0khoc

[76] Jama Software. (2024). "A Guide to Automotive Safety Integrity Levels (ASIL)."
Retrieved from https://www.jamasoftware.com/requirements-management-guide/automotive-engineering/guide-to-automotive-safety-integrity-levels-asil/

---

**Verification Note:** Citations verified as of October 30, 2025. Cross-check external sources for updates periodically. Content from [59] (2020) remains timeless for ASIL basics, but check for 2025+ updates if needed.

---

## 11. Next Steps

1. ✅ **Create `docs/standards/iso26262/` directory** - Structure established, ready for team
2. **Apply HARA template** to DrivaPi features (speed control, emergency stop, temperature monitoring)
3. **Link HARA/ASIL documents** in existing requirements using `type: "compliance"` blocks
4. **Conduct team workshop** on ASIL assignment using DrivaPi examples (reference `docs/tsf/training.md` for alignment)
5. **Establish independent review process** - Define who performs reviews per independence levels (ISO 26262-2:2018, Table 1):
   - **I0 (ASIL B):** Different person, same project
   - **I2 (ASIL C):** Independent from development team
   - **I3 (ASIL D):** Independent from organization
