#!/bin/bash
# reorganize.sh - Automated DrivaPi project reorganization
# Version: 1.0
# Date: October 17, 2025

set -e  # Exit on error

echo "════════════════════════════════════════════════════════════════"
echo "  DrivaPi Project Reorganization Script"
echo "════════════════════════════════════════════════════════════════"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Safety check
if [ ! -d "reqs" ] || [ ! -d "docs" ]; then
    echo -e "${RED}ERROR: Not in DrivaPi project root!${NC}"
    echo "Please run this script from the project root directory."
    exit 1
fi

# Confirm before proceeding
echo -e "${YELLOW}⚠️  This will reorganize your project structure.${NC}"
echo "A backup branch will be created first."
echo ""
read -p "Continue? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Aborted."
    exit 1
fi

# Create backup branch
echo ""
echo -e "${BLUE}📦 Creating backup branch...${NC}"
CURRENT_BRANCH=$(git branch --show-current)
BACKUP_BRANCH="backup-before-reorganization-$(date +%Y%m%d-%H%M%S)"
git branch "$BACKUP_BRANCH"
echo -e "${GREEN}✓${NC} Backup branch created: $BACKUP_BRANCH"

# Create reorganization branch
REORG_BRANCH="reorganize-project-structure"
if git show-ref --verify --quiet "refs/heads/$REORG_BRANCH"; then
    echo -e "${YELLOW}⚠️  Branch $REORG_BRANCH already exists${NC}"
    read -p "Delete and recreate? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        git branch -D "$REORG_BRANCH"
    else
        echo "Aborted."
        exit 1
    fi
fi
git checkout -b "$REORG_BRANCH"
echo -e "${GREEN}✓${NC} Working on branch: $REORG_BRANCH"

echo ""
echo -e "${BLUE}📁 Phase 1: Creating new directory structure...${NC}"
mkdir -p config
mkdir -p docs/{guides/{requirements,traceability,ci-cd},training,templates,reference,team/{standups,assessments},archive}
mkdir -p scripts/{requirements,traceability,trudag,analysis,ci,utils}
mkdir -p artifacts/{baselines,reports}
mkdir -p tests/{unit,integration,specs}
echo -e "${GREEN}✓${NC} Directory structure created"

echo ""
echo -e "${BLUE}⚙️  Phase 2: Moving configuration files...${NC}"
if [ -f "traceability_patterns.json" ]; then
    mv traceability_patterns.json config/
    echo -e "${GREEN}✓${NC} Moved traceability_patterns.json"
else
    echo -e "${YELLOW}⚠${NC}  traceability_patterns.json not found (may already be moved)"
fi

echo ""
echo -e "${BLUE}📦 Phase 3: Organizing artifacts...${NC}"

# Move baselines
if ls artifacts/DrivaPi-requirements-BASELINE-*.tar.gz 1> /dev/null 2>&1; then
    mv artifacts/DrivaPi-requirements-BASELINE-*.tar.gz artifacts/baselines/ 2>/dev/null || true
    echo -e "${GREEN}✓${NC} Moved baseline archives"
fi

