---

# Sprint 5 Review — Team04 DrivaPi

**Date:** December 22, 2025

**Sprint goal:** _Establish unit testing (with coverage and static analysis) for STM firmware and validate data-pipeline latency using KUKSA VSS._

---

## Table of contents
- Summary
- Key achievements
- Decisions
- Measurements & validation
- Blockers & risks
- Next steps (Sprint 6)
- Team highlights
- Team feedback (retrospective)
- Outcome

---

## Summary
Sprint 5 moved the team to a verification-first approach: an OSS verification stack was stood up, CAN and middleware latency were measured, and a middleware choice was made based on VSS/AGL fit and measured overhead.

## Key achievements
- Verification toolchain: **Unity, Ceedling, CMock** for unit tests; **gcov/lcov** for coverage; **cppcheck** and **CodeQL** for static analysis; **Dependabot** for dependencies.
- CAN latency analysis: **STM32 determinism** (mean ≈ **248 μs**, jitter ≈ **12 μs**); RPi adds scheduling variance; STM32↔RPi mean RTT ≈ **789 μs** with outliers.
- Middleware decision: **KUKSA** chosen (VSS support, AGL integration). Measured overhead on AGL: **CAN direct ≈ 0.148 ms → KUKSA ≈ 2.212 ms** (+≈2.06 ms).
- Rust joystick controller: PoC sending CAN ID 44; cross-compilation for AGL validated.
- CI: CodeQL integrated; High/Medium findings identified and triaged.
- Unit testing: Framework integrated into STM32; motor/servo tests in place; speed sensor tests started (Ceedling).

## Decisions
- Adopt OSS verification stack (CI-friendly, transparent).
- Select KUKSA for VSS/databroker (AGL native support).
- Keep STM32 for safety-critical control loops; consider PREEMPT_RT on RPi if stricter timing needed.

## Measurements & validation
| Path | Mean | Notes |
|------|------:|:-----|
| STM32 internal | ~248 μs | Low jitter — suitable for 10–100 Hz control |
| RPi loopback | ~592 μs | Scheduling variance on Linux |
| STM32 ↔ RPi | ~789 μs (with spikes) | Outliers due to OS scheduling |
| KUKSA on AGL | ~2.212 ms | ~+2.06 ms overhead vs direct CAN — acceptable for telemetry/display |

## Blockers & risks
- STM32 emulation: QEMU lacks STM32U5 peripheral support — CI cannot fully emulate board; using STUB/mocked-peripheral tests as workaround.
- RPi SSD boot issues (U-Boot/USB detection) blocking AGL-from-SSD workflows.
- Missing QtSerialBus on AGL causing Qt CAN failures — requires rebuilding AGL with that module.
- CodeQL noise from ThreadX-generated code — configure exclusions to reduce false positives.

## Next steps (Sprint 6 priorities)
1. Reach ≥80% coverage for STM32 modules; add coverage reports to CI.
2. Fix/triage CodeQL High/Medium findings and exclude ThreadX-generated code from scans.
3. Complete KuksaReader gRPC client and test end-to-end: sensor → CAN → KUKSA → Qt.
4. Test CAN at 1000 kbps and validate latency impact; document decision.
5. Resolve RPi boot issues and rebuild AGL with QtSerialBus.
6. Convert electrical sketches to structured diagrams (KiCAD/Fritzing) and publish pin table.

## Team highlights
- **Hugo:** CAN latency study, CodeQL pipeline, CAN docs, STUB testing approach.
- **Gaspar:** Dependabot/DNF server, Qt cross-compile docs, RPi boot investigation.
- **Miguel:** Unit test integration on STM32, motor/servo tests, I2C3 migration.
- **Bernardo:** CAN pin assignments, electrical sketch, speed sensor tests.
- **Melanie:** KUKSA tests, Yocto/AGL setup, KuksaReader prototype, CI design.

## Team feedback (retrospective)
> The team is proud of the technical results and CI/test achievements. Facilitators praised the demo and the reported 100% coverage during the presentation.

- Several members reported exhaustion after this sprint. Planning currently produces lower measurable output in week 1 (setup, environment, cross-compilation) and much higher output in week 2, causing bursts of high workload.
- Suggested adjustments: rethink sprint planning and story sizing to smooth work (reserve setup tickets, cap WIP, add a mid-sprint check-in), and explicitly consider sustainability when committing stories.

---

## Outcome
Sprint goal achieved: verification tooling and static analysis are in place, CAN latency validated, and KUKSA integration validated for telemetry/display. The team moved from "make it work" to "prove it works." Sprint 6 will focus on coverage, CodeQL follow-ups, and final middleware integration.

**Next review:** Sprint 6 Review (January 19, 2026)
