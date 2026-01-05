#!/bin/bash

# Stop the script immediately if any command fails (returns non-zero)
set -e

echo "----------------------------------------------------------------"
echo "🚀 STARTING SPEED SENSOR TEST SUITE"
echo "----------------------------------------------------------------"

# 1. Clean previous builds to ensure no stale coverage data
echo ""
echo "🧹 Cleaning up..."
ceedling clobber

# 2. Run the tests with coverage enabled
# If tests fail, the script stops here due to 'set -e'
echo ""
echo "🧪 Running Tests..."
ceedling gcov:all

# 3. Capture the raw coverage data
# We search the entire 'build' folder to find the hidden .gcda files
echo ""
echo "📸 Capturing Coverage Data..."
lcov --capture --directory build --output-file coverage.info --rc branch_coverage=1 --quiet

# 3b. Filter coverage data to remove system and vendor code
echo "📋 Filtering coverage data..."
lcov -r coverage.info '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
         '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' \
         --ignore-errors unused \
         -o coverage_filtered.info --rc branch_coverage=1 --quiet || \
  cp coverage.info coverage_filtered.info

# 4. Generate the HTML Report
echo ""
echo "📊 Generating HTML Report..."
genhtml coverage.info --output-directory coverage_report --branch-coverage --quiet

echo ""
echo "----------------------------------------------------------------"
echo "✅ SUCCESS!"
echo "----------------------------------------------------------------"
echo "Report available at: $PWD/coverage_report/index.html"

# 5. Copy coverage data BEFORE cleanup to standard location for aggregation
echo ""
echo "📋 Saving coverage data for aggregation..."
# Copy to parent directory's artifacts to survive the clobber
# Use absolute path to ensure it goes to the right place
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
PERSISTENT_COVERAGE_DIR="${PROJECT_ROOT}/build/coverage/speed_sensor"
mkdir -p "$PERSISTENT_COVERAGE_DIR"
if [[ -f coverage.info ]]; then
    cp coverage.info "$PERSISTENT_COVERAGE_DIR/coverage_combined.info" || true
fi
if [[ -f coverage_filtered.info ]]; then
    cp coverage_filtered.info "$PERSISTENT_COVERAGE_DIR/coverage_filtered.info" || true
fi
# Also try to save to build/artifacts for backward compatibility if it exists
if [[ -d build/artifacts/gcov ]]; then
    if [[ -f coverage.info ]]; then
        cp coverage.info build/artifacts/gcov/coverage_combined.info || true
    fi
    if [[ -f coverage_filtered.info ]]; then
        cp coverage_filtered.info build/artifacts/gcov/coverage_filtered.info || true
    fi
fi

# 5b. Do NOT auto-open the report; just show the path for manual access

# 6. Cleanup (but coverage data is already saved)
echo ""
echo "🧹 Cleaning up..."
echo "Removing local coverage files..."
rm -f coverage.info coverage_filtered.info || true
echo "Running clobber.."
ceedling clobber || true

echo ""
echo "Cleanup done!"
echo "Exiting speed sensor test..."
