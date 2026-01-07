#!/bin/bash

# Stop the script immediately if any command fails (returns non-zero)
set -e
set -o pipefail

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

# 4. Generate the HTML Report (with error tolerance)
echo ""
echo "📊 Generating HTML Report..."
if ! genhtml coverage.info --output-directory coverage_report --branch-coverage --quiet 2>/dev/null; then
  # If genhtml fails (e.g., in headless CI), try without quiet or just warn
  mkdir -p coverage_report
  echo "⚠️ genhtml had issues, but coverage files are saved"
fi

echo ""
echo "----------------------------------------------------------------"
echo "✅ SUCCESS! Report available at: $PWD/coverage_report/index.html"
echo "----------------------------------------------------------------"
echo ""

# Note: Not opening browser automatically (headless CI environment)


# 6. Cleanup
echo ""
echo "🧹 Cleaning up..."
echo "Removing coverage.info..."
rm -f coverage.info || true
echo "Running clobber.."
ceedling clobber || true

echo ""
echo "✓ Cleanup done!"
echo "✓ Speed Sensor tests PASSED"
