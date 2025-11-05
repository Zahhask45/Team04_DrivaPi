# .dotstop_extensions/validators.py
from typing import Any, Dict, List, Tuple
import xml.etree.ElementTree as ET
import os

Yaml = Any  # simple alias

def _read_file(path: str) -> str:
    with open(path, 'r', encoding='utf-8') as f:
        return f.read()

def junit_pass_rate_validator(configuration: Dict[str, Yaml]) -> Tuple[float, List[Exception]]:
    # Expects configuration['references'][0]['path'] -> junit xml file or configuration['path']
    try:
        cfg = configuration or {}
        refs = cfg.get("references") or []
        path = None
        if refs:
            path = refs[0].get("path")
        path = path or cfg.get("path")
        if not path:
            return (0.0, [ValueError("No junit XML path provided")])
        if not os.path.exists(path):
            return (0.0, [FileNotFoundError(path)])
        tree = ET.parse(path)
        root = tree.getroot()
        # support JUnit XML: tests, failures, errors, skipped or <testsuites>
        tests = int(root.attrib.get("tests", root.findtext("./tests") or 0))
        failures = int(root.attrib.get("failures", root.findtext("./failures") or 0))
        errors = int(root.attrib.get("errors", root.findtext("./errors") or 0))
        skipped = int(root.attrib.get("skipped", root.findtext("./skipped") or 0))
        passed = max(0, tests - failures - errors - skipped)
        pass_rate = 100.0 * (passed / tests) if tests > 0 else 0.0
        min_pass = cfg.get("min_pass_rate") or cfg.get("min_pass") or cfg.get("min_pass_rate", 100)
        # normalize to 0..1 score
        score = 1.0 if pass_rate >= min_pass else (pass_rate / min_pass if min_pass > 0 else 0.0)
        return (min(score, 1.0), [])
    except Exception as e:
        return (0.0, [e])

def cppcheck_error_validator(configuration: Dict[str, Yaml]) -> Tuple[float, List[Exception]]:
    try:
        cfg = configuration or {}
        refs = cfg.get("references") or []
        path = refs[0].get("path") if refs else cfg.get("path")
        if not path:
            return (0.0, [ValueError("No cppcheck xml path provided")])
        if not os.path.exists(path):
            return (0.0, [FileNotFoundError(path)])
        # very simple parser: count <error ...> tags
        text = _read_file(path)
        errors = text.count("<error ")
        max_errors = cfg.get("fail_on_severity")  # caller might use list of severities; adapt as needed
        # We assume fail_on_severity=['error'] means zero errors allowed.
        allowed = 0
        score = 1.0 if errors <= allowed else 0.0
        return (score, [])
    except Exception as e:
        return (0.0, [e])

def coverage_threshold_validator(configuration: Dict[str, Yaml]) -> Tuple[float, List[Exception]]:
    try:
        cfg = configuration or {}
        refs = cfg.get("references") or []
        path = refs[0].get("path") if refs else cfg.get("path")
        if not path:
            return (0.0, [ValueError("No coverage path provided")])
        if not os.path.exists(path):
            return (0.0, [FileNotFoundError(path)])
        text = _read_file(path)
        # assume a small coverage xml with lines like: line-rate="0.87" or <coverage line-rate="0.87">
        import re
        m = re.search(r'line-rate="([0-9.]+)"', text)
        if not m:
            return (0.0, [ValueError("Couldn't find line-rate in coverage file")])
        coverage = float(m.group(1)) * 100.0
        min_cov = float(cfg.get("min_line_rate", 80))
        score = 1.0 if coverage >= min_cov else coverage / min_cov
        return (min(score, 1.0), [])
    except Exception as e:
        return (0.0, [e])
