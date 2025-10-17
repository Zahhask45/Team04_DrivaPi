#!/usr/bin/env python3
"""Create a simple requirements artifact tarball for audit.

Packs the `reqs/` directory and the `.dotstop.dot` file into
`artifacts/DrivaPi-requirements-<tag>.tar.gz`.

Usage: python3 scripts/export_artifact.py --tag BASELINE-SWD-V1.0
"""
from __future__ import annotations
import argparse
from pathlib import Path
import tarfile
import datetime

ROOT = Path(__file__).resolve().parents[1]
ART = ROOT / "artifacts"
ART.mkdir(exist_ok=True)

def make_artifact(tag: str) -> Path:
    timestamp = datetime.datetime.utcnow().strftime("%Y%m%dT%H%M%SZ")
    name = f"DrivaPi-requirements-{tag}-{timestamp}.tar.gz"
    out = ART / name
    with tarfile.open(out, "w:gz") as tf:
        # include reqs/
        reqs = ROOT / "reqs"
        if reqs.exists():
            tf.add(reqs, arcname="reqs")
        dot = ROOT / ".dotstop.dot"
        if dot.exists():
            tf.add(dot, arcname=".dotstop.dot")
    return out

def main():
    p = argparse.ArgumentParser()
    p.add_argument("--tag", required=True, help="Baseline tag name to include in artifact filename")
    args = p.parse_args()
    out = make_artifact(args.tag)
    print(f"Wrote artifact: {out}")

if __name__ == '__main__':
    main()