# Move reports
if [ -d "reports" ]; then
    if [ "$(ls -A reports)" ]; then
        mv reports/* artifacts/reports/
        echo -e "${GREEN}✓${NC} Moved reports"
    fi
    rmdir reports 2>/dev/null || true
fi

# Consolidate traceability artifacts
cd artifacts
if [ -f "traceability-manifest.json" ]; then
    rm -f traceability-manifest.json
fi
if ls traceability-matrix*.csv 1> /dev/null 2>&1; then
    rm -f traceability-matrix*.csv
fi
if [ -f "traceability-report.md" ]; then
    rm -f traceability-report.md
fi
if [ -f "traceability.txt" ]; then
    rm -f traceability.txt
fi
echo -e "${GREEN}✓${NC} Cleaned duplicate traceability files"

# Rename files in traceability/ for clarity
if [ -d "traceability" ]; then
    cd traceability
    [ -f "traceability-manifest.json" ] && mv traceability-manifest.json manifest.json
    [ -f "traceability-matrix.csv" ] && mv traceability-matrix.csv matrix.csv
    [ -f "traceability-matrix-forward.csv" ] && mv traceability-matrix-forward.csv matrix-forward.csv
    [ -f "traceability-matrix-backward.csv" ] && mv traceability-matrix-backward.csv matrix-backward.csv
    [ -f "traceability-report.md" ] && mv traceability-report.md report.md
    echo -e "${GREEN}✓${NC} Renamed traceability files"
    cd ..
fi
cd ..

echo ""
echo -e "${BLUE}🛠️  Phase 4: Organizing scripts...${NC}"

# Requirements scripts
[ -f "scripts/new_req.py" ] && mv scripts/new_req.py scripts/requirements/ && echo "  → new_req.py"
[ -f "scripts/reqs_lint.py" ] && mv scripts/reqs_lint.py scripts/requirements/ && echo "  → reqs_lint.py"
[ -f "scripts/baseline_reqs.py" ] && mv scripts/baseline_reqs.py scripts/requirements/ && echo "  → baseline_reqs.py"
[ -f "scripts/export_artifact.py" ] && mv scripts/export_artifact.py scripts/requirements/ && echo "  → export_artifact.py"
[ -f "scripts/REQS_LINTER_README.md" ] && mv scripts/REQS_LINTER_README.md scripts/requirements/README.md && echo "  → REQS_LINTER_README.md"

# Traceability scripts
[ -f "scripts/traceability_check.py" ] && mv scripts/traceability_check.py scripts/traceability/ && echo "  → traceability_check.py"
[ -f "scripts/generate_bidirectional_matrix.py" ] && mv scripts/generate_bidirectional_matrix.py scripts/traceability/ && echo "  → generate_bidirectional_matrix.py"
[ -f "scripts/get_latest_traceability_manifest.sh" ] && mv scripts/get_latest_traceability_manifest.sh scripts/traceability/get_latest_manifest.sh && echo "  → get_latest_manifest.sh"

# Trudag scripts
[ -f "scripts/run_trudag_docker.sh" ] && mv scripts/run_trudag_docker.sh scripts/trudag/ && echo "  → run_trudag_docker.sh"
[ -f "scripts/trudag-helper.sh" ] && mv scripts/trudag-helper.sh scripts/trudag/ && echo "  → trudag-helper.sh"
[ -f "scripts/apply_trudag_shims.py" ] && mv scripts/apply_trudag_shims.py scripts/trudag/ && echo "  → apply_trudag_shims.py"
[ -f "scripts/remove_trudag_shims.py" ] && mv scripts/remove_trudag_shims.py scripts/trudag/ && echo "  → remove_trudag_shims.py"

# Analysis scripts
[ -f "scripts/tsf_confidence_assessment.py" ] && mv scripts/tsf_confidence_assessment.py scripts/analysis/ && echo "  → tsf_confidence_assessment.py"
[ -f "scripts/generate_missing_fields_csv.py" ] && mv scripts/generate_missing_fields_csv.py scripts/analysis/ && echo "  → generate_missing_fields_csv.py"

# CI scripts
[ -f "scripts/regenerate_dotstop_dot.py" ] && mv scripts/regenerate_dotstop_dot.py scripts/ci/ && echo "  → regenerate_dotstop_dot.py"

# Utilities
[ -f "scripts/print_all_files.sh" ] && mv scripts/print_all_files.sh scripts/utils/ && echo "  → print_all_files.sh"

echo -e "${GREEN}✓${NC} Scripts organized"

echo ""
echo -e "${BLUE}📚 Phase 5: Consolidating documentation...${NC}"

# Training materials
echo "  Moving training materials..."
[ -f "docs/tsf/TSF-training.md" ] && mv docs/tsf/TSF-training.md docs/training/ && echo "    → TSF-training.md"
[ -f "docs/tsf/TSF-training-slides.md" ] && mv docs/tsf/TSF-training-slides.md docs/training/ && echo "    → TSF-training-slides.md"
[ -f "docs/implementation/TSF-Team-Assessment.md" ] && mv docs/implementation/TSF-Team-Assessment.md docs/training/assessment.md && echo "    → assessment.md"
[ -f "docs/implementation/TSF-Quick-Reference.md" ] && mv docs/implementation/TSF-Quick-Reference.md docs/training/quick-reference.md && echo "    → quick-reference.md"
[ -f "docs/tsf/TSF-hands-on-lab.md" ] && mv docs/tsf/TSF-hands-on-lab.md docs/training/hands-on-lab.md && echo "    → hands-on-lab.md"
[ -f "docs/tsf/TSF-assessment-rubric.md" ] && mv docs/tsf/TSF-assessment-rubric.md docs/training/assessment-rubric.md && echo "    → assessment-rubric.md"

# Archive duplicates
echo "  Archiving duplicate files..."
[ -f "docs/tsf/TSF-quiz.md" ] && mv docs/tsf/TSF-quiz.md docs/archive/ && echo "    → TSF-quiz.md (superseded)"
[ -f "docs/tsf/TSF-quick-reference.md" ] && mv docs/tsf/TSF-quick-reference.md docs/archive/ && echo "    → TSF-quick-reference.md (superseded)"

# Requirements guides
echo "  Moving requirements guides..."
[ -f "docs/requirements/TSF_REQUIREMENTS.md" ] && mv docs/requirements/TSF_REQUIREMENTS.md docs/guides/requirements/workflow.md && echo "    → workflow.md"
[ -f "docs/requirements/REVIEWER_CHECKLIST.md" ] && mv docs/requirements/REVIEWER_CHECKLIST.md docs/guides/requirements/review-checklist.md && echo "    → review-checklist.md"
[ -f "docs/requirements/TRUDAG_RUN.md" ] && mv docs/requirements/TRUDAG_RUN.md docs/guides/requirements/trudag-usage.md && echo "    → trudag-usage.md"

# Traceability guides
echo "  Moving traceability guides..."
[ -f "docs/tsf/TRACEABILITY_MATRIX_SPEC.md" ] && mv docs/tsf/TRACEABILITY_MATRIX_SPEC.md docs/guides/traceability/matrix-spec.md && echo "    → matrix-spec.md"
[ -f "docs/tsf/TRACEABILITY_CHANGELOG.md" ] && mv docs/tsf/TRACEABILITY_CHANGELOG.md docs/guides/traceability/changelog.md && echo "    → changelog.md"
[ -f "docs/tsf/TRACEABILITY_README.md" ] && mv docs/tsf/TRACEABILITY_README.md docs/guides/traceability/README.md && echo "    → README.md"
[ -f "docs/tsf/TSF-traceability-guide.md" ] && mv docs/tsf/TSF-traceability-guide.md docs/guides/traceability/guide.md && echo "    → guide.md"

# CI/CD guides
echo "  Moving CI/CD guides..."
[ -f "docs/tsf/CI_ARTIFACT_RETENTION.md" ] && mv docs/tsf/CI_ARTIFACT_RETENTION.md docs/guides/ci-cd/artifact-retention.md && echo "    → artifact-retention.md"

# Reference materials
echo "  Moving reference materials..."
[ -f "docs/tsf/TSF-overview.md" ] && mv docs/tsf/TSF-overview.md docs/reference/tsf-overview.md && echo "    → tsf-overview.md"
[ -f "docs/tsf/SOURCES.md" ] && mv docs/tsf/SOURCES.md docs/reference/sources.md && echo "    → sources.md"
[ -f "docs/tsf/VV-plan.md" ] && mv docs/tsf/VV-plan.md docs/reference/vv-plan.md && echo "    → vv-plan.md"

# Templates
echo "  Moving templates..."
[ -f "docs/tsf/TSF-requirement-template.md" ] && mv docs/tsf/TSF-requirement-template.md docs/templates/requirement-template.md && echo "    → requirement-template.md"

# Team materials
echo "  Moving team materials..."
if [ -d "docs/standups" ]; then
    if [ "$(ls -A docs/standups)" ]; then
        mv docs/standups/* docs/team/standups/
    fi
    rmdir docs/standups 2>/dev/null || true
    echo "    → standups/"
fi

# Archive old workflow docs
echo "  Archiving old workflow docs..."
[ -f "docs/tsf/REQS_WORKFLOW.md" ] && mv docs/tsf/REQS_WORKFLOW.md docs/archive/ && echo "    → REQS_WORKFLOW.md (superseded)"
[ -f "docs/tsf/REQ_REVIEW_CHECKLIST.md" ] && mv docs/tsf/REQ_REVIEW_CHECKLIST.md docs/archive/ && echo "    → REQ_REVIEW_CHECKLIST.md (superseded)"

echo -e "${GREEN}✓${NC} Documentation consolidated"

echo ""
echo -e "${BLUE}🧹 Phase 6: Cleaning up empty directories...${NC}"
rmdir docs/requirements 2>/dev/null && echo "  → Removed docs/requirements" || true
rmdir docs/tsf 2>/dev/null && echo "  → Removed docs/tsf" || true
echo -e "${GREEN}✓${NC} Cleanup complete"

echo ""
echo -e "${BLUE}📝 Phase 7: Creating README files...${NC}"

# Create docs/README.md
cat > docs/README.md << 'EOF'
# DrivaPi Documentation

## 📚 Quick Navigation

### 🎓 Training Materials (`training/`)
Start here if you're new to the project or TSF:
- **[TSF Training](training/TSF-training.md)** - Complete TSF training course
- **[Quick Reference](training/quick-reference.md)** - Daily reference card
- **[Team Assessment](training/assessment.md)** - Quiz & lab exercises
- **[Hands-on Lab](training/hands-on-lab.md)** - Practical exercises
- **[Assessment Rubric](training/assessment-rubric.md)** - Grading criteria

### 📖 Process Guides (`guides/`)
Day-to-day workflow documentation:

**Requirements Management:**
- [Requirements Workflow](guides/requirements/workflow.md)
- [Review Checklist](guides/requirements/review-checklist.md)
- [Trudag Usage](guides/requirements/trudag-usage.md)

**Traceability:**
- [Traceability Guide](guides/traceability/guide.md)
- [Matrix Specification](guides/traceability/matrix-spec.md)
- [Changelog](guides/traceability/changelog.md)

**CI/CD:**
- [Artifact Retention](guides/ci-cd/artifact-retention.md)

### 🚀 Implementation Guides (`implementation/`)
Guides for completing TSF implementation:
- [Artifact Linking Guide](implementation/artifact-linking.md)
- [Review & Approval Workflow](implementation/review-workflow.md)
- [3-Week Completion Plan](implementation/completion-plan.md)

### 📚 Reference Materials (`reference/`)
Background and reference information:
- [TSF Overview](reference/tsf-overview.md)
- [V&V Plan](reference/vv-plan.md)
- [Sources & Bibliography](reference/sources.md)

### 📋 Templates (`templates/`)
Document templates for creating new artifacts:
- [Requirement Template](templates/requirement-template.md)

### 👥 Team Resources (`team/`)
Team-specific materials:
- **Standups:** Daily standup notes
- **Assessments:** Team assessment results

---

## 🔍 Finding What You Need

**If you want to...**
- Learn TSF from scratch → Start with [TSF Training](training/TSF-training.md)
- Create a requirement → Check [Quick Reference](training/quick-reference.md)
- Review a requirement → Follow [Review Checklist](guides/requirements/review-checklist.md)
- Understand traceability → Read [Traceability Guide](guides/traceability/guide.md)
- Complete TSF implementation → Follow [Completion Plan](implementation/completion-plan.md)

---

**Last Updated:** October 17, 2025
EOF
echo "  → docs/README.md"

# Create scripts/README.md
cat > scripts/README.md << 'EOF'
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
EOF
echo "  → scripts/README.md"

# Create artifacts/README.md
cat > artifacts/README.md << 'EOF'
# DrivaPi Artifacts

Auto-generated artifacts from requirements management and traceability processes.

## 📦 Directory Structure

### 📚 Baselines (`baselines/`)
Timestamped archives of approved requirements at specific points in time.

**Format:** `DrivaPi-requirements-BASELINE-{CATEGORY}-{VERSION}-{TIMESTAMP}.tar.gz`

**Example:** `DrivaPi-requirements-BASELINE-SWD-V1.0-20251015T141446Z.tar.gz`

**Usage:**
- Provides snapshot for auditing
- Enables rollback if needed
- Required for ISO 26262 compliance

### 🔗 Traceability (`traceability/`)
Auto-generated traceability matrices and gap reports.

**Files:**
- `manifest.json` - Metadata about generation (timestamp, file count, etc.)
- `matrix.csv` - Consolidated traceability matrix (all links)
- `matrix-forward.csv` - Requirements → Implementation (forward trace)
- `matrix-backward.csv` - Implementation → Requirements (backward trace)
- `matrix-urd.csv` - User requirements traceability
- `matrix-srd.csv` - System requirements traceability
- `matrix-swd.csv` - Software requirements traceability
- `matrix-lltc.csv` - Test case traceability
- `gap-report.csv` - Identifies missing links and orphaned requirements
- `report.md` - Human-readable summary report

**Generated by:**
```bash
python scripts/traceability/traceability_check.py
```

**Regenerated:** On every commit via CI/CD

### 📊 Reports (`reports/`)
Analysis reports and compliance assessments.

**Files:**
- `tsf-compliance.md` - TSF compliance status report
- `confidence-assessment.json` - Confidence scores per requirement
- `missing-fields.csv` - Requirements with incomplete fields
- `remediation.md` - Recommendations for improvements

**Generated by:**
```bash
python scripts/analysis/tsf_confidence_assessment.py
python scripts/analysis/generate_missing_fields_csv.py
```

---

## 🔄 Artifact Lifecycle

1. **Requirements created/updated** → Committed to `reqs/`
2. **CI runs validation** → Linter checks, Doorstop validation
3. **Traceability generated** → Matrices created in `artifacts/traceability/`
4. **Reports updated** → Analysis reports in `artifacts/reports/`
5. **Baseline created** (on approval) → Archive in `artifacts/baselines/`

---

## ⚠️ Important Notes

- **Do NOT commit artifacts to git** (except baselines)
  - Add to `.gitignore`: `artifacts/traceability/*`, `artifacts/reports/*`
  - Keep baselines in git for audit trail

- **Artifacts are ephemeral** - Regenerated on each CI run
- **Baselines are permanent** - Timestamped snapshots

---

## 🔍 Checking Artifacts

### View Traceability Status
```bash
# Quick summary
cat artifacts/traceability/report.md

# Check for gaps
cat artifacts/traceability/gap-report.csv

# View full matrix
cat artifacts/traceability/matrix.csv
```

### Check Compliance
```bash
# TSF compliance report
cat artifacts/reports/tsf-compliance.md

# Missing fields
cat artifacts/reports/missing-fields.csv
```

---

**Last Updated:** October 17, 2025
EOF
echo "  → artifacts/README.md"

echo -e "${GREEN}✓${NC} README files created"

echo ""
echo "════════════════════════════════════════════════════════════════"
echo -e "${GREEN}✅ Reorganization Complete!${NC}"
echo "════════════════════════════════════════════════════════════════"
echo ""
echo -e "${BLUE}📊 Summary:${NC}"
echo "  • Created new organized directory structure"
echo "  • Moved and renamed files for clarity"
echo "  • Removed duplicate traceability files"
echo "  • Consolidated documentation"
echo "  • Organized scripts by purpose"
echo "  • Created README files for navigation"
echo ""
echo -e "${YELLOW}⚠️  Next Steps:${NC}"
echo ""
echo "1. ${BLUE}Review changes:${NC}"
echo "   git status"
echo "   git diff --stat"
echo ""
echo "2. ${BLUE}Update script paths in CI:${NC}"
echo "   Edit .github/workflows/reqs-checks.yml"
echo "   Update script paths (e.g., scripts/requirements/reqs_lint.py)"
echo ""
echo "3. ${BLUE}Update hardcoded paths in scripts:${NC}"
echo "   grep -r 'traceability_patterns.json' scripts/"
echo "   Update to 'config/traceability_patterns.json'"
echo ""
echo "4. ${BLUE}Test that everything still works:${NC}"
echo "   python scripts/requirements/reqs_lint.py"
echo "   python scripts/traceability/traceability_check.py"
echo ""
echo "5. ${BLUE}Update main README.md:${NC}"
echo "   Update documentation links to new structure"
echo ""
echo "6. ${BLUE}Commit changes:${NC}"
echo "   git add -A"
echo "   git commit -m 'refactor: Reorganize project structure for clarity'"
echo ""
echo "7. ${BLUE}Push and create PR:${NC}"
echo "   git push origin $REORG_BRANCH"
echo "   # Create PR on GitHub"
echo ""
echo -e "${GREEN}Backup branch preserved:${NC} $BACKUP_BRANCH"
echo "If anything goes wrong, you can restore with:"
echo "  git checkout $BACKUP_BRANCH"
echo ""
echo "════════════════════════════════════════════════════════════════"