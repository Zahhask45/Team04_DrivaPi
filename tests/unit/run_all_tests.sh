#!/bin/bash
################################################################################
# Master Unit Test Automation Script with Aggregated Coverage
# 
# Purpose: Execute all unit tests and generate unified coverage report
# ASIL Level: B/D
# Author: DrivaPi Team
# Version: 2.1.0
# 
# Features:
# - Run motor_servo and speed_sensor tests sequentially
# - Aggregate LCOV coverage reports into single report (source only)
# - Overall test result summary
# - Color-coded output
################################################################################

set -e
set -u
set -o pipefail

readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly MOTOR_SERVO_DIR="${SCRIPT_DIR}/motor_servo"
readonly SPEED_SENSOR_DIR="${SCRIPT_DIR}/speed_sensor"

MASTER_COVERAGE_DIR="${COVERAGE_DIR:-${SCRIPT_DIR}/coverage}"

mkdir -p "${MASTER_COVERAGE_DIR}"

# Colors
if [[ -t 1 ]]; then
    readonly RED='\033[0;31m'
    readonly GREEN='\033[0;32m'
    readonly YELLOW='\033[1;33m'
    readonly BLUE='\033[0;34m'
    readonly CYAN='\033[0;36m'
    readonly BOLD='\033[1m'
    readonly NC='\033[0m'
else
    readonly RED='' GREEN='' YELLOW='' BLUE='' CYAN='' BOLD='' NC=''
fi

log_section() {
    echo -e "${BOLD}${BLUE}============================================================================${NC}"
    echo -e "${BOLD}${BLUE} $1${NC}"
    echo -e "${BOLD}${BLUE}============================================================================${NC}"
}

log_pass() {
    echo -e "${GREEN}✓ $1${NC}"
}

log_fail() {
    echo -e "${RED}✗ $1${NC}"
}

