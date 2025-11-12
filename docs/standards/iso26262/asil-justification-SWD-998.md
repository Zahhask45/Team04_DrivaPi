# ASIL Justification: SWD-998
**Date:** 2025-11-12 | **Author/Reviewer:** Bernardo Esteves/Hugo Lopes | **ASIL:** QM

This document provides the ASIL justification for the software design requirement SWD-998, which pertains to the motor speed sensor driver in the DrivaPi system. The ASIL level assigned to this requirement is QM, based on the hazard analysis and risk assessment conducted in accordance with ISO 26262 standards.


## 1. Requirement Summary
- **ID/Description:** SWD-998 - The software shall read the input from the speed sensor and convert it to RPM.
- **Related HARA/Safety Goal:** hara-motor-speed.md; SG-01 (ASIL QM)

## 2. ASIL Assignment & Decomposition
- **Assigned ASIL:** QM - Derived from HARA (S1 + E2 + C1 = ASIL QM)
- **Justification:** This requirement implements motor speed monitoring addressing hazard H-01 and H-02.

## 3. Risk Assessment Summary
| Factor | Classification | Justification/Evidence |
|:---|:---|:---|
| Severity (S) | S1 | Motor overheats - minor burn risk |
| Exposure (E) | E2 | Extended tests ~2-5% of operation |
| Controllability (C) | C1 | Operator observes and stops immediately |

## 4. Verification Requirements (ASIL A)
- **Unit/Integration Testing:** 5 unit tests (Target: ≥80% coverage)
- **Static Analysis:** cppcheck: 0 errors
- **Code Review:** 1 independent reviewer (Hugo Lopes)

## 5. Review Notes & Dependencies
- **Dependencies:** GPIO driver, emergency stop system
- **Review:** <Reviewer> approved YYYY-MM-DD (PR #<Number>)

**References:** ISO 26262-9:2018
