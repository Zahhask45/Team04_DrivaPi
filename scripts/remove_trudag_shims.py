#!/usr/bin/env python3
"""Remove the shims applied by apply_trudag_shims.py.

This script will:
- remove the created `distutils` shim directory inside `.venv` site-packages
- optionally reinstall Trustable/Doorstop from a specified source to restore original package files

Usage:
  python3 scripts/remove_trudag_shims.py [--reinstall-from <pip|git|none>] [--git-url <url>]

Examples:
  # Remove shims only
  python3 scripts/remove_trudag_shims.py

  # Remove shims and reinstall trustable from GitLab
  python3 scripts/remove_trudag_shims.py --reinstall-from git --git-url https://gitlab.com/CodethinkLabs/trustable/trustable.git

Note: Reinstalling from pip or git will modify files in the venv; run in a controlled environment.
"""
from __future__ import annotations
import argparse
import shutil
import subprocess
from pathlib import Path
import sys


def find_venv_site_packages(root: Path) -> Path | None:
    venv = root / '.venv'
    if not venv.exists():
        return None
    for py in venv.glob('lib/python*'):
        site = py / 'site-packages'
        if site.exists():
            return site
    return None


def remove_distutils(site: Path):
    dist_path = site / 'distutils'
    if dist_path.exists() and dist_path.is_dir():
        shutil.rmtree(dist_path)
        print(f"Removed {dist_path}")
    else:
        print(f"No distutils shim found at {dist_path}")


def reinstall_packages(site: Path, source: str, git_url: str | None):
    venv_python = Path(site).parents[1] / 'bin' / 'python'
    if not venv_python.exists():
        venv_python = Path(sys.executable)
    if source == 'pip':
        cmd = [str(venv_python), '-m', 'pip', 'install', '--upgrade', '--force-reinstall', 'trustable', 'doorstop']
    elif source == 'git':
        url = git_url or 'https://gitlab.com/CodethinkLabs/trustable/trustable.git'
        cmd = [str(venv_python), '-m', 'pip', 'install', '--upgrade', '--force-reinstall', f'git+{url}', 'doorstop']
    else:
        print('Unknown reinstall source; skipping')
        return

    print('Running:', ' '.join(cmd))
    subprocess.check_call(cmd)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--reinstall-from', choices=['pip', 'git', 'none'], default='none')
    parser.add_argument('--git-url', help='Git URL for reinstall when --reinstall-from git')
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    site = find_venv_site_packages(root)
    if site is None:
        print('No .venv site-packages found. Nothing to remove.')
        return 2

    remove_distutils(site)

    if args.reinstall_from != 'none':
        reinstall_packages(site, args.reinstall_from, args.git_url)

    print('Done')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
