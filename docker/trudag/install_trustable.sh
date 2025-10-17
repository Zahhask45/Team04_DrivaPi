#!/usr/bin/env bash
set -euo pipefail

# Clone trustable source and attempt to install a tested tag to avoid
# doorstop/trudag incompatibilities seen with latest main branch.
rm -rf /tmp/trustable

git clone https://gitlab.com/CodethinkLabs/trustable/trustable.git /tmp/trustable
cd /tmp/trustable

# If a TRUSTABLE_REF value is supplied, try to checkout that tag/branch/ref.
if [[ -n "${TRUSTABLE_REF:-}" ]]; then
	echo "Attempting to install trustable from ref: ${TRUSTABLE_REF}"
	if git rev-parse --verify --quiet "${TRUSTABLE_REF}" >/dev/null; then
		git checkout "${TRUSTABLE_REF}"
	else
		# try remote refs (tags/branches)
		if git ls-remote --exit-code --refs origin "refs/tags/${TRUSTABLE_REF}" >/dev/null 2>&1; then
			git fetch --tags
			git checkout "tags/${TRUSTABLE_REF}" || true
		else
			echo "Ref ${TRUSTABLE_REF} not found; continuing with default branch"
		fi
	fi
else
	# default safe tag if none provided
	if git rev-parse --verify --quiet refs/tags/2025.9.16 >/dev/null; then
		git checkout 2025.9.16
		echo "Checked out trustable@2025.9.16"
	else
		echo "No TRUSTABLE_REF supplied and default tag not found; installing default branch"
	fi
fi

# Install trustable from the checked-out source
pip install --no-cache-dir .

# Provide a small diagnostic of installed versions to help triage if errors occur
python3 -c "import pkgutil,sys; print('installed trustable:', pkgutil.find_loader('trustable') is not None); import importlib; print('trudag version:', getattr(importlib.import_module('trudag'), '__version__', 'unknown') if pkgutil.find_loader('trudag') else 'not-installed')" || true
