#!/usr/bin/env python3
"""Generate forward/backward traceability matrices and a simple gap report.

Usage: python3 scripts/generate_bidirectional_matrix.py <reqs_dir> --output <out_dir> [--patterns-file traceability_patterns.json]

The script emits:
- traceability-matrix-forward.csv
- traceability-matrix-backward.csv
- traceability-manifest.json
- gap-report.csv
"""
from pathlib import Path
import sys
import re
import csv
import json
from datetime import datetime


def parse_req_file(path: Path):
    text = path.read_text(encoding='utf-8')
    # find top-level id
    m = re.match(r'^([A-Za-z0-9_\-]+):\s*$', text.strip().splitlines()[0])
    req_id = m.group(1) if m else path.stem
    # find links: block
    links = []
    # inline links: links: [A,B]
    for lm in re.finditer(r'links\s*:\s*\[([^\]]+)\]', text):
        inner = lm.group(1)
        for token in re.split(r'[\s,]+', inner.strip()):
            token = token.strip().strip('"\'')
            if token:
                links.append(token)
    # block links: lines after 'links:' with '- ID'
    lines = text.splitlines()
    for i, ln in enumerate(lines):
        if ln.strip().startswith('links:'):
            for following in lines[i+1:]:
                if not following.strip():
                    continue
                if following and not following.startswith((' ', '\t', '-')):
                    break
                m2 = re.match(r'^\s*-\s*["\']?([A-Za-z0-9_\-/.]+)["\']?\s*$', following)
                if m2:
                    links.append(m2.group(1))

    return req_id, links


def detect_type(target: str, patterns: list[str]):
    low = target.lower()
    for p in patterns:
        if p.lower() in low:
            return 'artifact'
    # fallback: if looks like an ID pattern (ABC-123) treat as ref
    if re.match(r'^[A-Za-z0-9_\-]+$', target):
        return 'ref'
    return 'artifact'


def main(argv):
    if len(argv) < 3:
        print('Usage: generate_bidirectional_matrix.py <reqs_dir> --output <out_dir> [--patterns-file <path>]')
        return 2
    reqs_dir = Path(argv[1])
    if '--output' in argv:
        out_dir = Path(argv[argv.index('--output') + 1])
    else:
        out_dir = Path('artifacts/traceability')
    patterns_file = None
    if '--patterns-file' in argv:
        patterns_file = Path(argv[argv.index('--patterns-file') + 1])

    out_dir.mkdir(parents=True, exist_ok=True)

    patterns = ['artifacts/verification', 'test', 'junit', 'report', 'coverage', '.xml', '.json']
    if patterns_file and patterns_file.exists():
        try:
            cfg = json.loads(patterns_file.read_text(encoding='utf-8'))
            if isinstance(cfg, dict) and 'evidence_patterns' in cfg:
                patterns = cfg['evidence_patterns']
            elif isinstance(cfg, list):
                patterns = cfg
        except Exception:
            print('Warning: failed to parse patterns file; using defaults')

    files = [p for p in reqs_dir.rglob('*.yml') if p.name != '.doorstop.yml'] + [p for p in reqs_dir.rglob('*.yaml') if p.name != '.doorstop.yml']

    forward_rows = []
    req_ids = set()

    for f in sorted(files):
        rid, links = parse_req_file(f)
        req_ids.add(rid)
        if links:
            for t in links:
                ltype = detect_type(t, patterns)
                forward_rows.append({'req_id': rid, 'target_id': t, 'link_type': ltype, 'source_file': str(f)})
        else:
            # include a row with no target to make presence explicit
            forward_rows.append({'req_id': rid, 'target_id': '', 'link_type': '', 'source_file': str(f)})

    # write forward CSV
    fwd_path = out_dir / 'traceability-matrix-forward.csv'
    with fwd_path.open('w', newline='', encoding='utf-8') as fh:
        w = csv.DictWriter(fh, fieldnames=['req_id', 'target_id', 'link_type', 'source_file'])
        w.writeheader()
        for r in forward_rows:
            w.writerow(r)

    # write backward CSV (aggregate)
    bwd_path = out_dir / 'traceability-matrix-backward.csv'
    with bwd_path.open('w', newline='', encoding='utf-8') as fh:
        w = csv.DictWriter(fh, fieldnames=['target_id', 'req_id', 'link_type', 'source_file'])
        w.writeheader()
        for r in forward_rows:
            w.writerow({'target_id': r['target_id'], 'req_id': r['req_id'], 'link_type': r['link_type'], 'source_file': r['source_file']})

    # manifest
    manifest = {
        'generated_at': datetime.utcnow().isoformat() + 'Z',
        'paths': {
            'forward': str(fwd_path),
            'backward': str(bwd_path),
        },
        'counts': {
            'files_scanned': len(files),
            'links_extracted': sum(1 for r in forward_rows if r['target_id']),
        }
    }
    manifest_path = out_dir.parent / 'traceability-manifest.json'
    manifest_path.write_text(json.dumps(manifest, indent=2), encoding='utf-8')

    # gap report: requirements with no artifact/test links (only empty or only ref links)
    gap_rows = []
    for rid in sorted({r['req_id'] for r in forward_rows}):
        rows = [r for r in forward_rows if r['req_id'] == rid and r['target_id']]
        # consider covered if at least one artifact link or target in patterns
        covered = False
        missing = []
        for r in rows:
            t = r['target_id']
            if r['link_type'] == 'artifact':
                covered = True
            else:
                # if ref to another req, that may be acceptable; we flag non-coverage if no artifact link
                pass
        if not covered:
            # find file path and category
            src = next((r['source_file'] for r in forward_rows if r['req_id'] == rid), '')
            category = Path(src).parts[1] if src else ''
            gap_rows.append({'req_id': rid, 'category': category, 'missing_links': 'artifact'})

    gap_path = out_dir / 'gap-report.csv'
    with gap_path.open('w', newline='', encoding='utf-8') as fh:
        w = csv.DictWriter(fh, fieldnames=['req_id', 'category', 'missing_links'])
        w.writeheader()
        for r in gap_rows:
            w.writerow(r)

    print(f'Forward matrix: {fwd_path}\nBackward matrix: {bwd_path}\nManifest: {manifest_path}\nGap report: {gap_path}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
