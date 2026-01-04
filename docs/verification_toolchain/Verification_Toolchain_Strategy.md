# Software Verification Toolchain Strategy: OSS Selection for AGL and ISO 26262 Compliance

This document summarizes the technical analysis and justifies the final selection of our Open-Source Software (OSS) verification toolchain for the Team04_DrivaPi project. Our strategy is built on maximizing technical efficiency, leveraging native language features (C and Rust), and centralizing reporting via GitHub Actions and SonarQube.

## I. Strategic Decision: Why Open-Source over Commercial Solutions

The project chose to base its verification and validation (V&V) activities on robust OSS rather than adopting proprietary, licensed toolchains (e.g., VectorCAST, Parasoft, Helix QAC, Polyspace).

### 1. Rationale for Discarding Commercial Tools

Commercial solutions are known for providing comprehensive features like pre-certified Tool Qualification Kits for ISO 26262 and built-in support for metrics like MC/DC (Modified Condition/Decision Coverage). However, these solutions were discarded for the following reasons:

- **Licensing Cost**: Commercial frameworks impose high, recurring licensing fees, which constrain resource allocation during development.
- **Integration Complexity**: Large, proprietary toolchains can often be rigid and challenging to integrate seamlessly into a highly customized Linux/Yocto (AGL) build environment, contrasting with the modularity of OSS.
- **Vendor Lock-in**: The project prioritizes flexibility and long-term control over the entire toolchain stack, which is inherent in the OSS model.

### 2. Rationale for Selecting the OSS Toolchain

The selected OSS toolchain ensures high agility, low resource overhead, and strong technical coherence between languages:

- **Native Efficiency**: Choosing language-native tools (Unity for C, cargo for Rust) minimizes external dependencies and runtime overhead in embedded contexts.
- **Focused Analysis**: The strategy centralizes the most demanding task—Static Analysis—in the high-powered CodeQL engine, ensuring uniform security enforcement across both C and Rust.

## II. Comparative Analysis and Tool Selections

### A. Unit Testing Framework & Mocking

The primary goal of unit testing is host-based testing to ensure deterministic execution and isolation from hardware.

| Tool | Code Base | Why Selected | Discarded OSS Alternative |
|------|-----------|--------------|---------------------------|
| Unity | C (Pure) | Superior for Embedded C. Unity is pure ANSI C and has a minimal footprint, making it the most suitable choice for low-level testing (LLT) of drivers and application logic. | Google Test (GTest): Discarded for C pure modules. GTest is a C++ framework, requiring the C++ runtime and compiler, which adds significant overhead and complexity to testing pure C code. |
| CMock | C | Automatically generates pure C mocks from headers. Essential for decoupling the ThreadX RTOS APIs and hardware drivers, allowing the application logic to run purely on the Linux host. | N/A |
| cargo test | Rust | Native and Integrated. The built-in Rust testing solution, perfectly integrated with the cargo build manager, promoting efficiency and speed. | N/A |

### B. Static Analysis & Security (SAST)

Our Static Analysis strategy is built on depth and unified standards enforcement, which is critical for ISO 26262 compliance.

| Tool | Function | Why Selected for AGL/Compliance | Complementary Tool |
|------|----------|----------------------------------|-------------------|
| CodeQL | Deep SAST & Standards Enforcement (C, C++, Rust) | Selected as the Core Security Platform. CodeQL provides deep flow-of-data analysis and supports specialized query packs for critical automotive standards like MISRA C/C++ and AUTOSAR C++14. This capability is essential for formal compliance gating on every Pull Request. | Clang-Tidy / Clippy: Used as developer-facing linters for basic quality checks and enforcing idiomatic practices (Clippy for Rust). |

### C. Code Coverage & Quality Gates

The goal is to capture high-rigor structural coverage metrics using OSS tools that produce the universal Linux Coverage Visualizer (LCOV) format.

| Tool | Language | Rationale | Automotive Standard Alignment |
|------|----------|-----------|-------------------------------|
| gcovr/lcov | C/C++ | Standard Metric Generation. Used to process execution data (.gcda files) and generate the industry-standard LCOV report. | Branch Coverage: We implement Branch Coverage (via gcovr --decisions) as the baseline structural metric, which is a key step towards satisfying the higher-rigor Modified Condition/Decision Coverage (MC/DC) required by ASIL-D. |
| grcov | Rust | LCOV Aggregation. Processes Low Level Virtual Machine (LLVM) profile data (.profraw) and aggregates the results into the LCOV format, ensuring compatibility with the C coverage reports and the SonarQube dashboard. | |

## III. Project Architecture and CI/CD Implementation

### Project Architecture Decisions

The repository (/src, /lib/threadx, /test) is structured to support Requirements-Based Testing (RBT), where tests verify specific requirements and not just random functionality

### CI/CD Integration and Reporting

The seamless integration of the entire OSS toolchain is achieved using Ceedling (for C orchestration) and the SonarQube Community Edition as the central quality dashboard.

- **Static Analysis Gating**: CodeQL is executed in GitHub Actions to scan the codebase. Its results (in Static Analysis Results Interchange Format (SARIF) format) are uploaded to SonarQube to enforce the quality gate, blocking any Pull Request that introduces new security vulnerabilities or major standards violations.
- **Coverage Unification**: The Sonar Scanner CLI is configured to accept multiple LCOV reports (from gcovr and grcov). This allows the project to present a single, consolidated view of code coverage across both the C and Rust codebases.
- **Automation**: The use of simple shell scripting automates the complex gcovr steps, ensuring the continuous integration pipeline delivers consistent and repeatable results for V&V evidence.
