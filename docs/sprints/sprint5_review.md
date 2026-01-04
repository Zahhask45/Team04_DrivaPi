# Sprint 5 Review - Team04 DrivaPi
## Sprint 5 Review — Team04 DrivaPi

Date: December 22, 2025

Sprint goal: establish unit testing (with coverage and static analysis) for STM firmware and validate the data pipeline latency using KUKSA VSS.

Summary
-------
Sprint 5 shifted the team to a verification-first mindset. We stood up an open-source verification toolchain, measured CAN and middleware latency, and made an evidence-based middleware choice. The focus was on reproducible verification, determinism, and standards alignment (ISO 26262, VSS).

Key achievements
----------------
- Verification toolchain: Unity + Ceedling + CMock for unit tests; gcov/lcov for coverage; cppcheck and CodeQL for static analysis; Dependabot for dependency updates.
- CAN latency analysis: STM32 shows excellent determinism (mean ~248 μs, jitter ~12 μs). RPi adds scheduling variance; STM32↔RPi mean RTT ~789 μs with outliers due to Linux.
- Middleware decision: KUKSA selected for VSS support and AGL integration. Measured overhead on AGL: CAN direct ~0.148 ms vs KUKSA ~2.212 ms (≈+2.06 ms) — acceptable for display/telemetry.
- Rust joystick controller: Proof-of-concept sending CAN ID 44 established and cross-compiled for AGL.
- CI: CodeQL integrated in GitHub Actions; High/Medium warnings found and triage underway.
- Unit testing progress: Framework integrated into STM32; motor/servo tests developed; speed sensor tests started (Ceedling).

Decisions
---------
- Adopt OSS verification stack (cost-effective, CI-friendly).
- Select KUKSA for VSS/databroker (AGL support, standardization).
- Use STM32 for safety-critical control loops; consider PREEMPT_RT on RPi if needed.

Measurements & validation
-------------------------
- STM32 internal RTT mean ≈ 247.8 μs (low jitter — suitable for 10–100 Hz control).
- RPi loopback mean ≈ 592 μs; STM32↔RPi mean ≈ 789 μs (Linux scheduling causes spikes).
- KUKSA overhead on AGL: ~2.06 ms added vs direct CAN — acceptable for non-critical paths.

Blockers and technical risks
---------------------------
- STM32 emulation: QEMU does not support STM32U5 peripherals — CI cannot fully emulate board; working around with STUB/mocked-peripheral tests.
- RPi SSD boot (U-Boot/USB detection) blocking AGL-from-SSD workflows.
- Missing QtSerialBus on AGL causing Qt CAN failures — requires rebuilding AGL with that module.
- CodeQL noise from ThreadX-generated code — configure exclusions to reduce false positives.

Next steps (Sprint 6 priorities)
------------------------------
1. Reach ≥80% coverage for STM32 modules; add coverage reports to CI.
2. Fix/triage CodeQL High/Medium findings and exclude ThreadX generated code from scans.
3. Finish KuksaReader gRPC client and test end-to-end: sensor → CAN → KUKSA → Qt.
4. Test CAN at 1000 kbps and validate latency impact; document decision.
5. Resolve RPi boot and rebuild AGL with QtSerialBus.
6. Convert electrical sketches to structured diagrams (KiCAD/Fritzing) and publish pin table.

Team highlights
---------------
- Hugo: CAN latency study, CodeQL pipeline, CAN docs, STUB testing approach.
- Gaspar: Dependabot/DNF server, Qt cross-compile docs, RPi boot investigation.
- Miguel: Unit test integration on STM32, motor/servo tests, I2C3 migration.
- Bernardo: CAN pin assignments, electrical sketch, speed sensor tests.
- Melanie: KUKSA tests, Yocto/AGL setup, KuksaReader prototype, CI design.

Outcome
-------
Sprint goal achieved: verification tooling and static analysis in place, CAN latency validated, and KUKSA integration validated for telemetry/display. The team moved from "make it work" to "prove it works." Ready to focus on coverage, remaining CodeQL issues, and final middleware integration in Sprint 6.

Document prepared: December 22, 2025
Next review: January 5, 2026


---

**Document prepared on:** December 22, 2025
**Next review:** Sprint 6 Review (January 5, 2026)
