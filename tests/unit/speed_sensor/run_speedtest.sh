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

# 4. Generate the HTML Report
echo ""
echo "📊 Generating HTML Report..."
genhtml coverage.info --output-directory coverage_report --branch-coverage --quiet

echo ""
echo "----------------------------------------------------------------"
echo "✅ SUCCESS! Opening Report..."
echo "----------------------------------------------------------------"

# 5. Open the report in the default browser
xdg-open coverage_report/index.html


# 6. Cleanup
echo ""
echo "🧹 Cleaning up..."
echo "Removing coverage.info..."
rm coverage.info
echo "Running clobber.."
ceedling clobber

echo ""
echo "Cleanup done!"
echo "Exiting speed sensor test..."
