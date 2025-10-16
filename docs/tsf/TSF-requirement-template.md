Requirement template (YAML)

Use this template when creating new requirements. Save as `reqs/<category>/<ID>.yml` where `<category>` is one of `urd`, `srd`, `swd`, `lltc`.

```yaml
ID-001:
  header: "Short title"
  text: |
    Detailed description of the requirement.
  level: 1.0
  active: true
  derived: false
  normative: true
  links: []
  Requirement template (YAML)

  Use this template when creating new requirements. Save as `reqs/<category>/<ID>.yml` where `<category>` is one of `urd`, `srd`, `swd`, `lltc`.

  Template example

  ```yaml
  ID-001:
    header: "Short title"
    text: |
      Detailed description of the requirement.
    level: 1.0
    active: true
    derived: false
    normative: true
    links: []
    ref: "ID-001"
    reviewers:
      - name: "Reviewer Name"
        email: "reviewer@example.com"
    reviewed: ''
  ```

  Field explanations (line-by-line)

  - Top-level key (ID)
    - Example: `SWD-001:`
    - Purpose: unique identifier for the requirement; must match `ref` below.
    - Rule: prefix by category (URD/SRD/SWD/LLTC) and use consistent numbering.

  - `ref`
    - Type: string
    - Example: `ref: "ID-001"`
    - Purpose: canonical reference for the requirement. Tools expect this to match the top-level key.
    - Validation: must exactly equal the top-level mapping key.

  - `header`
    - Type: short string
    - Example: `header: "Short title"`
    - Purpose: one-line title shown in lists and UIs.
    - Tip: keep it concise and descriptive.

  - `text`
    - Type: multi-line block string (`|`)
    - Example:
      ```
      text: |
        The system shall ...
      ```
    - Purpose: full, testable description and acceptance criteria.
    - Tip: use active voice and "shall" for normative requirements.

  - `level`
    - Type: number or string (project-specific)
    - Example: `level: 1.0`
    - Purpose: indicates severity/importance/ASIL mapping if your team uses it.
    - Tip: define a project-level mapping and document it.

  - `active`
    - Type: boolean
    - Example: `active: true`
    - Purpose: whether the requirement is currently in effect.

  - `derived`
    - Type: boolean
    - Example: `derived: false`
    - Purpose: true if derived from other requirements; if true, include `links` to parents.

  - `normative`
    - Type: boolean
    - Example: `normative: true`
    - Purpose: marks whether the requirement is mandatory.

  - `links`
    - Type: list of strings
    - Example: `links: ["SRD-001", "URD-002"]`
    - Purpose: connect to upstream/downstream requirements or artifacts.
    - Tip: keep links to other req refs rather than free text when possible.

  - `reviewers`
    - Type: list of objects with `name` and `email`
    - Example:
      ```yaml
      reviewers:
        - name: "Alice Example"
          email: "alice@example.com"
      ```
    - Purpose: people responsible for review/approval.
    - Validation: must be present (our linter flags missing reviewers).

  - `reviewed`
    - Type: string (blank until approval)
    - Example: `reviewed: ''` (before approval) or `reviewed: "a1b2c3d4"` (after approval)
    - Purpose: provenance field set at approval (typically a git SHA or tag).
    - Workflow: `trudag --door manage set-item <path>` will populate this field during approval.

  Mandatory fields checklist

  - `ref` — must match top-level key
  - `header` — non-empty one-line title
  - `text` — non-empty, testable description
  - `reviewers` — at least one entry
  - `reviewed` — empty string until approved

  Notes and recommended extras

  - `assumptions:` — list or text documenting project assumptions
  - `acceptance:` — list of acceptance criteria or test-case refs
  - `owner:` — responsible team or person
  - Timestamps (`created`, `modified`) may be useful but are optional

  Validation and linting

  - Run the project linter:

    python3 scripts/reqs_lint.py reqs --verbose

  - Linter checks we recommend adding later:
    - `ref` matches filename and top-level key
    - `links` refer to existing requirement IDs
    - reviewer's email format validation

  How to cite sources

  This template and the field guidance follow the Trustable methodology and common automotive guidance. See `docs/tsf/SOURCES.md` for the primary references (Trustable docs, ISO 26262, ASPICE).
