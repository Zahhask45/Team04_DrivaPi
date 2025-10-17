#!/usr/bin/env python3
"""Generate a CSV of requirement files and which required fields are missing.

Writes: reports/reqs_missing_fields.csv
"""
import csv
from pathlib import Path
import yaml

REQ_DIR = Path('reqs')
OUT_DIR = Path('reports')
OUT_DIR.mkdir(exist_ok=True)

required_fields = ['text', 'ASIL', 'Verification Method', 'reviewers']

def read_req(fp: Path):
    try:
        with open(fp, 'r') as f:
            data = yaml.safe_load(f) or {}
            # Doorstop format often nests under the ID key; try to extract
            if isinstance(data, dict) and fp.stem in data:
                return data.get(fp.stem, {})
            # Otherwise, return first mapping value if present
            if isinstance(data, dict) and len(data) == 1:
                return list(data.values())[0]
            return data
    except Exception:
        return {}

files = sorted([p for p in REQ_DIR.rglob('*.yml') if p.name != '.doorstop.yml' and 'templates' not in p.parts] +
               [p for p in REQ_DIR.rglob('*.yaml') if p.name != '.doorstop.yml' and 'templates' not in p.parts])

rows = []
for fp in files:
    req = read_req(fp)
    missing = [f for f in required_fields if not req.get(f)]
    rows.append((str(fp), ';'.join(missing)))

outp = OUT_DIR / 'reqs_missing_fields.csv'
with open(outp, 'w', newline='') as csvf:
    w = csv.writer(csvf)
    w.writerow(['file', 'missing_fields'])
    for r in rows:
        w.writerow(r)

print(f"Wrote {outp} with {len(rows)} entries. Files missing fields: {sum(1 for _,m in rows if m)}")