log_info() {
    echo -e "${CYAN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

MOTOR_SERVO_PASSED=0
SPEED_SENSOR_PASSED=0

echo ""
log_section "ISO 26262 Master Unit Test Runner - DrivaPi (v2.1 - Unified Coverage)"
echo ""

# ===== Run Motor Servo Tests =====
log_section "Running Motor Servo Tests"
echo ""

if [[ -d "${MOTOR_SERVO_DIR}" ]] && cd "${MOTOR_SERVO_DIR}" && ./scripts/run_tests.sh; then
    log_pass "Motor Servo tests PASSED"
    MOTOR_SERVO_PASSED=1
else
    log_fail "Motor Servo tests FAILED"
fi

echo ""

# ===== Run Speed Sensor Tests =====
log_section "Running Speed Sensor Tests"
echo ""

if [[ -d "${SPEED_SENSOR_DIR}" ]] && cd "${SPEED_SENSOR_DIR}" && CALLED_FROM_MASTER=1 ./run_speedtest.sh; then
    log_pass "Speed Sensor tests PASSED"
    SPEED_SENSOR_PASSED=1
else
    log_fail "Speed Sensor tests FAILED"
fi

echo ""

# ===== Aggregate Coverage =====
if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    log_section "Aggregating Coverage Reports"
    echo ""
    
    MOTOR_SERVO_COVERAGE="${MOTOR_SERVO_DIR}/build/artifacts/gcov/coverage_filtered.info"
    SPEED_SENSOR_COVERAGE="${SPEED_SENSOR_DIR}/build/artifacts/gcov/coverage_filtered.info"
    
    # Check persistent locations (saved outside the build/ directory)
    PERSISTENT_MOTOR_SERVO="${SCRIPT_DIR}/../../build/coverage/motor_servo/coverage_filtered.info"
    PERSISTENT_SPEED_SENSOR="${SCRIPT_DIR}/../../build/coverage/speed_sensor/coverage_filtered.info"
    
    # Use persistent locations if they exist, otherwise try original locations
    if [[ -f "${PERSISTENT_MOTOR_SERVO}" ]]; then
        MOTOR_SERVO_COVERAGE="${PERSISTENT_MOTOR_SERVO}"
    fi
    
    if [[ -f "${PERSISTENT_SPEED_SENSOR}" ]]; then
        SPEED_SENSOR_COVERAGE="${PERSISTENT_SPEED_SENSOR}"
    fi
    
    # If speed_sensor coverage is not found, check alternative names
    if [[ ! -f "${SPEED_SENSOR_COVERAGE}" ]] && [[ -f "${SPEED_SENSOR_DIR}/build/artifacts/gcov/coverage_combined.info" ]]; then
        SPEED_SENSOR_COVERAGE="${SPEED_SENSOR_DIR}/build/artifacts/gcov/coverage_combined.info"
    fi
    if [[ ! -f "${SPEED_SENSOR_COVERAGE}" ]] && [[ -f "$(dirname "${PERSISTENT_SPEED_SENSOR}")/coverage_combined.info" ]]; then
        SPEED_SENSOR_COVERAGE="$(dirname "${PERSISTENT_SPEED_SENSOR}")/coverage_combined.info"
    fi
    
    if [[ -f "${MOTOR_SERVO_COVERAGE}" && -f "${SPEED_SENSOR_COVERAGE}" ]]; then
        log_info "Motor Servo coverage found"
        log_info "Speed Sensor coverage found"
        
        COMBINED="${MASTER_COVERAGE_DIR}/coverage_combined.info"
        FILTERED="${MASTER_COVERAGE_DIR}/coverage_filtered.info"
        
        # Merge coverage files (with branch coverage support)
        lcov -a "${MOTOR_SERVO_COVERAGE}" -a "${SPEED_SENSOR_COVERAGE}" \
             -o "${COMBINED}" \
             --rc lcov_branch_coverage=1 2>/dev/null || true
        
        # Extract source files only (no vendor, test runners, mocks, etc.)
        lcov --extract "${COMBINED}" \
             "*/motor_servo/src/*" "*/speed_sensor/src/*" \
             -o "${FILTERED}" \
             --rc lcov_branch_coverage=1 2>/dev/null || \
        # Fallback: comprehensive removal
        lcov -r "${COMBINED}" \
             '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
             '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' \
             -o "${FILTERED}" \
             --rc lcov_branch_coverage=1 2>/dev/null || \
        cp "${COMBINED}" "${FILTERED}"
        
        # Generate HTML
        if command -v genhtml &> /dev/null; then
            HTML="${MASTER_COVERAGE_DIR}/html"
            genhtml -o "${HTML}" "${FILTERED}" \
                --title "DrivaPi - Unified Test Coverage" \
                --branch-coverage \
                --function-coverage \
                --rc genhtml_branch_coverage=1 \
                --demangle-cpp > /dev/null 2>&1 || true
            
            if [[ -f "${HTML}/index.html" ]]; then
                log_pass "Coverage report: ${HTML}/index.html"
            fi
        fi
        
        echo ""
        log_info "Combined Coverage Summary:"
        lcov --summary "${FILTERED}" 2>/dev/null || true
        echo ""
        
    else
        log_warn "Coverage files not available"
        [[ ! -f "${MOTOR_SERVO_COVERAGE}" ]] && log_warn "  Missing: Motor Servo coverage"
        [[ ! -f "${SPEED_SENSOR_COVERAGE}" ]] && log_warn "  Missing: Speed Sensor coverage"
    fi
fi

echo ""
log_section "Test Summary"
echo ""
log_info "Motor Servo:  $([ $MOTOR_SERVO_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
log_info "Speed Sensor: $([ $SPEED_SENSOR_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
echo ""

if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    echo -e "${BOLD}${GREEN}============================================================================${NC}"
    echo -e "${BOLD}${GREEN} ✓ ALL TESTS PASSED - ISO 26262 COMPLIANT${NC}"
    
    # Determine which coverage report to show
    if [[ -f "${MASTER_COVERAGE_DIR}/html/index.html" ]]; then
        # Aggregated coverage exists
        echo -e "${BOLD}${GREEN} Coverage (Aggregated): ${MASTER_COVERAGE_DIR}/html/index.html${NC}"
    elif [[ -f "${MASTER_COVERAGE_DIR}/index.html" ]]; then
        # CMake-generated index exists
        echo -e "${BOLD}${GREEN} Coverage Index: ${MASTER_COVERAGE_DIR}/index.html${NC}"
    else
        # Fall back to individual reports
        echo -e "${BOLD}${GREEN} Coverage Reports:${NC}"
        echo -e "${BOLD}${GREEN}   Motor Servo:   ${MOTOR_SERVO_DIR}/build/artifacts/gcov/html/index.html${NC}"
        echo -e "${BOLD}${GREEN}   Speed Sensor:  ${SPEED_SENSOR_DIR}/coverage_report/index.html${NC}"
    fi
    
    echo -e "${BOLD}${GREEN}============================================================================${NC}"
    exit 0
else
    echo -e "${BOLD}${RED}============================================================================${NC}"
    echo -e "${BOLD}${RED} ✗ SOME TESTS FAILED${NC}"
    echo -e "${BOLD}${RED}============================================================================${NC}"
    exit 1
fi
