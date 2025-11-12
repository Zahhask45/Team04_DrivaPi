

The Hazard Analysis and Risk Assessment (HARA) for the motor speed sensor requirement **SWD-998** identifies potential hazards, classifies their severity, exposure, and controllability, and determines the Automotive Safety Integrity Level (ASIL) for the requirement.

# HARA: Motor Speed Sensor

**Date:** 2025-11-06| **Authors/Reviewers:** Bernardo/Hugo | **ASIL Target:** QM

## 1. Item Definition & Assumptions
- **Description/Boundaries/Interfaces:** Motor speed monitoring system for DrivaPi lab testing.
- **Assumptions:** Trained operators only; controlled lab environment.

## 2. Hazards & Operational Situations
| ID | Hazard Description | Operational Situation | Exposure (E) / Justification |
|:---|:---|:---|:---|
| H-01 | Sensor fails silently | Extended motor test | E2 (~2-5% of tests; historical data) |
| H-02 | Sensor uncalibrated | Extended motor test | E1 (~0.1-0.5% of tests; historical data) |

## 3. Risk Assessment & ASIL
- **Severity (S):** S1 - Motor overheats, minor burn risk if touched. Overspeed can happen.
- **Controllability (C):** C1 - Operator observes smoke/noise, presses emergency stop.
- **ASIL Calculation:** S1 + E2 + C1 = **QM** (from ISO 26262-3:2018 Table 4)

## 4. Safety Goals
| Goal ID | ASIL | Description | Safe State |
|:---|:---|:---|:---|
| SG-01 | QM | Motor speed sensor shall function reliably | Operator can monitor and stop motor |

## 5. Dependencies & Review Notes
- **Dependencies:** Emergency stop button, GPIO hardware driver
- **Review:** 2025-11-12 - Approved by Hugo Lopes hugo.lopes@seame.pt

**References:** ISO 26262-3:2018.
