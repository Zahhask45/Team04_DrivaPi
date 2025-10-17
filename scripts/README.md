# DrivaPi Scripts

Collection of tools and utilities for requirements management, traceability, and TSF compliance.

## 📂 Organization

### 📋 Requirements Tools (`requirements/`)
Tools for managing requirement lifecycle:
- **`new_req.py`** - Create new requirement from template
- **`reqs_lint.py`** - Validate requirement YAML syntax and structure
- **`baseline_reqs.py`** - Create timestamped requirement baselines
- **`export_artifact.py`** - Export requirements as artifacts

**Usage Examples:**
```bash
# Create new requirement
python scripts/requirements/new_req.py urd URD-002 "Display Battery Voltage"

# Validate all requirements
python scripts/requirements/reqs_lint.py

# Create baseline
python scripts/requirements/baseline_reqs.py SWD v1.1
```

### 🔗 Traceability Tools (`traceability/`)
Generate and manage traceability matrices:
- **`traceability_check.py`** - Generate traceability matrices and reports
- **`generate_bidirectional_matrix.py`** - Create bidirectional traceability
- **`get_latest_manifest.sh`** - Retrieve latest traceability manifest

**Usage Examples:**
```bash
# Generate traceability matrix
python scripts/traceability/traceability_check.py

# Get latest manifest
./scripts/traceability/get_latest_manifest.sh
```

### 🔧 Trudag Helpers (`trudag/`)
Trudag integration and Docker wrappers:
- **`run_trudag_docker.sh`** - Run trudag in Docker container
- **`trudag-helper.sh`** - Trudag wrapper with common operations
- **`apply_trudag_shims.py`** - Prepare requirements for trudag
- **`remove_trudag_shims.py`** - Clean up after trudag processing

**Usage Examples:**
```bash
# Run trudag validation
./scripts/trudag/run_trudag_docker.sh validate

# Run trudag report generation
./scripts/trudag/trudag-helper.sh report
```

### 📊 Analysis Tools (`analysis/`)
Analyze requirements and calculate metrics:
- **`tsf_confidence_assessment.py`** - Calculate TSF confidence scores
- **`generate_missing_fields_csv.py`** - Analyze and report missing fields

**Usage Examples:**
```bash
# Generate confidence assessment
python scripts/analysis/tsf_confidence_assessment.py

# Check for missing fields
python scripts/analysis/generate_missing_fields_csv.py
```

### 🔄 CI Tools (`ci/`)
Continuous integration automation:
- **`regenerate_dotstop_dot.py`** - Regenerate Doorstop visualizations

### 🛠️ Utilities (`utils/`)
General-purpose utilities:
- **`print_all_files.sh`** - Print project structure and contents

---

## 🚀 Quick Start

### Daily Validation Workflow
```bash
# 1. Validate requirements
python scripts/requirements/reqs_lint.py

# 2. Check traceability
python scripts/traceability/traceability_check.py

# 3. Review reports
cat artifacts/traceability/report.md
cat artifacts/traceability/gap-report.csv
```

### Creating & Approving Requirements
```bash
# 1. Create new requirement
python scripts/requirements/new_req.py swd SWD-003 "Battery Monitoring"

# 2. Edit requirement (fill in fields)
nano reqs/swd/SWD-003.yml

# 3. Validate
python scripts/requirements/reqs_lint.py

# 4. After review, create baseline
python scripts/requirements/baseline_reqs.py SWD v1.2
```

---

## 📦 Dependencies

- **Python 3.8+**
- **Doorstop** - Requirements management
- **Docker** - For trudag
- **Standard Unix tools** - bash, sed, awk

---

**Last Updated:** October 17, 2025
