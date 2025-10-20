# Requirements Workflow

**Version:** 2.0
**Date:** October 20, 2025

---

## 📋 Official Tools

- ✅ **trudag** - TSF validation, management, and reporting
- ✅ **doorstop** - Structured requirements backend

---

## 🔄 Complete Workflow

### 1️⃣ Create Requirement

#### Option A: Doorstop (Interactive)
```bash
# Add interactive requirement
doorstop add SWD

# Edit created requirement
doorstop edit SWD-002
```

#### Option B: Manual (Copy Template)
```bash
# Copy template
cp reqs/templates/SWD-template.yml reqs/swd/SWD-002.yml

# Edit file
nano reqs/swd/SWD-002.yml

# Fill ALL mandatory fields:
# - ref: SWD-002
# - header: "Title"
# - text: "The system SHALL..."
# - ASIL: B
# - Verification Method: Unit Test
# - links: [SRD-001]
# - reviewers: [name + email]
# - reviewed: ''
```

---

### 2️⃣ Validate Requirement

```bash
# Official TSF validation
trudag manage lint

# If errors, fix and validate again
```

**Common errors:**
- ❌ `ref:` doesn't match filename
- ❌ Missing mandatory fields
- ❌ Links to non-existent requirements
- ❌ Invalid YAML format

---

### 3️⃣ Create Traceability Links

```bash
# Link SWD to SRD (child → parent)
doorstop link SWD-002 SRD-001

# Or edit YAML manually:
# links: [SRD-001]

# Validate links
trudag manage lint
```

**V-Model Hierarchy:**
```
URD (User) → SRD (System) → SWD (Software) → LLTC (Tests)
```

---

### 4️⃣ Review & Approval

#### Review Process

1. **Author creates requirement** → Commit and Push
```bash
git add reqs/swd/SWD-002.yml
git commit -m "feat: Add SWD-002 - Battery monitoring"
git push origin feature/SWD-002
```

2. **Reviewer validates**
```bash
# Pull branch
git pull origin feature/SWD-002

# Validate
trudag manage lint

# Review content manually
cat reqs/swd/SWD-002.yml
```

3. **Official approval (trudag)**
```bash
# Approve requirement (updates 'reviewed' field)
trudag manage set-item reqs/swd/SWD-002.yml

# Commit approval
git add reqs/swd/SWD-002.yml
git commit -m "review: Approve SWD-002"
git push
```

---

### 5️⃣ Baseline (Release)

```bash
# 1. Migrate Doorstop structure
trudag manage migrate

# 2. Validate everything
trudag manage lint

# 3. Create baseline tag
git tag -a BASELINE-SWD-V1.0 -m "SWD baseline v1.0 - approved"
git push origin BASELINE-SWD-V1.0

# 4. Export Trustable report
trudag report export --output artifacts/DrivaPi-trustable-v1.0.zip

# 5. Commit artifacts
git add artifacts/
git commit -m "release: Trustable report v1.0"
git push
```

---

## 📝 Validation Checklist

### Before Commit
- [ ] `trudag manage lint` → No errors
- [ ] Mandatory fields filled
- [ ] `ref:` = filename
- [ ] Correct links (parent exists)
- [ ] Reviewers assigned
- [ ] Text uses "shall" for requirements

### Before Approval
- [ ] Requirement technically correct
- [ ] Testable (clear acceptance criteria)
- [ ] No ambiguities
- [ ] Traceability links verified
- [ ] Correct ASIL

### Before Baseline
- [ ] All requirements approved (`reviewed` != '')
- [ ] `trudag manage lint` → No errors
- [ ] Associated tests created (LLTC)
- [ ] Trustable report exported

---

## 🔧 Commands by Phase

### Initial Setup
```bash
# Install official tools
pip install doorstop pyyaml

# Install Trustable
git clone https://gitlab.com/CodethinkLabs/trustable/trustable.git /tmp/trustable
cd /tmp/trustable && git checkout 2025.9.16 && pip install .
```

### Daily Development
```bash
# Validate requirements
trudag manage lint

# Create requirement
doorstop add SWD

# Link requirements
doorstop link SWD-002 SRD-001

# Approve
trudag manage set-item reqs/swd/SWD-002.yml
```

### Release/Baseline
```bash
# Migrate structure
trudag manage migrate

# Validate all
trudag manage lint

# Tag baseline
git tag -a BASELINE-SWD-V1.0 -m "Baseline"

# Export report
trudag report export --output artifacts/trustable.zip
```

---

## 📊 Doorstop Structure

Each category has a `.doorstop.yml`:

```yaml
# reqs/swd/.doorstop.yml
settings:
  digits: 3
  itemformat: yaml
  parent: SRD        # Parent category
  prefix: SWD        # ID prefix
  sep: '-'
```

---

## 🎯 Complete Example

### 1. Create SWD-002
```bash
cp reqs/templates/SWD-template.yml reqs/swd/SWD-002.yml
```

### 2. Edit SWD-002.yml
```yaml
SWD-002:
  ref: SWD-002
  header: "Battery voltage ADC reading"
  text: |
    The software SHALL read battery voltage via ADC channel 0
    with 12-bit resolution at 10Hz sampling rate.
  ASIL: B
  Verification Method: Unit Test
  links: [SRD-001]
  reviewers:
    - name: "John Silva"
      email: "john@team.com"
  reviewed: ''
  active: true
  derived: false
  normative: true
  level: 1.0
```

### 3. Validate
```bash
trudag manage lint
```

### 4. Commit
```bash
git add reqs/swd/SWD-002.yml
git commit -m "feat: Add SWD-002 - Battery ADC reading"
git push
```

### 5. Approve (after review)
```bash
trudag manage set-item reqs/swd/SWD-002.yml
git add reqs/swd/SWD-002.yml
git commit -m "review: Approve SWD-002"
git push
```

---

## 📖 References

- **Training:** `docs/training/TSF-training.md`
- **Quick Ref:** `docs/training/quick-reference.md`
- **Templates:** `reqs/templates/`
- **Trustable Docs:** https://codethinklabs.gitlab.io/trustable/trustable/

---

**Last Updated:** October 20, 2025
