# dotstop_extensions/validators.py
from __future__ import annotations
from typing import TypeAlias
from typing import Any, Dict, List, Tuple, Union
import xml.etree.ElementTree as ET
import os

# yaml alias exactly as documented
yaml: TypeAlias = str | int | float | bool | list["yaml"] | dict[str, "yaml"]

# exact signature / return annotations expected by Trudag
def junit_pass_rate_validator(configuration: dict[str, yaml]) -> tuple[float, list[Exception | Warning]]:
    # --- your existing implementation adapted to refer to `configuration` typing ---
    try:
        cfg = configuration or {}
        refs = cfg.get("references") or []
        path = None
        if refs:
            path = refs[0].get("path")
        path = path or cfg.get("path")
        if not path:
            return (0.0, [ValueError("No path provided in validator configuration (references or path).")])
        if not os.path.exists(path):
            return (0.0, [FileNotFoundError(path)])
        tree = ET.parse(path)
        root = tree.getroot()
        tests = int(root.attrib.get("tests", root.findtext("./tests") or 0))
        failures = int(root.attrib.get("failures", root.findtext("./failures") or 0))
        errors = int(root.attrib.get("errors", root.findtext("./errors") or 0))
        skipped = int(root.attrib.get("skipped", root.findtext("./skipped") or 0))
        if root.tag.lower().endswith("testsuites") and tests == 0:
            total_tests = total_failures = total_errors = total_skipped = 0
            for ts in root.findall("testsuite"):
                total_tests += int(ts.attrib.get("tests", 0))
                total_failures += int(ts.attrib.get("failures", 0))
                total_errors += int(ts.attrib.get("errors", 0))
                total_skipped += int(ts.attrib.get("skipped", 0))
            if total_tests > 0:
                tests = total_tests
                failures = total_failures
                errors = total_errors
                skipped = total_skipped
        passed = max(0, tests - failures - errors - skipped)
        pass_rate = 100.0 * (passed / tests) if tests > 0 else 0.0
        min_pass = cfg.get("min_pass_rate") or cfg.get("min_pass") or 100
        min_pass = float(min_pass)
        if min_pass <= 0:
            score = 0.0
        else:
            score = 1.0 if pass_rate >= min_pass else (pass_rate / min_pass)
        return (min(max(score, 0.0), 1.0), [])
    except Exception as e:
        return (0.0, [e])


def cppcheck_error_validator(configuration: dict[str, yaml]) -> tuple[float, list[Exception | Warning]]:
    try:
        cfg = configuration or {}
        refs = cfg.get("references") or []
        path = None
        if refs:
            path = refs[0].get("path")
        path = path or cfg.get("path")
        if not path:
            return (0.0, [ValueError("No cppcheck xml path provided")])
        if not os.path.exists(path):
            return (0.0, [FileNotFoundError(path)])
        tree = ET.parse(path)
        root = tree.getroot()
        errors_element = root.find("errors")
        if errors_element is not None:
            error_count = len(errors_element.findall("error"))
        else:
            error_count = len(root.findall(".//error"))
        allowed = 0
        score = 1.0 if error_count <= allowed else 0.0
        return (score, [])
    except Exception as e:
        return (0.0, [e])


def coverage_threshold_validator(configuration: Dict) -> Tuple[float, List[Exception]]:
    """Read coverage XML (line-rate or percent) from common formats and compare to min_line_rate."""
    try:
        cfg = configuration or {}

        refs = cfg.get("references") or []
        path = None
        if refs:
            path = refs[0].get("path")
        path = path or cfg.get("path")

        if not path:
            return (0.0, [ValueError("No coverage path provided")])
        if not os.path.exists(path):
            return (0.0, [FileNotFoundError(path)])

        tree = ET.parse(path)
        root = tree.getroot()

        coverage = None

        # helper to coerce string values to percent float (handles "0.87" -> 87.0 and "87" -> 87.0)
        def _to_percent(s: str) -> float | None:
            try:
                v = float(s)
            except Exception:
                return None
            # if value looks like a fraction 0.xx, scale to percent
            if 0.0 < v <= 1.0:
                return v * 100.0
            return v

        # 1) check common attributes on root
        for attr in ("line-rate", "lineRate", "percent", "value"):
            val = root.attrib.get(attr)
            if val:
                coverage = _to_percent(val)
                if coverage is not None:
                    break

        # 2) check a common child element: <metrics> (many reports put lineRate here)
        if coverage is None:
            metrics = root.find(".//metrics")
            if metrics is not None:
                for attr in ("line-rate", "lineRate", "percent", "value"):
                    val = metrics.attrib.get(attr)
                    if val:
                        coverage = _to_percent(val)
                        if coverage is not None:
                            break

        # 3) check summary/statistics coverage entries (e.g. <statistics><coverage type="line" value="87%"/>)
        if coverage is None:
            # look for elements named 'coverage' with a 'value' attribute
            for cov_el in root.findall(".//coverage"):
                # value could be "87%" or "87"
                val = cov_el.attrib.get("value") or cov_el.attrib.get("percent")
                if val:
                    # strip trailing % if present
                    val_str = val.strip().rstrip("%")
                    coverage = _to_percent(val_str)
                    if coverage is not None:
                        break

        if coverage is None:
            return (0.0, [ValueError("Couldn't find coverage value in recognized attributes/elements (root.metrics.coverage) in coverage file")])

        min_cov = float(cfg.get("min_line_rate", 80))
        if min_cov <= 0:
            return (0.0, [ValueError("Invalid min_line_rate in configuration")])

        score = 1.0 if coverage >= min_cov else (coverage / min_cov)
        return (min(max(score, 0.0), 1.0), [])

    except Exception as e:
        return (0.0, [e])


__all__ = [
    "junit_pass_rate_validator",
    "cppcheck_error_validator",
    "coverage_threshold_validator",
]

# Ensure Trudag recognizes these functions by giving them the exact signature object it expects.
# This avoids subtle differences from string/forward-ref annotations.
try:
    # Import Validator from trudag (the pipx/trudag venv will have it installed)
    from trudag.dotstop.core.validator import Validator

    # List of functions to register (the names exported/used in YAML)
    _validator_fns = [
        junit_pass_rate_validator,
        cppcheck_error_validator,
        coverage_threshold_validator,
    ]

    # Assign the exact signature object Trudag uses so inspect.signature(...) == Validator._SIGNATURE
    for _fn in _validator_fns:
        try:
            _fn.__signature__ = Validator._SIGNATURE
        except Exception:
            # Silence any error during import/time-of-install; Trudag will still import the module later.
            pass
except Exception:
    # If trudag isn't importable at package-install time, ignore — the code above will run
    # when the module is imported in the trudag runtime (which is what matters).
    pass
